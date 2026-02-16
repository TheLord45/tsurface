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
#include "tpreferencesdialog.h"
#include "ui_tpreferencesdialog.h"
#include "tconfig.h"
#include "terror.h"

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

void TPreferencesDialog::init()
{
    DECL_TRACER("TPreferencesDialog::init()");

    ui->checkBoxSystemGeneratedName->setCheckState(TConfig::Current().getSystemGeneratedName() ? Qt::Checked : Qt::Unchecked);
    ui->checkBoxReloadLastWorkspace->setCheckState(TConfig::Current().getReloadLastWorkspace() ? Qt::Checked : Qt::Unchecked);
    ui->checkBoxExpandWorkspaceNavigator->setCheckState(TConfig::Current().getExpandWorkspaceNavigator() ? Qt::Checked : Qt::Unchecked);
    ui->checkBoxDeleteResource->setCheckState(TConfig::Current().getWarnOnDeletingSources() ? Qt::Checked : Qt::Unchecked);
    ui->checkBoxFlushClosedPages->setCheckState(TConfig::Current().getFlushClosedPages() ? Qt::Checked : Qt::Unchecked);
    ui->checkBoxCreateBackup->setCheckState(TConfig::Current().getCreateBackup() ? Qt::Checked : Qt::Unchecked);
    ui->checkBoxPostfix->setCheckState(TConfig::Current().getUsePostfix() ? Qt::Checked : Qt::Unchecked);
    ui->checkBoxRetainSelectedTool->setCheckState(TConfig::Current().getRetainSelectedTool() ? Qt::Checked : Qt::Unchecked);
    ui->spinBoxChacheSize->setValue(TConfig::Current().getImageCacheSize());

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
    // TODO: Add gutter color
    ui->comboBoxGridStyle->setCurrentIndex(TConfig::Current().getGridStyle());
    ui->spinBoxGridSize->setValue(TConfig::Current().getGridSize());
    ui->spinBoxSnapTolerance->setValue(TConfig::Current().getSnapTolerance());
    // TODO: Add grid color
    ui->comboBoxStyle->setCurrentIndex(TConfig::Current().getTransparencyStyle());
    ui->comboBoxSize->setCurrentIndex(TConfig::Current().getTransparencySize());
    // TODO: Add Custom transparency color
    ui->lineEditFolderPanels->setText(TConfig::Current().getFilesPanels());
    ui->lineEditFolderBackup->setText(TConfig::Current().getFilesBackups());

    ui->lineEditFolderTemp->setText(TConfig::Current().getFilesTemp());
    ui->lineEditFolderTemp->setDisabled(true);
    ui->toolButtonFolderTemp->setDisabled(true);

    // TODO: Add code to fill editor list
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

    mInitialized = true;
}

void TPreferencesDialog::on_checkBoxSystemGeneratedName_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxSystemGeneratedName_checkStateChanged(const Qt::CheckState &arg1)");

    mSystemGeneratedName = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxReloadLastWorkspace_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxReloadLastWorkspace_checkStateChanged(const Qt::CheckState &arg1)");

    mReloadLastWorkspace = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxExpandWorkspaceNavigator_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxExpandWorkspaceNavigator_checkStateChanged(const Qt::CheckState &arg1)");

    mExpandWorkspaceNavigator = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxDeleteResource_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxDeleteResource_checkStateChanged(const Qt::CheckState &arg1)");

    mWarnOnDeletingSources = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxFlushClosedPages_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxFlushClosedPages_checkStateChanged(const Qt::CheckState &arg1)");

    mFlushClosedPages = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxCreateBackup_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxCreateBackup_checkStateChanged(const Qt::CheckState &arg1)");

    mCreateBackup = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxPostfix_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxPostfix_checkStateChanged(const Qt::CheckState &arg1)");

    mUsePostfix = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxRetainSelectedTool_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxRetainSelectedTool_checkStateChanged(const Qt::CheckState &arg1)");

    mRetainSelectedTool = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_spinBoxChacheSize_valueChanged(int arg1)
{
    DECL_TRACER("TPreferencesDialog::on_spinBoxChacheSize_valueChanged(int arg1)");

    mImageCacheSize = arg1;
}

void TPreferencesDialog::on_pushButtonReset_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_pushButtonReset_clicked()");

    // TODO: Add code to reset dialog
}

