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
#include <QTableWidget>

#include "tpropertiesevents.h"
#include "telementevent.h"
#include "terror.h"

#define TTEXT_SHOW_PAGE             1
#define TTEXT_HIDE_PAGE             2
#define TTEXT_BUTTON_PRESS          3
#define TTEXT_BUTTON_RELEASE        4
#define TTEXT_GESTURE_ANY           5
#define TTEXT_GESTURE_UP            6
#define TTEXT_GESTURE_DOWN          7
#define TTEXT_GESTURE_RIGHT         8
#define TTEXT_GESTURE_LEFT          9
#define TTEXT_GESTURE_DBL_TAP       10
#define TTEXT_GESTURE_2FINGER_UP    11
#define TTEXT_GESTURE_2FINGER_DN    12
#define TTEXT_GESTURE_2FINGER_RT    13
#define TTEXT_GESTURE_2FINGER_LT    14
#define TTEXT_ITEM_SELECTED         15
#define TTEXT_SCROLLBAR_BEGIN       16
#define TTEXT_SCROLLBAR_END         17

#define LIST_SHOW_PAGE              0
#define LIST_HIDE_PAGE              1
#define LIST_BUTTON_PRESS           2
#define LIST_BUTTON_RELEASE         3
#define LIST_GESTURE_ANY            4
#define LIST_GESTURE_UP             5
#define LIST_GESTURE_DOWN           6
#define LIST_GESTURE_RIGHT          7
#define LIST_GESTURE_LEFT           8
#define LIST_GESTURE_DBL_TAP        9
#define LIST_GESTURE_2FINGER_UP     10
#define LIST_GESTURE_2FINGER_DN     11
#define LIST_GESTURE_2FINGER_RT     12
#define LIST_GESTURE_2FINGER_LT     13
#define LIST_ITEM_SELECTED          14
#define LIST_SCROLLBAR_BEGIN        15
#define LIST_SCROLLBAR_END          16

#define MAX_LIST_ENTRIES            17

TPropertiesEvents::TPropertiesEvents()
{
    DECL_TRACER("TPropertiesEvents::TPropertiesEvents()");
}

TPropertiesEvents::TPropertiesEvents(QTableWidget *table)
    : mTable(table)
{
    DECL_TRACER("TPropertiesEvents::TPropertiesEvents(QTableWidget *table)");

    doConnect(table);
}

TPropertiesEvents::~TPropertiesEvents()
{
    DECL_TRACER("TPropertiesEvents::~TPropertiesEvents()");

    if (mTable && mConnected)
    {
        disconnect(mTable, &QTableWidget::cellActivated, this, &TPropertiesEvents::onCellActivated);
        mConnected = false;
    }
}

void TPropertiesEvents::setTableWidget(QTableWidget *table)
{
    DECL_TRACER("TPropertiesEvents::setTableWidget(QTableWidget *table)");

    if (table && table != mTable)
        mTable = table;
    else
        return;

    doConnect(table);
}

void TPropertiesEvents::setPage(Page::PAGE_t *page, int oIndex)
{
    DECL_TRACER("TPropertiesEvents::setPage(Page::PAGE_t *page, int oIndex)");

    if (!page || page->pageID <= 0)
        return;

    mPage = page;
    mObjectIndex = oIndex;

    if (oIndex >= 0 && oIndex < page->objects.size())
        mObject = page->objects[oIndex]->getObject();

    // TODO: Add code to create and handle the table
}

void TPropertiesEvents::setObjectIndex(int index)
{
    DECL_TRACER("TPropertiesEvents::setObjectIndex(int index)");

    if (!mPage || index < 0 || index >= mPage->objects.size())
        return;

    mObjectIndex = index;
    mObject = mPage->objects[index]->getObject();
}

void TPropertiesEvents::reset()
{
    DECL_TRACER("TPropertiesEvents::reset()");

    if (mPage && mPage->pageID > 0 && mChanged)
        saveChangedData(mPage, TBL_EVENTS);

    mChanged = false;
    mPage = nullptr;
    mTable->clear();
}

