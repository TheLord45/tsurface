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

TDrawBgSlider *TDrawBgSlider::mCurrent = nullptr;

TDrawBgSlider::TDrawBgSlider()
{
    DECL_TRACER("TDrawBgSlider::TDrawBgSlider()");
}

TDrawBgSlider& TDrawBgSlider::Current()
{
    if (!mCurrent)
        mCurrent = new TDrawBgSlider;

    return *mCurrent;
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

    int width, height;
    bool horizontal = false;

    if (obj.dr == "horizontal")
    {
        width = (sst.fixedSize / 2) * 2 + sst.fixedSize;
        height = sst.fixedSize;
        horizontal = true;
    }
    else
    {
        width = sst.fixedSize;
        height = (sst.fixedSize / 2) * 2 + sst.fixedSize;
    }

    vector<Graphics::SLIDER_t> sltList = TGraphics::Current().getSliderFiles(slider);

    if (sltList.empty())
    {
        MSG_ERROR("No valid slider files defined!");
        return false;
    }

    *bm = QPixmap(width, height);
    bm->fill(Qt::transparent);
    QPainter paint(bm);

    vector<Graphics::SLIDER_t>::iterator iter;

    for (iter = sltList.begin(); iter != sltList.end(); ++iter)
    {
        QPixmap slPart;
        QPixmap slPartAlpha;
        QRect dst;

        if (horizontal && (iter->type == Graphics::SGR_LEFT || iter->type == Graphics::SGR_RIGHT || iter->type == Graphics::SGR_VERTICAL))
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

            QPixmap sl;

            if (!combineImages(&sl, slPart, slPartAlpha, obj.sc))
                return false;

            switch(iter->type)
            {
                case Graphics::SGR_LEFT:    dst = QRect(0, 0, sl.width(), sl.height()); break;
                case Graphics::SGR_RIGHT:   dst = QRect((sst.fixedSize / 2) + sst.fixedSize, 0, sl.width(), sl.height()); break;

                case Graphics::SGR_VERTICAL:
                    stretchImageWidth(&sl, sst.fixedSize);
                    dst = QRect(sst.fixedSize / 2, 0, sl.width(), sl.height());
                break;

                default:
                    MSG_WARNING("Invalid type " << iter->type << " found!");
            }

            paint.drawPixmap(dst, sl);
            paint.end();
        }
    }

    // TODO: Add rest of slider drawing (TButton)
    return false;
}

bool TDrawBgSlider::combineImages(QPixmap *bm, const QPixmap& base, const QPixmap& alpha, QColor col)
{
    DECL_TRACER("TDrawBgSlider::combineImages(QPixmap *bm, const QPixmap& base, const QPixmap& alpha, QColor col)");

    if (!bm || bm->isNull())
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

            if (pixelAlpha.alpha() == 0)
                imgBm.setPixelColor(ix, iy, Qt::transparent);
            else
                imgBm.setPixelColor(ix, iy, pixelAlpha);
        }
    }

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

    int rwidth = width;

    if (width <= 0)
        rwidth = bm->width() + width;

    if (rwidth <= 0)
        rwidth = 1;

    *bm = bm->scaled(rwidth, bm->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
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

    int rheight = height;

    if (height <= 0)
        rheight = bm->height() + height;

    if (rheight <= 0)
        rheight = 1;

    *bm = bm->scaled(bm->width(), rheight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    return true;
}
