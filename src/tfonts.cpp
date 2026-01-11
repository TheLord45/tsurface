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
#include <fontconfig/fontconfig.h>

#include <QFontDatabase>
#include <QStandardPaths>
#include <QFont>
#include <QFontInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>

#include "tfonts.h"
#include "terror.h"
#include "tmisc.h"

namespace fs = std::filesystem;
using std::string;

QList<TFonts::PRIVFONTS_t> TFonts::mLocalFonts;
bool TFonts::mInitialized{false};

FcConfig *gFontConfig{nullptr};

void TFonts::init()
{
    DECL_TRACER("TFonts::init()");

    if (mInitialized)
        return;

    gFontConfig = FcInitLoadConfigAndFonts();
}

/**
 * @brief TFonts::getFontFile
 * The method uses the function from the library "fontconfig" to get the file
 * name of a font from it's name. By name the family name is meant. If there
 * was a valid font passed, it returns the full file name of the font.
 *
 * @param qfont     The class containing the wanted font
 *
 * @return On success the file name is returned which was used for the font.
 * If the font is not valid, an empty string is returned.
 */
QString TFonts::getFontFile(const QFont& qfont)
{
    DECL_TRACER("TFonts::getFontFile(const QFont& font)");

    QFontInfo info(qfont);

    init();
    QString fontFile;
    // configure the search pattern,
    // assume "info.family()" is a string with the desired font name in it
    FcPattern* pat = FcNameParse((const FcChar8*)(info.family().toStdString().c_str()));
    FcConfigSubstitute(gFontConfig, pat, FcMatchPattern);
    FcDefaultSubstitute(pat);
    // find the font
    FcResult res;
    FcPattern *font = FcFontMatch(gFontConfig, pat, &res);

    if (font)
    {
        FcChar8 *file = NULL;

        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
            fontFile = (char*)file;

        FcPatternDestroy(font);
    }

    FcPatternDestroy(pat);
    MSG_DEBUG("Font file of font \"" << info.family().toStdString() << "\" is: " << fontFile.toStdString());
    return fontFile;
}

/**
 * @brief TFonts::releaseFontConfig
 * This method released the allocated memory of the library fontconfig. Once
 * a font file was requested (look at getFontFile()) it allocates memory. When
 * this is not used any more, the resources should be freed by calling this
 * method.
 */
void TFonts::releaseFontConfig()
{
    DECL_TRACER("TFonts::releaseFontConfig()");

    if (mInitialized)
    {
        FcFini();
        gFontConfig = nullptr;
        mInitialized = false;
    }
}

/**
 * @brief TFonts::addFont
 * The method adds a font together with the file from where it was taken into
 * an internal structure. The method checks whether the file name is already in
 * the internal structure. If yes, nothing happens.
 *
 * @param font      The Qt font
 * @param file      The file containing the font
 */
void TFonts::addFont(const QFont& font, const QString& file)
{
    DECL_TRACER("TFonts::addFont(const QFont& font, const QString& file)");

    // Look in the table if the string is not already in the list.
    QList<PRIVFONTS_t>::Iterator iter;

    for (iter = mLocalFonts.begin(); iter != mLocalFonts.end(); ++iter)
    {
        if (iter->file == file)
            return;
    }

    if (fs::exists(file.toStdString()))
    {
        PRIVFONTS_t pf;
        pf.file = basename(file);
        pf.intFile = file;
        pf.ID = -1;
        pf.family = font.families();
        mLocalFonts.append(pf);
    }
}

/**
 * @brief TFonts::addFontFile
 * This method adds a font file to the internal structure and the font database
 * of Qt. The internal structure stores also the ID of the font in the database,
 * so that it can be found later easily.
 * The method checks whether the file name is already in the internal structure.
 * If yes, nothing happens.
 * This method should be called only for fonts not yet in the database. After
 * calling this method, the new font is available and can be used.
 *
 * @param file  A valid file name of a font file. If this is an invalid or not
 * supported file, it is ignored.
 */
void TFonts::addFontFile(const QString& file)
{
    DECL_TRACER("TFonts::addFontFile(const QString& file)");

    // Look in the table if the string is not already in the list.
    QList<PRIVFONTS_t>::Iterator iter;

    for (iter = mLocalFonts.begin(); iter != mLocalFonts.end(); ++iter)
    {
        if (iter->file == file)
            return;
    }

    if (fs::exists(file.toStdString()))
    {
        int id = QFontDatabase::addApplicationFont(file);

        if (id < 0)
            return;

        PRIVFONTS_t pf;
        pf.file = basename(file);
        pf.intFile = file;
        pf.ID = id;
        pf.family = QFontDatabase::applicationFontFamilies(id);
        mLocalFonts.append(pf);
    }
}

/**
 * @brief TFonts::freePrivateFonts
 * This method removes all explitely added fonts from the font database. A font
 * can explicittely be added by calling addFontFile().
 * All standard fonts in the database remain there.
 */
