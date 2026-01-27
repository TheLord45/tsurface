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
#include <QPaintEvent>
#include <QToolTip>

#include "tcharacterwidget.h"
#include "terror.h"

TCharacterWidget::TCharacterWidget(QWidget *parent)
    : QWidget(parent)
{
    DECL_TRACER("TCharacterWidget::TCharacterWidget(QWidget *parent)");

    mColumns = 16;
    mLastKey = -1;
    setMouseTracking(true);
}

void TCharacterWidget::updateFont(const QFont &font)
{
    DECL_TRACER("TCharacterWidget::updateFont(const QFont &font)");

    mDisplayFont = font;
    mSquareSize = qMax(24, QFontMetrics(mDisplayFont).xHeight() * 3);
    adjustSize();
    update();
}

void TCharacterWidget::updateSize(const QString &fontSize)
{
    DECL_TRACER("TCharacterWidget::updateSize(const QString &fontSize)");

    mDisplayFont.setPointSize(fontSize.toInt());
    mSquareSize = qMax(24, QFontMetrics(mDisplayFont).xHeight() * 3);
    adjustSize();
    update();
}

QSize TCharacterWidget::sizeHint() const
{
    DECL_TRACER("TCharacterWidget::sizeHint()");

    return QSize(mColumns * mSquareSize, (65536 / mColumns) * mSquareSize);
}

void TCharacterWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), QBrush(Qt::white));
    painter.setFont(mDisplayFont);

    QRect redrawRect = event->rect();
    int beginRow = redrawRect.top() / mSquareSize;
    int endRow = redrawRect.bottom() / mSquareSize;
    int beginColumn = redrawRect.left() / mSquareSize;
    int endColumn = redrawRect.right() / mSquareSize;

    painter.setPen(QPen(Qt::gray));

    for (int row = beginRow; row <= endRow; ++row)
    {
        for (int column = beginColumn; column <= endColumn; ++column)
        {
            painter.drawRect(column*mSquareSize, row*mSquareSize, mSquareSize, mSquareSize);
        }
    }

    QFontMetrics fontMetrics(mDisplayFont);
    painter.setPen(QPen(Qt::black));

    for (int row = beginRow; row <= endRow; ++row)
    {
        for (int column = beginColumn; column <= endColumn; ++column)
        {
            int key = row*mColumns + column;
            painter.setClipRect(column*mSquareSize, row*mSquareSize, mSquareSize, mSquareSize);

            if (key == mLastKey)
                painter.fillRect(column*mSquareSize + 1, row*mSquareSize + 1, mSquareSize, mSquareSize, QBrush(Qt::red));

            painter.drawText(column * mSquareSize + (mSquareSize / 2) - fontMetrics.boundingRect(QChar(key)).width() / 2,
                             row * mSquareSize + 4 + fontMetrics.ascent(), QString(QChar(key)));
        }
    }
}

void TCharacterWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint pos = event->position().toPoint();
        mLastKey = (pos.y() / mSquareSize) * mColumns + pos.x() / mSquareSize;

        if (QChar(mLastKey).category() != QChar::Mark_NonSpacing)
            emit characterSelected(QString(QChar(mLastKey)));

        update();
    }
    else
        QWidget::mousePressEvent(event);
}

void TCharacterWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint widgetPosition = mapFromGlobal(event->globalPosition()).toPoint();
    uint key = (widgetPosition.y() / mSquareSize) * mColumns + widgetPosition.x() / mSquareSize;

    QString text = QString::fromLatin1("<p>Character: <span style=\"font-size: 24pt; font-family: %1\">").arg(mDisplayFont.family())
                   + QChar(key)
                   + QString::fromLatin1("</span><p>Value: 0x")
                   + QString::number(key, 16);
    QToolTip::showText(event->globalPosition().toPoint(), text, this);
}
