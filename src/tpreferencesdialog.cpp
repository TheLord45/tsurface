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
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardItemModel>

#include <filesystem>

#include "tpreferencesdialog.h"
#include "telementcolorselector.h"
#include "tcheckerboardwidget.h"
#include "ui_tpreferencesdialog.h"

#include "tconfig.h"
#include "terror.h"
#include "tmisc.h"

namespace fs = std::filesystem;

TPreferencesDialog::TPreferencesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TPreferencesDialog)
{
    DECL_TRACER("TPreferencesDialog::TPreferencesDialog(QWidget *parent)");

    ui->setupUi(this);

    init();
}

TPreferencesDialog::~TPreferencesDialog()
{
    delete ui;
}

void TPreferencesDialog::init(INIT_t i)
{
    DECL_TRACER("TPreferencesDialog::init()");

    if (i == INIT_ALL || i == INIT_APPLICATION)
    {
        ui->checkBoxSystemGeneratedName->setCheckState(TConfig::Current().getSystemGeneratedName() ? Qt::Checked : Qt::Unchecked);
        ui->checkBoxReloadLastWorkspace->setCheckState(TConfig::Current().getReloadLastWorkspace() ? Qt::Checked : Qt::Unchecked);
        ui->checkBoxExpandWorkspaceNavigator->setCheckState(TConfig::Current().getExpandWorkspaceNavigator() ? Qt::Checked : Qt::Unchecked);
        ui->checkBoxDeleteResource->setCheckState(TConfig::Current().getWarnOnDeletingSources() ? Qt::Checked : Qt::Unchecked);
        ui->checkBoxFlushClosedPages->setCheckState(TConfig::Current().getFlushClosedPages() ? Qt::Checked : Qt::Unchecked);
        ui->checkBoxCreateBackup->setCheckState(TConfig::Current().getCreateBackup() ? Qt::Checked : Qt::Unchecked);
        ui->checkBoxPostfix->setCheckState(TConfig::Current().getUsePostfix() ? Qt::Checked : Qt::Unchecked);
        ui->checkBoxRetainSelectedTool->setCheckState(TConfig::Current().getRetainSelectedTool() ? Qt::Checked : Qt::Unchecked);
        ui->spinBoxChacheSize->setValue(TConfig::Current().getImageCacheSize());
    }

    if (i == INIT_ALL || i == INIT_APPEARANCE)
    {
        int index = 0;

        switch(TConfig::Current().getInitialZoom())
        {
            case 25:  index = 0; break;
            case 50:  index = 1; break;
            case 75:  index = 2; break;
            case 100: index = 3; break;
            case 150: index = 4; break;
            case 200: index = 5; break;
            case 400: index = 6; break;
            case -1:  index = 7; break;
            case -2:  index = 8; break;
            case -3:  index = 9; break;
            case -4:  index = 10; break;
        }

        ui->comboBoxInitialZoom->setCurrentIndex(index);
        ui->lineEditVisibleSize->setText(QString("%1").arg(TConfig::Current().getVisibleSize()));
        // Add gutter color
        mGutterColor = TConfig::Current().getGutterColor();

        if (!mColGutter)
        {
            mColGutter = new TElementColorSelector(mGutterColor, "GutterColor", true, ui->frameGutterColor);
            connect(mColGutter, &TElementColorSelector::colorChanged, this, &TPreferencesDialog::onColorChanged);
        }
        else
            mColGutter->setColor(mGutterColor);

        ui->comboBoxGridStyle->setCurrentIndex(TConfig::Current().getGridStyle());
        ui->spinBoxGridSize->setValue(TConfig::Current().getGridSize());
        ui->spinBoxSnapTolerance->setValue(TConfig::Current().getSnapTolerance());
        // Add grid color
        mGridColor = TConfig::Current().getGridColor();

        if (!mColGrid)
        {
            mColGrid = new TElementColorSelector(mGridColor, "GridColor", true, ui->frameGridColor);
            connect(mColGrid, &TElementColorSelector::colorChanged, this, &TPreferencesDialog::onColorChanged);
        }
        else
            mColGrid->setColor(mGridColor);

        mTransparencyStyle = TConfig::Current().getTransparencyStyle();
        ui->comboBoxStyle->setCurrentIndex(mTransparencyStyle);
        ui->comboBoxSize->setCurrentIndex(TConfig::Current().getTransparencySize());

        if (mTransparencyStyle != 3)     // Custom?
        {
            ui->labelCustomColors->setDisabled(true);
            ui->frameCustomColor1->setDisabled(true);
            ui->frameCustomColor2->setDisabled(true);
        }
        else
        {
            ui->labelCustomColors->setEnabled(true);
            ui->frameCustomColor1->setEnabled(true);
            ui->frameCustomColor2->setEnabled(true);
        }

        // Set checkerboard pattern
        int square = 0;

        switch(TConfig::Current().getTransparencySize())
        {
            case 0: square = 5; break;      // Tiny
            case 1: square = 10; break;     // Small (default)
            case 2: square = 15; break;     // Medium
            case 3: square = 20; break;     // Large

            default:
                square = 10;
        }

        ui->labelCustomBackground->setSquareSize(square);
        // Add Custom transparency color
        mTransparencyCustomColor1 = TConfig::Current().getTransparencyCustomColor1();
        mTransparencyCustomColor2 = TConfig::Current().getTransparencyCustomColor2();

        if (!mColTrans1)
        {
            mColTrans1 = new TElementColorSelector(mTransparencyCustomColor1, "TransparencyColor1", true, ui->frameCustomColor1);
            connect(mColTrans1, &TElementColorSelector::colorChanged, this, &TPreferencesDialog::onColorChanged);
        }
        else
            mColTrans1->setColor(mTransparencyCustomColor1);

        if (!mColTrans2)
        {
            mColTrans2 = new TElementColorSelector(mTransparencyCustomColor2, "TransparencyColor2", true, ui->frameCustomColor2);
            connect(mColTrans2, &TElementColorSelector::colorChanged, this, &TPreferencesDialog::onColorChanged);
        }
        else
            mColTrans2->setColor(mTransparencyCustomColor2);

        setCheckerboardColor();
    }

    if (i == INIT_ALL || i == INIT_DIRECTORIES)
    {
        ui->lineEditFolderPanels->setText(TConfig::Current().getFilesPanels());
        ui->lineEditFolderBackup->setText(TConfig::Current().getFilesBackups());
        ui->lineEditFolderTemp->setText(TConfig::Current().getFilesTemp());

        ui->lineEditFolderTemp->setDisabled(true);
        ui->toolButtonFolderTemp->setDisabled(true);
    }

    if (i == INIT_ALL || i == INIT_EDITORS)
    {
        mEditorsImage = TConfig::Current().getEditorsImage();
        mEditorsSound = TConfig::Current().getEditorsSound();

        if (ui->comboBoxEditors->currentIndex() == 0)
            fillImageEditorTable();
        else
            fillSoundEditorTable();
    }

    if (i == INIT_ALL || i == INIT_UNDO)
    {
        ui->checkBoxEnableUndo->setChecked(TConfig::Current().getEnableUndoSystem());
        ui->spinBoxUndoLevels->setValue(TConfig::Current().getUndoLevels());
        ui->checkBoxShowAffectedPages->setChecked(TConfig::Current().getUndoShowAffectedPages());
        ui->checkBoxChangeSelection->setChecked(TConfig::Current().getUndoChangeSelection());
        ui->checkBoxEnableRedo->setChecked(TConfig::Current().getRedoEnableSystem());

        if (!TConfig::Current().getEnableUndoSystem())
        {
            ui->spinBoxUndoLevels->setDisabled(true);
            ui->checkBoxShowAffectedPages->setDisabled(true);
            ui->checkBoxChangeSelection->setDisabled(true);
            ui->checkBoxEnableRedo->setDisabled(true);
        }
    }

    mInitialized = true;
}

