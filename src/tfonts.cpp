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
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>

#include "tfonts.h"
#include "tconfmain.h"
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

QString TFonts::getFontName(const QFont& font)
{
    DECL_TRACER("TFonts::getFontName(const QFont& font)");

    QString fntName = font.family();

    if (font.bold())
        fntName += " Bold";

    if (font.italic())
        fntName += " Italic";

    if (font.strikeOut())
        fntName += " Strike";

    if (font.underline())
        fntName += " Underline";

    return fntName;
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

void TFonts::reset()
{
    DECL_TRACER("TFonts::reset()");

    freePrivateFonts();
    mLocalFonts.clear();
}

/**
 * @brief TFonts::getFont
 * This method takes the family name of a font woth the names of the attributes
 * appended. It searches in the internal font list for the name and returnes a
 * real font with all attributes set. The size is set to the defined project
 * font size.
 * If no font could be found, the default project font will be returned.
 *
 * @param ff    Font family name with optional attributes appended.
 * @return A real font with all attributes set. If the family name was not
 * found, the standard project font and size is returned.
 */
QFont TFonts::getFont(const QString& ff)
{
    DECL_TRACER("TFonts::getFont(const QString& ff)");

    QFont font;
    QString baseName = getFontBaseName(ff);

    if (ff.isEmpty())
    {
        font = TConfMain::Current().getFontBase();
        font.setPointSize(TConfMain::Current().getFontBaseSize());
        return font;
    }

    // Look in the table if the string is not already in the list.
    QList<PRIVFONTS_t>::Iterator iter;

    for (iter = mLocalFonts.begin(); iter != mLocalFonts.end(); ++iter)
    {
        if (iter->file == baseName)
        {
            font.setFamilies(iter->family);
            setFontAttributes(&font, ff);
            font.setPointSize(iter->size > 0 ? iter->size : TConfMain::Current().getFontBaseSize());
            return font;
        }
    }

    font.setFamily(ff);
    font.setPointSize(TConfMain::Current().getFontBaseSize());
    setFontAttributes(&font, ff);

    return font;
}

/**
 * @brief TFonts::getFontBaseName
 * This method takes the family name of a font and looks for some trailing
 * keywords in it. If there are any, they are removed from the name. Then the
 * remaining name is returned.
 *
 * @param ff    Font family with attributes appended.
 * @return In case there were attributes with the name, the attributes are
 * removed and the remaining name is returned. If there are no attributes in the
 * name, then the unchanged family name is returned.
 */
QString TFonts::getFontBaseName(const QString& ff)
{
    DECL_TRACER("TFonts::getFontBaseName(const QString& ff)");

    QList<int> index;
    int pos1 = ff.lastIndexOf(" bold", 0, Qt::CaseInsensitive);
    int pos2 = ff.lastIndexOf(" italic", 0, Qt::CaseInsensitive);
    int pos3 = ff.lastIndexOf(" strike", 0, Qt::CaseInsensitive);
    int pos4 = ff.lastIndexOf(" underline", 0, Qt::CaseInsensitive);

    if(pos1 > 0)
        index.append(pos1);

    if (pos2 > 0)
        index.append(pos2);

    if (pos3 > 0)
        index.append(pos3);

    if (pos4 > 0)
        index.append(pos4);

    if (index.empty())
        return ff;

    int pos = 0xffffffff;

    for (int p : index)
    {
        if (p < pos)
            pos = p;
    }

    return ff.left(pos);
}

/**
 * @brief TFonts::getFontFromIndex
 * If the project was loaded from a G4 file (TP4), this method searches for the
 * font index in the internal table. If found, the font is returned as a font
 * with all attributes set.
 *
 * This method returns guaranteed a useable font.
 *
 * @param index     The font index.
 *
 * @return In case the font index was found, the font with all attributes
 * including the size is returned. Otherwise the standard font set in the
 * project definitions is returned.
 */
QFont TFonts::getFontFromIndex(int index)
{
    DECL_TRACER("TFonts::getFontFromIndex(int index)");

    QFont font;
    QList<PRIVFONTS_t>::Iterator iter;

    for (iter = mLocalFonts.begin(); iter != mLocalFonts.end(); ++iter)
    {
        if (iter->fi == index)
        {
            font.setFamilies(iter->family);

            if (TConfMain::Current().isAMX() && !TConfMain::Current().isG5())
            {
                font.setPointSize(iter->size);

                if (!iter->subfamilyName.isEmpty())
                {
                    if (iter->subfamilyName.contains("Bold", Qt::CaseInsensitive))
                        font.setBold(true);

                    if (iter->subfamilyName.contains("Italic", Qt::CaseInsensitive))
                        font.setItalic(true);

                    if (iter->subfamilyName.contains("Strike", Qt::CaseInsensitive))
                        font.setStrikeOut(true);

                    if (iter->subfamilyName.contains("Underline", Qt::CaseInsensitive))
                        font.setUnderline(true);
                }
            }
            else
                font.setPointSize(TConfMain::Current().getFontBaseSize());

            return font;
        }
    }

    font = TConfMain::Current().getFontBase();
    font.setPointSize(TConfMain::Current().getFontBaseSize());
    return font;
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
        pf.bold = font.bold();
        pf.italic = font.italic();
        pf.strike = font.strikeOut();
        pf.underline = font.underline();
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
 * @brief TFonts::addFontFamily
 * The method take a family name and extracts the base name out of it. Then it
 * adds the font to the list. In case the font is already on the list, nothing
 * happens.
 *
 * @param family    The family name of a font.
 */
void TFonts::addFontFamily(const QString& family)
{
    DECL_TRACER("TFonts::addFontFamily(const QString& family)");

    if (family.isEmpty())
        return;

    QString baseName = getFontBaseName(family);
    QList<PRIVFONTS_t>::Iterator iter;
    // Look for the font in the table. If it exists, do nothing.
    for (iter = mLocalFonts.begin(); iter != mLocalFonts.end(); ++iter)
    {
        QStringList::Iterator famIter;

        for (famIter = iter->family.begin(); famIter != iter->family.end(); ++famIter)
        {
            if (*famIter == baseName)
                return;
        }
    }

    QFont font(baseName);

    PRIVFONTS_t pv;
    pv.intFile = getFontFile(font);
    pv.file = basename(pv.intFile);
    pv.family.append(baseName);
    mLocalFonts.append(pv);
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

        prvFont.ID = loadFont(path, prvFont.file, prvFont.family);
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

        if (!fs::exists(target.toStdString()) && !ffile.copy(target))
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

bool TFonts::readAMXFontFile(const QString& xmlFilePath)
{
    DECL_TRACER("TFonts::readAMXFontFile(const QString& file)");

    QFile file(xmlFilePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        MSG_ERROR("Failed to open file:" << xmlFilePath.toStdString() << ". Error: " << file.errorString().toStdString());
        return false;
    }

    QDomDocument doc;
    QString errorMsg;
    QDomDocument::ParseResult result;

    if (!(result = doc.setContent(&file)))
    {
        MSG_ERROR("Failed to parse XML: " << result.errorMessage.toStdString() << " at line " << result.errorLine << " column " << result.errorColumn);
        file.close();
        return false;
    }

    file.close();

    QDomElement root = doc.documentElement();

    if (root.tagName() != "root")
    {
        MSG_ERROR("Unexpected root element:" << root.tagName().toStdString());
        return false;
    }

    QDomElement fontList = root.firstChildElement("fontList");

    if (fontList.isNull())
    {
        MSG_ERROR("No <fontList> element found");
        return false;
    }

    QDomNodeList fonts = fontList.elementsByTagName("font");

    for (int i = 0; i < fonts.count(); ++i)
    {
        QDomElement font = fonts.at(i).toElement();
        parseFont(font);
    }

    // Delete the original file ...
    file.remove();
    // and save it as a JSON file.
    writeFontFile(pathname(xmlFilePath), "fonts_.json");
    TConfMain::Current().setFontFileName("fonts_.json");

    return true;
}

void TFonts::parseFont(const QDomElement &font)
{
//    DECL_TRACER("TFonts::parseFont(const QDomElement &font)");

    PRIVFONTS_t fnt;
    QString path = TConfMain::Current().getPathTemporary();

    if (font.hasAttribute("number"))
        fnt.fi = font.attribute("number").toInt();
    else
        fnt.fi = mLocalFonts.size();

    fnt.file = font.firstChildElement("file").text();
    fnt.intFile = path + "/fonts/" + fnt.file;
    fnt.family.append(font.firstChildElement("fullName").text());
    fnt.usageCount = font.firstChildElement("usageCount").text().toInt();
    // The rest is coming only from G4 files.
    if (!font.firstChildElement("fileSize").isNull())
        fnt.fileSize = font.firstChildElement("fileSize").text().toInt();

    if (!font.firstChildElement("faceIndex").isNull())
    {
        if (font.firstChildElement("faceIndex").text().toInt() > 0)
            fnt.ID = font.firstChildElement("faceIndex").text().toInt();
    }

    if (!font.firstChildElement("name").isNull())
        fnt.name = font.firstChildElement("name").text();

    if (!font.firstChildElement("subfamilyName").isNull())
        fnt.subfamilyName = font.firstChildElement("subfamilyName").text();

    if (!font.firstChildElement("size").isNull())
        fnt.size = font.firstChildElement("size").text().toInt();

    fnt.ID = loadFont(path, fnt.file, fnt.family);
    mLocalFonts.append(fnt);
}

void TFonts::setFontAttributes(QFont *font, const QString& name)
{
    DECL_TRACER("TFonts::setFontAttributes(QFont *font, const QString& name)");

    if (!font)
        return;

    if (name.contains(" bold",Qt::CaseInsensitive))
        font->setBold(true);

    if (name.contains(" italic", Qt::CaseInsensitive))
        font->setItalic(true);

    if (name.contains(" strike", Qt::CaseInsensitive))
        font->setStrikeOut(true);

    if (name.contains(" underline", Qt::CaseInsensitive))
        font->setUnderline(true);
}

int TFonts::loadFont(const QString& path, const QString& file, const QStringList& families)
{
    DECL_TRACER("TFonts::loadFont(const QString& path, const QString& file, const QStringList& families)");

    int ID = -1;
    // In any case we're getting the font file name. If this method does
    // not return a file name or returns another file name, then the font
    // in the local directory is not installed on the machine.
    QString sysFile = getFontFile(QFont(families));
    // If we got no or a different font file, we must load the local font
    // into the font database.
    if (basename(sysFile) != file)
    {
        QString srcFile = path + "/fonts/" + file;
        ID = QFontDatabase::addApplicationFont(srcFile);
        MSG_DEBUG("Added font file \"" << srcFile.toStdString() << "\" to font database.");
    }

    return ID;
}
