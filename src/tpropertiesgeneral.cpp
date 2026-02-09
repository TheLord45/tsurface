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
#include "telementbitmapselector.h"
#include "telementbordername.h"
#include "telementwidgetcombo.h"
#include "telementwidgettext.h"
#include "telementlineedit.h"
#include "telementspinbox.h"
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
#define TTEXT_TOUCH_STYLE           26
#define TTEXT_BORDER_STYLE          27
#define TTEXT_DISABLED              28
#define TTEXT_HIDDEN                29
#define TTEXT_PASSWORD_PROTECTION   30

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

    mPage.left = geom.left();
    mPage.top = geom.top();
    mPage.width = geom.width();
    mPage.height = geom.height();
    onSpinLeftValue(mPage.left);
    onSpinTopValue(mPage.top);
    onSpinWidthValue(mPage.width);
    onSpinHeightValue(mPage.height);
}

void TPropertiesGeneral::setGeometryButton(int bi, const QRect& geom)
{
    DECL_TRACER("TPropertiesGeneral::setGeometryButton(int bi, const QRect& geom)");

    if (!mTable)
        return;

    onSpinLeftValue(mPage.left);
    onSpinTopValue(mPage.top);
    onSpinWidthValue(mPage.width);
    onSpinHeightValue(mPage.height);
}

void TPropertiesGeneral::update()
{
    DECL_TRACER("TPropertiesGeneral::update()");

    ObjHandler::TOBJECT_t object = getActualObject();

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
        saveChangedData(&mPage, TBL_GENERIC);

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
        case TTEXT_TOUCH_STYLE:         return tr("Touch Style"); break;
        case TTEXT_BORDER_STYLE:        return tr("Border Style"); break;
        case TTEXT_DISABLED:            return tr("Disabled"); break;
        case TTEXT_HIDDEN:              return tr("Hidden"); break;
        case TTEXT_PASSWORD_PROTECTION: return tr("Password Protection"); break;
    }

        return QString();
}

void TPropertiesGeneral::loadPage(int pageID)
{
    DECL_TRACER("TPropertiesGeneral::loadPage(int pageID)");

    if (pageID == mPage.pageID)
        return;

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_GENERIC);

    mPage = TPageHandler::Current().getPage(pageID);
    mChanged = false;
}