void TPropertiesEvents::doConnect(QTableWidget *table)
{
    DECL_TRACER("TPropertiesEvents::doConnect(QTableWidget *table)");

    if ((table && table != mTable) || (table && !mConnected))
    {
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        connect(table, &QTableWidget::cellActivated, this, &TPropertiesEvents::onCellActivated);
        mConnected = true;
    }
}

QString TPropertiesEvents::getLabelText(int line)
{
    DECL_TRACER("TPropertiesEvents::getLabelText(int line)");

    switch(line)
    {
        case TTEXT_SHOW_PAGE:           return tr("Show Page");
        case TTEXT_HIDE_PAGE:           return tr("Hide Page");
        case TTEXT_BUTTON_PRESS:        return tr("Button Press");
        case TTEXT_BUTTON_RELEASE:      return tr("Button Release");
        case TTEXT_GESTURE_ANY:         return tr("Gesture Any");
        case TTEXT_GESTURE_UP:          return tr("Gesture Up");
        case TTEXT_GESTURE_DOWN:        return tr("Gesture Down");
        case TTEXT_GESTURE_RIGHT:       return tr("Gesture Right");
        case TTEXT_GESTURE_LEFT:        return tr("Gesture Left");
        case TTEXT_GESTURE_DBL_TAP:     return tr("Gesture Dbl Tap");
        case TTEXT_GESTURE_2FINGER_UP:  return tr("Gesture 2-Finger Up");
        case TTEXT_GESTURE_2FINGER_DN:  return tr("Gesture 2-Finger Dn");
        case TTEXT_GESTURE_2FINGER_RT:  return tr("Gesture 2-Finger Rt");
        case TTEXT_GESTURE_2FINGER_LT:  return tr("Gesture 2-Finger Lt");
        case TTEXT_ITEM_SELECTED:       return tr("Item Selected");
        case TTEXT_SCROLLBAR_BEGIN:     return tr("Scrollbar Begin");
        case TTEXT_SCROLLBAR_END:       return tr("Scrollbar End");
    }

    return QString();
}

