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
#include <QFileDialog>
#include <QProgressDialog>

#include "tbitmapselectdialog.h"
#include "ui_tbitmapselectdialog.h"
#include "tdynamicimagedialog.h"
#include "tmaps.h"
#include "tmisc.h"
#include "tconfig.h"
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

    for (iter = mImages.begin(); iter != mImages.end(); ++iter)
    {
        QString file = mPathTemporary + "/images/" + *iter;
        MSG_DEBUG("Loading image: " << file.toStdString());
        QStandardItem *item = new QStandardItem(*iter);
        QSize oriSize;
        item->setData(sizeImage(iconSize, file, &oriSize), Qt::DecorationRole);
        item->setData(*iter, Qt::UserRole);
        item->setSizeHint(gridSize);
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        item->setToolTip(QString("%1 (%2x%3)").arg(*iter).arg(oriSize.width()).arg(oriSize.height()));
        model->appendRow(item);
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

void TBitmapSelectDialog::setDefaultBitmap(const QString& file)
{
    DECL_TRACER("TBitmapSelectDialog::setDefaultBitmap(const QString& file)");

    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->listViewImages->model());

    if (!model)
        return;

    for (int i = 0; i < model->rowCount(); ++i)
    {
        QStandardItem *item =model->item(i, 0);

        if (!item)
            continue;

        if (item->data(Qt::UserRole).toString() == file)
        {
            QItemSelectionModel *selModel = ui->listViewImages->selectionModel();

            if (!selModel)
                return;

            selModel->setCurrentIndex(item->index(), QItemSelectionModel::ClearAndSelect);
            break;
        }
    }
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
    DECL_TRACER("TBitmapSelectDialog::on_pushButtonImport_clicked()");
    QString lastOpenPath = TConfig::Current().getLastDirectory();

    if (mCurrentTab == SEL_IMAGES)
    {
        mImportImagesDialog = new QFileDialog(this, tr("Import Image"), lastOpenPath, tr("Image (*.png *.jpg);;All (*)"));
        mImportImagesDialog->setFileMode(QFileDialog::ExistingFiles);
        connect(mImportImagesDialog, &QFileDialog::finished, this, &TBitmapSelectDialog::onImageImportFinished);
        mImportImagesDialog->open();
    }
    else if (mCurrentTab == SEL_DYNIMAGES)
    {
        TDynamicImageDialog dynDialog(this);

        if (dynDialog.exec() == QDialog::Rejected)
            return;

        ConfigMain::RESOURCE_t res;
        res.name = dynDialog.getName();
        res.protocol = dynDialog.getProtocol();
        res.host = dynDialog.getHost();
        res.path = dynDialog.getPath();
        res.file = dynDialog.getFile();
        res.user = dynDialog.getUser();
        res.password = dynDialog.getPassword();
        res.refreshRate = dynDialog.getRefreshRate();
        res.refreshStart = dynDialog.getRefreshStart();
        addDynamicResource(res);
        TConfMain::Current().setDynamicResource(res);
        ui->tableViewDynamic->resizeColumnsToContents();
    }
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

void TBitmapSelectDialog::onImageImportFinished(int result)
{
    DECL_TRACER("TBitmapSelectDialog::onImageImportFinished(int result)");

    if (!mImportImagesDialog)
        return;

    if (result != QDialog::Accepted)
    {
        mImportImagesDialog = nullptr;
        return;
    }

    QStringList files = mImportImagesDialog->selectedFiles();
    mImportImagesDialog = nullptr;
    importImagesToListView(files);
}

void TBitmapSelectDialog::importImagesToListView(const QStringList& images)
{
    DECL_TRACER("TBitmapSelectDialog::importImagesToListView(const QStringList& images)");

    if (images.empty())
        return;

    QProgressDialog progress(tr("Copying images"), tr("Abort copy"), 0, images.count(), this);
    progress.setWindowModality(Qt::WindowModal);

    TMaps::Current().setPathTemporary(mPathTemporary);
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->listViewImages->model());
    QStringList::ConstIterator iter;
    int pos = 0;

    for (iter = images.cbegin(); iter != images.cend(); ++iter)
    {
        MSG_DEBUG("Working on file " << iter->toStdString());
        pos++;

        progress.setValue(pos);

        if (progress.wasCanceled())
            break;

        if (!mImages.isEmpty())
        {
            QStringList::Iterator imIter;
            bool haveImage = false;

            for (imIter = mImages.begin(); imIter != mImages.end(); ++imIter)
            {
                if (*imIter == *iter)
                {
                    MSG_DEBUG("Image exists");
                    haveImage = true;
                    QList<QStandardItem *> items = model->findItems(*iter);
                    QList<QStandardItem *>::Iterator itItem;

                    for (itItem = items.begin(); itItem != items.end(); ++itItem)
                        model->itemChanged(*itItem);
                }
            }

            if (haveImage)
                continue;
        }

        QString bn = basename(*iter);
        QStandardItem *item = new QStandardItem(bn);
        QSize oriSize;
        item->setData(sizeImage(iconSize, *iter, &oriSize), Qt::DecorationRole);
        item->setSizeHint(gridSize);
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        item->setToolTip(QString("%1 (%2x%3)").arg(*iter).arg(oriSize.width()).arg(oriSize.height()));
        model->appendRow(item);
        mImages.append(bn);
        TMaps::Current().addBitmap(*iter, 0, 0, 0, 0);
    }

    model->sort(0);
}