void TPropertiesGeneral::setGeneralPage(Page::PAGE_t& page, STATE_TYPE stype, int objid)
{
    DECL_TRACER("TPropertiesGeneral::setGeneralPage(Page::PAGE_t& page, STATE_TYPE stype, int objid)");

    if (page.pageID <= 0)
        return;

    mPage = page;
    mActObjectID = objid;
    setTable(stype);
}
/*
void TPropertiesGeneral::setGeneralPage(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::setPage(const QString& name)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    Page::PAGE_t page = TPageHandler::Current().getPage(name);

    if (page.pageID == mPage.pageID)
        return;

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_GENERIC);

    mChanged = false;
    mPage = page;
    setGeneralPage(mPage.pageID, true);
}

void TPropertiesGeneral::setGeneralPage(int id, bool loaded)
{
    DECL_TRACER("TPropertiesGeneral::setGeneralPage(int id, bool loaded)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    Page::PAGE_t page;

    if (!loaded)
    {
        page = TPageHandler::Current().getPage(id);

        if (page.pageID == mPage.pageID)
            return;
    }

    if (!loaded && mPage.pageID > 0 && mChanged)
    {
        saveChangedData(&mPage, TBL_GENERIC);
        mPage = Page::PAGE_t();
    }

    mChanged = false;

    if (!loaded)
        mPage = page;

    if (mPage.pageID <= 0)
        return;

    mInitialized = false;
    mTable->clear();
    mTable->setColumnCount(2);
    mTable->setRowCount(2);

    for (int i = 0; i < 2; ++i)
    {
        QTableWidgetItem *cell1 = new QTableWidgetItem;
        QTableWidgetItem *cell2 = new QTableWidgetItem;

        cell1->setBackground(brush);

        switch(i)
        {
            case 0:
                cell1->setText(tr("Name"));
                cell2->setText(mPage.name);
            break;

            case 1:
                cell1->setText(tr("Description"));
                cell2->setText(mPage.description);
                delete cell2;
                cell2 = nullptr;
                mTable->setCellWidget(i, 1, makeLabelTool(mPage.description, i));
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

void TPropertiesGeneral::setGeneralPopup(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::setPopup(const QString& name)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    Page::PAGE_t page = TPageHandler::Current().getPage(name);

    if (page.pageID == mPage.pageID)
        return;

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_GENERIC);

    mChanged = false;
    mPage = page;
    setGeneralPopup(mPage.pageID, true);
}

void TPropertiesGeneral::setGeneralPopup(int id, bool loaded)
{
    DECL_TRACER("TPropertiesGeneral::setGeneralPopup(int id, bool loaded)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    Page::PAGE_t page;

    if (!loaded)
    {
        page = TPageHandler::Current().getPage(id);

        if (page.pageID == mPage.pageID)
            return;
    }

    if (!loaded && mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_GENERIC);

    mChanged = false;

    if (!loaded)
        mPage = page;

    if (mPage.pageID <= 0)
        return;

    mInitialized = false;
    mTable->clear();
    mTable->setColumnCount(2);
    mTable->setRowCount(14);
    QSize panelSize = TConfMain::Current().getPanelSize();

    for (int i = 0; i < 14; ++i)
    {
        QTableWidgetItem *cell1 = new QTableWidgetItem;
        QTableWidgetItem *cell2 = nullptr;

        cell1->setBackground(brush);

        switch(i)
        {
            case 0:
            {
                cell1->setText(tr("Popup type"));
                mComboPopupType = new QComboBox;
                mComboPopupType->addItem(tr("Standard"), Page::PT_POPUP);
                mComboPopupType->addItem(tr("Subpage"), Page::PT_SUBPAGE);
                mComboPopupType->setObjectName(QString("ComboBox_%1").arg(i));
                connect(mComboPopupType, &QComboBox::currentIndexChanged, this, &TPropertiesGeneral::onComboPopupTypeChanged);
                mTable->setCellWidget(i, 1, mComboPopupType);
            }
            break;

            case 1:
                cell1->setText(tr("Name"));
                cell2 = new QTableWidgetItem;
                cell2->setText(mPage.name);
            break;

            case 2:
                cell1->setText(tr("Description"));
                mTable->setCellWidget(i, 1, makeLabelTool(mPage.description, i));
            break;

            case 3:
            {
                cell1->setText(tr("Left"));
                mSpinLeft = new QSpinBox;
                mSpinLeft->setMinimum(0);
                mSpinLeft->setMaximum(panelSize.width());
                mSpinLeft->setValue(mPage.left);
                connect(mSpinLeft, &QSpinBox::valueChanged, this, &TPropertiesGeneral::onSpinLeftValue);
                mTable->setCellWidget(i, 1, mSpinLeft);
            }
            break;

            case 4:
            {
                cell1->setText(tr("Top"));
                mSpinTop = new QSpinBox;
                mSpinTop->setMinimum(0);
                mSpinTop->setMaximum(panelSize.height());
                mSpinTop->setValue(mPage.top);
                connect(mSpinTop, &QSpinBox::valueChanged, this, &TPropertiesGeneral::onSpinTopValue);
                mTable->setCellWidget(i, 1, mSpinTop);
            }
            break;

            case 5:
            {
                cell1->setText(tr("Width"));
                mSpinWidth = new QSpinBox;
                mSpinWidth->setMinimum(0);
                mSpinWidth->setMaximum(panelSize.width());
                mSpinWidth->setValue(mPage.width);
                connect(mSpinWidth, &QSpinBox::valueChanged, this, &TPropertiesGeneral::onSpinWidthValue);
                mTable->setCellWidget(i, 1, mSpinWidth);
            }
            break;

            case 6:
            {
                cell1->setText(tr("Height"));
                mSpinHeight = new QSpinBox;
                mSpinHeight->setMinimum(0);
                mSpinHeight->setMaximum(panelSize.height());
                mSpinHeight->setValue(mPage.height);
                connect(mSpinHeight, &QSpinBox::valueChanged, this, &TPropertiesGeneral::onSpinHeightValue);
                mTable->setCellWidget(i, 1, mSpinHeight);
            }
            break;

            case 7:
            {
                cell1->setText(tr("Reset pos on show"));
                mComboResetPos = new QComboBox;
                mComboResetPos->addItem(tr("No"), 0);
                mComboResetPos->addItem(tr("Yes"), 1);
                connect(mComboResetPos, &QComboBox::currentIndexChanged, this, &TPropertiesGeneral::onComboResetPosChanged);
                mTable->setCellWidget(i, 1, mComboResetPos);
            }
            break;

            case 8:
            {
                cell1->setText(tr("Group"));
                mComboGroupText = new QComboBox;
                mComboGroupText->setEditable(true);
                QStringList groups = TPageHandler::Current().getGroupNames();
                QStringList::Iterator iter;
                mComboGroupText->addItem("");

                for (iter = groups.begin(); iter != groups.end(); ++iter)
                    mComboGroupText->addItem(*iter);

                connect(mComboGroupText, &QComboBox::currentTextChanged, this, &TPropertiesGeneral::onComboGroupTextChanged);
                mTable->setCellWidget(i, 1, mComboGroupText);
            }
            break;

            case 9:
            {
                cell1->setText(tr("Timeout"));
                mSpinTimeout = new QSpinBox;
                mSpinTimeout->setMinimum(0);
                mSpinTimeout->setMaximum(300);
                mSpinTimeout->setValue(mPage.timeout);
                connect(mSpinTimeout, &QSpinBox::valueChanged, this, &TPropertiesGeneral::onSpinTimeoutValue);
                mTable->setCellWidget(i, 1, mSpinTimeout);
            }
            break;

            case 10:
            {
                cell1->setText(tr("Modal"));
                mComboModal = new QComboBox;
                mComboModal->addItem(tr("No"), 0);
                mComboModal->addItem(tr("Yes"), 1);
                connect(mComboModal, &QComboBox::currentIndexChanged, this, &TPropertiesGeneral::onComboModalChanged);
                mTable->setCellWidget(i, 1, mComboModal);
            }
            break;

            case 11:
            {
                cell1->setText(tr("Show effect"));
                mComboShow = new QComboBox;
                mComboShow->addItem(tr("None"), Page::SE_NONE);
                mComboShow->addItem(tr("Fade"), Page::SE_FADE);
                mComboShow->addItem(tr("Slide from left"), Page::SE_SLIDE_LEFT);
                mComboShow->addItem(tr("Slide from right"), Page::SE_SLIDE_RIGHT);
                mComboShow->addItem(tr("Slide from top"), Page::SE_SLIDE_TOP);
                mComboShow->addItem(tr("Slide from bottom"), Page::SE_SLIDE_BOTTOM);
                mComboShow->addItem(tr("Slide from left fade"), Page::SE_SLIDE_LEFT_FADE);
                mComboShow->addItem(tr("Slide from right fade"), Page::SE_SLIDE_RIGHT_FADE);
                mComboShow->addItem(tr("Slide from top fade"), Page::SE_SLIDE_TOP_FADE);
                mComboShow->addItem(tr("Slide from bottom fade"), Page::SE_SLIDE_BOTTOM_FADE);
                connect(mComboShow, &QComboBox::currentIndexChanged, this, &TPropertiesGeneral::onComboShowChanged);
                mTable->setCellWidget(i, 1, mComboShow);
            }
            break;

            case 12:
            {
                cell1->setText(tr("Hide effect"));
                mComboHide = new QComboBox;
                mComboHide->addItem(tr("None"), Page::SE_NONE);
                mComboHide->addItem(tr("Fade"), Page::SE_FADE);
                mComboHide->addItem(tr("Slide to left"), Page::SE_SLIDE_LEFT);
                mComboHide->addItem(tr("Slide to right"), Page::SE_SLIDE_RIGHT);
                mComboHide->addItem(tr("Slide to top"), Page::SE_SLIDE_TOP);
                mComboHide->addItem(tr("Slide to bottom"), Page::SE_SLIDE_BOTTOM);
                mComboHide->addItem(tr("Slide to left fade"), Page::SE_SLIDE_LEFT_FADE);
                mComboHide->addItem(tr("Slide to right fade"), Page::SE_SLIDE_RIGHT_FADE);
                mComboHide->addItem(tr("Slide to top fade"), Page::SE_SLIDE_TOP_FADE);
                mComboHide->addItem(tr("Slide to bottom fade"), Page::SE_SLIDE_BOTTOM_FADE);
                connect(mComboHide, &QComboBox::currentIndexChanged, this, &TPropertiesGeneral::onComboHideChanged);
                mTable->setCellWidget(i, 1, mComboHide);
            }
            break;

            case 13:
            {
                cell1->setText(tr("Collapse direction"));
                mComboColapse = new QComboBox;
                mComboColapse->addItem(tr("None"), Page::COLDIR_NONE);
                mComboColapse->addItem(tr("Left"), Page::COLDIR_LEFT);
                mComboColapse->addItem(tr("Right"), Page::COLDIR_RIGHT);
                mComboColapse->addItem(tr("Up"), Page::COLDIR_UP);
                mComboColapse->addItem(tr("Down"), Page::COLDIR_DOWN);
                connect(mComboColapse, &QComboBox::currentIndexChanged, this, &TPropertiesGeneral::onComboColapseChange);
                mTable->setCellWidget(i, 1, mComboColapse);
            }
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
*/

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
    for (int i = 0; i < 30; ++i)
        mTable->setRowHidden(i, true);

    // Now anable only the used rows
    switch(stype)
    {
        case STATE_PAGE:
            mTable->setRowHidden(2, false);
            mTable->setRowHidden(4, false);
        break;

        case STATE_POPUP:
            mTable->setRowHidden(0, false);
            mTable->setRowHidden(2, false);
            mTable->setRowHidden(4, false);
            mTable->setRowHidden(5, false);
            mTable->setRowHidden(6, false);
            mTable->setRowHidden(7, false);
            mTable->setRowHidden(8, false);
            mTable->setRowHidden(9, false);
            mTable->setRowHidden(10, false);
            mTable->setRowHidden(11, false);
            mTable->setRowHidden(12, false);
            mTable->setRowHidden(13, false);
            mTable->setRowHidden(14, false);
            mTable->setRowHidden(15, false);
        break;

        case STATE_BUTTON:
            mTable->setRowHidden(1, false);
            mTable->setRowHidden(3, false);
        break;
    }
}

