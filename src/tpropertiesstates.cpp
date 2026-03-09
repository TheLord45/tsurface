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
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QBrush>
#include <QComboBox>
#include <QColorDialog>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QHeaderView>
#include <QFontDialog>
#include <QSpinBox>

#include "tpropertiesstates.h"
#include "telementbitmapselector.h"
#include "telementwidgetcombo.h"
#include "telementwidgetfont.h"
#include "telementtexteffect.h"
#include "telementbordername.h"
#include "telementgradientcolors.h"
#include "telementsound.h"
#include "telementcolorselector.h"
#include "telementwidgettext.h"
#include "tconfmain.h"
#include "terror.h"

using namespace Page;

#define TTEXT_BORDER_NAME           0
#define TTEXT_CHAMELEON_IMAGE       1
#define TTEXT_BORDER_COLOR          2
#define TTEXT_FILL_TYPE             3
#define TTEXT_FILL_GRADIENT_COLORS  4
#define TTEXT_GRADIENT_RADIUS       5
#define TTEXT_GRADIENT_CENTER_X     6
#define TTEXT_GRADIENT_CENTER_Y     7
#define TTEXT_FILL_COLOR            8
#define TTEXT_TEXT_COLOR            9
#define TTEXT_TEXT_EFFECT_COLOR     10
#define TTEXT_OVERALL_OPACITY       11
#define TTEXT_VIDEO_FILL            12
#define TTEXT_STREAMING_SOURCE      13
#define TTEXT_BITMAPS               14
#define TTEXT_FONT                  15
#define TTEXT_FONT_SIZE             16
#define TTEXT_TEXT                  17
#define TTEXT_TEXT_JUSTIFICATION    18
#define TTEXT_TEXT_POSITION_X       19
#define TTEXT_TEXT_POSITION_Y       20
#define TTEXT_TEXT_EFFECT           21
#define TTEXT_WORD_WRAP             22
#define TTEXT_SOUND                 23

#define TTEXT_MAX                   24

TPropertiesStates::TPropertiesStates(QTreeWidget *widget)
    : mTreeWidget(widget)
{
    DECL_TRACER("TPropertiesStates::TPropertiesStates(QTreeWidget *widget)");
}

TPropertiesStates::~TPropertiesStates()
{
    DECL_TRACER("TPropertiesStates::~TPropertiesStates()");
}

/**
 * @brief TPropertiesStates::setPage
 * This method sets the @bold page. If the page is a new page then the old
 * page is saved in case there were changes. Then the new page is assigned
 * and created.
 * The method doesn't distinguish between a page or popup. The underlying
 * structure is the same for all kind of pages.
 *
 * @param page  The new page
 */
void TPropertiesStates::setPage(const Page::PAGE_t& page)
{
    DECL_TRACER("TPropertiesStates::setPage(const Page::PAGE_t& page)");

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_STATES);

    mChanged = false;
    mPage = page;
    mActInstance = -1;
    mActObjectID = -1;
    setSType();
    createPage(true);
}

void TPropertiesStates::setActualObject(int index, STATE_TYPE stype)
{
    DECL_TRACER("TPropertiesStates::setActualObject(int index, STATE_TYPE stype)");

    if (index <= 0 || index >= mPage.objects.size() || mActObjectID == index)
        return;

    mActObjectID = index;
    mActInstance = -1;
    mSType = stype;

    if (isValidObjectIndex())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();
        mActSr = mPage.objects[mActObjectID]->getSrCommon();
    }

    createPage(true);
    // Show the states of the object
    int inst = -1;

    for (QTableWidget *w : mStates)
    {
        setTable(w, inst);
        inst++;
    }
}

void TPropertiesStates::setObjectType(ObjHandler::BUTTONTYPE btype, int index)
{
    DECL_TRACER("TPropertiesStates::setObjectType(ObjHandler::BUTTONTYPE btype, int index)");

    if (index < 0 || index >= mPage.objects.size())
        return;

    bool force = false;

    if (mActObjectID != index)
    {
        MSG_WARNING("The new object id is different! Actual ID: " << mActObjectID << ", new ID: " << index);
        force = true;
    }

    mActObjectID = index;

    if (mPage.objects[index]->getType() != btype)
    {
        MSG_WARNING("Object " << index << " changed type from " << mPage.objects[index]->getType() << " to " << btype << "!");
        mPage.objects[index]->setObjectType(btype);
    }

    setSType();
    createPage(force);
}

void TPropertiesStates::setState(STATE_TYPE stype)
{
    DECL_TRACER("TPropertiesStates::setState(STATE_TYPE stype)");

    if (stype != mSType)
    {
        mSType = stype;
        createPage(true);
    }
    else
        createPage(false);
}

void TPropertiesStates::clear()
{
    DECL_TRACER("TPropertiesStates::clear()");

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_STATES);

    mChanged = true;
    mPage = Page::PAGE_t();
    mTreeWidget->clear();
    mTreeWidget->setColumnCount(0);
}

