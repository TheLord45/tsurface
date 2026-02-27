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

TDrawImage::TDrawImage()
{
    DECL_TRACER("TDrawImage::TDrawImage()");
}

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

    if ((!mObject || !mObject->widget) && !mPixmap)
    {
        MSG_ERROR("Got no widget to draw to.");
        return;
    }

    mHaveWidget = mObject && mObject->widget ? true : false;

    TCanvasWidget *widget = nullptr;

    if (mHaveWidget)
    {
        widget = mObject->widget;
        mPixmap = &mObject->pixmap;

        if (mPixmap->isNull())
            *mPixmap = QPixmap(widget->size());

        mPixmap->fill(Qt::transparent);
        mWidth = widget->width();
        mHeight = widget->height();
    }
    else if (!mPixmap || mPixmap->isNull())
    {
        MSG_ERROR("Have no valid pixmap to draw to!");
        return;
    }
    else
    {
        if (!mWidth)
            mWidth = mPixmap->width();

        if (!mHeight)
            mHeight = mPixmap->height();
    }

    if (mHaveWidget)
    {
        MSG_DEBUG("Drawing " << mBitmaps.size() << " bitmaps on a widget with size " << widget->width() << " x " << widget->height());
    }
    else
    {
        MSG_DEBUG("Drawing " << mBitmaps.size() << " on pixmap with size " << mPixmap->width() << " x " << mPixmap->height());
    }

    // The chameleon images is drawn first
    if (!mHaveWidget && !mChameleon.isEmpty())
        drawChameleon();

    // If there are more than one bitmap draw them now
    // on top of chameleon image, if there is one.
    // Or draw the bitmap stack.
    drawBitmapStack();
}

void TDrawImage::drawBitmapStack()
{
    DECL_TRACER("TDrawImage::drawBitmapStack()");

    QString pathTemp = TConfMain::Current().getPathTemporary();
    // Loop through the bitmaps and draw them one over the other
    bool stepOver = false;

    // If we have a chameleon image we must skip the first image
    // because it is the mask for the chameleon.
    if (!mHaveWidget && !mChameleon.isEmpty())
        stepOver = true;

    for (BITMAPS_t bm : mBitmaps)
    {
        if (stepOver)
        {
            stepOver = false;
            continue;
        }

        MSG_DEBUG("Drawing bitmap: " << bm.fileName.toStdString());
        QPixmap pix;
        // Load the image
        if (!pix.load(pathTemp + "/images/" + bm.fileName))
        {
            MSG_ERROR("Couldn't load image " << bm.fileName.toStdString());
            continue;
        }

        // Scale image to fit into widget, if necessary
        if (bm.justification == ORI_SCALE_FIT)
            pix = pix.scaled(QSize(mWidth, mHeight), Qt::IgnoreAspectRatio);
        else if (bm.justification == ORI_SCALE_ASPECT)
            pix = pix.scaled(QSize(mWidth, mHeight), Qt::KeepAspectRatio);

        // Find the wanted position of the pixmap
        int x, y;
        bm.width = pix.width();
        bm.height = pix.height();
        getLeftUpper(&x, &y, bm);
        // Draw the bitmap
        QPainter p(mPixmap);

        if (mOpacity >= 0)
            p.setOpacity(1.0 / 255.0 * static_cast<qreal>(mOpacity));         // set opacity from 0.0 to 1.0, where 0.0 is fully transparent and 1.0 is fully opaque.

        p.drawPixmap(x, y, pix);        // given pixmap into the paint device.
        p.end();
    }

    if (mHaveWidget)
    {
        QLabel *label = nullptr;
        QObjectList objects = mObject->widget->children();

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
            label = new QLabel(mObject->widget);
            label->setObjectName(LABEL_NAME);
            label->setGeometry(mObject->widget->rect());
            label->setAttribute(Qt::WA_TransparentForMouseEvents);
            label->setAttribute(Qt::WA_TranslucentBackground);
        }

        label->setPixmap(*mPixmap);
        label->lower();
        label->show();
    }
}

/**
 * @brief TDrawImage::drawChameleon
 * The method draws a chameleon image by putting a mask under an existing image.
 *
 * A chameleon image has always a mask. This mask is an image with red and
 * green pixels, where the red pixels are colored with the fill color (cf)
 * and the green ones with the border color (cb). All other pixels are
 * left as they are.
 * If there exist a bitmap image, it is drawn over the mask. The combination
 * results in a new bitmap. This allows to change the color of an image on
 * the fly even during runtime.
 * There can be up to 5 images. But only the first image in the
 * stack is the bitmap drawn over the mask (mi).
 */
