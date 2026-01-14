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
#include <filesystem>

#include <QFileDialog>
#include <QIcon>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemSelectionModel>
#include <QMessageBox>
#include <QProgressDialog>
#include <QClipboard>

#include "tresourcedialog.h"
#include "ui_tresourcedialog.h"
#include "tmaps.h"
#include "tconfmain.h"
#include "trenamefiledialog.h"
#include "tdynamicimagedialog.h"
#include "terror.h"
#include "tmisc.h"

namespace fs = std::filesystem;
QSize iconSize(100, 100);
QSize gridSize(128, 128);

using std::string;

TResourceDialog::TResourceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TResourceDialog)
{
    ui->setupUi(this);

    mPathTemporary = TConfMain::Current().getPathTemporary();
    mMapsFile = TConfMain::Current().getMapsFile();
    mLastOpenPath = QString::fromStdString(fs::current_path());
    // View style
    ui->comboBoxStyle->setItemIcon(0, QIcon(":/images/view-list-tree.svg"));
    ui->comboBoxStyle->setItemIcon(1, QIcon(":/images/view-list-details.svg"));
    ui->comboBoxStyle->setItemIcon(2, QIcon(":/images/view-list-icons.svg"));
    ui->comboBoxStyle->setCurrentIndex(mShowType);
    // Images
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

    setLabel(LABEL_LISTVIEW, row, "");
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

    ui->tableViewDynamicImages->setModel(dynModel);
    ui->tableViewDynamicImages->resizeColumnsToContents();
    ui->tableViewDynamicImages->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewDynamicImages->setSelectionMode(QAbstractItemView::SingleSelection);
    // Disable buttons
    disableClipboardButtons();
    ui->pushButtonUndo->setDisabled(true);
    ui->pushButtonRedo->setDisabled(true);
    ui->pushButtonRename->setDisabled(true);
    // Clipboard initializing
    mClipboard = QGuiApplication::clipboard();

    if (mClipboard)
    {
        connect(mClipboard, &QClipboard::changed, this, &TResourceDialog::onClipboardChanged);
        connect(mClipboard, &QClipboard::dataChanged, this, &TResourceDialog::onClipboardDataChanged);
        connect(mClipboard, &QClipboard::selectionChanged, this, &TResourceDialog::onClipboardDataChanged);
        connect(mClipboard, &QClipboard::findBufferChanged, this, &TResourceDialog::onClipboardDataChanged);
    }
}

TResourceDialog::~TResourceDialog()
{
    delete ui;

    if (mClipboard)
    {
        disconnect(mClipboard, &QClipboard::changed, this, &TResourceDialog::onClipboardChanged);
        disconnect(mClipboard, &QClipboard::dataChanged, this, &TResourceDialog::onClipboardDataChanged);
        disconnect(mClipboard, &QClipboard::selectionChanged, this, &TResourceDialog::onClipboardDataChanged);
        disconnect(mClipboard, &QClipboard::findBufferChanged, this, &TResourceDialog::onClipboardDataChanged);
    }
}

void TResourceDialog::on_pushButtonCut_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonCut_clicked()");

    if (mTabSelected == SEL_IMAGES)
    {
        copyImagesToClipboard();
        on_pushButtonDelete_clicked();
    }
    else if (mTabSelected == SEL_DYNIMAGES)
    {
        copyDynamicImageToClipboard();
        on_pushButtonDelete_clicked();
    }
}

void TResourceDialog::on_pushButtonCopy_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonCopy_clicked()");

    if (mTabSelected == SEL_IMAGES)
        copyImagesToClipboard();
    else if (mTabSelected == SEL_DYNIMAGES)
        copyDynamicImageToClipboard();
}

void TResourceDialog::on_pushButtonPaste_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonPaste_clicked()");

    if (!mClipboard)
        return;

    if (mTabSelected == SEL_IMAGES && !mClipboardPixmap.isNull())
    {
        QString fname(QString("Unknown_%1.png").arg(mClipboardPixmapNumber));
        mClipboardPixmapNumber++;
        mClipboardPixmap.save(mPathTemporary + "/images/" + fname);
        QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->listViewImages->model());
        QStandardItem *item = new QStandardItem(fname);
        item->setData(mClipboardPixmap.scaled(iconSize, Qt::KeepAspectRatio), Qt::DecorationRole);
        item->setSizeHint(gridSize);
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        model->appendRow(item);
        model->sort(0);
        setLabel(LABEL_LISTVIEW, model->rowCount(), "");
        mChanged = true;
    }
    else if (mTabSelected == SEL_DYNIMAGES)
    {
        ConfigMain::RESOURCE_t res;

        if (parseDynamicImageFromClipboard(&res, mClipboardText))
        {
            if (!TConfMain::Current().haveDynamicResource(res.name))
            {
                TConfMain::Current().setDynamicResource(res);
                addDynamicResource(res);
                mChanged = true;
            }
        }
    }
}

