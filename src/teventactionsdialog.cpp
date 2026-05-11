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
#include "telementspintext.h"
#include "telementspincustom.h"
#include "terror.h"

#define CMD_STANDARD_PAGE       0
#define CMD_PREVIOUS_PAGE       1
#define CMD_SHOW_POPUP          2
#define CMD_HIDE_POPUP          3
#define CMD_TOGGLE_POPUP        4
#define CMD_HIDE_POPUP_GROUP    5
#define CMD_HIDE_POPUP_ON_PAGE  6
#define CMD_HIDE_ALL_POUPS      7

#define CMD_LAUNCH_SHOW         0
#define CMD_LAUNCH_CLOSE        1
#define CMD_LAUNCH_CLOSE_ALL    2
#define CMD_LAUNCH_SHOW_STATUS  3
#define CMD_LAUNCH_HIDE_STATUS  4

#define CMD_ACTION_COMMAND      0
#define CMD_ACTION_STRING       1
#define CMD_ACTION_CUSTOM       2

TEventActionsDialog::TEventActionsDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TEventActionsDialog)
{
    DECL_TRACER("TEventActionsDialog::TEventActionsDialog(QWidget *parent)");

    ui->setupUi(this);

    //               0        1        2        3         4          5         6         7
    mCommands1 = { "sStan", "sPrev", "sShow", "sHide", "sToggle", "ClearG", "scPage", "scPanel" };
    mList1 = { "standard page", "previous page", "show popup", "hide popup", "toggle popup", "hide popup group", "hide popup on page", "hide all popups" };
    QList<int> disabled1 = { 0, 1, 5 };

    mCommands2 = { "show", "close", "close_all", "status_show", "status_hide" };
    mList2 = { "show", "close", "close all", "show status", "hide status" };
    QList<int> disabled2 = { 0, 1 };

    mList3 = { "command", "string", "custom" };

    mPopups = TPageHandler::Current().getPopups();
    mPages = TPageHandler::Current().getPages();
    mGroups = TPageHandler::Current().getGroupNames();

    if (!mGroups.empty())
    {
        disabled1.remove(2);
        MSG_DEBUG("Have " << mGroups.size() << " groups.");
    }

    mBlock = true;
    ui->comboButtonAddPageFlip->setItems(mList1);

    for (int i = 0; i < disabled1.size(); ++i)
        ui->comboButtonAddPageFlip->disableItem(disabled1[i], true);

    ui->comboButtonAddLaunchOption->setItems(mList2);

    for (int i = 0; i < disabled2.size(); ++i)
        ui->comboButtonAddLaunchOption->disableItem(disabled2[i], true);

    ui->comboButtonAddAction->setItems(mList3);

    ui->tableWidgetActions->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetActions->setSelectionMode(QAbstractItemView::SingleSelection);
    setNavButtons();
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
        MSG_DEBUG("Function: " << iter->pfType.toStdString() << ", item: " << iter->item << ", Popup: " << iter->pfName.toStdString());

        if (mCommands1.contains(iter->pfType))
        {
            int idx = mCommands1.indexOf(iter->pfType);

            if (idx >= 0)
                addPageFlip(*iter, mList1[idx]);
        }
        else if (mCommands2.contains(iter->pfAction))
        {
            int idx = mCommands2.indexOf(iter->pfAction);

            if (idx >= 0)
                addLaunchOption(*iter, mList2[idx]);
        }
        else
        {
            addAction(*iter, iter->pfAction);
        }
    }

    setNavButtons();
}

void TEventActionsDialog::setDuration(int dur)
{
    DECL_TRACER("TEventActionsDialog::setDuration(int dur)");

    mDuration = dur;
    QSignalBlocker sig(ui->spinBoxDuration);
    ui->spinBoxDuration->setValue(dur);
}

void TEventActionsDialog::setAnimationFlip(AnimFlip_t flip)
{
    DECL_TRACER("TEventActionsDialog::setAnimationFlip(AnimFlip_t flip)");

    QSignalBlocker sigBlock(ui->listWidgetAnimations);
    ui->listWidgetAnimations->setCurrentRow(flip);
}

