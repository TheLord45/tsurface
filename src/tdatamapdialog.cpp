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
#include <QListView>
#include <QStandardItemModel>

#include "tdatamapdialog.h"
#include "ui_tdatamapdialog.h"
#include "tdownloader.h"
#include "tcrypt.h"
#include "terror.h"

TDataMapDialog::TDataMapDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TDataMapDialog)
{
    DECL_TRACER("TDataMapDialog::TDataMapDialog(QWidget *parent)");

    ui->setupUi(this);
    ui->labelInfo->setText("");

    ui->groupBoxSortQuery->setVisible(false);

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setColumnCount(1);
    ui->listViewColums->setModel(model);
    ui->listViewColums->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->listViewColums->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listViewColums->setAlternatingRowColors(true);

    mLoader = new TDownloader(this);
    connect(mLoader, &TDownloader::onReady, this, &TDataMapDialog::onReady);
    connect(mLoader, &TDownloader::onError, this, &TDataMapDialog::onError);
}

TDataMapDialog::~TDataMapDialog()
{
    DECL_TRACER("TDataMapDialog::~TDataMapDialog()");

    delete ui;

    if (mLoader)
    {
        disconnect(mLoader, &TDownloader::onReady, this, &TDataMapDialog::onReady);
        disconnect(mLoader, &TDownloader::onError, this, &TDataMapDialog::onError);
        delete mLoader;
    }
}

void TDataMapDialog::setData(const ConfigMain::DATASOURCE_t& data)
{
    DECL_TRACER("TDataMapDialog::setData(const ConfigMain::DATASOURCE_t& data)");

    mData = data;
    setPrimaryText(data.mapIdT1);
    setSecondaryText(data.mapIdT2);
    setImageName(data.mapIdI1);
    setQuery(data.sortQuery);
    setSort(static_cast<SORT_t>(data.sort));
    mHaveLive = data.live;

    if (!data.sortOrder.empty())
    {
        QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->listViewColums->model());
        QStringList::ConstIterator iter;
        int row = 0;

        for (iter = data.sortOrder.cbegin(); iter != data.sortOrder.cend(); ++iter)
        {
            QStandardItem *cell1 = new QStandardItem;
            cell1->setEditable(true);
            cell1->setText(*iter);
            model->setItem(row, 0, cell1);
            row++;
        }
    }

    mHaveData = true;
}

void TDataMapDialog::setPrimaryText(const QString& txt)
{
    DECL_TRACER("TDataMapDialog::setPrimaryText(const QString& txt)");

    mPrimaryText = txt;
    ui->lineEditPrimaryText->setText(txt);
}

void TDataMapDialog::setSecondaryText(const QString& txt)
{
    DECL_TRACER("TDataMapDialog::setSecondaryText(const QString& txt)");

    mSecondaryText = txt;
    ui->lineEditSecondaryText->setText(txt);
}

void TDataMapDialog::setImageName(const QString& image)
{
    DECL_TRACER("TDataMapDialog::setImageName(const QString& image)");

    mImageName = image;
    ui->lineEditImage->setText(image);
}

void TDataMapDialog::setSort(SORT_t sort)
{
    DECL_TRACER("TDataMapDialog::setSort(SORT_t sort)");

    switch(sort)
    {
        case SORT_NONE: ui->radioButtonSortNone->setChecked(true); break;
        case SORT_ASC:  ui->radioButtonSortAsc->setChecked(true); break;
        case SORT_DESC: ui->radioButtonSortDesc->setChecked(true); break;

        case
            SORT_QUERY: ui->radioButtonSortQuery->setChecked(true);
            ui->groupBoxSortQuery->setVisible(true);
        break;
    }

        if (sort != SORT_QUERY)
            ui->groupBoxSortQuery->setVisible(false);
}

void TDataMapDialog::setColumns(const QStringList& cols)
{
    DECL_TRACER("TDataMapDialog::setColumns(const QStringList& cols)");

    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->listViewColums->model());
    bool haveModel = true;

    if (!model)
    {
        model = new QStandardItemModel(this);
        haveModel = false;
    }

    QStringList::Iterator iter;
    int row = 0;

    for (iter = mColumns.begin(); iter != mColumns.end(); ++iter)
    {
        QStandardItem *item = new QStandardItem(*iter);
        item->setData(*iter, Qt::DisplayRole);
        model->appendRow(item);
        row++;
    }

    if (!haveModel)
        ui->listViewColums->setModel(model);
}

void TDataMapDialog::setQuery(const QString& query)
{
    DECL_TRACER("TDataMapDialog::setQuery(const QString& query)");

    mQuery = query;
    ui->lineEditQuery->setText(query);
}

