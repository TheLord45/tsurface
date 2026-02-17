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
#ifndef TCHECKERBOARDWIDGET_H
#define TCHECKERBOARDWIDGET_H

#include <QLabel>

class TCheckerBoardWidget : public QLabel
{
    Q_OBJECT

    public:
        TCheckerBoardWidget(QWidget *parent=nullptr);

    public slots:
        void setSquareSize(int size);
        void setDarkColor(const QColor &color);
        void setLightColor(const QColor &color);

    private:
        void updateCheckerboard();

        int mSquareSize = 10;
        QColor mDarkColor = Qt::black;
        QColor mLightColor = Qt::white;
};

#endif // TCHECKERBOARDWIDGET_H