void TPropertiesGeneral::createTable(STATE_TYPE stype)
{
    DECL_TRACER("TPropertiesGeneral::createTable(STATE_TYPE stype)");

    mInitialized = false;
    int rows = 30;
    mTable->clear();
    mTable->setColumnCount(2);
    mTable->setRowCount(rows);
    QSize panelSize = TConfMain::Current().getPanelSize();

    for (int i = 0; i < rows; ++i)
    {
        QTableWidgetItem *cell1 = new QTableWidgetItem;
        QTableWidgetItem *cell2 = nullptr;

        cell1->setBackground(Qt::lightGray);

        switch(i)
        {
            case 0:
                if (stype == STATE_POPUP)
                {
                    cell1->setText(getLabelText(TTEXT_POPUPTYPE));
                    mTable->setCellWidget(i, 1, makePopupType("PopupPopupType"));
                }
            break;

            case 1:
                if (stype == STATE_BUTTON)
                {
                    cell1->setText(getLabelText(TTEXT_TYPE));
                    mTable->setCellWidget(i, 1, makeButtonType("ButtonType"));
                }
            break;

            case 2:
                cell1->setText(getLabelText(TTEXT_NAME));

                if (stype == STATE_PAGE || stype == STATE_POPUP)
                    mTable->setCellWidget(i, 1, makeObjectName(mPage.name, "PageName"));
                else if (mActObjectID >= 0 && mPage.objects.size() >= (mActObjectID + 1))
                    mTable->setCellWidget(i, 1, makeObjectName(mPage.objects[mActObjectID]->getButtonName(), "ObjectName"));
            break;

            case 3:
                if (stype == STATE_BUTTON)
                {
                    cell1->setText(getLabelText(TTEXT_LOCK_BUTTON_NAME));
                    mTable->setCellWidget(i, 1, makeButtonLockName("ButtonLockName"));
                }
            break;

            case 4:
                cell1->setText(getLabelText(TTEXT_DESCRIPTION));

                if (stype == STATE_PAGE || stype == STATE_POPUP)
                    mTable->setCellWidget(i, 1, makeDescription("PageDescription"));
                else
                    mTable->setCellWidget(i, 1, makeDescription("ObjectDescription"));
            break;

            case 5:
                cell1->setText(getLabelText(TTEXT_LEFT));

                if (stype == STATE_POPUP)
                    mTable->setCellWidget(i, 1, makeSpinGeometry("PopupLeft"));
                else if (stype != STATE_PAGE)
                    mTable->setCellWidget(i, 1, makeSpinGeometry("ObjectLeft"));
            break;

            case 6:
                cell1->setText(getLabelText(TTEXT_TOP));

                if (stype == STATE_POPUP)
                    mTable->setCellWidget(i, 1, makeSpinGeometry("PopupTop"));
                else if (stype != STATE_PAGE)
                    mTable->setCellWidget(i, 1, makeSpinGeometry("ObjectTop"));
            break;

            case 7:
                cell1->setText(getLabelText(TTEXT_WIDTH));

                if (stype == STATE_POPUP)
                    mTable->setCellWidget(i, 1, makeSpinGeometry("PopupWidth"));
                else if (stype != STATE_PAGE)
                    mTable->setCellWidget(i, 1, makeSpinGeometry("ObjectWidth"));
            break;

            case 8:
                cell1->setText(getLabelText(TTEXT_HEIGHT));

                if (stype == STATE_POPUP)
                    mTable->setCellWidget(i, 1, makeSpinGeometry("PopupHeight"));
                else if (stype != STATE_PAGE)
                    mTable->setCellWidget(i, 1, makeSpinGeometry("ObjectHeight"));
            break;

            case 9:
                if (stype == STATE_POPUP)
                {
                    cell1->setText(getLabelText(TTEXT_RESET_POS_ON_SHOW));
                    mTable->setCellWidget(i, 1, makePopupResetOnPos("PopupResetOnPos"));
                }
                else if (stype != STATE_PAGE)
                {
                    cell1->setText(getLabelText(TTEXT_ZORDER));
                    mTable->setCellWidget(i, 1, makePopupResetOnPos("ObjectZOrder"));
                }
            break;

            case 10:
                if (stype == STATE_POPUP)
                {
                    cell1->setText(getLabelText(TTEXT_GROUP));
                    mTable->setCellWidget(i, 1, makePopupGroups("PopupGroups"));
                }
            break;

            case 11:
                if (stype == STATE_POPUP)
                {
                    cell1->setText(getLabelText(TTEXT_TIMEOUT));
                    mTable->setCellWidget(i, 1, makePopupTimeout("PopupTimeout"));
                }
            break;

            case 12:
                if (stype == STATE_POPUP)
                {
                    cell1->setText(tr("Modal"));
                    mComboModal = new QComboBox;
                    mComboModal->addItem(tr("No"), 0);
                    mComboModal->addItem(tr("Yes"), 1);
                    connect(mComboModal, &QComboBox::currentIndexChanged, this, &TPropertiesGeneral::onComboModalChanged);
                    mTable->setCellWidget(i, 1, mComboModal);
                }
            break;

            case 13:
                if (stype == STATE_POPUP)
                {
                    cell1->setText(tr("Show effect"));
                    mComboShow = new QComboBox;
                    mComboShow->addItem(tr("None"), Page::SE_NONE);
                    mComboShow->addItem(tr("Fade"), Page::SE_FADE);
                    mComboShow->addItem(tr("Slide from left"), Page::SE_SLIDE_LEFT);
                    mComboShow->addItem(tr("Slide from right"), Page::SE_SLIDE_RIGHT);
                    mComboShow->addItem(tr("Slide from top"), Page::SE_SLIDE_TOP);
                    mComboShow->addItem(tr("Slide from bottom"), Page::SE_SLIDE_BOTTOM);
                    mComboShow->addItem(tr("Slide from left fade"), Page::SE_SLIDE_LEFT_FADE);
                    mComboShow->addItem(tr("Slide from right fade"), Page::SE_SLIDE_RIGHT_FADE);
                    mComboShow->addItem(tr("Slide from top fade"), Page::SE_SLIDE_TOP_FADE);
                    mComboShow->addItem(tr("Slide from bottom fade"), Page::SE_SLIDE_BOTTOM_FADE);
                    connect(mComboShow, &QComboBox::currentIndexChanged, this, &TPropertiesGeneral::onComboShowChanged);
                    mTable->setCellWidget(i, 1, mComboShow);
                }
            break;

            case 14:
                if (stype == STATE_POPUP)
                {
                    cell1->setText(tr("Hide effect"));
                    mComboHide = new QComboBox;
                    mComboHide->addItem(tr("None"), Page::SE_NONE);
                    mComboHide->addItem(tr("Fade"), Page::SE_FADE);
                    mComboHide->addItem(tr("Slide to left"), Page::SE_SLIDE_LEFT);
                    mComboHide->addItem(tr("Slide to right"), Page::SE_SLIDE_RIGHT);
                    mComboHide->addItem(tr("Slide to top"), Page::SE_SLIDE_TOP);
                    mComboHide->addItem(tr("Slide to bottom"), Page::SE_SLIDE_BOTTOM);
                    mComboHide->addItem(tr("Slide to left fade"), Page::SE_SLIDE_LEFT_FADE);
                    mComboHide->addItem(tr("Slide to right fade"), Page::SE_SLIDE_RIGHT_FADE);
                    mComboHide->addItem(tr("Slide to top fade"), Page::SE_SLIDE_TOP_FADE);
                    mComboHide->addItem(tr("Slide to bottom fade"), Page::SE_SLIDE_BOTTOM_FADE);
                    connect(mComboHide, &QComboBox::currentIndexChanged, this, &TPropertiesGeneral::onComboHideChanged);
                    mTable->setCellWidget(i, 1, mComboHide);
                }
            break;

            case 15:
                if (stype == STATE_POPUP)
                {
                    cell1->setText(tr("Collapse direction"));
                    mComboColapse = new QComboBox;
                    mComboColapse->addItem(tr("None"), Page::COLDIR_NONE);
                    mComboColapse->addItem(tr("Left"), Page::COLDIR_LEFT);
                    mComboColapse->addItem(tr("Right"), Page::COLDIR_RIGHT);
                    mComboColapse->addItem(tr("Up"), Page::COLDIR_UP);
                    mComboColapse->addItem(tr("Down"), Page::COLDIR_DOWN);
                    connect(mComboColapse, &QComboBox::currentIndexChanged, this, &TPropertiesGeneral::onComboColapseChange);
                    mTable->setCellWidget(i, 1, mComboColapse);
                }
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
    QList<QVariant> data = { 1, 2 };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);
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

    QStringList items = { "no", "yes" };
    QList<QVariant> data = { false, true };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);
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
    }

    TElementSpinBox *sbox = new TElementSpinBox(value, 0, 10000, name, mTable);
    connect(sbox, &TElementSpinBox::valueChanged, this, &TPropertiesGeneral::onSpinGeometryChanged);
    return sbox;
}

