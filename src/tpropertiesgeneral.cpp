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

void TPropertiesGeneral::setGeometryPopup(const QRect& geom)
{
    DECL_TRACER("TPropertiesGeneral::setGeometry(const QRect& geom)");

    if (!mTable || mPage.popupType != Page::PT_POPUP)
        return;

    onSpinGeometryChanged(geom.left(), "PopupLeft");
    onSpinGeometryChanged(geom.top(), "PopupTop");
    onSpinGeometryChanged(geom.width(), "PopupWidth");
    onSpinGeometryChanged(geom.height(), "PopupHeight");
    update();
}

void TPropertiesGeneral::setGeometryButton(int bi, const QRect& geom)
{
    DECL_TRACER("TPropertiesGeneral::setGeometryButton(int bi, const QRect& geom)");

    if (!mTable || bi <= 0)
        return;

    mActObjectID = bi - 1;
    onSpinGeometryChanged(geom.left(), "ObjectLeft");
    onSpinGeometryChanged(geom.top(), "ObjectTop");
    onSpinGeometryChanged(geom.width(), "ObjectWidth");
    onSpinGeometryChanged(geom.height(), "ObjectHeight");
}

void TPropertiesGeneral::update()
{
    DECL_TRACER("TPropertiesGeneral::update()");

    ObjHandler::TOBJECT_t object = getActualObject(mPage);

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
        mActObjectID = object.bi - 1;
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
}