void TPropertiesStates::setTable(QTableWidget *table, int instance)
{
    DECL_TRACER("TPropertiesStates::setTable(QTableWidget *table, int instance)");

    if (!table)
        return;

    for (int i = 0; i < table->rowCount(); ++i)     // Hide all rows
        table->setRowHidden(i, true);

    int totalHeight = 0;    // Calculate the total height of the table widget depending on the visible lines.

    if (!isValidObjectIndex())
    {
        if (mPage.popupType == Page::PT_PAGE)
        {
            table->setRowHidden(TTEXT_FILL_TYPE, false);
            totalHeight += setTableWidget(table, TTEXT_FILL_TYPE, 1, mPage.srPage.ft, W_COMBO);

            if (!mPage.srPage.ft.isEmpty() && mPage.srPage.ft != "solid")
            {
                table->setRowHidden(TTEXT_FILL_GRADIENT_COLORS, false);
                totalHeight += setTableWidget(table, TTEXT_FILL_GRADIENT_COLORS, 1, mPage.srPage.gradientColors, W_GRADIENTCOLORS);

                if (mPage.srPage.ft == "radial")
                {
                    table->setRowHidden(TTEXT_GRADIENT_RADIUS, false);
                    table->setRowHidden(TTEXT_GRADIENT_CENTER_X, false);
                    table->setRowHidden(TTEXT_GRADIENT_CENTER_Y, false);

                    totalHeight += setTableWidget(table, TTEXT_GRADIENT_RADIUS, 1, mPage.srPage.gr, W_SPINBOX);
                    totalHeight += setTableWidget(table, TTEXT_GRADIENT_RADIUS, 1, mPage.srPage.gx, W_SPINBOX);
                    totalHeight += setTableWidget(table, TTEXT_GRADIENT_RADIUS, 1, mPage.srPage.gy, W_SPINBOX);
                }
            }
            else
            {
                table->setRowHidden(TTEXT_FILL_COLOR, false);
                totalHeight += setTableWidget(table, TTEXT_FILL_COLOR, 1, mPage.srPage.cf, W_COLORSELECTOR);
            }

            table->setRowHidden(TTEXT_TEXT_COLOR, false);
            table->setRowHidden(TTEXT_TEXT_EFFECT_COLOR, false);
            table->setRowHidden(TTEXT_VIDEO_FILL, false);

            totalHeight += setTableWidget(table, TTEXT_TEXT_COLOR, 1, mPage.srPage.ct, W_COLORSELECTOR);
            totalHeight += setTableWidget(table, TTEXT_TEXT_EFFECT_COLOR, 1, mPage.srPage.ec, W_COLORSELECTOR);
            totalHeight += setTableWidget(table, TTEXT_VIDEO_FILL, 1, mPage.srPage.vf, W_COMBO);

            if (mPage.srPage.vf == "100")
            {
                table->setRowHidden(TTEXT_STREAMING_SOURCE, false);
                totalHeight += setTableWidget(table, TTEXT_STREAMING_SOURCE, 1, mPage.srPage.dv, W_TEXT);
            }

            table->setRowHidden(TTEXT_BITMAPS, false);
            table->setRowHidden(TTEXT_FONT, false);
            table->setRowHidden(TTEXT_FONT_SIZE, false);
            table->setRowHidden(TTEXT_TEXT, false);
            table->setRowHidden(TTEXT_TEXT_JUSTIFICATION, false);

            totalHeight += setTableWidget(table, TTEXT_BITMAPS, 1, mPage.srPage.bitmaps, W_BITMAPSELECTOR);
            totalHeight += setTableWidget(table, TTEXT_FONT, 1, mPage.srPage.ff, W_FONT);
            totalHeight += setTableWidget(table, TTEXT_FONT_SIZE, 1, mPage.srPage.fs, W_SPINBOX);
            totalHeight += setTableWidget(table, TTEXT_TEXT, 1, mPage.srPage.te, W_TEXT);
            totalHeight += setTableWidget(table, TTEXT_TEXT_JUSTIFICATION, 1, mPage.srPage.jt, W_COMBO);

            if (mPage.srPage.jt == ObjHandler::ORI_ABSOLUT)
            {
                table->setRowHidden(TTEXT_TEXT_POSITION_X, false);
                table->setRowHidden(TTEXT_TEXT_POSITION_Y, false);

                totalHeight += setTableWidget(table, TTEXT_TEXT_POSITION_X, 1, mPage.srPage.tx, W_SPINBOX);
                totalHeight += setTableWidget(table, TTEXT_TEXT_POSITION_Y, 1, mPage.srPage.ty, W_SPINBOX);
            }

            table->setRowHidden(TTEXT_TEXT_EFFECT, false);
            table->setRowHidden(TTEXT_WORD_WRAP, false);

            totalHeight += setTableWidget(table, TTEXT_TEXT_EFFECT, 1, mPage.srPage.et, W_TEXTEFFECT);
            totalHeight += setTableWidget(table, TTEXT_WORD_WRAP, 1, mPage.srPage.ty, W_COMBO);
        }
        else if (mPage.popupType == Page::PT_POPUP || mPage.popupType == Page::PT_SUBPAGE)
        {
            table->setRowHidden(TTEXT_BORDER_NAME, false);
            totalHeight += setTableWidget(table, TTEXT_BORDER_NAME, 1, mPage.srPage.bs, W_BORDERNAME);

            if (mPage.srPage.bs.isEmpty())
            {
                table->setRowHidden(TTEXT_CHAMELEON_IMAGE, false);
                totalHeight += setTableWidget(table, TTEXT_CHAMELEON_IMAGE, 1, mPage.srPage.mi, W_BITMAPSELECTOR);
            }

            table->setRowHidden(TTEXT_BORDER_COLOR, false);
            table->setRowHidden(TTEXT_FILL_TYPE, false);

            totalHeight += setTableWidget(table, TTEXT_BORDER_COLOR, 1, mPage.srPage.cb, W_COLORSELECTOR);
            totalHeight += setTableWidget(table, TTEXT_FILL_TYPE, 1, mPage.srPage.ty, W_COMBO);

            if (!mPage.srPage.ft.isEmpty() && mPage.srPage.ft != "solid")
            {
                table->setRowHidden(TTEXT_FILL_GRADIENT_COLORS, false);
                totalHeight += setTableWidget(table, TTEXT_FILL_GRADIENT_COLORS, 1, mPage.srPage.gradientColors, W_GRADIENTCOLORS);

                if (mPage.srPage.ft == "radial")
                {
                    table->setRowHidden(TTEXT_GRADIENT_RADIUS, false);
                    table->setRowHidden(TTEXT_GRADIENT_CENTER_X, false);
                    table->setRowHidden(TTEXT_GRADIENT_CENTER_Y, false);

                    totalHeight += setTableWidget(table, TTEXT_GRADIENT_RADIUS, 1, mPage.srPage.gr, W_SPINBOX);
                    totalHeight += setTableWidget(table, TTEXT_GRADIENT_CENTER_X, 1, mPage.srPage.gx, W_SPINBOX);
                    totalHeight += setTableWidget(table, TTEXT_GRADIENT_CENTER_Y, 1, mPage.srPage.gy, W_SPINBOX);
                }
            }
            else
            {
                table->setRowHidden(TTEXT_FILL_COLOR, false);
                totalHeight += setTableWidget(table, TTEXT_FILL_COLOR, 1, mPage.srPage.cf, W_COLORSELECTOR);
            }

            table->setRowHidden(TTEXT_TEXT_COLOR, false);
            table->setRowHidden(TTEXT_TEXT_EFFECT_COLOR, false);
            table->setRowHidden(TTEXT_VIDEO_FILL, false);

            totalHeight += setTableWidget(table, TTEXT_TEXT_COLOR, 1, mPage.srPage.ct, W_COLORSELECTOR);
            totalHeight += setTableWidget(table, TTEXT_TEXT_EFFECT_COLOR, 1, mPage.srPage.ec, W_COLORSELECTOR);
            totalHeight += setTableWidget(table, TTEXT_VIDEO_FILL, 1, mPage.srPage.vf, W_COMBO);

            if (mPage.srPage.vf == "100")
            {
                table->setRowHidden(TTEXT_STREAMING_SOURCE, false);
                totalHeight += setTableWidget(table, TTEXT_STREAMING_SOURCE, 1, mPage.srPage.dv, W_TEXT);
            }

            table->setRowHidden(TTEXT_BITMAPS, false);
            table->setRowHidden(TTEXT_FONT, false);
            table->setRowHidden(TTEXT_FONT_SIZE, false);
            table->setRowHidden(TTEXT_TEXT, false);
            table->setRowHidden(TTEXT_TEXT_JUSTIFICATION, false);

            totalHeight += setTableWidget(table, TTEXT_BITMAPS, 1, mPage.srPage.bitmaps, W_BITMAPSELECTOR);
            totalHeight += setTableWidget(table, TTEXT_FONT, 1, mPage.srPage.ff, W_FONT);
            totalHeight += setTableWidget(table, TTEXT_FONT_SIZE, 1, mPage.srPage.fs, W_SPINBOX);
            totalHeight += setTableWidget(table, TTEXT_TEXT, 1, mPage.srPage.te, W_TEXT);
            totalHeight += setTableWidget(table, TTEXT_TEXT_JUSTIFICATION, 1, mPage.srPage.jt, W_COMBO);

            if (mPage.srPage.jt == ObjHandler::ORI_ABSOLUT)
            {
                table->setRowHidden(TTEXT_TEXT_POSITION_X, false);
                table->setRowHidden(TTEXT_TEXT_POSITION_Y, false);

                totalHeight += setTableWidget(table, TTEXT_TEXT_POSITION_X, 1, mPage.srPage.tx, W_SPINBOX);
                totalHeight += setTableWidget(table, TTEXT_TEXT_POSITION_Y, 1, mPage.srPage.ty, W_SPINBOX);
            }

            table->setRowHidden(TTEXT_TEXT_EFFECT, false);
            table->setRowHidden(TTEXT_WORD_WRAP, false);

            totalHeight += setTableWidget(table, TTEXT_TEXT_EFFECT, 1, mPage.srPage.et, W_TEXTEFFECT);
            totalHeight += setTableWidget(table, TTEXT_WORD_WRAP, 1, mPage.srPage.ty, W_COMBO);
        }
    }
    else
    {
        mActObject = mPage.objects[mActObjectID]->getObject();
        mActSr = mPage.objects[mActObjectID]->getSrFromIndex(instance);
        mActInstance = instance;

        if (instance >= mActObject.sr.size())
        {
            MSG_ERROR("Instance " << instance << " is out of range! Have " << mActObject.sr.size() << " states.");
            return;
        }

        if (mActObject.type != ObjHandler::LISTVIEW)
        {
            table->setRowHidden(TTEXT_BORDER_NAME, false);
            totalHeight += setTableWidget(table, TTEXT_BORDER_NAME, 1, mActSr.bs, W_BORDERNAME);


            if (mPage.srPage.bs.isEmpty())
            {
                table->setRowHidden(TTEXT_CHAMELEON_IMAGE, false);
                totalHeight += setTableWidget(table, TTEXT_CHAMELEON_IMAGE, 1, mActSr.mi, W_BITMAPSELECTOR);
            }
        }

        table->setRowHidden(TTEXT_BORDER_COLOR, false);
        table->setRowHidden(TTEXT_FILL_TYPE, false);

        totalHeight += setTableWidget(table, TTEXT_BORDER_COLOR, 1, mActSr.cb, W_COLORSELECTOR);
        totalHeight += setTableWidget(table, TTEXT_FILL_TYPE, 1, mActSr.ty, W_COMBO);

        if (!mActSr.ft.isEmpty() && mActSr.ft != "solid")
        {
            table->setRowHidden(TTEXT_FILL_GRADIENT_COLORS, false);
            totalHeight += setTableWidget(table, TTEXT_FILL_GRADIENT_COLORS, 1, mActSr.gradientColors, W_GRADIENTCOLORS);

            if (mActSr.ft == "radial")
            {
                table->setRowHidden(TTEXT_GRADIENT_RADIUS, false);
                table->setRowHidden(TTEXT_GRADIENT_CENTER_X, false);
                table->setRowHidden(TTEXT_GRADIENT_CENTER_Y, false);

                totalHeight += setTableWidget(table, TTEXT_GRADIENT_RADIUS, 1, mActSr.gr, W_SPINBOX);
                totalHeight += setTableWidget(table, TTEXT_GRADIENT_CENTER_X, 1, mActSr.gx, W_SPINBOX);
                totalHeight += setTableWidget(table, TTEXT_GRADIENT_CENTER_Y, 1, mActSr.gy, W_SPINBOX);
            }
        }
        else
        {
            table->setRowHidden(TTEXT_FILL_COLOR, false);
            totalHeight += setTableWidget(table, TTEXT_FILL_COLOR, 1, mActSr.cf, W_COLORSELECTOR);
        }

        table->setRowHidden(TTEXT_TEXT_COLOR, false);
        table->setRowHidden(TTEXT_TEXT_EFFECT_COLOR, false);
        table->setRowHidden(TTEXT_OVERALL_OPACITY, false);

        totalHeight += setTableWidget(table, TTEXT_TEXT_COLOR, 1, mActSr.ct, W_COLORSELECTOR);
        totalHeight += setTableWidget(table, TTEXT_TEXT_EFFECT_COLOR, 1, mActSr.ec, W_COLORSELECTOR);
        totalHeight += setTableWidget(table, TTEXT_OVERALL_OPACITY, 1, mActSr.oo, W_SPINBOX);

        if (mActObject.type != ObjHandler::LISTVIEW)
        {
            table->setRowHidden(TTEXT_VIDEO_FILL, false);
            totalHeight += setTableWidget(table, TTEXT_VIDEO_FILL, 1, mActSr.vf, W_COMBO);

            if (mPage.srPage.vf == "100")
            {
                table->setRowHidden(TTEXT_STREAMING_SOURCE, false);
                totalHeight += setTableWidget(table, TTEXT_STREAMING_SOURCE, 1, mPage.srPage.dv, W_TEXT);
            }

            table->setRowHidden(TTEXT_BITMAPS, false);
            totalHeight += setTableWidget(table, TTEXT_BITMAPS, 1, mActSr.bitmaps, W_BITMAPSELECTOR);
        }

        table->setRowHidden(TTEXT_FONT, false);
        table->setRowHidden(TTEXT_FONT_SIZE, false);

        totalHeight += setTableWidget(table, TTEXT_FONT, 1, mActSr.ff, W_FONT);
        totalHeight += setTableWidget(table, TTEXT_FONT_SIZE, 1, mActSr.fs, W_SPINBOX);

        if (mActObject.type != ObjHandler::LISTVIEW)
        {
            table->setRowHidden(TTEXT_TEXT, false);
            table->setRowHidden(TTEXT_TEXT_JUSTIFICATION, false);

            totalHeight += setTableWidget(table, TTEXT_TEXT, 1, mActSr.te, W_TEXT);
            totalHeight += setTableWidget(table, TTEXT_TEXT_JUSTIFICATION, 1, mActSr.jt, W_COMBO);

            if (mPage.srPage.jt == ObjHandler::ORI_ABSOLUT)
            {
                table->setRowHidden(TTEXT_TEXT_POSITION_X, false);
                table->setRowHidden(TTEXT_TEXT_POSITION_Y, false);

                totalHeight += setTableWidget(table, TTEXT_TEXT_POSITION_X, 1, mActSr.tx, W_SPINBOX);
                totalHeight += setTableWidget(table, TTEXT_TEXT_POSITION_Y, 1, mActSr.ty, W_SPINBOX);
            }
        }

        table->setRowHidden(TTEXT_TEXT_EFFECT, false);
        totalHeight += setTableWidget(table, TTEXT_TEXT_EFFECT, 1, mActSr.et, W_TEXTEFFECT);

        if (mActObject.type != ObjHandler::LISTVIEW)
        {
            table->setRowHidden(TTEXT_WORD_WRAP, false);
            table->setRowHidden(TTEXT_SOUND, false);

            totalHeight += setTableWidget(table, TTEXT_WORD_WRAP, 1, mActSr.ty, W_COMBO);
            totalHeight += setTableWidget(table, TTEXT_SOUND, 1, mActSr.sd, W_SOUND);
        }
    }

    table->setMinimumHeight(totalHeight);
    mTreeWidget->setMaximumHeight(totalHeight + mTreeWidget->height());
}