void TResourceDialog::on_pushButtonDelete_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonDelete_clicked()");

    if (mTabSelected == SEL_IMAGES)
    {
        QItemSelectionModel *model = ui->listViewImages->selectionModel();

        if (model && model->hasSelection())
        {
            int count = 0;
            QModelIndexList list = model->selectedIndexes();
            QModelIndexList::Iterator iter;
            count = list.size();

            while (count > 0)
            {
                if (list.empty())
                    break;

                iter = list.begin();

                QString file = iter->data().toString();
                MSG_DEBUG("Selected file: " << file.toStdString());
                bool remove = false;

                if (TMaps::Current().isBitmapUsed(file))
                {
                    int ret = QMessageBox::question(this, tr("File use"), tr("The file %1 is in use!<br>Do you really want delete this file?"));

                    if (ret == QMessageBox::Yes)
                        remove = true;
                }
                else
                    remove = true;

                if (remove)
                {
                    removeItemFromListView(file, iter->row());
                    mChanged = true;
                }
                else
                    model->setCurrentIndex(*iter, QItemSelectionModel::Deselect);

                count--;
                list = model->selectedIndexes();
            }
        }

        if (model && !model->hasSelection())
        {
            disableClipboardButtons();
            setLabel(LABEL_LISTVIEW, ui->listViewImages->model()->rowCount(), "");
        }
    }
    else if (mTabSelected == SEL_DYNIMAGES)
    {
        QItemSelectionModel *model = ui->tableViewDynamicImages->selectionModel();

        if (!model || !model->hasSelection())
        {
            disableClipboardButtons();
            return;
        }

        QModelIndexList list = model->selectedIndexes();
        QStandardItemModel *mod = static_cast<QStandardItemModel *>(ui->tableViewDynamicImages->model());
        int row = list[0].row();
        QString name = mod->item(row, 0)->text();
        mod->removeRow(row);
        TConfMain::Current().removeDynamicImage(name);
        disableClipboardButtons();
        mChanged = true;
    }
}

void TResourceDialog::on_pushButtonRename_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonRename_clicked()");

    if (mTabSelected == SEL_IMAGES)
    {
        QItemSelectionModel *model = ui->listViewImages->selectionModel();

        if (model && model->hasSelection())
        {
            int count = 0;
            QModelIndexList list = model->selectedIndexes();
            QString srcFile = list[0].data().toString();
            TRenameFileDialog renameDialog(srcFile, this);

            if (renameDialog.exec() == QDialog::Rejected)
                return;

            QString newFile = renameDialog.getFileName();

            if (newFile.isEmpty())
                return;

            string strFile = QString("%1/images/%2").arg(mPathTemporary).arg(newFile).toStdString();

            if (fs::exists(strFile))
            {
                int ret = QMessageBox::question(this, tr("File exists"), tr("The file %1 exists already!<br>Do you want to overwrite it?").arg(newFile));

                if (ret == QMessageBox::No)
                    return;

                removeItemFromListView(newFile, getRowFromListView(newFile));
                setLabel(LABEL_LISTVIEW, ui->listViewImages->model()->rowCount(), "");
            }

            QStandardItem *item = getItemFromListView(srcFile);

            if (item)
                item->setData(newFile, Qt::DisplayRole);

            renameImageFile(srcFile, newFile);
            TMaps::Current().renameBitmap(srcFile, newFile);
            mChanged = true;
        }
    }
    else if (mTabSelected == SEL_DYNIMAGES)
    {
        QItemSelectionModel *model = ui->tableViewDynamicImages->selectionModel();

        if (model && model->hasSelection())
        {
            QModelIndexList list = model->selectedIndexes();

            if (list.empty())
                return;

            int row = list[0].row();
            QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewDynamicImages->model());
            QStandardItem *item = model->item(row, 0);
            editDynamicResource(item->text());
        }
    }
}

