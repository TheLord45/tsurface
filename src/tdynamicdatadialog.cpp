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
#include "tdynamicdatadialog.h"
#include "ui_tdynamicdatadialog.h"
#include "tcrypt.h"
#include "terror.h"

TDynamicDataDialog::TDynamicDataDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TDynamicDataDialog)
{
    DECL_TRACER("TDynamicDataDialog::TDynamicDataDialog(QWidget *parent)");

    ui->setupUi(this);
    ui->radioButtonXPort->setChecked(true);
    mProtocol = "http";
}

TDynamicDataDialog::~TDynamicDataDialog()
{
    DECL_TRACER("TDynamicDataDialog::~TDynamicDataDialog()");

    delete ui;
}

void TDynamicDataDialog::setName(const QString& name)
{
    DECL_TRACER("TDynamicDataDialog::setName(const QString& name)");

    mName = name;
    ui->lineEditName->setText(name);

    if (!name.isEmpty())
        ui->lineEditName->setDisabled(true);
}

void TDynamicDataDialog::setProtocol(const QString& proto)
{
    DECL_TRACER("TDynamicDataDialog::setProtocol(const QString& proto)");

    if (proto == "http" || proto == "https")
    {
        mProtocol = proto;

        if (proto == "http")
            ui->comboBoxProtocol->setCurrentIndex(0);
        else
            ui->comboBoxProtocol->setCurrentIndex(1);
    }
}

void TDynamicDataDialog::setHost(const QString& host)
{
    DECL_TRACER("TDynamicDataDialog::setHost(const QString& host)");

    mHost = host;
    ui->lineEditHost->setText(host);
}

void TDynamicDataDialog::setPath(const QString& path)
{
    DECL_TRACER("TDynamicDataDialog::setPath(const QString& path)");

    mPath = path;
    ui->lineEditPath->setText(path);
}

void TDynamicDataDialog::setFile(const QString& file)
{
    DECL_TRACER("TDynamicDataDialog::setFile(const QString& file)");

    mFile = file;
    ui->lineEditFile->setText(file);
}

void TDynamicDataDialog::setUser(const QString& user)
{
    DECL_TRACER("TDynamicDataDialog::setUser(const QString& user)");

    mUser = user;
    ui->lineEditUser->setText(user);
}

void TDynamicDataDialog::setPassword(const QString& pass)
{
    DECL_TRACER("TDynamicDataDialog::setPassword(const QString& pass)");

    TCrypt cr;
    mPassword = cr.doDecrypt(pass, cr.getPassword());
    ui->lineEditPassword->setText(mPassword);
}

QString TDynamicDataDialog::getPassword()
{
    DECL_TRACER("TDynamicDataDialog::getPassword()");

    TCrypt cr;
    return cr.doEncrypt(mPassword, cr.getPassword());
}

void TDynamicDataDialog::setRefreshRate(int rate)
{
    DECL_TRACER("TDynamicDataDialog::setRefreshRate(int rate)");

    mRefreshRate = rate;
    ui->spinBoxRefreshRate->setValue(rate);
}

void TDynamicDataDialog::setForceData(bool force)
{
    DECL_TRACER("TDynamicDataDialog::setForceData(bool force)");

    mForceData = force;
    ui->checkBoxForce->setCheckState(force ? Qt::Checked : Qt::Unchecked);
}

void TDynamicDataDialog::setFormat(FORMAT_t format)
{
    DECL_TRACER("TDynamicDataDialog::setFormat(FORMAT_t format)");

    switch(format)
    {
        case FORMAT_XPORT:   ui->radioButtonXPort->setChecked(true); break;
        case FORMAT_CSVHEAD: ui->radioButtonCsvHead->setChecked(true); break;
        case FORMAT_CSV:     ui->radioButtonCsv->setChecked(true); break;
    }
}


void TDynamicDataDialog::on_lineEditName_textChanged(const QString &arg1)
{
    mName = arg1;
}

void TDynamicDataDialog::on_comboBoxProtocol_currentIndexChanged(int index)
{
    DECL_TRACER("TDynamicDataDialog::on_comboBoxProtocol_currentIndexChanged(int index)");

    switch (index)
    {
        case 0: mProtocol = "http"; break;
        case 1: mProtocol = "https"; break;
    }
}

void TDynamicDataDialog::on_lineEditHost_textChanged(const QString &arg1)
{
    mHost = arg1;
}


void TDynamicDataDialog::on_lineEditPath_textChanged(const QString &arg1)
{
    mPath = arg1;
}


void TDynamicDataDialog::on_lineEditFile_textChanged(const QString &arg1)
{
    mFile = arg1;
}


void TDynamicDataDialog::on_lineEditUser_textChanged(const QString &arg1)
{
    mUser = arg1;
}


void TDynamicDataDialog::on_lineEditPassword_textChanged(const QString &arg1)
{
    mPassword = arg1;
}


void TDynamicDataDialog::on_spinBoxRefreshRate_valueChanged(int arg1)
{
    mRefreshRate = arg1;
}


void TDynamicDataDialog::on_checkBoxForce_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TDynamicDataDialog::on_checkBoxForce_checkStateChanged(const Qt::CheckState &arg1)");

    mForceData = arg1 == Qt::Checked ? true : false;
}


void TDynamicDataDialog::on_radioButtonXPort_clicked(bool checked)
{
    DECL_TRACER("TDynamicDataDialog::on_radioButtonXPort_clicked(bool checked)");

    if (checked)
        mFormat = FORMAT_XPORT;
}


void TDynamicDataDialog::on_radioButtonCsvHead_clicked(bool checked)
{
    DECL_TRACER("TDynamicDataDialog::on_radioButtonCsvHead_clicked(bool checked)");

    if (checked)
        mFormat = FORMAT_CSVHEAD;
}


void TDynamicDataDialog::on_radioButtonCsv_clicked(bool checked)
{
    DECL_TRACER("TDynamicDataDialog::on_radioButtonCsv_clicked(bool checked)");

    if (checked)
        mFormat = FORMAT_CSV;
}

