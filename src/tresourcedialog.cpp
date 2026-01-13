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

    ui->comboBoxStyle->setItemIcon(0, QIcon(":/images/view-list-tree.svg"));
    ui->comboBoxStyle->setItemIcon(1, QIcon(":/images/view-list-details.svg"));
    ui->comboBoxStyle->setItemIcon(2, QIcon(":/images/view-list-icons.svg"));
    ui->comboBoxStyle->setCurrentIndex(mShowType);

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

    disableClipboardButtons();
    ui->pushButtonUndo->setDisabled(true);
    ui->pushButtonRedo->setDisabled(true);
    ui->pushButtonRename->setDisabled(true);

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
}

void TResourceDialog::on_pushButtonCopy_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonCopy_clicked()");

    if (mTabSelected == SEL_IMAGES)
        copyImagesToClipboard();
}

void TResourceDialog::on_pushButtonPaste_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonPaste_clicked()");

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
        mChanged = true;
    }
}

void TResourceDialog::on_pushButtonDelete_clicked()
{
    DECL_TRACER("TResourceDialog::on_pushButtonDelete_clicked()");

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
            }

            QStandardItem *item = getItemFromListView(srcFile);

            if (item)
                item->setData(newFile, Qt::DisplayRole);

            renameImageFile(srcFile, newFile);
            TMaps::Current().renameBitmap(srcFile, newFile);
            mChanged = true;
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

    if (mTabSelected != SEL_IMAGES)
        return;

    mImportImagesDialog = new QFileDialog(this, tr("Import Image"), mLastOpenPath, tr("Image (*.png *.jpg);;All (*)"));
    mImportImagesDialog->setFileMode(QFileDialog::ExistingFiles);
    connect(mImportImagesDialog, &QFileDialog::finished, this, &TResourceDialog::onImageImportFinished);
    mImportImagesDialog->open();
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

}


void TResourceDialog::on_tableViewDynamicImages_doubleClicked(const QModelIndex &index)
{

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

    if (mTabSelected == SEL_IMAGES)
    {
        if (mClipboard)
        {
            mClipboardPixmap = mClipboard->pixmap(mClipboardMode);

            if (mClipboardPixmap.isNull())
                ui->pushButtonPaste->setDisabled(true);
            else
                ui->pushButtonPaste->setDisabled(false);
        }
    }
    else
        ui->pushButtonPaste->setDisabled(true);
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
        case LABEL_RESOURCES:   txt = text; break;
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