void TResourceDialog::on_pushButtonUndo_clicked()
{

}


void TResourceDialog::on_pushButtonRedo_clicked()
{

}

void TResourceDialog::on_pushButtonImport_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonImport_clicked()");

    if (mTabSelected == SEL_IMAGES)
    {
        mImportImagesDialog = new QFileDialog(this, tr("Import Image"), mLastOpenPath, tr("Image (*.png *.jpg);;All (*)"));
        mImportImagesDialog->setFileMode(QFileDialog::ExistingFiles);
        connect(mImportImagesDialog, &QFileDialog::finished, this, &TResourceDialog::onImageImportFinished);
        mImportImagesDialog->open();
    }
    else if (mTabSelected == SEL_DYNIMAGES)
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
        ui->tableViewDynamicImages->resizeColumnsToContents();
        setLabel(LABEL_RESOURCES, ui->tableViewDynamicImages->model()->rowCount(), "");
        mChanged = true;
    }
}

void TResourceDialog::onImageImportFinished(int result)
{
    DECL_TRACER("TResourceDialog::onImageImportFinished(int result)");

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

void TResourceDialog::on_pushButtonExport_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonExport_clicked()");

    if (mTabSelected == SEL_IMAGES)
    {
        QItemSelectionModel *model = ui->listViewImages->selectionModel();

        if (!model || !model->hasSelection())
            return;

        QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"), mLastOpenPath);

        if (dir.isEmpty())
            return;

        QModelIndexList list = model->selectedIndexes();
        QModelIndexList::Iterator iter;
        int count = 0;

        for (iter = list.begin(); iter != list.end(); ++iter)
        {
            QString file = iter->data().toString();

            if (file.isEmpty())
                continue;

            QFile image(mPathTemporary + "/images/" + file);
            QString target = dir + "/" + file;

            if (!image.copy(target))
            {
                int ret = QMessageBox::critical(this,
                                                tr("Copy error"),
                                                tr("Error copying file <b>%1</b><br>%2").arg(file).arg(image.errorString()),
                                                QMessageBox::Ok | QMessageBox::Cancel,
                                                QMessageBox::Cancel);

                if (ret == QMessageBox::Cancel || ret == QMessageBox::Escape)
                    break;
            }

            count++;
        }

        if (count > 1)
            QMessageBox::information(this, tr("Files copied"), tr("Copied %1 files to directory %2.").arg(count).arg(dir));
    }
    else if (mTabSelected == SEL_DYNIMAGES)
    {
        QMessageBox::information(this, tr("Information"), tr("There is no working file for the selected dynamic image."));
    }
}

void TResourceDialog::on_pushButtonDataMap_clicked()
{

}


void TResourceDialog::on_comboBoxStyle_currentIndexChanged(int index)
{
    DECL_TRACER("TResourceDialog::on_comboBoxStyle_currentIndexChanged(int index)");

    mShowType = static_cast<SHOW_t>(index);

    if (mTabSelected == SEL_IMAGES)
    {
        switch(mShowType)
        {
            case SHOW_ICON: ui->listViewImages->setViewMode(QListView::IconMode); break;
            case SHOW_DETAIL:
            case SHOW_LIST: ui->listViewImages->setViewMode(QListView::ListMode); break;
        }
    }
}

void TResourceDialog::on_tabWidgetAction_tabBarClicked(int index)
{
    DECL_TRACER("TResourceDialog::on_tabWidgetAction_tabBarClicked(int index)");

    if (index < 0 || index > 3)
        return;

    mTabSelected = static_cast<TABSEL_t>(index);

    if (mTabSelected == SEL_DATASOURCE || mTabSelected == SEL_DYNIMAGES)
    {
        ui->pushButtonImport->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew));
        ui->pushButtonImport->setText(tr("New"));
        ui->pushButtonRename->setIcon(QIcon(":/images/show-menu.png"));
        ui->pushButtonRename->setText(tr("Edit"));
    }
    else
    {
        ui->pushButtonImport->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));
        ui->pushButtonImport->setText(tr("Import"));
        ui->pushButtonRename->setIcon(QIcon(":/images/show-menu.png"));
        ui->pushButtonRename->setText(tr("Rename"));
    }

    switch(mTabSelected)
    {
        case SEL_IMAGES: setLabel(LABEL_LISTVIEW, ui->listViewImages->model()->rowCount(), ""); break;
        case SEL_DYNIMAGES: setLabel(LABEL_RESOURCES, ui->tableViewDynamicImages->model()->rowCount(), ""); break;
        default:
            return;
    }
}

