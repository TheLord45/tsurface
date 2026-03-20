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

class QDomElement;

class TFonts
{
    public:
        typedef struct PRIVFONTS_t
        {
            QStringList family;
            QString file;
            QString intFile;
            int ID{0};
            int fileSize{0};        // G4
            int fi{0};              // G4 font index
            QString name;           // G4 font name
            QString subfamilyName;  // G4 subfamily name (not available on *NIX fonts!)
            int size{0};            // G4 The used font size
            int usageCount{0};      // Statistics about usage
        }PRIVFONTS_t;

        static QString getFontFile(const QFont& font);
        static void addFont(const QFont& font, const QString& file);
        static void addFontFile(const QString& file);
        static void freePrivateFonts();
        static void releaseFontConfig();
        static QFont getFont(const QString& ff);
        static QFont getFontFromIndex(int index);

        static bool readFontFile(const QString& path, const QString& file);
        static bool writeFontFile(const QString& path, const QString& file);
        static bool readAMXFontFile(const QString& file);

    protected:
        static void init();
        static bool readSystemFonts(const QString& path);
        static void parseFont(const QDomElement &font);

    private:
        TFonts() {};    // Must never be called

        static QList<PRIVFONTS_t> mLocalFonts;
        static bool mInitialized;
};

#endif // TFONTS_H
