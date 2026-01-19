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

#ifndef TPAGETREE_H
#define TPAGETREE_H

#include <QString>
#include <QObject>

class QWidget;
class QTreeView;
class QStandardItemModel;
class QMenu;
class QModelIndex;
class QStandardItem;

class TPageTree : public QObject
{
    Q_OBJECT

    public:
        TPageTree(QTreeView *tree, QWidget *parent=nullptr);
        ~TPageTree();

        const int MENU_PAGE = 10001;
        const int MENU_POPUP = 10002;
        const int MENU_SUBPAGES = 10003;
        const int MENU_APPS = 10004;

        const int POPMEN_ADDPAGE = 1;
        const int POPMEN_ADDPOPUP = 2;
        const int POPMEN_ADDAPP = 3;
        const int POPMEN_SHOWPOP = 4;
        const int POPMEN_HIDEPOP = 5;
        const int POPMEN_SHOWAPP = 6;
        const int POPMEN_HIDEAPP = 7;
        const int POPMEN_CUT = 8;
        const int POPMEN_COPY = 9;
        const int POPMEN_PASTE = 10;
        const int POPMEN_DELETE = 11;

        typedef enum
        {
            WTYPE_PAGE = 1,
            WTYPE_POPUP,
            WTYPE_APP
        }WINTYPE_t;

        void setParent(QWidget *parent) { mParent = parent; }
        void setTreeView(QTreeView *tree) { mTreeView = tree; }
        void createNewTree(const QString& job, const QString& pname, const QString& panel);
        void createTree(const QString& job, const QString& panel);
        void resetTree();
        void addTreePage(const QString& name, int num);
        void addTreePopup(const QString& name, int num);
//        void addSubPage(const QString& name, int num);
//        void addApp(const QString& name, int num);
        void updatePageName(int id, const QString& name);
        void updatePopupName(int id, const QString& name);

    signals:
        void clicked(const WINTYPE_t wt, int num, const QString& name);
        void windowToFront(int number);
        void addNewTreePage();
        void addNewTreePopup();
//        void addNewSubPage();
//        void addNewApp();

    protected:
        void onClicked(const QModelIndex& index);
        void onDoubleClicked(const QModelIndex &index);
        void onMenuTriggeredAddPage(bool checked);
        void onMenuTriggeredAddPopup(bool checked);
        void onMenuTriggeredAddApp(bool checked);
        void onMenuTriggeredShowPopup(bool checked);
        void onMenuTriggeredHidePopup(bool checked);
        void onMenuTriggeredShowApp(bool checked);
        void onMenuTriggeredHideApp(bool checked);
        void onMenuTriggeredCut(bool checked);
        void onMenuTriggeredCopy(bool checked);
        void onMenuTriggeredPaste(bool checked);
        void onMenuTriggeredDelete(bool checked);

        void menuPopup();
        TPageTree *getPointer() { return this; }

    private:
        QTreeView *mTreeView{nullptr};
        QWidget *mParent{nullptr};
        QStandardItemModel *mItemModel{nullptr};
        QStandardItem *mPages{nullptr};     // Pointer to tree part containing the pages
        QStandardItem *mPopup{nullptr};     // Pointer to tree part containing the popups
        QStandardItem *mSubPages{nullptr};  // Pointer to tree part containing the subpages
        QStandardItem *mApps{nullptr};      // Pointer to tree part containing the apps
        QMenu *mMenuPopup{nullptr};
        bool mHaveModel{false};
        int mPageNum{0};            // The number of a page. Range 1 to 50
        int mPopupNum{500};         // The number of a subpage. Range 500 to 1000
        int mGroupNum{2000};        // The number of a group. Range 2000 to 2100
};

#endif // TPAGETREE_H