void TResourceDialog::on_listViewImages_activated(const QModelIndex &index)
{
    DECL_TRACER("TResourceDialog::on_listViewImages_activated(const QModelIndex &index)");

    QItemSelectionModel *model = ui->listViewImages->selectionModel();

    if (model && model->hasSelection())
    {
        enableClipboardButtons();
        QModelIndexList list = model->selectedIndexes();
    }
    else
        disableClipboardButtons();
}

void TResourceDialog::on_listViewImages_entered(const QModelIndex &index)
{
    DECL_TRACER("TResourceDialog::on_listViewImages_entered(const QModelIndex &index)");

    on_listViewImages_activated(index);
}

void TResourceDialog::on_tableViewDynamicImages_activated(const QModelIndex &index)
{
    DECL_TRACER("TResourceDialog::on_tableViewDynamicImages_activated(const QModelIndex &index)");

    mDynamicRowSelected = index.row();
    enableClipboardButtons();
}

void TResourceDialog::on_tableViewDynamicImages_doubleClicked(const QModelIndex &index)
{
    DECL_TRACER("TResourceDialog::on_tableViewDynamicImages_doubleClicked(const QModelIndex &index)");

    int row = index.row();
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewDynamicImages->model());
    QStandardItem *item = model->item(row, 0);
    editDynamicResource(item->text());
}

void TResourceDialog::on_tableViewSounds_activated(const QModelIndex &index)
{

}


void TResourceDialog::on_tableViewDynamicDataSources_activated(const QModelIndex &index)
{

}

void TResourceDialog::onClipboardChanged(QClipboard::Mode mode)
{
    DECL_TRACER("TResourceDialog::onClipboardChanged(QClipboard::Mode mode)");

    mClipboardMode = mode;
}

void TResourceDialog::onClipboardDataChanged()
{
    DECL_TRACER("TResourceDialog::onClipboardDataChanged()");

    if (!mClipboard)
        return;

    if (mTabSelected == SEL_IMAGES)
    {
        mClipboardPixmap = mClipboard->pixmap(mClipboardMode);

        if (mClipboardPixmap.isNull())
            ui->pushButtonPaste->setDisabled(true);
        else
            ui->pushButtonPaste->setDisabled(false);
    }
    else if (mTabSelected == SEL_DYNIMAGES)
    {
        mClipboardText = mClipboard->text(mClipboardMode);

        if (mClipboardText.isEmpty())
            ui->pushButtonPaste->setDisabled(true);
        else
            ui->pushButtonPaste->setDisabled(false);
    }
    else
        ui->pushButtonPaste->setDisabled(true);
}

bool TResourceDialog::parseDynamicImageFromClipboard(ConfigMain::RESOURCE_t *res, const QString& txt)
{
    DECL_TRACER("TResourceDialog::parseDynamicImageFromClipboard(ConfigMain::RESOURCE_t *res, const QString& txt)");

    if (!txt.contains(";") || !res)
        return false;

    QString text = txt;
    QStringList parts = text.split(';');

    if (parts.size() < 9)
        return false;

    if (parts[1] != "http" && parts[1] != "https" && parts[1] != "rtp" && parts[1] != "rtsp")
        return false;

    res->name = parts[0];
    res->protocol = parts[1];
    res->host = parts[2];
    res->path = parts[3];
    res->file = parts[4];
    res->user = parts[5];
    res->password = parts[6];
    res->refreshRate = parts[7].toInt();
    res->refreshStart = (parts[8].toInt() ? true : false);
    return true;
}

QPixmap TResourceDialog::sizeImage(const QSize& size, const QString& file)
{
    DECL_TRACER("TResourceDialog::sizeImage(const QSize& size, const QString& file)");

    QPixmap pm(file);
    return QPixmap(pm.scaled(size, Qt::KeepAspectRatio));
}

void TResourceDialog::importImagesToListView(const QStringList& images)
{
    DECL_TRACER("TResourceDialog::importImagesToListView(const QStringList& images)");

    if (images.empty())
        return;

    MSG_DEBUG("Selected " << images.count() << " images.");
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

        mLastOpenPath = *iter;

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
        item->setData(sizeImage(iconSize, *iter), Qt::DecorationRole);
        item->setSizeHint(gridSize);
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        model->appendRow(item);
        mImages.append(bn);
        TMaps::Current().addBitmap(*iter, 0, 0, 0, 0);
        mChanged = true;
    }

    model->sort(0);
    setLabel(LABEL_LISTVIEW, mImages.size(), "");

}

