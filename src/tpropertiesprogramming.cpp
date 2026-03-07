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
#include <QStandardItemModel>
#include <QComboBox>
#include <QSpinBox>
#include <QIntValidator>

#include "tpropertiesprogramming.h"
#include "tvalidateport.h"
#include "telementwidgetcombo.h"
#include "telementspinbox.h"
#include "terror.h"

#define TTEXT_FEEDBACK                  0
#define TTEXT_ADDRESS_PORT              1
#define TTEXT_ADDRESS_CODE              2
#define TTEXT_CHANNEL_PORT              3
#define TTEXT_CHANNEL_CODE              4
#define TTEXT_LEVEL_CONTROL_TYPE        5
#define TTEXT_LEVEL_PORT                6
#define TTEXT_LEVEL_CODE                7
#define TTEXT_LEVEL_CONTROL_VALUE       8
#define TTEXT_LEVEL_CONTROL_REPEAT_TIME 9
#define TTEXT_LEVEL_FUNCTION            10
#define TTEXT_RANGE_LOW                 11
#define TTEXT_RANGE_HIGH                12
#define TTEXT_RANGE_DRAG_INCREMENT      13
#define TTEXT_RANGE_INVERTED            14
#define TTEXT_RANGE_TIME_UP             15
#define TTEXT_RANGE_TIME_DOWN           16

const QStringList gMessages = {
    "Feedback",                         //  0
    "Address Port",                     //  1
    "Address Code",                     //  2
    "Channel Port",                     //  3
    "Channel Code",                     //  4
    "Level Control Type",               //  5
    "Level Port",                       //  6
    "Level Code",                       //  7
    "Level Control Value",              //  8
    "Level Control Repeat Time",        //  9
    "Level Function",                   // 10
    "Range Low",                        // 11
    "Range High",                       // 12
    "Range Drag Increment",             // 13
    "Range Inverted",                   // 14
    "Range Time Up",                    // 15
    "Range Time Down"                   // 16
};

TPropertiesProgramming::TPropertiesProgramming()
{
    DECL_TRACER("TPropertiesProgramming::TPropertiesProgramming()");

    mValidatePort = new TValidatePort(this);
    mIntValidator = new QIntValidator(this);
}

TPropertiesProgramming::TPropertiesProgramming(QTableWidget *table)
    : mTable(table)
{
    DECL_TRACER("TPropertiesProgramming::TPropertiesProgramming(QTableWidget *table)");

    mValidatePort = new TValidatePort(this);
    mIntValidator = new QIntValidator(this);
}

TPropertiesProgramming::~TPropertiesProgramming()
{
    DECL_TRACER("TPropertiesProgramming::~TPropertiesProgramming()");

    delete mValidatePort;
    delete mIntValidator;
}

void TPropertiesProgramming::setPage(const Page::PAGE_t& page)
{
    DECL_TRACER("TPropertiesProgramming::setPage(const Page::PAGE_t& page)");

    if (page.pageID == mPage.pageID)
        return;

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_PROGRAM);

    mChanged = false;
    mPage = Page::PAGE_t();
    mPage = page;
    mStype = page.popupType == Page::PT_PAGE ? STATE_PAGE : STATE_POPUP;
    setTable();
}

void TPropertiesProgramming::setProgrammingPage(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::setProgrammingPage(const QString& name)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    Page::PAGE_t page = TPageHandler::Current().getPage(name);

    if (page.pageID == mPage.pageID)
        return;

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_PROGRAM);

    mChanged = false;
    mPage = Page::PAGE_t();
    mPage = page;
    setProgrammingPage(mPage.pageID, true);
}

