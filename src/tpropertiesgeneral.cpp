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
#include <QStandardItemModel>
#include <QComboBox>
#include <QToolButton>
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include "tpropertiesgeneral.h"
#include "telementbordername.h"
#include "telementwidgetcombo.h"
#include "telementwidgettext.h"
#include "telementlineedit.h"
#include "telementspinbox.h"
#include "telementcolorselector.h"
#include "tgraphics.h"
#include "tconfmain.h"
#include "terror.h"

#define TTEXT_POPUPTYPE             1   // Popup only
#define TTEXT_TYPE                  2   // Buttons only
#define TTEXT_NAME                  3
#define TTEXT_LOCK_BUTTON_NAME      4   // Buttons
#define TTEXT_DESCRIPTION           5   // Page & Popup
#define TTEXT_LEFT                  6
#define TTEXT_TOP                   7
#define TTEXT_WIDTH                 8
#define TTEXT_HEIGHT                9
#define TTEXT_ZORDER                10  // Buttons
#define TTEXT_DRAGDROP_TYPE         11  // Buttons
#define TTEXT_DROP_GROUP            12  // Button; Only if "draggable"
#define TTEXT_RESET_POS_ON_SHOW     13
#define TTEXT_GROUP                 14
#define TTEXT_TIMEOUT               15
#define TTEXT_MODAL                 16
#define TTEXT_SHOW_EFFECT           17
#define TTEXT_SHOW_EFFECT_TIME      18
#define TTEXT_SHOW_EFFECT_X_POS     19
#define TTEXT_SHOW_EFFECT_Y_POS     20
#define TTEXT_HIDE_EFFECT           21
#define TTEXT_HIDE_EFFECT_TIME      22
#define TTEXT_HIDE_EFFECT_X_POS     23
#define TTEXT_HIDE_EFFECT_Y_POS     24
#define TTEXT_COLLAPSE_DIRECTION    25
#define TTEXT_COLLAPSE_OFFSET       26
#define TTEXT_COLLAPSE_SHOW_OPEN    27
#define TTEXT_TOUCH_STYLE           28
#define TTEXT_BORDER_STYLE          29
#define TTEXT_DISABLED              30
#define TTEXT_HIDDEN                31
#define TTEXT_PASSWORD_PROTECTION   32
#define TTEXT_STATE_COUNT           33
#define TTEXT_ANIMATE_TIME_UP       34
#define TTEXT_ANIMATE_TIME_DOWN     35
#define TTEXT_AUTO_REPEAT           36
#define TTEXT_VALUE_DIRECTION       37
#define TTEXT_SLIDER_NAME           38
#define TTEXT_SLIDER_COLOR          39
#define TTEXT_SUBPAGE_SET           40
#define TTEXT_ORIENTATION           41
#define TTEXT_SPACING               42
#define TTEXT_ANCHOR_POSITION       43
#define TTEXT_SHOW_SUBPAGES         44
#define TTEXT_ALLOW_DYN_REORDERING  45
#define TTEXT_RESET_VIEW_ON_SHOW    46
#define TTEXT_SCROLLBAR             47
#define TTEXT_SCROLLBAR_OFFSET      48
#define TTEXT_DISABLE_TOUCH_SCROLL  49
#define TTEXT_ENABLE_ANCHORING      50

#define LIST_POPUPTYPE              0
#define LIST_BUTTON_TYPE            1
#define LIST_PAGE_NAME              2
#define LIST_OBJECT_NAME            3
#define LIST_BUTTON_LOCK_NAME       4
#define LIST_PAGE_DESCRIPTION       5
#define LIST_OBJECT_DESCRIPTION     6
#define LIST_POPUP_LEFT             7
#define LIST_OBJECT_LEFT            8
#define LIST_POPUP_TOP              9
#define LIST_OBJECT_TOP             10
#define LIST_POPUP_WIDTH            11
#define LIST_OBJECT_WIDTH           12
#define LIST_POPUP_HEIGHT           13
#define LIST_OBJECT_HEIGHT          14
#define LIST_POPUP_RESET_ON_POS     15
#define LIST_OBJECT_Z_ORDER         16
#define LIST_POPUP_GROUPS           17
#define LIST_POPUP_TIMEOUT          18
#define LIST_POPUP_MODAL            19
#define LIST_POPUP_SHOW_EFFECT      20
#define LIST_POPUP_SHOW_EFFECT_TIME 21
#define LIST_POPUP_SHOW_EFFECT_X    22
#define LIST_POPUP_SHOW_EFFECT_Y    23
#define LIST_POPUP_HIDE_EFFECT      24
#define LIST_POPUP_HIDE_EFFECT_TIME 25
#define LIST_POPUP_HIDE_EFFECT_X    26
#define LIST_POPUP_HIDE_EFFECT_Y    27
#define LIST_POPUP_COLLAPSE_DIR     28
#define LIST_POPUP_COLLAPSE_OFFSET  29
#define LIST_POPUP_COLLAPSE_SHOWOP  30
#define LIST_OBJECT_DRAG_DROP_TYPE  31
#define LIST_OBJECT_DROP_GROUP      32
#define LIST_OBJECT_TOUCH_STYLE     33
#define LIST_OBJECT_BORDER_STYLE    34
#define LIST_OBJECT_STATE_COUNT     35
#define LIST_OBJECT_ANIMATE_TIME_UP 36
#define LIST_OBJECT_ANIMATE_TIME_DN 37
#define LIST_OBJECT_AUTOREPEAT      38
#define LIST_OBJECT_DISABLED        39
#define LIST_OBJECT_HIDDEN          40
#define LIST_OBJECT_VALUE_DIRECTION 41
#define LIST_OBJECT_SLIDER_NAME     42
#define LIST_OBJECT_SLIDER_COLOR    43
#define LIST_OBJECT_PASSWD_PROTECT  44
#define LIST_OBJECT_SUB_PAGE_SET    45

#define LIST_OBJECT_ORIENTATION     46
#define LIST_OBJECT_SPACING         47
#define LIST_OBJECT_ANCHOR_POSITION 48
#define LIST_OBJECT_SHOW_SUBPAGES   49
#define LIST_OBJECT_DYNAMIC_REORDER 50
#define LIST_OBJECT_RESET_VIEW_SHOW 51
#define LIST_OBJECT_SCROLLBAR       52
#define LIST_OBJECT_SCROLL_OFFSET   53
#define LIST_OBJECT_DISABLE_TOUCHSC 54
#define LIST_OBJECT_ENABLE_ANCHOR   55

#define LIST_MAX_ROWS               56

using namespace ObjHandler;

TPropertiesGeneral::TPropertiesGeneral()
{
    DECL_TRACER("TPropertiesGeneral::TPropertiesGeneral()");
}

TPropertiesGeneral::TPropertiesGeneral(QTableWidget *view)
    : mTable(view)
{
    DECL_TRACER("TPropertiesGeneral::TPropertiesGeneral(QTableWidget *view)");

    if (view)
    {
        connect(view, &QTableWidget::cellChanged, this, &TPropertiesGeneral::onCellChanged);
        connect(view, &QTableWidget::cellActivated, this, &TPropertiesGeneral::onCellActivated);
        mConnected = true;
    }
}

TPropertiesGeneral::~TPropertiesGeneral()
{
    DECL_TRACER("TPropertiesGeneral::~TPropertiesGeneral()");

    if (mTable && mConnected)
    {
        disconnect(mTable, &QTableWidget::cellChanged, this, &TPropertiesGeneral::onCellChanged);
        disconnect(mTable, &QTableWidget::cellActivated, this, &TPropertiesGeneral::onCellActivated);
        mConnected = false;
    }
}

void TPropertiesGeneral::setGeometryPopup(const QRect& geom, bool block)
{
    DECL_TRACER("TPropertiesGeneral::setGeometry(const QRect& geom, bool block)");

    if (!mTable || mPage.popupType != Page::PT_POPUP)
        return;

    mSignalBlocked = block;
    onSpinGeometryChanged(geom.left(), "PopupLeft");
    onSpinGeometryChanged(geom.top(), "PopupTop");
    onSpinGeometryChanged(geom.width(), "PopupWidth");
    onSpinGeometryChanged(geom.height(), "PopupHeight");

    if (!mSignalBlocked)
        update();

    mSignalBlocked = false;
}

void TPropertiesGeneral::setGeometryButton(int bi, const QRect& geom, bool block)
{
    DECL_TRACER("TPropertiesGeneral::setGeometryButton(int bi, const QRect& geom, bool block)");

    if (!mTable || bi <= 0)
        return;

    mActObjectID = bi - 1;
    mSignalBlocked = block;
    onSpinGeometryChanged(geom.left(), "ObjectLeft");
    onSpinGeometryChanged(geom.top(), "ObjectTop");
    onSpinGeometryChanged(geom.width(), "ObjectWidth");
    onSpinGeometryChanged(geom.height(), "ObjectHeight");
    mSignalBlocked = false;
}

void TPropertiesGeneral::update()
{
    DECL_TRACER("TPropertiesGeneral::update()");

    ObjHandler::TOBJECT_t object = getActualObject(mPage);
    MSG_DEBUG("Updating object " << object.bi << " on page " << mPage.pageID);
    // If the object ID (bi) is less or equal 0, then we must show the
    // data for a page or a popup.
    if (object.bi <= 0)
    {
        if (mPage.popupType == Page::PT_PAGE)
            setTable(STATE_PAGE);
        else if (mPage.popupType == Page::PT_POPUP)
            setTable(STATE_POPUP);
    }
    else
    {
        mActObjectID = TPageHandler::Current().getObjectIndex(mPage, object.bi);
        setTable(getStateFromButtonType(object.type), true);
    }
}

void TPropertiesGeneral::clear()
{
    DECL_TRACER("TPropertiesGeneral::clear()");

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_GENERAL);

    mPage = Page::PAGE_t();
    mChanged = false;
    mTable->clear();
    mTable->setRowCount(0);
    mTable->setColumnCount(0);
}

