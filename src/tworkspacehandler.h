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
#ifndef TWORKSPACEHANDLER_H
#define TWORKSPACEHANDLER_H

#include <QWidget>

#include "tpagetree.h"
#include "tpropertiesgeneral.h"

class QTreeView;
class QTableWidget;
class TPropertiesGeneral;

class TWorkSpaceHandler : public TPageTree, public TPropertiesGeneral
{
    public:
        TWorkSpaceHandler();
        TWorkSpaceHandler(QTreeView *tree, QTableWidget *general, QWidget *parent=nullptr);
        ~TWorkSpaceHandler();

        static TWorkSpaceHandler& Current();
        static TWorkSpaceHandler& Current(QTreeView *tree, QTableWidget *general, QWidget *parent=nullptr);

        void setParent(QWidget *widget);
        void setWorkspacePagesWidget(QTreeView *tree);
        void setPropertiesGeneralWidget(QTableWidget *widget);

        // Properties

    protected:
        void onAddNewPage();
        void onAddNewPopup();
        void onItemToFront(int id);

    private:
        static TWorkSpaceHandler *mCurrent;
        QWidget *mParent{nullptr};
};

#endif // TWORKSPACEHANDLER_H
