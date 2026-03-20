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
#ifndef TCONVERTICONS_H
#define TCONVERTICONS_H

#include <QString>
#include <QList>

class QDomElement;

class TConvertIcons
{
    public:
        TConvertIcons();
        TConvertIcons(const QString& filename);
        ~TConvertIcons();

        typedef struct ICON_t
        {
            int number{0};
            QString file;
        }ICON_t;

        bool readAMXIcons(const QString& filename);
        QString getIcon(int ic);
        QStringList getAllIcons();
        int getMaxIcons() { return mMaxIcons; }

    private:
        void parseIcon(const QDomElement& icon);

        QList<ICON_t> mIcons;
        int mMaxIcons{0};
};

#endif // TCONVERTICONS_H
