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

void TPropertiesStates::setGeometry(int bi, const QRect& geom)
{
    DECL_TRACER("TPropertiesStates::setGeometry(int bi, const QRect& geom)");

    for (TObjectHandler *obj : mPage.objects)
    {
        if (obj->getButtonIndex() == bi)
        {
            obj->setSize(geom);
            break;
        }
    }
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
    createPage();
}

void TPropertiesStates::setActualButton(int index, STATE_TYPE stype)
{
    DECL_TRACER("TPropertiesStates::setActualButton(int index, STATE_TYPE stype)");

    if (index <= 0 || index >= mPage.objects.size() || mActObjectID == index)
        return;

    mActObjectID = index;
    mSType = stype;
    createPage();
    // TODO: Add code to show the states for an object
}

void TPropertiesStates::setObjectType(ObjHandler::BUTTONTYPE btype, int index)
{
    DECL_TRACER("TPropertiesStates::setObjectType(ObjHandler::BUTTONTYPE btype, int index)");

    if (index < 0 || index >= mPage.objects.size())
        return;

    if (mActObjectID != index)
    {
        MSG_WARNING("The new object id is different! Actual ID: " << mActObjectID << ", new ID: " << index);
    }

    mPage.objects[index]->setObjectType(btype);
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

    for (int i = 0; i < table->rowCount(); ++i)
        table->setRowHidden(i, true);

    if (mActObjectID < 0 || mActObjectID > mPage.objects.size() || instance < 0)
    {
        if (mPage.popupType == Page::PT_PAGE)
        {
            table->setRowHidden(TTEXT_FILL_TYPE, false);

            if (mPage.srPage.ft != "solid")
            {
                table->setRowHidden(TTEXT_FILL_GRADIENT_COLORS, false);

                if (mPage.srPage.ft == "radial")
                {
                    table->setRowHidden(TTEXT_GRADIENT_RADIUS, false);
                    table->setRowHidden(TTEXT_GRADIENT_CENTER_X, false);
                    table->setRowHidden(TTEXT_GRADIENT_CENTER_Y, false);
                }
            }
            else
                table->setRowHidden(TTEXT_FILL_COLOR, false);

            table->setRowHidden(TTEXT_TEXT_COLOR, false);
            table->setRowHidden(TTEXT_TEXT_EFFECT_COLOR, false);
            table->setRowHidden(TTEXT_VIDEO_FILL, false);

            if (mPage.srPage.vf == "100")
                table->setRowHidden(TTEXT_STREAMING_SOURCE, false);

            table->setRowHidden(TTEXT_BITMAPS, false);
            table->setRowHidden(TTEXT_FONT, false);
            table->setRowHidden(TTEXT_FONT_SIZE, false);
            table->setRowHidden(TTEXT_TEXT, false);
            table->setRowHidden(TTEXT_TEXT_JUSTIFICATION, false);

            if (mPage.srPage.jt == ObjHandler::ORI_ABSOLUT)
            {
                table->setRowHidden(TTEXT_TEXT_POSITION_X, false);
                table->setRowHidden(TTEXT_TEXT_POSITION_Y, false);
            }

            table->setRowHidden(TTEXT_TEXT_EFFECT, false);
            table->setRowHidden(TTEXT_WORD_WRAP, false);
        }
        else if (mPage.popupType == Page::PT_POPUP || mPage.popupType == Page::PT_SUBPAGE)
        {
            table->setRowHidden(TTEXT_BORDER_NAME, false);

            if (mPage.srPage.bs.isEmpty())
                table->setRowHidden(TTEXT_CHAMELEON_IMAGE, false);

            table->setRowHidden(TTEXT_BORDER_COLOR, false);
            table->setRowHidden(TTEXT_FILL_TYPE, false);

            if (mPage.srPage.ft != "solid")
            {
                table->setRowHidden(TTEXT_FILL_GRADIENT_COLORS, false);

                if (mPage.srPage.ft == "radial")
                {
                    table->setRowHidden(TTEXT_GRADIENT_RADIUS, false);
                    table->setRowHidden(TTEXT_GRADIENT_CENTER_X, false);
                    table->setRowHidden(TTEXT_GRADIENT_CENTER_Y, false);
                }
            }
            else
                table->setRowHidden(TTEXT_FILL_COLOR, false);

            table->setRowHidden(TTEXT_TEXT_COLOR, false);
            table->setRowHidden(TTEXT_TEXT_EFFECT_COLOR, false);
            table->setRowHidden(TTEXT_VIDEO_FILL, false);

            if (mPage.srPage.vf == "100")
                table->setRowHidden(TTEXT_STREAMING_SOURCE, false);

            table->setRowHidden(TTEXT_BITMAPS, false);
            table->setRowHidden(TTEXT_FONT, false);
            table->setRowHidden(TTEXT_FONT_SIZE, false);
            table->setRowHidden(TTEXT_TEXT, false);
            table->setRowHidden(TTEXT_TEXT_JUSTIFICATION, false);

            if (mPage.srPage.jt == ObjHandler::ORI_ABSOLUT)
            {
                table->setRowHidden(TTEXT_TEXT_POSITION_X, false);
                table->setRowHidden(TTEXT_TEXT_POSITION_Y, false);
            }

            table->setRowHidden(TTEXT_TEXT_EFFECT, false);
            table->setRowHidden(TTEXT_WORD_WRAP, false);
        }
    }
    else
    {
        ObjHandler::TOBJECT_t obj = mPage.objects[mActObjectID]->getObject();
        mActInstance = instance;

        if (instance >= obj.sr.size())
        {
            MSG_ERROR("Instance " << instance << " is out of range! Have " << obj.sr.size() << " states.");
            return;
        }

        table->setRowHidden(TTEXT_BORDER_NAME, false);

        if (mPage.srPage.bs.isEmpty())
            table->setRowHidden(TTEXT_CHAMELEON_IMAGE, false);

        table->setRowHidden(TTEXT_BORDER_COLOR, false);
        table->setRowHidden(TTEXT_FILL_TYPE, false);

        if (mPage.srPage.ft != "solid")
        {
            table->setRowHidden(TTEXT_FILL_GRADIENT_COLORS, false);

            if (mPage.srPage.ft == "radial")
            {
                table->setRowHidden(TTEXT_GRADIENT_RADIUS, false);
                table->setRowHidden(TTEXT_GRADIENT_CENTER_X, false);
                table->setRowHidden(TTEXT_GRADIENT_CENTER_Y, false);
            }
        }
        else
            table->setRowHidden(TTEXT_FILL_COLOR, false);

        table->setRowHidden(TTEXT_TEXT_COLOR, false);
        table->setRowHidden(TTEXT_TEXT_EFFECT_COLOR, false);
        table->setRowHidden(TTEXT_OVERALL_OPACITY, false);
        table->setRowHidden(TTEXT_VIDEO_FILL, false);

        if (mPage.srPage.vf == "100")
            table->setRowHidden(TTEXT_STREAMING_SOURCE, false);

        table->setRowHidden(TTEXT_BITMAPS, false);
        table->setRowHidden(TTEXT_FONT, false);
        table->setRowHidden(TTEXT_FONT_SIZE, false);
        table->setRowHidden(TTEXT_TEXT, false);
        table->setRowHidden(TTEXT_TEXT_JUSTIFICATION, false);

        if (mPage.srPage.jt == ObjHandler::ORI_ABSOLUT)
        {
            table->setRowHidden(TTEXT_TEXT_POSITION_X, false);
            table->setRowHidden(TTEXT_TEXT_POSITION_Y, false);
        }

        table->setRowHidden(TTEXT_TEXT_EFFECT, false);
        table->setRowHidden(TTEXT_WORD_WRAP, false);
        table->setRowHidden(TTEXT_SOUND, false);
    }
}

