/*
 * Copyright (C) 2026 by Andreas Theofilu <andreas@theosys.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */
#include <QPainter>

#include "tdrawobject.h"
#include "tdrawimage.h"
//#include "tdrawtext.h"
#include "terror.h"

using namespace ObjHandler;

QList<QString> grTypes = {
    "sweep",                // 00: Sweep
    "radial",               // 01: Radial
    "linearCLCR",           // 02: Left to right
    "linearTLBR",           // 03: Top-left to Bottom-right
    "linearCTCB",           // 04: Top to bottom
    "linearTRBL",           // 05: Top-right to Bottom-left
    "linearCRCL",           // 06: Right to left
    "linearBRTL",           // 07: Bottom-right to top-left
    "linearCBCT",           // 08: Bottom to top
    "linearBLTR"            // 09: Bottom-left to top-right
};

QList<QString> altGrTypes = {

};
/********************************************************************************/

TDrawObject::TDrawObject(TObjectHandler *object, QWidget *widget)
    : mWidget(widget),
      mObject(object)
{
    DECL_TRACER("TDrawObject::TDrawObject(QWidget *widget)");
}

/**
 * @brief TDrawObject::draw
 * The method draws an object completely. The result is an array of QPixmaps
 * stored in the array @code mPixButtons. Each entry is a representation of
 * a state. Internally this states are called @bold instances.
 *
 * All parameters the method needs should have been set by calling the
 * constructor of the class.
 */
void TDrawObject::draw()
{
    DECL_TRACER("TDrawObject::draw()");

    if (!mObject || !mWidget)
    {
        MSG_ERROR("Can't draw an object because missing the obejct or widget!");
        return;
    }

    ObjHandler::TOBJECT_t object = mObject->getObject();

    for (int instance = 0; instance < object.sr.size(); ++instance)
    {
        getDrawOrder(object.sr[instance]._do, mDOrder);
        QPixmap button(object.wt, object.ht);               // Creates an empty image
        button.fill(Qt::transparent);                       // Fills the image and prepares it for QPainter

        for (int _order = 0; _order < ORD_ELEM_COUNT; ++_order)
        {
            DRAW_ORDER_t order = static_cast<DRAW_ORDER_t>(_order);

            switch(order)
            {
                case ORD_ELEM_FILL:
                    buttonFill(&button, object.sr[instance]);
                break;

                case ORD_ELEM_BITMAP:
                {
                    TDrawImage di;
                    di.setPixmap(&button);
                    di.setSize(object.wt, object.ht);
                    di.setBitmaps(object.sr[instance].bitmaps);
                    di.setOpacity(object.sr[instance].oo);

                    if (object.sr[instance].bs.isEmpty())
                    {
                        di.setChameleon(object.sr[instance].mi);
                        di.setChameleonColors(object.sr[instance].cf, object.sr[instance].cb);
                    }

                    di.draw();
                }
                break;
            }
        }
    }
}

/**
 * @brief TDrawObject::getDrawOrder - Find the order of the elements
 * The order of the elements to draw defines in which order the parts of a
 * button should be drawn. If there was no draw order found in configuration
 * file, the default draw order is used.
 *
 * The draw order must consist of a string containing numbers. Each number must
 * be 2 digits and if it is less than 10 it must have a leading 0. There must
 * not be more then 5 numbers (10 digits) in the string.
 *
 * The method uses the default order if there is no order (@code sdo has length
 * zero) or if the number of digits is odd.
 *
 * If there are more then 5 draw orders a warning is logged and only the first
 * 5 are honored. The rest will be ignored.
 *
 * If there is a wrong character detected in @code sdo (anything else than a digit)
 * a warning is logged and the default draw order is returned in @code order.
 *
 * @param sdo       String containing the draw order
 * @param order     Array of 5 elements containing the draw order as integers
 */
void TDrawObject::getDrawOrder(const QString& sdo, DRAW_ORDER_t *order)
{
    DECL_TRACER("TButton::getDrawOrder(const std::string& sdo, DRAW_ORDER *order)");

    if (!order)
        return;

    if (sdo.isEmpty() || (sdo.length() % 2) != 0)
    {
        getDefaultDrawOrder(order);
        return;
    }

    int elems = (int)(sdo.length() / 2);

    if (elems > 5)
    {
        MSG_WARNING("Found " << elems << " draw orders! Only 5 are allowed.");
        elems = 5;
    }

    for (int i = 0; i < elems; i++)
    {
        int e = sdo.mid(i * 2, 2).toInt();

        if (e < 1 || e > 5)
        {
            MSG_ERROR("Invalid draw order \"" << sdo.toStdString() << "\"!");
            getDefaultDrawOrder(order);
            return;
        }

        *(order+i) = static_cast<DRAW_ORDER_t>(e);
    }
}

void TDrawObject::getDefaultDrawOrder(DRAW_ORDER_t *order)
{
    if (!order)
        return;

    *order     = ORD_ELEM_FILL;
    *(order+1) = ORD_ELEM_BITMAP;
    *(order+2) = ORD_ELEM_TEXT;
    *(order+3) = ORD_ELEM_BORDER;
    *(order+4) = ORD_ELEM_NONE;
}

