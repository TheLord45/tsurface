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
#include <QGuiApplication>
#include <QTreeView>
#include <QStandardItem>
#include <QMenu>
#include <QCursor>

#include "tpagetree.h"
#include "terror.h"

TPageTree *TPageTree::mCurrent{nullptr};

TPageTree::TPageTree()
{
    DECL_TRACER("TPageTree::TPageTree()");
}

TPageTree::~TPageTree()
{
    DECL_TRACER("TPageTree::~TPageTree()");
}

TPageTree& TPageTree::Current()
{
    DECL_TRACER("TPageTree::Current()");

    if (!mCurrent)
        mCurrent = new TPageTree;

    return *mCurrent;
}

/**
 * @biref TPageTree::createNewTree
 * This method creates a new tree menu. The menu is initialized with 1 page
 * which is the main page. If not defined different by the settings, this page
 * will be drawn on startup of TPanel.
 *
 * @param tv    The pointer to the QTreeView element containing the page tree.
 * @param pname The name of the page. The page entry is named like this.
 * @param panel The name of the panel.
 */
void TPageTree::createNewTree(QTreeView* tv, const QString& job, const QString& pname, const QString& panel)
{
    DECL_TRACER("TPageTree::createNewTree(const QTreeView* tv, const QString& job, const QString& pname, const QString& panel)");

    if (!tv && !mTreeView)
    {
        MSG_ERROR("The tree view was not given and there exists none!");
        return;
    }

    if (mTreeView && tv && mTreeView != tv)
    {
        mTreeView->reset();
        disconnect(mTreeView, &QTreeView::pressed, this, &TPageTree::onClicked);
        disconnect(mTreeView, &QTreeView::doubleClicked, this, &TPageTree::onDoubleClicked);
        delete mTreeView;
        mTreeView = tv;
    }
    else if (!mTreeView)
        mTreeView = tv;
    else
        mTreeView->reset();

    mPageNum = 0;
    mPopupNum = 500;
    mGroupNum = 2000;

    if (!mItemModel)
        mItemModel = new QStandardItemModel;
    else
        mItemModel->clear();

    QString head = job + " [" + panel + "]";
    QStandardItem *header = new QStandardItem(head);
    mItemModel->setHorizontalHeaderItem(0, header);

    QStandardItem *parentItem = mItemModel->invisibleRootItem();
    // Folder Pages
    mPages = new QStandardItem(tr("Pages"));
    mPages->setEditable(false);
    mPages->setData(MENU_PAGE);
    // Page
    mPageNum++;
    QStandardItem *pageMain = new QStandardItem(pname);
    pageMain->setEditable(false);
    pageMain->setData(mPageNum);
    mPages->appendRow(pageMain);
    // Add page to folder Pages
    parentItem->appendRow(mPages);
    // Folder Popup Pages
    mPopup = new QStandardItem(tr("Popup pages"));
    mPopup->setEditable(false);
    mPopup->setData(MENU_POPUP);
    parentItem->appendRow(mPopup);
    // Folder Sup-pages
    mSubPages = new QStandardItem(tr("Sub-pages"));
    mSubPages->setEditable(false);
    mSubPages->setData(MENU_SUBPAGES);
    parentItem->appendRow(mSubPages);
    // Folder Apps
    mApps = new QStandardItem(tr("Application windows"));
    mApps->setEditable(false);
    mApps->setData(MENU_APPS);
    parentItem->appendRow(mApps);

    if (!mHaveModel)
        mTreeView->setModel(mItemModel);
    else
    {
        mHaveModel = true;
        mTreeView->show();
    }

    mTreeView->expandAll();
    connect(mTreeView, &QTreeView::pressed, this, &TPageTree::onClicked);
    connect(mTreeView, &QTreeView::doubleClicked, this, &TPageTree::onDoubleClicked);
}