QString TPropertiesGeneral::getLabelText(int line)
{
    DECL_TRACER("TPropertiesGeneral::getLabelText(int line)");

    switch(line)
    {
        case TTEXT_POPUPTYPE:           return tr("PopupType"); break;
        case TTEXT_TYPE:                return tr("Type"); break;
        case TTEXT_NAME:                return tr("Name");
        case TTEXT_LOCK_BUTTON_NAME:    return tr("Lock Button Name"); break;
        case TTEXT_DESCRIPTION:         return tr("Description"); break;
        case TTEXT_LEFT:                return tr("Left"); break;
        case TTEXT_TOP:                 return tr("Top"); break;
        case TTEXT_WIDTH:               return tr("Width"); break;
        case TTEXT_HEIGHT:              return tr("Height"); break;
        case TTEXT_ZORDER:              return tr("Z-Order"); break;
        case TTEXT_DRAGDROP_TYPE:       return tr("Drag/Drop Type");
        case TTEXT_DROP_GROUP:          return tr("Drop Group"); break;
        case TTEXT_RESET_POS_ON_SHOW:   return tr("Reset Pos On Show"); break;
        case TTEXT_GROUP:               return tr("Group"); break;
        case TTEXT_TIMEOUT:             return tr("Timeout"); break;
        case TTEXT_MODAL:               return tr("Modal"); break;
        case TTEXT_SHOW_EFFECT:         return tr("Show Effect"); break;
        case TTEXT_SHOW_EFFECT_TIME:    return tr("Show Effect Time"); break;
        case TTEXT_SHOW_EFFECT_X_POS:   return tr("Show Effect X Pos"); break;
        case TTEXT_SHOW_EFFECT_Y_POS:   return tr("Show Effect Y Pos"); break;
        case TTEXT_HIDE_EFFECT:         return tr("Hide Effect"); break;
        case TTEXT_HIDE_EFFECT_TIME:    return tr("Hide Effect Time"); break;
        case TTEXT_HIDE_EFFECT_X_POS:   return tr("Hide Effect X Pos"); break;
        case TTEXT_HIDE_EFFECT_Y_POS:   return tr("Hide Effect Y Pos"); break;
        case TTEXT_COLLAPSE_DIRECTION:  return tr("Collapse Direction"); break;
        case TTEXT_COLLAPSE_OFFSET:     return tr("Collapse Offset"); break;
        case TTEXT_COLLAPSE_SHOW_OPEN:  return tr("Show Open"); break;
        case TTEXT_TOUCH_STYLE:         return tr("Touch Style"); break;
        case TTEXT_BORDER_STYLE:        return tr("Border Style"); break;
        case TTEXT_DISABLED:            return tr("Disabled"); break;
        case TTEXT_HIDDEN:              return tr("Hidden"); break;
        case TTEXT_PASSWORD_PROTECTION: return tr("Password Protection"); break;
        case TTEXT_STATE_COUNT:         return tr("State Count"); break;
        case TTEXT_ANIMATE_TIME_UP:     return tr("Animate Time Up"); break;
        case TTEXT_ANIMATE_TIME_DOWN:   return tr("Animate Time Down"); break;
        case TTEXT_AUTO_REPEAT:         return tr("Auto-Repeat"); break;
        case TTEXT_VALUE_DIRECTION:     return tr("Value Direction"); break;
        case TTEXT_SLIDER_NAME:         return tr("Slider Name"); break;
        case TTEXT_SLIDER_COLOR:        return tr("Slider Color"); break;
        case TTEXT_SUBPAGE_SET:         return tr("Sub-Page Set"); break;
        case TTEXT_ORIENTATION:         return tr("Orientation"); break;
        case TTEXT_SPACING:             return tr("Spacing (%)"); break;
        case TTEXT_ANCHOR_POSITION:     return tr("Anchor Position"); break;
        case TTEXT_SHOW_SUBPAGES:       return tr("Schow Sub-Pages"); break;
        case TTEXT_ALLOW_DYN_REORDERING:return tr("Allow Dynamic Reordering"); break;
        case TTEXT_RESET_VIEW_ON_SHOW:  return tr("Reset View on Show"); break;
        case TTEXT_SCROLLBAR:           return tr("ScrollBar"); break;
        case TTEXT_SCROLLBAR_OFFSET:    return tr("Offset of Scrollbar"); break;
        case TTEXT_DISABLE_TOUCH_SCROLL:return tr("Disable Touch Scrolling"); break;
        case TTEXT_ENABLE_ANCHORING:    return tr("Enable Anchoring"); break;
    }

    return QString();
}

void TPropertiesGeneral::loadPage(int pageID)
{
    DECL_TRACER("TPropertiesGeneral::loadPage(int pageID)");

    if (pageID == mPage.pageID)
        return;

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_GENERAL);

    mPage = *TPageHandler::Current().getPage(pageID);
    mChanged = false;
    mInitialized = false;
}

void TPropertiesGeneral::setGeneralPage(Page::PAGE_t& page, STATE_TYPE stype, int objid)
{
    DECL_TRACER("TPropertiesGeneral::setGeneralPage(Page::PAGE_t& page, STATE_TYPE stype, int objid)");

    if (page.pageID <= 0)
        return;

    mInitialized = false;   // This ensures that the changes are visible

    mPage = page;
    mActObjectID = objid;
    MSG_DEBUG("Object index: " << objid);
    setTable(stype);
}

void TPropertiesGeneral::setGeneralObjectID(int index)
{
    DECL_TRACER("TPropertiesGeneral::setGeneralObjectID(int index)");

    if (index < 0 || index >= mPage.objects.size())
    {
        MSG_WARNING("Got wrong index (" << index << ")!");
        return;
    }

    mActObject = mPage.objects[index]->getObject();
    mActObjectID = index;
    setTable(getStateFromButtonType(mActObject.type), true);
}