void TDrawImage::drawChameleon()
{
    DECL_TRACER("TDrawImage::drawChameleon()");

    QString pathTemp = TConfMain::Current().getPathTemporary();
    QPixmap pix;
    BITMAPS_t bm;
    // Load the image
    if (!pix.load(pathTemp + "/images/" + mChameleon))
    {
        MSG_ERROR("Couldn't load image " << mChameleon.toStdString());
        return;
    }

    QPixmap imgRed(pix);
    QPixmap imgMask;

    if (mBitmaps.size() > 0)
    {
        bm = mBitmaps[0];

        if (!imgMask.load(pathTemp + "/images/" + mBitmaps[0].fileName))
        {
            MSG_ERROR("Couldn't load mask image " << mBitmaps[0].fileName.toStdString());
            imgMask = QPixmap();
        }
    }

    bool haveBothImages = mBitmaps.empty() || imgMask.isNull() ? false : true;

    if (!haveBothImages)
    {
        bm.justification = ObjHandler::ORI_CENTER_MIDDLE;
        bm.offsetX = 0;
        bm.offsetY = 0;
    }

    bm.width = pix.width();
    bm.height = pix.height();

    QImage pixmapRed = imgRed.toImage();
    QImage pixmapMask;
    QImage maskBm(mWidth, mHeight, QImage::Format_ARGB32);
    maskBm.fill(Qt::transparent);

    if (!imgMask.isNull())
        pixmapMask = imgMask.toImage();

    for (int ix = 0; ix < mWidth; ix++)
    {
        for (int iy = 0; iy < mHeight; iy++)
        {
            QColor pixelRed;
            QColor pixelMask;

            if (ix < pixmapRed.width() && iy < pixmapRed.height())
                pixelRed = pixmapRed.pixel(ix, iy);
            else
                pixelRed = Qt::black;

            if (haveBothImages && !imgMask.isNull() &&
                    ix < pixmapMask.width() && iy < pixmapMask.height())
                pixelMask = pixmapMask.pixel(ix, iy);
            else
                pixelMask = Qt::transparent;

            QColor pixel = baseColor(pixelRed, pixelMask);
            uint alpha = pixel.alpha();

            if (alpha == 0)
                pixel = pixelMask;

            maskBm.setPixel(ix, iy, pixel.rgba());
        }
    }

    if (maskBm.isNull())
    {
        QString bm;

        if (!mBitmaps.empty())
            bm = mBitmaps[0].fileName;

        MSG_ERROR("Error creating the chameleon image \"" << mChameleon.toStdString() << "\" / \"" << bm.toStdString() << "\"!");
        return;
    }

    int x, y;
    getLeftUpper(&x, &y, bm);

    QPainter p(mPixmap);

    if (mOpacity >= 0)
        p.setOpacity(1.0 / 255.0 * static_cast<qreal>(mOpacity));         // set opacity from 0.0 to 1.0, where 0.0 is fully transparent and 1.0 is fully opaque.

    p.drawImage(x, y, maskBm);
    p.end();
}

QColor TDrawImage::baseColor(QColor basePix, QColor maskPix)
{
    uint alpha = basePix.alpha();
    uint green = basePix.green();
    uint red = basePix.red();

    if (alpha == 0)
        return maskPix;

    if (red && green)
    {
        if (red < green)
            return mColor2;

        return mColor1;
    }

    if (red)
        return mColor1;

    if (green)
        return mColor2;

    return Qt::transparent;
}

void TDrawImage::setChameleonColors(const QColor& col1, const QColor& col2)
{
    DECL_TRACER("TDrawImage::setChameleonColors(const QColor& col1, const QColor& col2)");

    mColor1 = col1;
    mColor2 = col2;
}

void TDrawImage::getLeftUpper(int *x, int *y, ObjHandler::BITMAPS_t bm)
{
    DECL_TRACER("TDrawImage::getLeftUpper(int *x, int *y, ObjHandler::BITMAPS_t bm)");

    if (!x || !y)
        return;

    switch(bm.justification)
    {
        case ORI_ABSOLUT:
            *x = bm.offsetX;
            *y = bm.offsetY;
        break;

        case ORI_TOP_LEFT:
            *x = 0;
            *y = 0;
        break;

        case ORI_TOP_MIDDLE:
            *x = (mWidth - bm.width) / 2;
            *y = 0;
        break;

        case ORI_TOP_RIGHT:
            *x = mWidth - bm.width;
            *y = 0;
        break;

        case ORI_CENTER_LEFT:
            *x = 0;
            *y = (mHeight - bm.height) / 2;
        break;

        case ORI_CENTER_RIGHT:
            *x = mWidth - bm.width;
            *y = (mHeight - bm.height) / 2;
        break;

        case ORI_BOTTOM_LEFT:
            *x = 0;
            *y = mHeight - bm.height;
        break;

        case ORI_BOTTOM_MIDDLE:
            *x = (mWidth - bm.width) / 2;
            *y = mHeight - bm.height;
        break;

        case ORI_BOTTOM_RIGHT:
            *x = mWidth - bm.width;
            *y = mHeight - bm.height;
        break;

        default:    // ORI_CENTER_MIDDLE
            *x = (mWidth - bm.width) / 2;
            *y = (mHeight - bm.height) / 2;
    }
}
