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
#include "tgraphics.h"
#include "terror.h"

using namespace ObjHandler;

#define LABEL_NAME  "LabelPixmap"

TDrawImage::TDrawImage()
{
    DECL_TRACER("TDrawImage::TDrawImage()");
}

TDrawImage::TDrawImage(TBASEOBJ_t *object)
    : mObject(object)
{
    DECL_TRACER("TDrawImage::TDrawImage(TBASEOBJ_t *object)");
}

TDrawImage::TDrawImage(const QList<BITMAPS_t>& bitmaps, TBASEOBJ_t *object)
    : mBitmaps(bitmaps),
      mObject(object)
{
    DECL_TRACER("TDrawImage::TDrawImage(const QList<BITMAPS_t>& bitmaps, TBASEOBJ_t *object)");
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
    if (!mChameleon.isEmpty())
        drawChameleon();

    // If there are more than one bitmap draw them now
    // on top of chameleon image, if there is one.
    // Or draw the bitmap stack.
    drawBitmapStack();
}

void TDrawImage::drawPixmap(const QPixmap& bm)
{
    DECL_TRACER("TDrawImage::drawPixmap(const QPixmap& bm)");

    if ((!mObject || !mObject->widget) && !mPixmap)
    {
        MSG_ERROR("Got no widget to draw to.");
        return;
    }

    mHaveWidget = mObject && mObject->widget ? true : false;

    if (!mHaveWidget)
        return;

    TCanvasWidget *widget = nullptr;
    QPixmap pix = bm.scaled(mObject->widget->size());

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

    label->setPixmap(pix);
    label->lower();
    label->show();
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
    QPixmap imgRed;     // source: object.sr[instance].mi
    BITMAPS_t bitmapEntry;
    // Load the image
    if (!imgRed.load(pathTemp + "/images/" + mChameleon))
    {
        MSG_ERROR("Couldn't load image " << mChameleon.toStdString());
        return;
    }

    MSG_DEBUG("Loaded chameleon image " << mChameleon.toStdString());
    QPixmap imgMask;

    if (mBitmaps.size() > 0)
    {
        bitmapEntry = mBitmaps[0];

        if (!imgMask.load(pathTemp + "/images/" + bitmapEntry.fileName))
        {
            MSG_ERROR("Couldn't load mask image " << bitmapEntry.fileName.toStdString());
            imgMask = QPixmap();
        }
        else
        {
            MSG_DEBUG("Loaded chameleon mask " << bitmapEntry.fileName.toStdString());
        }
    }

    bool haveBothImages = mBitmaps.empty() || imgMask.isNull() ? false : true;
    MSG_DEBUG("Have " << (haveBothImages ? "both" : "only one") << " image(s).");

    if (!haveBothImages)
    {
        bitmapEntry.justification = ObjHandler::ORI_CENTER_MIDDLE;
        bitmapEntry.offsetX = 0;
        bitmapEntry.offsetY = 0;
    }

    bitmapEntry.width = imgRed.width();
    bitmapEntry.height = imgRed.height();

    QImage pixmapRed = imgRed.toImage();
    QImage pixmapMask;
    QImage maskbitmapEntry(mWidth, mHeight, QImage::Format_ARGB32);
    maskbitmapEntry.fill(Qt::transparent);
    MSG_DEBUG("Size of pixmapRed: " << pixmapRed.width() << " x " << pixmapRed.height());

    if (!imgMask.isNull())
    {
        pixmapMask = imgMask.toImage();
        MSG_DEBUG("Size of pixmapMask: " << pixmapMask.width() << " x " << pixmapMask.height());
    }

    for (int ix = 0; ix < mWidth; ix++)
    {
        for (int iy = 0; iy < mHeight; iy++)
        {
            QColor pixelRed;
            QColor pixelMask;

            if (ix < pixmapRed.width() && iy < pixmapRed.height())
                pixelRed = pixmapRed.pixelColor(ix, iy);
            else
                pixelRed = Qt::transparent;

            if (haveBothImages && !imgMask.isNull() &&
                    ix < pixmapMask.width() && iy < pixmapMask.height())
                pixelMask = pixmapMask.pixelColor(ix, iy);
            else
                pixelMask = Qt::transparent;

            QColor pixel = baseColor(pixelRed, pixelMask);
            maskbitmapEntry.setPixelColor(ix, iy, pixel);
        }
    }

    int x = 0, y = 0;
    // A button with a chameleon image can't have a border. Therefore we set the
    // 4th parameter to true which will ignore the frame and don't even try to
    // find one.
    // But the bitmap entry may have other positioning then center, middle.
    // Therefore we must check this and retrieve the upper left corner.
    getLeftUpper(&x, &y, bitmapEntry, true);
    // To have all transparent pixels from the chameleon image we must fill the
    // base image with transparent pixels. This removes the background which was
    // already drawn.
    mPixmap->fill(Qt::transparent);

    QPainter p(mPixmap);

    if (mOpacity >= 0 && mOpacity < 255)
        p.setOpacity(1.0 / 255.0 * static_cast<qreal>(mOpacity));         // set opacity from 0.0 to 1.0, where 0.0 is fully transparent and 1.0 is fully opaque.

    p.drawImage(x, y, maskbitmapEntry);

    if (haveBothImages)
        p.drawImage(x, y, pixmapMask);

    p.end();
}

