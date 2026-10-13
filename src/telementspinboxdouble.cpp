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
// SPDX-FileCopyrightText: 2026 Andreas Theofilu <andreas@theosys.at>
// SPDX-License-Identifier: GPL-3.0-or-later
#include <QDoubleSpinBox>

#include "telementspinboxdouble.h"
#include "terror.h"

TElementSpinBoxDouble::TElementSpinBoxDouble(double value, const QString& name, QWidget *parent)
    : QWidget(parent),
      mName(name),
      mValue(value)
{
    DECL_TRACER("TElementSpinBoxDouble::TElementSpinBoxDouble(double value, const QString& name, QWidget *parent)");

    init();
}

TElementSpinBoxDouble::TElementSpinBoxDouble(double value, double min, double max, const QString& name, QWidget *parent)
    : QWidget(parent),
      mName(name),
      mValue(value),
      mMinValue(min),
      mMaxValue(max)
{
    DECL_TRACER("TElementSpinBoxDouble::TElementSpinBoxDouble(double value, double min, double max, const QString& name, QWidget *parent)");

    init();
}

TElementSpinBoxDouble::~TElementSpinBoxDouble()
{
    DECL_TRACER("TElementSpinBoxDouble::~TElementSpinBoxDouble()");
}

void TElementSpinBoxDouble::init()
{
    DECL_TRACER("TElementSpinBoxDouble::init()");

    mSpinBox = new QDoubleSpinBox(this);
    mSpinBox->setRange(mMinValue, mMaxValue);
    mSpinBox->setValue(mValue);
    mSpinBox->setDecimals(mPrec);
    mSpinBox->setSingleStep(1.0);
    connect(mSpinBox, &QDoubleSpinBox::valueChanged, this, &TElementSpinBoxDouble::onValueChanged);
    setFocusProxy(mSpinBox);
}

void TElementSpinBoxDouble::setValue(double value)
{
    DECL_TRACER("TElementSpinBoxDouble::setValue(double value)");

    if (mValue == value)
        return;

    mValue = value;
    QSignalBlocker block(mSpinBox);

    if (mSpinBox)
        mSpinBox->setValue(value);
}

void TElementSpinBoxDouble::setMinValue(double value)
{
    DECL_TRACER("TElementSpinBoxDouble::setMinValue(double value)");

    QSignalBlocker sigBlock(mSpinBox);
    mMinValue = value;

    if (mSpinBox)
        mSpinBox->setMinimum(value);
}

void TElementSpinBoxDouble::setMaxValue(double value)
{
    DECL_TRACER("TElementSpinBoxDouble::setMaxValue(double value)");

    QSignalBlocker sigBlock(mSpinBox);
    mMaxValue = value;

    if (mSpinBox)
        mSpinBox->setMaximum(value);
}

void TElementSpinBoxDouble::setMinMaxValue(double min, double max)
{
    DECL_TRACER("TElementSpinBoxDouble::setMinMaxValue(double min, double max)");

    mMinValue = min;
    mMaxValue = max;

    QSignalBlocker sigBlock(mSpinBox);

    if (mSpinBox)
        mSpinBox->setRange(min, max);
}

void TElementSpinBoxDouble::setDecimals(int prec)
{
    DECL_TRACER("TElementSpinBoxDouble::setDecimals(int prec)");

    if (mPrec < 1 && mPrec > 10)
        return;

    mPrec = prec;

    if (mSpinBox)
        mSpinBox->setDecimals(mPrec);
}
void TElementSpinBoxDouble::onValueChanged(double value)
{
    DECL_TRACER("TElementSpinBoxDouble::onValueChanged(double value)");

    if (mValue == value)
        return;

    mValue = value;
    emit valueChanged(value, mName);
    emit valueChangedInst(value, mName, mInstance);
}
