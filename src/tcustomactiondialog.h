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
#ifndef TCUSTOMACTIONDIALOG_H
#define TCUSTOMACTIONDIALOG_H

#include <QDialog>

#include "tobjecthandler.h"

namespace Ui {
    class TCustomActionDialog;
}

class TCustomActionDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit TCustomActionDialog(QWidget *parent = nullptr);
        ~TCustomActionDialog();

        ObjHandler::PUSH_FUNC_T getFunc() const;
        void setFunc(const ObjHandler::PUSH_FUNC_T &newFunc);

    protected:
        virtual void accept() override;

    private slots:
        void on_spinBoxID_valueChanged(int arg1);
        void on_spinBoxFlag_valueChanged(int arg1);
        void on_spinBoxType_valueChanged(int arg1);
        void on_lineEditValue1_textEdited(const QString &arg1);
        void on_lineEditValue2_textEdited(const QString &arg1);
        void on_lineEditValue3_textEdited(const QString &arg1);
        void on_lineEditText_textEdited(const QString &arg1);
        void on_lineEditEncode_textEdited(const QString &arg1);

    private:
        Ui::TCustomActionDialog *ui;
        ObjHandler::PUSH_FUNC_T mFunc;
};

#endif // TCUSTOMACTIONDIALOG_H