void TPropertiesEvents::createTable()
{
    DECL_TRACER("TPropertiesEvents::createTable()");

    if (!mTable)
        return;

    QSignalBlocker sigBlock(mTable);
    mTable->clear();
    mTable->setColumnCount(2);
    mTable->setRowCount(MAX_LIST_ENTRIES);

    for (int i = 0; i < MAX_LIST_ENTRIES; ++i)
    {
        QTableWidgetItem *cell1 = new QTableWidgetItem;
        QTableWidgetItem *cell2 = nullptr;

        cell1->setBackground(Qt::lightGray);

        switch(i)
        {
            case LIST_SHOW_PAGE:
                cell1->setText(getLabelText(TTEXT_SHOW_PAGE));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_SHOW));
            break;

            case LIST_HIDE_PAGE:
                cell1->setText(getLabelText(TTEXT_HIDE_PAGE));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_HIDE));
            break;

            case LIST_BUTTON_PRESS:
                cell1->setText(getLabelText(TTEXT_BUTTON_PRESS));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_PRESS));
            break;

            case LIST_BUTTON_RELEASE:
                cell1->setText(getLabelText(TTEXT_BUTTON_RELEASE));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_RELEASE));
            break;

            case LIST_GESTURE_ANY:
                cell1->setText(getLabelText(TTEXT_GESTURE_ANY));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_GUESTURE_ANY));
            break;

            case LIST_GESTURE_UP:
                cell1->setText(getLabelText(TTEXT_GESTURE_UP));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_GUESTURE_UP));
            break;

            case LIST_GESTURE_DOWN:
                cell1->setText(getLabelText(TTEXT_GESTURE_DOWN));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_GUESTURE_DOWN));
            break;

            case LIST_GESTURE_RIGHT:
                cell1->setText(getLabelText(TTEXT_GESTURE_RIGHT));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_GUESTURE_RIGHT));
            break;

            case LIST_GESTURE_LEFT:
                cell1->setText(getLabelText(TTEXT_GESTURE_LEFT));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_GUESTURE_LEFT));
            break;

            case LIST_GESTURE_DBL_TAP:
                cell1->setText(getLabelText(TTEXT_GESTURE_DBL_TAP));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_GUESTURE_DBLTAP));
            break;

            case LIST_GESTURE_2FINGER_UP:
                cell1->setText(getLabelText(TTEXT_GESTURE_2FINGER_UP));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_GUESTURE_2FUP));
            break;

            case LIST_GESTURE_2FINGER_DN:
                cell1->setText(getLabelText(TTEXT_GESTURE_2FINGER_DN));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_GUESTURE_2FDN));
            break;

            case LIST_GESTURE_2FINGER_RT:
                cell1->setText(getLabelText(TTEXT_GESTURE_2FINGER_RT));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_GUESTURE_2FRT));
            break;

            case LIST_GESTURE_2FINGER_LT:
                cell1->setText(getLabelText(TTEXT_GESTURE_2FINGER_LT));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_GUESTURE_2FLT));
            break;

            case LIST_ITEM_SELECTED:
                cell1->setText(getLabelText(TTEXT_ITEM_SELECTED));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_ITEM_SELECTED));
            break;

            case LIST_SCROLLBAR_BEGIN:
                cell1->setText(getLabelText(TTEXT_SCROLLBAR_BEGIN));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_SCROLLBAR_BEGIN));
            break;

            case LIST_SCROLLBAR_END:
                cell1->setText(getLabelText(TTEXT_SCROLLBAR_END));
                mTable->setCellWidget(i, 1, makeEvent(ObjHandler::EVENT_SCROLLBAR_END));
            break;
        }

        cell1->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);
        mTable->setItem(i, 0, cell1);

        if (cell2)
        {
            cell2->setData(Qt::UserRole, i);
            mTable->setItem(i, 1, cell2);
        }
    }

    mTable->resizeColumnsToContents();
    mInitialized = true;
}

TElementEvent *TPropertiesEvents::makeEvent(ObjHandler::BUTTON_EVENT_t type)
{
    DECL_TRACER("TPropertiesEvents::makeEvent(ObjHandler::BUTTON_EVENT_t type)");

    TElementEvent *ev = new TElementEvent(type, "Event", -1, mTable);
    ev->setFuncs(collectEvents());

    return ev;
}

/**
 * @brief TPropertiesEvents::collectEvents
 *
 * Goes through all possible events on a page and collects this events into the
 * structure for object events. This simplifies the handling with the dialog
 * box.
 *
 * The method puts all events into the @struct mPageEvents. It is used in case
 * the events of a page should be managed.
 */
