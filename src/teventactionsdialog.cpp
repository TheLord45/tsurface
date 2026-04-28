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
#include "teventactionsdialog.h"
#include "ui_teventactionsdialog.h"
#include "tpagehandler.h"
#include "terror.h"

TEventActionsDialog::TEventActionsDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TEventActionsDialog)
{
    ui->setupUi(this);

    QStringList list1 = { "standard page", "previous page", "show popup", "hide popup", "toggle popup", "hide popup group", "hide popup on page", "hide all popups" };
    QList<int> disabled1 = { 0, 1, 5 };
    QStringList list2 = { "show", "close", "close all", "show status", "hide status" };
    QList<int> disabled2 = { 0, 1 };
    QStringList list3 = { "command", "string", "custom" };

    ui->comboButtonAddPageFlip->setItems(list1);

    for (int i = 0; i < disabled1.size(); ++i)
        ui->comboButtonAddPageFlip->disableItem(disabled1[i], true);

    ui->comboButtonAddLaunchOption->setItems(list2);

    for (int i = 0; i < disabled2.size(); ++i)
        ui->comboButtonAddPageFlip->disableItem(disabled2[i], true);

    ui->comboButtonAddAction->setItems(list3);


}

TEventActionsDialog::~TEventActionsDialog()
{
    delete ui;
}

void TEventActionsDialog::on_comboBoxAddPageFlip_currentIndexChanged(int index)
{
    DECL_TRACER("TEventActionsDialog::on_comboBoxAddPageFlip_currentIndexChanged(int index)");

}


void TEventActionsDialog::on_comboBoxAddLaunchAction_currentIndexChanged(int index)
{

}


void TEventActionsDialog::on_comboBoxAddAction_currentIndexChanged(int index)
{

}


void TEventActionsDialog::on_pushButtonDelete_clicked()
{

}


void TEventActionsDialog::on_pushButtonClearAll_clicked()
{

}


void TEventActionsDialog::on_pushButtonMoveUp_clicked()
{

}


void TEventActionsDialog::on_pushButtonMoveDown_clicked()
{

}


void TEventActionsDialog::on_listWidgetAnimations_itemClicked(QListWidgetItem *item)
{

}


void TEventActionsDialog::on_radioButtonLeft_clicked()
{

}


void TEventActionsDialog::on_radioButtonRight_clicked()
{

}


void TEventActionsDialog::on_radioButtonTop_clicked()
{

}


void TEventActionsDialog::on_radioButtonBottom_clicked()
{

}


void TEventActionsDialog::on_spinBoxDuration_valueChanged(int arg1)
{

}


void TEventActionsDialog::on_tableWidgetActions_cellActivated(int row, int column)
{

}

