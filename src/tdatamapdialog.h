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
#ifndef TDATAMAPDIALOG_H
#define TDATAMAPDIALOG_H

#include <QDialog>
#include "tconfmain.h"

namespace Ui {
class TDataMapDialog;
}

class TDownloader;

class TDataMapDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TDataMapDialog(QWidget *parent = nullptr);
        ~TDataMapDialog();

        typedef enum
        {
            SORT_NONE,
            SORT_ASC,
            SORT_DESC,
            SORT_QUERY
        }SORT_t;

        typedef enum
        {
            FORMAT_XPORT,
            FORMAT_CSVHEAD,
            FORMAT_CSV
        }FORMAT_t;

        bool haveLive() { return mHaveLive; }
        void setData(const ConfigMain::DATASOURCE_t& data);

        QString getPrimaryText() { return mPrimaryText; }
        QString getSecondaryText() { return mSecondaryText; }
        QString getImageName() { return mImageName; }
        SORT_t getSort() { return mSort; }
        QStringList getColumns() { return mColumns; }
        QString getQuery() { return mQuery; }
        QString getDelimiter() { return mDelimiter; }
        bool getQuoted() { return mQuoted; }
        int getHeadlines() { return mHeadlines; }

        void setPrimaryText(const QString& txt);
        void setSecondaryText(const QString& txt);
        void setImageName(const QString& image);
        void setSort(SORT_t sort);
        void setColumns(const QStringList& cols);
        void setQuery(const QString& query);
        void setDelimiter(const QString& deli);
        void setQuoted(bool quote);
        void setHeadlines(int lines);

    protected:
        void onReady();
        void onError(const QString& msg);

        void accept() override;

    private slots:
        void on_pushButtonGetFile_clicked();
        void on_lineEditPrimaryText_textChanged(const QString &arg1);
        void on_lineEditSecondaryText_textChanged(const QString &arg1);
        void on_lineEditImage_textChanged(const QString &arg1);
        void on_comboBoxSelectT1_currentIndexChanged(int index);
        void on_comboBoxSelectT2_currentIndexChanged(int index);
        void on_comboBoxSelectImage_currentIndexChanged(int index);
        void on_radioButtonSortNone_clicked(bool checked);
        void on_radioButtonSortAsc_clicked(bool checked);
        void on_radioButtonSortDesc_clicked(bool checked);
        void on_radioButtonSortQuery_clicked(bool checked);
        void on_listViewColums_activated(const QModelIndex &index);
        void on_pushButtonColumnAdd_clicked();
        void on_pushButtonColumnRemove_clicked();
        void on_pushButtonColumnUp_clicked();
        void on_pushButtonColumnDown_clicked();
        void on_comboBoxColumnSelect_currentIndexChanged(int index);
        void on_lineEditQuery_textChanged(const QString &arg1);
        void on_comboBoxDelimiter_currentIndexChanged(int index);
        void on_comboBoxDelimiter_editTextChanged(const QString &arg1);
        void on_checkBoxCSVQoted_clicked(bool checked);
        void on_spinBoxCSVHeadLines_valueChanged(int arg1);

    private:
        Ui::TDataMapDialog *ui;

        QString mPrimaryText;
        QString mSecondaryText;
        QString mImageName;
        SORT_t mSort{SORT_NONE};
        QStringList mColumns;
        QString mQuery;
        QString mDelimiter;
        bool mQuoted{false};
        int mHeadlines{0};
        bool mHaveLive{false};
        TDownloader *mLoader{nullptr};
        ConfigMain::DATASOURCE_t mData;
        bool mHaveData{false};          // TRUE = Data were set by a call to setData()
        bool mInitialized{false};
};

#endif // TDATAMAPDIALOG_H