void TPropertiesProgramming::setProgrammingPage(int id, bool loaded)
{
    DECL_TRACER("TPropertiesProgramming::setProgrammingPage(int id, bool loaded)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    Page::PAGE_t page;
    bool equal = false;

    if (!loaded)
    {
        page = *TPageHandler::Current().getPage(id);

        if (page.pageID == mPage.pageID)
            equal = true;
    }

    if (!loaded && !equal && mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_PROGRAM);

    mChanged = false;

    if (page.pageID > 0)
        mPage = page;

    if (mPage.pageID <= 0)
        return;

    mStype = STATE_PAGE;
    setTable();
}

void TPropertiesProgramming::setProgrammingPopup(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::setProgrammingPopup(const QString& name)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    bool equal = false;
    Page::PAGE_t page = TPageHandler::Current().getPage(name);

    if (page.pageID == mPage.pageID)
        equal = true;

    if (!equal && mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_PROGRAM);

    mChanged = false;
    mPage = page;
    setProgrammingPopup(mPage.pageID, true);
}

void TPropertiesProgramming::setProgrammingPopup(int id, bool loaded)
{
    DECL_TRACER("TPropertiesProgramming::setProgrammingPopup(int id, bool loaded)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    Page::PAGE_t page;
    bool equal = false;

    if (!loaded)
    {
        page = *TPageHandler::Current().getPage(id);

        if (page.pageID == mPage.pageID)
            equal = true;
    }

    if (!loaded && !equal && mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_PROGRAM);

    mChanged = false;

    if (page.pageID > 0)
        mPage = page;

    if (mPage.pageID <= 0)
        return;

    mStype = STATE_POPUP;
    setTable();
}

void TPropertiesProgramming::setObjectID(int id)
{
    DECL_TRACER("TPropertiesProgramming::setObjectID(int id)");

    if (id < 0 || id >= mPage.objects.size())
    {
        MSG_WARNING("Invalid object ID: " << id << ", Number objects: " << mPage.objects.size());
        return;
    }

    MSG_DEBUG("Old ID: " << mActObjectID << ", new ID: " << id);

    if (mActObjectID != id)
    {
        if (mChanged)
            mPage.objects[mActObjectID]->setObject(mActObject);

        mActObjectID = id;
        mActObject = mPage.objects[id]->getObject();
        mChanged = false;
    }

    setSType();
}

void TPropertiesProgramming::setObjectType(ObjHandler::BUTTONTYPE btype, int index)
{
    DECL_TRACER("TPropertiesProgramming::setObjectType(ObjHandler::BUTTONTYPE btype, int index)");

    if (index < 0 || index >= mPage.objects.size())
        return;

    if (index != mActObjectID)
    {
        MSG_WARNING("Type of a not loaded object changed! Loaded ID: " << mActObjectID << ". Object type to change: " << index);
        mPage.objects[index]->setObjectType(btype);
        return;
    }

    mActObject.type = btype;
    setSType();
    requestRedraw(&mPage);
    mChanged = true;
}

void TPropertiesProgramming::setObject(ObjHandler::TOBJECT_t& object, int id)
{
    DECL_TRACER("TPropertiesProgramming::setObject(ObjHandler::TOBJECT_t& object, int id)");

    MSG_DEBUG("Changed: " << (mChanged ? "YES" : "NO") << ", BI: " << object.bi << ", new ID: " << id << ", old ID: " << mActObjectID);

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size() && mActObjectID != id)
    {
        if (mChanged)
        {
            mPage.objects[mActObjectID]->setObject(mActObject);
            saveChangedData(&mPage, TBL_PROGRAM);
        }

        mActObject = object;
        mActObjectID = id;
        mChanged = false;
    }

    setSType();
}

void TPropertiesProgramming::setSType()
{
    DECL_TRACER("TPropertiesProgramming::setSType()");

    if (mActObject.bi <= 0 || mActObjectID < 0 || mActObjectID >= mPage.objects.size())
        return;

    switch(mActObject.type)
    {
        case ObjHandler::GENERAL:
        case ObjHandler::MULTISTATE_GENERAL:
            mStype = STATE_BUTTON;
        break;

        case ObjHandler::BARGRAPH:
        case ObjHandler::MULTISTATE_BARGRAPH:
            mStype = STATE_BARGRAPH;
        break;

        case ObjHandler::TEXT_INPUT:
            mStype = STATE_INPUT;
        break;

        case ObjHandler::LISTVIEW:
            mStype = STATE_LISTVIEW;
        break;

        case ObjHandler::SUBPAGE_VIEW:
            mStype = STATE_SUBPAGE;
        break;

        default:
            mStype = STATE_UNKNOWN;
    }

    setTable();
}

void TPropertiesProgramming::setTableWidget(int row, int col, const QVariant& data, ELEMENT_TYPE_t etype)
{
    DECL_TRACER("TPropertiesProgramming::setTableWidget(int row, int col, const QVariant& data, ELEMENT_TYPE_t etype)");

    if (!mTable)
        return;

    QWidget *w = mTable->cellWidget(row, col);

    if (!w)
        return;

    switch(etype)
    {
        case W_COMBO:
        {
            TElementWidgetCombo *p = static_cast<TElementWidgetCombo *>(w);
            p->selectItem(data);
        }
        break;

        case W_SPINBOX:
        {
            TElementSpinBox *p = static_cast<TElementSpinBox *>(w);
            p->setValue(data.toInt());
        }
        break;

        default:
            return;
    }
}

void TPropertiesProgramming::clear()
{
    DECL_TRACER("TPropertiesProgramming::clear()");

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_PROGRAM);

    mChanged = false;
    mPage = Page::PAGE_t();
    mTable->clear();
    mTable->setRowCount(0);
    mTable->setColumnCount(0);
}

