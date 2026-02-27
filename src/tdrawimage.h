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
#ifndef TDRAWIMAGE_H
#define TDRAWIMAGE_H

#include <QWidget>
#include "tobjecthandler.h"

class TDrawImage
{
    public:
        TDrawImage();
        TDrawImage(const QList<ObjHandler::BITMAPS_t>& bitmaps, ObjHandler::TBASEOBJ_t *object);

        /**
         * @brief setBitmaps
         * Sets the bitmap stack. This must not be more than 5 entries!
         *
         * @param bitmaps   The bitmap stack
         */
        void setBitmaps(const QList<ObjHandler::BITMAPS_t>& bitmaps) { mBitmaps = bitmaps; };
        /**
         * @brief setOpacity
         * This sets the opacity of the object. It can be a value between
         * 0 and 255, where 0 is fully transparent and 255 is fully opaque.
         *
         * @param oo    The opacity value between 0 and 255. 255 is default.
         */
        void setOpacity(int oo);
        /**
         * @brief setPixmap
         * In case an object, not the background of a page of popup, should be
         * drawn, this must be set to a valid QPixmap. The QPixmap must already
         * be initialized and have at least a background. Usually a color. The
         * size must be the size of the object.
         *
         * @param pm    A QPixmap. It is used to draw the bitmap stack on.
         */
        void setPixmap(QPixmap *pm) { mPixmap = pm; }
        /**
         * @brief setChameleon
         * If a chameleon image is wanted, the color mask (red and green pixels)
         * must be set here. The image should have the same size as the object
         * it belongs to.
         *
         * @param newChameleon  The file name of a file containing the image
         * of the mask.
         */
        void setChameleon(const QString& newChameleon) { mChameleon = newChameleon; }
        /**
         * @brief setChameleonColors
         * To set the wanted colors of an object in case a chameleon image should
         * be created, the colors must be set.
         * This is honored only if the image is for an object.
         *
         * @param col1  First color. Default: black
         * @param col2  Second color. Default: white
         */
        void setChameleonColors(const QColor& col1, const QColor& col2);
        /**
         * @brief setSize
         * Set the size of an object. This can be usefull if the bitmaps
         * have different sizes then the object. In this case this method
         * defines the size of the object. It is ignored for pages and
         * popups.
         * If this is not set, the size is taken from the pixmap set by
         * method @code setPixmap().
         *
         * @param w     Width of object
         * @param h     Height of object
         */
        void setSize(int w, int h) { mWidth = w; mHeight = h; }
        /**
         * @brief setSize
         * Set the size of an object. This can be usefull if the bitmaps
         * have different sizes then the object. In this case this method
         * defines the size of the object. It is ignored for pages and
         * popups.
         * If this is not set, the size is taken from the pixmap set by
         * method @code setPixmap().
         *
         * @param s     The size of the object.
         */
        void setSize(QSize s) { mWidth = s.width(); mHeight = s.height(); }
        /**
         * @brief getPixmap
         * Returns the internal pixmap where the method @code draw() painted
         * the bitmap stack.
         *
         * @return  A pointer to the pixmap containing the ready object.
         * If there was an error or no pixmap was drawn, @code nullptr is
         * returned.
         */
        QPixmap *getPixmap() { return mPixmap; }

        /**
         * @brief draw
         * The method draws the whole bitmap stack. The target can be either
         * the background of a page or a popup or an object. In case of an
         * object the images are drawn over an evenually existing image.
         *
         * If the credentials for a chameleon image were set, it is created.
         * In this case the first bitmap in the stack is taken as a mask.
         * All other bitmaps, if there are any, are drawn on top of the
         * chameleon image.
         */
        void draw();

    protected:
        void drawBitmapStack();
        void drawChameleon();
        QColor baseColor(QColor basePix, QColor maskPix);
        void getLeftUpper(int *x, int *y, ObjHandler::BITMAPS_t bm);

    private:
        ObjHandler::TBASEOBJ_t *mObject{nullptr};
        QPixmap *mPixmap{nullptr};
        QString mChameleon;
        QList<ObjHandler::BITMAPS_t> mBitmaps;
        int mOpacity{-1};
        bool mHaveWidget{false};
        int mWidth{0};
        int mHeight{0};
        QColor mColor1{qRgb(0, 0, 0)};
        QColor mColor2{qRgb(255, 255, 255)};
};

#endif // TDRAWIMAGE_H