void TPreferencesDialog::on_comboBoxInitialZoom_currentIndexChanged(int index)
{
    DECL_TRACER("TPreferencesDialog::on_comboBoxInitialZoom_currentIndexChanged(int index)");

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

    mStackWindows = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_comboBoxGridStyle_currentIndexChanged(int index)
{
    DECL_TRACER("TPreferencesDialog::on_comboBoxGridStyle_currentIndexChanged(int index)");

    mGridStyle = index;
}

void TPreferencesDialog::on_spinBoxGridSize_valueChanged(int arg1)
{
    DECL_TRACER("TPreferencesDialog::on_spinBoxGridSize_valueChanged(int arg1)");

    mGridSize = arg1;
}

void TPreferencesDialog::on_spinBoxSnapTolerance_valueChanged(int arg1)
{
    DECL_TRACER("TPreferencesDialog::on_spinBoxSnapTolerance_valueChanged(int arg1)");

    mSnapTolerance = arg1;
}

void TPreferencesDialog::on_comboBoxStyle_currentIndexChanged(int index)
{
    DECL_TRACER("TPreferencesDialog::on_comboBoxStyle_currentIndexChanged(int index)");

    mTransparencyStyle = index;
}

void TPreferencesDialog::on_comboBoxSize_currentIndexChanged(int index)
{
    DECL_TRACER("TPreferencesDialog::on_comboBoxSize_currentIndexChanged(int index)");

    mTransparencySize = index;
}

void TPreferencesDialog::on_pushButtonAppearanceReset_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_pushButtonAppearanceReset_clicked()");

}

void TPreferencesDialog::on_lineEditFolderPanels_textChanged(const QString &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_lineEditFolderPanels_textChanged(const QString &arg1)");

    mFilesPanels = arg1;
}

void TPreferencesDialog::on_toolButtonFolderPanels_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_toolButtonFolderPanels_clicked()");

    // TODO: Display file dialog
}

void TPreferencesDialog::on_lineEditFolderBackup_textChanged(const QString &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_lineEditFolderBackup_textChanged(const QString &arg1)");

    mFilesBackups = arg1;
}

void TPreferencesDialog::on_toolButtonFolderBackup_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_toolButtonFolderBackup_clicked()");

    // TODO: Display file dialog
}

void TPreferencesDialog::on_lineEditFolderTemp_textChanged(const QString &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_lineEditFolderTemp_textChanged(const QString &arg1)");

    mFilesTemp = arg1;
}

void TPreferencesDialog::on_toolButtonFolderTemp_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_toolButtonFolderTemp_clicked()");

    // TODO: Display file dialog
}

void TPreferencesDialog::on_pushButtonDirectoriesReset_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_pushButtonDirectoriesReset_clicked()")

    // TODO: Add reset function
}

void TPreferencesDialog::on_comboBoxEditors_currentIndexChanged(int index)
{
    DECL_TRACER("TPreferencesDialog::on_comboBox_currentIndexChanged(int index)");

    // TODO: Change view of table
}

void TPreferencesDialog::on_toolButtonAdd_clicked() {}

void TPreferencesDialog::on_toolButtonDelete_clicked() {}

void TPreferencesDialog::on_tableViewEditorList_clicked(const QModelIndex &index)
{}

void TPreferencesDialog::on_pushButtonEditorReset_clicked() {}

void TPreferencesDialog::on_checkBoxEnableUndo_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxEnableUndo_checkStateChanged(const Qt::CheckState &arg1)");

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

    mUndoLevels = arg1;
}

void TPreferencesDialog::on_checkBoxShowAffectedPages_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxShowAffectedPages_checkStateChanged(const Qt::CheckState &arg1)");

    mUndoShowAffectedPages = arg1 == Qt::Checked ? true : false;

    if (mUndoShowAffectedPages)
        ui->checkBoxChangeSelection->setEnabled(true);
    else
        ui->checkBoxChangeSelection->setDisabled(true);
}

void TPreferencesDialog::on_checkBoxChangeSelection_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxChangeSelection_checkStateChanged(const Qt::CheckState &arg1)");

    mUndoChangeSelection = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_checkBoxEnableRedo_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TPreferencesDialog::on_checkBoxEnableRedo_checkStateChanged(const Qt::CheckState &arg1)");

    mRedoEnableSystem = arg1 == Qt::Checked ? true : false;
}

void TPreferencesDialog::on_pushButtonUndoReset_clicked()
{

}

void TPreferencesDialog::on_pushButtonTakeOver_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_pushButtonTakeOver_clicked()");

    save();
}

void TPreferencesDialog::on_pushButtonHelp_clicked()
{

}

void TPreferencesDialog::on_pushButtonResetAll_clicked()
{
    DECL_TRACER("TPreferencesDialog::on_pushButtonResetAll_clicked()");

    init();
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

void TPreferencesDialog::accept()
{
    DECL_TRACER("TPreferencesDialog::accept()");

    save();
    done(QDialog::Accepted);
}
