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
#include "tbitmapselectdialog.h"
#include "telementbitmapselector.h"
#include "telementwidgetcombo.h"

#include "tconfmain.h"
#include "terror.h"
#include "tmisc.h"

using namespace ObjHandler;

TBitmapDialog::TBitmapDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TBitmapDialog)
{
    DECL_TRACER("TBitmapDialog::TBitmapDialog(QWidget *parent)");

    ui->setupUi(this);

    ui->pushButtonDelete->setDisabled(true);
    ui->pushButtonMoveUp->setDisabled(true);
    ui->pushButtonMoveDown->setDisabled(true);
    connect(ui->treeWidgetBitmaps, &QTreeWidget::itemActivated, this, &TBitmapDialog::onTreeWidgetItemActivated);
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
        top->setData(0, Qt::UserRole, i);
        top->setText(i, QString("Bitmap %1").arg(i+1));
        // Item for the table
        QTreeWidgetItem *item = new QTreeWidgetItem(top);
        item->setFirstColumnSpanned(true);
        item->setFlags(Qt::ItemIsEnabled);
        item->setData(0, Qt::UserRole, i);
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
            ui->pushButtonClear->setEnabled(true);
            break;
        }

        QTreeWidgetItem *top = new QTreeWidgetItem(ui->treeWidgetBitmaps);
        top->setData(0, Qt::UserRole, mRowCount);
        top->setText(0, QString("Bitmap %1").arg(mRowCount+1));

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
    DECL_TRACER("TBitmapDialog::on_pushButtonDelete_clicked()");

    if (mSelected < 0 || mSelected >= 5)
        return;

    // If we get a result here, we get the top level items!
    QList<QTreeWidgetItem *> list = ui->treeWidgetBitmaps->findItems(QString("Bitmap %1").arg(mSelected+1), Qt::MatchExactly, 0);

    if (list.empty())
    {
        MSG_WARNING("No item found!");
        return;
    }

    ui->treeWidgetBitmaps->removeItemWidget(list[0]->child(0), 0);
    delete list[0];
    mBitmaps.remove(mSelected);
    mRowCount--;
    // Rename the top level items
    QTreeWidgetItem *root = ui->treeWidgetBitmaps->invisibleRootItem();
    int number = root->childCount();

    for (int i = 0; i < number; ++i)
    {
        QTreeWidgetItem *item = root->child(i);
        item->setData(0, Qt::UserRole, i);
        item->setText(0, QString("Bitmap %1").arg(i+1));
    }
}


void TBitmapDialog::on_pushButtonMoveUp_clicked()
{
    DECL_TRACER("TBitmapDialog::on_pushButtonMoveUp_clicked()");

    if (mSelected == 0)
        ui->pushButtonMoveUp->setDisabled(true);

    // If we get a result here, we get the top level items!
    QList<QTreeWidgetItem *> list1 = ui->treeWidgetBitmaps->findItems(QString("Bitmap %1").arg(mSelected), Qt::MatchExactly, 0);
    QList<QTreeWidgetItem *> list2 = ui->treeWidgetBitmaps->findItems(QString("Bitmap %1").arg(mSelected+1), Qt::MatchExactly, 0);

    if (list1.empty() || list2.empty())
    {
        MSG_WARNING("Couldn't find 1 or 2 entries!");
        return;
    }

    QTreeWidgetItem *item1 = list1[0]->child(0);
    QTreeWidgetItem *item2 = list2[0]->child(0);

    if (!item1 || !item2)
    {
        MSG_WARNING("Missing 1 or 2 childs!");
        return;
    }

    QTableWidget *table1 = createTable(mBitmaps[mSelected-1]);
    QTableWidget *table2 = createTable(mBitmaps[mSelected]);

    ui->treeWidgetBitmaps->removeItemWidget(item1, 0);
    ui->treeWidgetBitmaps->setItemWidget(item1, 0, table2);

    ui->treeWidgetBitmaps->removeItemWidget(item2, 0);
    ui->treeWidgetBitmaps->setItemWidget(item2, 0, table1);

    mBitmaps.swapItemsAt(mSelected-1, mSelected);
}


