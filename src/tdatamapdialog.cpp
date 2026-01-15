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
#include <QListView>
#include <QStandardItemModel>

#include "tdatamapdialog.h"
#include "ui_tdatamapdialog.h"
#include "terror.h"

TDataMapDialog::TDataMapDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TDataMapDialog)
{
    DECL_TRACER("TDataMapDialog::TDataMapDialog(QWidget *parent)");

    ui->setupUi(this);

    ui->groupBoxSortQuery->setVisible(false);
    QStandardItemModel *model = new QStandardItemModel(this);
    ui->listViewColums->setModel(model);
}

TDataMapDialog::~TDataMapDialog()
{
    DECL_TRACER("TDataMapDialog::~TDataMapDialog()");

    delete ui;
}

void TDataMapDialog::setPrimaryText(const QString& txt)
{
    DECL_TRACER("TDataMapDialog::setPrimaryText(const QString& txt)");

    mPrimaryText = txt;
    ui->lineEditPrimaryText->setText(txt);
}

void TDataMapDialog::setSecondaryText(const QString& txt)
{
    DECL_TRACER("TDataMapDialog::setSecondaryText(const QString& txt)");

    mSecondaryText = txt;
    ui->lineEditSecondaryText->setText(txt);
}

void TDataMapDialog::setImageName(const QString& image)
{
    DECL_TRACER("TDataMapDialog::setImageName(const QString& image)");

    mImageName = image;
    ui->lineEditImage->setText(image);
}

void TDataMapDialog::setSort(SORT_t sort)
{
    DECL_TRACER("TDataMapDialog::setSort(SORT_t sort)");

    switch(sort)
    {
        case SORT_NONE: ui->radioButtonSortNone->setChecked(true); break;
        case SORT_ASC:  ui->radioButtonSortAsc->setChecked(true); break;
        case SORT_DESC: ui->radioButtonSortDesc->setChecked(true); break;

        case
            SORT_QUERY: ui->radioButtonSortQuery->setChecked(true);
            ui->groupBoxSortQuery->setVisible(true);
        break;
    }

        if (sort != SORT_QUERY)
            ui->groupBoxSortQuery->setVisible(false);
}

void TDataMapDialog::setColumns(const QStringList& cols)
{
    DECL_TRACER("TDataMapDialog::setColumns(const QStringList& cols)");

    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->listViewColums->model());
    bool haveModel = true;

    if (!model)
    {
        model = new QStandardItemModel(this);
        haveModel = false;
    }

    QStringList::Iterator iter;
    int row = 0;

    for (iter = mColumns.begin(); iter != mColumns.end(); ++iter)
    {
        QStandardItem *item = new QStandardItem(*iter);
        item->setData(*iter, Qt::DisplayRole);
        model->appendRow(item);
        row++;
    }

    if (!haveModel)
        ui->listViewColums->setModel(model);
}

void TDataMapDialog::setQuery(const QString& query)
{
    DECL_TRACER("TDataMapDialog::setQuery(const QString& query)");

    mQuery = query;
    ui->lineEditQuery->setText(query);
}

void TDataMapDialog::on_pushButtonGetFile_clicked()
{

}


void TDataMapDialog::on_lineEditPrimaryText_textChanged(const QString &arg1)
{
    mPrimaryText = arg1;
}

void TDataMapDialog::on_lineEditSecondaryText_textChanged(const QString &arg1)
{
    mSecondaryText = arg1;
}

void TDataMapDialog::on_lineEditImage_textChanged(const QString &arg1)
{
    mImageName = arg1;
}

void TDataMapDialog::on_comboBoxSelectT1_currentIndexChanged(int index)
{

}

void TDataMapDialog::on_comboBoxSelectT2_currentIndexChanged(int index)
{

}

void TDataMapDialog::on_comboBoxSelectImage_currentIndexChanged(int index)
{

}

void TDataMapDialog::on_radioButtonSortNone_clicked(bool checked)
{
    DECL_TRACER("TDataMapDialog::on_radioButtonSortNone_clicked(bool checked)");

    if (checked)
    {
        mSort = SORT_NONE;
        ui->groupBoxSortQuery->setVisible(false);
    }
}

void TDataMapDialog::on_radioButtonSortAsc_clicked(bool checked)
{
    DECL_TRACER("TDataMapDialog::on_radioButtonSortAsc_clicked(bool checked)");

    if (checked)
    {
        mSort = SORT_ASC;
        ui->groupBoxSortQuery->setVisible(false);
    }
}

void TDataMapDialog::on_radioButtonSortDesc_clicked(bool checked)
{
    DECL_TRACER("TDataMapDialog::on_radioButtonSortDesc_clicked(bool checked)");

    if (checked)
    {
        mSort = SORT_DESC;
        ui->groupBoxSortQuery->setVisible(false);
    }
}

void TDataMapDialog::on_radioButtonSortQuery_clicked(bool checked)
{
    DECL_TRACER("TDataMapDialog::on_radioButtonSortQuery_clicked(bool checked)");

    if (checked)
    {
        mSort = SORT_QUERY;
        ui->groupBoxSortQuery->setVisible(true);
    }
}

void TDataMapDialog::on_listViewColums_activated(const QModelIndex &index)
{
    DECL_TRACER("TDataMapDialog::on_listViewColums_activated(const QModelIndex &index)");
}


void TDataMapDialog::on_pushButtonColumnAdd_clicked()
{

}


void TDataMapDialog::on_pushButtonColumnRemove_clicked()
{

}


void TDataMapDialog::on_pushButtonColumnUp_clicked()
{

}


void TDataMapDialog::on_pushButtonColumnDown_clicked()
{

}


void TDataMapDialog::on_comboBoxColumnSelect_currentIndexChanged(int index)
{

}


void TDataMapDialog::on_lineEditQuery_textChanged(const QString &arg1)
{
    mQuery = arg1;
}