void TPropertiesStates::createPage()
{
    DECL_TRACER("TPropertiesStates::createPage()");

    if (!mTreeWidget)
    {
        MSG_WARNING("Tree widget is not initialized!")
        return;
    }

    mInitialized = false;
    mStates.clear();
    mTreeWidget->clear();
    mTreeWidget->setColumnCount(1);
    mTreeWidget->setHeaderHidden(true);

    QTreeWidgetItem *top = new QTreeWidgetItem(mTreeWidget);

    if (isAnyPage())
        top->setText(0, tr("Off"));
    else
        top->setText(0, tr("All"));

    QTreeWidgetItem *item = new QTreeWidgetItem(top);
    item->setFirstColumnSpanned(true);
    item->setFlags(Qt::ItemIsEnabled);      // Not selectable

    QTableWidget *table = createTableWidget(mTreeWidget);
    mStates.append(table);
    mTreeWidget->setItemWidget(item, 0, table);
    int totalHeight = table->height() + mTreeWidget->height();

    if (!isAnyPage() && isValidObjectIndex())
    {
        ObjHandler::TOBJECT_t obj = mPage.objects[mActObjectID]->getObject();

        for (int i = 0; i < obj.sr.size(); ++i)
        {
            QTreeWidgetItem *entry = new QTreeWidgetItem(mTreeWidget);

            entry->setText(0, tr("State %1").arg(obj.sr[i].number));

            item = new QTreeWidgetItem(entry);
            item->setFirstColumnSpanned(true);
            item->setFlags(Qt::ItemIsEnabled);      // Not selectable

            table = createTableWidget(mTreeWidget);
            mStates.append(table);
            mTreeWidget->setItemWidget(item, 0, table);
            totalHeight += table->height();
        }
    }

    MSG_DEBUG("Page total height: " << totalHeight);
    mTreeWidget->setMinimumHeight(totalHeight);

    for (int i = 0; i < mStates.size(); ++i)
    {
        mActState = i;
        setTable(mStates[i]);
    }

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

    createPage(table);

    table->resizeRowsToContents();
    table->resizeColumnsToContents();
    return table;
}