TElementWidgetCombo *TPropertiesGeneral::makePopupResetOnPos(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::makePopupResetOnPos(const QString& name)");

    QStringList items = { "no", "yes" };
    QList<QVariant> data = { false, true };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTable);
    combo->addItems(items);
    combo->addData(data);
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
    mComboGroupText->addItems(groups);

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

    int type = data.toInt();

    if (type == 1)
        mPage.popupType = Page::PT_POPUP;
    else
        mPage.popupType = Page::PT_SUBPAGE;

    mChanged = true;
    // TODO: Add code to change rows in mTable
}

void TPropertiesGeneral::onComboButtonTypeChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onComboButtonTypeChanged(const QString& text, const QVariant& data, const QString& name)");

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
    mChanged = true;
    markChanged();
}

void TPropertiesGeneral::onComboLockButtonName(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onComboLockButtonName(const QString& text, const QVariant& data, const QString& name)");

    if (mActObjectID < 0 || mPage.objects.size() <= mActObjectID)
        return;

    mActObject = mPage.objects[mActObjectID]->getObject();
    mActObject.li = data.toBool();
    mPage.objects[mActObjectID]->setObject(mActObject);
    mChanged = true;
    markChanged();
}

void TPropertiesGeneral::onObjectNameChanged(const QString& text, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onObjectNameChanged(const QString& text, const QString& name)");

    if (mActObjectID < 0 || mPage.objects.size() <= mActObjectID)
        return;

    mActObject = mPage.objects[mActObjectID]->getObject();
    mActObject.na = text;
    mPage.objects[mActObjectID]->setObject(mActObject);
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
    saveChangedData(&mPage, TBL_GENERIC);
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
    saveChangedData(&mPage, TBL_GENERIC);
}

