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
#ifndef TRESOURCEDIALOG_H
#define TRESOURCEDIALOG_H

#include <QDialog>
#include <QClipboard>

#include "tconfmain.h"

namespace Ui {
class TResourceDialog;
}

class QModelIndex;
class QItemSelection;
class QFileDialog;
class QStandardItem;
class QStandardItemModel;
class QItemSelectionModel;

class TResourceDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TResourceDialog(QWidget *parent = nullptr);
        ~TResourceDialog();

        typedef enum
        {
            SHOW_LIST,
            SHOW_DETAIL,
            SHOW_ICON
        }SHOW_t;

        typedef enum
        {
            LABEL_LISTVIEW,
            LABEL_FILESCOPIED,
            LABEL_RESOURCES,
            LABEL_SOUND
        }LABEL_t;

        typedef enum
        {
            SEL_IMAGES,
            SEL_DYNIMAGES,
            SEL_SOUNDS,
            SEL_DATASOURCE
        }TABSEL_t;

        void setPathTemporary(const QString& path) { mPathTemporary = path; }
        void setLastOpenPath(const QString& path) { mLastOpenPath = path; }
        QString& getLastOpenPath() { return mLastOpenPath; }
        bool haveChanged() { return mChanged; }
        QStringList& getImageList() { return mImages; }

    protected:
        QPixmap sizeImage(const QSize& size, const QString& file);
        void importImagesToListView(const QStringList& files);
        void removeItemFromListView(const QString& file, int row);
        int getRowFromListView(const QString& file);
        QStandardItem *getItemFromListView(const QString& file);
        void renameImageFile(const QString& ori, const QString& tgt);
        void setLabel(LABEL_t lb, int number, const QString& text);
        void disableClipboardButtons();
        void enableClipboardButtons();
        void copyImagesToClipboard();
        bool haveSelected(QItemSelectionModel *model);

        void addDynamicResource(const ConfigMain::RESOURCE_t& res, QStandardItemModel *model=nullptr);
        void editDynamicResource(const QString& name, bool neu=false);
        QString getUrl(const ConfigMain::RESOURCE_t& res);
        void copyDynamicImageToClipboard();

        void onImageImportFinished(int result);
        void onClipboardChanged(QClipboard::Mode mode);
        void onClipboardDataChanged();
        bool parseDynamicImageFromClipboard(ConfigMain::RESOURCE_t *res, const QString& txt);
        int getClipboardImageNumber();

        void addSoundFile(const QString& file, QStandardItemModel *model=nullptr);
        void importSoundFile();
        void removeSoundFile();
        void copySoundFileToClipboard();

        void addDynamicData(const ConfigMain::DATASOURCE_t& data, QStandardItemModel *model=nullptr);
        QString getUrl(const ConfigMain::DATASOURCE_t& data);
        void editDynamicData(ConfigMain::DATASOURCE_t& data);
        QString getFormat(int format);
        int getFormatFromString(const QString& format);
        void removeDynamicData();
        void copyDynamicDataToClipboard();
        ConfigMain::DATASOURCE_t parseDynamicData(const QString& txt);

    private slots:
        void on_pushButtonCut_clicked();
        void on_pushButtonCopy_clicked();
        void on_pushButtonPaste_clicked();
        void on_pushButtonDelete_clicked();
        void on_pushButtonRename_clicked();
        void on_pushButtonUndo_clicked();
        void on_pushButtonRedo_clicked();
        void on_pushButtonImport_clicked();
        void on_pushButtonExport_clicked();
        void on_pushButtonDataMap_clicked();
        void on_comboBoxStyle_currentIndexChanged(int index);
        void on_tabWidgetAction_tabBarClicked(int index);
        void on_listViewImages_activated(const QModelIndex &index);
        void on_listViewImages_entered(const QModelIndex &index);
        void on_tableViewDynamicImages_activated(const QModelIndex &index);
        void on_tableViewDynamicImages_doubleClicked(const QModelIndex &index);
        void on_tableViewSounds_activated(const QModelIndex &index);
        void on_tableViewDynamicDataSources_activated(const QModelIndex &index);
        void on_tableViewDynamicDataSources_doubleClicked(const QModelIndex &index);

    private:
        Ui::TResourceDialog *ui;

        TABSEL_t mTabSelected{SEL_IMAGES};          // Holds the currently selected tab
        QString mPathTemporary;                     // The path to the temporary files
        SHOW_t mShowType{SHOW_ICON};                // Holds how the list should be shown
        QString mMapsFile;                          // Path to system mapping file
        QString mLastOpenPath;                      // The lat path used to load or save a file
        QClipboard *mClipboard{nullptr};            // Pointer to the clipboard
        QClipboard::Mode mClipboardMode{QClipboard::Clipboard};
        QPixmap mClipboardPixmap;                   // Holds the pixmap from the clipboard, if there was one.
        QString mClipboardText;                     // Holds the text from the clipboard, if there was one.
        int mClipboardPixmapNumber{0};              // A number appended to the name of a pixmap received from the clipboard
        // Images
        QStringList mImages;                        // A list of images
        bool mChanged{false};                       // TRUE = Some data have changed e.g. the document has changed.
        QFileDialog *mImportImagesDialog{nullptr};  // The file dialog to import images. This is for an asynchronous dialog!
        // Dynamic resources
        QList<ConfigMain::RESOURCE_t> mDynamicResources;    // List of dynamic resources
        int mDynamicRowSelected{-1};                // The row selected in the dynamic image list.
        // Sound files
        QStringList mSoundFiles;
        // Dynamic data
        QList<ConfigMain::DATASOURCE_t> mDynamicData;
};

#endif // TRESOURCEDIALOG_H