void TEventActionsDialog::setAnimationOrigin(AnimOrigin_t origin)
{
    DECL_TRACER("TEventActionsDialog::setAnimationOrigin(AnimOrigin_t origin)");

    mBlock = true;
    ui->radioButtonLeft->setChecked(false);
    ui->radioButtonRight->setChecked(false);
    ui->radioButtonTop->setChecked(false);
    ui->radioButtonBottom->setChecked(false);

    switch(origin)
    {
        case AnimLeft:      ui->radioButtonLeft->setChecked(true); break;
        case AnimRight:     ui->radioButtonRight->setChecked(true); break;
        case AnimTop:       ui->radioButtonTop->setChecked(true); break;
        case AnimBottom:    ui->radioButtonBottom->setChecked(true); break;
    }

    mBlock = false;
}

void TEventActionsDialog::addPageFlip(const ObjHandler::PUSH_FUNC_T& pf, const QString& name)
{
    DECL_TRACER("TEventActionsDialog::addPageFlip(const ObjHandler::PUSH_FUNC_T& pf, const QString& name)");

    LINE_EVENT_t lev;

    lev.line = ui->tableWidgetActions->rowCount();
    lev.pf = pf;
    mLines.append(lev);

    ui->tableWidgetActions->setRowCount(lev.line + 1);
    QTableWidgetItem *cell1 = new QTableWidgetItem;
    cell1->setBackground(Qt::lightGray);
    cell1->setText(name);

    if (mCommands1.contains(pf.pfType, Qt::CaseInsensitive))
    {
        QString cName = QString("PageFlip_%1").arg(lev.line);
        TElementWidgetCombo *combo = new TElementWidgetCombo(cName, ui->tableWidgetActions);
        int idx = -1;

        if (pf.pfType == mCommands1[CMD_HIDE_POPUP_ON_PAGE])
        {
            combo->addItems(mPages);
            idx = mPages.indexOf(lev.pf.pfName);
        }
        else if (pf.pfType == mCommands1[CMD_HIDE_ALL_POUPS])
        {
            lev.pf.pfName = "n/a";
            combo->addItem(lev.pf.pfName);
            combo->setDisabled(true);
        }
        else if (pf.pfType == mCommands1[CMD_HIDE_POPUP_GROUP])
        {
            combo->addItems(mGroups);
            idx = mGroups.indexOf(lev.pf.pfName);
        }
        else
        {
            combo->addItems(mPopups);
            idx = mPopups.indexOf(lev.pf.pfName);
        }

        if (idx >= 0)
            combo->setCurrentIndex(idx);

        connect(combo, &TElementWidgetCombo::selectionChanged, this, &TEventActionsDialog::onPageFlipSelectionChanged);
        ui->tableWidgetActions->setCellWidget(lev.line, 1, combo);
    }

    cell1->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);
    ui->tableWidgetActions->setItem(lev.line, 0, cell1);
    ui->tableWidgetActions->resizeColumnsToContents();
}

void TEventActionsDialog::addLaunchOption(const ObjHandler::PUSH_FUNC_T& pf, const QString& name)
{
    DECL_TRACER("TEventActionsDialog::addLaunchOption(const ObjHandler::PUSH_FUNC_T& pf, const QString& name)");

    LINE_EVENT_t lev;

    lev.line = ui->tableWidgetActions->rowCount();
    lev.pf = pf;
    mLines.append(lev);

    ui->tableWidgetActions->setRowCount(lev.line + 1);
    QTableWidgetItem *cell1 = new QTableWidgetItem;
    cell1->setBackground(Qt::lightGray);
    cell1->setText(name);

    if (mCommands2.contains(pf.pfAction, Qt::CaseInsensitive))
    {
        QString cName = QString("Launch_%1").arg(lev.line);
        TElementWidgetCombo *combo = new TElementWidgetCombo(cName, ui->tableWidgetActions);
        combo->addItem(lev.pf.pfName);
        combo->setDisabled(true);
        connect(combo, &TElementWidgetCombo::selectionChanged, this, &TEventActionsDialog::onPageFlipSelectionChanged);
        ui->tableWidgetActions->setCellWidget(lev.line, 1, combo);
    }

    cell1->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);
    ui->tableWidgetActions->setItem(lev.line, 0, cell1);
    ui->tableWidgetActions->resizeColumnsToContents();
}