void TPreferencesDialog::setCheckerboardColor()
{
    DECL_TRACER("TPreferencesDialog::setCheckerboardColor()");

    if (mTransparencyStyle != 3)     // Custom?
    {
        ui->labelCustomColors->setDisabled(true);
        ui->frameCustomColor1->setDisabled(true);
        ui->frameCustomColor2->setDisabled(true);
    }
    else
    {
        ui->labelCustomColors->setEnabled(true);
        ui->frameCustomColor1->setEnabled(true);
        ui->frameCustomColor2->setEnabled(true);
    }

    QColor dark, light;

    switch(mTransparencyStyle)
    {
        case 0:
            dark = Qt::lightGray;
            light = Qt::white;
        break;

        case 1:
            dark = Qt::gray;
            light = Qt::lightGray;
        break;

        case 2:
            dark = Qt::darkGray;
            light = Qt::gray;
        break;

        case 3:
            dark = mTransparencyCustomColor2;
            light = mTransparencyCustomColor1;
        break;
    }

    ui->labelCustomBackground->setDarkColor(dark);
    ui->labelCustomBackground->setLightColor(light);
}
void TPreferencesDialog::on_checkBoxSystemGeneratedName_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxSystemGeneratedName_checkStateChanged(const Qt::CheckState &arg1)");

    if (!mInitialized)
        return;

    mSystemGeneratedName = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxReloadLastWorkspace_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxReloadLastWorkspace_checkStateChanged(const Qt::CheckState &arg1)");

    if (!mInitialized)
        return;

    mReloadLastWorkspace = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxExpandWorkspaceNavigator_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxExpandWorkspaceNavigator_checkStateChanged(const Qt::CheckState &arg1)");

    if (!mInitialized)
        return;

    mExpandWorkspaceNavigator = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxDeleteResource_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxDeleteResource_checkStateChanged(const Qt::CheckState &arg1)");

    if (!mInitialized)
        return;

    mWarnOnDeletingSources = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxFlushClosedPages_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxFlushClosedPages_checkStateChanged(const Qt::CheckState &arg1)");

    if (!mInitialized)
        return;

    mFlushClosedPages = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxCreateBackup_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxCreateBackup_checkStateChanged(const Qt::CheckState &arg1)");

    if (!mInitialized)
        return;

    mCreateBackup = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxPostfix_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxPostfix_checkStateChanged(const Qt::CheckState &arg1)");

    if (!mInitialized)
        return;

    mUsePostfix = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxRetainSelectedTool_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxRetainSelectedTool_checkStateChanged(const Qt::CheckState &arg1)");

    if (!mInitialized)
        return;

    mRetainSelectedTool = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_spinBoxChacheSize_valueChanged(int arg1)
{
    DECL_TRACER("TPreferencesDialog::on_spinBoxChacheSize_valueChanged(int arg1)");

    if (!mInitialized)
        return;

    mImageCacheSize = arg1;
}

