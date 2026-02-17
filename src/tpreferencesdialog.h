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
#ifndef TPREFERENCESDIALOG_H
#define TPREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {

class TPreferencesDialog;

}

class TElementColorSelector;
class QStandardItemModel;

class TPreferencesDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TPreferencesDialog(QWidget *parent = nullptr);
        ~TPreferencesDialog();

    private slots:
        void on_checkBoxSystemGeneratedName_checkStateChanged(const Qt::CheckState &arg1);
        void on_checkBoxReloadLastWorkspace_checkStateChanged(const Qt::CheckState &arg1);
        void on_checkBoxExpandWorkspaceNavigator_checkStateChanged(const Qt::CheckState &arg1);
        void on_checkBoxDeleteResource_checkStateChanged(const Qt::CheckState &arg1);
        void on_checkBoxFlushClosedPages_checkStateChanged(const Qt::CheckState &arg1);
        void on_checkBoxCreateBackup_checkStateChanged(const Qt::CheckState &arg1);
        void on_checkBoxPostfix_checkStateChanged(const Qt::CheckState &arg1);
        void on_checkBoxRetainSelectedTool_checkStateChanged(const Qt::CheckState &arg1);
        void on_spinBoxChacheSize_valueChanged(int arg1);
        void on_pushButtonReset_clicked();

        void on_comboBoxInitialZoom_currentIndexChanged(int index);
        void on_lineEditVisibleSize_textChanged(const QString &arg1);
        void on_checkBoxAutoStack_checkStateChanged(const Qt::CheckState &arg1);
        void on_comboBoxGridStyle_currentIndexChanged(int index);
        void on_spinBoxGridSize_valueChanged(int arg1);
        void on_spinBoxSnapTolerance_valueChanged(int arg1);
        void on_comboBoxStyle_currentIndexChanged(int index);
        void on_comboBoxSize_currentIndexChanged(int index);

        void on_pushButtonAppearanceReset_clicked();
        void on_lineEditFolderPanels_textChanged(const QString &arg1);
        void on_toolButtonFolderPanels_clicked();
        void on_lineEditFolderBackup_textChanged(const QString &arg1);
        void on_toolButtonFolderBackup_clicked();
        void on_lineEditFolderTemp_textChanged(const QString &arg1);
        void on_toolButtonFolderTemp_clicked();
        void on_pushButtonDirectoriesReset_clicked();

        void on_comboBoxEditors_currentIndexChanged(int index);
        void on_toolButtonAdd_clicked();
        void on_toolButtonDelete_clicked();
        void on_tableViewEditorList_clicked(const QModelIndex &index);
        void on_pushButtonEditorReset_clicked();

        void on_checkBoxEnableUndo_checkStateChanged(const Qt::CheckState &arg1);
        void on_spinBoxUndoLevels_valueChanged(int arg1);
        void on_checkBoxShowAffectedPages_checkStateChanged(const Qt::CheckState &arg1);
        void on_checkBoxChangeSelection_checkStateChanged(const Qt::CheckState &arg1);
        void on_checkBoxEnableRedo_checkStateChanged(const Qt::CheckState &arg1);
        void on_pushButtonUndoReset_clicked();

        void on_pushButtonTakeOver_clicked();
        void on_pushButtonHelp_clicked();
        void on_pushButtonResetAll_clicked();

    protected:
        void accept();
        void onColorChanged(const QColor& color, const QString& name);

    private:
        typedef enum
        {
            INIT_ALL,
            INIT_APPLICATION,
            INIT_APPEARANCE,
            INIT_DIRECTORIES,
            INIT_EDITORS,
            INIT_UNDO
        }INIT_t;

        void init(INIT_t i=INIT_ALL);
        void save();
        void setCheckerboardColor();
        void fillImageEditorTable();
        void fillSoundEditorTable();

        Ui::TPreferencesDialog *ui;

        bool mInitialized{false};
        // Preferences: Application
        bool mSystemGeneratedName{false};
        bool mReloadLastWorkspace{false};
        bool mExpandWorkspaceNavigator{true};
        bool mWarnOnDeletingSources{true};
        bool mFlushClosedPages{true};
        bool mCreateBackup{true};
        bool mUsePostfix{true};
        bool mRetainSelectedTool{true};
        qsizetype mImageCacheSize{8};       // Mib
        // Preferences: Appearance
        int mInitialZoom{100};              // Percent
        qreal mVisibleSize{0.0};            // Inches
        QColor mGutterColor{qRgb(0, 0, 0)};
        bool mStackWindows{true};
        int mGridStyle{1};
        int mGridSize{8};
        int mSnapTolerance{8};
        QColor mGridColor{Qt::gray};
        int mTransparencyStyle{0};
        int mTransparencySize{1};
        QColor mTransparencyCustomColor1{Qt::white};
        QColor mTransparencyCustomColor2{Qt::gray};
        TElementColorSelector *mColGutter{nullptr};
        TElementColorSelector *mColGrid{nullptr};
        TElementColorSelector *mColTrans1{nullptr};
        TElementColorSelector *mColTrans2{nullptr};
        // Preferences: Directories
        QString mFilesPanels;
        QString mFilesBackups;
        QString mFilesTemp;
        // Preferences: Editors
        QStringList mEditorsImage;
        QStringList mEditorsSound;
        QStandardItemModel *mModel{nullptr};
        // Preferences: Undo/Redo
        bool mEnableUndoSystem{false};
        int mUndoLevels{500};
        bool mUndoShowAffectedPages{false};
        bool mUndoChangeSelection{false};
        bool mRedoEnableSystem{false};
};

#endif // TPREFERENCESDIALOG_H