QList<ObjHandler::PUSH_FUNC_T> TPropertiesEvents::collectEvents()
{
    DECL_TRACER("TPropertiesEvents::collectEvents()");

    if (!mPage || mPage->pageID <= 0)
        return QList<ObjHandler::PUSH_FUNC_T>();

    if (mObjectIndex >= 0 || mObjectIndex < mPage->objects.size())
    {
        mObject = mPage->objects[mObjectIndex]->getObject();
        mPageEvents = mObject.pushFunc;
        return mObject.pushFunc;
    }

    mPageEvents.clear();

    QList<ObjHandler::BUTTON_EVENT_t> types = {
        ObjHandler::EVENT_PRESS, ObjHandler::EVENT_RELEASE,
        ObjHandler::EVENT_SHOW, ObjHandler::EVENT_HIDE,
        ObjHandler::EVENT_GUESTURE_ANY, ObjHandler::EVENT_GUESTURE_UP,
        ObjHandler::EVENT_GUESTURE_DOWN, ObjHandler::EVENT_GUESTURE_LEFT,
        ObjHandler::EVENT_GUESTURE_RIGHT, ObjHandler::EVENT_GUESTURE_DBLTAP,
        ObjHandler::EVENT_GUESTURE_2FUP, ObjHandler::EVENT_GUESTURE_2FDN,
        ObjHandler::EVENT_GUESTURE_2FLT, ObjHandler::EVENT_GUESTURE_2FRT,
        ObjHandler::EVENT_ITEM_SELECTED, ObjHandler::EVENT_SCROLLBAR_BEGIN,
        ObjHandler::EVENT_SCROLLBAR_END
    };

    for (ObjHandler::BUTTON_EVENT_t ev : types)
    {
        QList<Page::EVENT_t> events;

        switch (ev)
        {
            case ObjHandler::EVENT_SHOW:            events = mPage->eventShow; break;
            case ObjHandler::EVENT_HIDE:            events = mPage->eventHide; break;
            case ObjHandler::EVENT_GUESTURE_ANY:    events = mPage->guestureAny; break;
            case ObjHandler::EVENT_GUESTURE_UP:     events = mPage->guestureUp; break;
            case ObjHandler::EVENT_GUESTURE_DOWN:   events = mPage->guestureDown; break;
            case ObjHandler::EVENT_GUESTURE_LEFT:   events = mPage->guestureLeft; break;
            case ObjHandler::EVENT_GUESTURE_RIGHT:  events = mPage->guestureRight; break;
            case ObjHandler::EVENT_GUESTURE_DBLTAP: events = mPage->guestureDblTab; break;
            case ObjHandler::EVENT_GUESTURE_2FUP:   events = mPage->guesture2FUp; break;
            case ObjHandler::EVENT_GUESTURE_2FDN:   events = mPage->guesture2FDn; break;
            case ObjHandler::EVENT_GUESTURE_2FLT:   events = mPage->guesture2FLt; break;
            case ObjHandler::EVENT_GUESTURE_2FRT:   events = mPage->guesture2Frt; break;

            default:
                continue;
        }

        QList<Page::EVENT_t>::Iterator iter;

        for (iter = events.begin(); iter != events.end(); ++iter)
        {
            ObjHandler::PUSH_FUNC_T pf;
            assignEvent(*iter, ev, &pf);
            mPageEvents.append(pf);
        }
    }

    return mPageEvents;
}

void TPropertiesEvents::assignEvent(Page::EVENT_t& pEvent, ObjHandler::BUTTON_EVENT_t ev, ObjHandler::PUSH_FUNC_T *pf)
{
    DECL_TRACER("TPropertiesEvents::assignEvent(Page::EVENT_t& pEvent, ObjHandler::BUTTON_EVENT_t ev, ObjHandler::PUSH_FUNC_T *pf)");

    pf->event = ev;
    pf->item = pEvent.item;

    switch(pEvent.evCommand)
    {
        case Page::EV_CMD_SHOW:         pf->pfType = "sShow"; break;
        case Page::EV_CMD_HIDE:         pf->pfType = "sHide"; break;
        case Page::EV_CMD_TOGGLE:       pf->pfType = "sToggle"; break;
        case Page::EV_CMD_PAGE:         pf->pfType = "scPage"; break;
        case Page::EV_CMD_PANEL:        pf->pfType = "scPanel"; break;
        case Page::EV_CMD_CLOSE_ALL:    pf->pfType = "close_all"; break;

        default:
        break;
    }


//    pf->pfAction = pEvent.evAction;
    pf->pfName = pEvent.content;
    pf->ID = pEvent.ID;
    pf->action = pEvent.evAction;
    pf->port = pEvent.port;
    pf->key = pEvent.key;
    pf->name = pEvent.name;
    pf->flag = pEvent.flag;
    pf->value1 = pEvent.value1;
    pf->value2 = pEvent.value2;
    pf->value3 = pEvent.value3;
    pf->text = pEvent.text;
    pf->encode = pEvent.encode;
}
// Callbacks

void TPropertiesEvents::onCellActivated(int row, int column)
{

}
