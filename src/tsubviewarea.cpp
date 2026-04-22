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

TSubViewArea::TSubViewArea()
{
    DECL_TRACER("TSubViewArea::TSubViewArea()");
}

TSubViewArea::TSubViewArea(QPixmap *pm)
    : mPixmap(pm)
{
    DECL_TRACER("TSubViewArea::TSubViewArea(QPixmap *pm)");
}

QPixmap *TSubViewArea::drawSubViewMock(int width, int height, int numItems)
{
    DECL_TRACER("TSubViewArea::drawSubViewMock(int width, int height, int numItems)");

    if (!mPixmap)
    {
        mPixmap = new QPixmap(width, height);
        mPixmap->fill(Qt::transparent);
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

    MSG_DEBUG("Using layout color: " << mLayoutColor.name(QColor::HexArgb).toStdString());
    QPainter p(mPixmap);

    if (mScrollbarVisible)
    {
        MSG_DEBUG("Drawing scrollbar mock ...");
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
    int spacePixel = static_cast<int>(static_cast<double>(mVertical ? height : width) / 100.0 * static_cast<double>(mSpace));
    MSG_DEBUG("Drawing " << numItems << " items with a space of " << spacePixel << " pixel.");

    for (int i = 0; i < numItems; ++i)
    {
        MSG_DEBUG("Next item: " << startX << ", " << startY << ", " << mItemSize.width() << ", " << mItemSize.height());

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
