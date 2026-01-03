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
#include <QMessageBox>

#include "taddpagedialog.h"
#include "ui_taddpagedialog.h"

#include "terror.h"

TAddPageDialog::TAddPageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TAddPageDialog)
{
    DECL_TRACER("TAddPageDialog::TAddPageDialog(QWidget *parent)");

    ui->setupUi(this);

    QList<int> fSizes = QFontDatabase::pointSizes(mFont.family());
    QList<int>::Iterator iter;
    bool haveSize = false;
    int idx = 0;
    int index = 0;

    for (iter = fSizes.begin(); iter != fSizes.end(); ++iter)
    {
        ui->comboBoxFontSize->addItem(QString("%1").arg(*iter), *iter);

        if (*iter == mSize)
        {
            haveSize = true;
            ui->comboBoxFontSize->setCurrentIndex(idx);
            index = idx;
        }
        else if (!haveSize && *iter > mSize)
        {
            if (idx > 0)
            {
                ui->comboBoxFontSize->insertItem(idx - 1, QString("%1").arg(mSize), mSize);
                index = idx - 1;
            }
            else
            {
                ui->comboBoxFontSize->insertItem(0, QString("%1").arg(mSize), mSize);
                index = 0;
            }

            haveSize = true;
        }

        idx++;
    }

    if (!haveSize)
    {
        ui->comboBoxFontSize->addItem(QString("%1").arg(mSize), mSize);
        index = idx;
    }

    mInitialized = true;
    on_comboBoxFontSize_currentIndexChanged(index);
}

TAddPageDialog::~TAddPageDialog()
{
    DECL_TRACER("TAddPageDialog::~TAddPageDialog()");

    delete ui;
}

void TAddPageDialog::setFont(const QFont& font)
{
    DECL_TRACER("TAddPageDialog::setFont(const QFont& font)");

    mFont = font;
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

void TAddPageDialog::setFontSize(int size)
{
    DECL_TRACER("TAddPageDialog::setFontSize(int size)");

    mSize = size;

    for (int i = 0; i < ui->comboBoxFontSize->count(); ++i)
    {
        int s = ui->comboBoxFontSize->itemData(i).toInt();

        if (s == size)
        {
            ui->comboBoxFontSize->setCurrentIndex(i);
            break;
        }
    }
}

void TAddPageDialog::setColorBackground(const QColor& col)
{
    DECL_TRACER("TAddPageDialog::setColorBackground(const QColor& col)");

    mColorBackground = col;
    ui->frameColorBackground->setStyleSheet(QString("background-color: %1").arg(col.name()));
    ui->labelFontSample->setStyleSheet(styleSheetColor());
}

void TAddPageDialog::setColorText(const QColor& col)
{
    DECL_TRACER("TAddPageDialog::setColorText(const QColor& col)");

    mColorText = col;
    ui->frameColorText->setStyleSheet((QString("background-color: %1").arg(col.name())));
    ui->labelFontSample->setStyleSheet(styleSheetColor());
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

    if (!color.selectedColor().isValid())
        return;

    mColorBackground = color.selectedColor();
    ui->frameColorBackground->setStyleSheet(QString("background-color: %1").arg(mColorBackground.name()));
    ui->labelFontSample->setStyleSheet(styleSheetColor());
}

void TAddPageDialog::on_toolButtonColorText_clicked()
{
    DECL_TRACER("TAddPageDialog::on_toolButtonColorText_clicked()");

    QColorDialog color(this);
    color.setCurrentColor(mColorText);
    color.exec();

    if (!color.selectedColor().isValid())
        return;

    mColorText = color.selectedColor();
    ui->frameColorText->setStyleSheet((QString("background-color: %1").arg(mColorText.name())));
    ui->labelFontSample->setStyleSheet(styleSheetColor());
}

void TAddPageDialog::on_fontComboBoxFontName_currentFontChanged(const QFont& f)
{
    DECL_TRACER("TAddPageDialog::on_fontComboBoxFontName_currentFontChanged(const QFont& f)");

    mFont = f;
    mFont.setPointSize(mSize);
    int max = ui->comboBoxFontSize->count();

    for (int i = 0; i < max; ++i)
    {
        if (ui->comboBoxFontSize->itemData(i).toInt() == mSize)
        {
            ui->comboBoxFontSize->setCurrentIndex(i);
            break;
        }
    }

    ui->labelFontSample->setFont(mFont);
    ui->labelFontSample->setStyleSheet(styleSheetColor());
}

void TAddPageDialog::on_comboBoxFontSize_currentIndexChanged(int index)
{
    DECL_TRACER("TAddPageDialog::on_comboBoxFontSize_currentIndexChanged(int index)");

    if (!mInitialized)
        return;

    mSize = ui->comboBoxFontSize->itemData(index).toInt();
    mFont.setPointSize(mSize);
    ui->labelFontSample->setFont(mFont);
}

void TAddPageDialog::accept()
{
    DECL_TRACER("TAddPageDialog::accept()");

    if (mPageName.isEmpty())
    {
        QMessageBox::critical(this, tr("Missing name"), tr("Missing the name of the new page!"));
        return;
    }

    done(QDialog::Accepted);
}

QString TAddPageDialog::styleSheetColor()
{
    DECL_TRACER("TAddPageDialog::styleSheetColor()");

    return QString("background-color: %1;color: %2").arg(mColorBackground.name()).arg(mColorText.name());
}