void TPreferencesDialog::on_pushButtonReset_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_pushButtonReset_clicked()");

    init(INIT_APPLICATION);
}

void TPreferencesDialog::on_comboBoxInitialZoom_currentIndexChanged(int index)
{
    DECL_TRACER("TPreferencesDialog::on_comboBoxInitialZoom_currentIndexChanged(int index)");

    if (!mInitialized)
        return;

    switch(index)
    {
        case 0: mInitialZoom = 25; break;
        case 1: mInitialZoom = 50; break;
        case 2: mInitialZoom = 75; break;
        case 3: mInitialZoom = 100; break;
        case 4: mInitialZoom = 150; break;
        case 5: mInitialZoom = 200; break;
        case 6: mInitialZoom = 400; break;
        case 7: mInitialZoom = -1; break;
        case 8: mInitialZoom = -2; break;
        case 9: mInitialZoom = -3; break;
        case 10: mInitialZoom = -4; break;
    }
}

void TPreferencesDialog::on_lineEditVisibleSize_textChanged(const QString &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_lineEditVisibleSize_textChanged(const QString &arg1)");

    // TODO: Make sure there is a real number
}

void TPreferencesDialog::on_checkBoxAutoStack_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxAutoStack_checkStateChanged(const Qt::CheckState &arg1)");

    if (!mInitialized)
        return;

    mStackWindows = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_comboBoxGridStyle_currentIndexChanged(int index)
{
    DECL_TRACER("TPreferencesDialog::on_comboBoxGridStyle_currentIndexChanged(int index)");

    if (!mInitialized)
        return;

    mGridStyle = index;
}

void TPreferencesDialog::on_spinBoxGridSize_valueChanged(int arg1)
{
    DECL_TRACER("TPreferencesDialog::on_spinBoxGridSize_valueChanged(int arg1)");

    if (!mInitialized)
        return;

    mGridSize = arg1;
}

