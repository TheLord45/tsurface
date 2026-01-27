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
#include <QChar>

#include "ttextboxdialog.h"
#include "ui_ttextboxdialog.h"
#include "tcharactermapdialog.h"
#include "terror.h"
#include "tmisc.h"

TTextBoxDialog::TTextBoxDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TTextBoxDialog)
{
    DECL_TRACER("TTextBoxDialog::TTextBoxDialog(QWidget *parent)");

    ui->setupUi(this);
    ui->textEditText->setLineWrapMode(QTextEdit::WidgetWidth);
    ui->textEditText->setAcceptRichText(false);
    mDefaultFont = ui->textEditText->font();
    mDefaultFont.setPointSize(10);
}

TTextBoxDialog::~TTextBoxDialog()
{
    DECL_TRACER("TTextBoxDialog::~TTextBoxDialog()");

    delete ui;
}

void TTextBoxDialog::setTextFont(const QString& text, const QFont& font)
{
    DECL_TRACER("TTextBoxDialog::setTextFont(const QString& text, const QFont& font)");

    ui->textEditText->setText(text);
    ui->textEditText->setFont(font);
    mFont = font;

    if (mFont.pointSize() < 8)
        mFont.setPointSize(10);

    ui->checkBoxPreview->setCheckState(Qt::Checked);
}

void TTextBoxDialog::on_textEditText_textChanged()
{
    mText = ui->textEditText->toPlainText();
    QString unicode;

    for (QChar ch : mText)
        unicode.append(wcharToUnicodeString(ch)+" ");

    ui->plainTextEditUnicode->setPlainText(unicode);
}

void TTextBoxDialog::on_checkBoxPreview_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TTextBoxDialog::on_checkBoxPreview_checkStateChanged(const Qt::CheckState &arg1)");

    if (arg1 == Qt::Checked)
        ui->textEditText->setFont(mFont);
    else
        ui->textEditText->setFont(mDefaultFont);
}

void TTextBoxDialog::on_pushButtonCharacterMap_clicked()
{
    DECL_TRACER("TTextBoxDialog::on_pushButtonCharacterMap_clicked()");

    TCharacterMapDialog mapDialog(this);
    mapDialog.setFont(mFont);

    if (mapDialog.exec() == QDialog::Rejected)
        return;

    ui->textEditText->insertPlainText(mapDialog.getActualCharacter());
}

