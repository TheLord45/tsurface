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

#include "tdynamicimagedialog.h"
#include "ui_tdynamicimagedialog.h"
#include "terror.h"

TDynamicImageDialog::TDynamicImageDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TDynamicImageDialog)
{
    ui->setupUi(this);
    mProtocol = "http";
}

TDynamicImageDialog::~TDynamicImageDialog()
{
    delete ui;
}

void TDynamicImageDialog::on_lineEditName_textChanged(const QString &arg1)
{
    mName = arg1;
}


void TDynamicImageDialog::on_comboBox_currentIndexChanged(int index)
{
    DECL_TRACER("TDynamicImageDialog::on_comboBox_currentIndexChanged(int index)");

    switch(index)
    {
        case 0: mProtocol = "http"; break;
        case 1: mProtocol = "https"; break;
        case 2: mProtocol = "rtp"; break;
        case 3: mProtocol = "rtsp"; break;
    }
}


void TDynamicImageDialog::on_lineEditHost_textChanged(const QString &arg1)
{
    mHost = arg1;
}


void TDynamicImageDialog::on_lineEditPath_textChanged(const QString &arg1)
{
    mPath = arg1;
}


void TDynamicImageDialog::on_lineEditFile_textChanged(const QString &arg1)
{
    mFile = arg1;
}


void TDynamicImageDialog::on_lineEditUser_textChanged(const QString &arg1)
{
    mUser = arg1;
}


void TDynamicImageDialog::on_lineEditPassword_textChanged(const QString &arg1)
{
    mPassword = arg1;
}


void TDynamicImageDialog::on_spinBoxRefreshRate_valueChanged(int arg1)
{
    DECL_TRACER("TDynamicImageDialog::on_spinBoxRefreshRate_valueChanged(int arg1)");

    mRefreshRate = arg1;

    if (arg1 > 0)
        ui->checkBoxRefreshStart->setDisabled(true);
    else
        ui->checkBoxRefreshStart->setDisabled(false);
}


void TDynamicImageDialog::on_checkBoxRefreshStart_clicked(bool checked)
{
    DECL_TRACER("TDynamicImageDialog::on_checkBoxRefreshStart_clicked(bool checked)");

    mRefreshStart = checked;
}

void TDynamicImageDialog::accept()
{
    DECL_TRACER("TDynamicImageDialog::accept()");

    if (mName.isEmpty())
    {
        QMessageBox::warning(this, tr("Missing data"), tr("The resource must have a name!"));
        return;
    }

    if (mHost.isEmpty())
    {
        QMessageBox::warning(this, tr("Missing data"), tr("Specify the host either by name or the IP address!"));
        return;
    }

    if (mFile.isEmpty())
    {
        QMessageBox::warning(this, tr("Missing data"), tr("Resource file cannot be empty!"));
        return;
    }

    done(QDialog::Accepted);
}

void TDynamicImageDialog::setName(const QString& name)
{
    DECL_TRACER("TDynamicImageDialog::setName(const QString& name)");

    mName = name;
    ui->lineEditName->setText(name);
}

void TDynamicImageDialog::setProtocol(const QString& proto)
{
    DECL_TRACER("TDynamicImageDialog::setProtocol(const QString& proto)");

    if (proto.compare("HTTP", Qt::CaseInsensitive) == 0)
        ui->comboBox->setCurrentIndex(0);
    else if (proto.compare("HTTPS", Qt::CaseInsensitive) == 0)
        ui->comboBox->setCurrentIndex(1);
    else if (proto.compare("RTP", Qt::CaseInsensitive) == 0)
        ui->comboBox->setCurrentIndex(2);
    else if (proto.compare("RTSP", Qt::CaseInsensitive) == 0)
        ui->comboBox->setCurrentIndex(3);
}

void TDynamicImageDialog::setHost(const QString& host)
{
    DECL_TRACER("TDynamicImageDialog::setHost(const QString& host)");

    mHost = host;
    ui->lineEditHost->setText(host);
}

void TDynamicImageDialog::setPath(const QString& path)
{
    DECL_TRACER("TDynamicImageDialog::setPath(const QString& path)");

    mPath = path;
    ui->lineEditPath->setText(path);
}

void TDynamicImageDialog::setFile(const QString& file)
{
    DECL_TRACER("TDynamicImageDialog::setFile(const QString& file)");

    mFile = file;
    ui->lineEditFile->setText(file);
}

void TDynamicImageDialog::setUser(const QString& user)
{
    DECL_TRACER("TDynamicImageDialog::setUser(const QString& user)");

    mUser = user;
    ui->lineEditUser->setText(user);
}

void TDynamicImageDialog::setPassword(const QString& pass)
{
    DECL_TRACER("TDynamicImageDialog::setPassword(const QString& pass)");

    mPassword = pass;
    ui->lineEditPassword->setText(pass);
}

void TDynamicImageDialog::setRefreshRate(int rr)
{
    DECL_TRACER("TDynamicImageDialog::setRefreshRate(int rr)");

    mRefreshRate = rr;
    ui->spinBoxRefreshRate->setValue(rr);

    if (rr > 0)
        ui->checkBoxRefreshStart->setDisabled(true);
    else
        ui->checkBoxRefreshStart->setDisabled(false);
}

void TDynamicImageDialog::setRefreshStart(bool start)
{
    DECL_TRACER("TDynamicImageDialog::setRefreshStart(bool start)");

    mRefreshStart = start;
    ui->checkBoxRefreshStart->setCheckState(start ? Qt::Checked : Qt::Unchecked);
}

void TDynamicImageDialog::setEdit(bool state)
{
    DECL_TRACER("TDynamicImageDialog::setEdit(bool state)");

    if (state)
        ui->lineEditName->setDisabled(true);
    else
        ui->lineEditName->setDisabled(false);
}