void TPropertiesGeneral::setTable(STATE_TYPE stype, bool force)
{
    DECL_TRACER("TPropertiesGeneral::setTable(STATE_TYPE stype, bool force)");

    MSG_DEBUG("Setting table for type: " << stype << " (Forced: " << (force ? "YES" : "NO") << ")");

    if (!mInitialized || !mTable || force)
        createTable(stype);

    if (!mTable)
        return;

    // First hide all rows
    for (int i = 0; i < mTable->rowCount(); ++i)
        mTable->setRowHidden(i, true);

    QFont font = TConfMain::Current().getFontBase();
    font.setPointSize(TConfMain::Current().getFontBaseSize());

    // Now enable only the used rows
    switch(stype)
    {
        case STATE_PAGE:
            mTable->setRowHidden(LIST_PAGE_NAME, false);                            // PageName
            mTable->setRowHidden(LIST_PAGE_DESCRIPTION, false);                     // PageDescription

            setTableWidget(LIST_PAGE_NAME, 1, mPage.name, W_LINEEDIT, font);
            setTableWidget(LIST_PAGE_DESCRIPTION, 1, mPage.description, W_TEXT, font);
        break;

        case STATE_POPUP:
            mTable->setRowHidden(LIST_POPUPTYPE, false);                            // PopupPopupType
            setTableWidget(LIST_POPUPTYPE, 1, mPage.popupType, W_COMBO, font);

            mTable->setRowHidden(LIST_PAGE_NAME, false);                            // PageName
            setTableWidget(LIST_PAGE_NAME, 1, mPage.name, W_LINEEDIT, font);

            mTable->setRowHidden(LIST_PAGE_DESCRIPTION, false);                     // PageDescription
            setTableWidget(LIST_PAGE_DESCRIPTION, 1, mPage.description, W_TEXT, font);

            if (mPage.popupType == Page::PT_POPUP)
            {
                mTable->setRowHidden(LIST_POPUP_LEFT, false);                       // PopupLeft
                setTableWidget(LIST_POPUP_LEFT, 1, mPage.left, W_SPINBOX, font);
                mTable->setRowHidden(LIST_POPUP_TOP, false);                        // PopupTop
                setTableWidget(LIST_POPUP_TOP, 1, mPage.top, W_SPINBOX, font);
            }

            mTable->setRowHidden(LIST_POPUP_WIDTH, false);                          // PopupWidth
            setTableWidget(LIST_POPUP_WIDTH, 1, mPage.width, W_SPINBOX, font);
            mTable->setRowHidden(LIST_POPUP_HEIGHT, false);                         // PopupHeight
            setTableWidget(LIST_POPUP_HEIGHT, 1, mPage.height, W_SPINBOX, font);

            if (mPage.popupType == Page::PT_POPUP)
            {
                mTable->setRowHidden(LIST_POPUP_RESET_ON_POS, false);               // PopupResetOnPos
                setTableWidget(LIST_POPUP_RESET_ON_POS, 1, mPage.resetPos, W_COMBO, font);
                mTable->setRowHidden(LIST_POPUP_GROUPS, false);                     // PopupGroups
                setTableWidget(LIST_POPUP_GROUPS, 1, mPage.group, W_COMBO, font);
                mTable->setRowHidden(LIST_POPUP_TIMEOUT, false);                    // PopupTimeout
                setTableWidget(LIST_POPUP_TIMEOUT, 1, mPage.timeout, W_SPINBOX, font);
                mTable->setRowHidden(LIST_POPUP_MODAL, false);                      // PopupModal
                setTableWidget(LIST_POPUP_MODAL, 1, mPage.modal, W_COMBO, font);

                if (mPage.collapseDirection == Page::COLDIR_NONE)
                {
                    mTable->setRowHidden(LIST_POPUP_SHOW_EFFECT, false);            // PopupShowEffect
                    setTableWidget(LIST_POPUP_SHOW_EFFECT, 1, mPage.showEffect, W_COMBO, font);

                    if (mPage.showEffect != Page::SE_NONE)
                    {
                        mTable->setRowHidden(LIST_POPUP_SHOW_EFFECT_TIME, false);   // PopupShowEffectTime
                        setTableWidget(LIST_POPUP_SHOW_EFFECT_TIME, 1, mPage.showTime, W_SPINBOX, font);

                        if (mPage.showEffect != Page::SE_FADE)
                        {
                            if (mPage.showEffect == Page::SE_SLIDE_LEFT || mPage.showEffect == Page::SE_SLIDE_LEFT_FADE||
                                mPage.showEffect == Page::SE_SLIDE_RIGHT || mPage.showEffect == Page::SE_SLIDE_RIGHT_FADE)
                            {
                                mTable->setRowHidden(LIST_POPUP_SHOW_EFFECT_X, false);    // PopupShowEffectPosX
                                setTableWidget(LIST_POPUP_SHOW_EFFECT_X, 1, mPage.showX, W_SPINBOX, font);
                            }
                            else
                            {
                                mTable->setRowHidden(LIST_POPUP_SHOW_EFFECT_Y, false);    // PopupShowEffectPosY
                                setTableWidget(LIST_POPUP_SHOW_EFFECT_Y, 1, mPage.showY, W_SPINBOX, font);
                            }
                        }
                    }

                    mTable->setRowHidden(LIST_POPUP_HIDE_EFFECT, false);            // PopupHideEffect
                    setTableWidget(LIST_POPUP_HIDE_EFFECT, 1, mPage.hideEffect, W_COMBO, font);

                    if (mPage.hideEffect != Page::SE_NONE)
                    {
                        mTable->setRowHidden(LIST_POPUP_HIDE_EFFECT_TIME, false);   // PopupHideEffectTime
                        setTableWidget(LIST_POPUP_HIDE_EFFECT_TIME, 1, mPage.hideTime, W_SPINBOX, font);

                        if (mPage.hideEffect != Page::SE_FADE)
                        {
                            if (mPage.hideEffect == Page::SE_SLIDE_LEFT || mPage.hideEffect == Page::SE_SLIDE_LEFT_FADE||
                                mPage.hideEffect == Page::SE_SLIDE_RIGHT || mPage.hideEffect == Page::SE_SLIDE_RIGHT_FADE)
                            {
                                mTable->setRowHidden(LIST_POPUP_HIDE_EFFECT_X, false);    // PopupHideEffectPosX
                                setTableWidget(LIST_POPUP_HIDE_EFFECT_X, 1, mPage.hideX, W_SPINBOX, font);
                            }
                            else
                            {
                                mTable->setRowHidden(LIST_POPUP_HIDE_EFFECT_Y, false);    // PopupHideEffectPosY
                                setTableWidget(LIST_POPUP_HIDE_EFFECT_Y, 1, mPage.hideY, W_SPINBOX, font);
                            }
                        }
                    }

                    mTable->setRowHidden(LIST_POPUP_COLLAPSE_DIR, false);           // PopupCollapseDirection
                    setTableWidget(LIST_POPUP_COLLAPSE_DIR, 1, mPage.collapseDirection, W_COMBO, font);
                }
                else
                {
                    mTable->setRowHidden(LIST_POPUP_COLLAPSE_DIR, false);           // PopupCollapseDirection
                    setTableWidget(LIST_POPUP_COLLAPSE_DIR, 1, mPage.collapseDirection, W_COMBO, font);
                    mTable->setRowHidden(LIST_POPUP_COLLAPSE_OFFSET, false);        // PopupCollapseOffest
                    setTableWidget(LIST_POPUP_COLLAPSE_OFFSET, 1, mPage.collapseOffset, W_SPINBOX, font);
                    mTable->setRowHidden(LIST_POPUP_COLLAPSE_SHOWOP, false);        // PopupCollapseShowOpen
                    setTableWidget(LIST_POPUP_COLLAPSE_SHOWOP, 1, mPage.collapseShowOpen, W_COMBO, font);
                }
            }
        break;

        case STATE_BUTTON:
        case STATE_BARGRAPH:
            if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
                mActObject = mPage.objects[mActObjectID]->getObject();

            mTable->setRowHidden(LIST_BUTTON_TYPE, false);                          // ButtonType
            setTableWidget(LIST_BUTTON_TYPE, 1, mActObject.type, W_COMBO, font);
            mTable->setRowHidden(LIST_OBJECT_NAME, false);                          // ObjectName
            setTableWidget(LIST_OBJECT_NAME, 1, mActObject.na, W_LINEEDIT, font);
            mTable->setRowHidden(LIST_BUTTON_LOCK_NAME, false);                     // ButtonLockName
            setTableWidget(LIST_BUTTON_LOCK_NAME, 1, mActObject.li, W_COMBO, font);
            mTable->setRowHidden(LIST_OBJECT_DESCRIPTION, false);                   // ObjectDescription
            setTableWidget(LIST_OBJECT_DESCRIPTION, 1, mActObject.bd, W_TEXT, font);
            mTable->setRowHidden(LIST_OBJECT_LEFT, false);                          // ObjectLeft
            setTableWidget(LIST_OBJECT_LEFT, 1, mActObject.lt, W_SPINBOX, font);
            mTable->setRowHidden(LIST_OBJECT_TOP, false);                           // ObjectTop
            setTableWidget(LIST_OBJECT_TOP, 1, mActObject.tp, W_SPINBOX, font);
            mTable->setRowHidden(LIST_OBJECT_WIDTH, false);                         // ObjectWidth
            setTableWidget(LIST_OBJECT_WIDTH, 1, mActObject.wt, W_SPINBOX, font);
            mTable->setRowHidden(LIST_OBJECT_HEIGHT, false);                        // ObjectHeight
            setTableWidget(LIST_OBJECT_HEIGHT, 1, mActObject.ht, W_SPINBOX, font);
            mTable->setRowHidden(LIST_OBJECT_Z_ORDER, false);                       // ObjectZOrder
            setTableWidget(LIST_OBJECT_Z_ORDER, 1, mActObject.zo, W_LINEEDIT, font);

            if (mActObject.type != ObjHandler::BARGRAPH &&
                mActObject.type != ObjHandler::MULTISTATE_BARGRAPH)
            {
                mTable->setRowHidden(LIST_OBJECT_DRAG_DROP_TYPE, false);            // ObjectDragDropType
                setTableWidget(LIST_OBJECT_DRAG_DROP_TYPE, 1, mActObject.ddt, W_COMBO, font);

                if (mActObject.ddt == "dr")
                {
                    mTable->setRowHidden(LIST_OBJECT_DROP_GROUP, false);            // ObjectDropGroup
//                    setTableWidget(LIST_OBJECT_DROP_GROUP, 1, mActObject.ddt, W_COMBO, font);
                }

                mTable->setRowHidden(LIST_OBJECT_TOUCH_STYLE, false);               // ObjectTouchStyle
                setTableWidget(LIST_OBJECT_TOUCH_STYLE, 1, mActObject.hs, W_COMBO, font);
            }
            else if (mActObject.type == ObjHandler::TEXT_INPUT)
            {
                mTable->setRowHidden(LIST_OBJECT_TOUCH_STYLE, false);               // ObjectTouchStyle
                setTableWidget(LIST_OBJECT_TOUCH_STYLE, 1, mActObject.hs, W_COMBO, font);
            }

            if (mActObject.type != ObjHandler::LISTVIEW)
            {
                mTable->setRowHidden(LIST_OBJECT_BORDER_STYLE, false);              // ObjectBorderStyle
                setTableWidget(LIST_OBJECT_BORDER_STYLE, 1, mActObject.bs, W_BORDERNAME, font);
            }

            if (mActObject.type == ObjHandler::MULTISTATE_GENERAL ||
                mActObject.type == ObjHandler::MULTISTATE_BARGRAPH)
            {
                mTable->setRowHidden(LIST_OBJECT_STATE_COUNT, false);               // ObjectStateCount
                setTableWidget(LIST_OBJECT_STATE_COUNT, 1, mActObject.stateCount, W_SPINBOX, font);

                if (mActObject.type != ObjHandler::MULTISTATE_BARGRAPH)
                {
                    mTable->setRowHidden(LIST_OBJECT_ANIMATE_TIME_UP, false);       // ObjectAnimateTimeUp
                    setTableWidget(LIST_OBJECT_ANIMATE_TIME_UP, 1, mActObject.nu, W_SPINBOX, font);
                    mTable->setRowHidden(LIST_OBJECT_ANIMATE_TIME_DN, false);       // ObjectAnimateTimeDown
                    setTableWidget(LIST_OBJECT_ANIMATE_TIME_DN, 1, mActObject.nd, W_SPINBOX, font);
                }
            }

            if (mActObject.type == ObjHandler::SUBPAGE_VIEW)
            {
                mTable->setRowHidden(LIST_OBJECT_BORDER_STYLE, false);              // ObjectBorderStyle
                setTableWidget(LIST_OBJECT_BORDER_STYLE, 1, mActObject.bs, W_BORDERNAME, font);
            }

            mTable->setRowHidden(LIST_OBJECT_DISABLED, false);                      // ObjectDisabled
            setTableWidget(LIST_OBJECT_DISABLED, 1, mActObject.da, W_COMBO, font);
            mTable->setRowHidden(LIST_OBJECT_HIDDEN, false);                        // ObjectHidden
            setTableWidget(LIST_OBJECT_HIDDEN, 1, mActObject.hd, W_COMBO, font);

            if (mActObject.type == ObjHandler::LISTVIEW)
            {
                // TODO: Listview Components (Primary Text, Secondary Text, Image)
                // TODO: Item Hight
                // TODO: Listview Columns
                // TODO: Listview Item Layout
                // TODO: Primary Partition (%)

//                if ((mActObject.lvc & 0x004) > 0)
                    // TODO: Secondary Partition (%)

                // TODO: Filter Enabled

//                if (mActObject.lvs != 0)    // Filter enabled?
                    // TODO: Filter Height

                // TODO: Alphabet Scrollbar
                // TODO: Dynamic Data Source
            }
            else if (mActObject.type == ObjHandler::SUBPAGE_VIEW)
            {
                mTable->setRowHidden(LIST_OBJECT_SUB_PAGE_SET, false);              // ObjectSubPageView
//                setTableWidget(LIST_OBJECT_SUB_PAGE_SET, 1, mActObject.ddt, W_COMBO, font);
                mTable->setRowHidden(LIST_OBJECT_ORIENTATION, false);
                setTableWidget(LIST_OBJECT_ORIENTATION, 1, mActObject.on, W_COMBO, font);
                mTable->setRowHidden(LIST_OBJECT_SPACING, false);
                setTableWidget(LIST_OBJECT_SPACING, 1, mActObject.sa, W_SPINBOX, font);
                mTable->setRowHidden(LIST_OBJECT_ANCHOR_POSITION, false);
                setTableWidget(LIST_OBJECT_ANCHOR_POSITION, 1, mActObject.we, W_COMBO, font);
                mTable->setRowHidden(LIST_OBJECT_SHOW_SUBPAGES, false);
                setTableWidget(LIST_OBJECT_SHOW_SUBPAGES, 1, mActObject.sw, W_COMBO, font);
                mTable->setRowHidden(LIST_OBJECT_DYNAMIC_REORDER, false);
                setTableWidget(LIST_OBJECT_DYNAMIC_REORDER, 1, mActObject.dy, W_COMBO, font);
                mTable->setRowHidden(LIST_OBJECT_RESET_VIEW_SHOW, false);
                setTableWidget(LIST_OBJECT_RESET_VIEW_SHOW, 1, mActObject.rs, W_COMBO, font);
                mTable->setRowHidden(LIST_OBJECT_SCROLLBAR, false);
                setTableWidget(LIST_OBJECT_SCROLLBAR, 1, mActObject.ba, W_COMBO, font);

                if (mActObject.ba != 0)
                {
                    mTable->setRowHidden(LIST_OBJECT_SCROLL_OFFSET, false);
                    setTableWidget(LIST_OBJECT_SCROLL_OFFSET, 1, mActObject.bo, W_SPINBOX, font);
                }

                mTable->setRowHidden(LIST_OBJECT_DISABLE_TOUCHSC, false);
                setTableWidget(LIST_OBJECT_DISABLE_TOUCHSC, 1, mActObject.ds, W_COMBO, font);
                mTable->setRowHidden(LIST_OBJECT_ENABLE_ANCHOR, false);
                setTableWidget(LIST_OBJECT_ENABLE_ANCHOR, 1, mActObject.sdd, W_COMBO, font);
            }
            else
            {
                if (mActObject.type == ObjHandler::TEXT_INPUT)
                {
                    // TODO: Input type
                }

                if (mActObject.type == ObjHandler::BARGRAPH ||
                    mActObject.type == ObjHandler::MULTISTATE_BARGRAPH)
                {
                    mTable->setRowHidden(LIST_OBJECT_VALUE_DIRECTION, false);       // ObjectValueDirection
                    setTableWidget(LIST_OBJECT_VALUE_DIRECTION, 1, mActObject.dr, W_COMBO, font);

                    if (mActObject.type == ObjHandler::BARGRAPH)
                    {
                        mTable->setRowHidden(LIST_OBJECT_SLIDER_NAME, false);       // ObjectSliderName
                        setTableWidget(LIST_OBJECT_SLIDER_NAME, 1, mActObject.sd, W_COMBO, font);
                        mTable->setRowHidden(LIST_OBJECT_SLIDER_COLOR, false);      // ObjectSliderColor
                        setTableWidget(LIST_OBJECT_SLIDER_COLOR, 1, mActObject.sc, W_COLORSELECTOR, font);
                    }
                }
                else if (mActObject.type != ObjHandler::TEXT_INPUT)
                {
                    mTable->setRowHidden(LIST_OBJECT_PASSWD_PROTECT, false);        // ObjectPasswordProtection
                    setTableWidget(LIST_OBJECT_PASSWD_PROTECT, 1, mActObject.pp, W_COMBO, font);
                }

                if (mActObject.type == ObjHandler::TEXT_INPUT)
                {
                    // TODO: Password Character
                    // TODO: Display Type
                    // TODO: Max Text Length
                    // TODO: Input Mask
                }
            }
        break;

        case STATE_SUBPAGE:
            mTable->setRowHidden(LIST_POPUPTYPE, false);                        // PopupType
            setTableWidget(LIST_POPUPTYPE, 1, mPage.popupType, W_COMBO, font);
            mTable->setRowHidden(LIST_PAGE_NAME, false);                        // PageName
            setTableWidget(LIST_PAGE_NAME, 1, mPage.name, W_LINEEDIT, font);
            mTable->setRowHidden(LIST_PAGE_DESCRIPTION, false);                 // PageDescription
            setTableWidget(LIST_PAGE_DESCRIPTION, 1, mPage.description, W_TEXT, font);
            mTable->setRowHidden(LIST_POPUP_WIDTH, false);                      // PopupWidth
            setTableWidget(LIST_POPUP_WIDTH, 1, mPage.width, W_SPINBOX, font);
            mTable->setRowHidden(LIST_POPUP_HEIGHT, false);                     // PopupHeight
            setTableWidget(LIST_POPUP_HEIGHT, 1, mPage.height, W_SPINBOX, font);
        break;

        default:
            break;
    }
}

