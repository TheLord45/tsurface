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
#include "telementwidgetcombo.h"
#include "terror.h"

TEventActionsDialog::TEventActionsDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TEventActionsDialog)
{
    ui->setupUi(this);

    mCommands = { "sStan", "sPrev", "sShow", "sHide", "sToggle", "ClearG", "scPage", "scPanel" };
    mList1 = { "standard page", "previous page", "show popup", "hide popup", "toggle popup", "hide popup group", "hide popup on page", "hide all popups" };
    QList<int> disabled1 = { 0, 1, 5 };
    mList2 = { "show", "close", "close all", "show status", "hide status" };
    QList<int> disabled2 = { 0, 1 };
    mList3 = { "command", "string", "custom" };

    mBlock = true;
    ui->comboButtonAddPageFlip->setItems(mList1);

    for (int i = 0; i < disabled1.size(); ++i)
        ui->comboButtonAddPageFlip->disableItem(disabled1[i], true);

    ui->comboButtonAddLaunchOption->setItems(mList2);

    for (int i = 0; i < disabled2.size(); ++i)
        ui->comboButtonAddPageFlip->disableItem(disabled2[i], true);

    ui->comboButtonAddAction->setItems(mList3);
    mBlock = false;
}

TEventActionsDialog::~TEventActionsDialog()
{
    delete ui;
}

void TEventActionsDialog::setFuncs(const QList<ObjHandler::PUSH_FUNC_T>& funcs)
{
    DECL_TRACER("TEventActionsDialog::setFuncs(const QList<ObjHandler::PUSH_FUNC_T>& funcs)");

    mFuncs = funcs;

    if (mFuncs.empty())
        return;

    // Add the contents to the action table
    QList<ObjHandler::PUSH_FUNC_T>::Iterator iter;

    for (iter = mFuncs.begin(); iter != mFuncs.end(); ++iter)
    {
        if (mCommands.contains(iter->pfType))
        {
            int idx = mCommands.indexOf(iter->pfType);

            if (idx >= 0)
                addPageFlip(*iter, mList1[idx]);
        }
    }
}

void TEventActionsDialog::addPageFlip(const ObjHandler::PUSH_FUNC_T& pf, const QString& name)
{
    DECL_TRACER("TEventActionsDialog::addPageFlip(const ObjHandler::PUSH_FUNC_T& pf, const QString& name)");

    LINE_EVENT_t lev;
    QStringList popups = TPageHandler::Current().getPopups();

    lev.line = ui->tableWidgetActions->rowCount();
    lev.pf = pf;

    if (!popups.empty())
        lev.pf.pfName = popups[0];

    mLines.append(lev);

    ui->tableWidgetActions->setRowCount(lev.line + 1);
    QTableWidgetItem *cell1 = new QTableWidgetItem;
    cell1->setBackground(Qt::lightGray);
    cell1->setText(name);

    if (mCommands.contains(pf.pfType, Qt::CaseInsensitive))
    {
        QString cName = QString("PageFlip_%1").arg(lev.line);
        TElementWidgetCombo *combo = new TElementWidgetCombo(cName, ui->tableWidgetActions);
        combo->addItems(popups);
        connect(combo, &TElementWidgetCombo::selectionChanged, this, &TEventActionsDialog::onPageFlipSelectionChanged);
        ui->tableWidgetActions->setCellWidget(lev.line, 1, combo);
    }

    cell1->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);
    ui->tableWidgetActions->setItem(lev.line, 0, cell1);
    ui->tableWidgetActions->resizeColumnsToContents();
}

// Callbacks

void TEventActionsDialog::on_comboButtonAddPageFlip_currentIndexChanged(int index)
{
    DECL_TRACER("TEventActionsDialog::on_comboButtonAddPageFlip_currentIndexChanged(int index)");

    if (mBlock)
        return;

    ObjHandler::PUSH_FUNC_T func;
    func.ID = mFuncs.size();
    func.action = ObjHandler::BT_ACTION_PGFLIP;
    func.event = mEventType;
    func.pfType = mCommands[index];
    // Add an entry to the list
    addPageFlip(func, mList1[index]);
}


void TEventActionsDialog::on_comboButtonAddLaunchOption_currentIndexChanged(int index)
{
    DECL_TRACER("TEventActionsDialog::on_comboButtonAddLaunchOption_currentIndexChanged(int index)");

    if (mBlock)
        return;
}


void TEventActionsDialog::on_comboButtonAddAction_currentIndexChanged(int index)
{
    DECL_TRACER("TEventActionsDialog::on_comboButtonAddAction_currentIndexChanged(int index)");

    if (mBlock)
        return;
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

void TEventActionsDialog::onPageFlipSelectionChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TEventActionsDialog::onPageFlipSelectionChanged(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(data);

    int line = getObjectID(name, "PageFlip");

    if (line >= 0 && line < mLines.size())
        mLines[line].pf.pfName = text;
}
