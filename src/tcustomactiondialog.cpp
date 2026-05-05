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
#include <QMessageBox>

#include "tcustomactiondialog.h"
#include "ui_tcustomactiondialog.h"
#include "terror.h"

TCustomActionDialog::TCustomActionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TCustomActionDialog)
{
    ui->setupUi(this);
}

TCustomActionDialog::~TCustomActionDialog()
{
    delete ui;
}

void TCustomActionDialog::on_spinBoxID_valueChanged(int arg1)
{
    DECL_TRACER("TCustomActionDialog::on_spinBoxID_valueChanged(int arg1)");

    mFunc.ID = arg1;
}


void TCustomActionDialog::on_spinBoxFlag_valueChanged(int arg1)
{
    DECL_TRACER("TCustomActionDialog::on_spinBoxFlag_valueChanged(int arg1)");

    mFunc.flag = arg1;
}


void TCustomActionDialog::on_spinBoxType_valueChanged(int arg1)
{
    DECL_TRACER("TCustomActionDialog::on_spinBoxType_valueChanged(int arg1)");

    mFunc.type = arg1;
}


void TCustomActionDialog::on_lineEditValue1_textEdited(const QString &arg1)
{
    DECL_TRACER("TCustomActionDialog::on_lineEditValue1_textEdited(const QString &arg1)");

    mFunc.value1 = arg1;
}


void TCustomActionDialog::on_lineEditValue2_textEdited(const QString &arg1)
{
    DECL_TRACER("TCustomActionDialog::on_lineEditValue2_textEdited(const QString &arg1)");

    mFunc.value2 = arg1;
}


void TCustomActionDialog::on_lineEditValue3_textEdited(const QString &arg1)
{
    DECL_TRACER("TCustomActionDialog::on_lineEditValue3_textEdited(const QString &arg1)");

    mFunc.value3 = arg1;
}


void TCustomActionDialog::on_lineEditText_textEdited(const QString &arg1)
{
    DECL_TRACER("TCustomActionDialog::on_lineEditText_textEdited(const QString &arg1)");

    mFunc.text = arg1;
}


void TCustomActionDialog::on_lineEditEncode_textEdited(const QString &arg1)
{
    DECL_TRACER("TCustomActionDialog::on_lineEditEncode_textEdited(const QString &arg1)");

    mFunc.encode = arg1;
}

void TCustomActionDialog::accept()
{
    DECL_TRACER("TCustomActionDialog::accept()");

    QString title = tr("Wrong Value");
    QString hint = tr("You must enter a value between 1 and 65535!");

    if (mFunc.type < 1 || mFunc.type > 65535)
    {
        QMessageBox::critical(this, title, hint);
        ui->spinBoxType->setFocus();
        return;
    }

    if (mFunc.ID < 1 || mFunc.ID > 65535)
    {
        QMessageBox::critical(this, title, hint);
        ui->spinBoxID->setFocus();
        return;
    }

    done(QDialog::Accepted);
}

ObjHandler::PUSH_FUNC_T TCustomActionDialog::getFunc() const
{
    DECL_TRACER("TCustomActionDialog::getFunc()");

    return mFunc;
}

void TCustomActionDialog::setFunc(const ObjHandler::PUSH_FUNC_T &newFunc)
{
    DECL_TRACER("TCustomActionDialog::setFunc(const ObjHandler::PUSH_FUNC_T &newFunc)");

    mFunc = newFunc;
    ui->spinBoxID->setValue(mFunc.ID);
    ui->spinBoxFlag->setValue(mFunc.flag);
    ui->spinBoxType->setValue(mFunc.type);
    ui->lineEditValue1->setText(mFunc.value1);
    ui->lineEditValue2->setText(mFunc.value2);
    ui->lineEditValue3->setText(mFunc.value3);
    ui->lineEditText->setText(mFunc.text);
    ui->lineEditEncode->setText(mFunc.encode);
}