int TPropertiesGeneral::setTableWidget(int row, int col, const QVariant& data, ELEMENT_TYPE_t etype, const QFont& font)
{
    DECL_TRACER("TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QVariant& data, ELEMENT_TYPE_t etype, const QFont& font)");

    if (!mTable)
        return 0;

    QWidget *w = mTable->cellWidget(row, col);

    if (!w)
        return 0;

    QSignalBlocker sigBlock(this);

    switch(etype)
    {
        case W_COMBO:
        {
            TElementWidgetCombo *p = static_cast<TElementWidgetCombo *>(w);
            p->selectItem(data);
        }
        break;

        case W_BORDERNAME:
        {
            TElementBorderName *p = static_cast<TElementBorderName *>(w);
            p->setBorder(data.toString());
        }
        break;

        case W_LINEEDIT:
        {
            TElementLineEdit *p = static_cast<TElementLineEdit *>(w);
            p->setText(data.toString());
        }
        break;

        case W_TEXT:
        {
            TElementWidgetText *p = static_cast<TElementWidgetText *>(w);
            p->setText(data.toString());
            p->setFont(font);
        }
        break;

        case W_COLORSELECTOR:
        {
            TElementColorSelector *p = static_cast<TElementColorSelector *>(w);
            p->setColor(data.toString());
        }
        break;

        case W_SPINBOX:
        {
            TElementSpinBox *p = static_cast<TElementSpinBox *>(w);
            p->setValue(data.toInt());
        }
        break;

        default:
            return w->height();
    }

    return w->height();     // Return the height of 1 row.
}