bool TDrawObject::buttonFill(QPixmap* bm, SR_T sr)
{
    DECL_TRACER("TDrawObject::buttonFill(SkBitmap* bm, int instance)");

    if (!bm)
    {
        MSG_ERROR("Invalid bitmap!");
        return false;
    }

    // If there is a gradient color defined, we draw it now
    if (!sr.ft.isEmpty())
        return drawBackgroundColor(bm, sr, sr.gradientColors);

    QColor color;

    if (sr.vf.isEmpty())
        color = sr.cf;
    else
        color = sr.vf;

    MSG_DEBUG("Fill color: #" << color.name(QColor::HexArgb).toStdString() << ")");
    // We create a new bitmap and fill it with the given fill color. Then
    // we put this image over the existing image "bm". In case this method is
    // not the first in the draw order, it prevents the button from completely
    // overwrite.
    QPixmap bitmap(bm->size());
    bitmap.fill(color);                             // Fill the new bitmap with the fill color
    QPainter painter(bm);
    painter.drawPixmap(0, 0, bitmap);

    return true;
}

bool TDrawObject::drawBackgroundColor(QPixmap* bm, SR_T& sr, QList<QString>& grads)
{
    DECL_TRACER("TDrawObject::drawBackgroundColor(QPixmap* bm, SR_T& sr, QList<QString>& grads)");

    QList<QColor> gradients;
    GRAD_TYPE_t grad = getGradientType(sr.ft);

    for (QString s : grads)
        gradients.append(QColor(s));

    if (gradients.empty() || gradients.size() < 2)
    {
        MSG_WARNING("No gradient colors! Will use default colors!");
        gradients = { Qt::gray, Qt::white };
    }

    if (grad == GRAD_SOLID)
    {
        QPixmap bitmap(bm->size());
        bitmap.fill(sr.cf);                             // Fill the new bitmap with the fill color
        QPainter painter(bm);
        painter.drawPixmap(0, 0, bitmap);
        return true;
    }

    int width = bm->width();
    int height = bm->height();

    int entity = qMax(width, height);
    qreal stop = static_cast<qreal>(entity) / static_cast<qreal>(gradients.size()) / static_cast<qreal>(entity);
    QGradientStops gstops;
    qreal point = 0.0;

    if (gradients.size() == 2)
        stop = 1.0;

    for (QColor col : gradients)
    {
        gstops.append(std::pair<qreal, QColor>(point, col));
        point += stop;
    }

    gstops.last() = std::pair<qreal, QColor>(1.0, gradients.last());
    QPainter painter(bm);
    QRect rect(0, 0, width, height);

    if (grad == GRAD_RADIAL)    // Colors in circles
    {
        QRadialGradient radial;
        // Calculate center point;
        qreal rx = static_cast<qreal>(width) / 100.0 * static_cast<qreal>(sr.gx);
        qreal ry = static_cast<qreal>(height) / 100.0 * static_cast<qreal>(sr.gy);
        radial.setCenter(rx, ry);
        radial.setFocalPoint(rx, ry);
        radial.setRadius(static_cast<qreal>(sr.gr));
        radial.setStops(gstops);
        QBrush brush(radial);
        painter.fillRect(rect, brush);
        return true;
    }
    else if (grad == GRAD_SWEEP)
    {
        QConicalGradient grad;
        grad.setAngle(0.0);
        grad.setCenter(rect.center());
        grad.setStops(gstops);
        QBrush brush(grad);
        painter.fillRect(rect, brush);
        return true;
    }

    QLinearGradient linear;
    linear.setStops(gstops);

    if (grad == GRAD_CLCR)
    {
        linear.setStart(0.0, static_cast<qreal>(height / 2));
        linear.setFinalStop(static_cast<qreal>(width), static_cast<qreal>(height / 2));
    }
    else if (grad == GRAD_TLBR)
    {
        linear.setStart(0.0, 0.0);
        linear.setFinalStop(static_cast<qreal>(width), static_cast<qreal>(height));
    }
    else if (grad == GRAD_CTCB)
    {
        linear.setStart(static_cast<qreal>(width / 2), 0.0);
        linear.setFinalStop(static_cast<qreal>(width / 2), static_cast<qreal>(height));
    }
    else if (grad == GRAD_TRBL)
    {
        linear.setStart(static_cast<qreal>(width), 0.0);
        linear.setFinalStop(0.0, static_cast<qreal>(height));
    }
    else if (grad == GRAD_CRCL)
    {
        linear.setStart(static_cast<qreal>(width), static_cast<qreal>(height / 2));
        linear.setFinalStop(0.0, static_cast<qreal>(height / 2));
    }
    else if (grad == GRAD_BRTL)
    {
        linear.setStart(static_cast<qreal>(width), static_cast<qreal>(height));
        linear.setFinalStop(0.0, 0.0);
    }
    else if (grad == GRAD_CBCT)
    {
        linear.setStart(static_cast<qreal>(width / 2), static_cast<qreal>(height));
        linear.setFinalStop(static_cast<qreal>(width / 2), 0.0);
    }
    else if (grad == GRAD_BLTR)
    {
        linear.setStart(0.0, static_cast<qreal>(height));
        linear.setFinalStop(static_cast<qreal>(width), 0.0);
    }

    QBrush brush(linear);
    painter.fillRect(rect, brush);
    return true;
}

GRAD_TYPE_t TDrawObject::getGradientType(const QString& grad)
{
    DECL_TRACER("TDrawObject::getGradientType(const QString& grad)");

    int idx = 0;

    for (QString s : grTypes)
    {
        if (grad == s)
            return static_cast<GRAD_TYPE_t>(idx + 1);

        idx++;
    }

    return GRAD_SOLID;
}
