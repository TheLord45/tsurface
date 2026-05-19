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
#ifndef TDRAWBGSLIDER_H
#define TDRAWBGSLIDER_H

#include "tobjecthandler.h"

class QString;
class QPixmap;

/**
 * @brief The TDrawBgSlider class
 *
 * This class draws the slider button of a bargraph. It is used by TDrawObject
 * to draw the slider button if the slider style is defined. It combines the
 * base image, the alpha image and the color to create the final pixmap for the
 * slider button.
 */
class TDrawBgSlider
{
    public:
        TDrawBgSlider();
        ~TDrawBgSlider();

        /**
         * @brief drawSliderButton
         *
         * This method draws the slider button of a bargraph. It is used by
         * TDrawObject.
         *
         * @param bm        A pointer to the pixmap where the slider button
         * should be drawn.
         *
         * @param slider    The name of the slider style to use. This must be a
         * valid slider style defined in TGraphics.
         *
         * @param obj       The object for which the slider button should be
         * drawn. This is needed to get the color of the slider button.
         *
         * @return On success it returns true. On error it returns false.
         */
        bool drawSliderButton(QPixmap *bm, const QString& slider, const ObjHandler::TOBJECT_t& obj);

    protected:
        /**
         * @brief combineImages
         *
         * This method combines the base image, the alpha image and the color to
         * create the final pixmap for the slider button.
         *
         * @param bm    A pointer to the pixmap where the combined image should be stored.
         * @param base  The base image of the slider button.
         * @param alpha The alpha image of the slider button.
         * @param col   The color to apply to the base image. This is used to create the final
         * @return On success it returns true. On error it returns false.
         */
        bool combineImages(QPixmap *bm, const QPixmap& base, const QPixmap& alpha, QColor col);
        /**
         * @brief stretchImageWidth
         *
         * This method stretches the given pixmap to the given width. The height
         * of the pixmap is not changed. The method calculates the new width
         * based on the fixed size and the current width of the pixmap. If the
         * given width is negative, the new width is calculated as current
         * width + given width. If the new width is less than or equal to 0,
         * it is set to 1 to avoid errors.
         *
         * @param bm        A pointer to the pixmap to stretch. The stretched
         * pixmap is stored in this pointer.
         *
         * @param width     The width to stretch the pixmap to.
         *
         * @return On success it returns true. On error it returns false.
         */
        bool stretchImageWidth(QPixmap *bm, int width);
        /**
         * @brief stretchImageHeight
         *
         * This method stretches the given pixmap to the given height. The width
         * of the pixmap is not changed. The method calculates the new height
         * based on the fixed size and the current height of the pixmap. If the
         * given height is negative, the new height is calculated as current
         * height + given height. If the new height is less than or equal to 0,
         * it is set to 1 to avoid errors.
         *
         * @param bm        A pointer to the pixmap to stretch. The stretched
         * pixmap is stored in this pointer.
         *
         * @param height    The height to stretch the pixmap to.
         *
         * @return On success it returns true. On error it returns false.
         */
        bool stretchImageHeight(QPixmap *bm, int height);
};

#endif // TDRAWBGSLIDER_H
