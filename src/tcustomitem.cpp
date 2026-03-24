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
#include "tcustomitem.h"
#include "terror.h"

TCustomItem::TCustomItem()
{
    DECL_TRACER("TCustomItem::TCustomItem()");
}

TCustomItem::TCustomItem(const QString &text)
    : QStandardItem(text)
{
    DECL_TRACER("TCustomItem::TCustomItem(const QString &text)");
}

TCustomItem::TCustomItem(const QIcon &icon, const QString &text)
    : QStandardItem(icon, text)
{
    DECL_TRACER("TCustomItem::TCustomItem(const QIcon &icon, const QString &text)");
}

TCustomItem::TCustomItem(int rows, int columns)
    : QStandardItem(rows, columns)
{
    DECL_TRACER("TCustomItem::TCustomItem(int rows, int columns)");
}

TCustomItem::~TCustomItem()
{
    DECL_TRACER("TCustomItem::~TCustomItem()");
}

bool TCustomItem::operator<(const QStandardItem &other) const
{
    QString name = text();
    QString otherName = other.text();

    if (hasChildren() && !other.hasChildren())
        return false;

    if (!hasChildren() && other.hasChildren())
        return true;

    if ((hasChildren() && other.hasChildren()) ||
        !hasChildren() && !other.hasChildren())
    {
        if (name.compare(otherName) > 0)
            return false;
    }

    return true;
}