void TPropertiesGeneral::onPopupTimeout(int value, const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::onPopupTimeout(int value, const QString& name)");

    mPage.timeout = value;
    mChanged = true;
    saveChangedData(&mPage, TBL_GENERIC);
}

void TPropertiesGeneral::onSpinLeftValue(int value)
{
    DECL_TRACER("TPropertiesGeneral::onSpinLeftValue(int value)");

    mPage.left = value;
    saveChangedData(&mPage, TBL_GENERIC);
    mChanged = true;
}

void TPropertiesGeneral::onSpinTopValue(int value)
{
    DECL_TRACER("TPropertiesGeneral::onSpinTopValue(int value)");

    mPage.top = value;
    saveChangedData(&mPage, TBL_GENERIC);
    mChanged = true;
}

void TPropertiesGeneral::onSpinWidthValue(int value)
{
    DECL_TRACER("TPropertiesGeneral::onSpinWidthValue(int value)");

    mPage.width = value;
    saveChangedData(&mPage, TBL_GENERIC);
    mChanged = true;
}

void TPropertiesGeneral::onSpinHeightValue(int value)
{
    DECL_TRACER("TPropertiesGeneral::onSpinHeightValue(int value)");

    mPage.height = value;
    saveChangedData(&mPage, TBL_GENERIC);
    mChanged = true;
}

