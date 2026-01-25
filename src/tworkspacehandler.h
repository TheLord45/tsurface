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

#include <functional>

#include "tpagetree.h"
#include "tpropertiesgeneral.h"
#include "tpropertiesprogramming.h"
#include "tpropertiesstates.h"
#include "tobjecthandler.h"
#include "tmisc.h"

class QTreeView;
class QTableWidget;
class QTreeWidget;

class TWorkSpaceHandler
    : public TPageTree,
      public TPropertiesGeneral,
      public TPropertiesProgramming,
      public TPropertiesStates
{
    public:
        TWorkSpaceHandler();
        TWorkSpaceHandler(QTreeView *tree, QTableWidget *general, QTableWidget *prog, QTreeWidget *states, QWidget *parent=nullptr);
        ~TWorkSpaceHandler();

        static TWorkSpaceHandler& Current();
        static TWorkSpaceHandler& Current(QTreeView *tree, QTableWidget *general, QTableWidget *prog, QTreeWidget *states, QWidget *parent=nullptr);

        void setParent(QWidget *widget);
        void setWorkspacePagesWidget(QTreeView *tree);
        void setPropertiesGeneralWidget(QTableWidget *widget);
        bool isChanged();
        Page::PAGE_t& getActualPage() { return TPropertiesGeneral::getActualPage(); }
        void setStateType(STATE_TYPE st) { mStateType = st; }
        void setObjectGeometry(int pageID, int bi, const QRect& geom);
        void setActualObject(TObjectHandler *object);

        void setPage(const QString& name);
        void setPage(int id);
        void setPopup(const QString& name);
        void setPopup(int id);

        // Callbacks
        // In this case we can't use the mechanism of Qt with "signals",
        // because all three subclasses define the Q_OBJECT. Therefore
        // we would have an ambiguous name lookup. To overcome this we use
        // the methods of "functional". The result is the same.
        void regDataChanged(std::function<void (Page::PAGE_t *page)> func) { _dataChanged = func; }
        void regMarkDirty(std::function<void ()> func) { _markDirty = func; }

    protected:
        void pageNameChanged(int id, const QString& name) override;
        void saveChangedData(Page::PAGE_t *page, PROPERTIES_t prop) override;
        void markChanged() override;
        ObjHandler::TOBJECT_t getActualObject() override;

    private:
        std::function<void (Page::PAGE_t *page)> _dataChanged{nullptr};
        std::function<void ()> _markDirty{nullptr};

        static TWorkSpaceHandler *mCurrent;
        QWidget *mParent{nullptr};
        STATE_TYPE mStateType{STATE_UNKNOWN};
        TObjectHandler *mObject{nullptr};
};

#endif // TWORKSPACEHANDLER_H