void TResourceDialog::removeItemFromListView(const QString& file, int row)
{
    DECL_TRACER("TResourceDialog::removeItemFromListView(const QString& file, int row)");

    MSG_DEBUG("Removing file and item " << file.toStdString() << " in row " << row);
    ui->listViewImages->model()->removeRow(row);
    TMaps::Current().removeBitmap(file);
    // Remove from interbal list
    QStringList::Iterator inIter;

    for (inIter = mImages.begin(); inIter != mImages.end(); ++inIter)
    {
        if (*inIter == file)
        {
            mImages.erase(inIter);
            break;
        }
    }
}

int TResourceDialog::getRowFromListView(const QString& file)
{
    DECL_TRACER("TResourceDialog::getRowFromListView(const QString& file)");

    MSG_DEBUG("Searching for item in row with name " << file.toStdString());
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->listViewImages->model());
    QList<QStandardItem *> list = model->findItems(file);

    if (list.isEmpty())
        return -1;

    MSG_DEBUG("Found item in row " << list[0]->row());
    return list[0]->row();
}

QStandardItem *TResourceDialog::getItemFromListView(const QString& file)
{
    DECL_TRACER("TResourceDialog::getItemromListView(const QString& file)");

    MSG_DEBUG("Searching for item with name " << file.toStdString());
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->listViewImages->model());
    QList<QStandardItem *> list = model->findItems(file);

    if (list.isEmpty())
        return nullptr;

    MSG_DEBUG("Found item in row " << list[0]->row());
    return list[0];
}

void TResourceDialog::renameImageFile(const QString& ori, const QString& tgt)
{
    DECL_TRACER("TResourceDialog::renameImageFile(const QString& ori, const QString& tgt)");

    MSG_DEBUG("Renaming file " << ori.toStdString() << " to " << tgt.toStdString());
    QStringList::Iterator iter;

    for (iter = mImages.begin(); iter != mImages.end(); ++iter)
    {
        if (*iter == ori)
        {
            *iter = tgt;
            MSG_DEBUG("File was found and renamed.");
            break;
        }
    }
}

void TResourceDialog::setLabel(LABEL_t lb, int number, const QString& text)
{
    DECL_TRACER("TResourceDialog::setLabel(LABEL_t lb, int number, const QString& text)");

    QString txt;

    switch (lb)
    {
        case LABEL_LISTVIEW:    txt = tr("%1 images in list.").arg(number); break;
        case LABEL_FILESCOPIED: txt = tr("Copied %1 files.").arg(number); break;
        case LABEL_RESOURCES:   txt = tr("%1 item(s)").arg(number); break;
    }

    ui->labelStatus->setText(txt);
}

void TResourceDialog::disableClipboardButtons()
{
    DECL_TRACER("TResourceDialog::disableClipboardButtons()");

    ui->pushButtonCut->setDisabled(true);
    ui->pushButtonCopy->setDisabled(true);
    ui->pushButtonPaste->setDisabled(true);
    ui->pushButtonDelete->setDisabled(true);
    ui->pushButtonRename->setDisabled(true);
    ui->pushButtonExport->setDisabled(true);
}

void TResourceDialog::enableClipboardButtons()
{
    DECL_TRACER("TResourceDialog::enableClipboardButtons()");

    ui->pushButtonCut->setDisabled(false);
    ui->pushButtonCopy->setDisabled(false);
    ui->pushButtonPaste->setDisabled(false);
    ui->pushButtonDelete->setDisabled(false);
    ui->pushButtonRename->setDisabled(false);
    ui->pushButtonExport->setDisabled(false);
}

void TResourceDialog::copyImagesToClipboard()
{
    DECL_TRACER("TResourceDialog::copyImagesToClipboard()");

    if (mTabSelected == SEL_IMAGES)
    {
        QItemSelectionModel *model = ui->listViewImages->selectionModel();

        if (!model || !model->hasSelection())
        {
            disableClipboardButtons();
            return;
        }

        if (mClipboard)
        {
            QModelIndexList list = model->selectedIndexes();
            QModelIndexList::Iterator iter;

            for (iter = list.begin(); iter != list.end(); ++iter)
            {
                QPixmap pm(mPathTemporary + "/images/" + iter->data().toString());
                mClipboard->setPixmap(pm);
            }
        }
    }
}

