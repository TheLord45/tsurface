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

#include "tconvertcolors.h"
#include "terror.h"

QList<COLOR_t> TConvertColors::mColors;

QColor TConvertColors::getColorFromIndex(int idx)
{
    DECL_TRACER("TConvertColors::getColorFromIndex(int idx)");

    for (COLOR_t col : mColors)
    {
        if (col.index == idx)
            return col.color;
    }

    return Qt::transparent;
}

QColor TConvertColors::getColorFromName(const QString& name)
{
    DECL_TRACER("TConvertColors::getColorFromName(const QString& name)");

    for (COLOR_t col : mColors)
    {
        if (col.name == name)
            return col.color;
    }

    return Qt::transparent;
}

QColor TConvertColors::getColorFromColor(const QString& name)
{
    DECL_TRACER("TConvertColors::getColorFromColor(const QString& name)");

    if (name.startsWith("#"))
    {
        if (name.length() == 9)
        {
            QColor color(name.left(7));
            color.setAlpha(name.right(2).toInt(nullptr, 16));
            return color;
        }

        return QColor(name);
    }

    return getColorFromName(name);
}

bool TConvertColors::readAMXPalette(const QString& filename)
{
    DECL_TRACER("TConvertColors::readAMXPalette(const QString& filename)");

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
    {
        MSG_ERROR("Failed to open file:" << filename.toStdString() << ". Error: " << file.errorString().toStdString());
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

    QDomElement paletteData = root.firstChildElement("paletteData");
    QString paletteName = paletteData.attribute("name");
    MSG_DEBUG("Parsing palette " << paletteName.toStdString());

    if (paletteData.isNull())
    {
        MSG_ERROR("No <paletteData> element found");
        return false;
    }

    QDomNodeList colors = paletteData.elementsByTagName("color");

    for (int i = 0; i < colors.count(); ++i)
    {
        QDomElement color = colors.at(i).toElement();
        parseColor(color);
    }

    return true;
}

void TConvertColors::parseColor(const QDomElement& color)
{
    COLOR_t col;
    col.index = color.attribute("index").toInt();
    col.name = color.attribute("name");
    QString sCol = color.firstChildElement().text();

    if (sCol.length() == 9)
    {
        col.color = QColor(sCol.left(7));
        col.color.setAlpha(sCol.right(2).toInt(nullptr, 16));
    }
    else
        col.color = QColor(sCol);

    // Check if the color is not already in the list
    QList<COLOR_t>::Iterator iter;

    for (iter = mColors.begin(); iter != mColors.end(); ++iter)
    {
        if (iter->name == col.name)
        {
            iter->color = col.color;
            return;
        }
    }

    mColors.append(col);
}
