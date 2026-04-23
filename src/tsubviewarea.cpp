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
#include <QPixmap>

#include "tsubviewarea.h"
#include "terror.h"

/**
 * @brief TSubViewArea::TSubViewArea
 *
 * Simple constructor. If there will be no assigning of a pixmap with
 * setPixmap(), the class will assign a pixmap.
 */
TSubViewArea::TSubViewArea()
{
    DECL_TRACER("TSubViewArea::TSubViewArea()");
}

/**
 * @brief TSubViewArea::TSubViewArea
 *
 * Constructor which takes a pointer to a pixmap. The pixmap must be a valid one
 * with the size of the sub-view.
 *
 * @param pm    A pointer to a pixmap.
 */
TSubViewArea::TSubViewArea(QPixmap *pm)
    : mPixmap(pm)
{
    DECL_TRACER("TSubViewArea::TSubViewArea(QPixmap *pm)");
}

TSubViewArea::~TSubViewArea()
{
    DECL_TRACER("TSubViewArea::~TSubViewArea()");

    if (mOwnPixmap && mPixmap)
        delete mPixmap;
}

/**
 * @brief TSubViewArea::drawSubViewMock
 *
 * This method draws a sub-view object in the correct size and the defined
 * colors and images. The items in the sub-view are in the correct size but are
 * only placeholders. Also the scrollbar is not real.
 * If there was no pixmap set by the constructor or the method @b setPixmap(),
 * the method allocates a pixmap with a transparent background. Then it returns
 * a pointer to this pixmap. In this case, if the class is destroyed, the pixmap
 * is deleted automatically. Otherwise it is up to the caller to delete the
 * pixmap.
 *
 * @param width     The width ob the sub-view
 * @param height    The height of the sub-view
 * @param numItems  The number of items to display
 *
 * @return On success the method returns a pointer to the QPixmap containing
 * the representation of the sub-view.
 * If The QPixmap was set by the constructor or the method @b setPixmap(), the
 * pointer points to the set pixmap.
 */
QPixmap *TSubViewArea::drawSubViewMock(int width, int height, int numItems)
{
    DECL_TRACER("TSubViewArea::drawSubViewMock(int width, int height, int numItems)");

    if (!mPixmap)
    {
        mPixmap = new QPixmap(width, height);
        mPixmap->fill(Qt::transparent);
        mOwnPixmap = true;
    }

    // Calculate start position
    int startX = 0;
    int startY = 0;
    int sb = mScrollbarVisible ? 6 : 0;

    if (mVertical)
    {
        switch(mAnchor)
        {
            case ANCHOR_LEFT_TOP:
                startX = (width - sb - mItemSize.width()) / 2;
                startY = 0;
            break;

            case ANCHOR_RIGHT_BOTTOM:
                startX = (width - sb - mItemSize.width()) / 2;
                startY = height - mItemSize.height();
            break;

            default:
                startX = (width - sb - mItemSize.width()) / 2;
                startY = (height - mItemSize.height()) / 2;
        }
    }
    else
    {
        switch(mAnchor)
        {
            case ANCHOR_LEFT_TOP:
                startX = 0;
                startY = (height - sb - mItemSize.height()) / 2;
            break;

            case ANCHOR_RIGHT_BOTTOM:
                startX = width - mItemSize.width();
                startY = (height - sb - mItemSize.height()) / 2;
            break;

            default:
                startX = (width - mItemSize.width()) / 2;
                startY = (height - sb - mItemSize.height()) / 2;
        }
    }

    QPainter p(mPixmap);

    if (mScrollbarVisible)
    {
        p.setPen(Qt::black);

        if (mVertical)
        {
            p.fillRect(width - sb, 0, sb, height, Qt::gray);
            p.drawLine(width - sb, 0, width - sb + 1, height);
        }
        else
        {
            p.fillRect(0, height - sb, width, sb, Qt::gray);
            p.drawLine(0, height - sb, width, height - sb + 1);
        }
    }

    QBrush brush(Qt::Dense6Pattern);
    brush.setColor(mLayoutColor);
    QPen pen(mLayoutColor);
    pen.setStyle(Qt::SolidLine);

    p.setPen(pen);
    p.setBrush(brush);
    p.setBackground(Qt::transparent);
    int spacePixel = static_cast<int>(static_cast<double>(mVertical ? mItemSize.height() : mItemSize.width()) / 100.0 * static_cast<double>(mSpace));

    for (int i = 0; i < numItems; ++i)
    {
        if (mVertical && startY >= height)
            break;
        else if (!mVertical && startX >= width)
            break;

        p.drawRect(startX, startY, mItemSize.width(), mItemSize.height());
        p.fillRect(startX + 1, startY + 1, mItemSize.width() - 2, mItemSize.height() - 2, brush);

        if (mVertical)
            startY += mItemSize.height() + spacePixel;
        else
            startX += mItemSize.width() + spacePixel;
    }

    p.end();
    return mPixmap;
}

/**
 * @brief TSubViewArea::setAnchor
 * This defines the anchor. The anchor defines the start of the first item.
 *
 * @param we    This can be "l/t" (left, top), "r/b" (right, bottom) or empty
 * for center. Everything not "l/t" or "r/b" is treated as center.
 */
void TSubViewArea::setAnchor(const QString& we)
{
    DECL_TRACER("TSubViewArea::setAnchor(const QString& we)");

    if (we == "l/t")
        mAnchor = ANCHOR_LEFT_TOP;
    else if (we == "r/b")
        mAnchor = ANCHOR_RIGHT_BOTTOM;
    else
        mAnchor = ANCHOR_CENTER;
}

/**
 * @brief TSubViewArea::setPixmap
 *
 * Assigns a pixmap. It is up to the caller to assign a valid pixmap with the
 * correct size.
 * If the parameter @b pm is a @a nullptr the method ignores it and does
 * nothing. If there was already a pixmap assigned by the constructor, the
 * pointer to it is overwritten. If the pixmap was assigned by the class, it is
 * deleted and then the new pointer is assigned.
 *
 * @param pm    A pointer to a valid pixmap.
 */
void TSubViewArea::setPixmap(QPixmap *pm)
{
    DECL_TRACER("TSubViewArea::setPixmap(QPixmap *pm)");

    if (!pm)
        return;

    if (mOwnPixmap && mPixmap)
        delete mPixmap;

    mPixmap = pm;
    mOwnPixmap = false;
}