void TResourceDialog::copyDynamicImageToClipboard()
{
    DECL_TRACER("TResourceDialog::copyDynamicImageToClipboard()");

    if (mTabSelected != SEL_DYNIMAGES)
        return;

    QItemSelectionModel *selModel = ui->tableViewDynamicImages->selectionModel();

    if (!selModel || !selModel->hasSelection())
    {
        disableClipboardButtons();
        return;
    }

    if (!mClipboard)
        return;

    QModelIndexList list = selModel->selectedIndexes();
    int row = list[0].row();
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewDynamicImages->model());
    QString name = model->item(row, 0)->text();
    ConfigMain::RESOURCE_t res = TConfMain::Current().getDynamicResource(name);

    if (res.name.isEmpty())
        return;

    QString clipText = res.name + ";" + res.protocol + ";" + res.host + ";" +
                       res.path + ";" + res.file + ";" + res.user + ";" +
                       res.password + ";" + QString("%1;%2").arg(res.refreshRate).arg(res.refreshStart);
    mClipboard->setText(clipText);
}

void TResourceDialog::addDynamicResource(const ConfigMain::RESOURCE_t& res, QStandardItemModel *model)
{
    DECL_TRACER("TResourceDialog::addDynamicResource(const ConfigMain::RESOURCE_t& res, QStandardItemModel *model)");

    QStandardItemModel *dynModel = nullptr;

    if (model)
        dynModel = model;
    else
        dynModel = static_cast<QStandardItemModel *>(ui->tableViewDynamicImages->model());

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
    cell2->setText(getUrl(res));
    cell3->setText(QString("%1").arg(res.refreshRate));

    dynModel->setItem(row, 0, cell1);
    dynModel->setItem(row, 1, cell2);
    dynModel->setItem(row, 2, cell3);
}

void TResourceDialog::editDynamicResource(const QString& name, bool neu)
{
    DECL_TRACER("TResourceDialog::editDynamicResource(const QString& name, bool neu)");

    ConfigMain::RESOURCE_t res;
    TDynamicImageDialog dynDialog(this);

    if (!neu)
    {
        res = TConfMain::Current().getDynamicResource(name);

        dynDialog.setName(res.name);
        dynDialog.setProtocol(res.protocol);
        dynDialog.setHost(res.host);
        dynDialog.setPath(res.path);
        dynDialog.setFile(res.file);
        dynDialog.setUser(res.user);
        dynDialog.setPassword(res.password);
        dynDialog.setRefreshRate(res.refreshRate);
        dynDialog.setRefreshStart(res.refreshStart);
        dynDialog.setEdit(true);
    }

    if (dynDialog.exec() == QDialog::Rejected)
        return;

    if (neu)
        res.name = dynDialog.getName();

    res.protocol = dynDialog.getProtocol();
    res.host = dynDialog.getHost();
    res.path = dynDialog.getPath();
    res.file = dynDialog.getFile();
    res.user = dynDialog.getUser();
    res.password = dynDialog.getPassword();
    res.refreshRate = dynDialog.getRefreshRate();
    res.refreshStart = dynDialog.getRefreshStart();

    if (neu)
        addDynamicResource(res);
    else
    {
        QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewDynamicImages->model());

        if (!model)
            return;

        QList<QStandardItem*> list = model->findItems(name);

        if (list.empty())
            return;

        int row = list[0]->row();
        model->item(row, 1)->setText(getUrl(res));
        model->item(row, 2)->setText(QString("%1").arg(res.refreshRate));
    }

    TConfMain::Current().setDynamicResource(res);
    ui->tableViewDynamicImages->resizeColumnsToContents();
    setLabel(LABEL_RESOURCES, ui->tableViewDynamicImages->model()->rowCount(), "");
    mChanged = true;
}

QString TResourceDialog::getUrl(const ConfigMain::RESOURCE_t& res)
{
    DECL_TRACER("TResourceDialog::getUrl(const ConfigMain::RESOURCE_t& res)");

    QString txt = res.protocol + "://";

    if (!res.user.isEmpty())
        txt.append(res.user + "@");

    txt.append(res.host + "/");

    if (!res.path.isEmpty())
        txt.append(res.path + "/");

    txt.append(res.file);
    return txt;
}
