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
#include "tdynamicdatadialog.h"
#include "tdatamapdialog.h"
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
    // Sound files
    QStandardItemModel *soundModel = new QStandardItemModel(this);
    soundModel->setColumnCount(2);
    headers.clear();
    headers << tr("Name") << tr("Size");
    soundModel->setHorizontalHeaderLabels(headers);
    mSoundFiles = TMaps::Current().getAllSoundFiles();
    QStringList::Iterator sndIter;

    for (sndIter = mSoundFiles.begin(); sndIter != mSoundFiles.end(); ++sndIter)
        addSoundFile(*sndIter, soundModel);

    ui->tableViewSounds->setModel(soundModel);
    ui->tableViewSounds->resizeColumnsToContents();
    ui->tableViewSounds->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewSounds->setSelectionMode(QAbstractItemView::SingleSelection);
    // Dynamic data
    QStandardItemModel *dataModel = new QStandardItemModel(this);
    dataModel->setColumnCount(6);
    headers.clear();
    headers << tr("Name") << tr("URL") << tr("Format") << tr("Refresh") << tr("Mapping") << tr("Live");
    dataModel->setHorizontalHeaderLabels(headers);
    QList<ConfigMain::DATASOURCE_t> dataList = TConfMain::Current().getAllDynamicData();
    QList<ConfigMain::DATASOURCE_t>::Iterator dataIter;

    for (dataIter = dataList.begin(); dataIter != dataList.end(); ++dataIter)
        addDynamicData(*dataIter, dataModel);

    ui->tableViewDynamicDataSources->setModel(dataModel);
    ui->tableViewDynamicDataSources->resizeColumnsToContents();
    ui->tableViewDynamicDataSources->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewDynamicDataSources->setSelectionMode(QAbstractItemView::SingleSelection);
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
    else if (mTabSelected == SEL_SOUNDS)
    {
        copySoundFileToClipboard();
        removeSoundFile();
    }
    else if (mTabSelected == SEL_DATASOURCE)
    {
        copyDynamicDataToClipboard();
        removeDynamicData();
    }
}

void TResourceDialog::on_pushButtonCopy_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonCopy_clicked()");

    if (mTabSelected == SEL_IMAGES)
        copyImagesToClipboard();
    else if (mTabSelected == SEL_DYNIMAGES)
        copyDynamicImageToClipboard();
    else if (mTabSelected == SEL_SOUNDS)
        copySoundFileToClipboard();
    else if (mTabSelected == SEL_DATASOURCE)
        copyDynamicDataToClipboard();
}

void TResourceDialog::on_pushButtonPaste_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonPaste_clicked()");

    if (!mClipboard)
        return;

    if (mTabSelected == SEL_IMAGES && !mClipboardPixmap.isNull())
    {
        QString fname(QString("Unknown_%1.png").arg(getClipboardImageNumber()));
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
    else if (mTabSelected == SEL_SOUNDS)
    {
        if (mClipboardText.isEmpty())
            return;

        if (mClipboardText.length() > PATH_MAX || (mClipboardText.endsWith(".wav") == false && mClipboardText.endsWith(".mp3") == false))
            return;

        QFile f(mClipboardText);

        if (!f.exists())
            return;

        if (!f.copy(mPathTemporary + "/sounds/" + basename(mClipboardText)))
        {
            MSG_ERROR("Couldn't copy sound file \"" << mClipboardText.toStdString() << "\" to temporary folder!");
            return;
        }

        TMaps::Current().addSound(mClipboardText);
        addSoundFile(basename(mClipboardText));
        mChanged = true;
    }
    else if (mTabSelected == SEL_DATASOURCE)
    {
        if (mClipboardText.isEmpty())
            return;

        ConfigMain::DATASOURCE_t data = parseDynamicData(mClipboardText);

        if (data.name.isEmpty())
            return;

        addDynamicData(data);
        TConfMain::Current().setDynamicData(data);
        mChanged = true;
    }
}

void TResourceDialog::on_pushButtonDelete_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonDelete_clicked()");

    if (mTabSelected == SEL_IMAGES)
    {
        QItemSelectionModel *model = ui->listViewImages->selectionModel();

        if (haveSelected(model))
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

        if (!haveSelected(model))
            return;

        QModelIndexList list = model->selectedIndexes();
        QStandardItemModel *mod = static_cast<QStandardItemModel *>(ui->tableViewDynamicImages->model());
        int row = list[0].row();
        QString name = mod->item(row, 0)->text();
        mod->removeRow(row);
        TConfMain::Current().removeDynamicImage(name);
        setLabel(LABEL_RESOURCES, ui->tableViewDynamicImages->model()->rowCount(), "");
        disableClipboardButtons();
        mChanged = true;
    }
    else if (mTabSelected == SEL_SOUNDS)
        removeSoundFile();
    else if (mTabSelected == SEL_DATASOURCE)
        removeDynamicData();
}

