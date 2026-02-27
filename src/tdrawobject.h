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
#ifndef TDRAWOBJECT_H
#define TDRAWOBJECT_H

#include <QWidget>

#include "tobjecthandler.h"

#define ORD_ELEM_COUNT  5

/**
 * @brief The TDrawObject class
 * This class draws all instances of an object. The object is stored as
 * a QPixmap in an array. There is an entry in the array for each instance.
 * The order in the array is the same as the order in the array \a sr[],
 * which defines the instances (state array).
 *
 * \b DRAW_ORDER_t \b mDOrder[]: This defines the draw order. It defines
 * the order the elements of an object are drawn. With G4 this order was
 * definable by the user. With G5 it is fixed because it makes no sense to
 * change it. However: TSurface is able to import G4 designs and therefore
 * this draw order may be different.
 */
class TDrawObject
{
    public:
        TDrawObject(TObjectHandler *object, QWidget *widget);

        typedef enum DRAW_ORDER_t
        {
            ORD_ELEM_NONE,
            ORD_ELEM_FILL,
            ORD_ELEM_BITMAP,
            ORD_ELEM_ICON,
            ORD_ELEM_TEXT,
            ORD_ELEM_BORDER
        }DRAW_ORDER_t;

        void draw();
        static ObjHandler::GRAD_TYPE_t getGradientType(const QString& grad);

    private:
        void getDrawOrder(const QString& sdo, DRAW_ORDER_t *order);
        void getDefaultDrawOrder(DRAW_ORDER_t *order);
        bool buttonFill(QPixmap* bm, ObjHandler::SR_T sr);
        bool drawBackgroundColor(QPixmap* bm, ObjHandler::SR_T& sr, QList<QString>& gradients);

        TObjectHandler *mObject{nullptr};       // A pointer to the object
        QWidget *mWidget{nullptr};              // The widget to place the pixmap representing the object
        QList<QPixmap> mPixButtons;             // The object as a pixmap; A pixmap for each instance

        DRAW_ORDER_t mDOrder[ORD_ELEM_COUNT];   // The order to draw the elements of an object

};

#endif // TDRAWOBJECT_H
