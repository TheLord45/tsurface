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
#include <QPixmap>

#include <sstream>
#include <iomanip>

#include "tmisc.h"
#include "terror.h"

using std::stringstream;
using std::hex;
using std::setw;
using std::setfill;

QString basename(const QString& path)
{
    qsizetype pos = path.lastIndexOf('/');

    if (pos < 0)
        return path;

    return path.mid(pos + 1);
}

QString pathname(const QString& path)
{
    qsizetype pos = path.lastIndexOf('/');

    if (pos < 0)
        return ".";

    return path.left(pos);
}

int getObjectID(const QString& name, const QString& hint)
{
    MSG_DEBUG("Searching for object " << name.toStdString() << " with hint " << hint.toStdString());

    if (!name.contains("_"))
        return -1;

    if (!hint.isEmpty() && !name.startsWith(hint))
        return -1;

    QString s = name;
    qsizetype pos = s.lastIndexOf('_');
    return s.remove(0, pos+1).toInt();
}

QString wcharToUnicodeString(const QChar& ch)
{
    char16_t letter = ch.unicode();
    stringstream ss;
    ss << setw(4) << setfill('0') << hex << letter;
    return QString::fromStdString(ss.str());
}

QPixmap makePixmapFromString(const QString& str, int width)
{
    QString byte;
    int pos = 1;
    int x = 0, y = 0;
    QPixmap px;
    QImage img;

    for (QChar c : str)
    {
        if ((pos % 2) == 0)
        {
            uint pixel = byte.toUInt(nullptr, 16);
            byte.clear();
            img.setPixel(x, y, pixel);
            x++;

            if (x >= width)
            {
                x = 0;
                y++;
            }
            else
                x++;
        }

        byte.append(c);
    }

    px = QPixmap::fromImage(img);
    return px;
}
