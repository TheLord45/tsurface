/*
 * Copyright (C) 2025 by Andreas Theofilu <andreas@theosys.at>
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
#include "tpopuplist.h"
#include "ui_tpopuplist.h"

#include "terror.h"

TPopupList::TPopupList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TPopupList)
{
    DECL_TRACER("TPopupList::TPopupList(QWidget *parent)");

    ui->setupUi(this);
}

TPopupList::~TPopupList()
{
    DECL_TRACER("TPopupList::~TPopupList()");

    delete ui;
}

void TPopupList::setPopupList(const QList<QString>& list)
{
    DECL_TRACER("TPopupList::setPopupList(const QList<QString>& list)");

    if (list.isEmpty())
        return;

    QList<QString>::ConstIterator iter;

    for (iter = list.constBegin(); iter != list.constEnd(); ++iter)
        ui->listWidgetPopup->addItem(*iter);

    ui->listWidgetPopup->sortItems();
}

QList<QString> TPopupList::getSelectedPopups()
{
    DECL_TRACER("TPopupList::getSelectedPopups()");

    return mSelected;
}

void TPopupList::on_listWidgetPopup_itemClicked(QListWidgetItem *item)
{
    DECL_TRACER("TPopupList::on_listWidgetPopup_itemClicked(QListWidgetItem *item)");

    QString name = item->text();

    QList<QString>::ConstIterator iter;
    bool add = true;

    for (iter = mSelected.constBegin(); iter != mSelected.constEnd(); ++iter)
    {
        if (*iter == name)
        {
            ui->listWidgetPopup->setCurrentItem(item, QItemSelectionModel::ToggleCurrent);
            mSelected.erase(iter);
            add = false;
            break;
        }
    }

    if (!add)
        return;

    mSelected.append(name);
}

