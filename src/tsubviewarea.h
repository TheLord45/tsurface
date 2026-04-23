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

/**
 * @brief The TSubViewArea class
 *
 * It draws a mockup of a sub-view-area. By setting all credentials for the
 * optical view, the mockup will show how the sub-view may look on a real panel.
 */
class TSubViewArea
{
    public:
        TSubViewArea();
        TSubViewArea(QPixmap *pm);
        ~TSubViewArea();

        typedef enum
        {
            ANCHOR_UNKNOWN,
            ANCHOR_LEFT_TOP,
            ANCHOR_CENTER,
            ANCHOR_RIGHT_BOTTOM
        }ANCHOR_POS_t;

        QPixmap *drawSubViewMock(int width, int height, int numItems);
        /**
         * @brief setScrollbarVisible
         *
         * Defines whether the scrollbar should be visible or not. In case it
         * is visible, the class draws a small gray rectangular representing a
         * scrollbar.
         *
         * @param on    TRUE: The scrollbar is visible.
         */
        void setScrollbarVisible(bool on) { mScrollbarVisible = on; }
        /**
         * @brief setScrollbarOffset
         *
         * Defines the scrollbar offset. This parameter does not influence the
         * representation.
         *
         * @param off   A value defining the offset.
         */
        void setScrollbarOffset(int off) { mScrollbarOffset = off; }
        void setAnchor(const QString& we);
        /**
         * @brief setAnchor
         *
         * Defines the anchor position.  The anchor defines the start of the
         * first item.
         *
         * @param pos   The anchor position. The value @a ANCHOR_UNKNOWN is
         * treated as center.
         */
        void setAnchor(ANCHOR_POS_t pos) { mAnchor = pos; };
        /**
         * @brief setVertical
         *
         * Defines the direction of the scrolling. By default it scrolls
         * horizontally.
         *
         * @param on    TRUE: Scroll vertically.
         */
        void setVertical(bool on) { mVertical = on; }
        /**
         * @brief setLayoutColor
         *
         * Defines the color used to draw the mocked items. By default this is
         * white.
         *
         * @param col   Any color.
         */
        void setLayoutColor(const QColor& col) { mLayoutColor = col; }
        /**
         * @brief setSpace - Space in percent
         *
         * Defines the space between the items. This is calculated from the
         * width or height of an item, depnding whether it is vertical or
         * horizontal direction. The space must be defined as percent.
         *
         * @param percent   The percentage value of the space between the items.
         * Valid values are 0 to 100.
         */
        void setSpace(int percent) { mSpace = percent; }
        /**
         * @brief setItemSize
         *
         * Defines the size of the items. All items are treated as the same
         * size, as it should be.
         *
         * @param size  The size of 1 item. The width and/or the height must not
         * be grater then the width or height of the sub-view.
         */
        void setItemSize(const QSize& size) { mItemSize = size; }
        /**
         * @brief setItemSize
         *
         * Defines the size of the items. All items are treated as the same
         * size, as it should be.
         *
         * @param size  The size of 1 item. The width and/or the height must not
         * be grater then the width or height of the sub-view.
         */
        void setItemSize(int width, int height) { mItemSize = QSize(width, height); }
        void setPixmap(QPixmap *pm);

    private:
        QPixmap *mPixmap{nullptr};
        bool mScrollbarVisible{false};
        int mScrollbarOffset{0};
        ANCHOR_POS_t mAnchor{ANCHOR_CENTER};
        bool mVertical{false};
        QSize mItemSize;
        QColor mLayoutColor{Qt::white};
        int mSpace{10};
        bool mOwnPixmap{false};
};

#endif // TSUBVIEWAREA_H