void TPropertiesStates::createPage(bool force)
{
    DECL_TRACER("TPropertiesStates::createPage(bool force)");

    if (!mTreeWidget)
    {
        MSG_WARNING("Tree widget is not initialized!")
        return;
    }

    if (mInitialized && !force)
    {
        rebuildTree();
        return;
    }

    mStates.clear();
    mTreeWidget->clear();
    mTreeWidget->setColumnCount(1);
    mTreeWidget->setHeaderHidden(true);
    QSignalBlocker sigBlocker(mTreeWidget);

    QTreeWidgetItem *top = new QTreeWidgetItem(mTreeWidget);
    QString name;

    if (isAnyPage())
        name = tr("Off");
    else
        name = getStateName(mPage.objects[mActObjectID]->getType(), -1);

    top->setText(0, name);
    QTreeWidgetItem *item = new QTreeWidgetItem(top);
    item->setFirstColumnSpanned(true);
    item->setFlags(Qt::ItemIsEnabled);      // Not selectable

    mActInstance = -1;
    QTableWidget *table = createTableWidget(mTreeWidget);
    mStates.append(table);
    mTreeWidget->setItemWidget(item, 0, table);
    int totalHeight = table->height();

    if (!isAnyPage() && isValidObjectIndex())
    {
        ObjHandler::TOBJECT_t obj = mPage.objects[mActObjectID]->getObject();

        for (int i = 0; i < obj.sr.size(); ++i)
        {
            QTreeWidgetItem *entry = new QTreeWidgetItem(mTreeWidget);

            QString name = getStateName(obj.type, i);
            entry->setText(0, name);

            item = new QTreeWidgetItem(entry);
            item->setFirstColumnSpanned(true);
            item->setFlags(Qt::ItemIsEnabled);      // Not selectable

            mActInstance = i;
            table = createTableWidget(mTreeWidget);
            mStates.append(table);
            mTreeWidget->setItemWidget(item, 0, table);
//            totalHeight += table->height();
        }
    }

    MSG_DEBUG("Page total height: " << totalHeight);
    mTreeWidget->setMaximumHeight(totalHeight + mTreeWidget->height());

    rebuildTree();
    mInitialized = true;
}

