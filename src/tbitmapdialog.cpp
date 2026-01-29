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
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>

#include "tbitmapdialog.h"
#include "ui_tbitmapdialog.h"
#include "twidgetcombo.h"
#include "tbitmapselectdialog.h"
#include "telementbitmapselector.h"
#include "tconfmain.h"
#include "terror.h"

using namespace ObjHandler;

TBitmapDialog::TBitmapDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TBitmapDialog)
{
    DECL_TRACER("TBitmapDialog::TBitmapDialog(QWidget *parent)");

    ui->setupUi(this);
}

TBitmapDialog::~TBitmapDialog()
{
    DECL_TRACER("TBitmapDialog::~TBitmapDialog()");

    delete ui;
}

void TBitmapDialog::setBitmaps(const QList<BITMAPS_t>& bm)
{
    DECL_TRACER("TBitmapDialog::setBitmaps(const QList<BITMAPS_t>& bm)");

    ui->treeWidgetBitmaps->clear();
    ui->treeWidgetBitmaps->setColumnCount(1);
    ui->treeWidgetBitmaps->setHeaderHidden(true);

    int number = qMin(bm.size(), 5);

    for (int i = 0; i < number; ++i)
    {
        // Top item
        QTreeWidgetItem *top = new QTreeWidgetItem(ui->treeWidgetBitmaps);
        top->setText(i, QString("Bitmap %1").arg(i+1));
        // Item for the table
        QTreeWidgetItem *item = new QTreeWidgetItem(top);
        item->setFirstColumnSpanned(true);
        item->setFlags(Qt::ItemIsEnabled);
        // Add the table
        QTableWidget *table = createTable(bm[i]);
        ui->treeWidgetBitmaps->setItemWidget(item, 0, table);
    }

    ui->treeWidgetBitmaps->expandAll();
}

void TBitmapDialog::on_pushButtonAdd_clicked()
{
    DECL_TRACER("TBitmapDialog::on_pushButtonAdd_clicked()");

    TBitmapSelectDialog sDialog(this);
    sDialog.setTemporaryPath(TConfMain::Current().getPathTemporary());

    if (sDialog.exec() == QDialog::Rejected)
        return;

    QList<ObjHandler::BITMAPS_t> sel = sDialog.getSelected();

    if (sel.empty())
        return;

    for (ObjHandler::BITMAPS_t bm : sel)
    {
        if (mRowCount >= 5)
        {
            ui->pushButtonAdd->setDisabled(true);
            ui->pushButtonDelete->setDisabled(true);
            ui->pushButtonMoveUp->setDisabled(true);
            ui->pushButtonMoveDown->setDisabled(true);
            break;
        }

        QTreeWidgetItem *top = new QTreeWidgetItem(ui->treeWidgetBitmaps);
        top->setText(0, tr("Bitmap %1").arg(mRowCount+1));

        QTreeWidgetItem *item = new QTreeWidgetItem(top);
        item->setFirstColumnSpanned(true);
        item->setFlags(Qt::ItemIsEnabled);      // Not selectable

        QTableWidget *widget = createTable(bm);
        ui->treeWidgetBitmaps->setItemWidget(item, 0, widget);

        mBitmaps.append(bm);
        mRowCount++;
    }

    ui->treeWidgetBitmaps->expandAll();
}


void TBitmapDialog::on_pushButtonDelete_clicked()
{

}


void TBitmapDialog::on_pushButtonMoveUp_clicked()
{

}


void TBitmapDialog::on_pushButtonMoveDown_clicked()
{

}


void TBitmapDialog::on_pushButton_clicked()
{

}

QTableWidget *TBitmapDialog::createTable(const BITMAPS_t& bm)
{
    DECL_TRACER("TBitmapDialog::createTable(const BITMAPS_t& bm)");

    QBrush brush;
    brush.setColor(Qt::GlobalColor::lightGray);

    QTableWidget *table = new QTableWidget(this);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setVisible(false);
    table->setShowGrid(true);
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table->setColumnCount(2);

    int rows = 2;

    if (bm.justification == ObjHandler::ORI_ABSOLUT)
        rows = 4;

    // The row count must be defined before rows are added to the table! Otherwise
    // an empty table with only the grid will be visible.
    table->setRowCount(rows);

    for (int row = 0; row < rows; ++row)
    {
        QTableWidgetItem *col0 = new QTableWidgetItem;
        col0->setBackground(brush);
        col0->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);
        table->setItem(row, 0, col0);

        switch(row)
        {
            case 0:
                col0->setText(tr("Bitmap"));
                table->setCellWidget(row, 1, makeBitmapSelector(bm.fileName, QString("Bitmap_%1").arg(row)));
            break;

            case 1:
                col0->setText(tr("Bitmap Justification"));
                table->setCellWidget(row, 1, makeTextJustification(bm.justification, QString("Orientation_%1").arg(row)));
            break;

            case 2:
                col0->setText(tr("Bitmap X Offset"));
                table->setCellWidget(row, 1, makeValueSelector(bm.offsetX, QString("XOffset_%1").arg(row)));
            break;

            case 3:
                col0->setText(tr("Bitmap Y Offset"));
                table->setCellWidget(row, 1, makeValueSelector(bm.offsetX, QString("YOffset_%1").arg(row)));
            break;
        }
    }

    return table;
}

TElementBitmapSelector *TBitmapDialog::makeBitmapSelector(const QString& bitmap, const QString& name)
{
    DECL_TRACER("TBitmapDialog::makeBitmapSelector(const QString& bitmap, const QString& name)");

    ObjHandler::BITMAPS_t bm;
    bm.fileName = bitmap;
    return new TElementBitmapSelector(name, bm);
}

TWidgetCombo *TBitmapDialog::makeTextJustification(ORIENTATION ori, const QString& name)
{
    DECL_TRACER("TBitmapDialog::makeTextJustification(ObjHandler::ORIENTATION ori, const QString& name)");

    TWidgetCombo *cbox = new TWidgetCombo;
    cbox->setObjectName(name);
    QList<QString> items = { "absolute", "top-left", "top-middle", "top-right",
                            "center-left", "center-middle", "center-right",
                            "bottom-left", "bottom-middle", "bottom-right" };
    QList<QVariant> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    cbox->addItems(items);
    cbox->addData(data);
    cbox->setCurrentIndex(ori);
    // Add code to set data
    return cbox;
}

QSpinBox *TBitmapDialog::makeValueSelector(int value, const QString& name)
{
    DECL_TRACER("TBitmapDialog::makeValueSelector(int value, const QString& name)");

    QSpinBox *spin = new QSpinBox;
    spin->setObjectName(name);
    spin->setValue(value);

    connect(spin, &QSpinBox::valueChanged, [this, spin](int value) { setValue(spin->objectName(), value); });
    return spin;
}

void TBitmapDialog::setBitmap(QLineEdit *line)
{
    DECL_TRACER("TBitmapDialog::setBitmap(QLineEdit *line)");

    TBitmapSelectDialog sDialog(this);
    sDialog.setTemporaryPath(TConfMain::Current().getPathTemporary());

    if (sDialog.exec() == QDialog::Rejected)
        return;

    QList<ObjHandler::BITMAPS_t> sel = sDialog.getSelected();

    if (sel.empty())
        return;

    line->setText(sel[0].fileName);
}

void TBitmapDialog::setValue(const QString& name, const QVariant& value)
{
    DECL_TRACER("TBitmapDialog::setValue(const QString& name, const QVariant& value)");

}

