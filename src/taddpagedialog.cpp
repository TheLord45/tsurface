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
#include <QColorDialog>

#include "taddpagedialog.h"
#include "ui_taddpagedialog.h"

#include "terror.h"

TAddPageDialog::TAddPageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TAddPageDialog)
{
    DECL_TRACER("TAddPageDialog::TAddPageDialog(QWidget *parent)");

    ui->setupUi(this);

    for (int i = 6; i < 37; i += 2)
        ui->comboBoxFontSize->addItem(QString("%1").arg(i));
}

TAddPageDialog::~TAddPageDialog()
{
    DECL_TRACER("TAddPageDialog::~TAddPageDialog()");

    delete ui;
}

void TAddPageDialog::setFont(const QFont& font)
{
    DECL_TRACER("TAddPageDialog::setFont(const QFont& font)");

    ui->labelFontSample->setFont(font);
    ui->fontComboBoxFontName->setCurrentFont(font);
    mSize = font.pointSize();

    if ((mSize % 2) != 0 || mSize < 6 || mSize > 36)
    {
        if (mSize < 6)
            ui->comboBoxFontSize->insertItem(0, QString("%1").arg(mSize), mSize);
        else if (mSize > 36)
            ui->comboBoxFontSize->addItem(QString("%1").arg(mSize), mSize);
        else
        {
            for (int i = 0; i < ui->comboBoxFontSize->count(); ++i)
            {
                int size = ui->comboBoxFontSize->itemData(i).toInt();

                if (size > mSize)
                {
                    ui->comboBoxFontSize->insertItem(i - 1, QString("%1").arg(mSize), mSize);
                    break;
                }
            }
        }
    }
}

void TAddPageDialog::setColorBackground(const QColor& col)
{
    DECL_TRACER("TAddPageDialog::setColorBackground(const QColor& col)");

    ui->frameColorBackground->setStyleSheet(QString("background-color: %1").arg(col.name()));
}

void TAddPageDialog::setColorText(const QColor& col)
{
    DECL_TRACER("TAddPageDialog::setColorText(const QColor& col)");

    ui->frameColorText->setStyleSheet((QString("background-color: %1").arg(col.name())));
}

void TAddPageDialog::on_lineEditPageName_textChanged(const QString &arg1)
{
    DECL_TRACER("TAddPageDialog::on_lineEditPageName_textChanged(const QString &arg1)");

    mPageName = arg1;
}

void TAddPageDialog::on_toolButtonColorBackground_clicked()
{
    DECL_TRACER("TAddPageDialog::on_toolButtonColorBackground_clicked()");

    QColorDialog color(this);
    color.setCurrentColor(mColorBackground);
    color.exec();
    mColorBackground = color.selectedColor();
    ui->frameColorBackground->setStyleSheet((QString("background-color: %1").arg(mColorBackground.name())));
    ui->labelFontSample->setStyleSheet(QString("background-color: %1").arg(mColorBackground.name()));
}

void TAddPageDialog::on_toolButtonColorText_clicked()
{
    DECL_TRACER("TAddPageDialog::on_toolButtonColorText_clicked()");

    QColorDialog color(this);
    color.setCurrentColor(mColorText);
    color.exec();
    mColorText = color.selectedColor();
    ui->frameColorText->setStyleSheet((QString("background-color: ").arg(mColorText.name())));
    ui->labelFontSample->setStyleSheet(QString("color: %1").arg(mColorText.name()));
}

void TAddPageDialog::on_fontComboBoxFontName_currentFontChanged(const QFont& f)
{
    DECL_TRACER("TAddPageDialog::on_fontComboBoxFontName_currentFontChanged(const QFont& f)");

    mFont = f;
    mSize = f.pointSize();
    int max = ui->comboBoxFontSize->count();

    for (int i = 0; i < max; ++i)
    {
        if (ui->comboBoxFontSize->itemData(i).toInt() == mSize)
        {
            ui->comboBoxFontSize->setCurrentIndex(i);
            break;
        }
    }

    mFont.setPointSize(mSize);

}

void TAddPageDialog::on_comboBoxFontSize_currentIndexChanged(int index)
{
    DECL_TRACER("TAddPageDialog::on_comboBoxFontSize_currentIndexChanged(int index)");

    mSize = ui->comboBoxFontSize->itemData(index).toInt();
    mFont.setPointSize(mSize);
    ui->labelFontSample->setFont(mFont);
}