void TResourceDialog::on_pushButtonRename_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonRename_clicked()");

    if (mTabSelected == SEL_IMAGES)
    {
        QItemSelectionModel *model = ui->listViewImages->selectionModel();

        if (haveSelected(model))
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

        if (haveSelected(model))
        {
            QModelIndexList list = model->selectedIndexes();
            int row = list[0].row();
            QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewDynamicImages->model());
            QStandardItem *item = model->item(row, 0);
            editDynamicResource(item->text());
        }
    }
    else if (mTabSelected == SEL_DATASOURCE)
    {
        QItemSelectionModel *model = ui->tableViewDynamicDataSources->selectionModel();

        if (!haveSelected(model))
            return;

        QModelIndexList list = model->selectedIndexes();
        QString name = list[0].data().toString();
        int row = list[0].row();
        QStandardItemModel *m = static_cast<QStandardItemModel *>(ui->tableViewDynamicDataSources->model());
        QStandardItem *item = m->item(row, 0);
        ConfigMain::DATASOURCE_t data = TConfMain::Current().getDynamicData(item->text());
        editDynamicData(data);
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
    else if (mTabSelected == SEL_SOUNDS)
        importSoundFile();
    else if (mTabSelected == SEL_DATASOURCE)
    {
        ConfigMain::DATASOURCE_t data;
        editDynamicData(data);
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

        if (!haveSelected(model))
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
    else if (mTabSelected == SEL_SOUNDS)
    {
        QItemSelectionModel *model = ui->tableViewSounds->selectionModel();

        if (!haveSelected(model))
            return;

        QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"), mLastOpenPath);

        if (dir.isEmpty())
            return;

        QModelIndexList list = model->selectedIndexes();

        if (list.empty())
        {
            disableClipboardButtons();
            return;
        }

        QString file = list[0].data().toString();
        QString target = dir + "/" + file;

        if (fs::exists(target.toStdString()))
        {
            int ret = QMessageBox::question(this, tr("File exists"), tr("The file <b>%1</b> exists already.<br>Do you want to overwrite it?"));

            if (ret == QMessageBox::No)
                return;
        }

        QFile f(mPathTemporary + "/sounds/" + file);

        if (!f.copy(target))
        {
            QMessageBox::critical(this,
                                  tr("Copy error"),
                                  tr("Error copying file <b>%1</b><br>%2").arg(file).arg(f.errorString()));
        }
    }
}

void TResourceDialog::on_pushButtonDataMap_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonDataMap_clicked()");

    QItemSelectionModel *model = ui->tableViewDynamicDataSources->selectionModel();

    if (!haveSelected(model))
        return;

    QModelIndexList list = model->selectedRows(0);
    QString name = ui->tableViewDynamicDataSources->model()->data(list[0]).toString();
    ConfigMain::DATASOURCE_t data = TConfMain::Current().getDynamicData(name);

    if (data.name.isEmpty())
    {
        MSG_WARNING("Got no data for " << name.toStdString() << "!");
        return;
    }

    TDataMapDialog mapDialog(this);
    mapDialog.setData(data);

    if (mapDialog.exec() == QDialog::Rejected)
        return;

    data.mapIdT1 = mapDialog.getPrimaryText();
    data.mapIdT2 = mapDialog.getSecondaryText();
    data.mapIdI1 = mapDialog.getImageName();
    data.sortOrder = mapDialog.getColumns();
    data.sortQuery = mapDialog.getQuery();
    data.sort = static_cast<ConfigMain::SORTMAP_t>(mapDialog.getSort());
    data.live = mapDialog.haveLive();
    TConfMain::Current().setDynamicData(data);
    QStandardItemModel *m = static_cast<QStandardItemModel *>(ui->tableViewDynamicDataSources->model());
    QStandardItem *item = m->item(list[0].row(), 5);
    item->setText(data.live ? "Yes" : "No");
    m->setItem(list[0].row(), 5, item);

    item = m->item(list[0].row(), 4);

    if (!data.mapIdT1.isEmpty() || !data.mapIdT2.isEmpty() || !data.mapIdI1.isEmpty())
        item->setText("Yes");

    m->setItem(list[0].row(), 4, item);

    mChanged = true;
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

        if (mTabSelected == SEL_DATASOURCE)
            ui->pushButtonExport->setDisabled(true);
        else
            ui->pushButtonExport->setDisabled(false);
    }
    else
    {
        ui->pushButtonImport->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));
        ui->pushButtonImport->setText(tr("Import"));
        ui->pushButtonRename->setIcon(QIcon(":/images/show-menu.png"));
        ui->pushButtonRename->setText(tr("Rename"));
        ui->pushButtonExport->setDisabled(false);
    }

    QItemSelectionModel *model = nullptr;

    switch(mTabSelected)
    {
        case SEL_IMAGES:
            setLabel(LABEL_LISTVIEW, ui->listViewImages->model()->rowCount(), "");
            model = ui->tableViewDynamicDataSources->selectionModel();
        break;

        case SEL_DYNIMAGES:
            setLabel(LABEL_RESOURCES, ui->tableViewDynamicImages->model()->rowCount(), "");
        break;

        case SEL_SOUNDS:
            setLabel(LABEL_SOUND, ui->tableViewSounds->model()->rowCount(), "");
        break;

        case SEL_DATASOURCE:
            setLabel(LABEL_RESOURCES, ui->tableViewDynamicDataSources->model()->rowCount(), "");
        break;
    }
}

