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
#include <QColorDialog>

#include "taddpopupdialog.h"
#include "ui_taddpopupdialog.h"
#include "tgraphics.h"
#include "terror.h"

TAddPopupDialog::TAddPopupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TAddPopupDialog)
{
    DECL_TRACER("TAddPopupDialog::TAddPopupDialog(QWidget *parent)");

    ui->setupUi(this);

    initFontSizes();
    ui->comboBoxBorder->addItem("None");
    ui->frameColorBorder->setStyleSheet("background-color: " + mColorBorder.name());
    ui->radioButtonTypeStandard->setChecked(true);
    QStringList list = TGraphics::Current().getBorderNames();
    QStringList::Iterator iter;

    for (iter = list.begin(); iter != list.end(); ++iter)
        ui->comboBoxBorder->addItem(*iter);
}

TAddPopupDialog::~TAddPopupDialog()
{
    DECL_TRACER("TAddPopupDialog::~TAddPopupDialog()");

    delete ui;
}

void TAddPopupDialog::on_lineEditPopupName_textChanged(const QString &arg1)
{
    DECL_TRACER("TAddPopupDialog::on_lineEditPopupName_textChanged(const QString &arg1)");

    mPopupName = arg1;
}

void TAddPopupDialog::on_comboBoxGroupName_currentIndexChanged(int index)
{
    DECL_TRACER("TAddPopupDialog::on_comboBoxGroupName_currentIndexChanged(int index)");

    QString group = ui->comboBoxGroupName->itemText(index);

    if (group.isEmpty() || group == "- - - - - - - -")
    {
        mGroupName.clear();
        return;
    }

    mGroupName = group;
}

void TAddPopupDialog::on_comboBoxGroupName_editTextChanged(const QString &arg1)
{
    DECL_TRACER("TAddPopupDialog::on_comboBoxGroupName_editTextChanged(const QString &arg1)");

    mGroupName = arg1;
}

void TAddPopupDialog::on_spinBoxPositionLeft_valueChanged(int arg1)
{
    DECL_TRACER("TAddPopupDialog::on_spinBoxPositionLeft_valueChanged(int arg1)");

    MSG_DEBUG("Position left: (" << arg1 << " + " << mWidth << ") < " << mPageSize.width());

    if ((arg1 + mWidth) < mPageSize.width())
        mLeft = arg1;
    else
    {
        QMessageBox::warning(this, tr("Size warning"), tr("The left point is too high! Ignoring it."));
        ui->spinBoxPositionLeft->setValue(mPageSize.width() - mWidth - 1);
        return;
    }
}

void TAddPopupDialog::on_spinBoxPositionTop_valueChanged(int arg1)
{
    DECL_TRACER("TAddPopupDialog::on_spinBoxPositionTop_valueChanged(int arg1)");

    MSG_DEBUG("Position top: (" << arg1 << " + " << mHeight << ") < " << mPageSize.height());

    if ((arg1 + mHeight) < mPageSize.height())
        mTop = arg1;
    else
    {
        QMessageBox::warning(this, tr("Size warning"), tr("The top point is too high! Ignoring it."));
        ui->spinBoxPositionTop->setValue(mPageSize.height() - mHeight - 1);
        return;
    }
}

void TAddPopupDialog::on_spinBoxSizeWidth_valueChanged(int arg1)
{
    DECL_TRACER("TAddPopupDialog::on_spinBoxSizeWidth_valueChanged(int arg1)");

    MSG_DEBUG("Size width: (" << mLeft << " + " << arg1 << ") < " << mPageSize.width());

    if ((mLeft + arg1) < mPageSize.width())
        mWidth = arg1;
    else
    {
        QMessageBox::warning(this, tr("Size warning"), tr("The width is too high! Ignoring it."));
        ui->spinBoxSizeWidth->setValue(mPageSize.width() - mLeft - 1);
        return;
    }
}

void TAddPopupDialog::on_spinBoxSizeHeight_valueChanged(int arg1)
{
    DECL_TRACER("TAddPopupDialog::on_spinBoxSizeHeight_valueChanged(int arg1)");

    MSG_DEBUG("Size height: (" << mTop << " + " << arg1 << ") < " << mPageSize.height());

    if ((mTop + arg1) < mPageSize.height())
        mHeight = arg1;
    else
    {
        QMessageBox::warning(this, tr("Size warning"), tr("The height is too high! Ignoring it."));
        ui->spinBoxSizeHeight->setValue(mPageSize.height() - mTop - 1);
        return;
    }
}

void TAddPopupDialog::on_toolButtonColorPageBackground_clicked()
{
    DECL_TRACER("TAddPopupDialog::on_toolButtonColorPageBackground_clicked()");

    QColorDialog color(this);
    color.setCurrentColor(mColorPageBackground);
    color.exec();

    if (!color.selectedColor().isValid())
        return;

    mColorPageBackground = color.selectedColor();
    ui->frameColorPageBackground->setStyleSheet(QString("background-color: %1").arg(mColorPageBackground.name()));
    ui->labelFontSampleShow->setStyleSheet(styleSheetColor());
}

void TAddPopupDialog::on_toolButtonColorBorder_clicked()
{
    DECL_TRACER("TAddPopupDialog::on_toolButtonColorBorder_clicked()");

    QColorDialog color(this);
    color.setCurrentColor(mColorBorder);
    color.exec();

    if (!color.selectedColor().isValid())
        return;

    mColorBorder = color.selectedColor();
    ui->frameColorBorder->setStyleSheet(QString("background-color: %1").arg(mColorBorder.name()));
}