void TPropertiesGeneral::createTable(STATE_TYPE stype)
{
    DECL_TRACER("TPropertiesGeneral::createTable(STATE_TYPE stype)");

    if (!mTable)
        return;

    Q_UNUSED(stype);
    QSignalBlocker sigBlock(this);
    mTable->clear();
    mTable->setColumnCount(2);
    mTable->setRowCount(LIST_MAX_ROWS);

    for (int i = 0; i < LIST_MAX_ROWS; ++i)
    {
        QTableWidgetItem *cell1 = new QTableWidgetItem;
        QTableWidgetItem *cell2 = nullptr;

        cell1->setBackground(Qt::lightGray);

        switch(i)
        {
            case LIST_POPUPTYPE:
                cell1->setText(getLabelText(TTEXT_POPUPTYPE));
                mTable->setCellWidget(i, 1, makePopupType("PopupPopupType"));
            break;

            case LIST_BUTTON_TYPE:
                cell1->setText(getLabelText(TTEXT_TYPE));
                mTable->setCellWidget(i, 1, makeButtonType("ButtonType"));
            break;

            case LIST_PAGE_NAME:
                cell1->setText(getLabelText(TTEXT_NAME));
                mTable->setCellWidget(i, 1, makeObjectName(mPage.name, "PageName"));
            break;

            case LIST_OBJECT_NAME:
            {
                cell1->setText(getLabelText(TTEXT_NAME));
                QString name;

                if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
                    name = mPage.objects[mActObjectID]->getButtonName();

                mTable->setCellWidget(i, 1, makeObjectName(name, "ObjectName"));
            }
            break;

            case LIST_BUTTON_LOCK_NAME:
                cell1->setText(getLabelText(TTEXT_LOCK_BUTTON_NAME));
                mTable->setCellWidget(i, 1, makeButtonLockName("ButtonLockName"));
            break;

            case LIST_PAGE_DESCRIPTION:
                cell1->setText(getLabelText(TTEXT_DESCRIPTION));
                mTable->setCellWidget(i, 1, makeDescription("PageDescription"));
            break;

            case LIST_OBJECT_DESCRIPTION:
                cell1->setText(getLabelText(TTEXT_DESCRIPTION));
                mTable->setCellWidget(i, 1, makeDescription("ObjectDescription"));
            break;

            case LIST_POPUP_LEFT:
                cell1->setText(getLabelText(TTEXT_LEFT));
                mTable->setCellWidget(i, 1, makeSpinGeometry("PopupLeft"));
            break;

            case LIST_OBJECT_LEFT:
                cell1->setText(getLabelText(TTEXT_LEFT));
                mTable->setCellWidget(i, 1, makeSpinGeometry("ObjectLeft"));
            break;

            case LIST_POPUP_TOP:
                cell1->setText(getLabelText(TTEXT_TOP));
                mTable->setCellWidget(i, 1, makeSpinGeometry("PopupTop"));
            break;

            case LIST_OBJECT_TOP:
                cell1->setText(getLabelText(TTEXT_TOP));
                mTable->setCellWidget(i, 1, makeSpinGeometry("ObjectTop"));
            break;

            case LIST_POPUP_WIDTH:
                cell1->setText(getLabelText(TTEXT_WIDTH));
                mTable->setCellWidget(i, 1, makeSpinGeometry("PopupWidth"));
            break;

            case LIST_OBJECT_WIDTH:
                cell1->setText(getLabelText(TTEXT_WIDTH));
                mTable->setCellWidget(i, 1, makeSpinGeometry("ObjectWidth"));
            break;

            case LIST_POPUP_HEIGHT:
                cell1->setText(getLabelText(TTEXT_HEIGHT));
                mTable->setCellWidget(i, 1, makeSpinGeometry("PopupHeight"));
            break;

            case LIST_OBJECT_HEIGHT:
                cell1->setText(getLabelText(TTEXT_HEIGHT));
                mTable->setCellWidget(i, 1, makeSpinGeometry("ObjectHeight"));
            break;

            case LIST_POPUP_RESET_ON_POS:
                cell1->setText(getLabelText(TTEXT_RESET_POS_ON_SHOW));
                mTable->setCellWidget(i, 1, makePopupResetOnPos("PopupResetOnPos"));
            break;

            case LIST_OBJECT_Z_ORDER:
                cell1->setText(getLabelText(TTEXT_ZORDER));
                mTable->setCellWidget(i, 1, makeObjectZOrder("ObjectZOrder"));
            break;

            case LIST_POPUP_GROUPS:
                cell1->setText(getLabelText(TTEXT_GROUP));
                mTable->setCellWidget(i, 1, makePopupGroups("PopupGroups"));
            break;

            case LIST_POPUP_TIMEOUT:
                cell1->setText(getLabelText(TTEXT_TIMEOUT));
                mTable->setCellWidget(i, 1, makePopupTimeout("PopupTimeout"));
            break;

            case LIST_POPUP_MODAL:
                cell1->setText(getLabelText(TTEXT_MODAL));
                mTable->setCellWidget(i, 1, makePopupModal("PopupModal"));
            break;

            case LIST_POPUP_SHOW_EFFECT:
                cell1->setText(getLabelText(TTEXT_SHOW_EFFECT));
                mTable->setCellWidget(i, 1, makePopupShowEffect("PopupShowEffect"));
            break;

            case LIST_POPUP_SHOW_EFFECT_TIME:
                cell1->setText(getLabelText(TTEXT_SHOW_EFFECT_TIME));
                mTable->setCellWidget(i, 1, makePopupEffectTime(mPage.showTime, "PopupShowEffectTime"));
            break;

            case LIST_POPUP_SHOW_EFFECT_X:
                cell1->setText(getLabelText(TTEXT_SHOW_EFFECT_X_POS));
                mTable->setCellWidget(i, 1, makePopupEffectPos(mPage.showEffect, "PopupShowEffectPosX"));
            break;

            case LIST_POPUP_SHOW_EFFECT_Y:
                cell1->setText(getLabelText(TTEXT_SHOW_EFFECT_Y_POS));
                mTable->setCellWidget(i, 1, makePopupEffectPos(mPage.showEffect, "PopupShowEffectPosY"));
            break;

            case LIST_POPUP_HIDE_EFFECT:
                cell1->setText(getLabelText(TTEXT_HIDE_EFFECT));
                mTable->setCellWidget(i, 1, makePopupHideEffect("PopupHideEffect"));
            break;

            case LIST_POPUP_HIDE_EFFECT_TIME:
                cell1->setText(getLabelText(TTEXT_HIDE_EFFECT_TIME));
                mTable->setCellWidget(i, 1, makePopupEffectTime(mPage.hideTime, "PopupHideEffectTime"));
            break;

            case LIST_POPUP_HIDE_EFFECT_X:
                cell1->setText(getLabelText(TTEXT_HIDE_EFFECT_X_POS));
                mTable->setCellWidget(i, 1, makePopupEffectPos(mPage.hideEffect, "PopupHideEffectPosX"));
            break;

            case LIST_POPUP_HIDE_EFFECT_Y:
                cell1->setText(getLabelText(TTEXT_HIDE_EFFECT_Y_POS));
                mTable->setCellWidget(i, 1, makePopupEffectPos(mPage.hideEffect, "PopupHideEffectPosY"));
            break;

            case LIST_POPUP_COLLAPSE_DIR:
                cell1->setText(getLabelText(TTEXT_COLLAPSE_DIRECTION));
                mTable->setCellWidget(i, 1, makePopupCollapseDir("PopupCollapseDirection"));
            break;

            case LIST_POPUP_COLLAPSE_OFFSET:
                cell1->setText(getLabelText(TTEXT_COLLAPSE_OFFSET));
                mTable->setCellWidget(i, 1, makePopupCollapseOffset("PopupCollapseOffest"));
            break;

            case LIST_POPUP_COLLAPSE_SHOWOP:
                cell1->setText(getLabelText(TTEXT_COLLAPSE_SHOW_OPEN));
                mTable->setCellWidget(i, 1, makePopupCollapseShowOpen("PopupCollapseShowOpen"));
            break;

            case LIST_OBJECT_DRAG_DROP_TYPE:
                cell1->setText(getLabelText(TTEXT_DRAGDROP_TYPE));
                mTable->setCellWidget(i, 1, makeObjectDragDropType("ObjectDragDropType"));
            break;

            case LIST_OBJECT_DROP_GROUP:
                cell1->setText(getLabelText(TTEXT_DROP_GROUP));
                mTable->setCellWidget(i, 1, makeObjectDropGroup("ObjectDropGroup"));
            break;

            case LIST_OBJECT_TOUCH_STYLE:
                cell1->setText(getLabelText(TTEXT_TOUCH_STYLE));
                mTable->setCellWidget(i, 1, makeObjectTouchStyle("ObjectTouchStyle"));
            break;

            case LIST_OBJECT_BORDER_STYLE:
                cell1->setText(getLabelText(TTEXT_BORDER_STYLE));
                mTable->setCellWidget(i, 1, makeObjectBorderStyle("ObjectBorderStyle"));
            break;

            case LIST_OBJECT_STATE_COUNT:
                cell1->setText(getLabelText(TTEXT_STATE_COUNT));
                mTable->setCellWidget(i, 1, makeObjectStateCount("ObjectStateCount"));
            break;

            case LIST_OBJECT_ANIMATE_TIME_UP:
                cell1->setText(getLabelText(TTEXT_ANIMATE_TIME_UP));
                mTable->setCellWidget(i, 1, makeObjectAnimateTime("ObjectAnimateTimeUp"));
            break;

            case LIST_OBJECT_ANIMATE_TIME_DN:
                cell1->setText(getLabelText(TTEXT_ANIMATE_TIME_DOWN));
                mTable->setCellWidget(i, 1, makeObjectAnimateTime("ObjectAnimateTimeDown"));
            break;

            case LIST_OBJECT_AUTOREPEAT:
                cell1->setText(getLabelText(TTEXT_AUTO_REPEAT));
                mTable->setCellWidget(i, 1, makeObjectAutoRepeat("ObjectAutoRepeat"));
            break;

            case LIST_OBJECT_DISABLED:
                cell1->setText(getLabelText(TTEXT_DISABLED));
                mTable->setCellWidget(i, 1, makeObjectYesNoSelect("ObjectDisabled"));
            break;

            case LIST_OBJECT_HIDDEN:
                cell1->setText(getLabelText(TTEXT_HIDDEN));
                mTable->setCellWidget(i, 1, makeObjectYesNoSelect("ObjectHidden"));
            break;

            case LIST_OBJECT_VALUE_DIRECTION:
                cell1->setText(getLabelText(TTEXT_VALUE_DIRECTION));
                mTable->setCellWidget(i, 1, makeObjectValueDirection("ObjectValueDirection"));
            break;

            case LIST_OBJECT_SLIDER_NAME:
                cell1->setText(getLabelText(TTEXT_SLIDER_NAME));
                mTable->setCellWidget(i, 1, makeObjectSliderName("ObjectSliderName"));
                break;

            case LIST_OBJECT_SLIDER_COLOR:
                cell1->setText(getLabelText(TTEXT_SLIDER_COLOR));
                mTable->setCellWidget(i, 1, makeObjectSliderColor("ObjectSliderColor"));
            break;

            case LIST_OBJECT_PASSWD_PROTECT:
                cell1->setText(getLabelText(TTEXT_PASSWORD_PROTECTION));
                mTable->setCellWidget(i, 1, makeObjectPasswordProtected("ObjectPasswordProtection"));
            break;

            case LIST_OBJECT_SUB_PAGE_SET:
                cell1->setText(getLabelText(TTEXT_SUBPAGE_SET));
                mTable->setCellWidget(i, 1, makeObjectSubPageSet("ObjectSubPageSet"));
            break;

            case LIST_OBJECT_ORIENTATION:
                cell1->setText(getLabelText(TTEXT_ORIENTATION));
                mTable->setCellWidget(i, 1, makeObjectOrientation("ObjectOrientation"));
            break;

            case LIST_OBJECT_SPACING:
                cell1->setText(getLabelText(TTEXT_SPACING));
                mTable->setCellWidget(i, 1, makeObjectSpacing("ObjectSpacing"));
            break;

            case LIST_OBJECT_ANCHOR_POSITION:
                cell1->setText(getLabelText(TTEXT_ANCHOR_POSITION));
                mTable->setCellWidget(i, 1, makeObjectAnchorPosition("ObjectAnchorPosition"));
            break;

            case LIST_OBJECT_SHOW_SUBPAGES:
                cell1->setText(getLabelText(TTEXT_SHOW_SUBPAGES));
                mTable->setCellWidget(i, 1, makeObjectYesNoSelect("ObjectShowSubpages"));
            break;

            case LIST_OBJECT_DYNAMIC_REORDER:
                cell1->setText(getLabelText(TTEXT_ALLOW_DYN_REORDERING));
                mTable->setCellWidget(i, 1, makeObjectYesNoSelect("ObjectDynamicReorder"));
            break;

            case LIST_OBJECT_RESET_VIEW_SHOW:
                cell1->setText(getLabelText(TTEXT_RESET_VIEW_ON_SHOW));
                mTable->setCellWidget(i, 1, makeObjectYesNoSelect("ObjectResetViewOnShow"));
            break;

            case LIST_OBJECT_SCROLLBAR:
                cell1->setText(getLabelText(TTEXT_SCROLLBAR));
                mTable->setCellWidget(i, 1, makeObjectYesNoSelect("ObjectScrollbar"));
            break;

            case LIST_OBJECT_SCROLL_OFFSET:
                cell1->setText(getLabelText(TTEXT_SCROLLBAR_OFFSET));
                mTable->setCellWidget(i, 1, makeObjectScrollbarOffset("ObjectScrollbarOffset"));
            break;

            case LIST_OBJECT_DISABLE_TOUCHSC:
                cell1->setText(getLabelText(TTEXT_DISABLE_TOUCH_SCROLL));
                mTable->setCellWidget(i, 1, makeObjectYesNoSelect("ObjectDisableTouchScroll"));
            break;

            case LIST_OBJECT_ENABLE_ANCHOR:
                cell1->setText(getLabelText(TTEXT_ENABLE_ANCHORING));
                mTable->setCellWidget(i, 1, makeObjectYesNoSelect("ObjectEnableAnchor"));
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

TElementWidgetCombo *TPropertiesGeneral::makePopupType(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makePopupType(const QString& name)");

    QStringList items = { "Standard", "Subpage" };
    QList<QVariant> data = { Page::PT_POPUP, Page::PT_SUBPAGE };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);
    combo->setCurrentIndex(mPage.popupType == Page::PT_POPUP ? 0 : 1);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onComboPopupTypeChanged);
    return combo;
}

TElementWidgetCombo *TPropertiesGeneral::makeButtonType(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeButtonType(const QString& name)");

    QStringList items = { "general", "multi-state general", "bargraph",
                          "multi-state bargraph", "text input",
                          "sub-page view", "listview" };
    QList<QVariant> data = { GENERAL, MULTISTATE_GENERAL, BARGRAPH,
                             MULTISTATE_BARGRAPH, TEXT_INPUT, SUBPAGE_VIEW, LISTVIEW };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size() && mActObject.bi > 0)
        combo->setCurrentIndex(TObjectHandler::getButtonTypeIndex(mActObject.type));
    else
        combo->setCurrentIndex(0);

    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onComboButtonTypeChanged);
    return combo;
}

TElementLineEdit *TPropertiesGeneral::makeObjectName(const QString& text, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectName(const QString& text, const QString& name)");

    TElementLineEdit *le = new TElementLineEdit(text, name, mTable);
    connect(le, &TElementLineEdit::inputTextChanged, this, &TPropertiesGeneral::onObjectNameChanged);
    return le;
}