void TResourceDialog::on_listViewImages_activated(const QModelIndex &index)
{
    DECL_TRACER("TResourceDialog::on_listViewImages_activated(const QModelIndex &index)");

    QItemSelectionModel *model = ui->listViewImages->selectionModel();

    if (model && model->hasSelection())
        enableClipboardButtons();
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

    if (!model)
        return;

    QStandardItem *item = model->item(row, 0);
    editDynamicResource(item->text());
}

void TResourceDialog::on_tableViewSounds_activated(const QModelIndex &index)
{
    DECL_TRACER("TResourceDialog::on_tableViewSounds_activated(const QModelIndex &index)");

    QItemSelectionModel *model = ui->tableViewSounds->selectionModel();

    if (model && model->hasSelection())
        enableClipboardButtons();
    else
        disableClipboardButtons();
}

void TResourceDialog::on_tableViewDynamicDataSources_activated(const QModelIndex &index)
{
    DECL_TRACER("TResourceDialog::on_tableViewDynamicDataSources_activated(const QModelIndex &index)");

    QItemSelectionModel *model = ui->tableViewDynamicDataSources->selectionModel();

    if (model && model->hasSelection())
        enableClipboardButtons();
    else
        disableClipboardButtons();
}

void TResourceDialog::on_tableViewDynamicDataSources_doubleClicked(const QModelIndex &index)
{
    DECL_TRACER("TResourceDialog::on_tableViewDynamicDataSources_doubleClicked(const QModelIndex &index)");

    int row = index.row();
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewDynamicDataSources->model());

    if (!model)
        return;

    QStandardItem *item = model->item(row, 0);
    QString name = item->text();
    ConfigMain::DATASOURCE_t data = TConfMain::Current().getDynamicData(name);
    editDynamicData(data);
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
    else if (mTabSelected == SEL_DYNIMAGES || mTabSelected == SEL_SOUNDS || mTabSelected == SEL_DATASOURCE)
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

int TResourceDialog::getClipboardImageNumber()
{
    DECL_TRACER("TResourceDialog::getClipboardImageNumber()");

    QStringList::Iterator iter;

    for (iter = mImages.begin(); iter != mImages.end(); ++iter)
    {
        if (iter->startsWith("Unknown_"))
        {
            qsizetype pos = iter->lastIndexOf('_');
            int num = iter->mid(pos + 1).toInt();

            if (num > 0)
            {
                mClipboardPixmapNumber = num + 1;
                return mClipboardPixmapNumber;
            }
        }
    }

    mClipboardPixmapNumber++;
    return mClipboardPixmapNumber;
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
        case LABEL_SOUND:       txt = tr("%1 sound file(s) in list.").arg(number); break;
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

    if (mTabSelected != SEL_DATASOURCE)
        ui->pushButtonDataMap->setDisabled(true);
}