void TAddPopupDialog::on_toolButtonColorText_clicked()
{
    DECL_TRACER("TAddPopupDialog::on_toolButtonColorText_clicked()");

    QColorDialog color(this);
    color.setCurrentColor(mColorText);
    color.exec();

    if (!color.selectedColor().isValid())
        return;

    mColorText = color.selectedColor();
    ui->frameColorText->setStyleSheet(QString("background-color: %1").arg(mColorText.name()));
    ui->labelFontSampleShow->setStyleSheet(styleSheetColor());
}

void TAddPopupDialog::on_radioButtonTypeStandard_toggled(bool checked)
{
    DECL_TRACER("TAddPopupDialog::on_radioButtonTypeStandard_toggled(bool checked)");

    if (checked)
        setStandard(true);
}

void TAddPopupDialog::on_radioButtonTypeSubPage_toggled(bool checked)
{
    DECL_TRACER("TAddPopupDialog::on_radioButtonTypeSubPage_toggled(bool checked)");

    if (checked)
        setStandard(false);
}

void TAddPopupDialog::on_comboBoxBorder_currentIndexChanged(int index)
{
    DECL_TRACER("TAddPopupDialog::on_comboBoxBorder_currentIndexChanged(int index)");

    QString border = ui->comboBoxBorder->itemText(index);

    if (border != "None")
        mBorder = border;
}

void TAddPopupDialog::on_fontComboBoxFontName_currentFontChanged(const QFont &f)
{
    DECL_TRACER("TAddPopupDialog::on_fontComboBoxFontName_currentFontChanged(const QFont &f)");

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

    ui->labelFontSampleShow->setFont(mFont);
}

void TAddPopupDialog::on_comboBoxFontSize_currentIndexChanged(int index)
{
    DECL_TRACER("TAddPopupDialog::on_comboBoxFontSize_currentIndexChanged(int index)");

    if (!mInitialized)
        return;

    mSize = ui->comboBoxFontSize->itemData(index).toInt();
    mFont.setPointSize(mSize);
    ui->labelFontSampleShow->setFont(mFont);
}

void TAddPopupDialog::setGroupNames(const QStringList& groups)
{
    DECL_TRACER("TAddPopupDialog::setGroupName(const QStringList& group)");

    if (groups.isEmpty())
        return;

    ui->comboBoxGroupName->addItem("- - - - - - - -");
    QStringList::ConstIterator iter;

    for (iter = groups.constBegin(); iter != groups.constEnd(); ++iter)
        ui->comboBoxGroupName->addItem(*iter);
}

void TAddPopupDialog::setColorPageBackground(const QColor& col)
{
    DECL_TRACER("TAddPopupDialog::setColorPageBackground(const QColor& col)");

    mColorPageBackground = col;
    ui->frameColorPageBackground->setStyleSheet("background-color: " + col.name());
    ui->labelFontSampleShow->setStyleSheet(styleSheetColor());
}

void TAddPopupDialog::setColorBorder(const QColor& col)
{
    DECL_TRACER("TAddPopupDialog::setColorBorder(const QColor& col)");

    mColorBorder = col;
    ui->frameColorBorder->setStyleSheet("background-color: " + col.name());
    ui->labelFontSampleShow->setStyleSheet(styleSheetColor());
}

void TAddPopupDialog::setColorText(const QColor& col)
{
    DECL_TRACER("TAddPopupDialog::setColorText(const QColor& col)");

    mColorText = col;
    ui->frameColorText->setStyleSheet("background-color: " + col.name());
    ui->labelFontSampleShow->setStyleSheet(styleSheetColor());
}

void TAddPopupDialog::setFont(const QFont& font)
{
    DECL_TRACER("TAddPopupDialog::setFont(const QFont& font)");

    mFont = font;
    mFont.setPointSize(mSize);
    ui->labelFontSampleShow->setFont(font);
    ui->fontComboBoxFontName->setCurrentFont(font);
}

void TAddPopupDialog::setFontSize(int size)
{
    DECL_TRACER("TAddPopupDialog::setFontSize(int size)");

    mSize = size;
    ui->comboBoxFontSize->clear();
    initFontSizes();
}

void TAddPopupDialog::setPageSize(const QSize& size)
{
    DECL_TRACER("TAddPopupDialog::setPageSize(const QSize& size)");

    mPageSize = size;
    ui->spinBoxPositionLeft->setRange(0, size.width());
    ui->spinBoxPositionTop->setRange(0, size.height());
    ui->spinBoxSizeWidth->setRange(0, size.width());
    ui->spinBoxSizeHeight->setRange(0, size.height());
}

QString TAddPopupDialog::styleSheetColor()
{
    DECL_TRACER("TAddPopupDialog::styleSheetColor()");

    return QString("background-color: %1;color: %2").arg(mColorPageBackground.name()).arg(mColorText.name());
}

void TAddPopupDialog::initFontSizes()
{
    DECL_TRACER("TAddPopupDialog::initFontSizes()");

    mInitialized = false;
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

void TAddPopupDialog::setStandard(bool state)
{
    DECL_TRACER("TAddPopupDialog::setStandard(bool state)");

    mStandardSub = !state;
    ui->labelGroupName->setVisible(state);
    ui->comboBoxGroupName->setVisible(state);
    ui->groupBoxPosition->setVisible(state);
}

void TAddPopupDialog::accept()
{
    DECL_TRACER("TAddPopupDialog::accept()");

    if (mPopupName.isEmpty())
    {
        QMessageBox::critical(this, tr("Missing entries"), tr("The popup must have a name!"));
        return;
    }

    if (mWidth <= 0 || mHeight <= 0)
    {
        QMessageBox::critical(this, tr("Missing entries"), tr("The popup has no or illegal size!"));
        return;
    }

    done(QDialog::Accepted);
}