QTableWidget *TPropertiesStates::createTableWidget(QWidget *parent)
{
    DECL_TRACER("TPropertiesStates::createTableWidget(STATE_TYPE stype, QWidget *parent)");

    QTableWidget *table = new QTableWidget(parent);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setVisible(false);
    table->setShowGrid(true);
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table->setColumnCount(2);

    createPage(table, mActInstance);

    table->resizeRowsToContents();
    table->resizeColumnsToContents();
    return table;
}

void TPropertiesStates::createPage(QTableWidget *table, int instance)
{
    DECL_TRACER("TPropertiesStates::createTablePage(QTableWidget *table, int instance)");

    mActObject = ObjHandler::TOBJECT_t();
    mActSr = ObjHandler::SR_T();

    if (mSType == STATE_PAGE || mSType == STATE_POPUP || mSType == STATE_SUBPAGE)
    {
        if (mPage.srPage.ff.isEmpty())
        {
            QFont font = mParent->font();
            mPage.srPage.ff = font.family();
        }

        if (mPage.srPage.fs <= 0)
            mPage.srPage.fs = 10;
    }
    else if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        mActObject = mPage.objects[mActObjectID]->getObject();

        if (instance < 0)
            mActSr = mPage.objects[mActObjectID]->getSrFromIndex(instance);

        if (mActSr.ff.isEmpty())
        {
            QFont font = TConfMain::Current().getFontBase();
            mActSr.ff = font.family();
        }

        if (mActSr.fs <= 0)
            mActSr.fs = 10;
    }
    else
        return;

    int rows = TTEXT_MAX;
    // The row count must be defined before rows are added to the table! Otherwise
    // an empty table with only the grid will be visible.
    table->setRowCount(rows);
//    int totalHeight = 0;

    for (int row = 0; row < rows; ++row)
    {
        QTableWidgetItem *col0 = new QTableWidgetItem;
        col0->setBackground(Qt::lightGray);
        col0->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);

        switch(row)
        {
            case TTEXT_BORDER_NAME:
                col0->setText(getLeftColText(TTEXT_BORDER_NAME));
                table->setCellWidget(row, 1, makeBorderName("PopupBorderName"));
            break;

            case TTEXT_CHAMELEON_IMAGE:
                col0->setText(getLeftColText(TTEXT_CHAMELEON_IMAGE));
                table->setCellWidget(row, 1, makeBitmapSelector("ChameleonImage"));
            break;

            case TTEXT_BORDER_COLOR:
                col0->setText(getLeftColText(TTEXT_BORDER_COLOR));
                table->setCellWidget(row, 1, makeColorSelector("BorderColor"));
            break;

            case TTEXT_FILL_TYPE:
                col0->setText(getLeftColText(TTEXT_FILL_TYPE));
                table->setCellWidget(row, 1, makeFillType("FillType"));
            break;

            case TTEXT_FILL_COLOR:
                col0->setText(getLeftColText(TTEXT_FILL_COLOR));
                table->setCellWidget(row, 1, makeColorSelector("FillColor"));
            break;

            case TTEXT_FILL_GRADIENT_COLORS:
                col0->setText(getLeftColText(TTEXT_FILL_GRADIENT_COLORS));
                table->setCellWidget(row, 1, makeGradientColors("FillGradientColors"));
            break;

            case TTEXT_GRADIENT_RADIUS:
                col0->setText(getLeftColText(TTEXT_GRADIENT_RADIUS));
                table->setCellWidget(row, 1, makeValueSelector("GradientRadius", 100000));
            break;

            case TTEXT_GRADIENT_CENTER_X:
                col0->setText(getLeftColText(TTEXT_GRADIENT_CENTER_X));
                table->setCellWidget(row, 1, makeValueSelector("GradientCenterX", 100000));
            break;

            case TTEXT_GRADIENT_CENTER_Y:
                col0->setText(getLeftColText(TTEXT_GRADIENT_CENTER_Y));
                table->setCellWidget(row, 1, makeValueSelector("GradientCenterX", 100000));
            break;

            case TTEXT_TEXT_COLOR:
                col0->setText(getLeftColText(TTEXT_TEXT_COLOR));
                table->setCellWidget(row, 1, makeColorSelector("TextColor"));
            break;

            case TTEXT_TEXT_EFFECT_COLOR:
                col0->setText(getLeftColText(TTEXT_TEXT_EFFECT_COLOR));
                table->setCellWidget(row, 1, makeColorSelector("TextEffectColor"));
            break;

            case TTEXT_OVERALL_OPACITY:
                col0->setText(getLeftColText(TTEXT_OVERALL_OPACITY));
                table->setCellWidget(row, 1, makeValueSelector("OverallOpacity", 255));
            break;

            case TTEXT_VIDEO_FILL:
                col0->setText(getLeftColText(TTEXT_VIDEO_FILL));
                table->setCellWidget(row, 1, makeVideoFill("VideoFill"));
            break;

            case TTEXT_STREAMING_SOURCE:
                col0->setText(getLeftColText(TTEXT_STREAMING_SOURCE));
                table->setCellWidget(row, 1, makeTextValue("StreamingSource"));
            break;

            case TTEXT_BITMAPS:
                col0->setText(getLeftColText(TTEXT_BITMAPS));
                table->setCellWidget(row, 1, makeBitmapSelector("BitmapSelector"));
            break;

            case TTEXT_FONT:
                col0->setText(getLeftColText(TTEXT_FONT));
                table->setCellWidget(row, 1, makeFontSelector("FontSelector"));
            break;

            case TTEXT_FONT_SIZE:
                col0->setText(getLeftColText(TTEXT_FONT_SIZE));
                table->setCellWidget(row, 1, makeValueSelector("FontSize", 500));
            break;

            case TTEXT_TEXT:
                col0->setText(getLeftColText(TTEXT_TEXT));
                table->setCellWidget(row, 1, makeTextValue("Text"));
            break;

            case TTEXT_TEXT_JUSTIFICATION:
                col0->setText(getLeftColText(TTEXT_TEXT_JUSTIFICATION));
                table->setCellWidget(row, 1, makeTextJustification("TextOrientation"));
            break;

            case TTEXT_TEXT_POSITION_X:
                col0->setText(getLeftColText(TTEXT_TEXT_POSITION_X));
                table->setCellWidget(row, 1, makeValueSelector("TextPositionX", 100000));
            break;

            case TTEXT_TEXT_POSITION_Y:
                col0->setText(getLeftColText(TTEXT_TEXT_POSITION_Y));
                table->setCellWidget(row, 1, makeValueSelector("TextPositionY", 100000));
            break;

            case TTEXT_TEXT_EFFECT:
                col0->setText(getLeftColText(TTEXT_TEXT_EFFECT));
                table->setCellWidget(row, 1, makeTextEffect("TextEffect"));
            break;

            case TTEXT_WORD_WRAP:
                col0->setText(getLeftColText(TTEXT_WORD_WRAP));
                table->setCellWidget(row, 1, makeWordWrap("WordWrap"));
            break;

            case TTEXT_SOUND:
                col0->setText(getLeftColText(TTEXT_SOUND));
                table->setCellWidget(row, 1, makeSoundSelector("Sound"));
            break;
        }