void TEventActionsDialog::addAction(const ObjHandler::PUSH_FUNC_T& pf, const QString& name)
{
    DECL_TRACER("TEventActionsDialog::addAction(const ObjHandler::PUSH_FUNC_T& pf, const QString& name)");

    LINE_EVENT_t lev;

    lev.line = ui->tableWidgetActions->rowCount();
    lev.pf = pf;
    mLines.append(lev);

    ui->tableWidgetActions->setRowCount(lev.line + 1);
    QTableWidgetItem *cell1 = new QTableWidgetItem;
    cell1->setBackground(Qt::lightGray);
    cell1->setText(name);

    QString cName = QString("Action_%1").arg(lev.line);

    if (pf.action == ObjHandler::BT_ACTION_CUSTOM)
    {
        TElementSpinCustom *cust = new TElementSpinCustom(pf, cName, ui->tableWidgetActions);
        connect(cust, &TElementSpinCustom::contentChanged, this, &TEventActionsDialog::onActionChanged);
        ui->tableWidgetActions->setCellWidget(lev.line, 1, cust);
    }
    else
    {
        TElementSpinText *spt = new TElementSpinText(pf.port, pf.pfName, cName, ui->tableWidgetActions);
        connect(spt, &TElementSpinText::contentChanged, this, &TEventActionsDialog::onLaunchOptionChanged);
        ui->tableWidgetActions->setCellWidget(lev.line, 1, spt);
    }

    cell1->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);
    ui->tableWidgetActions->setItem(lev.line, 0, cell1);
    ui->tableWidgetActions->resizeColumnsToContents();
}

void TEventActionsDialog::setNavButtons()
{
    DECL_TRACER("TEventActionsDialog::setNavButtons()");

    ui->pushButtonDelete->setDisabled(true);
    ui->pushButtonClearAll->setDisabled(true);
    ui->pushButtonMoveUp->setDisabled(true);
    ui->pushButtonMoveDown->setDisabled(true);

    if (mFuncs.empty())             // Do we have any functions in table?
    {                               // No, then disable all buttons
        return;                     // We're already done.
    }
    // Get the selected row, if there is any.
    int row = getSelectedRow();

    if (row >= 0)                   // Do we have a selected row?
        ui->pushButtonDelete->setEnabled(true);
    else                            // No, then disable the delete button.
        ui->pushButtonDelete->setDisabled(true);

    ui->pushButtonClearAll->setEnabled(true);   // We can clear all any time

    if (mFuncs.size() > 1)          // Do we have more than 1 entry in the table?
    {                               // Yes ...
        if (row < 0)                // Are there no rows selected?
        {                           // No, then disable the Up/Down buttons
            ui->pushButtonMoveUp->setDisabled(true);
            ui->pushButtonMoveDown->setDisabled(true);
        }
        else if (row == 0)          // Is the 1st row selected?
        {                           // Yes, then enable only the Down button
            ui->pushButtonMoveUp->setDisabled(true);
            ui->pushButtonMoveDown->setEnabled(true);
        }
        else if (row == (ui->tableWidgetActions->rowCount() - 1))   // Is the last row selected?
        {                           // Yes, then enable only the Up button
            ui->pushButtonMoveUp->setEnabled(true);
            ui->pushButtonMoveDown->setDisabled(true);
        }
        else                        // A row between is selected?
        {                           // Yes, then enable both buttons
            ui->pushButtonMoveUp->setEnabled(true);
            ui->pushButtonMoveDown->setEnabled(true);
        }
    }
    else                            // Else we have no selection?
    {                               // Then disable both buttons
        ui->pushButtonMoveUp->setDisabled(true);
        ui->pushButtonMoveDown->setDisabled(true);
    }
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
    func.pfType = mCommands1[index];

    if (index == CMD_HIDE_POPUP_ON_PAGE)
        func.pfName = mPages.empty() ? "" : mPages[0];
    else if (index == CMD_HIDE_POPUP_GROUP)
        func.pfName = mGroups.empty() ? "" : mGroups[0];
    else
        func.pfName = mPopups.empty() ? "" : mPopups[0];

    mFuncs.append(func);
    // Add an entry to the list
    addPageFlip(func, mList1[index]);
    setNavButtons();
}


