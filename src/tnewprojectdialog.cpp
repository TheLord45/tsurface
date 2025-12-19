/*
 * Copyright (C) 2025 by Andreas Theofilu <andreas@theosys.at>
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
#include <string>
#include <vector>

#include <QPixmap>
#include <QMessageBox>

#include "tnewprojectdialog.h"
#include "ui_tnewprojectdialog.h"

#include "tpaneltypes.h"
#include "terror.h"

using std::string;
using std::vector;

TNewProjectDialog::TNewProjectDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TNewProjectDialog)
{
    DECL_TRACER("TNewProjectDialog::TNewProjectDialog(QWidget *parent)");

    ui->setupUi(this);
    ui->labelDescription->setWordWrap(true);
    ui->pushButtonPrevious->setEnabled(false);
    mPanelType = new TPanelType;
    on_comboBoxPanelTypes_currentIndexChanged(0);
}

TNewProjectDialog::~TNewProjectDialog()
{
    DECL_TRACER("TNewProjectDialog::~TNewProjectDialog()");

    delete ui;
    delete mPanelType;
}

void TNewProjectDialog::on_lineEditJobName_textChanged(const QString &arg1)
{
    DECL_TRACER("TNewProjectDialog::on_lineEditJobName_textChanged(const QString &arg1)");

    mProjectName = arg1;
}


void TNewProjectDialog::on_comboBoxPanelTypes_currentIndexChanged(int index)
{
    DECL_TRACER("TNewProjectDialog::on_comboBoxPanelTypes_currentIndexChanged(int index)");

    mPanelName = ui->comboBoxPanelTypes->itemText(index);
    mResolutions = mPanelType->getResolutions(mPanelName);
    QString image = mPanelType->getImageName(mPanelName);
    ui->comboBoxResolutions->clear();
    mResolution = mResolutions[0];

    if (mResolutions.size() > 1)
    {
        vector<QSize>::iterator iter;
        ui->comboBoxResolutions->setEnabled(true);

        for (iter = mResolutions.begin(); iter != mResolutions.end(); ++iter)
        {
            QString item = QVariant::fromValue<int>(iter->width()).toString() + "x" +
                        QVariant::fromValue<int>(iter->height()).toString();
            ui->comboBoxResolutions->addItem(item);
        }
    }
    else
    {
        QString item = QVariant::fromValue<int>(mResolutions[0].width()).toString() + "x" +
                       QVariant::fromValue<int>(mResolutions[0].height()).toString();
        ui->comboBoxResolutions->addItem(item);
        ui->comboBoxResolutions->setEnabled(false);
    }

    MSG_DEBUG("Selected resolution: " << mResolution.width() << "x" << mResolution.height());
    image = ":images/" + image;
    ui->labelPanelImage->setPixmap(QPixmap(image));
    QString descr = mPanelType->getDescription(mPanelName);
    ui->labelDescription->setText(descr);

    MSG_DEBUG("Panel: " << mPanelName.toStdString());
    MSG_DEBUG("Image: " << image.toStdString());
}

void TNewProjectDialog::on_comboBoxResolutions_currentIndexChanged(int index)
{
    DECL_TRACER("TNewProjectDialog::on_comboBoxResolutions_currentIndexChanged(int index)");

    if (mResolutions.size() > index)
        mResolution = mResolutions[index];
    else
        return;

    MSG_DEBUG("Selected resolution: " << mResolution.width() << "x" << mResolution.height());
}


void TNewProjectDialog::on_checkBoxGenFiles_stateChanged(int arg1)
{
    DECL_TRACER("TNewProjectDialog::on_checkBoxGenFiles_stateChanged(int arg1)");
}


void TNewProjectDialog::on_pushButtonPrevious_clicked()
{
    DECL_TRACER("TNewProjectDialog::on_pushButtonPrevious_clicked()");
}


void TNewProjectDialog::on_pushButtonNext_clicked()
{
    DECL_TRACER("TNewProjectDialog::on_pushButtonNext_clicked()");
}


void TNewProjectDialog::on_pushButtonFinish_clicked()
{
    DECL_TRACER("TNewProjectDialog::on_pushButtonFinish_clicked()");

    if (mProjectName.isEmpty() && !ui->checkBoxGenFiles->isChecked())
    {
        QMessageBox::critical(this, tr("Error"), tr("No project name defined!"), QMessageBox::Ok);
        return;
    }

    accept();
}


void TNewProjectDialog::on_pushButtonCancel_clicked()
{
    DECL_TRACER("TNewProjectDialog::on_pushButtonCancel_clicked()");

    close();
}