//        totalHeight += table->cellWidget(row, 1)->size().height();
        table->setItem(row, 0, col0);
    }

//    table->setMaximumHeight(totalHeight);
}

QString TPropertiesStates::getLeftColText(int line)
{
    DECL_TRACER("TPropertiesStates::getLeftColText(int line)");

    switch (line)
    {
        case TTEXT_BORDER_NAME:         return tr("Border Name"); break;
        case TTEXT_CHAMELEON_IMAGE:     return tr("Chameleon Image"); break;   // Only if there is no border
        case TTEXT_BORDER_COLOR:        return tr("Border Color"); break;
        case TTEXT_FILL_TYPE:           return tr("Fill Type"); break;
        case TTEXT_FILL_GRADIENT_COLORS: return tr("Gradient Colors"); break;
        case TTEXT_GRADIENT_RADIUS:     return tr("Gradient Radius"); break;
        case TTEXT_GRADIENT_CENTER_X:   return tr("Gradient Center X%"); break;
        case TTEXT_GRADIENT_CENTER_Y:   return tr("Gradient Center Y%"); break;
        case TTEXT_FILL_COLOR:          return tr("Fill Color"); break;
        case TTEXT_TEXT_COLOR:          return tr("Text Color"); break;
        case TTEXT_TEXT_EFFECT_COLOR:   return tr("Text Effect Color"); break;
        case TTEXT_OVERALL_OPACITY:     return tr("Overall Opacity"); break;
        case TTEXT_VIDEO_FILL:          return tr("Video Fill"); break;
        case TTEXT_STREAMING_SOURCE:    return tr("Streaming Source"); break;
        case TTEXT_BITMAPS:             return tr("Bitmaps"); break;
        case TTEXT_FONT:                return tr("Font"); break;
        case TTEXT_FONT_SIZE:           return tr("Font Size"); break;
        case TTEXT_TEXT:                return tr("Text"); break;
        case TTEXT_TEXT_JUSTIFICATION:  return tr("Text Justification"); break;
        case TTEXT_TEXT_POSITION_X:     return tr("Absolut Position X"); break;
        case TTEXT_TEXT_POSITION_Y:     return tr("Absolut Position Y"); break;
        case TTEXT_TEXT_EFFECT:         return tr("Text Effect"); break;
        case TTEXT_WORD_WRAP:           return tr("Word Wrap"); break;
        case TTEXT_SOUND:               return tr("Sound"); break;
    }

    return "???";
}

void TPropertiesStates::setSType()
{
    DECL_TRACER("TPropertiesStates::setSType()");

    if (!isValidObjectIndex())
    {
        if (mPage.popupType == Page::PT_PAGE)
            mSType = STATE_PAGE;
        else if (mPage.popupType == Page::PT_POPUP)
            mSType = STATE_POPUP;
        else if (mPage.popupType == Page::PT_SUBPAGE)
            mSType = STATE_SUBPAGE;
        else
            mSType = STATE_UNKNOWN;

        MSG_DEBUG("State was set to page type " << mSType);
        return;
    }

    ObjHandler::BUTTONTYPE btype = mPage.objects[mActObjectID]->getType();

    switch(btype)
    {
        case ObjHandler::GENERAL:
        case ObjHandler::MULTISTATE_GENERAL:
            mSType = STATE_BUTTON;
        break;

        case ObjHandler::BARGRAPH:
        case ObjHandler::MULTISTATE_BARGRAPH:
            mSType = STATE_BARGRAPH;
        break;

        case ObjHandler::TEXT_INPUT:
            mSType = STATE_INPUT;
        break;

        case ObjHandler::LISTVIEW:
            mSType = STATE_LISTVIEW;
        break;

        case ObjHandler::SUBPAGE_VIEW:
            mSType = STATE_SUBPAGE;
        break;

        default:
            mSType = STATE_UNKNOWN;
    }

    MSG_DEBUG("State for object " << mActObjectID << " is " << mSType);
}

bool TPropertiesStates::isAnyPage()
{
    DECL_TRACER("TPropertiesStates::isAnyPage()");

    if (mSType == STATE_PAGE || mSType == STATE_POPUP || mSType == STATE_SUBPAGE)
        return true;

    return false;
}

bool TPropertiesStates::isValidObjectIndex()
{
    DECL_TRACER("TPropertiesStates::isValidObjectIndex()");

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
        return true;

    return false;
}

void TPropertiesStates::rebuildTree()
{
    DECL_TRACER("TPropertiesStates::rebuildTree()");

    MSG_DEBUG("Have " << mStates.size() << " states.");

    for (int i = 0; i < mStates.size(); ++i)
    {
        mActState = i - 1;
        setTable(mStates[i], mActState);
    }
}

QString TPropertiesStates::getStateName(ObjHandler::BUTTONTYPE type, int instance)
{
    DECL_TRACER("TPropertiesStates::getStateName(ObjHandler::BUTTONTYPE type, int instance)");

    QString name;

    switch(type)
    {
        case ObjHandler::GENERAL:
        case ObjHandler::BARGRAPH:
        case ObjHandler::TEXT_INPUT:
            switch(instance)
            {
                case 0: name = tr("Off"); break;
                case 1: name = tr("On"); break;
                default:
                    name = tr("All States");
            }
        break;

        case ObjHandler::MULTISTATE_GENERAL:
        case ObjHandler::MULTISTATE_BARGRAPH:
            if (instance >= 0)
                name = tr("State %1").arg(instance + 1);
            else
                name = tr("All States");
        break;

        case ObjHandler::LISTVIEW:
            if (instance == 0)
                name = tr("Default");
            else if (instance == 1)
                name = tr("Selected");
            else
                name = tr("All States");
        break;

        default:
            name = tr("Off");
    }

    return name;
}

int TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QVariant& data, ELEMENT_TYPE_t etype)
{
    DECL_TRACER("TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QVariant& data, ELEMENT_TYPE_t etype)");

    if (!table)
    {
        table = getTableWidget(mActState);

        if (!table)
            return 0;
    }

    QWidget *w = table->cellWidget(row, col);

    if (!w)
        return 0;

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

        case W_TEXTEFFECT:
        {
            TElementTextEffect *p = static_cast<TElementTextEffect *>(w);
            p->setEffect(data.toInt());
        }
        break;

        case W_SOUND:
        {
            TElementSound *p = static_cast<TElementSound *>(w);
            p->setSound(data.toString());
        }
        break;

        case W_TEXT:
        {
            TElementWidgetText *p = static_cast<TElementWidgetText *>(w);
            p->setText(data.toString());
        }
        break;

        case W_COLORSELECTOR:
        {
            TElementColorSelector *p = static_cast<TElementColorSelector *>(w);
            p->setColor(data.toString());
        }

        default:
            return w->height();
    }

    return w->height();     // Return the height of 1 row.
}

int TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QList<ObjHandler::BITMAPS_t>& bm, ELEMENT_TYPE_t etype)
{
    DECL_TRACER("TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QList<ObjHandler::BITMAPS_t>& bm, ELEMENT_TYPE_t etype)");

    if (!table)
    {
        table = getTableWidget(mActState);

        if (!table)
            return 0;
    }

    QWidget *w = table->cellWidget(row, col);

    if (!w)
        return 0;

    if (etype == W_BITMAPSELECTOR)
    {
        TElementBitmapSelector *p = static_cast<TElementBitmapSelector *>(w);
        p->setBitmaps(bm);
    }

    return w->height();
}

int TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QList<QColor>& colors, ELEMENT_TYPE_t etype)
{
    DECL_TRACER("TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QList<QColor>& bm, ELEMENT_TYPE_t etype)");

    if (!table)
    {
        table = getTableWidget(mActState);

        if (!table)
            return 0;
    }

    QWidget *w = table->cellWidget(row, col);

    if (!w)
        return 0;

    if (etype == W_GRADIENTCOLORS)
    {
        TElementGradientColors *p = static_cast<TElementGradientColors *>(w);
        p->setGradientColors(colors);
    }

    return w->height();
}

int TPropertiesStates::setTableWidgetFont(QTableWidget *table, int row, int col, const QFont& font, ELEMENT_TYPE_t etype)
{
    DECL_TRACER("TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QFont& font, ELEMENT_TYPE_t etype)");

    if (!table)
    {
        table = getTableWidget(mActState);

        if (!table)
            return 0;
    }

    QWidget *w = table->cellWidget(row, col);

    if (!w)
        return 0;

    if (etype == W_FONT)
    {
        TElementWidgetFont *p = static_cast<TElementWidgetFont *>(w);
        p->setFont(font);
    }

    return w->height();
}

QTableWidget *TPropertiesStates::getTableWidget(int state)
{
    DECL_TRACER("TPropertiesStates::getTableWidget(int state)");

    if (state < 0 || state >= mStates.size())
        return nullptr;

    return mStates[state];
}

TElementBorderName *TPropertiesStates::makeBorderName(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeBorderName(const QString& name)");

    QString border;

    if (isAnyPage())
        border = mPage.srPage.bs;
    else if (isValidObjectIndex())
        border = mActSr.bs;

    TElementBorderName *brd = new TElementBorderName(border, name, mParent);
    brd->setInstance(mActInstance);
    connect(brd, &TElementBorderName::borderChangedInst, this, &TPropertiesStates::onBorderNameChanged);
    return brd;
}

QComboBox *TPropertiesStates::makeFillType(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeFillType(const QString& name)");

    QList<QString> list = {
        "solid", "radial", "sweep", "left to right",
        "top-left to bottom-right", "top to bottom",
        "top-right to bottom-left", "right to left",
        "bottom-right to top-left", "bottom to top",
        "bottom-left to top-right"
    };

    QList<QVariant> data = {
        "solid",
        "radial",               // 01: Radial
        "sweep",                // 00: Sweep
        "linearCLCR",           // 02: Left to right
        "linearTLBR",           // 03: Top-left to Bottom-right
        "linearCTCB",           // 04: Top to bottom
        "linearTRBL",           // 05: Top-right to Bottom-left
        "linearCRCL",           // 06: Right to left
        "linearBRTL",           // 07: Bottom-right to top-left
        "linearCBCT",           // 08: Bottom to top
        "linearBLTR"            // 09: Bottom-left to top-right
    };

    QString ftype;

    if (isAnyPage())
        ftype = mPage.srPage.ft;
    else if (isValidObjectIndex())
        ftype = mActSr.ft;

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTreeWidget);
    combo->addItems(list);
    combo->addData(data);
    combo->setDefaultData(ftype);
    combo->setInstance(mActInstance);
    connect(combo, &TElementWidgetCombo::selectionChangedInst, this, &TPropertiesStates::onFillTypeChanged);
    return combo;
}

QWidget *TPropertiesStates::makeColorSelector(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeColorSelector(const QString& name)");

    QColor col;

    if (isAnyPage())
    {
        if (name == "BorderColor")
            col = mPage.srPage.cb;
        else if (name == "TextColor")
            col = mPage.srPage.ct;
        else if (name == "TextEffectColor")
            col = mPage.srPage.ec;
    }
    else if (isValidObjectIndex())
    {
        if (name == "BorderColor")
            col = mActSr.cb;
        else if (name == "TextColor")
            col = mActSr.ct;
        else if (name == "TextEffectColor")
            col = mActSr.ec;
    }

    TElementColorSelector *colsel = new TElementColorSelector(name, mTreeWidget);
    colsel->setColor(col);
    colsel->setInstance(mActInstance);

    connect(colsel, &TElementColorSelector::colorChangedInst, this, &TPropertiesStates::onColorChanged);
    return colsel;
}

QWidget *TPropertiesStates::makeVideoFill(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeVideoFill(const QString& name)");

    QList<QString> items = { "none", "streaming video", "MXA-MPL" };
    QList<QVariant> data = { "", "100", "101" };
    QString vf;

    if (isAnyPage())
        vf = mPage.srPage.vf;
    else if (isValidObjectIndex())
        vf = mActSr.vf;

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTreeWidget);
    combo->addItems(items);
    combo->addData(data);
    combo->setInstance(mActInstance);

    if (vf == "100")
        combo->setCurrentIndex(1);
    else if (vf == "101")
        combo->setCurrentIndex(2);

    connect(combo, &TElementWidgetCombo::selectionChangedInst, this, &TPropertiesStates::onVideoFillChanged);
    return combo;
}

TElementBitmapSelector *TPropertiesStates::makeBitmapSelector(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeBitmapSelector(const QString& name)");

    QList<ObjHandler::BITMAPS_t> bitmaps;

    if (isAnyPage())
        bitmaps = mPage.srPage.bitmaps;
    else if (isValidObjectIndex())
        bitmaps = mActSr.bitmaps;

    TElementBitmapSelector *bs = new TElementBitmapSelector(name, bitmaps, mParent);
    bs->setInstance(mActInstance);
    connect(bs, &TElementBitmapSelector::bitmapsChangedInst, this, &TPropertiesStates::onBitmapsChanged);
    return bs;
}