TElementWidgetCombo *TPropertiesGeneral::makeButtonLockName(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeButtonLockName(const QString& name)");

    QStringList items;
    QList<QVariant> data;
    initYesNo(items, data);

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size() && mActObject.bi > 0)
        combo->setCurrentIndex(mActObject.li ? 1 : 0);

    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onComboLockButtonName);
    return combo;
}

TElementWidgetText *TPropertiesGeneral::makeDescription(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeDescription(const QString& name)");

    QString text;

    if (name == "PageDescription")
        text = mPage.description;
    else
    {
        if (mActObject.bi == (mActObjectID + 1))
            text = mActObject.bd;
        else // We have no valid object. Maybe we can load it
        {
            if (mActObjectID >= 0 && mPage.objects.size() > mActObjectID)
                text = mPage.objects[mActObjectID]->getObject().bd;
        }
    }

    TElementWidgetText *wt = new TElementWidgetText(text, name, mTable);
    connect(wt, &TElementWidgetText::textChanged, this, &TPropertiesGeneral::onDescriptionChanged);
    return wt;
}

TElementSpinBox *TPropertiesGeneral::makeSpinGeometry(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeSpinGeometry(const QString& name)");

    int value = 0;

    if (name.startsWith("Popup"))
    {
        if (name.endsWith("Left"))
            value = mPage.left;
        else if (name.endsWith("Top"))
            value = mPage.top;
        else if (name.endsWith("Width"))
            value = mPage.width;
        else if (name.endsWith("Height"))
            value = mPage.height;
    }
    else
    {
        if (mActObjectID >= 0 && mPage.objects.size() > mActObjectID)
        {
            mActObject = mPage.objects[mActObjectID]->getObject();

            if (name.endsWith("Left"))
                value = mActObject.lt;
            else if (name.endsWith("Top"))
                value = mActObject.tp;
            else if (name.endsWith("Width"))
                value = mActObject.wt;
            else if (name.endsWith("Height"))
                value = mActObject.ht;
        }
        else
        {
            MSG_WARNING("No object with index " << mActObjectID << " found!");
        }
    }

    TElementSpinBox *sbox = new TElementSpinBox(value, 0, 10000, name, mTable);
    connect(sbox, &TElementSpinBox::valueChanged, this, &TPropertiesGeneral::onSpinGeometryChanged);
    return sbox;
}

TElementWidgetCombo *TPropertiesGeneral::makePopupResetOnPos(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makePopupResetOnPos(const QString& name)");

    QStringList items;
    QList<QVariant> data;
    initYesNo(items, data);

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);
    combo->setCurrentIndex(mPage.resetPos);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onComboLockButtonName);
    return combo;
}

TElementLineEdit *TPropertiesGeneral::makeObjectZOrder(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectZOrder(const QString& name)");

    QString value("0");

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();

        if (mActObject.zo > 0)
            value = QString("%1").arg(mActObject.zo);
        else
            value = QString("%1").arg(mActObject.bi);
    }

    TElementLineEdit *le = new TElementLineEdit(value, name, mTable);
    le->setDisabled(true);
    return le;
}

TElementWidgetCombo *TPropertiesGeneral::makePopupGroups(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makePopupGroups(const QString& name)");

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->setEditable(true);
    QStringList groups = TPageHandler::Current().getGroupNames();
    groups.insert(0, "none");
    combo->addItems(groups);
    combo->setCurrentText(mPage.group);

    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onComboPopupGroupChanged);
    return combo;
}

TElementSpinBox *TPropertiesGeneral::makePopupTimeout(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makePopupTimeout(const QString& name)");

    TElementSpinBox *sb = new TElementSpinBox(mPage.timeout, 0, 10000, name);
    connect(sb, &TElementSpinBox::valueChanged, this, &TPropertiesGeneral::onSpinTimeoutValue);
    return sb;
}

TElementWidgetCombo *TPropertiesGeneral::makePopupModal(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makePopupModal(const QString& name)");

    QStringList items;
    QList<QVariant> data;
    initYesNo(items, data);

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);
    combo->setCurrentIndex(mPage.modal);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onPopupModalChanged);
    return combo;
}

TElementWidgetCombo *TPropertiesGeneral::makePopupShowEffect(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makePopupShowEffect(const QString& name)");

    QStringList items = { tr("None"), tr("Fade"), tr("Slide from left"), tr("Slide from right"),
                          tr("Slide from top"), tr("Slide from bottom"), tr("Slide from left fade"),
                         tr("Slide from right fade"), tr("Slide from top fade"), tr("Slide from bottom fade") };
    QList<QVariant> data = { Page::SE_NONE, Page::SE_FADE, Page::SE_SLIDE_LEFT,
                             Page::SE_SLIDE_RIGHT, Page::SE_SLIDE_TOP, Page::SE_SLIDE_BOTTOM,
                             Page::SE_SLIDE_LEFT_FADE, Page::SE_SLIDE_RIGHT_FADE,
                             Page::SE_SLIDE_TOP_FADE, Page::SE_SLIDE_BOTTOM_FADE };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);
    combo->setCurrentIndex(mPage.showEffect);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onPopupShowEffectChanged);
    return combo;
}

TElementWidgetCombo *TPropertiesGeneral::makePopupHideEffect(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makePopupShowEffect(const QString& name)");

    QStringList items = { tr("None"), tr("Fade"), tr("Slide to left"), tr("Slide to right"),
                         tr("Slide to top"), tr("Slide to bottom"), tr("Slide to left fade"),
                         tr("Slide to right fade"), tr("Slide to top fade"), tr("Slide to bottom fade") };
    QList<QVariant> data = { Page::SE_NONE, Page::SE_FADE, Page::SE_SLIDE_LEFT,
                            Page::SE_SLIDE_RIGHT, Page::SE_SLIDE_TOP, Page::SE_SLIDE_BOTTOM,
                            Page::SE_SLIDE_LEFT_FADE, Page::SE_SLIDE_RIGHT_FADE,
                            Page::SE_SLIDE_TOP_FADE, Page::SE_SLIDE_BOTTOM_FADE };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);
    combo->setCurrentIndex(mPage.hideEffect);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onPopupHideEffectChanged);
    return combo;
}

TElementWidgetCombo *TPropertiesGeneral::makePopupCollapseDir(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makePopupCollapseDir(const QString& name)");

    QStringList items = { tr("None"), tr("Left"), tr("Right"), tr("Up"), tr("Down") };
    QList<QVariant> data = { Page::COLDIR_NONE, Page::COLDIR_LEFT, Page::COLDIR_RIGHT,
                             Page::COLDIR_UP, Page::COLDIR_DOWN };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onPopupCollapseDirChanged);
    return combo;
}

TElementSpinBox *TPropertiesGeneral::makePopupEffectTime(int t, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makePopupEffectTime(int t, const QString& name)");

    TElementSpinBox *sbox = new TElementSpinBox(t, 0, 10000, name, mTable);
    connect(sbox, &TElementSpinBox::valueChanged, this, &TPropertiesGeneral::onSpinPopupEffectTimeChnaged);
    return sbox;
}

TElementWidgetCombo *TPropertiesGeneral::makePopupEffectPos(Page::SHOWEFFECT effect, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makePopupEffectPos(Page::SHOWEFFECT effect, const QString& name)");

    Q_UNUSED(effect);

    int start = 0;
    QStringList items;
    QList<QVariant> data;

    if (name == "PopupShowEffectPosX")
    {
        start = mPage.showX <= 0 ? mPage.width : mPage.showX;
        items = { "screen left", "screen right" };
        data = { 0, start };
    }
    else if (name == "PopupShowEffectPosY")
    {
        start = mPage.showY <= 0 ? mPage.height : mPage.showY;
        items = { "screen top", "screen bottom" };
        data = { 0, start };
    }
    else if (name == "PopupHideEffectPosX")
    {
        start = mPage.hideX <= 0 ? mPage.width : mPage.hideX;
        items = { "screen left", "screen right" };
        data = { 0, start };
    }
    else if (name == "PopupHideEffectPosY")
    {
        start = mPage.hideY <= 0 ? mPage.height : mPage.hideY;
        items = { "screen top", "screen bottom" };
        data = { 0, start };
    }

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->setEditable(true);
    combo->setInputMethodHints(Qt::ImhDigitsOnly);
    combo->addItems(items);
    combo->addData(data);
    combo->setCurrentText(QString("%1").arg(start));
    combo->setCurrentIndex(start > 0 ? 1 : 0);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onComboPopupEffectPos);
    return combo;
}

TElementSpinBox *TPropertiesGeneral::makePopupCollapseOffset(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makePopupCollapseOffset(const QString& name)");

    TElementSpinBox *spin = new TElementSpinBox(mPage.collapseOffset, 0, 10000, name, mTable);
    connect(spin, &TElementSpinBox::valueChanged, this, &TPropertiesGeneral::onSpinCollapseOffset);
    return spin;
}

TElementWidgetCombo *TPropertiesGeneral::makePopupCollapseShowOpen(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makePopupCollapseShowOpen(const QString& name)");

    QStringList items;
    QList<QVariant> data;
    initYesNo(items, data);

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);
    combo->setCurrentIndex(mPage.collapseShowOpen ? 1 : 0);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onComboCollapseShowOpen);
    return combo;
}

TElementWidgetCombo *TPropertiesGeneral::makeObjectDragDropType(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectDragDropType(const QString& name)");

    QStringList items = { "none", "draggable", "drop target" };
    QList<QVariant> data = { "none", "dr", "dt" };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();
        int idx = 0;

        if (mActObject.ddt == "dr")
            idx = 1;
        else if (mActObject.ddt == "dt")
            idx = 2;

        combo->setCurrentIndex(idx);
    }

    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onComboObjectDragDropType);
    return combo;
}

TElementWidgetCombo *TPropertiesGeneral::makeObjectDropGroup(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectDropGroup(const QString& name)");

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->setItemText(0, "none");
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onComboObjectDropGroup);
    return combo;
}

TElementWidgetCombo *TPropertiesGeneral::makeObjectTouchStyle(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectTouchStyle(const QString& name)");

    QStringList items = { "active touch", "bounding box", "pass through" };
    QList<QVariant> data = { "none", "bounding", "passThru" };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();
        int idx = 0;

        if (mActObject.hs == "bounding")
            idx = 1;
        else if (mActObject.hs == "passThru")
            idx = 2;

        combo->setCurrentIndex(idx);
    }

    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onComboObjectTouchStyle);
    return combo;
}

