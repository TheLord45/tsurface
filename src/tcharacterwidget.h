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
#ifndef TCHARACTERWIDGET_H
#define TCHARACTERWIDGET_H

#include <QWidget>

class TCharacterWidget : public QWidget
{
    Q_OBJECT

    public:
        TCharacterWidget(QWidget *parent=nullptr);
        QSize sizeHint() const;

    public slots:
        void updateFont(const QFont &font);
        void updateSize(const QString &fontSize);

    signals:
        void characterSelected(const QString &character);

    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void paintEvent(QPaintEvent *event);

    private:
        QFont mDisplayFont;
        int mColumns;
        int mLastKey;
        int mSquareSize;
};

#endif // TCHARACTERWIDGET_H