void TDataMapDialog::on_pushButtonGetFile_clicked()
{
    DECL_TRACER("TDataMapDialog::on_pushButtonGetFile_clicked()");

    if (!mLoader)
    {
        ui->labelInfo->setText(tr("No downloader available!"));
        return;
    }

    if (!mHaveData)
    {
        ui->labelInfo->setText(tr("No URL data available!"));
        return;
    }

    QUrl url;
    url.setScheme(mData.protocol);
    url.setHost(mData.host);

    if (mData.path.isEmpty())
        url.setPath("/" + mData.file);
    else
        url.setPath("/" + mData.path + "/" + mData.file);

    if (!mData.user.isEmpty())
        url.setUserName(mData.user);

    if (mData.sort == ConfigMain::SORT_ADVANCED && !mData.sortQuery.isEmpty())
        url.setQuery(mData.sortQuery);

    if (!mData.password.isEmpty())
    {
        TCrypt cr;
        url.setPassword(cr.doDecrypt(mData.password, cr.getPassword()));
    }

    mLoader->getData(url);
    ui->labelInfo->setText(tr("Trying to load data ..."));
}

void TDataMapDialog::on_lineEditPrimaryText_textChanged(const QString &arg1)
{
    mPrimaryText = arg1;
}

void TDataMapDialog::on_lineEditSecondaryText_textChanged(const QString &arg1)
{
    mSecondaryText = arg1;
}

void TDataMapDialog::on_lineEditImage_textChanged(const QString &arg1)
{
    mImageName = arg1;
}

void TDataMapDialog::on_comboBoxSelectT1_currentIndexChanged(int index)
{
    DECL_TRACER("TDataMapDialog::on_comboBoxSelectT1_currentIndexChanged(int index)");

    if (!mInitialized)
        return;

    setPrimaryText(ui->comboBoxSelectT1->itemText(index));
}

void TDataMapDialog::on_comboBoxSelectT2_currentIndexChanged(int index)
{
    DECL_TRACER("TDataMapDialog::on_comboBoxSelectT2_currentIndexChanged(int index)");

    if (!mInitialized)
        return;

    setSecondaryText(ui->comboBoxSelectT2->itemText(index));
}

void TDataMapDialog::on_comboBoxSelectImage_currentIndexChanged(int index)
{
    DECL_TRACER("TDataMapDialog::on_comboBoxSelectImage_currentIndexChanged(int index)");

    if (!mInitialized)
        return;

    setImageName(ui->comboBoxSelectImage->itemText(index));
}

void TDataMapDialog::on_radioButtonSortNone_clicked(bool checked)
{
    DECL_TRACER("TDataMapDialog::on_radioButtonSortNone_clicked(bool checked)");

    if (checked)
    {
        mSort = SORT_NONE;
        ui->groupBoxSortQuery->setVisible(false);
    }
}

void TDataMapDialog::on_radioButtonSortAsc_clicked(bool checked)
{
    DECL_TRACER("TDataMapDialog::on_radioButtonSortAsc_clicked(bool checked)");

    if (checked)
    {
        mSort = SORT_ASC;
        ui->groupBoxSortQuery->setVisible(false);
    }
}

void TDataMapDialog::on_radioButtonSortDesc_clicked(bool checked)
{
    DECL_TRACER("TDataMapDialog::on_radioButtonSortDesc_clicked(bool checked)");

    if (checked)
    {
        mSort = SORT_DESC;
        ui->groupBoxSortQuery->setVisible(false);
    }
}

void TDataMapDialog::on_radioButtonSortQuery_clicked(bool checked)
{
    DECL_TRACER("TDataMapDialog::on_radioButtonSortQuery_clicked(bool checked)");

    if (checked)
    {
        mSort = SORT_QUERY;
        ui->groupBoxSortQuery->setVisible(true);
    }
}

void TDataMapDialog::on_listViewColums_activated(const QModelIndex &index)
{
    DECL_TRACER("TDataMapDialog::on_listViewColums_activated(const QModelIndex &index)");

    ui->listViewColums->setCurrentIndex(index);
    ui->listViewColums->edit(index);
}

void TDataMapDialog::on_pushButtonColumnAdd_clicked()
{
    DECL_TRACER("TDataMapDialog::on_pushButtonColumnAdd_clicked()");

    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->listViewColums->model());
    int row = model->rowCount();
    QStandardItem *cell1 = new QStandardItem;
    cell1->setEditable(true);
    cell1->setText("");
    model->setItem(row, 0, cell1);
}

