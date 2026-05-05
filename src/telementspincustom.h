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
#ifndef TELEMENTSPINCUSTOM_H
#define TELEMENTSPINCUSTOM_H

#include <QWidget>

#include "tobjecthandler.h"

class QSpinBox;
class QPushButton;

class TElementSpinCustom : public QWidget
{
    Q_OBJECT

    public:
        TElementSpinCustom(const ObjHandler::PUSH_FUNC_T& pf, const QString& name, QWidget *parent=nullptr);

        void setFunc(const ObjHandler::PUSH_FUNC_T& pf);

    signals:
        void contentChanged(const ObjHandler::PUSH_FUNC_T& pf, const QString& name);

    protected:
        void onValueChanged(int value);
        void onButtonClicked();

    private:
        ObjHandler::PUSH_FUNC_T mFunc;
        QString mName;

        QSpinBox *mSpinBox{nullptr};
        QPushButton *mButton{nullptr};
};

#endif // TELEMENTSPINCUSTOM_H