void TPropertiesStates::createPage(QTableWidget *table, int instance)
{
    DECL_TRACER("TPropertiesStates::createTablePage(QTableWidget *table, int instance)");

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
    else if (instance >= 0 && mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        ObjHandler::TOBJECT_t obj = mPage.objects[mActObjectID]->getObject();

        if (obj.sr[instance].ff.isEmpty())
        {
            QFont font = mParent->font();
            obj.sr[instance].ff = font.family();
        }

        if (obj.sr[instance].fs <= 0)
            obj.sr[instance].fs = 10;
    }
    else
        return;

    int rows = TTEXT_MAX;
    // The row count must be defined before rows are added to the table! Otherwise
    // an empty table with only the grid will be visible.
    table->setRowCount(rows);
    int totalHeight = 0;

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

        totalHeight += table->cellWidget(row, 1)->size().height();
        table->setItem(row, 0, col0);
    }

    table->setMinimumHeight(totalHeight);
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

    if (mActObjectID < 0 || mActObjectID >= mPage.objects.size())
    {
        if (mPage.popupType == Page::PT_PAGE)
            mSType = STATE_PAGE;
        else if (mPage.popupType == Page::PT_POPUP)
            mSType = STATE_POPUP;
        else if (mPage.popupType == Page::PT_SUBPAGE)
            mSType = STATE_SUBPAGE;
        else
            mSType = STATE_UNKNOWN;

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
        case ObjHandler::SUBPAGE_VIEW:
            mSType = STATE_SUBPAGE;
        break;

        default:
            mSType = STATE_UNKNOWN;
    }
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

void TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QVariant& data, ELEMENT_TYPE_t etype)
{
    DECL_TRACER("TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QVariant& data, ELEMENT_TYPE_t etype)");

    if (!table)
    {
        table = getTableWidget(mActState);

        if (!table)
            return;
    }

    QWidget *w = table->cellWidget(row, col);

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

        default:
            return;
    }
}

void TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QList<ObjHandler::BITMAPS_t>& bm, ELEMENT_TYPE_t etype)
{
    DECL_TRACER("TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QList<ObjHandler::BITMAPS_t>& bm, ELEMENT_TYPE_t etype)");

    if (!table)
    {
        table = getTableWidget(mActState);

        if (!table)
            return;
    }

    QWidget *w = table->cellWidget(row, col);

    if (!w)
        return;

    if (etype == W_BITMAPSELECTOR)
    {
        TElementBitmapSelector *p = static_cast<TElementBitmapSelector *>(w);
        p->setBitmaps(bm);
    }
}

void TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QFont& font, ELEMENT_TYPE_t etype)
{
    DECL_TRACER("TPropertiesStates::setTableWidget(QTableWidget *table, int row, int col, const QFont& font, ELEMENT_TYPE_t etype)");

    if (!table)
    {
        table = getTableWidget(mActState);

        if (!table)
            return;
    }

    QWidget *w = table->cellWidget(row, col);

    if (!w)
        return;

    if (etype == W_FONT)
    {
        TElementWidgetFont *p = static_cast<TElementWidgetFont *>(w);
        p->setFont(font);
    }
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
        border = mPage.objects[0]->getObject().sr[0].bs;

    TElementBorderName *brd = new TElementBorderName(border, name, mParent);
    connect(brd, &TElementBorderName::borderChanged, this, &TPropertiesStates::onBorderNameChanged);
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
        ftype = mPage.objects[0]->getObject().sr[0].ft;

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTreeWidget);
    combo->addItems(list);
    combo->addData(data);
    combo->setDefaultData(ftype);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesStates::onFillTypeChanged);
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
            col = mPage.objects[mActObjectID]->getBorderColor(mActInstance);
        else if (name == "TextColor")
            col = mPage.objects[mActObjectID]->getTextColor(mActInstance);
        else if (name == "TextEffectColor")
            col = mPage.objects[mActObjectID]->getTextEffectColor(mActInstance);
    }

    TElementColorSelector *colsel = new TElementColorSelector(name, mTreeWidget);
    colsel->setColor(col);

    connect(colsel, &TElementColorSelector::colorChanged, this, &TPropertiesStates::onColorChanged);
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
        vf = mPage.objects[mActObjectID]->getVideoFill(mActInstance);

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mTreeWidget);
    combo->addItems(items);
    combo->addData(data);

    if (vf == "100")
        combo->setCurrentIndex(1);
    else if (vf == "101")
        combo->setCurrentIndex(2);

    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesStates::onVideoFillChanged);
    return combo;
}

TElementBitmapSelector *TPropertiesStates::makeBitmapSelector(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeBitmapSelector(const QString& name)");

    QList<ObjHandler::BITMAPS_t> bitmaps;

    if (isAnyPage())
        bitmaps = mPage.srPage.bitmaps;
    else if (isValidObjectIndex())
        bitmaps = mPage.objects[mActObjectID]->getBitmaps(mActInstance);

    TElementBitmapSelector *bs = new TElementBitmapSelector(name, bitmaps, mParent);
    connect(bs, &TElementBitmapSelector::bitmapsChanged, this, &TPropertiesStates::onBitmapsChanged);
    return bs;
}

TElementWidgetFont *TPropertiesStates::makeFontSelector(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeFontSelector(const QString& fname, const QString& name)");

    QString ff;

    if (isAnyPage())
        ff = mPage.srPage.ff;
    else if (isValidObjectIndex())
        ff = mPage.objects[mActObjectID]->getFontFile(mActInstance);

    TElementWidgetFont *widget = new TElementWidgetFont(QFont(ff), name, mParent);
    connect(widget, &TElementWidgetFont::fontChanged, this, &TPropertiesStates::onFontChanged);
    return widget;
}

