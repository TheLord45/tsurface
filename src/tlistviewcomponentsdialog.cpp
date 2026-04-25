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
#include "tlistviewcomponentsdialog.h"
#include "ui_tlistviewcomponentsdialog.h"

#include "terror.h"

TListViewComponentsDialog::TListViewComponentsDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TListViewComponentsDialog)
{
    DECL_TRACER("TListViewComponentsDialog::TListViewComponentsDialog(QWidget *parent)");

    ui->setupUi(this);
}

TListViewComponentsDialog::~TListViewComponentsDialog()
{
    DECL_TRACER("TListViewComponentsDialog::~TListViewComponentsDialog()");

    delete ui;
}

void TListViewComponentsDialog::on_checkBoxPrimaryText_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TListViewComponentsDialog::on_checkBoxPrimaryText_checkStateChanged(const Qt::CheckState &arg1)");

    if (arg1 == Qt::Checked)
    {
        mPrimaryText = true;
        ui->checkBoxSecondaryText->setEnabled(true);
    }
    else
    {
        mPrimaryText = false;
        ui->checkBoxSecondaryText->setCheckState(Qt::Unchecked);
        ui->checkBoxSecondaryText->setDisabled(true);
    }
}

void TListViewComponentsDialog::on_checkBoxSecondaryText_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TListViewComponentsDialog::on_checkBoxSecondaryText_checkStateChanged(const Qt::CheckState &arg1)");

    mSecondaryText = arg1 == Qt::Checked ? true : false;
}

void TListViewComponentsDialog::on_checkBoxImage_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TListViewComponentsDialog::on_checkBoxImage_checkStateChanged(const Qt::CheckState &arg1)");

    mImage = arg1 == Qt::Checked ? true : false;
}

void TListViewComponentsDialog::accept()
{
    DECL_TRACER("TListViewComponentsDialog::accept()");

    if (!mImage && !mPrimaryText)
        return;

    done(QDialog::Accepted);
}

bool TListViewComponentsDialog::primaryText() const
{
    DECL_TRACER("TListViewComponentsDialog::primaryText()");

    return mPrimaryText;
}

void TListViewComponentsDialog::setPrimaryText(bool newPrimaryText)
{
    DECL_TRACER("TListViewComponentsDialog::setPrimaryText(bool newPrimaryText)");

    mPrimaryText = newPrimaryText;

    if (!mPrimaryText)
        ui->checkBoxSecondaryText->setDisabled(true);
    else
        ui->checkBoxSecondaryText->setEnabled(true);

    QSignalBlocker sblock(this);
    ui->checkBoxPrimaryText->setChecked(newPrimaryText);
}

bool TListViewComponentsDialog::secondaryText() const
{
    DECL_TRACER("TListViewComponentsDialog::secondaryText()");

    return mSecondaryText;
}

void TListViewComponentsDialog::setSecondaryText(bool newSecondaryText)
{
    DECL_TRACER("TListViewComponentsDialog::setSecondaryText(bool newSecondaryText)");

    mSecondaryText = newSecondaryText;
    QSignalBlocker sBlock(this);
    ui->checkBoxSecondaryText->setChecked(newSecondaryText);
}

bool TListViewComponentsDialog::image() const
{
    DECL_TRACER("TListViewComponentsDialog::image()");

    return mImage;
}

void TListViewComponentsDialog::setImage(bool newImage)
{
    DECL_TRACER("TListViewComponentsDialog::setImage(bool newImage)");

    mImage = newImage;
    QSignalBlocker sBlock(this);
    ui->checkBoxImage->setChecked(newImage);
}
