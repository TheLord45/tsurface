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
#include "telementwidgetcombo.h"
#include "terror.h"

TElementWidgetCombo::TElementWidgetCombo(QWidget *parent)
    : QComboBox(parent)
{
    DECL_TRACER("TElementWidgetCombo::TElementWidgetCombo(QWidget *parent)");

    init();
}

TElementWidgetCombo::TElementWidgetCombo(const QString& name, QWidget *parent)
    : QComboBox(parent),
      mName(name)
{
    DECL_TRACER("TElementWidgetCombo::TElementWidgetCombo(const QString& name, QWidget *parent)");

    if (!name.isEmpty())
        setObjectName(name);

    init();
}

TElementWidgetCombo::~TElementWidgetCombo()
{
    DECL_TRACER("TElementWidgetCombo::~TElementWidgetCombo()");
}

void TElementWidgetCombo::init()
{
    DECL_TRACER("TElementWidgetCombo::init()");

    connect(this, &QComboBox::currentTextChanged, this, &TElementWidgetCombo::onCurrentTextChanged);
    connect(this, &QComboBox::currentIndexChanged, this, &TElementWidgetCombo::onCurrentIndexChanged);
}

void TElementWidgetCombo::addData(const QList<QVariant>& data)
{
    DECL_TRACER("TElementWidgetCombo::addData(const QList<QVariant>& data)");

    if (data.size() < count())
        return;

    QSignalBlocker sigBlock(this);
    int idx = 0;

    for (QVariant d : data)
    {
        setItemData(idx, d);
        idx++;
    }

    mHaveData = true;
}

void TElementWidgetCombo::setDefaultData(const QVariant& data)
{
    DECL_TRACER("TElementWidgetCombo::setDefaultData(const QVariant& data)");

    QSignalBlocker sigBlock(this);
    int idx = findData(data);

    if (idx >= 0)
        setCurrentIndex(idx);
}

void TElementWidgetCombo::setDefaultText(const QString& text)
{
    DECL_TRACER("TElementWidgetCombo::setDefaultText(const QString& text)");

    QSignalBlocker sigBlock(this);
    int idx = findText(text);

    if (idx >= 0)
    {
        setCurrentIndex(idx);
        return;
    }

    insertItem(0, text);
    setCurrentIndex(0);
}

void TElementWidgetCombo::setDefaultText(const QString& text, const QVariant& data)
{
    DECL_TRACER("TElementWidgetCombo::setDefaultText(const QString& text, const QVariant& data)");

    QSignalBlocker sigBlock(this);
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

void TElementWidgetCombo::selectItem(const QString& text)
{
    DECL_TRACER("TElementWidgetCombo::selectItem(const QString& text)");

    QSignalBlocker sigBlock(this);
    int idx = findText(text);

    if (idx >= 0)
        setCurrentIndex(idx);
}

void TElementWidgetCombo::selectItem(const QVariant& data)
{
    DECL_TRACER("TElementWidgetCombo::selectItem(const QVariant& data)");

    QSignalBlocker sigBlock(this);
    int idx = findData(data);

    if (idx >= 0)
        setCurrentIndex(idx);
}

void TElementWidgetCombo::onCurrentTextChanged(const QString &text)
{
    DECL_TRACER("TElementWidgetCombo::onCurrentTextChanged(const QString &text)");

    mActText = text;
    int idx = findText(text);

    if (idx >= 0)
        mActData = itemData(idx);
    else
        mActData = itemData(0);

    emit selectionChanged(text, mActData, mName);
    emit selectionChangedInst(text, mActData, mName, mInstance);
}

void TElementWidgetCombo::onCurrentIndexChanged(int index)
{
    DECL_TRACER("TElementWidgetCombo::onCurrentIndexChanged(int index)");

    mActText = itemText(index);
    mActData = itemData(index);
    emit selectionChanged(mActText, mActData, mName);
    emit selectionChangedInst(mActText, mActData, mName, mInstance);
}