QSpinBox *TPropertiesStates::makeValueSelector(const QString& name, int max)
{
    DECL_TRACER("TPropertiesStates::makeValueSelector(const QString& name, int max)");

    int value = 0;

    QSpinBox *spin = new QSpinBox;
    spin->setObjectName(name);

    spin->setRange(0, max);
    spin->setValue(value);

    connect(spin, &QSpinBox::valueChanged, [this, name](int value) { setValue(name, value); });
    return spin;
}

QWidget *TPropertiesStates::makeTextValue(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeTextValue(const QString& name)");

    QString text;

    if (isAnyPage())
        text = mPage.srPage.te;
    else
        text = mPage.objects[mActObjectID]->getText(mActInstance);

    TElementWidgetText *wtext = new TElementWidgetText(text, name, mTreeWidget);
    connect(wtext, &TElementWidgetText::textChanged, this, &TPropertiesStates::onTextValueChanged);
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
        ori = mPage.objects[mActObjectID]->getTextOrientation(mActInstance);

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mParent);
    combo->addItems(items);
    combo->addData(data);
    combo->setCurrentIndex(ori);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesStates::onOrientationChanged);
    return combo;
}

TElementTextEffect *TPropertiesStates::makeTextEffect(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeTextEffect(const QString& name)");

    int et = 0;

    if (isAnyPage())
        et = mPage.srPage.et;
    else
        et = mPage.objects[mActObjectID]->getTextEffect(mActInstance);

    TElementTextEffect *eff = new TElementTextEffect(et, name, mParent);
    connect(eff, &TElementTextEffect::effectChanged, this, &TPropertiesStates::onTextEffectChanged);
    return eff;
}

TElementWidgetCombo *TPropertiesStates::makeWordWrap(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeWordWrap(const QString& name)");

    bool ww = false;

    if (isAnyPage())
        ww = mPage.srPage.ww;
    else
        ww = mPage.objects[mActObjectID]->getWordWrap(mActInstance);

    QList<QString> items = { tr("no"), tr("yes") };
    QList<QVariant> data = { false, true };

    TElementWidgetCombo *cbox = new TElementWidgetCombo(name, mParent);
    cbox->addItems(items);
    cbox->addData(data);

    if (ww)
        cbox->setCurrentIndex(1);

    connect(cbox, &TElementWidgetCombo::selectionChanged, this, &TPropertiesStates::onWordWrapChanged);
    return cbox;
}

TElementGradientColors *TPropertiesStates::makeGradientColors(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeGradientColors(const QString& name)");

    QList<QColor> color;

    if (isAnyPage())
        color = mPage.srPage.gradientColors;
    else
        color = mPage.objects[mActObjectID]->getGradientColors(mActInstance);

    TElementGradientColors *grad = new TElementGradientColors(color, name, mParent);
    connect(grad, &TElementGradientColors::gradientColorChanged, this, &TPropertiesStates::onGradientColorChanged);
    return grad;
}

TElementSound *TPropertiesStates::makeSoundSelector(const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeSoundSelector(const QString& name)");

    QString sfile;
    int instance = 0;

    if (mActObjectID >= 0 && mActObjectID < mPage.objects.size())
    {
        if (mActInstance >= 0  && mActInstance < mPage.objects[mActObjectID]->getObject().sr.size())
            instance = mActInstance;

        sfile = mPage.objects[mActObjectID]->getSound(instance);
    }

    TElementSound *sound = new TElementSound(sfile, name, mParent);
    connect(sound, &TElementSound::soundFileChanged, this, &TPropertiesStates::onSoundChanged);
    return sound;
}

