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
#ifndef TSUBVIEWAREA_H
#define TSUBVIEWAREA_H

#include <QSize>
#include <QColor>

class QPixmap;
class QString;

class TSubViewArea
{
    public:
        TSubViewArea();
        TSubViewArea(QPixmap *pm);

        typedef enum
        {
            ANCHOR_UNKNOWN,
            ANCHOR_LEFT_TOP,
            ANCHOR_CENTER,
            ANCHOR_RIGHT_BOTTOM
        }ANCHOR_POS_t;

        QPixmap *drawSubViewMock(int width, int height, int numItems);
        void setScrollbarVisible(bool on) { mScrollbarVisible = on; }
        void setScrollbarOffset(int off) { mScrollbarOffset = off; }
        void setAnchor(const QString& we);
        void setAnchor(ANCHOR_POS_t pos) { mAnchor = pos; };
        void setVertical(bool on) { mVertical = on; }
        void setLayoutColor(const QColor& col) { mLayoutColor = col; }
        void setSpace(int percent) { mSpace = percent; }
        void setItemSize(const QSize& size) { mItemSize = size; }
        void setItemSize(int width, int height) { mItemSize = QSize(width, height); }

    private:
        QPixmap *mPixmap{nullptr};
        bool mScrollbarVisible{false};
        int mScrollbarOffset{0};
        ANCHOR_POS_t mAnchor{ANCHOR_CENTER};
        bool mVertical{false};
        QSize mItemSize;
        QColor mLayoutColor{Qt::white};
        int mSpace{0};
};

#endif // TSUBVIEWAREA_H