TElementBorderName *TPropertiesGeneral::makeObjectBorderStyle(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectBorderStyle(const QString& name)");

    QString border;

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();
        border = mActObject.bs;
    }

    TElementBorderName *bn = new TElementBorderName(border, name, mTable);
    connect(bn, &TElementBorderName::borderChanged, this, &TPropertiesGeneral::onObjectBorderStyle);
    return bn;
}

TElementWidgetCombo *TPropertiesGeneral::makeObjectYesNoSelect(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectYesNoSelect(const QString& name)");

    QStringList items;
    QList<QVariant> data;
    initYesNo(items, data);

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();

        if (name == "ObjectDisabled")
            combo->setCurrentIndex(mActObject.da);
        else if (name == "ObjectHidden")
            combo->setCurrentIndex(mActObject.hd);
    }

    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onObjectYesNoSelection);
    return combo;
}

TElementWidgetCombo *TPropertiesGeneral::makeObjectPasswordProtected(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectPasswordProtected(const QString& name)");

    QStringList items = { "none", "password 1", "password 2", "password 3", "password 4" };
    QList<QVariant> data = { 0, 1, 2, 3, 4 };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();
        combo->setCurrentIndex(mActObject.pp);
    }

    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onObjectPasswordProtection);
    return combo;
}

TElementSpinBox *TPropertiesGeneral::makeObjectStateCount(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectStateCount(const QString& name)");

    int value = 2;

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();
        value = mActObject.stateCount;
    }

    TElementSpinBox *sbox = new TElementSpinBox(value, 2, 256, name, mTable);
    connect(sbox, &TElementSpinBox::valueChanged, this, &TPropertiesGeneral::onObjectStateCount);
    return sbox;
}

TElementSpinBox *TPropertiesGeneral::makeObjectAnimateTime(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectAnimateTime(const QString& name)");

    int value = 2;

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();

        if (name == "ObjectAnimateTimeUp")
            value = mActObject.nu;
        else if (name == "ObjectAnimateTimeDown")
            value = mActObject.nd;
    }

    TElementSpinBox *sbox = new TElementSpinBox(value, 0, 10000, name, mTable);
    connect(sbox, &TElementSpinBox::valueChanged, this, &TPropertiesGeneral::onObjectStateCount);
    return sbox;
}

TElementWidgetCombo *TPropertiesGeneral::makeObjectAutoRepeat(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectAutoRepeat(const QString& name)");

    QStringList items;
    QList<QVariant> data;
    initYesNo(items, data);

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();
        combo->setCurrentIndex(mActObject.ar);
    }

    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onObjectYesNoSelection);
    return combo;
}

TElementWidgetCombo *TPropertiesGeneral::makeObjectValueDirection(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectValueDirection(const QString& name)");

    QStringList items = { tr("vertical"), tr("horizontal") };
    QList<QVariant> data = { "vertical", "horizontal" };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();
        int index = mActObject.dr == "vertical" || mActObject.dr.isEmpty() ? 0 : 1;
        combo->setCurrentIndex(index);
    }

    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onObjectValueDirection);
    return combo;
}

TElementWidgetCombo *TPropertiesGeneral::makeObjectSliderName(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectSliderName(const QString& name)");

    QStringList items = TGraphics::Current().getSliderNames();
    items.insert(0, "none");
    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();

        if (!mActObject.sd.isEmpty() && mActObject.sd != "none")
            combo->setCurrentText(mActObject.sd);
    }

    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onObjectSliderName);
    return combo;
}

TElementColorSelector *TPropertiesGeneral::makeObjectSliderColor(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectSliderColor(const QString& name)");

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
        mActObject = mPage.objects[mActObjectID]->getObject();

    TElementColorSelector *colsel = new TElementColorSelector(mActObject.sc, name, mTable);
    connect(colsel, &TElementColorSelector::colorChanged, this, &TPropertiesGeneral::onObjectSliderColor);
    return colsel;
}

TElementWidgetCombo *TPropertiesGeneral::makeObjectSubPageSet(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectSubPageSet(const QString& name)");

    QStringList items = TConfMain::Current().getSubPageSets();
    items.insert(0, "none");
    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);

    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onObjectSubPageSet);
    return combo;
}

TElementWidgetCombo *TPropertiesGeneral::makeObjectOrientation(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectOrientation(const QString& name)");

    QStringList items = { "horizontal", "vertical" };
    QList<QVariant> states = { 0, 1 };
    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(states);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onObjectOrientation);
    return combo;
}

TElementSpinBox *TPropertiesGeneral::makeObjectSpacing(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectSpacing(const QString& name)");

    int value = 5;

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();
        value = mActObject.sa;
    }

    TElementSpinBox *sbox = new TElementSpinBox(value, 0, 100, name, mTable);
    connect(sbox, &TElementSpinBox::valueChanged, this, &TPropertiesGeneral::onObjectSpacing);
    return sbox;
}

TElementWidgetCombo *TPropertiesGeneral::makeObjectAnchorPosition(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectAnchorPosition(const QString& name)");

    QStringList items = { "left", "middle", "right" };
    QList<QVariant> states = { "l/t", "", "r/b"};

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(states);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesGeneral::onObjectAnchorPosition);
    return combo;
}

TElementSpinBox *TPropertiesGeneral::makeObjectScrollbarOffset(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makeObjectScrollbarOffset(const QString& name)");

    int value = 0;

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();
        value = mActObject.bo;
    }

    TElementSpinBox *sbox = new TElementSpinBox(value, 0, 10000, name, mTable);
    connect(sbox, &TElementSpinBox::valueChanged, this, &TPropertiesGeneral::onObjectScrollbarOffset);
    return sbox;
}

void TPropertiesGeneral::setWidget(QTableWidget *view)
{
    DECL_TRACER("TPropertiesGeneral::setWidget(QTableWidget *view)");

    if (mTable && mConnected && mTable != view)
    {
        disconnect(view, &QTableWidget::cellChanged, this, &TPropertiesGeneral::onCellChanged);
        disconnect(view, &QTableWidget::cellActivated, this, &TPropertiesGeneral::onCellActivated);
        mConnected = false;
    }

    mTable = view;

    if (view && !mConnected)
    {
        connect(view, &QTableWidget::cellChanged, this, &TPropertiesGeneral::onCellChanged);
        connect(view, &QTableWidget::cellActivated, this, &TPropertiesGeneral::onCellActivated);
        mConnected = true;
    }
}

void TPropertiesGeneral::onCellChanged(int row, int column)
{
    DECL_TRACER("TPropertiesGeneral::onCellChanged(int row, int column)");

    if (!mInitialized)
        return;

    mCellActive.row = row;
    mCellActive.col = column;
    MSG_DEBUG("Cell changed: row: " << row << ", col: " << column);

    if (column != 1)
        return;

    if (mPage.popupType == Page::PT_PAGE)
    {
        QTableWidgetItem *item = mTable->item(row, column);

        if (item && item->data(Qt::UserRole).toInt() == 0)  // Page name
        {
            mPage.name = item->text();
            pageNameChanged(mPage.pageID, mPage.name);
            markChanged();
            mChanged = true;
        }
    }
    else if (mPage.popupType == Page::PT_POPUP)
    {
        QTableWidgetItem *item = mTable->item(row, column);

        if (!item)
            return;

        if (row ==  1) // Name of poup
        {
            mPage.name = item->text();
            pageNameChanged(mPage.pageID, mPage.name);
            markChanged();
            mChanged = true;
        }
    }
}

void TPropertiesGeneral::onCellActivated(int row, int column)
{
    DECL_TRACER("TPropertiesGeneral::onCellActivated(int row, int column)");

    if (!mInitialized)
        return;

    mCellActive.row = row;
    mCellActive.col = column;
    MSG_DEBUG("Cell activated: row: " << row << ", col: " << column);

    if (column != 1)
        return;
}

void TPropertiesGeneral::onComboPopupTypeChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onComboPopupTypeChanged(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    int type = data.toInt();

    if (type == Page::PT_POPUP)
        mPage.popupType = Page::PT_POPUP;
    else
        mPage.popupType = Page::PT_SUBPAGE;

    mChanged = true;
    setTable(STATE_POPUP);
    // TODO: Add code to move popup to subpages, if this was selected and vice versa.
    pageTypeChanged(mPage.popupType, mPage.pageID);
}

void TPropertiesGeneral::onComboButtonTypeChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onComboButtonTypeChanged(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    if (mActObjectID < 0 || mPage.objects.size() <= mActObjectID)
        return;

    mActObject = mPage.objects[mActObjectID]->getObject();
    BUTTONTYPE oldType = mActObject.type;
    mActObject.type = static_cast<BUTTONTYPE>(data.toInt());

    if (mActObject.type == BARGRAPH && mActObject.type != oldType)
    {
        if (mActObject.sr[0].cf == mActObject.sr[1].cf)
        {
            mActObject.sr[0].cf = qRgb(0xff, 0xa1, 0);
            mActObject.sr[1].cf = qRgb(0x97, 0xbe, 0x0d);
        }
    }

    mPage.objects[mActObjectID]->setObject(mActObject);
    setTable(STATE_BUTTON);
    mChanged = true;
    markChanged();
    // TODO: Add code to change the view of the object accordingly
    objectTypeChanged(mActObject.type, mActObjectID);
}

void TPropertiesGeneral::onComboLockButtonName(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onComboLockButtonName(const QString& text, const QVariant& data, const QString& name)");

    if (mActObjectID < 0 || mPage.objects.size() <= mActObjectID)
        return;

    Q_UNUSED(text);
    Q_UNUSED(name);

    mActObject = mPage.objects[mActObjectID]->getObject();
    mActObject.li = data.toBool();
    mPage.objects[mActObjectID]->setObject(mActObject);
    mChanged = true;
    markChanged();
}

void TPropertiesGeneral::onObjectNameChanged(const QString& text, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectNameChanged(const QString& text, const QString& name)");

    if (name.startsWith("Page"))
    {
        mPage.name = text;
        pageNameChanged(mPage.pageID, text);
    }
    else
    {
        if (mActObjectID < 0 || mPage.objects.size() <= mActObjectID)
            return;

        mActObject = mPage.objects[mActObjectID]->getObject();
        mActObject.na = text;
        mPage.objects[mActObjectID]->setObject(mActObject);
    }

    mChanged = true;
    markChanged();
}