void TDrawImage::drawBargraph(const TOBJECT_t& object)
{
    DECL_TRACER("TDrawImage::drawBargraph(const TOBJECT_t& object)");

    // A normal bargraph must have 2 stages. If not we return here.
    if (object.sr.size() != 2)
    {
        MSG_WARNING("Object " << object.bi << " (" << object.na.toStdString() << ") is not a bargraph!");
        return;
    }

    QString path = TConfMain::Current().getPathTemporary();
    int level = (object.rh - object.rl) / 2;

    // Check for a chameleon image
    if (!object.sr[0].mi.isEmpty() && object.sr[0].bs.isEmpty() && !object.sr[1].bitmaps.empty())
    {
        MSG_DEBUG("Detected a chameleon image...");
        QPixmap bmMi, bmBm;

        if (!bmMi.load(path + "/images/" + object.sr[0].mi))
        {
            MSG_ERROR("Couldn't load image " << object.sr[0].mi.toStdString() << "!");
            return;
        }

        QString sBitmap = object.sr[1].bitmaps[0].fileName;

        if (!bmBm.load(path + "/images/" + sBitmap))
        {
            MSG_ERROR("Couldn't load image " << sBitmap.toStdString() << "!");
            return;
        }

        QImage pixmapRed = bmMi.toImage();
        QImage pixmapMask = bmBm.toImage();

        int width = bmMi.width();
        int height = bmMi.height();
        int startX = 0;
        int startY = 0;

        if (object.dr == "horizontal")
            width = static_cast<int>(static_cast<double>(width) / static_cast<double>(object.rh - object.rl) * static_cast<double>(level));
        else
        {
            height = static_cast<int>(static_cast<double>(height) / static_cast<double>(object.rh - object.rl) * static_cast<double>(level));
            startY = bmMi.height() - height;
            height = bmMi.height();
        }

        QImage img(bmMi.width(), bmMi.height(), QImage::Format_ARGB32);
        img.fill(Qt::transparent);

        mColor1 = object.sr[1].cf;
        mColor2 = object.sr[1].cb;

        for (int ix = 0; ix < bmMi.width(); ++ix)
        {
            for (int iy = 0; iy < bmMi.height(); ++iy)
            {
                QColor pixel;

                if (ix >= startX && ix < width && iy >= startY && iy < height)
                {
                    QColor pixelRed = pixmapRed.pixelColor(ix, iy);
                    QColor pixelMask;

                    if (!bmBm.isNull())
                        pixelMask = pixmapMask.pixelColor(ix, iy);
                    else
                        pixelMask = Qt::white;

                    pixel = baseColor(pixelRed, pixelMask);
                }
                else
                    pixel = Qt::transparent;

                img.setPixelColor(ix, iy, pixel);
            }
        }

        QPainter painter(mPixmap);
        painter.drawImage(0, 0, img);
        painter.end();
    }
    else if (!object.sr[0].bitmaps.empty() && !object.sr[1].bitmaps.empty())
    {
        MSG_DEBUG("Drawing normal bargraph...");

        QImage image1, image2;

        if (!image1.load(path + "/images/" + object.sr[0].bitmaps[0].fileName))
        {
            MSG_ERROR("Couldn't load image " << object.sr[0].bitmaps[0].fileName.toStdString() << "!");
            return;
        }

        if (!image2.load(path + "/images/" + object.sr[1].bitmaps[0].fileName))
        {
            MSG_ERROR("Couldn't load image " << object.sr[1].bitmaps[0].fileName.toStdString() << "!");
            return;
        }

        int width = image2.width();
        int height = image2.height();
        int startX = 0;
        int startY = 0;

        if (object.dr == "horizontal")
            width = static_cast<int>(static_cast<double>(width) / static_cast<double>(object.rh - object.rl) * static_cast<double>(level));
        else
        {
            height = static_cast<int>(static_cast<double>(height) / static_cast<double>(object.rh - object.rl) * static_cast<double>(level));
            startY = image1.height() - height;
            height = image1.height();
        }

        int bm_width = image2.width();
        int bm_height = image2.height();

        QImage img_bar(bm_width, bm_height, QImage::Format_ARGB32);
        img_bar.fill(Qt::transparent);

        for (int ix = 0; ix < bm_width; ++ix)
        {
            for (int iy = 0; iy < bm_height; ++iy)
            {
                QColor pixel;

                if (ix > startX && ix < width && iy >= startY && iy < height)
                    pixel = image2.pixelColor(ix, iy);
                else
                    pixel = Qt::transparent;

                img_bar.setPixelColor(ix, iy, pixel);
            }
        }

        QPainter painter(mPixmap);
        painter.drawImage(0, 0, image1);
        painter.drawImage(0, 0, image2);
        painter.drawImage(0, 0, img_bar);
        painter.end();
    }
    else if (object.sr[0].bitmaps.empty() && !object.sr[1].bitmaps.empty())
    {
        MSG_DEBUG("Drawing second image...");

        QImage image;

        if (!image.load(path + "/images/" + object.sr[1].bitmaps[0].fileName))
        {
            MSG_ERROR("Couldn't load image " << object.sr[1].bitmaps[0].fileName.toStdString() << "!");
            return;
        }

        int width = image.width();
        int height = image.height();
        int startX = 0;
        int startY = 0;

        if (object.dr == "horizontal")
            width = static_cast<int>(static_cast<double>(width) / static_cast<double>(object.rh - object.rl) * static_cast<double>(level));
        else
        {
            height = static_cast<int>(static_cast<double>(height) / static_cast<double>(object.rh - object.rl) * static_cast<double>(level));
            startY = image.height() - height;
            height = image.height();
        }

        QImage img_bar(image.width(), image.height(), QImage::Format_ARGB32);
        img_bar.fill(Qt::transparent);
        int bm_width = image.width();
        int bm_height = image.height();

        for (int ix = 0; ix < bm_width; ++ix)
        {
            for (int iy = 0; iy < bm_height; ++iy)
            {
                QColor pixel;

                if (ix >= startX && ix < width && iy >= startY && iy < height)
                    pixel = image.pixelColor(iy, iy);
                else
                    pixel = Qt::transparent;

                img_bar.setPixelColor(ix, iy, pixel);
            }
        }

        QPainter painter(mPixmap);
        painter.drawImage(0, 0, image);
        painter.drawImage(0, 0, img_bar);
        painter.end();
    }
    else
    {
        MSG_DEBUG("No bitmap defined...");

        int width = object.wt;
        int height = object.ht;
        int startX = 0;
        int startY = 0;

        if (object.dr == "horizontal")
            width = static_cast<int>(static_cast<double>(width) / static_cast<double>(object.rh - object.rl) * static_cast<double>(level));
        else
        {
            height = static_cast<int>(static_cast<double>(height) / static_cast<double>(object.rh - object.rl) * static_cast<double>(level));
            startY = object.ht - height;
            height = object.ht;
        }

        QPainter painter(mPixmap);
        QRect dst(startX, startY, width, height);
        QBrush brush(object.sr[1].cf);
        painter.setBrush(brush);
        painter.fillRect(dst, Qt::SolidPattern);
    }
}