void TEventActionsDialog::on_comboButtonAddLaunchOption_currentIndexChanged(int index)
{
    DECL_TRACER("TEventActionsDialog::on_comboButtonAddLaunchOption_currentIndexChanged(int index)");

    if (mBlock)
        return;

    ObjHandler::PUSH_FUNC_T func;
    func.ID = 0;
    func.action = ObjHandler::BT_ACTION_LAUNCH;
    func.pfAction = mCommands2[index];
    func.event = mEventType;
    func.pfName = "n/a";
    mFuncs.append(func);
    // Add an entry to the list
    addLaunchOption(func, mList2[index]);
    setNavButtons();
}


void TEventActionsDialog::on_comboButtonAddAction_currentIndexChanged(int index)
{
    DECL_TRACER("TEventActionsDialog::on_comboButtonAddAction_currentIndexChanged(int index)");

    if (mBlock)
        return;

    ObjHandler::PUSH_FUNC_T func;

    switch(index)
    {
        case CMD_ACTION_COMMAND:
            func.action = ObjHandler::BT_ACTION_COMMAND;
            func.port = 1;
        break;

        case CMD_ACTION_STRING:
            func.action = ObjHandler::BT_ACTION_STRING;
            func.port = 1;
        break;

        case CMD_ACTION_CUSTOM:
            func.action = ObjHandler::BT_ACTION_CUSTOM;
            func.port = 1;
            func.key = getRandomNumber(100000000, 999999999);
        break;
    }

    func.pfAction = mList3[index];
    func.event = mEventType;
    func.pfName = "n/a";
    func.ID = func.type = 1;
    mFuncs.append(func);
    // Add an entry to the list
    addAction(func, mList3[index]);
    setNavButtons();
}

void TEventActionsDialog::on_pushButtonDelete_clicked()
{
    DECL_TRACER("TEventActionsDialog::on_pushButtonDelete_clicked()");

    int row = getSelectedRow();

    mLines.removeAt(row);
    ui->tableWidgetActions->removeRow(row);
}

void TEventActionsDialog::on_pushButtonClearAll_clicked()
{
    DECL_TRACER("TEventActionsDialog::on_pushButtonClearAll_clicked()");

    if (mLines.empty())
        return;

    mLines.clear();
    ui->tableWidgetActions->clear();
}

void TEventActionsDialog::on_pushButtonMoveUp_clicked()
{
    DECL_TRACER("TEventActionsDialog::on_pushButtonMoveUp_clicked()");

    int row = getSelectedRow();

    if (row == 0)
    {
        ui->pushButtonMoveUp->setDisabled(true);
        return;
    }

    QSignalBlocker sigBlock(ui->tableWidgetActions);
    moveRow(row, row - 1);
    mLines.move(row, row-1);
    setNavButtons();
    renumberLines();
}


void TEventActionsDialog::on_pushButtonMoveDown_clicked()
{
    DECL_TRACER("TEventActionsDialog::on_pushButtonMoveDown_clicked()");

    int row = getSelectedRow();

    if (row == (ui->tableWidgetActions->rowCount() - 1))
    {
        ui->pushButtonMoveDown->setDisabled(true);
        return;
    }

    QSignalBlocker sigBlock(ui->tableWidgetActions);
    moveRow(row, row+1);
    mLines.move(row, row+1);
    setNavButtons();
    renumberLines();
}


void TEventActionsDialog::on_listWidgetAnimations_itemClicked(QListWidgetItem *item)
{
    DECL_TRACER("TEventActionsDialog::on_listWidgetAnimations_itemClicked(QListWidgetItem *item)");

    int flip = ui->listWidgetAnimations->row(item);
    mAnimFlip = static_cast<AnimFlip_t>(flip);
}


void TEventActionsDialog::on_radioButtonLeft_clicked()
{
    DECL_TRACER("TEventActionsDialog::on_radioButtonLeft_clicked()");

    if (mBlock)
        return;

    mAnimOrigin = AnimLeft;
}