TElementWidgetFont *TPropertiesStates::makeFontSelector(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeFontSelector(const QString& fname, const QString& name)");

    QString ff;

    if (isAnyPage())
        ff = mPage.srPage.ff;
    else if (isValidObjectIndex())
        ff = mActSr.ff;

    TElementWidgetFont *widget = new TElementWidgetFont(QFont(ff), name, mParent);
    widget->setInstance(mActInstance);
    connect(widget, &TElementWidgetFont::fontChangedInst, this, &TPropertiesStates::onFontChanged);
    return widget;
}

QSpinBox *TPropertiesStates::makeValueSelector(const QString& name, int max)
{
    DECL_TRACER("TPropertiesStates::makeValueSelector(const QString& name, int max)");

    int value = 0;

    if (isAnyPage())
    {
        if (name == "GradientRadius")
            value = mPage.srPage.gr;
        else if (name == "GradientCenterX")
            value = mPage.srPage.gx;
        else if (name == "GradientCenterX")
            value = mPage.srPage.gy;
        else if (name == "OverallOpacity")
            value = mPage.srPage.oo;
        else if (name == "FontSize")
            value = mPage.srPage.fs;
        else if (name == "TextPositionX")
            value = mPage.srPage.tx;
        else if (name == "TextPositionY")
            value = mPage.srPage.ty;
    }
    else
    {
        if (name == "GradientRadius")
            value = mActSr.gr;
        else if (name == "GradientCenterX")
            value = mActSr.gx;
        else if (name == "GradientCenterX")
            value = mActSr.gy;
        else if (name == "OverallOpacity")
            value = mActSr.oo;
        else if (name == "FontSize")
            value = mActSr.fs;
        else if (name == "TextPositionX")
            value = mActSr.tx;
        else if (name == "TextPositionY")
            value = mActSr.ty;
    }

    QSpinBox *spin = new QSpinBox;
    spin->setObjectName(name);

    spin->setRange(0, max);
    spin->setValue(value);

    int inst = mActInstance;
    connect(spin, &QSpinBox::valueChanged, [this, name, inst](int value) {
        mActInstance = inst;
        setValue(name, value);
    });

    return spin;
}

QWidget *TPropertiesStates::makeTextValue(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeTextValue(const QString& name)");

    QString text;

    if (isAnyPage())
    {
        if (name == "StreamingSource")
            text = mPage.srPage.dv;
        else if (name == "Text")
            text = mPage.srPage.te;
    }
    else
    {
        if (name == "StreamingSource")
            text = mActSr.dv;
        else if (name == "Text")
            text = mActSr.te;
    }

    TElementWidgetText *wtext = new TElementWidgetText(text, name, mTreeWidget);
    wtext->setInstance(mActInstance);
    connect(wtext, &TElementWidgetText::textChangedInst, this, &TPropertiesStates::onTextValueChanged);
    return wtext;
}

TElementWidgetCombo *TPropertiesStates::makeTextJustification(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeTextJustification(const QString& name)");

    QList<QString> items = { "absolute", "top-left", "top-middle", "top-right",
                            "center-left", "center-middle", "center-right",
                            "bottom-left", "bottom-middle", "bottom-right" };
    QList<QVariant> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    ObjHandler::ORIENTATION ori;

    if (isAnyPage())
        ori = mPage.srPage.jt;
    else
        ori = mActSr.jt;

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mParent);
    combo->addItems(items);
    combo->addData(data);
    combo->setCurrentIndex(ori);
    combo->setInstance(mActInstance);
    connect(combo, &TElementWidgetCombo::selectionChangedInst, this, &TPropertiesStates::onOrientationChanged);
    return combo;
}

TElementTextEffect *TPropertiesStates::makeTextEffect(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeTextEffect(const QString& name)");

    int et = 0;

    if (isAnyPage())
        et = mPage.srPage.et;
    else
        et = mActSr.et;

    TElementTextEffect *eff = new TElementTextEffect(et, name, mParent);
    eff->setInstance(mActInstance);
    connect(eff, &TElementTextEffect::effectChangedInst, this, &TPropertiesStates::onTextEffectChanged);
    return eff;
}

TElementWidgetCombo *TPropertiesStates::makeWordWrap(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeWordWrap(const QString& name)");

    bool ww = false;

    if (isAnyPage())
        ww = mPage.srPage.ww;
    else
        ww = mActSr.ww;

    QList<QString> items = { tr("no"), tr("yes") };
    QList<QVariant> data = { false, true };

    TElementWidgetCombo *cbox = new TElementWidgetCombo(name, mParent);
    cbox->addItems(items);
    cbox->addData(data);
    cbox->setInstance(mActInstance);

    if (ww)
        cbox->setCurrentIndex(1);

    connect(cbox, &TElementWidgetCombo::selectionChangedInst, this, &TPropertiesStates::onWordWrapChanged);
    return cbox;
}

TElementGradientColors *TPropertiesStates::makeGradientColors(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeGradientColors(const QString& name)");

    QList<QColor> color;

    if (isAnyPage())
        color = mPage.srPage.gradientColors;
    else
        color = mActSr.gradientColors;

    TElementGradientColors *grad = new TElementGradientColors(color, name, mParent);
    grad->setInstance(mActInstance);
    connect(grad, &TElementGradientColors::gradientColorChangedInst, this, &TPropertiesStates::onGradientColorChanged);
    return grad;
}

TElementSound *TPropertiesStates::makeSoundSelector(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeSoundSelector(const QString& name)");

    QString sfile;

    if (isValidObjectIndex())
        sfile = mActSr.sd;

    TElementSound *sound = new TElementSound(sfile, name, mParent);
    sound->setInstance(mActInstance);
    connect(sound, &TElementSound::soundFileChangedInst, this, &TPropertiesStates::onSoundChanged);
    return sound;
}