void TPropertiesGeneral::onDescriptionChanged(const QString& text, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onDescriptionChanged(const QString& text, const QString& name)");

    if (name.startsWith("Page"))
        mPage.description = text;
    else
    {
        if (mActObjectID < 0 || mPage.objects.size() <= mActObjectID)
            return;

        mActObject = mPage.objects[mActObjectID]->getObject();
        mActObject.bd = text;
        mPage.objects[mActObjectID]->setObject(mActObject);
    }

    mChanged = true;
    markChanged();
}

void TPropertiesGeneral::onSpinGeometryChanged(int value, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onSpinGeometryChanged(int value, const QString& name)");

    if (name.startsWith("Popup"))
    {
        if (name.endsWith("Left"))
            mPage.left = value;
        else if (name.endsWith("Top"))
            mPage.top = value;
        else if (name.endsWith("Width"))
            mPage.width = value;
        else if (name.endsWith("Height"))
            mPage.height = value;

        if (!mSignalBlocked)
            setPosition(QRect(mPage.left, mPage.top, mPage.width, mPage.height), mPage, 0, STATE_POPUP);
    }
    else
    {
        if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
        {
            if (name.endsWith("Left"))
                mActObject.lt = value;
            else if (name.endsWith("Top"))
                mActObject.tp = value;
            else if (name.endsWith("Width"))
                mActObject.wt = value;
            else if (name.endsWith("Height"))
                mActObject.ht = value;

            mPage.objects[mActObjectID]->setObject(mActObject);

            if (!mSignalBlocked)
                setPosition(QRect(mActObject.lt, mActObject.tp, mActObject.wt, mActObject.ht), mPage, mActObject.bi, STATE_BUTTON);
        }
    }

    mChanged = true;
    requestRedraw(&mPage);
}

void TPropertiesGeneral::onComboPopupGroupChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onComboPopupGroupChanged(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(data);
    Q_UNUSED(name);

    if (text.isEmpty() || text == "none")
        mPage.group = QString();
    else
        mPage.group = text;

    mChanged = true;
    saveChangedData(&mPage, TBL_GENERAL);
    // TODO: Add code to change group in page tree widget
}

void TPropertiesGeneral::onPopupTimeout(int value, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onPopupTimeout(int value, const QString& name)");

    Q_UNUSED(name);

    mPage.timeout = value;
    mChanged = true;
    saveChangedData(&mPage, TBL_GENERAL);
}

void TPropertiesGeneral::onPopupModalChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onPopupModalChanged(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    mPage.modal = (data.toBool() ? 1 : 0);
    mChanged = true;
    saveChangedData(&mPage, TBL_GENERAL);
}

void TPropertiesGeneral::onPopupShowEffectChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onPopupShowEffectChanged(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    mPage.showEffect = static_cast<Page::SHOWEFFECT>(data.toInt());
    mChanged = true;
    saveChangedData(&mPage, TBL_GENERAL);
    setTable(STATE_POPUP);
}

void TPropertiesGeneral::onPopupHideEffectChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onPopupHideEffectChanged(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    mPage.hideEffect = static_cast<Page::SHOWEFFECT>(data.toInt());
    mChanged = true;
    saveChangedData(&mPage, TBL_GENERAL);
    setTable(STATE_POPUP);
}

void TPropertiesGeneral::onPopupCollapseDirChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onPopupCollapseDirChanged(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    mPage.collapseDirection = static_cast<Page::COLDIR_t>(data.toInt());
    mChanged = true;
    saveChangedData(&mPage, TBL_GENERAL);
    setTable(STATE_POPUP);
}

void TPropertiesGeneral::onSpinTimeoutValue(int value, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onSpinTimeoutValue(int value, const QStringName)");

    Q_UNUSED(name);

    mPage.timeout = value;
    saveChangedData(&mPage, TBL_GENERAL);
    mChanged = true;
}

void TPropertiesGeneral::onSpinPopupEffectTimeChnaged(int value, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onSpinPopupEffectTimeChnaged(int value, const QString& name)");

    if (name.endsWith("ShowEffectTime"))
        mPage.showTime = value;
    else
        mPage.hideTime = value;

    saveChangedData(&mPage, TBL_GENERAL);
    mChanged = true;
}

void TPropertiesGeneral::onComboPopupEffectPos(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onComboPopupEffectPos(const QString& text, const QVariant& data, const QString& name)");

    bool ok = false;
    int num = text.toInt(&ok);

    if (!ok)
        num = data.toInt();

    if (name == "PopupShowEffectPosX")
        mPage.showX = num;
    else if (name == "PopupShowEffectPosY")
        mPage.showY = num;
    else if (name == "PopupHideEffectPosX")
        mPage.hideX = num;
    else if (name == "PopupHideEffectPosY")
        mPage.hideY = num;

    saveChangedData(&mPage, TBL_GENERAL);
    mChanged = true;
}

void TPropertiesGeneral::onSpinCollapseOffset(int value, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onSpinCollapseOffset(int value, const QString& name)");

    Q_UNUSED(name);

    mPage.collapseOffset = value;
    saveChangedData(&mPage, TBL_GENERAL);
    mChanged = true;
}

void TPropertiesGeneral::onComboCollapseShowOpen(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onComboCollapseShowOpen(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    mPage.collapseShowOpen = data.toBool();
    saveChangedData(&mPage, TBL_GENERAL);
    mChanged = true;
}

void TPropertiesGeneral::onComboObjectDragDropType(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onComboObjectDragDropType(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    if (data.toString() == "none")
        mActObject.ddt.clear();
    else
        mActObject.ddt = data.toString();

    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
    setTable(STATE_BUTTON);
}

void TPropertiesGeneral::onComboObjectDropGroup(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onComboObjectDropGroup(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(data);
    Q_UNUSED(name);
    // TODO: Find out what it is for.
}

void TPropertiesGeneral::onComboObjectTouchStyle(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onComboObjectTouchStyle(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    QString val = data.toString();

    if (val == "none")
        mActObject.hs.clear();
    else
        mActObject.hs = val;

    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
}

void TPropertiesGeneral::onObjectBorderStyle(const QString& border, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectBorderStyle(const QString& border, const QString& name)");

    Q_UNUSED(name);

    if (border == "none")
        mActObject.bs.clear();
    else
        mActObject.bs = border;

    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
    requestRedraw(&mPage);
}

void TPropertiesGeneral::onObjectYesNoSelection(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectYesNoSelection(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);

    if (name == "ObjectDisabled")
        mActObject.da = data.toBool() ? 1 : 0;
    else if (name == "ObjectHidden")
        mActObject.hd = data.toBool() ? 1 : 0;
    else if (name == "ObjectAutoRepeat")
        mActObject.ar = data.toBool() ? 1 : 0;
    else if (name == "ObjectShowSubpages")
        mActObject.sw = data.toInt();
    else if (name == "ObjectDynamicReorder")
        mActObject.dy = data.toInt();
    else if (name == "ObjectResetViewOnShow")
        mActObject.rs = data.toInt();
    else if (name == "ObjectScrollbar")
        mActObject.ba = data.toInt();
    else if (name == "ObjectDisableTouchScroll")
        mActObject.ds = data.toInt();
    else if (name == "ObjectEnableAnchor")
        mActObject.sdd = data.toInt();

    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
    requestRedraw(&mPage);
    update();
}


void TPropertiesGeneral::onObjectPasswordProtection(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectPasswordProtection(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    mActObject.pp = data.toInt();
    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
}

void TPropertiesGeneral::onObjectStateCount(int value, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectStateCount(int value, const QString& name)");

    Q_UNUSED(name);

    mActObject.stateCount = value;
    mActObject.rm = value;          // Yes, this exists twice!
    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
}

void TPropertiesGeneral::onObjectAnimateTime(int value, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectAnimateTime(int value, const QString& name)");

    if (name == "ObjectAnimateTimeUp")
    {
        mActObject.nu = value;
        mActObject.ru = value;
    }
    else if (name == "ObjectAnimateTimeDown")
    {
        mActObject.nd = value;
        mActObject.rd = value;
    }

    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
}

void TPropertiesGeneral::onObjectValueDirection(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectValueDirection(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    mActObject.dr = data.toString();
    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
}

void TPropertiesGeneral::onObjectSliderName(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectSliderName(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(data);
    Q_UNUSED(name);

    if (text == "none")
        mActObject.sd.clear();
    else
        mActObject.sd = text;

    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
}

void TPropertiesGeneral::onObjectSliderColor(const QColor& col, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectSliderColor(const QColor& col, const QString& name)");

    Q_UNUSED(name);
    mActObject.sc = col;

    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
    requestRedraw(&mPage);
}

void TPropertiesGeneral::onObjectSubPageSet(const QString& group, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectSubPageSet(const QString& group, const QVariant& data, const QString& name)");

    Q_UNUSED(name);
    Q_UNUSED(data);

    int oldID = mActObject.st;
    mActObject.st = TConfMain::Current().getSubPageID(group);

    if (oldID != mActObject.st)
    {
        mPage.objects[mActObjectID]->setObject(mActObject);
        markChanged();
        mChanged = true;
        requestRedraw(&mPage);
    }
}

void TPropertiesGeneral::onObjectOrientation(const QString& ori, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectOrientation(const QString& ori, const QVariant& data, const QString& name)");

    mActObject.on = data.toInt() == 1 ? ori : "";
    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
    requestRedraw(&mPage);
}

void TPropertiesGeneral::onObjectSpacing(int value, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectSpacing(int value, const QString& name)");

    mActObject.sa = value;
    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
}

void TPropertiesGeneral::onObjectAnchorPosition(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectAnchorPosition(const QString& text, const QVariant& data, const QString& name)");

    mActObject.we = data.toString();
    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
    requestRedraw(&mPage);
}

void TPropertiesGeneral::onObjectScrollbarOffset(int value, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectScrollbarOffset(int value, const QString& name)");

    mActObject.bo = value;
    mPage.objects[mActObjectID]->setObject(mActObject);
    markChanged();
    mChanged = true;
}

void TPropertiesGeneral::initYesNo(QStringList& list, QList<QVariant>& data)
{
    DECL_TRACER("TPropertiesGeneral::initYesNo(QStringList& list, QList<QVariant>& data)");

    list = { tr("no"), tr("yes") };
    data = { false, true };
}
