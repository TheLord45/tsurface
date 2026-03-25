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

/**
 * @brief The TFonts class
 * This class manages the font handling. It is able to hold the newer G5 format
 * as well as the older G4 format. While the G4 format had for every font used
 * somewhere the information about which font, attributes and size, the new
 * format holds only the file name, family and a usage counter. We don't use the
 * usage counter because we keep no statistics.
 *
 * The structure @struct PRIVFONTS_t holds all information about a font. The
 * fields have the following meaning:
 *      family      This is a list of family names contained in 1 font file.
 *                  Usually a font have only 1 family name and there is only
 *                  1 font in a file. But exceptions exist.
 *
 *      file        Contains the plain file name of the font. This is really
 *                  the name of the file the font were loaded from.
 *
 *      intFile     Holds the real file name inclusive the path where it is
 *                  located.
 *
 *      bold,       Font attributes. Instead of keeping this indormation in
 *      italic,     the family name of the font, it is kept in this variables.
 *      strike,
 *      underline
 *
 *      ID          The internal ID. On *NIX systems this is coming from the
 *                  system font manager. It is an index into the list of fonts
 *                  and points to a particular font. It is possible to select
 *                  a particular font with this number from the system font
 *                  manager.
 *
 * The rest of the variables in the structure are used when a TP4 file is
 * loaded. They hold all the information for the old format. After the fonts
 * were read, they are converted into the new format.
 */
class TFonts
{
    public:
        typedef struct PRIVFONTS_t
        {
            QStringList family;     // The list of font families
            QString file;           // Only the file name
            QString intFile;        // Intern: Real file name with full path
            bool bold{false};       // Intern: TRUE = bold font
            bool italic{false};     // Intern: TRUE = italic font
            bool strike{false};     // Intern: TRUE = strike out font
            bool underline{false};  // Intern: TRUE = underline font
            int ID{-1};             // Intern: Holds the font ID to mark a font as an extra font loaded from a font file.
            // Old G4 font information
            int fileSize{0};        // G4 size of font file
            int fi{0};              // G4 font index
            QString name;           // G4 font name
            QString subfamilyName;  // G4 subfamily name (not available on *NIX fonts!)
            int size{0};            // G4 The used font size
            int usageCount{0};      // Statistics about usage
        }PRIVFONTS_t;

        static QString getFontFile(const QFont& font);
        static QString getFontName(const QFont& font);
        static void addFont(const QFont& font, const QString& file);
        static void addFontFile(const QString& file);
        static void addFontFamily(const QString& family);
        static void freePrivateFonts();
        static void releaseFontConfig();
        static void reset();
        static QFont getFont(const QString& ff);
        static QString getFontBaseName(const QString& ff);
        static QFont getFontFromIndex(int index);

        static bool readFontFile(const QString& path, const QString& file);
        static bool writeFontFile(const QString& path, const QString& file);
        static bool readAMXFontFile(const QString& file);

    protected:
        static void init();
        static bool readSystemFonts(const QString& path);
        static void parseFont(const QDomElement &font);
        static void setFontAttributes(QFont *font, const QString& name);

    private:
        TFonts() {};    // Must never be called

        static QList<PRIVFONTS_t> mLocalFonts;
        static bool mInitialized;
};

#endif // TFONTS_H
