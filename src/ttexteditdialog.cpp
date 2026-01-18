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
#include "ttexteditdialog.h"
#include "ui_ttexteditdialog.h"
#include "terror.h"

TTextEditDialog::TTextEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TTextEditDialog)
{
    DECL_TRACER("TTextEditDialog::TTextEditDialog(QWidget *parent)");

    ui->setupUi(this);
}

TTextEditDialog::~TTextEditDialog()
{
    delete ui;
}

void TTextEditDialog::on_plainTextEditDescription_textChanged()
{
    mText = ui->plainTextEditDescription->toPlainText();
}

void TTextEditDialog::setCaption(const QString& text)
{
    DECL_TRACER("TTextEditDialog::setCaption(const QString& text)");

    ui->labelDescription->setText(text);
}

void TTextEditDialog::setDescription(const QString& text)
{
    DECL_TRACER("TTextEditDialog::setDescription(const QString& text)");

    mText = text;
    ui->plainTextEditDescription->setPlainText(text);
}
