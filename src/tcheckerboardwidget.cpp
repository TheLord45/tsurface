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

#include "tcheckerboardwidget.h"
#include "terror.h"

TCheckerBoardWidget::TCheckerBoardWidget(QWidget *parent)
    : QLabel(parent)
{
    DECL_TRACER("TCheckerBoardWidget::TCheckerBoardWidget(QWidget *parent)");

    if (parent)
        setFixedSize(parent->size());
}

void TCheckerBoardWidget::setSquareSize(int size)
{
    DECL_TRACER("TCheckerBoardWidget::setSquareSize(int size)");

    mSquareSize = size;
    updateCheckerboard();
}

void TCheckerBoardWidget::setDarkColor(const QColor &color)
{
    DECL_TRACER("TCheckerBoardWidget::setDarkColor(const QColor &color)");

    mDarkColor = color;
    updateCheckerboard();
}

void TCheckerBoardWidget::setLightColor(const QColor &color)
{
    DECL_TRACER("TCheckerBoardWidget::setLightColor(const QColor &color)");

    mLightColor = color;
    updateCheckerboard();
}

void TCheckerBoardWidget::updateCheckerboard()
{
    int w = width();
    int h = height();
    QPixmap pixmap(w, h);
    pixmap.fill(mLightColor);

    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);

    for (int y = 0; y < h; y += mSquareSize)
    {
        for (int x = 0; x < w; x += mSquareSize)
        {
            bool isBlackSquare = ((x / mSquareSize) + (y / mSquareSize)) % 2 == 0;
            painter.setBrush(isBlackSquare ? mDarkColor : mLightColor);
            painter.drawRect(x, y, mSquareSize, mSquareSize);
        }
    }

    setPixmap(pixmap);
}