void TPreferencesDialog::on_spinBoxSnapTolerance_valueChanged(int arg1)
{
    DECL_TRACER("TPreferencesDialog::on_spinBoxSnapTolerance_valueChanged(int arg1)");

    if (!mInitialized)
        return;

    mSnapTolerance = arg1;
}

void TPreferencesDialog::on_comboBoxStyle_currentIndexChanged(int index)
{
    DECL_TRACER("TPreferencesDialog::on_comboBoxStyle_currentIndexChanged(int index)");

    if (!mInitialized)
        return;

    mTransparencyStyle = index;
    setCheckerboardColor();
}

void TPreferencesDialog::on_comboBoxSize_currentIndexChanged(int index)
{
    DECL_TRACER("TPreferencesDialog::on_comboBoxSize_currentIndexChanged(int index)");

    if (!mInitialized)
        return;

    mTransparencySize = index;

    int square = 0;

    switch(mTransparencySize)
    {
        case 0: square = 5; break;      // Tiny
        case 1: square = 10; break;     // Small (default)
        case 2: square = 15; break;     // Medium
        case 3: square = 20; break;     // Large

        default:
            square = 10;
    }

    ui->labelCustomBackground->setSquareSize(square);
}

void TPreferencesDialog::on_pushButtonAppearanceReset_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_pushButtonAppearanceReset_clicked()");

    init(INIT_APPEARANCE);
}

void TPreferencesDialog::on_lineEditFolderPanels_textChanged(const QString &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_lineEditFolderPanels_textChanged(const QString &arg1)");

    if (!mInitialized)
        return;

    mFilesPanels = arg1;
}

void TPreferencesDialog::on_toolButtonFolderPanels_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_toolButtonFolderPanels_clicked()");

    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Panels Directory"), mFilesPanels);

    if (dir.isEmpty())
        return;

    mFilesPanels = dir;
    ui->lineEditFolderPanels->setText(mFilesPanels);
}

void TPreferencesDialog::on_lineEditFolderBackup_textChanged(const QString &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_lineEditFolderBackup_textChanged(const QString &arg1)");

    if (!mInitialized)
        return;

    mFilesBackups = arg1;
}

void TPreferencesDialog::on_toolButtonFolderBackup_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_toolButtonFolderBackup_clicked()");

    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Panels Directory"), mFilesBackups);

    if (dir.isEmpty())
        return;

    mFilesBackups = dir;
    ui->lineEditFolderBackup->setText(mFilesBackups);
}

void TPreferencesDialog::on_lineEditFolderTemp_textChanged(const QString &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_lineEditFolderTemp_textChanged(const QString &arg1)");

    if (!mInitialized)
        return;

    mFilesTemp = arg1;
}

void TPreferencesDialog::on_toolButtonFolderTemp_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_toolButtonFolderTemp_clicked()");

    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Panels Directory"), mFilesTemp);

    if (dir.isEmpty())
        return;

    mFilesTemp = dir;
    ui->lineEditFolderTemp->setText(mFilesTemp);
}

void TPreferencesDialog::on_pushButtonDirectoriesReset_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_pushButtonDirectoriesReset_clicked()")

    init(INIT_DIRECTORIES);
}

void TPreferencesDialog::on_comboBoxEditors_currentIndexChanged(int index)
{
    DECL_TRACER("TPreferencesDialog::on_comboBox_currentIndexChanged(int index)");

    if (index == 0)
        fillImageEditorTable();
    else
        fillSoundEditorTable();
}

void TPreferencesDialog::on_toolButtonAdd_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_toolButtonAdd_clicked()");

    QString binPath;
#ifdef __linux
    binPath = "/usr/bin";
#else
    binPath = "/";
#endif
    int index = ui->comboBoxEditors->currentIndex();
    QString title = index == 0 ? tr("Select an image editor") : tr("Select a sound editor");
    QString file = QFileDialog::getOpenFileName(this, title, binPath);

    if (file.isEmpty())
        return;

    if (!index)
    {
        mEditorsImage.append(file);
        fillImageEditorTable();
    }
    else
    {
        mEditorsSound.append(file);
        fillSoundEditorTable();
    }
}

void TPreferencesDialog::on_toolButtonDelete_clicked()
{

}

