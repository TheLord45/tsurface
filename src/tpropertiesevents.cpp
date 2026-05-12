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
#include <QHeaderView>

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
    {
        if (mTable && mConnected)
            disconnect(mTable, &QTableWidget::cellActivated, this, &TPropertiesEvents::onCellActivated);

        mConnected = false;
        mTable = table;
    }
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

    // Create and handle the table
    setTable();
}

void TPropertiesEvents::setObjectIndex(int index)
{
    DECL_TRACER("TPropertiesEvents::setObjectIndex(int index)");

    if (!mPage || index < 0 || index >= mPage->objects.size())
        return;

    mObjectIndex = index;
    mObject = mPage->objects[index]->getObject();
    setTable();
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

void TPropertiesEvents::update()
{
    DECL_TRACER("TPropertiesEvents::update()");

    setTable();
}

void TPropertiesEvents::doConnect(QTableWidget *table)
{
    DECL_TRACER("TPropertiesEvents::doConnect(QTableWidget *table)");

    if ((table && table != mTable) || (table && !mConnected))
    {
        table->verticalHeader()->setVisible(false);
        table->horizontalHeader()->setVisible(false);
        table->setShowGrid(true);
        table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        table->setColumnCount(2);
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

void TPropertiesEvents::setTable(bool force)
{
    DECL_TRACER("TPropertiesEvents::setTable(bool force)");

    if (!mPage)
    {
        MSG_ERROR("The properties for events have no page set!");
        return;
    }

    if (!mInitialized || !mTable || force)
        createTable();

    if (!mTable)
        return;

    // Hide all rows
    for (int i = 0; i < mTable->rowCount(); ++i)
        mTable->setRowHidden(i, true);

    collectEvents();
    bool haveEvents = false;
    QList<ObjHandler::PUSH_FUNC_T> list;
    // Enable only the rows needed for the actual selection
    switch(mPage->popupType)
    {
        case Page::PT_PAGE:
            if (mObjectIndex < 0 || mObjectIndex >= mPage->objects.size())
            {
                mTable->setRowHidden(LIST_SHOW_PAGE, false);
                mTable->setRowHidden(LIST_HIDE_PAGE, false);

                assignAllEvent(mPage->eventShow, ObjHandler::EVENT_SHOW, &list);
                setCellWidget(LIST_SHOW_PAGE, 1, filterEventType(ObjHandler::EVENT_SHOW));

                assignAllEvent(mPage->eventHide, ObjHandler::EVENT_HIDE, &list);
                setCellWidget(LIST_HIDE_PAGE, 1, filterEventType(ObjHandler::EVENT_HIDE));

                assignAllEvent(mPage->guestureAny, ObjHandler::EVENT_GUESTURE_ANY, &list);
                setCellWidget(LIST_GESTURE_ANY, 1, filterEventType(ObjHandler::EVENT_GUESTURE_ANY));
                assignAllEvent(mPage->guestureUp, ObjHandler::EVENT_GUESTURE_UP, &list);
                setCellWidget(LIST_GESTURE_UP, 1, filterEventType(ObjHandler::EVENT_GUESTURE_UP));
                assignAllEvent(mPage->guestureDown, ObjHandler::EVENT_GUESTURE_DOWN, &list);
                setCellWidget(LIST_GESTURE_DOWN, 1, filterEventType(ObjHandler::EVENT_GUESTURE_DOWN));
                assignAllEvent(mPage->guestureLeft, ObjHandler::EVENT_GUESTURE_LEFT, &list);
                setCellWidget(LIST_GESTURE_LEFT, 1, filterEventType(ObjHandler::EVENT_GUESTURE_LEFT));
                assignAllEvent(mPage->guestureRight, ObjHandler::EVENT_GUESTURE_RIGHT, &list);
                setCellWidget(LIST_GESTURE_RIGHT, 1, filterEventType(ObjHandler::EVENT_GUESTURE_RIGHT));
                assignAllEvent(mPage->guestureDblTab, ObjHandler::EVENT_GUESTURE_DBLTAP, &list);
                setCellWidget(LIST_GESTURE_DBL_TAP, 1, filterEventType(ObjHandler::EVENT_GUESTURE_RIGHT));
                assignAllEvent(mPage->guesture2FUp, ObjHandler::EVENT_GUESTURE_2FUP, &list);
                setCellWidget(LIST_GESTURE_2FINGER_UP, 1, filterEventType(ObjHandler::EVENT_GUESTURE_2FUP));
                assignAllEvent(mPage->guesture2FDn, ObjHandler::EVENT_GUESTURE_2FDN, &list);
                setCellWidget(LIST_GESTURE_2FINGER_DN, 1, filterEventType(ObjHandler::EVENT_GUESTURE_2FDN));
                assignAllEvent(mPage->guesture2FLt, ObjHandler::EVENT_GUESTURE_2FLT, &list);
                setCellWidget(LIST_GESTURE_2FINGER_LT, 1, filterEventType(ObjHandler::EVENT_GUESTURE_2FLT));
                assignAllEvent(mPage->guesture2Frt, ObjHandler::EVENT_GUESTURE_2FRT, &list);
                setCellWidget(LIST_GESTURE_2FINGER_RT, 1, filterEventType(ObjHandler::EVENT_GUESTURE_2FRT));

                haveEvents = true;
            }
            else if (mObject.type == ObjHandler::GENERAL)
            {
                mTable->setRowHidden(LIST_BUTTON_PRESS, false);
                mTable->setRowHidden(LIST_BUTTON_RELEASE, false);

                setCellWidget(LIST_BUTTON_PRESS, 1, filterEventType(ObjHandler::EVENT_PRESS));
                setCellWidget(LIST_BUTTON_RELEASE, 1, filterEventType(ObjHandler::EVENT_RELEASE));
                setCellWidget(LIST_GESTURE_ANY, 1, filterEventType(ObjHandler::EVENT_GUESTURE_ANY));
                setCellWidget(LIST_GESTURE_UP, 1, filterEventType(ObjHandler::EVENT_GUESTURE_UP));
                setCellWidget(LIST_GESTURE_2FINGER_DN, 1, filterEventType(ObjHandler::EVENT_GUESTURE_DOWN));
                setCellWidget(LIST_GESTURE_LEFT, 1, filterEventType(ObjHandler::EVENT_GUESTURE_LEFT));
                setCellWidget(LIST_GESTURE_RIGHT, 1, filterEventType(ObjHandler::EVENT_GUESTURE_RIGHT));
                setCellWidget(LIST_GESTURE_DBL_TAP, 1, filterEventType(ObjHandler::EVENT_GUESTURE_DBLTAP));
                setCellWidget(LIST_GESTURE_2FINGER_UP, 1, filterEventType(ObjHandler::EVENT_GUESTURE_2FUP));
                setCellWidget(LIST_GESTURE_2FINGER_DN, 1, filterEventType(ObjHandler::EVENT_GUESTURE_2FDN));
                setCellWidget(LIST_GESTURE_2FINGER_LT, 1, filterEventType(ObjHandler::EVENT_GUESTURE_2FLT));
                setCellWidget(LIST_GESTURE_2FINGER_RT, 1, filterEventType(ObjHandler::EVENT_GUESTURE_2FRT));
                haveEvents = true;
            }

            if (haveEvents)
                enableCommon();
        break;

        default:
            if (mObjectIndex >= 0 && mObjectIndex < mPage->objects.size())
            {
                if (mObject.type == ObjHandler::GENERAL)
                {
                    mTable->setRowHidden(LIST_BUTTON_PRESS, false);
                    mTable->setRowHidden(LIST_BUTTON_RELEASE, false);

                    setCellWidget(LIST_BUTTON_PRESS, 1, filterEventType(ObjHandler::EVENT_PRESS));
                    setCellWidget(LIST_BUTTON_RELEASE, 1, filterEventType(ObjHandler::EVENT_RELEASE));
                    enableCommon();
                }
                else if (mObject.type == ObjHandler::LISTVIEW)
                {
                    mTable->setRowHidden(LIST_ITEM_SELECTED, false);
                    mTable->setRowHidden(LIST_SCROLLBAR_BEGIN, false);
                    mTable->setRowHidden(LIST_SCROLLBAR_END, false);

                    setCellWidget(LIST_ITEM_SELECTED, 1, filterEventType(ObjHandler::EVENT_ITEM_SELECTED));
                    setCellWidget(LIST_SCROLLBAR_BEGIN, 1, filterEventType(ObjHandler::EVENT_SCROLLBAR_BEGIN));
                    setCellWidget(LIST_SCROLLBAR_END, 1, filterEventType(ObjHandler::EVENT_SCROLLBAR_END));
                }
            }
    }
}

void TPropertiesEvents::enableCommon()
{
    DECL_TRACER("TPropertiesEvents::enableCommon()");

    mTable->setRowHidden(LIST_GESTURE_ANY, false);
    mTable->setRowHidden(LIST_GESTURE_UP, false);
    mTable->setRowHidden(LIST_GESTURE_DOWN, false);
    mTable->setRowHidden(LIST_GESTURE_LEFT, false);
    mTable->setRowHidden(LIST_GESTURE_RIGHT, false);
    mTable->setRowHidden(LIST_GESTURE_DBL_TAP, false);
    mTable->setRowHidden(LIST_GESTURE_2FINGER_UP, false);
    mTable->setRowHidden(LIST_GESTURE_2FINGER_DN, false);
    mTable->setRowHidden(LIST_GESTURE_2FINGER_LT, false);
    mTable->setRowHidden(LIST_GESTURE_2FINGER_RT, false);
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

void TPropertiesEvents::setCellWidget(int row, int col, const QList<ObjHandler::PUSH_FUNC_T>& pf)
{
    DECL_TRACER("TPropertiesEvents::setCellWidget(int row, int col, const QList<ObjHandler::PUSH_FUNC_T>& pf)");

    if (!mTable || !mPage)
        return;

    QWidget *w = mTable->cellWidget(row, col);

    if (!w)
        return;

    QSignalBlocker sigBlock(w);

    TElementEvent *p = static_cast<TElementEvent *>(w);
    p->setFuncs(pf);
}

TElementEvent *TPropertiesEvents::makeEvent(ObjHandler::BUTTON_EVENT_t type)
{
    DECL_TRACER("TPropertiesEvents::makeEvent(ObjHandler::BUTTON_EVENT_t type)");

    TElementEvent *ev = new TElementEvent(type, "Event", -1, mTable);
    ev->setFuncs(collectEvents());
    connect(ev, &TElementEvent::eventChanged, this, &TPropertiesEvents::onEventsChanged);
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

    if (mObjectIndex >= 0 && mObjectIndex < mPage->objects.size())
    {
        mObject = mPage->objects[mObjectIndex]->getObject();
        mPageEvents = mObject.pushFunc;
        MSG_DEBUG("Object " << mObject.bi << " has " << mPageEvents.size() << " push functions.");
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

    MSG_DEBUG("Page " << mPage->pageID << " has " << mPageEvents.size() << " push functions.");
    return mPageEvents;
}

void TPropertiesEvents::assignAllEvent(const QList<Page::EVENT_t>& pEvent, ObjHandler::BUTTON_EVENT_t ev, QList<ObjHandler::PUSH_FUNC_T> *pf)
{
    DECL_TRACER("TPropertiesEvents::assignAllEvent(const QList<Page::EVENT_t>& pEvent, ObjHandler::BUTTON_EVENT_t ev, QList<ObjHandler::PUSH_FUNC_T> *pf)");

    if (!pf)
        return;

    pf->clear();
    QList<Page::EVENT_t>::ConstIterator iter;

    for (iter = pEvent.constBegin(); iter != pEvent.constEnd(); ++iter)
    {
        ObjHandler::PUSH_FUNC_T f;
        assignEvent(*iter, ev, &f);
        pf->append(f);
    }
}

void TPropertiesEvents::assignEvent(const Page::EVENT_t& pEvent, ObjHandler::BUTTON_EVENT_t ev, ObjHandler::PUSH_FUNC_T *pf)
{
    DECL_TRACER("TPropertiesEvents::assignEvent(Page::EVENT_t& pEvent, ObjHandler::BUTTON_EVENT_t ev, ObjHandler::PUSH_FUNC_T *pf)");

    if (!pf)
        return;

    pf->event = ev;
    pf->item = pEvent.item;
    pf->action = pEvent.evAction;
    pf->pfName = pEvent.content;

    switch(pEvent.evCommand)
    {
        case Page::EV_CMD_STANDARD:     pf->pfType = "sStan"; break;
        case Page::EV_CMD_PREVIOUS:     pf->pfType = "sPrev"; break;
        case Page::EV_CMD_SHOW:         pf->pfType = "sShow"; break;
        case Page::EV_CMD_HIDE:         pf->pfType = "sHide"; break;
        case Page::EV_CMD_TOGGLE:       pf->pfType = "sToggle"; break;
        case Page::EV_CMD_GROUP:        pf->pfType = "ClearG"; break;
        case Page::EV_CMD_PAGE:         pf->pfType = "scPage"; break;
        case Page::EV_CMD_PANEL:        pf->pfType = "scPanel"; break;

        default:
        break;
    }

    switch(pEvent.evAction)
    {
        case ObjHandler::BT_ACTION_PGFLIP:  pf->pfAction = "pgFlip"; break;
        case ObjHandler::BT_ACTION_LAUNCH:  pf->pfAction = "launch"; break;
        case ObjHandler::BT_ACTION_COMMAND: pf->pfAction = "command"; break;
        case ObjHandler::BT_ACTION_STRING:  pf->pfAction = "string"; break;
        case ObjHandler::BT_ACTION_CUSTOM:  pf->pfAction = "custom"; break;
    }

    pf->port = pEvent.port;
    pf->ID = pEvent.ID;
    pf->key = pEvent.key;
    pf->type = pEvent.type;
    pf->name = pEvent.name;
    pf->flag = pEvent.flag;
    pf->value1 = pEvent.value1;
    pf->value2 = pEvent.value2;
    pf->value3 = pEvent.value3;
    pf->text = pEvent.text;
    pf->encode = pEvent.encode;
}

Page::EVENT_t TPropertiesEvents::eventFromPushFunc(const ObjHandler::PUSH_FUNC_T& pf)
{
    DECL_TRACER("TPropertiesEvents::eventFromPushFunc(const ObjHandler::PUSH_FUNC_T& pf)");

    Page::EVENT_t ev;
    ev.item = pf.item;

    if (pf.pfType == "sShow")
        ev.evCommand = Page::EV_CMD_SHOW;
    else if (pf.pfType == "sHide")
        ev.evCommand = Page::EV_CMD_HIDE;
    else if (pf.pfType == "sToggle")
        ev.evCommand = Page::EV_CMD_TOGGLE;
    else if (pf.pfType == "scPage")
        ev.evCommand = Page::EV_CMD_PAGE;
    else if (pf.pfType == "scPanel")
        ev.evCommand = Page::EV_CMD_PANEL;

    ev.content = pf.pfName;
    ev.ID = pf.ID;
    ev.evAction = pf.action;
    ev.port = pf.port;
    ev.key = pf.key;
    ev.name = pf.name;
    ev.flag = pf.flag;
    ev.value1 = pf.value1;
    ev.value2 = pf.value2;
    ev.value3 = pf.value3;
    ev.text = pf.text;
    ev.encode = pf.encode;

    return ev;
}

QList<ObjHandler::PUSH_FUNC_T> TPropertiesEvents::filterEventType(ObjHandler::BUTTON_EVENT_t ev)
{
    DECL_TRACER("TPropertiesEvents::filterEventType(ObjHandler::BUTTON_EVENT_t ev)");

    if (!mPage)
        return QList<ObjHandler::PUSH_FUNC_T>();

    QList<ObjHandler::PUSH_FUNC_T> list;

    if (mObjectIndex < 0 || mObjectIndex >= mPage->objects.size())  // Is it a page?
    {
        switch(ev)
        {
            case ObjHandler::EVENT_SHOW:            assignAllEvent(mPage->eventShow, ev, &list); break;
            case ObjHandler::EVENT_HIDE:            assignAllEvent(mPage->eventHide, ev, &list); break;
            case ObjHandler::EVENT_GUESTURE_ANY:    assignAllEvent(mPage->guestureAny, ev, &list); break;
            case ObjHandler::EVENT_GUESTURE_UP:     assignAllEvent(mPage->guestureUp, ev, &list); break;
            case ObjHandler::EVENT_GUESTURE_DOWN:   assignAllEvent(mPage->guestureDown, ev, &list); break;
            case ObjHandler::EVENT_GUESTURE_LEFT:   assignAllEvent(mPage->guestureLeft, ev, &list); break;
            case ObjHandler::EVENT_GUESTURE_RIGHT:  assignAllEvent(mPage->guestureRight, ev, &list); break;
            case ObjHandler::EVENT_GUESTURE_DBLTAP: assignAllEvent(mPage->guestureDblTab, ev, &list); break;
            case ObjHandler::EVENT_GUESTURE_2FUP:   assignAllEvent(mPage->guesture2FUp, ev, &list); break;
            case ObjHandler::EVENT_GUESTURE_2FDN:   assignAllEvent(mPage->guesture2FDn, ev, &list); break;
            case ObjHandler::EVENT_GUESTURE_2FLT:   assignAllEvent(mPage->guesture2FLt, ev, &list); break;
            case ObjHandler::EVENT_GUESTURE_2FRT:   assignAllEvent(mPage->guesture2Frt, ev, &list); break;

            default:
            break;
        }

        return list;
    }

    QList<ObjHandler::PUSH_FUNC_T>::Iterator iter;

    for (iter = mObject.pushFunc.begin(); iter != mObject.pushFunc.end(); ++iter)
    {
        if (iter->event == ev)
            list.append(*iter);
    }

    return list;
}

// Callbacks

void TPropertiesEvents::onCellActivated(int row, int column)
{
    DECL_TRACER("TPropertiesEvents::onCellActivated(int row, int column)");

    Q_UNUSED(column);

    if (!mInitialized)
        return;

    QWidget *w = mTable->cellWidget(row, 1);

    if (!w)
        return;

    w->setFocus(Qt::OtherFocusReason);
}

void TPropertiesEvents::onEventsChanged(const QList<ObjHandler::PUSH_FUNC_T>& funcs, ObjHandler::BUTTON_EVENT_t evt, const QString& name, int instance)
{
    DECL_TRACER("TPropertiesEvents::onEventsChanged(const QList<ObjHandler::PUSH_FUNC_T>& funcs, ObjHandler::BUTTON_EVENT_t evt, const QString& name, int instance)");

    Q_UNUSED(name);
    Q_UNUSED(instance);

    MSG_DEBUG("Having " << funcs.size() << " commands.");

    if (mObjectIndex < 0 || mObjectIndex >= mPage->objects.size())  // Do we act on a page?
    {                                                               // Probably yes
        if (mPage->popupType != Page::PT_PAGE)                      // Is it realy a page?
        {                                                           // No, then return
            MSG_WARNING("Changes should be for a page, but are for an undefined object!");
            return;
        }

        MSG_DEBUG("Setting commands of type " << evt << " for a page.");
        // For the page we must distinguish between the types of events
        QList<ObjHandler::PUSH_FUNC_T>::ConstIterator iter;

        switch(evt)
        {
            case ObjHandler::EVENT_SHOW:            mPage->eventShow.clear(); break;
            case ObjHandler::EVENT_HIDE:            mPage->eventHide.clear(); break;
            case ObjHandler::EVENT_GUESTURE_ANY:    mPage->guestureAny.clear(); break;
            case ObjHandler::EVENT_GUESTURE_UP:     mPage->guestureUp.clear(); break;
            case ObjHandler::EVENT_GUESTURE_DOWN:   mPage->guestureDown.clear(); break;
            case ObjHandler::EVENT_GUESTURE_LEFT:   mPage->guestureLeft.clear(); break;
            case ObjHandler::EVENT_GUESTURE_RIGHT:  mPage->guestureRight.clear(); break;
            case ObjHandler::EVENT_GUESTURE_DBLTAP: mPage->guestureDblTab.clear(); break;
            case ObjHandler::EVENT_GUESTURE_2FUP:   mPage->guesture2FUp.clear(); break;
            case ObjHandler::EVENT_GUESTURE_2FDN:   mPage->guesture2FDn.clear(); break;
            case ObjHandler::EVENT_GUESTURE_2FLT:   mPage->guesture2FLt.clear(); break;
            case ObjHandler::EVENT_GUESTURE_2FRT:   mPage->guesture2Frt.clear(); break;

            default:
                return;
        }

        for (iter = funcs.constBegin(); iter != funcs.constEnd(); ++iter)
        {
            switch(evt)
            {
                case ObjHandler::EVENT_SHOW:            mPage->eventShow.append(eventFromPushFunc(*iter)); break;
                case ObjHandler::EVENT_HIDE:            mPage->eventHide.append(eventFromPushFunc(*iter)); break;
                case ObjHandler::EVENT_GUESTURE_ANY:    mPage->guestureAny.append(eventFromPushFunc(*iter)); break;
                case ObjHandler::EVENT_GUESTURE_UP:     mPage->guestureUp.append(eventFromPushFunc(*iter)); break;
                case ObjHandler::EVENT_GUESTURE_DOWN:   mPage->guestureDown.append(eventFromPushFunc(*iter)); break;
                case ObjHandler::EVENT_GUESTURE_LEFT:   mPage->guestureLeft.append(eventFromPushFunc(*iter)); break;
                case ObjHandler::EVENT_GUESTURE_RIGHT:  mPage->guestureRight.append(eventFromPushFunc(*iter)); break;
                case ObjHandler::EVENT_GUESTURE_DBLTAP: mPage->guestureDblTab.append(eventFromPushFunc(*iter)); break;
                case ObjHandler::EVENT_GUESTURE_2FUP:   mPage->guesture2FUp.append(eventFromPushFunc(*iter)); break;
                case ObjHandler::EVENT_GUESTURE_2FDN:   mPage->guesture2FDn.append(eventFromPushFunc(*iter)); break;
                case ObjHandler::EVENT_GUESTURE_2FLT:   mPage->guesture2FLt.append(eventFromPushFunc(*iter)); break;
                case ObjHandler::EVENT_GUESTURE_2FRT:   mPage->guesture2Frt.append(eventFromPushFunc(*iter)); break;

                default:
                    continue;
            }
        }

        markChanged(TBL_EVENTS);
        mChanged = true;
        return;
    }

    MSG_DEBUG("Setting the commands for an object.");
    // First remove all events of a particular type
    QList<ObjHandler::PUSH_FUNC_T>::Iterator iter;
    bool found = true;

    while(found)
    {
        found = false;

        for (iter = mObject.pushFunc.begin(); iter != mObject.pushFunc.end(); ++iter)
        {
            if (iter->event == evt)
            {
                mObject.pushFunc.erase(iter);
                found = true;
                break;
            }
        }
    }
    // Now add the events
    QList<ObjHandler::PUSH_FUNC_T>::ConstIterator cIter;

    for (cIter = funcs.constBegin(); cIter != funcs.constEnd(); ++cIter)
        mObject.pushFunc.append(*cIter);
    // Renumber the items
    int item = 0;

    for (iter = mObject.pushFunc.begin(); iter != mObject.pushFunc.end(); ++iter)
    {
        iter->item = item;
        item++;
    }
    // Finally save the object
    mPage->objects[mObjectIndex]->setObject(mObject);
    markChanged(TBL_EVENTS);
    mChanged = true;
}
