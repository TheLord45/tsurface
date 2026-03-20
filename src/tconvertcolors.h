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
#ifndef TCONVERTCOLORS_H
#define TCONVERTCOLORS_H

#include <QString>
#include <QColor>
#include <QList>

class QDomElement;

typedef struct COLOR_t
{
    int index{0};
    QString name;
    QColor color;
}COLOR_t;

class TConvertColors
{
    public:
        static bool readAMXPalette(const QString& filename);
        static QColor getColorFromIndex(int i);
        static QColor getColorFromName(const QString& name);
        static QColor getColorFromColor(const QString& name);

    private:
        TConvertColors() {};
        static void parseColor(const QDomElement& color);

        static QList<COLOR_t> mColors;
};

#endif // TCONVERTCOLORS_H