void TPropertiesGeneral::setTable(STATE_TYPE stype)
{
    DECL_TRACER("TPropertiesGeneral::setTable(STATE_TYPE stype)");

    if (!mInitialized || !mTable)
        createTable(stype);

    // First hide all rows
    for (int i = 0; i < mTable->rowCount(); ++i)
        mTable->setRowHidden(i, true);

    // Now enable only the used rows
    switch(stype)
    {
        case STATE_PAGE:
            mTable->setRowHidden(2, false);         // PageName
            mTable->setRowHidden(5, false);         // PageDescription
        break;

        case STATE_POPUP:
            mTable->setRowHidden(0, false);         // PopupPopupType
            mTable->setRowHidden(2, false);         // PageName
            mTable->setRowHidden(5, false);         // PageDescription

            if (mPage.popupType == Page::PT_POPUP)
            {
                mTable->setRowHidden(7, false);     // PopupLeft
                mTable->setRowHidden(9, false);     // PopupTop
            }

            mTable->setRowHidden(11, false);        // PopupWidth
            mTable->setRowHidden(13, false);        // PopupHeight

            if (mPage.popupType == Page::PT_POPUP)
            {
                mTable->setRowHidden(15, false);    // PopupResetOnPos
                mTable->setRowHidden(17, false);    // PopupGroups
                mTable->setRowHidden(18, false);    // PopupTimeout
                mTable->setRowHidden(19, false);    // PopupModal

                if (mPage.collapseDirection == Page::COLDIR_NONE)
                {
                    mTable->setRowHidden(20, false);    // PopupShowEffect

                    if (mPage.showEffect != Page::SE_NONE)
                    {
                        mTable->setRowHidden(21, false);// PopupShowEffectTime

                        if (mPage.showEffect != Page::SE_FADE)
                        {
                            if (mPage.showEffect == Page::SE_SLIDE_LEFT || mPage.showEffect == Page::SE_SLIDE_LEFT_FADE||
                                mPage.showEffect == Page::SE_SLIDE_RIGHT || mPage.showEffect == Page::SE_SLIDE_RIGHT_FADE)
                                mTable->setRowHidden(22, false);    // PopupShowEffectPosX
                            else
                                mTable->setRowHidden(23, false);    // PopupShowEffectPosY
                        }
                    }

                    mTable->setRowHidden(24, false);    // PopupHideEffect

                    if (mPage.hideEffect != Page::SE_NONE)
                    {
                        mTable->setRowHidden(25, false);        // PopupHideEffectTime

                        if (mPage.hideEffect != Page::SE_FADE)
                        {
                            if (mPage.hideEffect == Page::SE_SLIDE_LEFT || mPage.hideEffect == Page::SE_SLIDE_LEFT_FADE||
                                mPage.hideEffect == Page::SE_SLIDE_RIGHT || mPage.hideEffect == Page::SE_SLIDE_RIGHT_FADE)
                                mTable->setRowHidden(26, false);    // PopupHideEffectPosX
                            else
                                mTable->setRowHidden(27, false);    // PopupHideEffectPosY
                        }
                    }

                    mTable->setRowHidden(28, false);    // PopupCollapseDirection
                }
                else
                {
                    mTable->setRowHidden(28, false);    // PopupCollapseDirection
                    mTable->setRowHidden(29, false);    // PopupCollapseOffest
                    mTable->setRowHidden(30, false);    // PopupCollapseShowOpen
                }
            }
        break;

        case STATE_BUTTON:
            if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
                mActObject = mPage.objects[mActObjectID]->getObject();

            mTable->setRowHidden(1, false);             // ButtonType
            mTable->setRowHidden(3, false);             // ObjectName
            mTable->setRowHidden(4, false);             // ButtonLockName
            mTable->setRowHidden(6, false);             // ObjectDescription
            mTable->setRowHidden(8, false);             // ObjectLeft
            mTable->setRowHidden(10, false);            // ObjectTop
            mTable->setRowHidden(12, false);            // ObjectTop
            mTable->setRowHidden(14, false);            // ObjectHeight
            mTable->setRowHidden(16, false);            // ObjectZOrder
            mTable->setRowHidden(31, false);            // ObjectDragDropType

            if (mActObject.ddt == "dr")
                mTable->setRowHidden(32, false);        // ObjectDropGroup

            mTable->setRowHidden(33, false);            // ObjectTouchStyle
            mTable->setRowHidden(34, false);            // ObjectBorderStyle

            if (mActObject.type == ObjHandler::MULTISTATE_GENERAL ||
                mActObject.type == ObjHandler::MULTISTATE_BARGRAPH)
            {
                mTable->setRowHidden(35, false);        // ObjectStateCount

                if (mActObject.type != ObjHandler::MULTISTATE_BARGRAPH)
                {
                    mTable->setRowHidden(36, false);    // ObjectAnimateTimeUp
                    mTable->setRowHidden(37, false);    // ObjectAnimateTimeDown
                }
            }

            mTable->setRowHidden(38, false);            // ObjectDisabled
            mTable->setRowHidden(39, false);            // ObjectHidden

            if (mActObject.type == ObjHandler::BARGRAPH)
            {
                mTable->setRowHidden(40, false);        // ObjectValueDirection
                mTable->setRowHidden(41, false);        // ObjectSliderName
                mTable->setRowHidden(42, false);        // ObjectSliderColor
            }
            else
                mTable->setRowHidden(43, false);    // ObjectPasswordProtection

            // TODO: To be continued
        break;

        default:
            break;
    }
}

