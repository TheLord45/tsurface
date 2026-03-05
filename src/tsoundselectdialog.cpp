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
#include <QStandardItem>
#include <QItemSelectionModel>

#include "tsoundselectdialog.h"
#include "ui_tsoundselectdialog.h"

#include "tmaps.h"
#include "tconfmain.h"
#include "tconfig.h"
#include "terror.h"
#include "tmisc.h"

TSoundSelectDialog::TSoundSelectDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TSoundSelectDialog)
{
    DECL_TRACER("TSoundSelectDialog::TSoundSelectDialog(QWidget *parent)");

    ui->setupUi(this);

    QStandardItemModel *soundModel = new QStandardItemModel(this);
    soundModel->setColumnCount(2);
    QStringList headers;
    headers << tr("Name") << tr("Size");
    soundModel->setHorizontalHeaderLabels(headers);
    mSoundFiles = TMaps::Current().getAllSoundFiles();
    QStringList::Iterator sndIter;
    addSoundFile("(None)", soundModel);     // First permanent entry is an empty file

    for (sndIter = mSoundFiles.begin(); sndIter != mSoundFiles.end(); ++sndIter)
        addSoundFile(*sndIter, soundModel);

    ui->tableViewSounds->setModel(soundModel);
    ui->tableViewSounds->resizeColumnsToContents();
    ui->tableViewSounds->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewSounds->setSelectionMode(QAbstractItemView::SingleSelection);
}

TSoundSelectDialog::~TSoundSelectDialog()
{
    DECL_TRACER("TSoundSelectDialog::~TSoundSelectDialog()");

    delete ui;
}

void TSoundSelectDialog::on_tableViewSounds_clicked(const QModelIndex &index)
{
    DECL_TRACER("TSoundSelectDialog::on_tableViewSounds_clicked(const QModelIndex &index)");

    int row = index.row();

    if (row == 0)
    {
        mSound.clear();
        return;
    }

    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewSounds->model());

    if (!model)
        return;

    QStandardItem *item = model->item(row, 0);
    mSound = item->text();
}

void TSoundSelectDialog::on_pushButtonImport_clicked()
{
    DECL_TRACER("TSoundSelectDialog::on_pushButtonImport_clicked()");

    QFileDialog fileDialog(this, tr("Load sound file"), TConfig::Current().getLastDirectory(), tr("Audio (*.wav *.mp3 *.wma *.flac *.aac *.alac *.eac3 *.ogg *.opus);;All (*)"));
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

        mSoundFiles.append(*flIter);
        TMaps::Current().addSound(*flIter);
        addSoundFile(basename(*flIter), nullptr);
    }
}

QString TSoundSelectDialog::getSound() const
{
    DECL_TRACER("TSoundSelectDialog::getSound()");

    return mSound;
}

void TSoundSelectDialog::addSoundFile(const QString& file, QStandardItemModel *model)
{
    DECL_TRACER("TSoundSelectDialog::addSoundFile(const QString& file, QStandardItemModel *model)");

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

    if (file != "(None)")
    {
        cell1->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MultimediaPlayer));

        QFile path(TConfMain::Current().getPathTemporary() + "/sounds/" + file);
        cell2->setText(QString("%1 Kib").arg(path.size() / 1024));
    }

    soundModel->setItem(row, 0, cell1);
    soundModel->setItem(row, 1, cell2);
    ui->tableViewSounds->resizeColumnsToContents();
}