void TEventActionsDialog::on_radioButtonRight_clicked()
{
    DECL_TRACER("TEventActionsDialog::on_radioButtonRight_clicked()");

    if (mBlock)
        return;

    mAnimOrigin = AnimRight;
}


void TEventActionsDialog::on_radioButtonTop_clicked()
{
    DECL_TRACER("TEventActionsDialog::on_radioButtonTop_clicked()");

    if (mBlock)
        return;

    mAnimOrigin = AnimTop;
}


void TEventActionsDialog::on_radioButtonBottom_clicked()
{
    DECL_TRACER("TEventActionsDialog::on_radioButtonBottom_clicked()");

    if (mBlock)
        return;

    mAnimOrigin = AnimBottom;
}


void TEventActionsDialog::on_spinBoxDuration_valueChanged(int arg1)
{
    DECL_TRACER("TEventActionsDialog::on_spinBoxDuration_valueChanged(int arg1)");

    if (mBlock)
        return;

    mDuration = arg1;
}


void TEventActionsDialog::on_tableWidgetActions_cellActivated(int row, int column)
{
    DECL_TRACER("TEventActionsDialog::on_tableWidgetActions_cellActivated(int row, int column)");

    Q_UNUSED(column);

    QWidget *w = ui->tableWidgetActions->cellWidget(row, 1);

    if (!w)
        return;

    w->setFocus(Qt::OtherFocusReason);
    setNavButtons();
}

void TEventActionsDialog::on_tableWidgetActions_itemSelectionChanged()
{
    DECL_TRACER("TEventActionsDialog::on_tableWidgetActions_itemSelectionChanged()");

    setNavButtons();
}

void TEventActionsDialog::onPageFlipSelectionChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TEventActionsDialog::onPageFlipSelectionChanged(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(data);

    int line = getObjectID(name, "PageFlip");

    if (line >= 0 && line < mLines.size())
        mLines[line].pf.pfName = text;
}

void TEventActionsDialog::onLaunchOptionChanged(int port, const QString& text, const QString& name)
{
    DECL_TRACER("TEventActionsDialog::onLaunchOptionChanged(int port, const QString& text, const QString& name)");

    int line = getObjectID(name);

    if (line >= 0 && line < mLines.size())
    {
        mLines[line].pf.port = port;
        mLines[line].pf.pfName = text;
    }
}

void TEventActionsDialog::onActionChanged(const ObjHandler::PUSH_FUNC_T& pf, const QString& name)
{
    DECL_TRACER("TEventActionsDialog::onActionChanged(const ObjHandler::PUSH_FUNC_T& pf, const QString& name)");

    int line = getObjectID(name);

    if (line >= 0 && line < mLines.size())
        mLines[line].pf = pf;
}

int TEventActionsDialog::getSelectedRow()
{
    DECL_TRACER("TEventActionsDialog::getSelectedRow()");

    QItemSelectionModel *selModel = ui->tableWidgetActions->selectionModel();

    if (!selModel || !selModel->hasSelection())
    {
        MSG_WARNING("Got no selection model from table or have no selected row!");
        return -1;
    }

    QModelIndexList list = selModel->selectedRows();

    if (list.empty())   // Should never be true, but who knows
        return -1;

    int row = list[0].row();

    if (row < 0 || row >= mLines.size())
    {
        MSG_WARNING("Row " << row << " is out of range!");
        return -1;
    }

    return row;
}

TEventActionsDialog::RowData TEventActionsDialog::captureRow(int r) const
{
    DECL_TRACER("TEventActionsDialog::captureRow(int r)");

    RowData d;
    QTableWidget *table = ui->tableWidgetActions;

    if (QTableWidgetItem *it = table->item(r, 0))
        d.item0 = *it;

    if (QWidget *w = table->cellWidget(r, 1))
    {
        d.row = r;
        d.pf = mLines[r].pf;
    }

    MSG_DEBUG("pfType: " << d.pf.pfType.toStdString() << ", pfAction: " << d.pf.pfAction.toStdString() << ", action: " << d.pf.action << ", pfName: " << d.pf.pfName.toStdString());
    return d;
}

