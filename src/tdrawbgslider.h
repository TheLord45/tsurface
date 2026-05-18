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

class TDrawBgSlider
{
    public:
        TDrawBgSlider();

        static TDrawBgSlider& Current();

        bool drawSliderButton(QPixmap *bm, const QString& slider, const ObjHandler::TOBJECT_t& obj);

    protected:
        bool combineImages(QPixmap *bm, const QPixmap& base, const QPixmap& alpha, QColor col);
        bool stretchImageWidth(QPixmap *bm, int width);
        bool stretchImageHeight(QPixmap *bm, int height);

    private:
        static TDrawBgSlider *mCurrent;
};

#endif // TDRAWBGSLIDER_H
