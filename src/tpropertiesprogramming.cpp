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
#include "tconfmain.h"
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

    if (!loaded)
    {
        page = *TPageHandler::Current().getPage(id);

        if (page.pageID == mPage.pageID)
            return;
    }

    if (!loaded && mPage.pageID > 0 && mChanged)
    {
        saveChangedData(&mPage, TBL_PROGRAM);
        mPage = Page::PAGE_t();
    }

    mChanged = false;

    if (!loaded)
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

    Page::PAGE_t page = TPageHandler::Current().getPage(name);

    if (page.pageID == mPage.pageID)
        return;

    if (mPage.pageID > 0 && mChanged)
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

    if (!loaded)
    {
        page = *TPageHandler::Current().getPage(id);

        if (page.pageID == mPage.pageID)
            return;
    }

    if (!loaded && mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_PROGRAM);

    mChanged = false;

    if (!loaded)
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

    mActObjectID = id;
    mActObject = mPage.objects[id]->getObject();
    setSType();
}

void TPropertiesProgramming::setObject(ObjHandler::TOBJECT_t& object, int id)
{
    DECL_TRACER("TPropertiesProgramming::setObject(ObjHandler::TOBJECT_t& object, int id)");

    mActObject = object;
    mActObjectID = id;
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
        case ObjHandler::SUBPAGE_VIEW:
            mStype = STATE_SUBPAGE;
        break;

        default:
            mStype = STATE_UNKNOWN;
    }

    setTable();
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

void TPropertiesProgramming::setTable()
{
    DECL_TRACER("TPropertiesProgramming::setTable()");

    if (!mInitialized)
        createPage();

    for (int i = 0; i < mTable->rowCount(); ++i)
        mTable->setRowHidden(i, true);

    MSG_DEBUG("Rows: " << mTable->rowCount() << ", SType: " << mStype);

    if (mStype == STATE_PAGE || mStype == STATE_POPUP || mStype == STATE_INPUT)
    {
        mTable->setRowHidden(TTEXT_ADDRESS_PORT, false);
        mTable->setRowHidden(TTEXT_ADDRESS_CODE, false);
        mTable->setRowHidden(TTEXT_CHANNEL_PORT, false);
        mTable->setRowHidden(TTEXT_CHANNEL_CODE, false);
    }
    else if (mStype == STATE_SUBPAGE)
    {
        mTable->setRowHidden(TTEXT_ADDRESS_PORT, false);
        mTable->setRowHidden(TTEXT_ADDRESS_CODE, false);
    }
    else if (mStype == STATE_BUTTON)
    {
        mTable->setRowHidden(TTEXT_FEEDBACK, false);
        mTable->setRowHidden(TTEXT_ADDRESS_PORT, false);
        mTable->setRowHidden(TTEXT_ADDRESS_CODE, false);
        mTable->setRowHidden(TTEXT_CHANNEL_PORT, false);
        mTable->setRowHidden(TTEXT_CHANNEL_CODE, false);
        mTable->setRowHidden(TTEXT_LEVEL_CONTROL_TYPE, false);

        if (mActObject.vt == "abs")
        {
            mTable->setRowHidden(TTEXT_LEVEL_PORT, false);
            mTable->setRowHidden(TTEXT_LEVEL_CODE, false);
            mTable->setRowHidden(TTEXT_LEVEL_CONTROL_VALUE, false);
            mTable->setRowHidden(TTEXT_RANGE_LOW, false);
            mTable->setRowHidden(TTEXT_RANGE_HIGH, false);
            mTable->setRowHidden(TTEXT_RANGE_TIME_UP, false);
            mTable->setRowHidden(TTEXT_RANGE_TIME_DOWN, false);
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

        if (mActObject.lf.startsWith("drag"))
            mTable->setRowHidden(TTEXT_RANGE_DRAG_INCREMENT, false);

        mTable->setRowHidden(TTEXT_RANGE_INVERTED, false);

        if (mActObject.lf.startsWith("active"))
        {
            mTable->setRowHidden(TTEXT_RANGE_TIME_UP, false);
            mTable->setRowHidden(TTEXT_RANGE_TIME_DOWN, false);
        }
    }
}

void TPropertiesProgramming::createPage()
{
    DECL_TRACER("TPropertiesProgramming::createPage()");

    int setupPort = TConfMain::Current().getSetupPort();
    mInitialized = false;
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
                mTable->setCellWidget(i, 1, makeAddressPort(isAnyPage() ? "PageAddressPort" : "ObjectAddressPort"));
            break;

            case TTEXT_ADDRESS_CODE:
                cell1->setText(gMessages[TTEXT_ADDRESS_CODE]);
                mTable->setCellWidget(i, 1, makeAddressCode(isAnyPage() ? "PageAddressCode" : "ObjectAddressCode"));
            break;

            case TTEXT_CHANNEL_PORT:
                cell1->setText(gMessages[TTEXT_CHANNEL_PORT]);
                mTable->setCellWidget(i, 1, makeChannelPort(isAnyPage() ? "PageChannelPort" : "ObjectChannelPort"));
            break;

            case TTEXT_CHANNEL_CODE:
                cell1->setText(gMessages[TTEXT_CHANNEL_CODE]);
                mTable->setCellWidget(i, 1, makeChannelCode(isAnyPage() ? "PageChannelCode" : "ObjectChannelCode"));
            break;

            case TTEXT_LEVEL_CONTROL_TYPE:
                cell1->setText(gMessages[TTEXT_LEVEL_CONTROL_TYPE]);
            break;

            case TTEXT_LEVEL_PORT:
                cell1->setText(gMessages[TTEXT_LEVEL_PORT]);
            break;

            case TTEXT_LEVEL_CODE:
                cell1->setText(gMessages[TTEXT_LEVEL_CODE]);
            break;

            case TTEXT_LEVEL_CONTROL_VALUE:
                cell1->setText(gMessages[TTEXT_LEVEL_CONTROL_VALUE]);
            break;

            case TTEXT_LEVEL_CONTROL_REPEAT_TIME:
                cell1->setText(gMessages[TTEXT_LEVEL_CONTROL_REPEAT_TIME]);
            break;

            case TTEXT_LEVEL_FUNCTION:
                cell1->setText(gMessages[TTEXT_LEVEL_FUNCTION]);
            break;

            case TTEXT_RANGE_LOW:
                cell1->setText(gMessages[TTEXT_RANGE_LOW]);
            break;

            case TTEXT_RANGE_HIGH:
                cell1->setText(gMessages[TTEXT_RANGE_HIGH]);
            break;

            case TTEXT_RANGE_DRAG_INCREMENT:
                cell1->setText(gMessages[TTEXT_RANGE_DRAG_INCREMENT]);
            break;

            case TTEXT_RANGE_INVERTED:
                cell1->setText(gMessages[TTEXT_RANGE_INVERTED]);
            break;

            case TTEXT_RANGE_TIME_UP:
                cell1->setText(gMessages[TTEXT_RANGE_TIME_UP]);
            break;

            case TTEXT_RANGE_TIME_DOWN:
                cell1->setText(gMessages[TTEXT_RANGE_TIME_DOWN]);
            break;
        }

        mTable->setItem(i, 0, cell1);
    }

    mInitialized = true;
}