void TPropertiesProgramming::setTable(STATE_TYPE stype)
{
    DECL_TRACER("TPropertiesProgramming::setTable()");

    if (stype != STATE_UNKNOWN)
        mStype = stype;

    if (!mInitialized)
        createPage();

    for (int i = 0; i < mTable->rowCount(); ++i)
        mTable->setRowHidden(i, true);

    MSG_DEBUG("Rows: " << mTable->rowCount() << ", SType: " << mStype);

    if (mStype == STATE_PAGE || mStype == STATE_POPUP || mStype == STATE_INPUT || mStype == STATE_LISTVIEW)
    {
        mTable->setRowHidden(TTEXT_ADDRESS_PORT, false);
        mTable->setRowHidden(TTEXT_ADDRESS_CODE, false);
        mTable->setRowHidden(TTEXT_CHANNEL_PORT, false);
        mTable->setRowHidden(TTEXT_CHANNEL_CODE, false);

        setTableWidget(TTEXT_ADDRESS_PORT, 1, mPage.ap, W_SPINBOX);
        setTableWidget(TTEXT_ADDRESS_CODE, 1, mPage.ad, W_SPINBOX);
        setTableWidget(TTEXT_CHANNEL_PORT, 1, mPage.cp, W_SPINBOX);
        setTableWidget(TTEXT_CHANNEL_CODE, 1, mPage.ch, W_SPINBOX);
    }
    else if (mStype == STATE_SUBPAGE)
    {
        mTable->setRowHidden(TTEXT_ADDRESS_PORT, false);
        mTable->setRowHidden(TTEXT_ADDRESS_CODE, false);

        setTableWidget(TTEXT_ADDRESS_PORT, 1, mPage.ap, W_SPINBOX);
        setTableWidget(TTEXT_ADDRESS_CODE, 1, mPage.ad, W_SPINBOX);
    }
    else if (mStype == STATE_BUTTON)
    {
        mTable->setRowHidden(TTEXT_FEEDBACK, false);
        mTable->setRowHidden(TTEXT_ADDRESS_PORT, false);
        mTable->setRowHidden(TTEXT_ADDRESS_CODE, false);
        mTable->setRowHidden(TTEXT_CHANNEL_PORT, false);
        mTable->setRowHidden(TTEXT_CHANNEL_CODE, false);
        mTable->setRowHidden(TTEXT_LEVEL_CONTROL_TYPE, false);

        setTableWidget(TTEXT_FEEDBACK, 1, mActObject.fb, W_COMBO);
        setTableWidget(TTEXT_ADDRESS_PORT, 1, mActObject.ap, W_SPINBOX);
        setTableWidget(TTEXT_ADDRESS_CODE, 1, mActObject.ad, W_SPINBOX);
        setTableWidget(TTEXT_CHANNEL_PORT, 1, mActObject.cp, W_SPINBOX);
        setTableWidget(TTEXT_CHANNEL_CODE, 1, mActObject.ch, W_SPINBOX);
        setTableWidget(TTEXT_LEVEL_CONTROL_TYPE, 1, mActObject.vt, W_COMBO);

        if (mActObject.vt == "abs")
        {
            mTable->setRowHidden(TTEXT_LEVEL_PORT, false);
            mTable->setRowHidden(TTEXT_LEVEL_CODE, false);
            mTable->setRowHidden(TTEXT_LEVEL_CONTROL_VALUE, false);
            mTable->setRowHidden(TTEXT_RANGE_LOW, false);
            mTable->setRowHidden(TTEXT_RANGE_HIGH, false);
            mTable->setRowHidden(TTEXT_RANGE_TIME_UP, false);
            mTable->setRowHidden(TTEXT_RANGE_TIME_DOWN, false);

            setTableWidget(TTEXT_LEVEL_PORT, 1, mActObject.lp, W_SPINBOX);
            setTableWidget(TTEXT_LEVEL_CODE, 1, mActObject.lv, W_SPINBOX);
            setTableWidget(TTEXT_LEVEL_CONTROL_VALUE, 1, mActObject.va, W_SPINBOX);
            setTableWidget(TTEXT_RANGE_LOW, 1, mActObject.rl, W_SPINBOX);
            setTableWidget(TTEXT_RANGE_HIGH, 1, mActObject.rh, W_SPINBOX);
            setTableWidget(TTEXT_RANGE_TIME_UP, 1, mActObject.ru, W_SPINBOX);
            setTableWidget(TTEXT_RANGE_TIME_DOWN, 1, mActObject.rd, W_SPINBOX);
        }
        else if (mActObject.vt == "rel")
        {
            mTable->setRowHidden(TTEXT_LEVEL_PORT, false);
            mTable->setRowHidden(TTEXT_LEVEL_CODE, false);
            mTable->setRowHidden(TTEXT_LEVEL_CONTROL_VALUE, false);
            mTable->setRowHidden(TTEXT_LEVEL_CONTROL_REPEAT_TIME, false);
            mTable->setRowHidden(TTEXT_RANGE_LOW, false);
            mTable->setRowHidden(TTEXT_RANGE_HIGH, false);
            mTable->setRowHidden(TTEXT_RANGE_TIME_UP, false);
            mTable->setRowHidden(TTEXT_RANGE_TIME_DOWN, false);

            setTableWidget(TTEXT_LEVEL_PORT, 1, mActObject.lp, W_SPINBOX);
            setTableWidget(TTEXT_LEVEL_CODE, 1, mActObject.lv, W_SPINBOX);
            setTableWidget(TTEXT_LEVEL_CONTROL_VALUE, 1, mActObject.va, W_SPINBOX);
            setTableWidget(TTEXT_LEVEL_CONTROL_REPEAT_TIME, 1, mActObject.rv, W_SPINBOX);
            setTableWidget(TTEXT_RANGE_LOW, 1, mActObject.rl, W_SPINBOX);
            setTableWidget(TTEXT_RANGE_HIGH, 1, mActObject.rh, W_SPINBOX);
            setTableWidget(TTEXT_RANGE_TIME_UP, 1, mActObject.ru, W_SPINBOX);
            setTableWidget(TTEXT_RANGE_TIME_DOWN, 1, mActObject.rd, W_SPINBOX);
        }
    }
    else if (mStype == STATE_BARGRAPH)
    {
        mTable->setRowHidden(TTEXT_ADDRESS_PORT, false);
        mTable->setRowHidden(TTEXT_ADDRESS_CODE, false);
        mTable->setRowHidden(TTEXT_CHANNEL_PORT, false);
        mTable->setRowHidden(TTEXT_CHANNEL_CODE, false);
        mTable->setRowHidden(TTEXT_LEVEL_PORT, false);
        mTable->setRowHidden(TTEXT_LEVEL_CODE, false);
        mTable->setRowHidden(TTEXT_LEVEL_FUNCTION, false);
        mTable->setRowHidden(TTEXT_RANGE_LOW, false);
        mTable->setRowHidden(TTEXT_RANGE_HIGH, false);

        setTableWidget(TTEXT_ADDRESS_PORT, 1, mActObject.ap, W_SPINBOX);
        setTableWidget(TTEXT_ADDRESS_CODE, 1, mActObject.ad, W_SPINBOX);
        setTableWidget(TTEXT_CHANNEL_PORT, 1, mActObject.cp, W_SPINBOX);
        setTableWidget(TTEXT_CHANNEL_CODE, 1, mActObject.ch, W_SPINBOX);
        setTableWidget(TTEXT_LEVEL_PORT, 1, mActObject.lp, W_SPINBOX);
        setTableWidget(TTEXT_LEVEL_CODE, 1, mActObject.lv, W_SPINBOX);
        setTableWidget(TTEXT_LEVEL_FUNCTION, 1, mActObject.lf, W_COMBO);
        setTableWidget(TTEXT_RANGE_LOW, 1, mActObject.rl, W_SPINBOX);
        setTableWidget(TTEXT_RANGE_HIGH, 1, mActObject.rh, W_SPINBOX);

        if (mActObject.lf.startsWith("drag"))
        {
            mTable->setRowHidden(TTEXT_RANGE_DRAG_INCREMENT, false);
            setTableWidget(TTEXT_RANGE_DRAG_INCREMENT, 1, mActObject.rn, W_SPINBOX);
        }

        mTable->setRowHidden(TTEXT_RANGE_INVERTED, false);
        setTableWidget(TTEXT_RANGE_INVERTED, 1, mActObject.ri, W_COMBO);

        if (mActObject.lf.startsWith("active"))
        {
            mTable->setRowHidden(TTEXT_RANGE_TIME_UP, false);
            mTable->setRowHidden(TTEXT_RANGE_TIME_DOWN, false);

            setTableWidget(TTEXT_RANGE_LOW, 1, mActObject.lu, W_SPINBOX);
            setTableWidget(TTEXT_RANGE_HIGH, 1, mActObject.ld, W_SPINBOX);
        }
    }
}

