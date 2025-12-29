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

class QTreeView;
class QStandardItemModel;
class QMenu;
class QModelIndex;

class TPageTree : public QObject
{
    Q_OBJECT;

    public:
        TPageTree();
        ~TPageTree();

        static TPageTree& Current();
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

        void createNewTree(QTreeView *tv, const QString& job, const QString& pname, const QString& panel);

    signals:
        void clicked(const WINTYPE_t wt, int num, const QString& name);

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

    private:
        static TPageTree *mCurrent;

        QTreeView *mTreeView{nullptr};
        QStandardItemModel *mItemModel{nullptr};
        QMenu *mMenuPopup{nullptr};
        bool mHaveModel{false};
        int mPageNum{0};            // The number of a page. Range 1 to 50
        int mSubPageNum{500};       // The number of a subpage. Range 500 to 1000
        int mGroupNum{2000};        // The number of a group. Range 2000 to 2100
};

#endif // TPAGETREE_H
