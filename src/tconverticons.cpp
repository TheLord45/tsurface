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
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>

#include "tconverticons.h"
#include "terror.h"

TConvertIcons::TConvertIcons()
{
    DECL_TRACER("TConvertIcons::TConvertIcons()");
}

TConvertIcons::~TConvertIcons()
{
    DECL_TRACER("TConvertIcons::~TConvertIcons()");
}

TConvertIcons::TConvertIcons(const QString& filename)
{
    DECL_TRACER("TConvertIcons::TConvertIcons(const QString& filename)");

    readAMXIcons(filename);
}

bool TConvertIcons::readAMXIcons(const QString& filename)
{
    DECL_TRACER("TConvertIcons::readAMXIcons(const QString& filename)");

    QFile file(filename);

    if (!file.open(QFile::ReadOnly))
    {
        MSG_ERROR("Error opening file " << filename.toStdString() << "! Error: " << file.errorString().toStdString());
        return false;
    }

    QDomDocument doc;
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

    QDomElement iconList = root.firstChildElement("iconList");

    if (iconList.isNull())
    {
        MSG_ERROR("No <iconList> element found");
        return false;
    }

    mMaxIcons = iconList.firstChildElement("maxIcons").text().toInt();
    MSG_DEBUG("Max Icons: " << mMaxIcons);

    QDomNodeList icons = iconList.elementsByTagName("icon");

    for (int i = 0; i < icons.count(); ++i)
    {
        QDomElement icon = icons.at(i).toElement();
        parseIcon(icon);
    }

    return true;
}

void TConvertIcons::parseIcon(const QDomElement &icon)
{
    ICON_t ic;
    ic.number = icon.attribute("number").toInt();
    ic.file = icon.firstChildElement("file").text();
    mIcons.append(ic);
}

QString TConvertIcons::getIcon(int ic)
{
    DECL_TRACER("TConvertIcons::getIcon(int ic)");

    if (ic > mMaxIcons)
    {
        MSG_ERROR("Invalid icon ID " << ic << "!");
        return QString();
    }

    for (const ICON_t i : mIcons)
    {
        if (i.number == ic)
            return i.file;
    }

    return QString();
}

QStringList TConvertIcons::getAllIcons()
{
    DECL_TRACER("TConvertIcons::getAllIcons()");

    QStringList list;

    for (const ICON_t icon : mIcons)
        list.append(icon.file);

    return list;
}
