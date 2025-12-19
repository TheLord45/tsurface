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
#include <QTreeView>
#include <QStandardItemModel>
#include <QJsonObject>
#include <QJsonDocument>

#include "tconfmain.h"
#include "terror.h"

using namespace ConfigMain;

TConfMain *TConfMain::mCurrent{nullptr};

TConfMain::TConfMain()
{
    DECL_TRACER("TConfMain::TConfMain()");
}

TConfMain::~TConfMain()
{
    DECL_TRACER("TConfMain::~TConfMain()");

    if (mConfMain)
    {
        delete mConfMain;
        mConfMain = nullptr;
    }
}

TConfMain& TConfMain::Current()
{
    if (!mCurrent)
        mCurrent = new TConfMain;

    return *mCurrent;
}

void TConfMain::createNew()
{
    DECL_TRACER("TConfMain::createNew()");

    if (mConfMain)
        delete mConfMain;

    mConfMain = new CONFMAIN_t;

    if (!mTreeView)
    {
        MSG_WARNING("There is no tree view set. Can't update the tree of pages!");
        return;
    }

    // If there exists already a tree model, we must delete it first.
    mTreeView->reset();

    if (!mItemModel)
        mItemModel = new QStandardItemModel;

    QStandardItem *parentItem = mItemModel->invisibleRootItem();
    QStandardItem *pages = new QStandardItem(tr("Pages"));
    pages->setEditable(false);
    pages->setData(1);
    parentItem->appendRow(pages);
    QStandardItem *subpages = new QStandardItem(tr("Popup pages"));
    pages->setEditable(false);
    pages->setData(2);
    parentItem->appendRow(subpages);

    if (!mHaveModel)
        mTreeView->setModel(mItemModel);
    else
    {
        mHaveModel = true;
        mTreeView->show();
    }
}

bool TConfMain::readMain(const QString& path)
{
    DECL_TRACER("TConfMain::readMain(const QString& path)");

    return false;
}