void TResourceDialog::enableClipboardButtons()
{
    DECL_TRACER("TResourceDialog::enableClipboardButtons()");

    ui->pushButtonCut->setDisabled(false);
    ui->pushButtonCopy->setDisabled(false);
    ui->pushButtonPaste->setDisabled(false);
    ui->pushButtonDelete->setDisabled(false);
    ui->pushButtonRename->setDisabled(false);

    if (mTabSelected != SEL_DATASOURCE)
        ui->pushButtonExport->setDisabled(false);

    if (mTabSelected == SEL_DATASOURCE)
    {
        QItemSelectionModel *model = ui->tableViewDynamicDataSources->selectionModel();

        if (model && model->hasSelection())
            ui->pushButtonDataMap->setDisabled(false);
        else
            ui->pushButtonDataMap->setDisabled(true);
    }
}

void TResourceDialog::copyImagesToClipboard()
{
    DECL_TRACER("TResourceDialog::copyImagesToClipboard()");

    if (mTabSelected == SEL_IMAGES)
    {
        QItemSelectionModel *model = ui->listViewImages->selectionModel();

        if (!haveSelected(model))
            return;

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

    if (!haveSelected(selModel))
        return;

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

void TResourceDialog::addDynamicData(const ConfigMain::DATASOURCE_t& data, QStandardItemModel *model)
{
    DECL_TRACER("TResourceDialog::addDynamicData(const ConfigMain::DATASOURCE_t& data, QStandardItemModel *model)");

    QStandardItemModel *dataModel = nullptr;

    if (model)
        dataModel = model;
    else
        dataModel = static_cast<QStandardItemModel *>(ui->tableViewDynamicDataSources->model());

    if (!dataModel)
        return;

    int row = dataModel->rowCount();
    QStandardItem *cell1 = new QStandardItem;
    QStandardItem *cell2 = new QStandardItem;
    QStandardItem *cell3 = new QStandardItem;
    QStandardItem *cell4 = new QStandardItem;
    QStandardItem *cell5 = new QStandardItem;
    QStandardItem *cell6 = new QStandardItem;

    cell1->setEditable(false);
    cell2->setEditable(false);
    cell3->setEditable(false);
    cell4->setEditable(false);
    cell5->setEditable(false);
    cell6->setEditable(false);

    cell1->setText(data.name);
    cell2->setText(getUrl(data));

    cell3->setText(data.format);
    cell4->setText(QString("%1").arg(data.refreshRate));

    if (data.mapIdT1.isEmpty() && data.mapIdT2.isEmpty() && data.mapIdI1.isEmpty())
        cell5->setText(tr("No"));
    else
        cell5->setText(tr("Yes"));

    cell6->setText(data.live ? tr("Yes") : tr("No"));

    dataModel->setItem(row, 0, cell1);
    dataModel->setItem(row, 1, cell2);
    dataModel->setItem(row, 2, cell3);
    dataModel->setItem(row, 3, cell4);
    dataModel->setItem(row, 4, cell5);
    dataModel->setItem(row, 5, cell6);
}

void TResourceDialog::editDynamicData(ConfigMain::DATASOURCE_t& data)
{
    DECL_TRACER("TResourceDialog::TResourceDialog::editDynamicData(const ConfigMain::DATASOURCE_t& data)");

    TDynamicDataDialog dataDialog(this);

    dataDialog.setName(data.name);
    dataDialog.setProtocol(data.protocol);
    dataDialog.setHost(data.host);
    dataDialog.setPath(data.path);
    dataDialog.setFile(data.file);
    dataDialog.setUser(data.user);
    dataDialog.setPassword(data.password);
    dataDialog.setRefreshRate(data.refreshRate);
    dataDialog.setForceData(data.force);
    dataDialog.setFormat(static_cast<TDynamicDataDialog::FORMAT_t>(getFormatFromString(data.format)));

    if (dataDialog.exec() == QDialog::Rejected)
        return;

    bool neu = false;

    if (data.name.isEmpty())
    {
        data.name = dataDialog.getName();
        neu = true;
    }

    data.protocol = dataDialog.getProtocol();
    data.host = dataDialog.getHost();
    data.path = dataDialog.getPath();
    data.file = dataDialog.getFile();
    data.user = dataDialog.getUser();
    data.password = dataDialog.getPassword();
    data.refreshRate = dataDialog.getRefreshRate();
    data.force = dataDialog.getForceData();
    data.protocol = dataDialog.getProtocol();
    data.format = getFormat(dataDialog.getFormat());

    if (neu)
        addDynamicData(data);
    else
    {
        QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewDynamicDataSources->model());

        if (!model)
            return;

        QList<QStandardItem*> list = model->findItems(data.name);

        if (list.empty())
            return;

        int row = list[0]->row();
        model->item(row, 1)->setText(getUrl(data));
        model->item(row, 2)->setText(data.format);
        model->item(row, 3)->setText(QString("%1").arg(data.refreshRate));

        if (data.mapIdT1.isEmpty() && data.mapIdT2.isEmpty() && data.mapIdI1.isEmpty())
            model->item(row, 4)->setText(tr("No"));
        else
            model->item(row, 4)->setText(tr("Yes"));

        model->item(row, 5)->setText(data.live ? tr("Yes") : tr("No"));
    }

    TConfMain::Current().setDynamicData(data);
    ui->tableViewDynamicDataSources->resizeColumnsToContents();
    setLabel(LABEL_RESOURCES, ui->tableViewDynamicDataSources->model()->rowCount(), "");
    mChanged = true;
}

void TResourceDialog::removeDynamicData()
{
    DECL_TRACER("TResourceDialog::removeDynamicData()");

    QItemSelectionModel *model = ui->tableViewDynamicDataSources->selectionModel();

    if (!haveSelected(model))
        return;

    QModelIndexList idxList = model->selectedRows();
    QString name = idxList[0].data().toString();
    ui->tableViewDynamicDataSources->model()->removeRow(idxList[0].row());
    TConfMain::Current().removeDynamicData(name);
    disableClipboardButtons();
    setLabel(LABEL_RESOURCES, ui->tableViewDynamicDataSources->model()->rowCount(), "");
    mChanged = true;
}

void TResourceDialog::copyDynamicDataToClipboard()
{
    DECL_TRACER("TResourceDialog::copyDynamicDataToClipboard()");

    if (!mClipboard)
        return;

    QItemSelectionModel *model = ui->tableViewDynamicDataSources->selectionModel();

    if (!haveSelected(model))
        return;

    QModelIndexList idxList = model->selectedRows();
    QString name = idxList[0].data().toString();
    ConfigMain::DATASOURCE_t data = TConfMain::Current().getDynamicData(name);
    QString txt = data.name + ";" + data.protocol + ";" + data.host + ";" + data.path + ";" +
                  data.file + ";" + data.user + ";" + data.password + ";" + data.format + ";" +
                  (data.force ? "true" : "false") + ";" + QString("%1").arg(data.refreshRate);

    mClipboard->setText(txt);
}

ConfigMain::DATASOURCE_t TResourceDialog::parseDynamicData(const QString& txt)
{
    DECL_TRACER("TResourceDialog::parseDynamicData(const QString& txt)");

    ConfigMain::DATASOURCE_t data;

    if (!txt.contains(";") || txt.length() > 512)
        return data;

    QStringList parts = txt.split(';');

    if (parts.size() < 10 || (parts[1] != "http" && parts[1] != "https"))
        return data;

    if (parts[7] != "xport-s" && parts[7] != "csv-headers" && parts[7] != "csv")
        return data;

    if (parts[8] != "true" && parts[8] != "false")
        return data;

    data.name = parts[0];
    data.protocol = parts[1];
    data.host = parts[2];
    data.path = parts[3];
    data.file = parts[4];
    data.user = parts[5];
    data.password = parts[6];
    data.format = parts[7];

    if (parts[8] == "true")
        data.force = true;
    else
        data.force = false;

    data.refreshRate = parts[9].toInt();
    return data;
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

QString TResourceDialog::getUrl(const ConfigMain::DATASOURCE_t& data)
{
    DECL_TRACER("TResourceDialog::getUrl(const ConfigMain::DATASOURCE_t& data)");

    QString txt = data.protocol + "://";

    if (!data.user.isEmpty())
        txt.append(data.user + "@");

    txt.append(data.host + "/");

    if (!data.path.isEmpty())
        txt.append(data.path + "/");

    txt.append(data.file);
    return txt;
}

QString TResourceDialog::getFormat(int format)
{
    DECL_TRACER("TResourceDialog::getFormat(int format)");

    switch(format)
    {
        case TDynamicDataDialog::FORMAT_XPORT: return "xport-s";
        case TDynamicDataDialog::FORMAT_CSVHEAD: return "csv-headers";
        case TDynamicDataDialog::FORMAT_CSV: return "csv";
    }

    return QString();
}

int TResourceDialog::getFormatFromString(const QString& format)
{
    DECL_TRACER("TResourceDialog::getFormatFromString(const QString& format)");

    if (format.compare("xport-s", Qt::CaseInsensitive) == 0)
        return TDynamicDataDialog::FORMAT_XPORT;
    else if (format.compare("csv-headers", Qt::CaseInsensitive) == 0)
        return TDynamicDataDialog::FORMAT_CSVHEAD;
    else if (format.compare("csv", Qt::CaseInsensitive) == 0)
        return TDynamicDataDialog::FORMAT_CSV;
    else
        return TDynamicDataDialog::FORMAT_XPORT;
}

void TResourceDialog::addSoundFile(const QString& file, QStandardItemModel *model)
{
    DECL_TRACER("TResourceDialog::addSoundFile(const QString& file, QStandardItemModel *model)");

    QStandardItemModel *soundModel = nullptr;

    if (model)
        soundModel = model;
    else
        soundModel = static_cast<QStandardItemModel *>(ui->tableViewSounds->model());

    if (!soundModel)
        return;

    int row = soundModel->rowCount();
    QStandardItem *cell1 = new QStandardItem;
    QStandardItem *cell2 = new QStandardItem;

    cell1->setEditable(false);
    cell2->setEditable(false);

    cell1->setText(file);
//    cell1->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaOptical));
    cell1->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MultimediaPlayer));

    QFile path(mPathTemporary + "/sounds/" + file);
    cell2->setText(QString("%1 Kib").arg(path.size() / 1024));

    soundModel->setItem(row, 0, cell1);
    soundModel->setItem(row, 1, cell2);
    ui->tableViewSounds->resizeColumnsToContents();
}