void TPageTree::addPage(const QString& name, int num)
{
    DECL_TRACER("TPageTree::addPage(const QString& name, int num)");

    if (!mPages)
    {
        MSG_ERROR("Can't add a page to the tree because the tree was not initialized!");
        return;
    }

    QStandardItem *pg = new QStandardItem(name);
    pg->setEditable(false);
    pg->setData(num);
    mPages->appendRow(pg);
}

void TPageTree::addPopup(const QString& name, int num)
{
    DECL_TRACER("TPageTree::addPopup(const QString& name, int num)");

    if (!mPopup)
    {
        MSG_ERROR("Can't add a popup page to the tree because the tree was not initialized!");
        return;
    }

    QStandardItem *pg = new QStandardItem(name);
    pg->setEditable(false);
    pg->setData(num);
    mPopup->appendRow(pg);
}

void TPageTree::onDoubleClicked(const QModelIndex& index)
{
    DECL_TRACER("TPageTree::onDoubleClicked(const QModelIndex& index)");

    QStandardItem *item = mItemModel->itemFromIndex(index);
    int menu = item->data().toInt();
    MSG_DEBUG("Menu " << menu << " was double clicked.");

    // Is it a page or popup?
    if ((menu >= 1 && menu <= 50) ||
        (menu >= 500 && menu <= 1000))
    {
        if (menu <= 50)
            clicked(WTYPE_PAGE, menu, item->text());
        else
            clicked(WTYPE_POPUP, menu, item->text());
    }
}

void TPageTree::onClicked(const QModelIndex& index)
{
    DECL_TRACER("TPageTree::onClicked()");

    QStandardItem *item = mItemModel->itemFromIndex(index);
    int menu = item->data().toInt();
    MSG_DEBUG("Menu " << menu << " was clicked.");

    if (QGuiApplication::mouseButtons() == Qt::LeftButton && menu < MENU_PAGE)
        toFront(item->data().toInt());

    if (QGuiApplication::mouseButtons() != Qt::RightButton)
        return;

    if (menu >= MENU_PAGE && menu <= MENU_APPS)
    {
        menuPopup();
    }
}

void TPageTree::menuPopup()
{
    DECL_TRACER("TPageTree::menuPopup()");

    if (!mMenuPopup)
    {
        mMenuPopup = new QMenu;

        QAction *action = mMenuPopup->addAction(tr("Add page ..."));
        action->setData(POPMEN_ADDPAGE);
        connect(action, &QAction::triggered, this, &TPageTree::onMenuTriggeredAddPage);
        action = mMenuPopup->addAction(tr("Add popup page ..."));
        action->setData(POPMEN_ADDPOPUP);
        connect(action, &QAction::triggered, this, &TPageTree::onMenuTriggeredAddPopup);
        action = mMenuPopup->addAction(tr("Add application window ..."));
        action->setData(POPMEN_ADDAPP);
        connect(action, &QAction::triggered, this, &TPageTree::onMenuTriggeredAddApp);
        mMenuPopup->addSeparator();
        action = mMenuPopup->addAction(tr("Show popup page"));
        action->setData(POPMEN_SHOWPOP);
        action->setDisabled(true);
        connect(action, &QAction::triggered, this, &TPageTree::onMenuTriggeredShowPopup);
        action = mMenuPopup->addAction(tr("Hide popup page"));
        action->setData(POPMEN_HIDEPOP);
        action->setDisabled(true);
        connect(action, &QAction::triggered, this, &TPageTree::onMenuTriggeredHidePopup);
        action = mMenuPopup->addAction(tr("Show application window"));
        action->setData(POPMEN_SHOWAPP);
        action->setDisabled(true);
        connect(action, &QAction::triggered, this, &TPageTree::onMenuTriggeredShowApp);
        action = mMenuPopup->addAction(tr("Hide application window"));
        action->setData(POPMEN_HIDEAPP);
        action->setDisabled(true);
        connect(action, &QAction::triggered, this, &TPageTree::onMenuTriggeredHideApp);
        mMenuPopup->addSeparator();
        action = mMenuPopup->addAction(tr("Cut"));
        action->setData(POPMEN_CUT);
        action->setDisabled(true);
        connect(action, &QAction::triggered, this, &TPageTree::onMenuTriggeredCut);
        action = mMenuPopup->addAction(tr("Copy"));
        action->setData(POPMEN_COPY);
        action->setDisabled(true);
        connect(action, &QAction::triggered, this, &TPageTree::onMenuTriggeredCopy);
        action = mMenuPopup->addAction(tr("Paste"));
        action->setData(POPMEN_PASTE);
        action->setDisabled(true);
        connect(action, &QAction::triggered, this, &TPageTree::onMenuTriggeredPaste);
        action = mMenuPopup->addAction(tr("Delete"));
        action->setData(POPMEN_DELETE);
        action->setDisabled(true);
        connect(action, &QAction::triggered, this, &TPageTree::onMenuTriggeredDelete);
    }

    mMenuPopup->popup(QCursor::pos());
}