void TPropertiesGeneral::createTable(STATE_TYPE stype)
{
    DECL_TRACER("TPropertiesGeneral::createTable(STATE_TYPE stype)");

    if (!mTable)
        return;

    Q_UNUSED(stype);
    int rows = 41;
    mTable->clear();
    mTable->setColumnCount(2);
    mTable->setRowCount(rows);

    for (int i = 0; i < rows; ++i)
    {
        QTableWidgetItem *cell1 = new QTableWidgetItem;
        QTableWidgetItem *cell2 = nullptr;

        cell1->setBackground(Qt::lightGray);

        switch(i)
        {
            case 0:
                cell1->setText(getLabelText(TTEXT_POPUPTYPE));
                mTable->setCellWidget(i, 1, makePopupType("PopupPopupType"));
            break;

            case 1:
                cell1->setText(getLabelText(TTEXT_TYPE));
                mTable->setCellWidget(i, 1, makeButtonType("ButtonType"));
            break;

            case 2:
                cell1->setText(getLabelText(TTEXT_NAME));
                mTable->setCellWidget(i, 1, makeObjectName(mPage.name, "PageName"));
            break;

            case 3:
            {
                cell1->setText(getLabelText(TTEXT_NAME));
                QString name;

                if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
                    name = mPage.objects[mActObjectID]->getButtonName();

                mTable->setCellWidget(i, 1, makeObjectName(name, "ObjectName"));
            }
            break;

            case 4:
                cell1->setText(getLabelText(TTEXT_LOCK_BUTTON_NAME));
                mTable->setCellWidget(i, 1, makeButtonLockName("ButtonLockName"));
            break;

            case 5:
                cell1->setText(getLabelText(TTEXT_DESCRIPTION));
                mTable->setCellWidget(i, 1, makeDescription("PageDescription"));
            break;

            case 6:
                cell1->setText(getLabelText(TTEXT_DESCRIPTION));
                mTable->setCellWidget(i, 1, makeDescription("ObjectDescription"));
            break;

            case 7:
                cell1->setText(getLabelText(TTEXT_LEFT));
                mTable->setCellWidget(i, 1, makeSpinGeometry("PopupLeft"));
            break;

            case 8:
                cell1->setText(getLabelText(TTEXT_LEFT));
                mTable->setCellWidget(i, 1, makeSpinGeometry("ObjectLeft"));
            break;

            case 9:
                cell1->setText(getLabelText(TTEXT_TOP));
                mTable->setCellWidget(i, 1, makeSpinGeometry("PopupTop"));
            break;

            case 10:
                cell1->setText(getLabelText(TTEXT_TOP));
                mTable->setCellWidget(i, 1, makeSpinGeometry("ObjectTop"));
            break;

            case 11:
                cell1->setText(getLabelText(TTEXT_WIDTH));
                mTable->setCellWidget(i, 1, makeSpinGeometry("PopupWidth"));
            break;

            case 12:
                cell1->setText(getLabelText(TTEXT_WIDTH));
                mTable->setCellWidget(i, 1, makeSpinGeometry("ObjectWidth"));
            break;

            case 13:
                cell1->setText(getLabelText(TTEXT_HEIGHT));
                mTable->setCellWidget(i, 1, makeSpinGeometry("PopupHeight"));
            break;

            case 14:
                cell1->setText(getLabelText(TTEXT_HEIGHT));
                mTable->setCellWidget(i, 1, makeSpinGeometry("ObjectHeight"));
            break;

            case 15:
                cell1->setText(getLabelText(TTEXT_RESET_POS_ON_SHOW));
                mTable->setCellWidget(i, 1, makePopupResetOnPos("PopupResetOnPos"));
            break;

            case 16:
                cell1->setText(getLabelText(TTEXT_ZORDER));
                mTable->setCellWidget(i, 1, makeObjectZOrder("ObjectZOrder"));
            break;

            case 17:
                cell1->setText(getLabelText(TTEXT_GROUP));
                mTable->setCellWidget(i, 1, makePopupGroups("PopupGroups"));
            break;

            case 18:
                cell1->setText(getLabelText(TTEXT_TIMEOUT));
                mTable->setCellWidget(i, 1, makePopupTimeout("PopupTimeout"));
            break;

            case 19:
                cell1->setText(getLabelText(TTEXT_MODAL));
                mTable->setCellWidget(i, 1, makePopupModal("PopupModal"));
            break;

            case 20:
                cell1->setText(getLabelText(TTEXT_SHOW_EFFECT));
                mTable->setCellWidget(i, 1, makePopupShowEffect("PopupShowEffect"));
            break;

            case 21:
                cell1->setText(getLabelText(TTEXT_SHOW_EFFECT_TIME));
                mTable->setCellWidget(i, 1, makePopupEffectTime(mPage.showTime, "PopupShowEffectTime"));
            break;

            case 22:
                cell1->setText(getLabelText(TTEXT_SHOW_EFFECT_X_POS));
                mTable->setCellWidget(i, 1, makePopupEffectPos(mPage.showEffect, "PopupShowEffectPosX"));
            break;

            case 23:
                cell1->setText(getLabelText(TTEXT_SHOW_EFFECT_Y_POS));
                mTable->setCellWidget(i, 1, makePopupEffectPos(mPage.showEffect, "PopupShowEffectPosY"));
            break;

            case 24:
                cell1->setText(getLabelText(TTEXT_HIDE_EFFECT));
                mTable->setCellWidget(i, 1, makePopupHideEffect("PopupHideEffect"));
            break;

            case 25:
                cell1->setText(getLabelText(TTEXT_HIDE_EFFECT_TIME));
                mTable->setCellWidget(i, 1, makePopupEffectTime(mPage.hideTime, "PopupHideEffectTime"));
            break;

            case 26:
                cell1->setText(getLabelText(TTEXT_HIDE_EFFECT_X_POS));
                mTable->setCellWidget(i, 1, makePopupEffectPos(mPage.hideEffect, "PopupHideEffectPosX"));
            break;

            case 27:
                cell1->setText(getLabelText(TTEXT_HIDE_EFFECT_Y_POS));
                mTable->setCellWidget(i, 1, makePopupEffectPos(mPage.hideEffect, "PopupHideEffectPosY"));
            break;

            case 28:
                cell1->setText(getLabelText(TTEXT_COLLAPSE_DIRECTION));
                mTable->setCellWidget(i, 1, makePopupCollapseDir("PopupCollapseDirection"));
            break;

            case 29:
                cell1->setText(getLabelText(TTEXT_COLLAPSE_OFFSET));
                mTable->setCellWidget(i, 1, makePopupCollapseOffset("PopupCollapseOffest"));
            break;

            case 30:
                cell1->setText(getLabelText(TTEXT_COLLAPSE_SHOW_OPEN));
                mTable->setCellWidget(i, 1, makePopupCollapseShowOpen("PopupCollapseShowOpen"));
            break;

            case 31:
                cell1->setText(getLabelText(TTEXT_DRAGDROP_TYPE));
                mTable->setCellWidget(i, 1, makeObjectDragDropType("ObjectDragDropType"));
            break;

            case 32:
                cell1->setText(getLabelText(TTEXT_DROP_GROUP));
                mTable->setCellWidget(i, 1, makeObjectDropGroup("ObjectDropGroup"));
            break;

            case 33:
                cell1->setText(getLabelText(TTEXT_TOUCH_STYLE));
                mTable->setCellWidget(i, 1, makeObjectTouchStyle("ObjectTouchStyle"));
            break;

            case 34:
                cell1->setText(getLabelText(TTEXT_BORDER_STYLE));
                mTable->setCellWidget(i, 1, makeObjectBorderStyle("ObjectBorderStyle"));
            break;

            case 35:
                cell1->setText(getLabelText(TTEXT_STATE_COUNT));
                mTable->setCellWidget(i, 1, makeObjectStateCount("ObjectStateCount"));
            break;

            case 36:
                cell1->setText(getLabelText(TTEXT_ANIMATE_TIME_UP));
                mTable->setCellWidget(i, 1, makeObjectAnimateTime("ObjectAnimateTimeUp"));
            break;

            case 37:
                cell1->setText(getLabelText(TTEXT_ANIMATE_TIME_DOWN));
                mTable->setCellWidget(i, 1, makeObjectAnimateTime("ObjectAnimateTimeDown"));
            break;

            case 38:
                cell1->setText(getLabelText(TTEXT_DISABLED));
                mTable->setCellWidget(i, 1, makeObjectYesNoSelect("ObjectDisabled"));
            break;

            case 39:
                cell1->setText(getLabelText(TTEXT_HIDDEN));
                mTable->setCellWidget(i, 1, makeObjectYesNoSelect("ObjectHidden"));
            break;
/*
            case 40:
                cell1->setText(getLabelText(TTEXT_PASSWORD_PROTECTION));
                mTable->setCellWidget(i, 1, makeObjectPasswordProtected("ObjectValueDirection"));
                break;

            case 41:
                cell1->setText(getLabelText(TTEXT_PASSWORD_PROTECTION));
                mTable->setCellWidget(i, 1, makeObjectPasswordProtected("ObjectSliderName"));
                break;

            case 42:
                cell1->setText(getLabelText(TTEXT_PASSWORD_PROTECTION));
                mTable->setCellWidget(i, 1, makeObjectPasswordProtected("ObjectSliderColor"));
                break;
*/
            case 43:
                cell1->setText(getLabelText(TTEXT_PASSWORD_PROTECTION));
                mTable->setCellWidget(i, 1, makeObjectPasswordProtected("ObjectPasswordProtection"));
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
    QList<QVariant> data = { 1, 2, 3, 4, 5, 6, 7 };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size() && mActObject.bi > 0)
        combo->setCurrentIndex(mActObject.type - 1);

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

        MSG_DEBUG("Value: " << value);
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
}

void TPropertiesGeneral::onComboButtonTypeChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onComboButtonTypeChanged(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    if (mActObjectID < 0 || mPage.objects.size() <= mActObjectID)
        return;

    mActObject = mPage.objects[mActObjectID]->getObject();
    int type = data.toInt();

    switch(type)
    {
        case 1: mActObject.type = ObjHandler::GENERAL; break;
        case 2: mActObject.type = ObjHandler::MULTISTATE_GENERAL; break;
        case 3: mActObject.type = ObjHandler::BARGRAPH; break;
        case 4: mActObject.type = ObjHandler::MULTISTATE_BARGRAPH; break;
        case 5: mActObject.type = ObjHandler::TEXT_INPUT; break;
        case 6: mActObject.type = ObjHandler::SUBPAGE_VIEW; break;
        case 7: mActObject.type = ObjHandler::LISTVIEW; break;
    }

    mPage.objects[mActObjectID]->setObject(mActObject);
    setTable(STATE_BUTTON);
    mChanged = true;
    markChanged();
    // TODO: Add code to change the view of the object accordingly
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
    }
    else
    {
        if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
        {
            mActObject = mPage.objects[mActObjectID]->getObject();

            if (name.endsWith("Left"))
                mActObject.lt = value;
            else if (name.endsWith("Top"))
                mActObject.tp = value;
            else if (name.endsWith("Width"))
                mActObject.wt = value;
            else if (name.endsWith("Height"))
                mActObject.ht = value;
        }
    }

    mChanged = true;
    saveChangedData(&mPage, TBL_GENERAL);
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

    saveChangedData(&mPage, TBL_GENERAL);
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

    saveChangedData(&mPage, TBL_GENERAL);
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

    saveChangedData(&mPage, TBL_GENERAL);
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

    saveChangedData(&mPage, TBL_GENERAL);
    mChanged = true;
}


void TPropertiesGeneral::onObjectPasswordProtection(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectPasswordProtection(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);
    Q_UNUSED(name);

    mActObject.pp = data.toInt();
    saveChangedData(&mPage, TBL_GENERAL);
    mChanged = true;
}

void TPropertiesGeneral::onObjectStateCount(int value, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectStateCount(int value, const QString& name)");

    Q_UNUSED(name);

    mActObject.stateCount = value;
    mActObject.rm = value;          // Yes, this exists twice!
    saveChangedData(&mPage, TBL_GENERAL);
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

    saveChangedData(&mPage, TBL_GENERAL);
    mChanged = true;
}

void TPropertiesGeneral::initYesNo(QStringList& list, QList<QVariant>& data)
{
    DECL_TRACER("TPropertiesGeneral::initYesNo(QStringList& list, QList<QVariant>& data)");

    list = { tr("no"), tr("yes") };
    data = { false, true };
}