void TPreferencesDialog::on_tableViewEditorList_clicked(const QModelIndex &index)
{}

void TPreferencesDialog::on_pushButtonEditorReset_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_pushButtonEditorReset_clicked()");

    init(INIT_EDITORS);
}

void TPreferencesDialog::on_checkBoxEnableUndo_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxEnableUndo_checkStateChanged(const Qt::CheckState &arg1)");

    if (!mInitialized)
        return;

    mEnableUndoSystem = arg1 == Qt::Checked ? true : false;

    if (!mEnableUndoSystem)
    {
        ui->spinBoxUndoLevels->setDisabled(true);
        ui->checkBoxShowAffectedPages->setDisabled(true);
        ui->checkBoxChangeSelection->setDisabled(true);
        ui->checkBoxEnableRedo->setDisabled(true);
    }
    else
    {
        ui->spinBoxUndoLevels->setEnabled(true);
        ui->checkBoxShowAffectedPages->setEnabled(true);

        if (mUndoShowAffectedPages)
            ui->checkBoxChangeSelection->setEnabled(true);
        else
            ui->checkBoxChangeSelection->setDisabled(true);

        ui->checkBoxEnableRedo->setEnabled(true);
    }
}

void TPreferencesDialog::on_spinBoxUndoLevels_valueChanged(int arg1)
{
    DECL_TRACER("TPreferencesDialog::on_spinBoxUndoLevels_valueChanged(int arg1)");

    if (!mInitialized)
        return;

    mUndoLevels = arg1;
}

void TPreferencesDialog::on_checkBoxShowAffectedPages_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxShowAffectedPages_checkStateChanged(const Qt::CheckState &arg1)");

    if (!mInitialized)
        return;

    mUndoShowAffectedPages = arg1 == Qt::Checked ? true : false;

    if (mUndoShowAffectedPages)
        ui->checkBoxChangeSelection->setEnabled(true);
    else
        ui->checkBoxChangeSelection->setDisabled(true);
}

void TPreferencesDialog::on_checkBoxChangeSelection_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxChangeSelection_checkStateChanged(const Qt::CheckState &arg1)");

    if (!mInitialized)
        return;

    mUndoChangeSelection = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxEnableRedo_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxEnableRedo_checkStateChanged(const Qt::CheckState &arg1)");

    if (!mInitialized)
        return;

    mRedoEnableSystem = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_pushButtonUndoReset_clicked()
{
    DECL_TRACER("void TPreferencesDialog::on_pushButtonUndoReset_clicked()");

    init(INIT_UNDO);
}

void TPreferencesDialog::on_pushButtonTakeOver_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_pushButtonTakeOver_clicked()");

    save();
}

void TPreferencesDialog::on_pushButtonHelp_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_pushButtonHelp_clicked()");

    QMessageBox::information(this, tr("Help"), tr("Currently there is no help available!"));
}

void TPreferencesDialog::on_pushButtonResetAll_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_pushButtonResetAll_clicked()");

    init();
}

void TPreferencesDialog::onColorChanged(const QColor& color, const QString& name)
{
    DECL_TRACER("TPreferencesDialog::onColorChanged(const QColor& color, const QString& name)");

    if (name == "GutterColor")
        mGutterColor = color;
    else if (name == "GridColor")
        mGridColor = color;
    else if (name == "TransparencyColor1")
    {
        mTransparencyCustomColor1 = color;
        setCheckerboardColor();
    }
    else if (name == "TransparencyColor2")
    {
        mTransparencyCustomColor2 = color;
        setCheckerboardColor();
    }
}

