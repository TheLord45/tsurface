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
#include <QSpinBox>

#include "telementspinbox.h"
#include "terror.h"

TElementSpinBox::TElementSpinBox(int value, const QString& name, QWidget * parent)
    : QWidget(parent),
      mName(name),
      mValue(value)
{
    DECL_TRACER("TElementSpinBox::TElementSpinBox(int value, const QString& name, QWidget * parent)");

    init();
}

TElementSpinBox::TElementSpinBox(int value, int min, int max, const QString& name, QWidget *parent)
    : QWidget(parent),
      mName(name),
      mValue(value),
      mMinValue(min),
      mMaxValue(max)
{
    DECL_TRACER("TElementSpinBox::TElementSpinBox(int value, int min, int max, const QString& name, QWidget *parent)");

    init();
}

TElementSpinBox::~TElementSpinBox()
{
    DECL_TRACER("TElementSpinBox::~TElementSpinBox()");
}

void TElementSpinBox::init()
{
    DECL_TRACER("TElementSpinBox::init()");

    mSpinBox = new QSpinBox(this);
    mSpinBox->setRange(mMinValue, mMaxValue);
    mSpinBox->setValue(mValue);
    connect(mSpinBox, &QSpinBox::valueChanged, this, &TElementSpinBox::onValueChanged);
}

void TElementSpinBox::setValue(int value)
{
    DECL_TRACER("TElementSpinBox::setValue(int value)");

    mValue = value;

    if (mSpinBox)
        mSpinBox->setValue(value);
}

void TElementSpinBox::setMinValue(int value)
{
    DECL_TRACER("TElementSpinBox::setMinValue(int value)");

    mMinValue = value;

    if (mSpinBox)
        mSpinBox->setMinimum(value);
}

void TElementSpinBox::setMaxValue(int value)
{
    DECL_TRACER("TElementSpinBox::setMaxValue(int value)");

    mMaxValue = value;

    if (mSpinBox)
        mSpinBox->setMaximum(value);
}

void TElementSpinBox::setMinMaxValue(int min, int max)
{
    DECL_TRACER("TElementSpinBox::setMinMaxValue(int min, int max)");

    mMinValue = min;
    mMaxValue = max;

    if (mSpinBox)
        mSpinBox->setRange(min, max);
}

void TElementSpinBox::onValueChanged(int value)
{
    DECL_TRACER("TElementSpinBox::onValueChanged(int value)");

    mValue = value;
    emit valueChanged(value, mName);
}