void TPropertiesProgramming::onComboAddrPort(int index)
{
    DECL_TRACER("TPropertiesProgramming::onComboAddrPort(int index)");

    mPage.ap = mAddrPort->itemText(index).toInt();
    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboAddrPortText(const QString& text)
{
    DECL_TRACER("TPropertiesProgramming::onComboAddrPortText(const QString& text)")

    if (text.endsWith("setup port"))
        mPage.ap = TConfMain::Current().getSetupPort();
    else
        mPage.ap = text.toInt();

    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboAddrCode(int index)
{
    DECL_TRACER("TPropertiesProgramming::onComboAddrCode(int index)");

    mPage.ad = mAddrCode->itemText(index).toInt();
    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboAddrCodeText(const QString& text)
{
    DECL_TRACER("TPropertiesProgramming::onComboAddrCodeText(const QString& text)");

    if (text == "none")
        mPage.ad = 0;
    else
        mPage.ad = text.toInt();

    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboChanPort(int index)
{
    DECL_TRACER("TPropertiesProgramming::onComboChanPort(int index)");

    mPage.cp = mChanPort->itemText(index).toInt();
    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboChanPortText(const QString& text)
{
    DECL_TRACER("TPropertiesProgramming::onComboChanPortText(const QString& text)");

    if (text.endsWith("setup port"))
        mPage.cp = TConfMain::Current().getSetupPort();
    else
        mPage.cp = text.toInt();

    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboChanCode(int index)
{
    DECL_TRACER("TPropertiesProgramming::onComboChanCode(int index)");

    mPage.ch = mChanCode->itemText(index).toInt();
    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboChanCodeText(const QString& text)
{
    DECL_TRACER("TPropertiesProgramming::onComboChanCodeText(const QString& text)");

    if (text == "none")
        mPage.ch = 0;
    else
        mPage.ch = text.toInt();

    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboObjectFeedback(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onComboObjectFeedback(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject.fb = static_cast<ObjHandler::FEEDBACK_t>(data.toInt());
        mPage.objects[mActObjectID]->setObject(mActObject);
    }

    mChanged = true;
    markChanged();
}

void TPropertiesProgramming::onSpinAddressPort(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinAddressPort(int value, const QString& name)");

    if (name.startsWith("Page") || name.startsWith("Popup"))
        mPage.ap = value;
    else
    {
        mActObject.ap = value;
        mPage.objects[mActObjectID]->setObject(mActObject);
    }
}

void TPropertiesProgramming::onSpinAddressCode(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinAddressCode(int value, const QString& name)");

    if (name.startsWith("Page") || name.startsWith("Popup"))
        mPage.ad = value;
    else
    {
        mActObject.ad = value;
        mPage.objects[mActObjectID]->setObject(mActObject);
    }
}

void TPropertiesProgramming::onSpinChannelPort(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinChannelPort(int value, const QString& name)");

    if (name.startsWith("Page") || name.startsWith("Popup"))
        mPage.cp = value;
    else
    {
        mActObject.cp = value;
        mPage.objects[mActObjectID]->setObject(mActObject);
    }
}

void TPropertiesProgramming::onSpinChannelCode(int value, const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::onSpinChannelCode(int value, const QString& name)");

    if (name.startsWith("Page") || name.startsWith("Popup"))
        mPage.ch = value;
    else
    {
        mActObject.ch = value;
        mPage.objects[mActObjectID]->setObject(mActObject);
    }
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

bool TPropertiesProgramming::isAnyPage()
{
    DECL_TRACER("TPropertiesProgramming::isAnyPage()");

    if (mStype == STATE_PAGE || mStype == STATE_POPUP || mStype == STATE_SUBPAGE)
        return true;

    return false;
}

