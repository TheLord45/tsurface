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
#include <QPixmap>
#include <QString>
#include <QList>
#include <QPainter>

#include "tdrawbgslider.h"
#include "tgraphics.h"
#include "terror.h"

using std::vector;

TDrawBgSlider::TDrawBgSlider()
{
    DECL_TRACER("TDrawBgSlider::TDrawBgSlider()");
}

TDrawBgSlider::~TDrawBgSlider()
{
    DECL_TRACER("TDrawBgSlider::~TDrawBgSlider()");
}

bool TDrawBgSlider::drawSliderButton(QPixmap *bm, const QString& slider, const ObjHandler::TOBJECT_t& obj)
{
    DECL_TRACER("TDrawBgSlider::drawSliderButton(QPixmap *bm, const QString& slider, const ObjHandler::SR_T& sr)");

    if (!bm || bm->isNull())
    {
        MSG_ERROR("No valid pixmap defined!");
        return false;
    }

    if (slider.isEmpty() || !TGraphics::Current().existSlider(slider))
    {
        MSG_ERROR("No valid slider defined!");
        return false;
    }

    Graphics::SLIDER_STYLE_t sst;

    if (!TGraphics::Current().getSlider(slider, &sst))
    {
        MSG_ERROR("No valid slider style defined!");
        return false;
    }

    vector<Graphics::SLIDER_t> sltList = TGraphics::Current().getSliderFiles(slider);

    if (sltList.empty())
    {
        MSG_ERROR("No valid slider files defined!");
        return false;
    }

    int width = 0, height = 0;
    bool horizontal = false;

    if (obj.dr != "horizontal")
    {
        width = sst.fixedSize * 3;
        height = sst.fixedSize;
    }
    else
    {
        width = sst.fixedSize;
        height = sst.fixedSize * 3;
        horizontal = true;
    }


    QPixmap px(width, height);
    px.fill(Qt::transparent);
    QPainter paint(&px);

    vector<Graphics::SLIDER_t>::iterator iter;

    for (iter = sltList.begin(); iter != sltList.end(); ++iter)
    {
        QPixmap slPart;
        QPixmap slPartAlpha;
        QRect dst;

        if (!horizontal && (iter->type == Graphics::SGR_LEFT || iter->type == Graphics::SGR_RIGHT || iter->type == Graphics::SGR_VERTICAL))
        {
            if (!iter->path.isEmpty() && !slPart.load(iter->path))
            {
                MSG_ERROR("Missing slider button mask image " << iter->path.toStdString());
                return false;
            }

            if (!iter->pathAlpha.isEmpty() && !slPartAlpha.load(iter->pathAlpha))
            {
                MSG_ERROR("Missing slider button image " << iter->pathAlpha.toStdString());
                return false;
            }

            if (slPart.isNull() && slPartAlpha.isNull())
            {
                MSG_ERROR("No valid slider button images defined for " << slider.toStdString());
                return false;
            }

            if (slPart.isNull() && !slPartAlpha.isNull())
            {
                slPart = QPixmap(slPartAlpha.width(), slPartAlpha.height());
                slPart.fill(Qt::transparent);
            }

            if (!slPart.isNull() && slPartAlpha.isNull())
            {
                slPartAlpha = QPixmap(slPart.width(), slPart.height());
                QColor white = Qt::white;
                slPartAlpha.fill(white);
            }

            QPixmap sl;

            if (!combineImages(&sl, slPart, slPartAlpha, obj.sc))
                return false;

            switch(iter->type)
            {
                case Graphics::SGR_LEFT:
                    stretchImageWidth(&sl, sst.fixedSize);
                    dst = QRect(0, 0, sl.width(), sl.height());
                break;

                case Graphics::SGR_RIGHT:
                    stretchImageWidth(&sl, sst.fixedSize);
                    dst = QRect(sst.fixedSize * 2, 0, sl.width(), sl.height());
                break;

                case Graphics::SGR_VERTICAL:
                    stretchImageWidth(&sl, sst.fixedSize);
                    dst = QRect(sst.fixedSize, 0, sl.width(), sl.height());
                break;

                default:
                    MSG_WARNING("Invalid type " << iter->type << " found!");
            }

            paint.drawPixmap(dst, sl);
        }
        else if (horizontal && (iter->type == Graphics::SGR_TOP || iter->type == Graphics::SGR_BOTTOM || iter->type == Graphics::SGR_HORIZONTAL)) // horizontal slider
        {
            if (!iter->path.isEmpty() && !slPart.load(iter->path))
            {
                MSG_ERROR("Missing slider button image " << iter->path.toStdString());
                return false;
            }

            if (!iter->pathAlpha.isEmpty() && !slPartAlpha.load(iter->pathAlpha))
            {
                MSG_ERROR("Missing slider button alpha image " << iter->pathAlpha.toStdString());
                return false;
            }

            if (slPart.isNull() && slPartAlpha.isNull())
            {
                MSG_ERROR("No valid slider button images defined for " << slider.toStdString());
                return false;
            }

            if (slPart.isNull() && !slPartAlpha.isNull())
            {
                slPart = QPixmap(slPartAlpha.width(), slPartAlpha.height());
                slPart.fill(Qt::transparent);
            }

            if (!slPart.isNull() && slPartAlpha.isNull())
            {
                slPartAlpha = QPixmap(slPart.width(), slPart.height());
                QColor white = Qt::white;
                slPartAlpha.fill(white);
            }

            QPixmap sl;

            if (!combineImages(&sl, slPart, slPartAlpha, obj.sc))
                return false;

            switch (iter->type)
            {
                case Graphics::SGR_TOP:
                    stretchImageHeight(&sl, sst.fixedSize);
                    dst = QRect(0, 0, sl.width(), sl.height());
                break;

                case Graphics::SGR_BOTTOM:
                    stretchImageHeight(&sl, sst.fixedSize);
                    dst = QRect(0, sst.fixedSize * 2, sl.width(), sl.height());
                break;

                case Graphics::SGR_HORIZONTAL:
                    stretchImageHeight(&sl, sst.fixedSize);
                    dst = QRect(0, sst.fixedSize, sl.width(), sl.height());
                break;

                default:
                    MSG_WARNING("Invalid type " << iter->type << " found!");
            }

            paint.drawPixmap(dst, sl);
        }
    }

    paint.end();
    QPainter finalPainter(bm);

    if (!horizontal)
        finalPainter.drawPixmap(0, obj.ht / 2 - px.height() / 2, px.scaled(obj.wt, px.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    else
        finalPainter.drawPixmap(obj.wt / 2 - px.width() / 2, 0, px.scaled(px.width(), obj.ht, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    finalPainter.end();
    return true;
}

bool TDrawBgSlider::combineImages(QPixmap *bm, const QPixmap& base, const QPixmap& alpha, QColor col)
{
    DECL_TRACER("TDrawBgSlider::combineImages(QPixmap *bm, const QPixmap& base, const QPixmap& alpha, QColor col)");

    if (!bm)
    {
        MSG_ERROR("No valid pixmap defined!");
        return false;
    }

    int width = base.width();
    int height = base.height();

    if (width != alpha.width() || height != alpha.height())
    {
        MSG_ERROR("Mask and alpha have different size! [ " << width << " x " << height << " to " << alpha.width() << " x " << alpha.height() << "]");
        return false;
    }

    if (bm->isNull())
        *bm = QPixmap(width, height);

    bm->fill(Qt::transparent);
    QImage imgBm = bm->toImage();
    QImage imgBase = base.toImage();
    QImage imgAlpha = alpha.toImage();

    for (int ix = 0; ix < width; ix++)
    {
        for (int iy = 0; iy < height; iy++)
        {
            QColor pixelAlpha = imgAlpha.pixelColor(ix, iy);
            QColor color = col;

            if (pixelAlpha.alpha() == 0)
                imgBm.setPixelColor(ix, iy, Qt::transparent);
            else
            {
                color.setAlpha(pixelAlpha.alpha());
                imgBm.setPixelColor(ix, iy, color);
            }
        }
    }

    *bm = QPixmap::fromImage(imgBm);
    QPainter painter(bm);
    painter.drawImage(0, 0, imgBase);
    painter.end();
    return true;
}

bool TDrawBgSlider::stretchImageWidth(QPixmap *bm, int width)
{
    DECL_TRACER("TDrawBgSlider::stretchImageWidth(QPixmap *bm, int width)");

    if (!bm || bm->isNull())
    {
        MSG_ERROR("No valid pixmap defined!");
        return false;
    }

    if (bm->width() == width)
        return true;

    *bm = bm->scaled(width, bm->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    return true;
}

bool TDrawBgSlider::stretchImageHeight(QPixmap *bm, int height)
{
    DECL_TRACER("TDrawBgSlider::stretchImageHeight(QPixmap *bm, int height)");

    if (!bm || bm->isNull())
    {
        MSG_ERROR("No valid pixmap defined!");
        return false;
    }

    if (bm->height() == height)
        return true;

    *bm = bm->scaled(bm->width(), height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    return true;
}