QFont TPropertiesStates::chooseFont(const QFont& font)
{
    DECL_TRACER("TPropertiesStates::chooseFont(const QFont& font)");

    bool ok = false;
    QFont f = QFontDialog::getFont(&ok, font, mParent, tr("Select font"));

    if (ok)
        return f;

    return font;
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

void TPropertiesStates::addGradientLines(const QString& gradient, const QString& name, bool init)
{
    DECL_TRACER("TPropertiesStates::addGradientLines(const QString& gradient, const QString& name, bool init)");

    Q_UNUSED(init);

    QTreeWidgetItem *root = mTreeWidget->invisibleRootItem();
    QTreeWidgetItem *top = root->child(0);
    QTableWidget *widget = static_cast<QTableWidget *>(mTreeWidget->itemWidget(top->child(0), 0));

    if (!widget)
    {
        MSG_WARNING("Couldn't get the table widget!");
        return;
    }
/*
    if (name == "PgFillType")
        adjustTablePage(widget, gradient);
    else if (name == "PopupFillType")
        adjustTablePopup(widget, gradient);
*/
}

// Callbacks

void TPropertiesStates::onBitmapsChanged(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onBitmapsChanged(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name)");

    MSG_DEBUG("Bitmap for name: " << name.toStdString());

    if (name == "PgBitmapSelector" || name == "PopupBitmapSelector")
    {
        mPage.srPage.bitmaps = bitmaps;
        saveChangedData(&mPage, TBL_STATES);
        mChanged = false;
        requestRedraw(&mPage);
    }
}

void TPropertiesStates::onOrientationChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onOrientationChanged(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text);

    QTreeWidgetItem *root = mTreeWidget->invisibleRootItem();
    QTreeWidgetItem *top = root->child(0);
    QTableWidget *widget = static_cast<QTableWidget *>(mTreeWidget->itemWidget(top->child(0), 0));

    if (!widget)
    {
        MSG_WARNING("Couldn't get the table widget!");
        return;
    }
/*
    if (name == "PgTextOrientation")
        adjustTablePage(widget);
    else if (name == "PopupTextOrientation")
        adjustTablePopup(widget);
*/
    setValue(name, data);
}

void TPropertiesStates::onFontChanged(const QFont& font, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onFontChanged(const QFont& font, const QString& name)");

    setValue(name, font.family());
}

void TPropertiesStates::onTextEffectChanged(int eff, const QString& effect, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onTextEffectChanged(int eff, const QString& effect, const QString& name)");

    MSG_DEBUG("Number: " << eff << ", Name: " << effect.toStdString() << ", Element name: " << name.toStdString());
    setValue(name, eff);
}

void TPropertiesStates::onBorderNameChanged(const QString& border, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onBorderNameChanged(const QString& border, const QString& name)");

    MSG_DEBUG("Selected border: " << border.toStdString() << ", Elemnt name: " << name.toStdString());
    setValue(name, border);
}

void TPropertiesStates::onWordWrapChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onWordWrapChanged(const QString& text, const QVariant& data, const QString& name)");

    Q_UNUSED(text)
    setValue(name, data);
}

void TPropertiesStates::onGradientColorChanged(const QList<QColor>& colors, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onGradientColorChanged(const QList<QColor>& colors, const QString& name)");

    MSG_DEBUG("Name: " << name.toStdString());

    if (isAnyPage())
        mPage.srPage.gradientColors = colors;
    else
        mPage.objects[mActObjectID]->setGradientColors(colors, mActInstance);

    saveChangedData(&mPage, TBL_STATES);
    mChanged = false;
    requestRedraw(&mPage);
}

void TPropertiesStates::onSoundChanged(const QString& file, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onSoundChanged(const QString& file, const QString& name)");

    if (isAnyPage())
        return;         // Pages can't play sounds

    mPage.objects[mActObjectID]->setSound(file, mActInstance);
    mChanged = true;
    markChanged();
}

void TPropertiesStates::onVideoFillChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onVideoFillChanged(const QString& text, const QVariant& data, const QString& name)");

    setValue(name, data);
}

void TPropertiesStates::onColorChanged(const QColor& color, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onColorChanged(const QColor& color, const QString& name)");

    setValue(name, color);
}

void TPropertiesStates::onTextValueChanged(const QString& text, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onTextValueChanged(const QString& text, const QString& name)");

    setValue(name, text);
}

void TPropertiesStates::onFillTypeChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onFillTypeChanged(const QString& text, const QVariant& data, const QString& name)");

    setValue(name, data);
}