void TPropertiesStates::setValue(const QString& name, const QVariant& value)
{
    DECL_TRACER("TPropertiesStates::setValue(const QString& name, const QVariant& value)");

    if (isAnyPage())
    {
        if (name == "FillType")
            mPage.srPage.ft = value.toString();
        else if (name == "FillColor")
            mPage.srPage.cf = value.toString();
        else if (name == "TextColor")
            mPage.srPage.ct = value.toString();
        else if (name == "TextEffectColor")
            mPage.srPage.ec = value.toString();
        else if (name == "VideoFill")
            mPage.srPage.vf = value.toString();
        else if (name == "FontSelector")
            mPage.srPage.ff = value.toString();
        else if (name == "FontSize")
            mPage.srPage.fs = value.toInt();
        else if (name == "Text")
            mPage.srPage.te = value.toString();
        else if (name == "TextOrientation")
            mPage.srPage.jt = static_cast<ObjHandler::ORIENTATION>(value.toInt());
        else if (name == "TextEffect")
            mPage.srPage.et = value.toInt();
        else if (name == "WordWrap")
            mPage.srPage.ww = (value.toBool() ? 1 : 0);
        else if (name == "GradientRadius")
            mPage.srPage.gr = value.toInt();
        else if (name == "GradientCenterX")
            mPage.srPage.gx = value.toInt();
        else if (name == "GradientCenterY")
            mPage.srPage.gy = value.toInt();
        else if (name == "TextPositionX")
            mPage.srPage.tx = value.toInt();
        else if (name == "TextPositionY")
            mPage.srPage.ty = value.toInt();
        else if (name == "BorderName")
            mPage.srPage.bs = value.toString();
        else if (name == "BorderColor")
            mPage.srPage.cb = value.toString();
        else if (name == "OverallOpacity")
            mPage.srPage.oo = value.toInt();
    }
    else
    {
        if (name == "BorderName")
            mPage.objects[mActObjectID]->setBorder(value.toString(), mActInstance);
        else if (name == "FillType")
            mPage.objects[mActObjectID]->setGradientFillType(value.toString(), mActInstance);
        else if (name == "FillColor")
            mPage.objects[mActObjectID]->setFillColor(value.toString(), mActInstance);
        else if (name == "TextColor")
            mPage.objects[mActObjectID]->setTextColor(value.toString(), mActInstance);
        else if (name == "TextEffectColor")
            mPage.objects[mActObjectID]->setTextEffectColor(value.toString(), mActInstance);
        else if (name == "GradientRadius")
            mPage.objects[mActObjectID]->setGradientRadius(value.toInt(), mActInstance);
        else if (name == "GradientCenterX")
            mPage.objects[mActObjectID]->setGradientCenterX(value.toInt(), mActInstance);
        else if (name == "GradientCenterY")
            mPage.objects[mActObjectID]->setGradientCenterY(value.toInt(), mActInstance);
        else if (name == "Text")
            mPage.objects[mActObjectID]->setText(value.toString(), mActInstance);
        else if (name == "TextOrientation")
            mPage.objects[mActObjectID]->setTextOrientation(static_cast<ObjHandler::ORIENTATION>(value.toInt()), mActInstance);
        else if (name == "TextPositionX")
            mPage.objects[mActObjectID]->setTextAbsoluteX(value.toInt(), mActInstance);
        else if (name == "TextPositionY")
            mPage.objects[mActObjectID]->setTextAbsoluteY(value.toInt(), mActInstance);
        else if (name == "FontSelector")
            mPage.objects[mActObjectID]->setFontFile(value.toString(), mActInstance);
        else if (name == "FontSize")
            mPage.objects[mActObjectID]->setFontSize(value.toInt(), mActInstance);
        else if (name == "WordWrap")
            mPage.objects[mActObjectID]->setWordWrap(value.toInt(), mActInstance);
        else if (name == "TextEffect")
            mPage.objects[mActObjectID]->setTextEffect(value.toInt(), mActInstance);
        else if (name == "OverallOpacity")
            mPage.objects[mActObjectID]->setOverallOpacity(value.toInt(), mActInstance);
        else if (name == "VideoFill")
            mPage.objects[mActObjectID]->setVideoFill(value.toString(), mActInstance);
    }

    mChanged = true;
    saveChangedData(&mPage, TBL_STATES);
    // Call to draw immediately
    requestRedraw(&mPage);
}

void TPropertiesStates::setColor(QLabel *label, QColor& color)
{
    DECL_TRACER("TPropertiesStates::setColor(QLabel *label, QColor& color)");

    QColorDialog colDialog(mParent);
    colDialog.setCurrentColor(color);

    if (colDialog.exec() == QDialog::Rejected)
        return;

    color = colDialog.selectedColor();
    label->setStyleSheet(QString("background-color: %1").arg(color.name(QColor::HexArgb)));
}

// Callbacks

void TPropertiesStates::onBitmapsChanged(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name, int instance)
{
    DECL_TRACER("TPropertiesStates::onBitmapsChanged(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name, int instance)");

    MSG_DEBUG("Bitmap for name: " << name.toStdString());

    if (isAnyPage())
        mPage.srPage.bitmaps = bitmaps;
    else
        mPage.objects[mActObjectID]->setBitmaps(bitmaps, instance);

    saveChangedData(&mPage, TBL_STATES);
    mChanged = false;
    requestRedraw(&mPage);
}

void TPropertiesStates::onOrientationChanged(const QString& text, const QVariant& data, const QString& name, int instance)
{
    DECL_TRACER("TPropertiesStates::onOrientationChanged(const QString& text, const QVariant& data, const QString& name, int instance)");

    Q_UNUSED(text);

    mActInstance = instance;
    setValue(name, data);
}

void TPropertiesStates::onFontChanged(const QFont& font, const QString& name, int instance)
{
    DECL_TRACER("TPropertiesStates::onFontChanged(const QFont& font, const QString& name, int instance)");

    mActInstance = instance;
    setValue(name, font.family());
}

void TPropertiesStates::onTextEffectChanged(int eff, const QString& effect, const QString& name, int instance)
{
    DECL_TRACER("TPropertiesStates::onTextEffectChanged(int eff, const QString& effect, const QString& name, int instance)");

    Q_UNUSED(effect);

    mActInstance = instance;
    setValue(name, eff);
}

void TPropertiesStates::onBorderNameChanged(const QString& border, const QString& name, int instance)
{
    DECL_TRACER("TPropertiesStates::onBorderNameChanged(const QString& border, const QString& name, int instance)");

    mActInstance = instance;
    setValue(name, border);
}

void TPropertiesStates::onWordWrapChanged(const QString& text, const QVariant& data, const QString& name, int instance)
{
    DECL_TRACER("TPropertiesStates::onWordWrapChanged(const QString& text, const QVariant& data, const QString& name, int instance)");

    Q_UNUSED(text)
    mActInstance = instance;
    setValue(name, data);
}

void TPropertiesStates::onGradientColorChanged(const QList<QColor>& colors, const QString& name, int instance)
{
    DECL_TRACER("TPropertiesStates::onGradientColorChanged(const QList<QColor>& colors, const QString& name, int instance)");

    MSG_DEBUG("Name: " << name.toStdString());

    if (isAnyPage())
        mPage.srPage.gradientColors = colors;
    else
        mPage.objects[mActObjectID]->setGradientColors(colors, instance);

    saveChangedData(&mPage, TBL_STATES);
    mChanged = false;
    requestRedraw(&mPage);
}

void TPropertiesStates::onSoundChanged(const QString& file, const QString& name, int instance)
{
    DECL_TRACER("TPropertiesStates::onSoundChanged(const QString& file, const QString& name, int instance)");

    Q_UNUSED(name);

    if (isAnyPage())
        return;         // Pages can't play sounds

    mPage.objects[mActObjectID]->setSound(file, instance);
    mChanged = true;
    markChanged();
}

void TPropertiesStates::onVideoFillChanged(const QString& text, const QVariant& data, const QString& name, int instance)
{
    DECL_TRACER("TPropertiesStates::onVideoFillChanged(const QString& text, const QVariant& data, const QString& name, int instance)");

    Q_UNUSED(text);

    mActInstance = instance;
    setValue(name, data);
}

void TPropertiesStates::onColorChanged(const QColor& color, const QString& name, int instance)
{
    DECL_TRACER("TPropertiesStates::onColorChanged(const QColor& color, const QString& name, int instance)");

    mActInstance = instance;
    setValue(name, color);
}

void TPropertiesStates::onTextValueChanged(const QString& text, const QString& name, int instance)
{
    DECL_TRACER("TPropertiesStates::onTextValueChanged(const QString& text, const QString& name, int instance)");

    mActInstance = instance;
    setValue(name, text);
}

void TPropertiesStates::onFillTypeChanged(const QString& text, const QVariant& data, const QString& name, int instance)
{
    DECL_TRACER("TPropertiesStates::onFillTypeChanged(const QString& text, const QVariant& data, const QString& name, int instance)");

    Q_UNUSED(text);

    mActInstance = instance;
    setValue(name, data);
}
