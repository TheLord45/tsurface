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
#ifndef TFONTS_H
#define TFONTS_H

#include <QString>
#include <QStringList>
#include <QFont>

class TFonts
{
    public:
        typedef struct PRIVFONTS_t
        {
            QStringList family;
            QString file;
            QString intFile;
            int ID{0};
        }PRIVFONTS_t;

        static QString getFontFile(const QFont& font);
        static void addFont(const QFont& font, const QString& file);
        static void addFontFile(const QString& file);
        static void freePrivateFonts();
        static void releaseFontConfig();

        static bool readFontFile(const QString& path, const QString& file);
        static bool writeFontFile(const QString& path, const QString& file);

    protected:
        static void init();

    private:
        TFonts() {};    // Must never be called

        static QList<PRIVFONTS_t> mLocalFonts;
        static bool mInitialized;
};

#endif // TFONTS_H
