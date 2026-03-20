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
#ifndef TMISC_H
#define TMISC_H

#include <QString>

typedef enum
{
    TBL_UNKNOWN,
    TBL_GENERAL,
    TBL_PROGRAM,
    TBL_STATES,
    TBL_EVENTS
}PROPERTIES_t;

enum TOOL
{
    TOOL_NONE,
    TOOL_DRAW,
    TOOL_SELECT,
    TOOL_POPUP
};

enum STATE_TYPE
{
    STATE_UNKNOWN,
    STATE_PAGE,
    STATE_POPUP,
    STATE_SUBPAGE,
    STATE_BUTTON,
    STATE_BARGRAPH,
    STATE_INPUT,
    STATE_LISTVIEW,
    STATE_APP
};

typedef enum
{
    W_NONE,
    W_BITMAPSELECTOR,
    W_BORDERNAME,
    W_COLORSELECTOR,
    W_GRADIENTCOLORS,
    W_LINEEDIT,
    W_SPINBOX,
    W_TEXTEFFECT,
    W_COMBO,
    W_FONT,
    W_TEXT,
    W_SOUND
}ELEMENT_TYPE_t;

class QPixmap;
class QSize;

QString basename(const QString& path);
QString pathname(const QString& path);
int getObjectID(const QString& name, const QString& hint="");
QString wcharToUnicodeString(const QChar& ch);
QPixmap makePixmapFromString(const QString& str, int width);
STATE_TYPE getStateFromButtonType(int bt);
STATE_TYPE getStateFromPageType(int pt);
QPixmap sizeImage(const QSize& size, const QString& file, QSize *ori=nullptr);
QString convertToUTF8(const QString& file);

#define INSERTJ(json, name, src, ref)  if (src != ref) json.insert(name, src)

#endif // TMISC_H