void TEventActionsDialog::populateRow(int r, const RowData& d)
{
    DECL_TRACER("TEventActionsDialog::populateRow(int r, const RowData& d)");

    QTableWidget *table = ui->tableWidgetActions;
    table->setItem(r, 0, new QTableWidgetItem(d.item0));

    if (d.pf.action == ObjHandler::BT_ACTION_PGFLIP)
    {
        QString cName = QString("PageFlip_%1").arg(r);
        TElementWidgetCombo *combo = new TElementWidgetCombo(cName, ui->tableWidgetActions);

        if (d.pf.pfType == mCommands1[CMD_HIDE_POPUP_ON_PAGE])
            combo->addItems(TPageHandler::Current().getPages());
        else if (d.pf.pfType == mCommands1[CMD_HIDE_ALL_POUPS])
        {
            combo->addItem(d.pf.pfName);
            combo->setDisabled(true);
        }
        else
            combo->addItems(mPopups);

        int idx = mPopups.indexOf(d.pf.pfName);

        if (idx >= 0)
            combo->setCurrentIndex(idx);

        connect(combo, &TElementWidgetCombo::selectionChanged, this, &TEventActionsDialog::onPageFlipSelectionChanged);
        table->setCellWidget(r, 1, combo);
    }
    else if (d.pf.action == ObjHandler::BT_ACTION_LAUNCH)
    {
        QString cName = QString("Launch_%1").arg(r);
        TElementWidgetCombo *combo = new TElementWidgetCombo(cName, ui->tableWidgetActions);
        combo->addItem(d.pf.pfName);
        combo->setDisabled(true);
        connect(combo, &TElementWidgetCombo::selectionChanged, this, &TEventActionsDialog::onPageFlipSelectionChanged);
        table->setCellWidget(r, 1, combo);
    }
    else if (d.pf.action == ObjHandler::BT_ACTION_COMMAND || d.pf.action == ObjHandler::BT_ACTION_STRING)
    {
        QString cName = QString("Action_%1").arg(r);

        TElementSpinText *spt = new TElementSpinText(d.pf.port, d.pf.pfName, cName, ui->tableWidgetActions);
        connect(spt, &TElementSpinText::contentChanged, this, &TEventActionsDialog::onLaunchOptionChanged);
        ui->tableWidgetActions->setCellWidget(r, 1, spt);
    }
    else if (d.pf.action == ObjHandler::BT_ACTION_CUSTOM)
    {
        QString cName = QString("Action_%1").arg(r);

        TElementSpinCustom *cust = new TElementSpinCustom(d.pf, cName, ui->tableWidgetActions);
        connect(cust, &TElementSpinCustom::contentChanged, this, &TEventActionsDialog::onActionChanged);
        ui->tableWidgetActions->setCellWidget(r, 1, cust);
    }
    else
        MSG_WARNING("No line! " << d.pf.pfType.toStdString() << ", " << d.pf.pfAction.toStdString() << ", " << d.pf.pfName.toStdString());
}

void TEventActionsDialog::moveRow(int from, int to)
{
    DECL_TRACER("TEventActionsDialog::moveRow(int from, int to) ");

    QTableWidget *table = ui->tableWidgetActions;

    if (from == to || from < 0 || to < 0 || from >= table->rowCount() || to >= table->rowCount())
        return;
    // Move by snapshotting and reconstructing; avoids destroying of live widgets
    RowData d = captureRow(from);
    table->removeRow(from);
    table->insertRow(to);
    populateRow(to, d);
}

void TEventActionsDialog::renumberLines()
{
    DECL_TRACER("TEventActionsDialog::renumberLines()");

    QList<LINE_EVENT_t>::Iterator iter;
    int line = 0;

    for (iter = mLines.begin(); iter != mLines.end(); ++iter)
    {
        iter->line = line;
        line++;
    }
}

void TEventActionsDialog::accept()
{
    DECL_TRACER("TEventActionsDialog::accept()");

    mFuncs.clear();
    QList<LINE_EVENT_t>::Iterator iter;
    int item = 0;

    for (iter = mLines.begin(); iter != mLines.end(); ++iter)
    {
        iter->pf.item = item;
        mFuncs.append(iter->pf);
        item++;
    }

    done(QDialog::Accepted);
}