void TBitmapDialog::on_pushButtonMoveDown_clicked()
{
    DECL_TRACER("TBitmapDialog::on_pushButtonMoveDown_clicked()");

    if (mSelected < 0 || mSelected >= 5)
        return;

    if (mSelected == 4)
        ui->pushButtonMoveDown->setDisabled(true);

    // If we get a result here, we get the top level items!
    QList<QTreeWidgetItem *> list1 = ui->treeWidgetBitmaps->findItems(QString("Bitmap %1").arg(mSelected+1), Qt::MatchExactly, 0);
    QList<QTreeWidgetItem *> list2 = ui->treeWidgetBitmaps->findItems(QString("Bitmap %1").arg(mSelected+2), Qt::MatchExactly, 0);

    if (list1.empty() || list2.empty())
    {
        MSG_WARNING("Couldn't find 1 or 2 entries!");
        return;
    }

    QTreeWidgetItem *item1 = list1[0]->child(0);
    QTreeWidgetItem *item2 = list2[0]->child(0);

    if (!item1 || !item2)
    {
        MSG_WARNING("Missing 1 or 2 childs!");
        return;
    }

    QTableWidget *table1 = createTable(mBitmaps[mSelected]);
    QTableWidget *table2 = createTable(mBitmaps[mSelected+1]);

    ui->treeWidgetBitmaps->removeItemWidget(item1, 0);
    ui->treeWidgetBitmaps->setItemWidget(item1, 0, table2);

    ui->treeWidgetBitmaps->removeItemWidget(item2, 0);
    ui->treeWidgetBitmaps->setItemWidget(item2, 0, table1);

    mBitmaps.swapItemsAt(mSelected, mSelected+1);
}


void TBitmapDialog::on_pushButtonClear_clicked()
{
    DECL_TRACER("TBitmapDialog::on_pushButtonClear_clicked()");

    ui->treeWidgetBitmaps->clear();
    mBitmaps.clear();
    mSelected = -1;
    mRowCount = 0;
    ui->pushButtonAdd->setEnabled(true);
    ui->pushButtonDelete->setDisabled(true);
    ui->pushButtonMoveUp->setDisabled(true);
    ui->pushButtonMoveDown->setDisabled(true);
    ui->pushButtonClear->setDisabled(true);
}

void TBitmapDialog::onTreeWidgetItemActivated(QTreeWidgetItem *item, int column)
{
    DECL_TRACER("TBitmapDialog::onTreeWidgetItemActivated(QTreeWidgetItem *item, int column)");

    ui->pushButtonDelete->setEnabled(true);
    ui->pushButtonMoveUp->setEnabled(true);
    ui->pushButtonMoveDown->setEnabled(true);
    mSelected = item->data(0, Qt::UserRole).toInt();
    MSG_DEBUG("Current selected bitmap: " << mSelected);

    if (mSelected == 0)
    {
        ui->pushButtonMoveUp->setDisabled(true);
        ui->pushButtonMoveDown->setEnabled(true);
    }

    if (mSelected == (mBitmaps.size() - 1))
    {
        ui->pushButtonMoveDown->setDisabled(true);

        if (mBitmaps.size() > 1)
        ui->pushButtonMoveUp->setEnabled(true);
    }

    if (mBitmaps.size() < 5)
        ui->pushButtonAdd->setEnabled(true);
    else
        ui->pushButtonAdd->setDisabled(true);
}

void TBitmapDialog::onBitmapsChanged(const QList<BITMAPS_t>& bitmaps, const QString& name)
{
    DECL_TRACER("TBitmapDialog::onBitmapsChanged(const QList<BITMAPS_t>& bitmaps, const QString& name)");

    Q_UNUSED(name);
    mBitmaps = bitmaps;
}

