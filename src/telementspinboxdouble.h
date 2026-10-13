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
#ifndef TELEMENTSPINBOXDOUBLE_H
#define TELEMENTSPINBOXDOUBLE_H

#include <QWidget>

class QDoubleSpinBox;

class TElementSpinBoxDouble : public QWidget
{
    Q_OBJECT

    public:
        TElementSpinBoxDouble(double value, const QString& name, QWidget *parent=nullptr);
        TElementSpinBoxDouble(double value, double min, double max, const QString& name, QWidget *parent=nullptr);
        ~TElementSpinBoxDouble();

        double getValue() { return mValue; }
        void setValue(double value);
        void setMinValue(double value);
        void setMaxValue(double value);
        void setMinMaxValue(double min, double max);
        void setDecimals(int prec);
        QString& getName() { return mName; }
        void setInstance(int inst) { mInstance = inst; }
        int getInstance() { return mInstance; }

    signals:
        void valueChanged(double value, const QString& name);
        void valueChangedInst(double value, const QString& name, int instance);

    protected:
        void init();
        void onValueChanged(double value);

    private:
        QString mName;
        double mValue{0.0};
        double mMinValue{0.0};
        double mMaxValue{1000.0};
        int mPrec{1};
        int mInstance{-1};

        QDoubleSpinBox *mSpinBox{nullptr};
};

#endif // TELEMENTSPINBOXDOUBLE_H