void TPropertiesProgramming::createPage()
{
    DECL_TRACER("TPropertiesProgramming::createPage()");

    if (mInitialized)
        return;

    mTable->clear();
    mTable->setColumnCount(2);
    mTable->setRowCount(gMessages.size());

    for (int i = 0; i < gMessages.size(); ++i)
    {
        QTableWidgetItem *cell1 = new QTableWidgetItem;
        cell1->setBackground(Qt::lightGray);
        cell1->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);

        switch(i)
        {
            case TTEXT_FEEDBACK:
                cell1->setText(gMessages[TTEXT_FEEDBACK]);
                mTable->setCellWidget(i, 1, makeObjectFeedback("ObjectFeedback"));
            break;

            case TTEXT_ADDRESS_PORT:
                cell1->setText(gMessages[TTEXT_ADDRESS_PORT]);
                mTable->setCellWidget(i, 1, makeAddressPort("AddressPort"));
            break;

            case TTEXT_ADDRESS_CODE:
                cell1->setText(gMessages[TTEXT_ADDRESS_CODE]);
                mTable->setCellWidget(i, 1, makeAddressCode("AddressCode"));
            break;

            case TTEXT_CHANNEL_PORT:
                cell1->setText(gMessages[TTEXT_CHANNEL_PORT]);
                mTable->setCellWidget(i, 1, makeChannelPort("ChannelPort"));
            break;

            case TTEXT_CHANNEL_CODE:
                cell1->setText(gMessages[TTEXT_CHANNEL_CODE]);
                mTable->setCellWidget(i, 1, makeChannelCode("ChannelCode"));
            break;

            case TTEXT_LEVEL_CONTROL_TYPE:
                cell1->setText(gMessages[TTEXT_LEVEL_CONTROL_TYPE]);
                mTable->setCellWidget(i, 1, makeLevelControlType("ObjectLevelControlType"));
            break;

            case TTEXT_LEVEL_PORT:
                cell1->setText(gMessages[TTEXT_LEVEL_PORT]);
                mTable->setCellWidget(i, 1, makeLevelPort("ObjectLevelPort"));
            break;

            case TTEXT_LEVEL_CODE:
                cell1->setText(gMessages[TTEXT_LEVEL_CODE]);
                mTable->setCellWidget(i, 1, makeLevelCode("ObjectLevelCode"));
            break;

            case TTEXT_LEVEL_CONTROL_VALUE:
                cell1->setText(gMessages[TTEXT_LEVEL_CONTROL_VALUE]);
                mTable->setCellWidget(i, 1, makeLevelControlValue("ObjectLevelControlValue"));
            break;

            case TTEXT_LEVEL_CONTROL_REPEAT_TIME:
                cell1->setText(gMessages[TTEXT_LEVEL_CONTROL_REPEAT_TIME]);
                mTable->setCellWidget(i, 1, makeControlRepeatTime("ObjectLevelControlRepeatTime"));
            break;

            case TTEXT_LEVEL_FUNCTION:
                cell1->setText(gMessages[TTEXT_LEVEL_FUNCTION]);
                mTable->setCellWidget(i, 1, makeLevelFunction("ObjectLevelFunction"));
            break;

            case TTEXT_RANGE_LOW:
                cell1->setText(gMessages[TTEXT_RANGE_LOW]);
                mTable->setCellWidget(i, 1, makeRangeLowHigh("ObjectRangeLow"));
            break;

            case TTEXT_RANGE_HIGH:
                cell1->setText(gMessages[TTEXT_RANGE_HIGH]);
                mTable->setCellWidget(i, 1, makeRangeLowHigh("ObjectRangeHigh"));
            break;

            case TTEXT_RANGE_DRAG_INCREMENT:
                cell1->setText(gMessages[TTEXT_RANGE_DRAG_INCREMENT]);
                mTable->setCellWidget(i, 1, makeRangeDragIncrement("ObjectRangeDragIncrement"));
            break;

            case TTEXT_RANGE_INVERTED:
                cell1->setText(gMessages[TTEXT_RANGE_INVERTED]);
                mTable->setCellWidget(i, 1, makeRangeInverted("ObjectRangeInverted"));
            break;

            case TTEXT_RANGE_TIME_UP:
                cell1->setText(gMessages[TTEXT_RANGE_TIME_UP]);
                mTable->setCellWidget(i, 1, makeRangeTimeUpDown("ObjectRangeTimeUp"));
            break;

            case TTEXT_RANGE_TIME_DOWN:
                cell1->setText(gMessages[TTEXT_RANGE_TIME_DOWN]);
                mTable->setCellWidget(i, 1, makeRangeTimeUpDown("ObjectRangeTimeDown"));
            break;
        }

        mTable->setItem(i, 0, cell1);
    }

    mInitialized = true;
}

