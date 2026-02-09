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
#ifndef TELEMENTSPINBOX_H
#define TELEMENTSPINBOX_H

#include <QWidget>

class QSpinBox;

class TElementSpinBox : public QWidget
{
    Q_OBJECT

    public:
        TElementSpinBox(int value, const QString& name, QWidget * parent=nullptr);
        TElementSpinBox(int value, int min, int max, const QString& name, QWidget *parent=nullptr);
        ~TElementSpinBox();

        int getValue() { return mValue; }
        QString& getName() { return mName; }

    signals:
        void valueChanged(int value, const QString& name);

    protected:
        void init();
        void onValueChanged(int value);

    private:
        QString mName;
        int mValue{0};

        QSpinBox *mSpinBox{nullptr};
};

#endif // TELEMENTSPINBOX_H
