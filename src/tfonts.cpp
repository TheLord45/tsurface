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
#include <filesystem>

#include <QFontDatabase>
#include <QStandardPaths>
#include <QFont>
#include <QFontInfo>

#include "tfonts.h"
#include "terror.h"

namespace fs = std::filesystem;
using std::string;

TFonts::TFonts()
{
}

QString TFonts::getFontPath(const QString& family)
{
    DECL_TRACER("TFonts::getFontPath(const QString& family)");

    QString path = QStandardPaths::displayName(QStandardPaths::FontsLocation);
    MSG_DEBUG("Search through directory: " << path.toStdString());

    if (path.isEmpty() || !fs::is_directory(path.toStdString()))
    {
        MSG_ERROR("No font file path was found!");
        return QString();
    }

    for (const fs::directory_entry& entry : fs::recursive_directory_iterator(path.toStdString()))
    {
        const string filenameStr = entry.path().filename().string();

        if (entry.is_directory())
            continue;

        if (entry.is_regular_file())
            std::cout << "file: " << filenameStr << std::endl;
    }

    return path;
}

QString TFonts::getFontFile(const QFont& font)
{
    DECL_TRACER("TFonts::getFontFile(const QFont& font)");

    QFont myFont;
    QFontInfo info(myFont);
    QFont realFont(info.family());
//    QString rawName = realFont.rawName();
    return QString();
}
