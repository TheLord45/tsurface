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
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>

#include "tgradientcolordialog.h"
#include "ui_tgradientcolordialog.h"
#include "terror.h"

TGradientColorDialog::TGradientColorDialog(const QList<QColor>& colors, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TGradientColorDialog),
      mColors(colors)
{
    DECL_TRACER("TGradientColorDialog::TGradientColorDialog(const QList<QColor>& colors, QWidget *parent)");

    ui->setupUi(this);

    ui->pushButtonDelete->setDisabled(true);
    ui->pushButtonMoveUp->setDisabled(true);
    ui->pushButtonMoveDown->setDisabled(true);

    createTable();
}

TGradientColorDialog::~TGradientColorDialog()
{
    DECL_TRACER("TGradientColorDialog::~TGradientColorDialog()");

    delete ui;
}

void TGradientColorDialog::createTable()
{
    DECL_TRACER("TGradientColorDialog::createTable()");

    ui->tableWidgetColors->setRowCount(mColors.size());
    int row = 0;

    for (QColor color : mColors)
    {
        QTableWidgetItem *col0 = new QTableWidgetItem;
        col0->setBackground(Qt::lightGray);
        col0->setText(tr("Color %1").arg(row+1));
        //        col0->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);
        ui->tableWidgetColors->setItem(row, 0, col0);
        ui->tableWidgetColors->setCellWidget(row, 1, makeColorSelector(color, row));
        row++;
    }

    ui->tableWidgetColors->resizeRowsToContents();
    ui->tableWidgetColors->resizeColumnsToContents();
}

QWidget *TGradientColorDialog::makeColorSelector(const QColor& color, int row)
{
    DECL_TRACER("TGradientColorDialog::makeColorSelector(const QColor& color, int row)");

    QWidget *widget = new QWidget(ui->tableWidgetColors);
    widget->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    QLabel *colLabel = new QLabel;
    colLabel->setFixedWidth(35);
    colLabel->setStyleSheet(QString("background-color: %1").arg(color.name(QColor::HexArgb)));
    layout->addWidget(colLabel);

    QLabel *txtLabel = new QLabel;
    txtLabel->setText(color.name(QColor::HexArgb));
    layout->addWidget(txtLabel);

    layout->addStretch();

    QPushButton *button = new QPushButton("...");
    button->setFixedWidth(30);
    layout->addWidget(button);

    connect(button, &QPushButton::clicked, [this, color, row] {
        QColorDialog dialog(color, this);

        if (dialog.exec() == QDialog::Rejected)
            return;

        QColor col = dialog.selectedColor();

        if (row >= 0 && row < mColors.size())
            mColors[row] = col;

        updateRow(col, row);
    });

    return widget;
}

void TGradientColorDialog::updateRow(const QColor& color, int row)
{
    DECL_TRACER("TGradientColorDialog::updateRow(const QColor& color, int row)");

    ui->tableWidgetColors->setCellWidget(row, 1, makeColorSelector(color, row));
}

void TGradientColorDialog::deselectAll()
{
    DECL_TRACER("TGradientColorDialog::deselctAll()");

    QList<QTableWidgetSelectionRange> list = ui->tableWidgetColors->selectedRanges();

    if (list.empty())
        return;

    for (QTableWidgetSelectionRange sel : list)
        ui->tableWidgetColors->setRangeSelected(sel, false);

    mSelectedRow = -1;
    ui->pushButtonDelete->setDisabled(true);
    ui->pushButtonMoveUp->setDisabled(true);
    ui->pushButtonMoveDown->setDisabled(true);
}

void TGradientColorDialog::on_pushButtonAdd_clicked()
{
    DECL_TRACER("TGradientColorDialog::on_pushButtonAdd_clicked()");

    QColorDialog dialog(this);

    if (dialog.exec() == QDialog::Rejected)
        return;

    QColor col = dialog.selectedColor();
    mColors.append(col);
    int row = mColors.size() - 1;

    ui->tableWidgetColors->setRowCount(mColors.size());
    QTableWidgetItem *col0 = new QTableWidgetItem;
    col0->setBackground(Qt::lightGray);
    col0->setText(tr("Color %1").arg(row+1));
    ui->tableWidgetColors->setItem(row, 0, col0);
    ui->tableWidgetColors->setCellWidget(row, 1, makeColorSelector(col, row));
    deselectAll();
}