void TBitmapDialog::onOrientationChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TBitmapDialog::onOrientationChanged(const QString& text, const QVariant& data, const QString& name)");

    int idx = getObjectID(name, "Orientation_");

    if (idx < 1 || idx > 5 || idx > mBitmaps.size())
    {
        MSG_WARNING("Got invalid index " << idx);
        return;
    }

    MSG_DEBUG("Item with index: " << idx);
    idx--;
    ORIENTATION oldOri = mBitmaps[idx].justification;
    mBitmaps[idx].justification = static_cast<ORIENTATION>(idx);

    if (idx != oldOri && idx == ORI_ABSOLUT)
    {
        QTreeWidgetItem *root = ui->treeWidgetBitmaps->invisibleRootItem();
        QTreeWidgetItem *top = root->child(idx);
        QTableWidget *widget = static_cast<QTableWidget *>(ui->treeWidgetBitmaps->itemWidget(top->child(0), 0));

        if (!widget)
        {
            MSG_WARNING("Couldn't get the table widget!");
            return;
        }

        MSG_DEBUG("Object name of widget: " << widget->objectName().toStdString());
        QBrush brush;
        brush.setColor(Qt::GlobalColor::lightGray);

        for (int row = 2; row < 4; ++row)
        {
            widget->insertRow(row);
            QTableWidgetItem *col0 = new QTableWidgetItem;
            col0->setBackground(brush);
            col0->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);
            widget->setItem(row, 0, col0);

            switch(row)
            {
                case 2:
                    col0->setText(tr("Bitmap X Offset"));
                    widget->setCellWidget(row, 1, makeValueSelector(mBitmaps[idx].offsetX, QString("XOffset_%1").arg(row)));
                break;

                case 3:
                    col0->setText(tr("Bitmap Y Offset"));
                    widget->setCellWidget(row, 1, makeValueSelector(mBitmaps[idx].offsetX, QString("YOffset_%1").arg(row)));
                break;
            }
        }

        ui->treeWidgetBitmaps->resizeColumnToContents(0);
    }
    else if (oldOri == ORI_ABSOLUT && idx != oldOri)
    {
        QTreeWidgetItem *root = ui->treeWidgetBitmaps->invisibleRootItem();
        QTreeWidgetItem *top = root->child(idx);
        QTableWidget *widget = static_cast<QTableWidget *>(ui->treeWidgetBitmaps->itemWidget(top->child(0), 0));

        if (!widget)
        {
            MSG_WARNING("Couldn't get the table widget!");
            return;
        }

        MSG_DEBUG("Object name of widget (removing rows): " << widget->objectName().toStdString());
        widget->removeRow(3);
        widget->removeRow(2);
        widget->setRowCount(2);
        ui->treeWidgetBitmaps->resizeColumnToContents(0);
    }
}

QTableWidget *TBitmapDialog::createTable(const BITMAPS_t& bm)
{
    DECL_TRACER("TBitmapDialog::createTable(const BITMAPS_t& bm)");

    QBrush brush;
    brush.setColor(Qt::GlobalColor::lightGray);

    QTableWidget *table = new QTableWidget(this);
    table->setObjectName(bm.fileName);
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
                table->setCellWidget(row, 1, makeBitmapSelector(bm, QString("Bitmap_%1").arg(row)));
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

TElementBitmapSelector *TBitmapDialog::makeBitmapSelector(const BITMAPS_t& bitmap, const QString& name)
{
    DECL_TRACER("TBitmapDialog::makeBitmapSelector(const BITMAPS_t& bitmap, const QString& name)");

    TElementBitmapSelector *bs = new TElementBitmapSelector(name, bitmap, this);
    connect(bs, &TElementBitmapSelector::bitmapsChanged, this, &TBitmapDialog::onBitmapsChanged);
    return bs;
}

TElementWidgetCombo *TBitmapDialog::makeTextJustification(ORIENTATION ori, const QString& name)
{
    DECL_TRACER("TBitmapDialog::makeTextJustification(ObjHandler::ORIENTATION ori, const QString& name)");

    TElementWidgetCombo *cbox = new TElementWidgetCombo(name);
    QList<QString> items = { "absolute", "top-left", "top-middle", "top-right",
                            "center-left", "center-middle", "center-right",
                            "bottom-left", "bottom-middle", "bottom-right" };
    QList<QVariant> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    cbox->addItems(items);
    cbox->addData(data);
    cbox->setCurrentIndex(ori);
    connect(cbox, &TElementWidgetCombo::selectionChanged, this, &TBitmapDialog::onOrientationChanged);
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

void TBitmapDialog::setValue(const QString& name, const QVariant& value)
{
    DECL_TRACER("TBitmapDialog::setValue(const QString& name, const QVariant& value)");

}

