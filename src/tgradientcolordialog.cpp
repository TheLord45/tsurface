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

    ui->tableWidgetColors->setRowCount(colors.size());
    int row = 0;

    for (QColor color : colors)
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

TGradientColorDialog::~TGradientColorDialog()
{
    DECL_TRACER("TGradientColorDialog::TGradientColorDialog(const QList<QColor>& colors, QWidget *parent)");

    delete ui;
}

QWidget *TGradientColorDialog::makeColorSelector(const QColor& color, int row)
{
    DECL_TRACER("TGradientColorDialog::makeColorSelector(const QColor& color, int row)");

    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(widget);

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
        return;
    }

    int row = selItems[0]->row();

    if (row < 0 || row >= mColors.size())
    {
        MSG_WARNING("Number of rows in table is different then number of entries in list! (" << ui->tableWidgetColors->rowCount() << " != " << mColors.size() << ")");
        return;
    }

    mColors.remove(row);
    ui->tableWidgetColors->removeRow(row);
    // Renumber column 0
    for (int i = 0; i < mColors.size(); ++i)
        ui->tableWidgetColors->item(i, 0)->setText(tr("Color %1").arg(i+1));

    ui->pushButtonDelete->setDisabled(true);
    ui->pushButtonMoveUp->setDisabled(true);
    ui->pushButtonMoveDown->setDisabled(true);
}

void TGradientColorDialog::on_pushButtonMoveUp_clicked()
{
    DECL_TRACER("TGradientColorDialog::on_pushButtonMoveUp_clicked()");

    QList<QTableWidgetItem *> selItems = ui->tableWidgetColors->selectedItems();

    if (selItems.empty())
    {
        ui->pushButtonDelete->setDisabled(true);
        ui->pushButtonMoveUp->setDisabled(true);
        ui->pushButtonMoveDown->setDisabled(true);
        return;
    }

    int selRow = selItems[0]->row();
//    ui->tableWidgetColors->t
}

void TGradientColorDialog::on_pushButtonMoveDown_clicked()
{
    DECL_TRACER("TGradientColorDialog::on_pushButtonMoveDown_clicked()");

    QList<QTableWidgetItem *> selItems = ui->tableWidgetColors->selectedItems();

    if (selItems.empty())
    {
        ui->pushButtonDelete->setDisabled(true);
        ui->pushButtonMoveUp->setDisabled(true);
        ui->pushButtonMoveDown->setDisabled(true);
        return;
    }
}

void TGradientColorDialog::on_tableWidgetColors_cellEntered(int row, int column)
{
    DECL_TRACER("TGradientColorDialog::on_tableWidgetColors_cellEntered(int row, int column)");

    QList<QTableWidgetItem *> selItems = ui->tableWidgetColors->selectedItems();

    if (!selItems.empty())
        ui->pushButtonDelete->setEnabled(true);

    if (mColors.size() > 1)
    {
        ui->pushButtonMoveUp->setEnabled(true);
        ui->pushButtonMoveDown->setEnabled(true);
    }
    else
    {
        ui->pushButtonMoveUp->setDisabled(true);
        ui->pushButtonMoveDown->setDisabled(true);
    }
}