void TGradientColorDialog::on_pushButtonDelete_clicked()
{
    DECL_TRACER("TGradientColorDialog::on_pushButtonDelete_clicked()");

    QList<QTableWidgetItem *> selItems = ui->tableWidgetColors->selectedItems();

    if (selItems.empty())
    {
        ui->pushButtonDelete->setDisabled(true);
        ui->pushButtonMoveUp->setDisabled(true);
        ui->pushButtonMoveDown->setDisabled(true);
        MSG_DEBUG("No line selected");
        return;
    }

    int row = selItems[0]->row();

    if (row < 0 || row >= mColors.size())
    {
        MSG_WARNING("Number of rows in table is different then number of entries in list! (" << ui->tableWidgetColors->rowCount() << " != " << mColors.size() << ")");
        return;
    }

    mColors.remove(row);
    ui->tableWidgetColors->clear();
    createTable();

    deselectAll();
}

void TGradientColorDialog::on_pushButtonMoveUp_clicked()
{
    DECL_TRACER("TGradientColorDialog::on_pushButtonMoveUp_clicked()");

    if (mSelectedRow < 0 || mSelectedRow >= mColors.size())
    {
        ui->pushButtonDelete->setDisabled(true);
        ui->pushButtonMoveUp->setDisabled(true);
        ui->pushButtonMoveDown->setDisabled(true);
        MSG_DEBUG("No line selected");
        return;
    }

    if (mSelectedRow == 0)
    {
        ui->pushButtonMoveUp->setDisabled(true);
        ui->pushButtonMoveDown->setEnabled(true);
        return;
    }

    QTableWidgetItem *col1 = ui->tableWidgetColors->takeItem(mSelectedRow, 1);
    QTableWidgetItem *col2 = ui->tableWidgetColors->takeItem(mSelectedRow-1, 1);

    ui->tableWidgetColors->setItem(mSelectedRow, 1, col2);
    ui->tableWidgetColors->setItem(mSelectedRow-1, 1, col1);

    deselectAll();
}

void TGradientColorDialog::on_pushButtonMoveDown_clicked()
{
    DECL_TRACER("TGradientColorDialog::on_pushButtonMoveDown_clicked()");

    if (mSelectedRow < 0 || mSelectedRow >= mColors.size())
    {
        ui->pushButtonDelete->setDisabled(true);
        ui->pushButtonMoveUp->setDisabled(true);
        ui->pushButtonMoveDown->setDisabled(true);
        MSG_DEBUG("No line selected");
        return;
    }

    if (mSelectedRow >= mColors.size())
    {
        ui->pushButtonMoveUp->setEnabled(true);
        ui->pushButtonMoveDown->setDisabled(true);
        return;
    }

    QTableWidgetItem *col1 = ui->tableWidgetColors->takeItem(mSelectedRow, 1);
    QTableWidgetItem *col2 = ui->tableWidgetColors->takeItem(mSelectedRow-1, 1);

    ui->tableWidgetColors->setItem(mSelectedRow, 1, col2);
    ui->tableWidgetColors->setItem(mSelectedRow-1, 1, col1);

    deselectAll();
}

void TGradientColorDialog::on_tableWidgetColors_cellActivated(int row, int column)
{
    DECL_TRACER("TGradientColorDialog::on_tableWidgetColors_cellActivated(int row, int column)");

    Q_UNUSED(column);
    mSelectedRow = row;
    ui->tableWidgetColors->selectRow(row);

    ui->pushButtonDelete->setEnabled(true);

    if (mColors.size() > 1)
    {
        if (row > 0)
            ui->pushButtonMoveUp->setEnabled(true);
        else
            ui->pushButtonMoveUp->setDisabled(true);

        if (row < (ui->tableWidgetColors->rowCount()-1))
            ui->pushButtonMoveDown->setEnabled(true);
        else
            ui->pushButtonMoveDown->setDisabled(true);
    }
    else
    {
        ui->pushButtonMoveUp->setDisabled(true);
        ui->pushButtonMoveDown->setDisabled(true);
    }
}