void TPreferencesDialog::save()
{
    DECL_TRACER("TPreferencesDialog::save()");

    TConfig::Current().setSystemGeneratedName(mSystemGeneratedName);
    TConfig::Current().setReloadLastWorkspace(mReloadLastWorkspace);
    TConfig::Current().setExpandWorkspaceNavigator(mExpandWorkspaceNavigator);
    TConfig::Current().setWarnOnDeletingSources(mWarnOnDeletingSources);
    TConfig::Current().setFlushClosedPages(mFlushClosedPages);
    TConfig::Current().setCreateBackup(mCreateBackup);
    TConfig::Current().setUsePostfix(mUsePostfix);
    TConfig::Current().setRetainSelectedTool(mRetainSelectedTool);
    TConfig::Current().setImageCacheSize(mImageCacheSize);

    TConfig::Current().setInitialZoom(mInitialZoom);
    TConfig::Current().setVisibleSize(mVisibleSize);
    TConfig::Current().setGutterColor(mGutterColor);
    TConfig::Current().setStackWindows(mStackWindows);
    TConfig::Current().setGridStyle(mGridStyle);
    TConfig::Current().setGridSize(mGridSize);
    TConfig::Current().setSnapTolerance(mSnapTolerance);
    TConfig::Current().setGridColor(mGridColor);
    TConfig::Current().setTransparencyStyle(mTransparencyStyle);
    TConfig::Current().setTransparencySize(mTransparencySize);
    TConfig::Current().setTransparencyCustomColor1(mTransparencyCustomColor1);
    TConfig::Current().setTransparencyCustomColor2(mTransparencyCustomColor2);

    TConfig::Current().setFilesPanels(mFilesPanels);
    TConfig::Current().setFilesBackups(mFilesBackups);
    TConfig::Current().setFilesTemp(mFilesTemp);

    TConfig::Current().setEditorsImage(mEditorsImage);
    TConfig::Current().setEditorsSound(mEditorsSound);

    TConfig::Current().setEnableUndoSystem(mEnableUndoSystem);
    TConfig::Current().setUndoLevels(mUndoLevels);
    TConfig::Current().setUndoShowAffectedPages(mUndoShowAffectedPages);
    TConfig::Current().setUndoChangeSelection(mUndoChangeSelection);
    TConfig::Current().setRedoEnableSystem(mUndoChangeSelection);
}

void TPreferencesDialog::fillImageEditorTable()
{
    DECL_TRACER("TPreferencesDialog::fillImageEditorTable()");

    bool haveModel = false;

    if (!mModel)
        mModel = new QStandardItemModel(this);
    else
    {
        mModel->clear();
        haveModel = true;
    }

    mModel->setColumnCount(3);
    QStringList headers = { tr("Name"), tr("Path"), tr("Default") };
    mModel->setHorizontalHeaderLabels(headers);
    int row = 0;

    for (QString str : mEditorsImage)
    {
        QStandardItem *cell1 = new QStandardItem;
        QStandardItem *cell2 = new QStandardItem;
        QStandardItem *cell3 = new QStandardItem;

        cell1->setEditable(false);
        cell2->setEditable(false);
        cell3->setEditable(false);

        cell1->setText(basename(str));
        cell2->setText(pathname(str));
        cell3->setText(tr("No"));

        mModel->setItem(row, 0, cell1);
        mModel->setItem(row, 1, cell2);
        mModel->setItem(row, 2, cell3);
    }

    if (!haveModel)
    {
        ui->tableViewEditorList->setModel(mModel);
        ui->tableViewEditorList->resizeColumnsToContents();
        ui->tableViewEditorList->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableViewEditorList->setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

void TPreferencesDialog::fillSoundEditorTable()
{
    bool haveModel = false;

    if (!mModel)
        mModel = new QStandardItemModel(this);
    else
    {
        mModel->clear();
        haveModel = true;
    }

    mModel->setColumnCount(3);
    QStringList headers = { tr("Name"), tr("Path"), tr("Default") };
    mModel->setHorizontalHeaderLabels(headers);
    int row = 0;

    for (QString str : mEditorsSound)
    {
        QStandardItem *cell1 = new QStandardItem;
        QStandardItem *cell2 = new QStandardItem;
        QStandardItem *cell3 = new QStandardItem;

        cell1->setEditable(false);
        cell2->setEditable(false);
        cell3->setEditable(false);

        cell1->setText(basename(str));
        cell2->setText(pathname(str));
        cell3->setText(tr("No"));

        mModel->setItem(row, 0, cell1);
        mModel->setItem(row, 1, cell2);
        mModel->setItem(row, 2, cell3);
    }

    if (!haveModel)
    {
        ui->tableViewEditorList->setModel(mModel);
        ui->tableViewEditorList->resizeColumnsToContents();
        ui->tableViewEditorList->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableViewEditorList->setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

void TPreferencesDialog::accept()
{
    DECL_TRACER("TPreferencesDialog::accept()");

    save();
    done(QDialog::Accepted);
}
