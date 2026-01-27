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
#include "twidgetcombo.h"
#include "terror.h"

TWidgetCombo::TWidgetCombo(QWidget *parent)
    : QComboBox(parent)
{
    DECL_TRACER("TWidgetCombo::TWidgetCombo(QWidget *parent)");

    connect(this, &QComboBox::currentTextChanged, this, &TWidgetCombo::onCurrentTextChanged);
    connect(this, &QComboBox::currentIndexChanged, this, &TWidgetCombo::onCurrentIndexChanged);
}

void TWidgetCombo::addData(const QList<QVariant>& data)
{
    DECL_TRACER("TWidgetCombo::addData(const QList<QVariant>& data)");

    if (data.size() < count())
        return;

    int idx = 0;

    for (QVariant d : data)
    {
        setItemData(idx, d);
        idx++;
    }

    mHaveData = true;
}

void TWidgetCombo::setDefaultText(const QString& text)
{
    DECL_TRACER("TWidgetCombo::setDefaultText(const QString& text)");

    int idx = findText(text);

    if (idx >= 0)
    {
        setCurrentIndex(idx);
        return;
    }

    insertItem(0, text);
    setCurrentIndex(0);
}

void TWidgetCombo::setDefaultText(const QString& text, const QVariant& data)
{
    DECL_TRACER("TWidgetCombo::setDefaultText(const QString& text, const QVariant& data)");

    int idx = findText(text);

    if (idx >= 0)
    {
        setCurrentIndex(idx);
        setItemData(idx, data);
        return;
    }

    insertItem(0, text, data);
    setCurrentIndex(0);
}

void TWidgetCombo::onCurrentTextChanged(const QString &text)
{
    DECL_TRACER("TWidgetCombo::onCurrentTextChanged(const QString &text)");

    mActText = text;
    int idx = findText(text);

    if (idx >= 0)
        mActData = itemData(idx);
    else
        mActData = itemData(0);

    emit selectionChanged(text, mActData);
}

void TWidgetCombo::onCurrentIndexChanged(int index)
{
    DECL_TRACER("TWidgetCombo::onCurrentIndexChanged(int index)");

    mActText = itemText(index);
    mActData = itemData(index);
    emit selectionChanged(mActText, mActData);
}
