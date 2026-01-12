#include <filesystem>

#include <QFileDialog>
#include <QIcon>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemSelectionModel>
#include <QMessageBox>

#include "tresourcedialog.h"
#include "ui_tresourcedialog.h"
#include "tmaps.h"
#include "tconfmain.h"
#include "terror.h"
#include "tmisc.h"

namespace fs = std::filesystem;
QSize iconSize(100, 100);
QSize gridSize(128, 128);

TResourceDialog::TResourceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TResourceDialog)
{
    ui->setupUi(this);

    mPathTemplate = TConfMain::Current().getPathTemporary();
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
        QString file = mPathTemplate + "/images/" + *iter;
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

    ui->pushButtonCut->setDisabled(true);
    ui->pushButtonCopy->setDisabled(true);
    ui->pushButtonPaste->setDisabled(true);
    ui->pushButtonDelete->setDisabled(true);
    ui->pushButtonUndo->setDisabled(true);
    ui->pushButtonRedo->setDisabled(true);
    ui->pushButtonExport->setDisabled(true);
    ui->pushButtonRename->setDisabled(true);
}

TResourceDialog::~TResourceDialog()
{
    delete ui;
}

void TResourceDialog::on_pushButtonCut_clicked()
{

}

void TResourceDialog::on_pushButtonCopy_clicked()
{

}

void TResourceDialog::on_pushButtonPaste_clicked()
{

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
        ui->pushButtonCut->setDisabled(true);
        ui->pushButtonCopy->setDisabled(true);
        ui->pushButtonDelete->setDisabled(true);
        ui->pushButtonExport->setDisabled(true);
        setLabel(LABEL_LISTVIEW, ui->listViewImages->model()->rowCount(), "");
    }
}


void TResourceDialog::on_pushButtonRename_clicked()
{

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

    QFileDialog fileDialog(this, tr("Import Image"), mLastOpenPath, tr("Image (*.png *.jpg);;All (*)"));
    fileDialog.setFileMode(QFileDialog::ExistingFiles);

    if (fileDialog.exec() == QDialog::Rejected)
        return;

    QStringList images = fileDialog.selectedFiles();

    if (images.empty())
        return;

    MSG_DEBUG("Selected " << images.count() << " images.");
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->listViewImages->model());
    QStringList::Iterator iter;

    for (iter = images.begin(); iter != images.end(); ++iter)
    {
        MSG_DEBUG("Working on file " << iter->toStdString());
        mLastOpenPath = basename(*iter);

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

void TResourceDialog::on_pushButtonExport_clicked()
{

}


void TResourceDialog::on_pushButtonDataMap_clicked()
{

}


void TResourceDialog::on_comboBoxStyle_currentIndexChanged(int index)
{
    DECL_TRACER("TResourceDialog::on_comboBoxStyle_currentIndexChanged(int index)");

    mShowType = static_cast<SHOW_t>(index);
}


void TResourceDialog::on_listViewImages_activated(const QModelIndex &index)
{
    DECL_TRACER("TResourceDialog::on_listViewImages_activated(const QModelIndex &index)");

    QItemSelectionModel *model = ui->listViewImages->selectionModel();

    if (model && model->hasSelection())
    {
        ui->pushButtonCut->setDisabled(false);
        ui->pushButtonCopy->setDisabled(false);
        ui->pushButtonDelete->setDisabled(false);
        ui->pushButtonExport->setDisabled(false);
        QModelIndexList list = model->selectedIndexes();
    }
    else
    {
        ui->pushButtonCut->setDisabled(true);
        ui->pushButtonCopy->setDisabled(true);
        ui->pushButtonDelete->setDisabled(true);
        ui->pushButtonExport->setDisabled(true);
    }
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

QPixmap TResourceDialog::sizeImage(const QSize& size, const QString& file)
{
    DECL_TRACER("TResourceDialog::sizeImage(const QSize& size, const QString& file)");

    QPixmap pm(file);
    return QPixmap(pm.scaled(size, Qt::KeepAspectRatio));
}

void TResourceDialog::removeItemFromListView(const QString& file, int row)
{
    DECL_TRACER("TResourceDialog::removeItemFromListView(const QString& file, int row)");

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

void TResourceDialog::setLabel(LABEL_t lb, int number, const QString& text)
{
    DECL_TRACER("TResourceDialog::setLabel(LABEL_t lb, int number, const QString& text)");

    QString txt;

    switch (lb)
    {
        case LABEL_LISTVIEW:    txt = tr("%1 images in list.").arg(number); break;
        case LABEL_RESOURCES:   txt = text; break;
    }

    ui->labelStatus->setText(txt);
}