void TPropertiesGeneral::onComboResetPosChanged(int index)
{
    DECL_TRACER("TPropertiesGeneral::onComboResetPosChanged(int index)");

    mPage.resetPos = mComboResetPos->itemData(index).toInt();
    saveChangedData(&mPage, TBL_GENERIC);
    mChanged = true;
}

void TPropertiesGeneral::onComboGroupTextChanged(const QString& text)
{
    DECL_TRACER("TPropertiesGeneral::onComboGroupTextChanged(const QString& text)");

    if (mPage.group != text)
    {
        mPage.group = text;
        mChanged = false;
        saveChangedData(&mPage, TBL_GENERIC);
    }
}

void TPropertiesGeneral::onSpinTimeoutValue(int value)
{
    DECL_TRACER("TPropertiesGeneral::onSpinTimeoutValue(int value)");

    mPage.timeout = value;
    saveChangedData(&mPage, TBL_GENERIC);
    mChanged = true;
}

void TPropertiesGeneral::onComboModalChanged(int index)
{
    DECL_TRACER("TPropertiesGeneral::onComboModalChanged(int index)");

    mPage.modal = mComboModal->itemData(index).toInt();
    saveChangedData(&mPage, TBL_GENERIC);
    mChanged = true;
}

void TPropertiesGeneral::onComboShowChanged(int index)
{
    DECL_TRACER("TPropertiesGeneral::onComboShowChanged(int index)");

    mPage.showEffect = static_cast<Page::SHOWEFFECT>(mComboShow->itemData(index).toInt());
    saveChangedData(&mPage, TBL_GENERIC);
    mChanged = true;
}

void TPropertiesGeneral::onComboHideChanged(int index)
{
    DECL_TRACER("TPropertiesGeneral::onComboHideChanged(int index)");

    mPage.hideEffect = static_cast<Page::SHOWEFFECT>(mComboHide->itemData(index).toInt());
    saveChangedData(&mPage, TBL_GENERIC);
    mChanged = true;
}

void TPropertiesGeneral::onComboColapseChange(int index)
{
    DECL_TRACER("TPropertiesGeneral::onComboColapseChange(int index)");

    mPage.collapseDirection = static_cast<Page::COLDIR_t>(mComboColapse->itemData(index).toInt());
    saveChangedData(&mPage, TBL_GENERIC);
    mChanged = true;
}
