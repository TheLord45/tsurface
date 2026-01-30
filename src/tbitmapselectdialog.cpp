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
#include <QStandardItemModel>

#include "tbitmapselectdialog.h"
#include "ui_tbitmapselectdialog.h"
#include "tmaps.h"
#include "terror.h"

static QSize iconSize(64, 64);
static QSize gridSize(80, 80);

TBitmapSelectDialog::TBitmapSelectDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TBitmapSelectDialog)
{
    DECL_TRACER("TBitmapSelectDialog::TBitmapSelectDialog(QWidget *parent)");
}

TBitmapSelectDialog::~TBitmapSelectDialog()
{
    DECL_TRACER("TBitmapSelectDialog::~TBitmapSelectDialog()");

    delete ui;
}

void TBitmapSelectDialog::setTemporaryPath(const QString& path)
{
    DECL_TRACER("TBitmapSelectDialog::setTemporaryPath(const QString& path)");

    mPathTemporary = path;
    init();
}

void TBitmapSelectDialog::init()
{
    DECL_TRACER("TBitmapSelectDialog::init()");

    ui->setupUi(this);
    ui->listViewImages->setGridSize(gridSize);
    ui->listViewImages->setViewMode(QListView::IconMode);
    ui->listViewImages->setResizeMode(QListView::Adjust);
    ui->listViewImages->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->listViewImages->setSelectionRectVisible(true);
    ui->listViewImages->setSpacing(10);
    ui->listViewImages->setDragEnabled(false);

    mImages = TMaps::Current().getAllImageFiles();
    QStandardItemModel *model = new QStandardItemModel(this);
    QStringList::Iterator iter;
    int row = 0;

    for (iter = mImages.begin(); iter != mImages.end(); ++iter)
    {
        QString file = mPathTemporary + "/images/" + *iter;
        MSG_DEBUG("Loading image: " << file.toStdString());
        QStandardItem *item = new QStandardItem(*iter);
        item->setData(sizeImage(iconSize, file), Qt::DecorationRole);
        item->setSizeHint(gridSize);
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        model->appendRow(item);
        row++;
    }

    model->sort(0);
    ui->listViewImages->setModel(model);
    // Dynamic resources
    QStandardItemModel *dynModel = new QStandardItemModel(this);
    dynModel->setColumnCount(3);
    QStringList headers;
    headers << tr("Name") << tr("URL") << tr("Refresh");
    dynModel->setHorizontalHeaderLabels(headers);
    mDynamicResources = TConfMain::Current().getAllDynamicResources();
    QList<ConfigMain::RESOURCE_t>::Iterator dynIter;

    for (dynIter = mDynamicResources.begin(); dynIter != mDynamicResources.end(); ++dynIter)
        addDynamicResource(*dynIter, dynModel);

    ui->tableViewDynamic->setModel(dynModel);
    ui->tableViewDynamic->resizeColumnsToContents();
    ui->tableViewDynamic->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewDynamic->setSelectionMode(QAbstractItemView::SingleSelection);
}

void TBitmapSelectDialog::setSingleSelect(bool sel)
{
    DECL_TRACER("TBitmapSelectDialog::setSingleSelect(bool sel)");

    mSingleSelect = sel;

    if (sel)
        ui->listViewImages->setSelectionMode(QAbstractItemView::SingleSelection);
    else
        ui->listViewImages->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void TBitmapSelectDialog::on_pushButtonImport_clicked()
{

}


void TBitmapSelectDialog::on_listViewImages_clicked(const QModelIndex &index)
{

}


void TBitmapSelectDialog::on_listViewDynamic_clicked(const QModelIndex &index)
{

}


void TBitmapSelectDialog::on_tabWidget_tabBarClicked(int index)
{
    DECL_TRACER("TBitmapSelectDialog::on_tabWidget_tabBarClicked(int index)");

    mCurrentTab = index;
}

QPixmap TBitmapSelectDialog::sizeImage(const QSize& size, const QString& file)
{
    DECL_TRACER("TBitmapSelectDialog::sizeImage(const QSize& size, const QString& file)");

    QPixmap pm(file);
    return QPixmap(pm.scaled(size, Qt::KeepAspectRatio));
}

void TBitmapSelectDialog::addDynamicResource(const ConfigMain::RESOURCE_t& res, QStandardItemModel *model)
{
    DECL_TRACER("TBitmapSelectDialog::addDynamicResource(const ConfigMain::RESOURCE_t& res, QStandardItemModel *model)");

    QStandardItemModel *dynModel = nullptr;

    if (model)
        dynModel = model;
    else
        dynModel = static_cast<QStandardItemModel *>(ui->tableViewDynamic->model());

    if (!dynModel)
        return;

    int row = dynModel->rowCount();
    QStandardItem *cell1 = new QStandardItem;
    QStandardItem *cell2 = new QStandardItem;
    QStandardItem *cell3 = new QStandardItem;

    cell1->setEditable(false);
    cell2->setEditable(false);
    cell3->setEditable(false);

    cell1->setText(res.name);
    cell2->setText(TConfMain::Current().getUrl(res));
    cell3->setText(QString("%1").arg(res.refreshRate));

    dynModel->setItem(row, 0, cell1);
    dynModel->setItem(row, 1, cell2);
    dynModel->setItem(row, 2, cell3);
}

void TBitmapSelectDialog::accept()
{
    DECL_TRACER("TBitmapSelectDialog::accept()");

    QItemSelectionModel *selModel = ui->listViewImages->selectionModel();

    if (selModel && selModel->hasSelection())
    {
        QModelIndexList list = selModel->selectedIndexes();

        for (QModelIndex idx : list)
        {
            ObjHandler::BITMAPS_t bm;
            bm.fileName = idx.data().toString();
            bm.dynamic = false;
            mSelected.append(bm);
        }
    }

    selModel = ui->tableViewDynamic->selectionModel();

    if (selModel && selModel->hasSelection())
    {
        QModelIndexList list = selModel->selectedIndexes();
        QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewDynamic->model());

        for (QModelIndex idx : list)
        {
            int row = idx.row();
            ObjHandler::BITMAPS_t bm;
            bm.fileName = model->item(row, 0)->text();
            bm.dynamic = true;
            mSelected.append(bm);
        }
    }

    done(QDialog::Accepted);
}