void TResourceDialog::importSoundFile()
{
    DECL_TRACER("TResourceDialog::importSoundFile()");

    QFileDialog fileDialog(this, tr("Load sound file"), mLastOpenPath, tr("Audio (*.wav *.mp3 *.wma *.flac *.aac *.alac *.eac3 *.ogg *.opus);;All (*)"));
    fileDialog.setFileMode(QFileDialog::ExistingFiles);

    if (fileDialog.exec() == QDialog::Rejected)
        return;

    QStringList files = fileDialog.selectedFiles();

    if (files.empty())
        return;

    QStringList::Iterator flIter;

    for (flIter = files.begin(); flIter != files.end(); ++flIter)
    {
        // Make sure the file is not already in the list
        QStringList::Iterator iter;
        bool haveFile = false;

        for (iter = mSoundFiles.begin(); iter != mSoundFiles.end(); ++iter)
        {
            if (*iter == *flIter)
            {
                haveFile = true;
                break;
            }
        }

        if (haveFile)
            continue;

        TMaps::Current().addSound(*flIter);
        addSoundFile(basename(*flIter));
        mLastOpenPath = pathname(*flIter);
        mChanged = true;
    }

    setLabel(LABEL_SOUND, mSoundFiles.size(), "");
}

void TResourceDialog::removeSoundFile()
{
    DECL_TRACER("TResourceDialog::removeSoundFile()");

    QItemSelectionModel *model = ui->tableViewSounds->selectionModel();

    if (!haveSelected(model))
        return;

    QModelIndexList idxList = model->selectedRows();
    QString file = idxList[0].data().toString();

    if (!TMaps::Current().removeSound(file))
        return;

    ui->tableViewSounds->model()->removeRow(idxList[0].row());
    disableClipboardButtons();
    setLabel(LABEL_SOUND, mSoundFiles.size(), "");
    mChanged = true;
}

void TResourceDialog::copySoundFileToClipboard()
{
    DECL_TRACER("TResourceDialog::copySoundFileToClipboard()");

    QItemSelectionModel *model = ui->tableViewSounds->selectionModel();

    if (!haveSelected(model))
        return;

    QModelIndexList idxList = model->selectedRows();
    QString file = idxList[0].data().toString();

    if (mClipboard)
        mClipboard->setText(file);
}

bool TResourceDialog::haveSelected(QItemSelectionModel *model)
{
    DECL_TRACER("TResourceDialog::haveSelected(QItemSelectionModel *model)");

    if (!model || !model->hasSelection())
    {
        disableClipboardButtons();
        return false;
    }

    QModelIndexList idxList = model->selectedRows();

    if (idxList.empty())
    {
        disableClipboardButtons();
        return false;
    }

    return true;
}