QColor TDrawImage::baseColor(QColor basePix, QColor maskPix)
{
    int alpha = basePix.alpha();
    int green = basePix.green();
    int red = basePix.red();

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

void TDrawImage::getLeftUpper(int *x, int *y, ObjHandler::BITMAPS_t bm, bool noFrame)
{
    DECL_TRACER("TDrawImage::getLeftUpper(int *x, int *y, ObjHandler::BITMAPS_t bm, bool noFrame)");

    if (!x || !y)
        return;

    Graphics::BORDER_SIZE_t borderSize;

    if (!noFrame)
        borderSize = TGraphics::Current().getBorderSize(mBorder);

    switch(bm.justification)
    {
        case ORI_ABSOLUT:
            *x = bm.offsetX;
            *y = bm.offsetY;
        break;

        case ORI_TOP_LEFT:
            *x = borderSize.fillLeft;
            *y = borderSize.fillTop;
        break;

        case ORI_TOP_MIDDLE:
            *x = (mWidth - bm.width) / 2;
            *y = borderSize.fillTop;
        break;

        case ORI_TOP_RIGHT:
            *x = mWidth - bm.width - borderSize.fillRight;
            *y = borderSize.fillTop;
        break;

        case ORI_CENTER_LEFT:
            *x = borderSize.fillLeft;
            *y = (mHeight - bm.height) / 2;
        break;

        case ORI_CENTER_RIGHT:
            *x = mWidth - bm.width - borderSize.fillRight;
            *y = (mHeight - bm.height) / 2;
        break;

        case ORI_BOTTOM_LEFT:
            *x = borderSize.fillLeft;
            *y = mHeight - bm.height - borderSize.fillBottom;
        break;

        case ORI_BOTTOM_MIDDLE:
            *x = (mWidth - bm.width) / 2;
            *y = mHeight - bm.height - borderSize.fillBottom;
        break;

        case ORI_BOTTOM_RIGHT:
            *x = mWidth - bm.width - borderSize.fillRight;
            *y = mHeight - bm.height - borderSize.fillBottom;
        break;

        default:    // ORI_CENTER_MIDDLE
            *x = (mWidth - bm.width) / 2;
            *y = (mHeight - bm.height) / 2;
    }
}
