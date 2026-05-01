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
#ifndef TPROPERTIESEVENTS_H
#define TPROPERTIESEVENTS_H

#include <QObject>

#include "tpagehandler.h"

class QTableWidget;
class TElementEvent;

class TPropertiesEvents : public QObject
{
    Q_OBJECT

    public:
        TPropertiesEvents();
        TPropertiesEvents(QTableWidget *table);
        ~TPropertiesEvents();

        void setTableWidget(QTableWidget *table);
        void setPage(Page::PAGE_t *page, int oIndex=-1);
        void setObjectIndex(int index);
        void setState(STATE_TYPE st) { mSType = st; }
        void reset();

    protected:
        virtual void pageNameChanged(int id, const QString& name) = 0;
        virtual void saveChangedData(Page::PAGE_t *page, PROPERTIES_t prop=TBL_UNKNOWN) = 0;
        virtual void markChanged() = 0;
        virtual void requestRedraw(Page::PAGE_t *page) = 0;
        // Callbacks
        void onCellActivated(int row, int column);

    private:
        void doConnect(QTableWidget *table);
        QString getLabelText(int line);
        void createTable();
        TElementEvent *makeEvent(ObjHandler::BUTTON_EVENT_t type);
        QList<ObjHandler::PUSH_FUNC_T> collectEvents();
        void assignEvent(Page::EVENT_t& pEvent, ObjHandler::BUTTON_EVENT_t ev, ObjHandler::PUSH_FUNC_T *pf);

        QTableWidget *mTable{nullptr};
        bool mConnected{false};
        bool mInitialized{false};
        Page::PAGE_t *mPage{nullptr};
        int mObjectIndex{-1};
        ObjHandler::TOBJECT_t mObject;
        QList<ObjHandler::PUSH_FUNC_T> mPageEvents;
        STATE_TYPE mSType{STATE_UNKNOWN};
        bool mChanged = false;
};

#endif // TPROPERTIESEVENTS_H
