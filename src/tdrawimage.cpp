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
#include <QLabel>

#include "tdrawimage.h"
#include "tcanvaswidget.h"
#include "tconfmain.h"
#include "terror.h"

using namespace ObjHandler;

#define LABEL_NAME  "LabelPixmap"

TDrawImage::TDrawImage(const QList<BITMAPS_t>& bitmaps, TBASEOBJ_t *object)
    : mBitmaps(bitmaps),
      mObject(object)
{
    DECL_TRACER("TDrawImage::TDrawImage(const QString& file, TBASEOBJ_t *object)");
}

void TDrawImage::setOpacity(int oo)
{
    DECL_TRACER("TDrawImage::setOpacity(int oo)");

    if (oo < -1 || oo > 255)
        return;

    mOpacity = oo;
}

void TDrawImage::draw()
{
    DECL_TRACER("TDrawImage::draw()");

    if (mBitmaps.empty())
    {
        MSG_ERROR("Got no bitmaps to draw!");
        return;
    }

    if (!mObject || !mObject->widget)
    {
        MSG_ERROR("Got no widget to draw to.");
        return;
    }

    TCanvasWidget *widget = mObject->widget;
    QPixmap *pixmap = &mObject->pixmap;

    if (pixmap->isNull())
        *pixmap = QPixmap(widget->size());

    pixmap->fill(Qt::transparent);
    QString pathTemp = TConfMain::Current().getPathTemporary();
    // Loop through the bitmaps and draw them one over the other
    MSG_DEBUG("Drawing " << mBitmaps.size() << " bitmaps on a widget with size " << widget->width() << " x " << widget->height());

    for (BITMAPS_t bm : mBitmaps)
    {
        MSG_DEBUG("Drawing bitmap: " << bm.fileName.toStdString());
        QPixmap pix;
        // Load the image
        if (!pix.load(pathTemp + "/images/" + bm.fileName))
        {
            MSG_ERROR("Couldn't load image " << bm.fileName.toStdString());
            continue;
        }

        // Scale image to fit into widget, if necessary
        if (pix.width() > widget->width() || pix.height() > widget->height())
            pix = pix.scaled(widget->size(), Qt::KeepAspectRatio);

        // Find the wanted position of the pixmap
        int x, y;

        switch(bm.justification)
        {
            case ORI_ABSOLUT:
                x = bm.offsetX;
                y = bm.offsetY;
            break;

            case ORI_TOP_LEFT:
                x = 0;
                y = 0;
            break;

            case ORI_TOP_MIDDLE:
                x = (widget->width() - pix.width()) / 2;
                y = 0;
            break;

            case ORI_TOP_RIGHT:
                x = widget->width() - pix.width();
                y = 0;
            break;

            case ORI_CENTER_LEFT:
                x = 0;
                y = (widget->height() - pix.height()) / 2;
            break;

            case ORI_CENTER_MIDDLE:
                x = (widget->width() - pix.width()) / 2;
                y = (widget->height() - pix.height()) / 2;
            break;

            case ORI_CENTER_RIGHT:
                x = widget->width() - pix.width();
                y = (widget->height() - pix.height()) / 2;
            break;

            case ORI_BOTTOM_LEFT:
                x = 0;
                y = widget->height() - pix.height();
            break;

            case ORI_BOTTOM_MIDDLE:
                x = (widget->width() - pix.width()) / 2;
                y = widget->height() - pix.height();
            break;

            case ORI_BOTTOM_RIGHT:
                x = widget->width() - pix.width();
                y = widget->height() - pix.height();
            break;

            default:
                x = (widget->width() - pix.width()) / 2;
                y = (widget->height() - pix.height()) / 2;
        }

        QPainter p(pixmap);

        if (mOpacity >= 0)
            p.setOpacity(1.0 / 255.0 * static_cast<qreal>(mOpacity));         // set opacity from 0.0 to 1.0, where 0.0 is fully transparent and 1.0 is fully opaque.

        p.drawPixmap(x, y, pix);        // given pixmap into the paint device.
        p.end();
    }

    QLabel *label = nullptr;
    QObjectList objects = widget->children();

    for (QObject *obj : objects)
    {
        if (obj->objectName() == LABEL_NAME)
        {
            label = static_cast<QLabel *>(obj);
            break;
        }
    }

    if (!label)
    {
        MSG_DEBUG("Adding a new pixmap to widget ...");
        label = new QLabel(widget);
        label->setObjectName(LABEL_NAME);
        label->setGeometry(widget->rect());
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
        label->setAttribute(Qt::WA_TranslucentBackground);
    }

    label->setPixmap(*pixmap);
    label->lower();
    label->show();
}