void TPropertiesProgramming::onComboObjectFeedback(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onComboObjectFeedback(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    mActObject.fb = static_cast<ObjHandler::FEEDBACK_t>(data.toInt());

    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onSpinAddressPort(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinAddressPort(int value, const QString& name)");

    Q_UNUSED(name);

    if (isAnyPage())
        mPage.ap = value;
    else
        mActObject.ap = value;

    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onSpinAddressCode(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinAddressCode(int value, const QString& name)");

    Q_UNUSED(name);

    if (isAnyPage())
        mPage.ad = value;
    else
        mActObject.ad = value;

    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onSpinChannelPort(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinChannelPort(int value, const QString& name)");

    Q_UNUSED(name);

    if (isAnyPage())
        mPage.cp = value;
    else
        mActObject.cp = value;

    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onSpinChannelCode(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinChannelCode(int value, const QString& name)");

    Q_UNUSED(name);

    if (isAnyPage())
        mPage.ch = value;
    else
        mActObject.ch = value;

    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onComboLevelControlType(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onComboLevelControlType(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    mActObject.vt = data.toString();
    setTable();
    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onSpinLevelPort(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinLevelPort(int value, const QString& name)");

    Q_UNUSED(name);

    mActObject.lp = value;
    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onSpinLevelCode(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinLevelCode(int value, const QString& name)");

    Q_UNUSED(name);

    mActObject.lv = value;
    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onSpinLevelControlValue(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinLevelControlValue(int value, const QString& name)");

    Q_UNUSED(name);

    mActObject.va = value;
    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onSpinControlRepeatTime(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinControlRepeatTime(int value, const QString& name)");

    Q_UNUSED(name);

    mActObject.rv = value;
    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onComboLevelFunction(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onComboLevelFunction(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    mActObject.lf = data.toString();
    setTable();
    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onSpinRangeLowHigh(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinRangeLowHigh(int value, const QString& name)");

    if (name.endsWith("Low"))
        mActObject.rl = value;
    else
        mActObject.rh = value;

    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onSpinRangeDragIncrement(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinRangeDragIncrement(int value, const QString& name)");

    Q_UNUSED(name);

    mActObject.rn = value;
    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onComboRangeInverted(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onComboRangeInverted(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    mActObject.ri = data.toInt();
    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onSpinRangeTimeUpDown(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinRangeTimeUpDown(int value, const QString& name)");

    if (mStype == STATE_BARGRAPH)
    {
        if (name.endsWith("Up"))
            mActObject.lu = value;
        else
            mActObject.ld = value;
    }
    else
    {
        if (name.endsWith("Up"))
            mActObject.ru = value;
        else
            mActObject.rd = value;
    }

    mChanged = true;
    markChanged();
}

TElementWidgetCombo *TPropertiesProgramming::makeObjectFeedback(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeObjectFeedback(const QString& name)");

    QStringList items = { "none", "channel", "inverted channel", "always on", "momentary" };
    QList<QVariant> data = { ObjHandler::FB_NONE, ObjHandler::FB_CHANNEL, ObjHandler::FB_INV_CHANNEL,
                           ObjHandler::FB_ALWAYS_ON, ObjHandler::FB_MOMENTARY};

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
        combo->setCurrentIndex(mActObject.fb);

    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesProgramming::onComboObjectFeedback);
    return combo;
}

TElementSpinBox *TPropertiesProgramming::makeAddressPort(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeAddressPort(const QString& name)");

    int value = 0;

    if (name.startsWith("Page") || name.startsWith("Popup"))
        value = mPage.ap;
    else
        value = mActObject.ap;

    TElementSpinBox *spin = new TElementSpinBox(value, 0, 100, name, mTable);
    connect(spin, &TElementSpinBox::valueChanged, this, &TPropertiesProgramming::onSpinAddressPort);
    return spin;
}

TElementSpinBox *TPropertiesProgramming::makeAddressCode(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeAddressCode(const QString& name)");

    int value = 0;

    if (name.startsWith("Page") || name.startsWith("Popup"))
        value = mPage.ad;
    else
        value = mActObject.ad;

    TElementSpinBox *spin = new TElementSpinBox(value, 0, 4000, name, mTable);
    connect(spin, &TElementSpinBox::valueChanged, this, &TPropertiesProgramming::onSpinAddressCode);
    return spin;
}

TElementSpinBox *TPropertiesProgramming::makeChannelPort(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeChannelPort(const QString& name)");

    int value = 0;

    if (name.startsWith("Page") || name.startsWith("Popup"))
        value = mPage.cp;
    else
        value = mActObject.cp;

    TElementSpinBox *spin = new TElementSpinBox(value, 0, 100, name, mTable);
    connect(spin, &TElementSpinBox::valueChanged, this, &TPropertiesProgramming::onSpinChannelPort);
    return spin;
}

TElementSpinBox *TPropertiesProgramming::makeChannelCode(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeChannelCode(const QString& name)");

    int value = 0;

    if (name.startsWith("Page") || name.startsWith("Popup"))
        value = mPage.ch;
    else
        value = mActObject.ch;

    TElementSpinBox *spin = new TElementSpinBox(value, 0, 4000, name, mTable);
    connect(spin, &TElementSpinBox::valueChanged, this, &TPropertiesProgramming::onSpinChannelCode);
    return spin;
}

TElementWidgetCombo *TPropertiesProgramming::makeLevelControlType(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeLevelControlType(const QString& name)");

    QStringList items = { "none", "absolute", "relativ" };
    QList<QVariant> data = { "", "abs", "rel" };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesProgramming::onComboLevelControlType);
    return combo;
}

TElementSpinBox *TPropertiesProgramming::makeLevelPort(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeLevelPort(const QString& name)");

    TElementSpinBox *spin = new TElementSpinBox(mActObject.lp, 0, 100, name, mTable);
    connect(spin, &TElementSpinBox::valueChanged, this, &TPropertiesProgramming::onSpinLevelPort);
    return spin;
}

TElementSpinBox *TPropertiesProgramming::makeLevelCode(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeLevelCode(const QString& name)");

    TElementSpinBox *spin = new TElementSpinBox(mActObject.lv, 0, 4000, name, mTable);
    connect(spin, &TElementSpinBox::valueChanged, this, &TPropertiesProgramming::onSpinLevelCode);
    return spin;
}

TElementSpinBox *TPropertiesProgramming::makeLevelControlValue(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeLevelControlValue(const QString& name)");

    TElementSpinBox *spin = new TElementSpinBox(mActObject.va, 0, 1000000, name, mTable);
    connect(spin, &TElementSpinBox::valueChanged, this, &TPropertiesProgramming::onSpinLevelControlValue);
    return spin;
}

TElementSpinBox *TPropertiesProgramming::makeControlRepeatTime(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeControlRepeatTime(const QString& name)");

    TElementSpinBox *spin = new TElementSpinBox(mActObject.rv, 0, 10000, name, mTable);
    connect(spin, &TElementSpinBox::valueChanged, this, &TPropertiesProgramming::onSpinControlRepeatTime);
    return spin;
}

TElementWidgetCombo *TPropertiesProgramming::makeLevelFunction(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeLevelFunction(const QString& name)");

    QStringList items = { tr("display only"), tr("active"), tr("active centering"), tr("drag"), tr("drag centering") };
    QList<QVariant> data = { "", "active", "active centering", "drag", "drag centering" };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesProgramming::onComboLevelFunction);
    return combo;
}

TElementSpinBox *TPropertiesProgramming::makeRangeLowHigh(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeRangeLowHigh(const QString& name)");

    int value = 0;

    if (name.endsWith("Low"))
        value = mActObject.rl;
    else
        value = mActObject.rh;

    TElementSpinBox *spin = new TElementSpinBox(value, 0, 1000, name, mTable);
    connect(spin, &TElementSpinBox::valueChanged, this, &TPropertiesProgramming::onSpinRangeLowHigh);
    return spin;
}

TElementSpinBox *TPropertiesProgramming::makeRangeDragIncrement(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeRangeDragIncrement(const QString& name)");

    TElementSpinBox *spin = new TElementSpinBox(mActObject.rn, 0, 100, name, mTable);
    connect(spin, &TElementSpinBox::valueChanged, this, &TPropertiesProgramming::onSpinRangeDragIncrement);
    return spin;
}

TElementWidgetCombo *TPropertiesProgramming::makeRangeInverted(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeRangeInverted(const QString& name)");

    QStringList items = { tr("no"), tr("yes") };
    QList<QVariant> data = { 0, 1 };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);
    combo->setCurrentIndex(mActObject.ri);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesProgramming::onComboRangeInverted);
    return combo;
}

TElementSpinBox *TPropertiesProgramming::makeRangeTimeUpDown(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::makeRangeTimeUpDown(const QString& name)");

    int value = 0;

    if (mStype == STATE_BARGRAPH)
    {
        if (name.endsWith("Up"))
            value = mActObject.lu;
        else
            value = mActObject.ld;
    }
    else
    {
        if (name.endsWith("Up"))
            value = mActObject.ru;
        else
            value = mActObject.rd;
    }

    TElementSpinBox *spin = new TElementSpinBox(value, 0, 10000, name, mTable);
    connect(spin, &TElementSpinBox::valueChanged, this, &TPropertiesProgramming::onSpinRangeTimeUpDown);
    return spin;
}

bool TPropertiesProgramming::isAnyPage()
{
    DECL_TRACER("TPropertiesProgramming::isAnyPage()");

    if (mStype == STATE_PAGE || mStype == STATE_POPUP || mStype == STATE_SUBPAGE)
        return true;

    return false;
}

