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
#include <QColorDialog>
#include "tnewprojectdialog.h"
#include "ui_tnewprojectdialog.h"

#include "tpaneltypes.h"
#include "terror.h"

using std::string;
using std::vector;
using std::to_string;

TNewProjectDialog::TNewProjectDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TNewProjectDialog)
{
    DECL_TRACER("TNewProjectDialog::TNewProjectDialog(QWidget *parent)");

    ui->setupUi(this);
    ui->labelDescription->setWordWrap(true);
    mColorBackground = palette().color(QPalette::Base);
    mColorText = palette().color(QPalette::Text);
    ui->frameColorBackground->setStyleSheet("background-color: " + mColorBackground.name());
    MSG_DEBUG("Backgroud color: \"background-color: " << mColorBackground.name().toStdString() << "\"");
    ui->frameColorText->setStyleSheet("background-color: " + mColorText.name());
    MSG_DEBUG("Text color: \"background-color: " << mColorText.name().toStdString() << "\"");
    mFont = parent->font();
    mFont.setPointSize(mSize);
    MSG_DEBUG("Current font: " << mFont.family().toStdString() << " (" << mSize << ")");

    int idx = 0;
    bool set = false;
    int index = 0;

    for (int i = 6; i < 37; i += 2)
    {
        MSG_DEBUG("Setting size " << i);

        if ((mSize % 2) == 0 && i == mSize)
        {
            ui->comboBoxSize->addItem(QString::fromStdString(to_string(i)), i);
            ui->comboBoxSize->setCurrentIndex(idx);
            index = idx;
            set = true;
        }
        else if (!set && i > mSize)
        {
            ui->comboBoxSize->addItem(QString::fromStdString(to_string(mSize)), mSize);
            ui->comboBoxSize->addItem(QString::fromStdString(to_string(i)), i);
            set = true;
            index = idx;
        }
        else
            ui->comboBoxSize->addItem(QString::fromStdString(to_string(i)), i);

        idx++;
    }

    mPanelType = new TPanelType;
    on_comboBoxPanelTypes_currentIndexChanged(0);
    mInitialized = true;
    on_comboBoxSize_currentIndexChanged(index);
}

TNewProjectDialog::~TNewProjectDialog()
{
    DECL_TRACER("TNewProjectDialog::~TNewProjectDialog()");

    delete ui;
    delete mPanelType;
}

void TNewProjectDialog::init()
{
    DECL_TRACER("TNewProjectDialog::init()");


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

void TNewProjectDialog::on_toolButtonColorBackground_clicked()
{
    DECL_TRACER("TNewProjectDialog::on_toolButtonColorBackground_clicked()");

    QColorDialog color(this);
    color.setCurrentColor(mColorBackground);
    color.exec();
    mColorBackground = color.selectedColor();
    ui->frameColorBackground->setStyleSheet("background-color: " + mColorBackground.name());
}


void TNewProjectDialog::on_toolButtonColorText_clicked()
{
    DECL_TRACER("TNewProjectDialog::on_toolButtonColorText_clicked()");

    QColorDialog color(this);
    color.setCurrentColor(mColorText);
    color.exec();
    mColorText = color.selectedColor();
    ui->frameColorText->setStyleSheet("background-color: " + mColorText.name());
}


void TNewProjectDialog::on_fontComboBox_currentFontChanged(const QFont &f)
{
    DECL_TRACER("TNewProjectDialog::on_fontComboBox_currentFontChanged(const QFont &f)");

    mFont = f;
    mSize = f.pointSize();
    int max = ui->comboBoxSize->count();

    for (int i = 0; i < max; ++i)
    {
        if (ui->comboBoxSize->itemData(i).toInt() == mSize)
        {
            ui->comboBoxSize->setCurrentIndex(i);
            break;
        }
    }

    mFont.setPointSize(mSize);
    ui->labelFontSample->setFont(mFont);
    ui->labelFontSample->setText(mFont.family() + " <Lorem Ipsum>");
}


void TNewProjectDialog::on_comboBoxSize_currentIndexChanged(int index)
{
    DECL_TRACER("TNewProjectDialog::on_comboBoxSize_currentIndexChanged(int index)");

    if (!mInitialized)
        return;

    mSize = ui->comboBoxSize->itemData(index).toInt();
    mFont.setPointSize(mSize);
    ui->labelFontSample->setFont(mFont);
    ui->labelFontSample->setText(mFont.family() + " <Lorem Ipsum>");
}


void TNewProjectDialog::on_lineEditDesigner_textChanged(const QString &arg1)
{
    DECL_TRACER("TNewProjectDialog::on_lineEditDesigner_textChanged(const QString &arg1)");

    mDesigner = arg1;
}


void TNewProjectDialog::on_lineEditDealer_textChanged(const QString &arg1)
{
    DECL_TRACER("TNewProjectDialog::on_lineEditDealer_textChanged(const QString &arg1)");

    mDealer = arg1;
}


void TNewProjectDialog::on_lineEditRevision_textChanged(const QString &arg1)
{
    DECL_TRACER("TNewProjectDialog::on_lineEditRevision_textChanged(const QString &arg1)");

    mRevision = arg1;
}


void TNewProjectDialog::on_plainTextEditComments_textChanged()
{
    DECL_TRACER("TNewProjectDialog::on_plainTextEditComments_textChanged()");

    mComment = ui->plainTextEditComments->toPlainText();
}


void TNewProjectDialog::on_lineEditPageName_textChanged(const QString &arg1)
{
    DECL_TRACER("TNewProjectDialog::on_lineEditPageName_textChanged(const QString &arg1)");

    mPageName = arg1;
}