void TFonts::freePrivateFonts()
{
    DECL_TRACER("TFonts::freePrivateFonts()");

    if (mLocalFonts.empty())
        return;

    QList<PRIVFONTS_t>::Iterator iter;

    for (iter = mLocalFonts.begin(); iter != mLocalFonts.end(); ++iter)
    {
        if (iter->ID >= 0)
            QFontDatabase::removeApplicationFont(iter->ID);
    }

    mLocalFonts.clear();
}

/**
 * @brief TFonts::readFontFile
 * The method opens the JSON file containing the information about the fonts
 * used in the project. It reads the file and loads all fonts into the internal
 * font database.
 *
 * @param path  The temporary path to the directory containing all configuration
 * files.
 *
 * @return On success TRUE is returned.
 */
bool TFonts::readFontFile(const QString& path, const QString& qfile)
{
    DECL_TRACER("TFonts::readFontFile(const QString& path, const QString& qfile)");

    QString fFile = path + "/" + qfile;
    QFile fonts(fFile);

    if (!fonts.exists() || !fs::is_regular_file(fFile.toStdString()))
    {
        MSG_ERROR("The font file " << fFile.toStdString() << " was not found!");
        return false;
    }

    if (!fonts.open(QIODevice::ReadOnly))
    {
        MSG_ERROR("Error reading file \"" << fFile.toStdString() << "\": " << fonts.errorString().toStdString());
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(fonts.readAll());
    fonts.close();

    QJsonObject root = doc.object();
    QJsonArray fontList = root.value("fontList").toArray();

    for (int i = 0; i < fontList.count(); ++i)
    {
        QJsonObject entry = fontList[i].toObject();
        PRIVFONTS_t prvFont;
        prvFont.file = entry.value("file").toString();
        prvFont.ID = -1;
        QJsonArray families = entry.value("families").toArray();

        for (int j = 0; j < families.count(); ++j)
            prvFont.family.append(families[j].toString());

        // In any case we're getting the font file name. If this method does
        // not return a file name or returns another file name, then the font
        // in the local directory is not installed on the machine.
        prvFont.intFile = getFontFile(QFont(prvFont.family));

        // If we got no or a different font file, we must load the local font
        // into the font database.
        if (basename(prvFont.intFile) != prvFont.file)
        {
            QString srcFile = path + "/fonts/" + prvFont.file;
            prvFont.ID = QFontDatabase::addApplicationFont(srcFile);
            MSG_DEBUG("Added font file \"" << srcFile.toStdString() << "\" to font database.");
        }

        mLocalFonts.append(prvFont);
    }

    readSystemFonts(path);
    return true;
}

/**
 * @brief TFonts::writeFontFile
 * This method iterates through the internal font structure and creates the file
 * \b path. It stores all the vailable fonts into this database and copies all
 * fonts into the directory where they belong.
 *
 * @param path  The path to the temporary directory.
 *
 * @return On success TRUE is returned.
 */
bool TFonts::writeFontFile(const QString& path, const QString& qfile)
{
    DECL_TRACER("TFonts::writeFontFile(const QString& path, const QString& qfile)");

    if (mLocalFonts.empty())
        return true;

    if (!fs::is_directory(path.toStdString()))
    {
        MSG_ERROR("The path \"" << path.toStdString() << "\" is not a directory!");
        return false;
    }

    QString fontFile = path + "/" + qfile;
    QString fontTargets = path + "/fonts";

    QJsonObject root;
    QJsonArray fontList;
    QList<PRIVFONTS_t>::Iterator iter;

    for (iter = mLocalFonts.begin(); iter != mLocalFonts.end(); ++iter)
    {
        QJsonObject entry;
        QJsonArray families;
        entry.insert("file", iter->file);
        QStringList::Iterator famIter;

        for (famIter = iter->family.begin(); famIter != iter->family.end(); ++famIter)
            families.append(*famIter);

        entry.insert("families", families);
        fontList.append(entry);
        // Copy font file
        QFile ffile(iter->intFile);
        QString target = fontTargets + "/" + basename(iter->file);

        if (!ffile.copy(target))
        {
            MSG_ERROR("Couldn't copy font file from \"" << iter->intFile.toStdString() << "\" to \"" << target.toStdString() << "\"");
        }
    }

    root.insert("fontList", fontList);

    QJsonDocument doc;
    doc.setObject(root);
    QFile file(fontFile);

    if(!file.open(QIODevice::WriteOnly))
    {
        MSG_ERROR("Error opening file \"" << fontFile.toStdString() << "\" for writing!");
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool TFonts::readSystemFonts(const QString& path)
{
    DECL_TRACER("TFonts::readSystemFonts(const QString& path)");

    if (path.isEmpty() || !fs::is_directory(path.toStdString()))
        return false;

    // "path" is the base path of the temporary directory!
    QString dirSysFonts = path + "/__system/graphics/fonts";
    QDir dir(dirSysFonts);
    QFileInfoList list = dir.entryInfoList();
    QFileInfoList::Iterator iter;

    for (iter = list.begin(); iter != list.end(); ++iter)
    {
        if (iter->fileName().startsWith("."))
            continue;

        int id = QFontDatabase::addApplicationFont(iter->absoluteFilePath());
        MSG_DEBUG("Added system font file \"" << iter->fileName().toStdString() << "\" to font database with ID " << id);
    }

    return true;
}
