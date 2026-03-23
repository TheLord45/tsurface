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

TPageTree::TPageTree(QTreeView *tree, QWidget *parent)
    : mTreeView(tree),
      mParent(parent)
{
    DECL_TRACER("TPageTree::TPageTree(QTreeView *tree, QWidget *parent)");
}

TPageTree::~TPageTree()
{
    DECL_TRACER("TPageTree::~TPageTree()");
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
void TPageTree::createNewTree(const QString& job, const QString& pname, const QString& panel, int pageID)
{
    DECL_TRACER("TPageTree::createNewTree(const QString& job, const QString& pname, const QString& panel, pageID)");

    if (!mTreeView)
    {
        MSG_ERROR("The tree view was not given and there exists none!");
        return;
    }

    makeTree(job, panel, pname, pageID);
}

void TPageTree::createTree(const QString& job, const QString& panel)
{
    DECL_TRACER("TPageTree::createTree(const QString& job, const QString& panel)");

    if (!mTreeView)
    {
        MSG_ERROR("The tree view was not given and there exists none!");
        return;
    }

    makeTree(job, panel, QString(), 0);
}

/**
 * @brief TPageTree::makeTree
 * This method creates a tree from scratch. In case there is already a tree it
 * deletes it first. Then a standrd tree is created.
 * If the parameter @b pname contains a string, a page entry is added to the
 * pages.
 * The parameters @b job and @b panel are used to write their content into the
 * header of the tree.
 *
 * @param job       The name of the job. This appears in the herader of the
 * tree.
 * @param panel     The type name of the panel. This is written inside of
 * brackets into the head.
 * @param pname     An optional page name. If it contains a string, a page with
 * this name is visible.
 * @param pageID    The ID of the page.
 */
void TPageTree::makeTree(const QString& job, const QString& panel, const QString& pname, int pageID)
{
    DECL_TRACER("TPageTree::makeTree(const QString& job, const QString& panel, const QString& pname, int pageID)");

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

    if (!pname.isEmpty())
    {
        // Page
        mPageNum++;
        QStandardItem *pageMain = new QStandardItem(pname);
        pageMain->setEditable(false);
        pageMain->setData(pageID);
        mPages->appendRow(pageMain);
    }

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

/**
 * @brief TPageTree::resetTree
 * Reset all parts of the tree. It removes all parts of the tree and disconnects
 * the callbacks. The internal counters are also reset.
 */
void TPageTree::resetTree()
{
    DECL_TRACER("TPageTree::reset()");

    if (!mTreeView)
        return;

    mTreeView->reset();
    disconnect(mTreeView, &QTreeView::pressed, this, &TPageTree::onClicked);
    disconnect(mTreeView, &QTreeView::doubleClicked, this, &TPageTree::onDoubleClicked);
    delete mTreeView;
    mPageNum = 0;
    mPopupNum = 500;
    mGroupNum = 2000;
}

/**
 * @brief TPageTree::addTreePage
 * This method adds a page to the page part.
 *
 * @param name  The name of the page
 * @param num   The ID of the page
 */
void TPageTree::addTreePage(const QString& name, int num)
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

/**
 * @brief TPageTree::addTreePopup
 * This method adds a popup to the popup part of the tree. If there is also a
 * group name, the popup is added to this group. If the group doesn't exist, it
 * will be created.
 *
 * @param name      The name of the popup
 * @param group     Optional: The name of the group the popup belongs to.
 * @param num       The ID of the popup.
 */
void TPageTree::addTreePopup(const QString& name, const QString& group, int num)
{
    DECL_TRACER("TPageTree::addPopup(const QString& name, const QString& group, int num)");

    if (!mPopup)
    {
        MSG_ERROR("Can't add a popup page to the tree because the tree was not initialized!");
        return;
    }

    QStandardItem *pg = new QStandardItem(name);
    pg->setEditable(false);
    pg->setData(num);

    if (!group.isEmpty())
    {
        // Search for the group
        for (QStandardItem *item : mPopupGroups)
        {
            if (item->text() == group)
            {
                item->appendRow(pg);
                return;
            }
        }

        // Here we know that the group doesn't exist. Therefor we add it.
        QStandardItem *gr = new QStandardItem(group);
        gr->setEditable(false);
        gr->setData(MENU_POPUPGROUP);
        mPopup->appendRow(gr);
        mPopupGroups.append(gr);
        // At least add the popup to the new group.
        gr->appendRow(pg);
    }
    else
        mPopup->appendRow(pg);
}

/**
 * @brief TPageTree::addTreeSubPage
 * This method adds a subpage to the subpage part of the tree.
 *
 * @param name  The name of the subpage
 * @param num   The ID of the subpage.
 */
void TPageTree::addTreeSubPage(const QString& name, int num)
{
    DECL_TRACER("TPageTree::addTreeSubPage(const QString& name, int num)");

    if (!mSubPages)
    {
        MSG_ERROR("Can't add a subpage to the tree because the tree was not initialized!");
        return;
    }

    QStandardItem *pg = new QStandardItem(name);
    pg->setEditable(false);
    pg->setData(num);
    mSubPages->appendRow(pg);
}

void TPageTree::updatePageName(int id, const QString& name)
{
    DECL_TRACER("TPageTree::updatePageName(int id, const QString& name)");

    if (!mPages || !mPages->hasChildren())
        return;

    int rows = mPages->rowCount();

    for (int i = 0; i < rows; ++i)
    {
        QStandardItem *item = mPages->child(i, 0);

        if (item && item->data().toInt() == id)
        {
            item->setText(name);
            break;
        }
    }
}

void TPageTree::updatePopupName(int id, const QString& name)
{
    DECL_TRACER("TPageTree::updatePopupName(int id, const QString& name)");

    if (!mPopup || !mPopup->hasChildren())
        return;

    int rows = mPopup->rowCount();

    for (int i = 0; i < rows; ++i)
    {
        QStandardItem *item = mPopup->child(i, 0);

        if (item && item->data().toInt() == id)
        {
            item->setText(name);
            break;
        }
    }
}

void TPageTree::updateSubPageName(int id, const QString& name)
{
    DECL_TRACER("TPageTree::updateSubPageName(int id, const QString& name)");

    if (!mSubPages || !mSubPages->hasChildren())
        return;

    int rows = mSubPages->rowCount();

    for (int i = 0; i < rows; ++i)
    {
        QStandardItem *item = mSubPages->child(i, 0);

        if (item && item->data().toInt() == id)
        {
            item->setText(name);
            break;
        }
    }
}

/**
 * @brief TPageTree::setPageType
 * This method moves a subpage to a page and the other way around depending on
 * the parameter @b ptype.
 *
 * @param ptype     The new type of the page
 * @param pageID    The ID of the page. This ID never changes.
 */
void TPageTree::setPageType(Page::PAGE_TYPE ptype, int pageID, const QString& group)
{
    DECL_TRACER("TPageTree::setPageType(Page::PAGE_TYPE ptype, int pageID, const QString& group)");

    if (ptype == Page::PT_SUBPAGE)
    {
        if (!mPopup || !mPopup->hasChildren())
            return;

        int rows = mPopup->rowCount();

        for (int i = 0; i < rows; ++i)
        {
            QStandardItem *item = mPopup->child(i, 0);

            if (item && item->data().toInt() == pageID)
            {
                addTreeSubPage(item->text(), pageID);
                mPopup->removeRow(i);
                return;
            }
            else if (item && item->data().toInt() == MENU_POPUPGROUP)
            {
                for (int j = 0; j < item->rowCount(); ++j)
                {
                    QStandardItem *it = item->child(j, 0);

                    if (it && it->data().toInt() == pageID)
                    {
                        addTreeSubPage(it->text(), pageID);
                        item->removeRow(j);
                        return;
                    }
                }
            }
        }
    }
    else
    {
        if (!mSubPages || !mSubPages->hasChildren())
            return;

        int rows = mSubPages->rowCount();

        for (int i = 0; i < rows; ++i)
        {
            QStandardItem *item = mSubPages->child(i, 0);

            if (item && item->data().toInt() == pageID)
            {
                addTreePopup(item->text(), group, pageID);
                mSubPages->removeRow(i);
                return;
            }
        }
    }
}

/**
 * @brief TPageTree::setPopupGroup
 * The method searches for a popup with the page ID @b pageID. If it finds the
 * popup, it removes it first. Then it adds it again by passing the group name.
 * If the popup was in a group and the group name was empty, it is moved to the
 * main part of popups. A popup from the main part or any group can be moved to
 * any other group.
 * The method assumes the popup with the ID @b pageID exists. If not, nothing
 * happens.
 *
 * @param group     The new group name the popup belongs to. This may be empty.
 * @param pageID    The page ID of the popup.
 */
void TPageTree::setPopupGroup(const QString& group, int pageID)
{
    DECL_TRACER("TPageTree::setPopupGroup(const QString& group, int pageID)");

    if (!mPopup || !mPopup->hasChildren())
        return;

    bool found = false;
    QString name;
    int rows = mPopup->rowCount();

    for (int i = 0; i < rows; ++i)
    {
        QStandardItem *item = mPopup->child(i, 0);

        if (item)
        {
            if (item->data().toInt() == pageID)
            {
                name = item->text();
                mPopup->removeRow(i);
                found = true;
                break;
            }
            else if (item->data().toInt() == MENU_POPUPGROUP)
            {
                for (int j = 0; j < item->rowCount(); ++j)
                {
                    QStandardItem *it = item->child(j, 0);

                    if (it && it->data().toInt() == pageID)
                    {
                        name = it->text();
                        item->removeRow(j);

                        if (!item->hasChildren())
                            removeGroupFromList(item->text());

                        found = true;
                        break;
                    }
                }

                if (found)
                    break;
            }
        }
    }

    if (found)
        addTreePopup(name, group, pageID);
}

void TPageTree::removeGroupFromList(const QString& name)
{
    DECL_TRACER("TPageTree::removeGroupFromList(const QString& name)");

    QList<QStandardItem *>::Iterator iter;

    for (iter = mPopupGroups.begin(); iter != mPopupGroups.end(); ++iter)
    {
        QStandardItem *item = *iter;

        if (item->text() == name)
        {
            if (item->hasChildren())
            {
                MSG_WARNING("Group " << item->text().toStdString() << " is not empty! Will not remove it.");
                return;
            }

            mPopupGroups.erase(iter);
            return;
        }
    }
}

void TPageTree::setFocus(int id)
{
    DECL_TRACER("TPageTree::setFocus(int id)");

    QItemSelectionModel *selModel = mTreeView->selectionModel();

    if (!selModel)
    {
        MSG_DEBUG("No selection model found!");
        return;
    }

    int rows = mPages->rowCount();

    for (int i = 0; i < rows; ++i)
    {
        QStandardItem *item = mPages->child(i, 0);

        if (item && item->data().toInt() == id)
        {
            selModel->setCurrentIndex(item->index(), QItemSelectionModel::ClearAndSelect);
            return;
        }
    }

    rows = mPopup->rowCount();

    for (int i = 0; i < rows; ++i)
    {
        QStandardItem *item = mPopup->child(i, 0);

        if (item && item->data().toInt() == id)
        {
            selModel->setCurrentIndex(item->index(), QItemSelectionModel::ClearAndSelect);
            return;
        }
    }

    rows = mSubPages->rowCount();

    for (int i = 0; i < rows; ++i)
    {
        QStandardItem *item = mSubPages->child(i, 0);

        if (item && item->data().toInt() == id)
        {
            selModel->setCurrentIndex(item->index(), QItemSelectionModel::ClearAndSelect);
            return;
        }
    }

    rows = mApps->rowCount();

    for (int i = 0; i < rows; ++i)
    {
        QStandardItem *item = mApps->child(i, 0);

        if (item && item->data().toInt() == id)
        {
            selModel->setCurrentIndex(item->index(), QItemSelectionModel::ClearAndSelect);
            return;
        }
    }
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
        emit windowToFront(item->data().toInt());

    if (QGuiApplication::mouseButtons() != Qt::RightButton)
        return;

    if (menu >= MENU_PAGE && menu <= MENU_APPS)
        menuPopup();
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
    emit addNewTreePage();
}

void TPageTree::onMenuTriggeredAddPopup(bool checked)
{
    DECL_TRACER("TPageTree::onMenuTriggeredAddPopup(bool checked)");

    Q_UNUSED(checked);
    emit addNewTreePopup();
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