void TPageTree::onMenuTriggeredAddPage(bool checked)
{
    DECL_TRACER("TPageTree::onMenuTriggeredAddPage(bool checked)");

    Q_UNUSED(checked);
    addNewPage();
}

void TPageTree::onMenuTriggeredAddPopup(bool checked)
{
    DECL_TRACER("TPageTree::onMenuTriggeredAddPopup(bool checked)");

    Q_UNUSED(checked);
    addNewPopup();
}

void TPageTree::onMenuTriggeredAddApp(bool checked)
{
    DECL_TRACER("TPageTree::onMenuTriggeredAddApp(bool checked)");

    Q_UNUSED(checked);
    MSG_DEBUG("Popup action: Add application window ...");
}

void TPageTree::onMenuTriggeredShowPopup(bool checked)
{
    DECL_TRACER("TPageTree::onMenuTriggeredShowPopup(bool checked)");

    Q_UNUSED(checked);
    MSG_DEBUG("Popup action: Show popup page");
}

void TPageTree::onMenuTriggeredHidePopup(bool checked)
{
    DECL_TRACER("TPageTree::onMenuTriggeredHidePopup(bool checked)");

    Q_UNUSED(checked);
    MSG_DEBUG("Popup action: Hide popup page");
}

void TPageTree::onMenuTriggeredShowApp(bool checked)
{
    DECL_TRACER("TPageTree::onMenuTriggeredShowApp(bool checked)");

    Q_UNUSED(checked);
    MSG_DEBUG("Popup action: Show application window");
}

void TPageTree::onMenuTriggeredHideApp(bool checked)
{
    DECL_TRACER("TPageTree::onMenuTriggeredHideApp(bool checked)");

    Q_UNUSED(checked);
    MSG_DEBUG("Popup action: Hide application window");
}

void TPageTree::onMenuTriggeredCut(bool checked)
{
    DECL_TRACER("TPageTree::onMenuTriggeredCut(bool checked)");

    Q_UNUSED(checked);
    MSG_DEBUG("Popup action: Cut");
}

void TPageTree::onMenuTriggeredCopy(bool checked)
{
    DECL_TRACER("TPageTree::onMenuTriggeredCopy(bool checked)");

    Q_UNUSED(checked);
    MSG_DEBUG("Popup action: Copy");
}

void TPageTree::onMenuTriggeredPaste(bool checked)
{
    DECL_TRACER("TPageTree::onMenuTriggeredPaste(bool checked)");

    Q_UNUSED(checked);
    MSG_DEBUG("Popup action: Paste");
}

void TPageTree::onMenuTriggeredDelete(bool checked)
{
    DECL_TRACER("TPageTree::onMenuTriggeredDelete(bool checked)");

    Q_UNUSED(checked);
    MSG_DEBUG("Popup action: Delete");
}
