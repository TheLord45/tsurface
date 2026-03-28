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
#ifndef TBITMAPSELECTDIALOG_H
#define TBITMAPSELECTDIALOG_H

#include <QDialog>
#include "tconfmain.h"
#include "tobjecthandler.h"

namespace Ui {
    class TBitmapSelectDialog;
}

class QStandardItemModel;
class QFileDialog;

class TBitmapSelectDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TBitmapSelectDialog(QWidget *parent = nullptr);
        ~TBitmapSelectDialog();

        void setTemporaryPath(const QString& path);
        void setDefaultBitmap(const QString& file);
        QList<ObjHandler::BITMAPS_t> getSelected() { return mSelected; }
        void setSingleSelect(bool sel);

    protected:
        virtual void accept() override;
        void onImageImportFinished(int result);

    private slots:
        void on_pushButtonImport_clicked();
        void on_listViewImages_clicked(const QModelIndex &index);
        void on_listViewDynamic_clicked(const QModelIndex &index);
        void on_tabWidget_tabBarClicked(int index);

    private:
        typedef enum
        {
            SEL_IMAGES,
            SEL_DYNIMAGES
        }TABSEL_t;

        void init();
        void addDynamicResource(const ConfigMain::RESOURCE_t& res, QStandardItemModel *model=nullptr);
        void importImagesToListView(const QStringList& images);

        Ui::TBitmapSelectDialog *ui;

        QFileDialog *mImportImagesDialog{nullptr};
        QList<QString> mImages;
        QString mPathTemporary;
        QList<ConfigMain::RESOURCE_t> mDynamicResources;
        QList<ObjHandler::BITMAPS_t> mSelected;
        int mCurrentTab{0};
        bool mSingleSelect{false};
};

#endif // TBITMAPSELECTDIALOG_H