void TDataMapDialog::on_pushButtonColumnRemove_clicked()
{
    DECL_TRACER("TDataMapDialog::on_pushButtonColumnRemove_clicked()");

    QItemSelectionModel *selModel = ui->listViewColums->selectionModel();

    if (!selModel || !selModel->hasSelection())
        return;

    QModelIndexList list = selModel->selectedIndexes();

    if (list.empty())
        return;

    int row = list[0].row();
    ui->listViewColums->model()->removeRow(row);
}

void TDataMapDialog::on_pushButtonColumnUp_clicked()
{
    DECL_TRACER("TDataMapDialog::on_pushButtonColumnUp_clicked()");

    QItemSelectionModel *selModel = ui->listViewColums->selectionModel();

    if (!selModel || !selModel->hasSelection())
        return;

    QModelIndexList list = selModel->selectedIndexes();

    if (list.empty())
        return;

    int row = list[0].row();

    if (row == 0)
        return;

    ui->listViewColums->model()->moveRow(list[0].parent(), row, list[0].parent().parent(), row-1);
}

void TDataMapDialog::on_pushButtonColumnDown_clicked()
{
    DECL_TRACER("TDataMapDialog::on_pushButtonColumnDown_clicked()");

    QItemSelectionModel *selModel = ui->listViewColums->selectionModel();

    if (!selModel || !selModel->hasSelection())
        return;

    QModelIndexList list = selModel->selectedIndexes();

    if (list.empty())
        return;

    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->listViewColums->model());
    int row = list[0].row();

    if (row >= model->rowCount())
        return;

    QStandardItem *next = model->item(row+1, 0);
    model->moveRow(list[0].parent(), row, next->index().parent(), row+1);
}

void TDataMapDialog::on_comboBoxColumnSelect_currentIndexChanged(int index)
{
    DECL_TRACER("TDataMapDialog::on_comboBoxColumnSelect_currentIndexChanged(int index)");

    QItemSelectionModel *selModel = ui->listViewColums->selectionModel();

    if (!selModel || !selModel->hasSelection())
        return;

    QModelIndexList list = selModel->selectedIndexes();

    if (list.empty())
        return;

    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->listViewColums->model());
    int row = list[0].row();
    model->item(row, 0)->setText(ui->comboBoxColumnSelect->itemText(index));
}


void TDataMapDialog::on_lineEditQuery_textChanged(const QString &arg1)
{
    mQuery = arg1;
}

void TDataMapDialog::accept()
{
    DECL_TRACER("TDataMapDialog::accept()");

    QStandardItemModel *model = static_cast<QStandardItemModel * >(ui->listViewColums->model());
    int rows = model->rowCount();
    mColumns.clear();

    if (rows)
    {
        for (int i = 0; i < rows; ++i)
        {
            QString txt = model->index(i, 0).data().toString();

            if (!txt.isEmpty())
                mColumns.append(txt);
        }

        mData.sortOrder = mColumns;
    }

    done(QDialog::Accepted);
}

void TDataMapDialog::onReady()
{
    DECL_TRACER("TDataMapDialog::onReady()");

    if (!mLoader)
        return;

    QString buffer = mLoader->getBuffer();

    if (buffer.isEmpty())
    {
        ui->labelInfo->setText(tr("Data couldn't be downloaded! Either the URL is wrong or the server is not reachable."));
        return;
    }

    if (mData.format != "xport-s" && buffer.contains(";"))
    {
        // Get the first line from the buffer
        qsizetype pos = buffer.indexOf('\n');

        if (pos > 0)
        {
            QString line = buffer.left(pos);
            QStringList parts = line.split(";");
            ui->labelInfo->setText(tr("Data were successfully downloaded! There are %1 columns in the file.").arg(parts.size()));

            if (mData.format == "csv-headers")
            {
                mInitialized = false;
                ui->comboBoxSelectT1->clear();
                ui->comboBoxSelectT2->clear();
                ui->comboBoxSelectImage->clear();
                ui->comboBoxColumnSelect->clear();

                ui->comboBoxSelectT1->addItems(parts);
                ui->comboBoxSelectT2->addItems(parts);
                ui->comboBoxSelectImage->addItems(parts);
                ui->comboBoxColumnSelect->addItems(parts);
                mInitialized = true;
            }
        }
        else
            ui->labelInfo->setText(tr("Data were downloaded. Got %1 Bytes.").arg(buffer.length()));
    }
    else
        ui->labelInfo->setText(tr("Data were downloaded. Got %1 Bytes.").arg(buffer.length()));

    mHaveLive = true;
}

void TDataMapDialog::onError(const QString& msg)
{
    DECL_TRACER("TDataMapDialog::onError(const QString& msg)");

    ui->labelInfo->setText(msg);
    mHaveLive = false;
}
