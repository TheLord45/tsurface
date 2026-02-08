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
#include "ttextboxdialog.h"
#include "telementbitmapselector.h"
#include "telementwidgetcombo.h"
#include "telementwidgetfont.h"
#include "telementtexteffect.h"
#include "telementbordername.h"
#include "telementgradientcolors.h"
#include "terror.h"

using namespace Page;

#define TTEXT_FILL_TYPE             1
#define TTEXT_FILL_COLOR            2
#define TTEXT_TEXT_COLOR            3
#define TTEXT_TEXT_EFFECT_COLOR     4
#define TTEXT_VIDEO_FILL            5
#define TTEXT_BITMAPS               6
#define TTEXT_FONT                  7
#define TTEXT_FONT_SIZE             8
#define TTEXT_TEXT                  9
#define TTEXT_TEXT_JUSTIFICATION    10
#define TTEXT_TEXT_EFFECT           11
#define TTEXT_WORD_WRAP             12

#define TTEXT_BORDER_NAME           13
#define TTEXT_BORDER_COLOR          14
#define TTEXT_OVERALL_OPACITY       15

#define TTEXT_CHAMELEON_IMAGE       16
#define TTEXT_SOUND                 17

#define TTEXT_FILL_GRADIENT_COLORS  18
#define TTEXT_GRADIENT_RADIUS       19
#define TTEXT_GRADIENT_CENTER_X     20
#define TTEXT_GRADIENT_CENTER_Y     21

#define TTEXT_TEXT_POSITION_X       22
#define TTEXT_TEXT_POSITION_Y       23

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

void TPropertiesStates::setStatesPage(const QString& name)
{
    DECL_TRACER("TPropertiesStates::setStatesPage(const QString& name)");

    if (!mTreeWidget)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTreeWidget\"!");
        return;
    }

    Page::PAGE_t page = TPageHandler::Current().getPage(name);

    if (page.pageID == mPage.pageID)
        return;

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_STATES);

    mChanged = false;
    mPage = page;
    setStatesPage(mPage.pageID, true);
}

void TPropertiesStates::setStatesPage(int id, bool loaded)
{
    DECL_TRACER("TPropertiesStates::setStatesPage(int id, bool loaded)");

    if (!mTreeWidget)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTreeWidget\"!")
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
        saveChangedData(&mPage, TBL_STATES);
        mPage = Page::PAGE_t();
    }

    mChanged = false;

    if (!loaded)
        mPage = page;

    if (mPage.pageID <= 0)
        return;

    createPage();
}

void TPropertiesStates::setActualButton(int bi)
{
    DECL_TRACER("TPropertiesStates::setActualButton(int bi)");

    if (bi <= 0 || bi >= mPage.objects.size() || mActualObjectID == bi)
        return;

    mActualObjectID = bi;
    // TODO: Add code to show the states for an object
}

void TPropertiesStates::update()
{

}

void TPropertiesStates::clear()
{
    DECL_TRACER("TPropertiesStates::clear()");

    if (mBlocked)
        return;

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_STATES);

    mChanged = true;
    mPage = Page::PAGE_t();
    mTreeWidget->clear();
    mTreeWidget->setColumnCount(0);
}

void TPropertiesStates::createPage()
{
    DECL_TRACER("TPropertiesStates::createPage()");

    if (mBlocked)
        return;

    if (!mTreeWidget)
    {
        MSG_WARNING("Tree widget is not initialized!")
        return;
    }

    mInitialized = false;
    QBrush brush;
    brush.setColor(Qt::GlobalColor::lightGray);
    mTreeWidget->clear();
    mTreeWidget->setColumnCount(1);
    mTreeWidget->setHeaderHidden(true);

    if (mPage.popupType == PT_PAGE)
    {
        QTreeWidgetItem *top = new QTreeWidgetItem(mTreeWidget);
        top->setText(0, tr("Off"));

        QTreeWidgetItem *item = new QTreeWidgetItem(top);
        item->setFirstColumnSpanned(true);
        item->setFlags(Qt::ItemIsEnabled);      // Not selectable

        QTableWidget *table = createTableWidget(STATE_PAGE);
        mTreeWidget->setItemWidget(item, 0, table);
        addGradientLines(mPage.srPage.ft, "PgFillType", true);
    }
    else if (mPage.popupType == PT_POPUP)
    {
        QTreeWidgetItem *top = new QTreeWidgetItem(mTreeWidget);
        top->setText(0, tr("Off"));

        QTreeWidgetItem *item = new QTreeWidgetItem(top);
        item->setFirstColumnSpanned(true);
        item->setFlags(Qt::ItemIsEnabled);      // Not selectable

        QTableWidget *table = createTableWidget(STATE_POPUP, mTreeWidget);
        mTreeWidget->setItemWidget(item, 0, table);
        addGradientLines(mPage.srPage.ft, "PopupFillType", true);
    }

    mInitialized = true;
}

QTableWidget *TPropertiesStates::createTableWidget(STATE_TYPE stype, QWidget *parent)
{
    DECL_TRACER("TPropertiesStates::createTableWidget(STATE_TYPE stype, QWidget *parent)");

    if (mBlocked)
        return nullptr;

    QTableWidget *table = new QTableWidget(parent);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setVisible(false);
    table->setShowGrid(true);
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table->setColumnCount(2);
//    int rows = 0;

    if (stype == STATE_PAGE)
        createTablePage(table);
    else if (stype == STATE_POPUP)
        createTablePopup(table);
//    else if (stype == STATE_BUTTON)
//        rows = 17;
//    else if (stype == STATE_BARGRAPH)
//        rows = 16;

    table->resizeRowsToContents();
    table->resizeColumnsToContents();
    return table;
}

void TPropertiesStates::createTablePage(QTableWidget *table)
{
    DECL_TRACER("TPropertiesStates::createTablePage(QTableWidget *table)");

    if (mPage.srPage.ff.isEmpty())
    {
        QFont font = mParent->font();
        mPage.srPage.ff = font.family();
    }

    if (mPage.srPage.fs <= 0)
        mPage.srPage.fs = 10;

    int rows = 18;
    // The row count must be defined before rows are added to the table! Otherwise
    // an empty table with only the grid will be visible.
    table->setRowCount(rows);

    for (int row = 0; row < rows; ++row)
    {
        QTableWidgetItem *col0 = new QTableWidgetItem;
        col0->setBackground(Qt::lightGray);
        col0->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);

        switch(row)
        {
            case 0:
                col0->setText(getLeftColText(TTEXT_FILL_TYPE));
                table->setCellWidget(row, 1, makeFillType(mPage.srPage.ft, "PgFillType"));
            break;

            case 1:
                col0->setText(getLeftColText(TTEXT_FILL_COLOR));
                table->setCellWidget(row, 1, makeColorSelector(mPage.srPage.cf, "PgFillColor"));

                if (mPage.srPage.ft != "solid")
                    table->hideRow(row);
            break;

            case 2:
                col0->setText(getLeftColText(TTEXT_FILL_GRADIENT_COLORS));
                table->setCellWidget(row, 1, makeGradientColors(mPage.srPage.gradientColors, "PgFillGradientColors"));

                if (mPage.srPage.ft == "solid")
                    table->hideRow(row);
            break;

            case 3:
                col0->setText(getLeftColText(TTEXT_GRADIENT_RADIUS));
                table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.gr, "PgGradientRadius", 0, 5000));

                if (mPage.srPage.ft != "radial")
                    table->hideRow(row);
            break;

            case 4:
                col0->setText(getLeftColText(TTEXT_GRADIENT_CENTER_X));
                table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.gx, "PgGradientCenterX"));

                if (mPage.srPage.ft != "radial")
                    table->hideRow(row);
            break;

            case 5:
                col0->setText(getLeftColText(TTEXT_GRADIENT_CENTER_Y));
                table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.gy, "PgGradientCenterX"));

                if (mPage.srPage.ft != "radial")
                    table->hideRow(row);
            break;

            case 6:
                col0->setText(getLeftColText(TTEXT_TEXT_COLOR));
                table->setCellWidget(row, 1, makeColorSelector(mPage.srPage.ct, "PgTextColor"));
            break;

            case 7:
                col0->setText(getLeftColText(TTEXT_TEXT_EFFECT_COLOR));
                table->setCellWidget(row, 1, makeColorSelector(mPage.srPage.ec, "PgTextEffectColor"));
            break;

            case 8:
                col0->setText(getLeftColText(TTEXT_VIDEO_FILL));
                table->setCellWidget(row, 1, makeVideoFill(mPage.srPage.vf, "PgVideoFill"));
            break;

            case 9:
                col0->setText(getLeftColText(TTEXT_BITMAPS));
                table->setCellWidget(row, 1, makeBitmapSelector(mPage.srPage.bitmaps, "PgBitmapSelector"));
            break;

            case 10:
                col0->setText(getLeftColText(TTEXT_FONT));
                table->setCellWidget(row, 1, makeFontSelector(mPage.srPage.ff, "PgFontSelector"));
            break;

            case 11:
                col0->setText(getLeftColText(TTEXT_FONT_SIZE));
                table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.fs, "PgFontSize"));
            break;

            case 12:
            {
                col0->setText(getLeftColText(TTEXT_TEXT));
                QFont font;

                if (mPage.srPage.ff.isEmpty())
                {
                    font = mParent->font();
                    mPage.srPage.ff = font.family();
                }

                if (mPage.srPage.fs <= 0)
                    font.setPointSize(10);
                else
                    font.setPointSize(mPage.srPage.fs);

                table->setCellWidget(row, 1, makeTextValue(mPage.srPage.te, font, "PgText"));
                table->resizeRowToContents(row);
            }
            break;

            case 13:
                col0->setText(getLeftColText(TTEXT_TEXT_JUSTIFICATION));
                table->setCellWidget(row, 1, makeTextJustification(mPage.srPage.jt, "PgTextOrientation"));
            break;

            case 14:
                col0->setText(getLeftColText(TTEXT_TEXT_POSITION_X));
                table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.tx, "PgTextPositionX", 0, mPage.width));

                if (mPage.srPage.jt != ObjHandler::ORI_ABSOLUT)
                    table->hideRow(row);
            break;

            case 15:
                col0->setText(getLeftColText(TTEXT_TEXT_POSITION_Y));
                table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.ty, "PgTextPositionY", 0, mPage.height));

                if (mPage.srPage.jt != ObjHandler::ORI_ABSOLUT)
                    table->hideRow(row);
            break;

            case 16:
                col0->setText(getLeftColText(TTEXT_TEXT_EFFECT));
                table->setCellWidget(row, 1, makeTextEffect(mPage.srPage.et, "PgTextEffect"));
            break;

            case 17:
                col0->setText(getLeftColText(TTEXT_WORD_WRAP));
                table->setCellWidget(row, 1, makeWordWrap(mPage.srPage.ww, "PgWordWrap"));
            break;
        }

        table->setItem(row, 0, col0);
    }
}

void TPropertiesStates::adjustTablePage(QTableWidget *table, QString gradient)
{
    DECL_TRACER("TPropertiesStates::adjustTablePage(QTableWidget *table, QString gradient)");

    QString grad;

    if (!gradient.isEmpty())
        grad = gradient;
    else
        grad = mPage.srPage.ft;

    if (grad == "solid")
    {
        table->hideRow(2);
        table->hideRow(3);
        table->hideRow(4);
        table->hideRow(5);
        table->showRow(1);
    }
    else if (grad == "radial")
    {
        table->showRow(2);
        table->showRow(3);
        table->showRow(4);
        table->showRow(5);
        table->hideRow(1);
    }
    else
    {
        table->hideRow(1);
        table->showRow(2);
        table->hideRow(3);
        table->hideRow(4);
        table->hideRow(5);
    }

    if (mPage.srPage.jt == ObjHandler::ORI_ABSOLUT)
    {
        table->showRow(14);
        table->showRow(15);
    }
    else
    {
        table->hideRow(14);
        table->hideRow(15);
    }
}

void TPropertiesStates::createTablePopup(QTableWidget *table)
{
    DECL_TRACER("TPropertiesStates::createTablePopup(QTableWidget *table)");

    int rows = 21;

    if (mPage.srPage.ff.isEmpty())
    {
        QFont font = mParent->font();
        mPage.srPage.ff = font.family();
    }

    if (mPage.srPage.fs <= 0)
        mPage.srPage.fs = 10;

    // The row count must be defined before rows are added to the table! Otherwise
    // an empty table with only the grid will be visible.
    table->setRowCount(rows);

    for (int row = 0; row < rows; ++row)
    {
        QTableWidgetItem *col0 = new QTableWidgetItem;
        col0->setBackground(Qt::lightGray);
        col0->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);

        switch(row)
        {
            case 0:
                col0->setText(getLeftColText(TTEXT_BORDER_NAME));
                table->setCellWidget(row, 1, makeBorderName(mPage.srPage.bs, "PopupBorderName"));
            break;

            case 1:
                col0->setText(getLeftColText(TTEXT_BORDER_COLOR));
                table->setCellWidget(row, 1, makeColorSelector(mPage.srPage.cb, "PopupBorderColor"));
            break;

            case 2:
                col0->setText(getLeftColText(TTEXT_FILL_TYPE));
                table->setCellWidget(row, 1, makeFillType(mPage.srPage.ft, "PopupFillType"));
            break;

            case 3:
                col0->setText(getLeftColText(TTEXT_FILL_COLOR));
                table->setCellWidget(row, 1, makeColorSelector(mPage.srPage.cf, "PopupFillColor"));
            break;

            case 4:
                col0->setText(getLeftColText(TTEXT_FILL_GRADIENT_COLORS));
                table->setCellWidget(row, 1, makeGradientColors(mPage.srPage.gradientColors, "PopupFillGradientColors"));

                if (mPage.srPage.ft == "solid")
                    table->hideRow(row);
                break;

            case 5:
                col0->setText(getLeftColText(TTEXT_GRADIENT_RADIUS));
                table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.gr, "PopupGradientRadius", 0, 5000));

                if (mPage.srPage.ft != "radial")
                    table->hideRow(row);
                break;

            case 6:
                col0->setText(getLeftColText(TTEXT_GRADIENT_CENTER_X));
                table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.gx, "PopupGradientCenterX"));

                if (mPage.srPage.ft != "radial")
                    table->hideRow(row);
                break;

            case 7:
                col0->setText(getLeftColText(TTEXT_GRADIENT_CENTER_Y));
                table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.gy, "PopupGradientCenterX"));

                if (mPage.srPage.ft != "radial")
                    table->hideRow(row);
                break;

            case 8:
                col0->setText(getLeftColText(TTEXT_TEXT_COLOR));
                table->setCellWidget(row, 1, makeColorSelector(mPage.srPage.ct, "PopupTextColor"));
            break;

            case 9:
                col0->setText(getLeftColText(TTEXT_TEXT_EFFECT_COLOR));
                table->setCellWidget(row, 1, makeColorSelector(mPage.srPage.ec, "PopupTextEffectColor"));
            break;

            case 10:
                col0->setText(getLeftColText(TTEXT_OVERALL_OPACITY));
                table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.oo, "PopupOverallOpacity", 0, 255));
            break;

            case 11:
                col0->setText(getLeftColText(TTEXT_VIDEO_FILL));
                table->setCellWidget(row, 1, makeVideoFill(mPage.srPage.vf, "PopupVideoFill"));
            break;

            case 12:
                col0->setText(getLeftColText(TTEXT_BITMAPS));
                table->setCellWidget(row, 1, makeBitmapSelector(mPage.srPage.bitmaps, "PopupBitmapSelector"));
            break;

            case 13:
                col0->setText(getLeftColText(TTEXT_FONT));
                table->setCellWidget(row, 1, makeFontSelector(mPage.srPage.ff, "PopupFontSelector"));
            break;

            case 14:
                col0->setText(getLeftColText(TTEXT_FONT_SIZE));
                table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.fs, "PopupFontSize"));
            break;

            case 15:
            {
                col0->setText(getLeftColText(TTEXT_TEXT));
                QFont font;

                if (mPage.srPage.ff.isEmpty())
                {
                    font = mParent->font();
                    mPage.srPage.ff = font.family();
                }

                if (mPage.srPage.fs <= 0)
                    font.setPointSize(10);
                else
                    font.setPointSize(mPage.srPage.fs);

                table->setCellWidget(row, 1, makeTextValue(mPage.srPage.te, font, "PopupText"));
                table->resizeRowToContents(row);
            }
            break;

            case 16:
                col0->setText(getLeftColText(TTEXT_TEXT_JUSTIFICATION));
                table->setCellWidget(row, 1, makeTextJustification(mPage.srPage.jt, "PopupTextOrientation"));
            break;

            case 17:
                col0->setText(getLeftColText(TTEXT_TEXT_POSITION_X));
                table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.tx, "PopupTextPositionX", 0, mPage.width));

                if (mPage.srPage.jt != ObjHandler::ORI_ABSOLUT)
                    table->hideRow(row);
                break;

            case 18:
                col0->setText(getLeftColText(TTEXT_TEXT_POSITION_Y));
                table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.ty, "PopupTextPositionY", 0, mPage.height));

                if (mPage.srPage.jt != ObjHandler::ORI_ABSOLUT)
                    table->hideRow(row);
                break;

            case 19:
                col0->setText(getLeftColText(TTEXT_TEXT_EFFECT));
                table->setCellWidget(row, 1, makeTextEffect(mPage.srPage.et, "PopupTextEffect"));
            break;

            case 20:
                col0->setText(getLeftColText(TTEXT_WORD_WRAP));
                table->setCellWidget(row, 1, makeWordWrap(mPage.srPage.ww, "PopupWordWrap"));
            break;
        }

        table->setItem(row, 0, col0);
    }
}

void TPropertiesStates::adjustTablePopup(QTableWidget *table, QString gradient)
{
    DECL_TRACER("TPropertiesStates::adjustTablePopup(QTableWidget *table, QString gradient)");

    QString grad;

    if (!gradient.isEmpty())
        grad = gradient;
    else
        grad = mPage.srPage.ft;

    if (grad == "solid")
    {
        table->hideRow(4);
        table->hideRow(5);
        table->hideRow(6);
        table->hideRow(7);
        table->showRow(3);
    }
    else if (grad == "radial")
    {
        table->showRow(4);
        table->showRow(5);
        table->showRow(6);
        table->showRow(7);
        table->hideRow(3);
    }
    else
    {
        table->hideRow(3);
        table->showRow(4);
        table->hideRow(5);
        table->hideRow(6);
        table->hideRow(7);
    }

    if (mPage.srPage.jt == ObjHandler::ORI_ABSOLUT)
    {
        table->showRow(17);
        table->showRow(18);
    }
    else
    {
        table->hideRow(17);
        table->hideRow(18);
    }
}

QString TPropertiesStates::getLeftColText(int line)
{
    DECL_TRACER("TPropertiesStates::getLeftColText(int line)");

    switch (line)
    {
        case TTEXT_FILL_TYPE:           return tr("Fill Type"); break;
        case TTEXT_FILL_COLOR:          return tr("Fill Color"); break;
        case TTEXT_TEXT_COLOR:          return tr("Text Color"); break;
        case TTEXT_TEXT_EFFECT_COLOR:   return tr("Text Effect Color"); break;
        case TTEXT_VIDEO_FILL:          return tr("Video Fill"); break;
        case TTEXT_BITMAPS:             return tr("Bitmaps"); break;
        case TTEXT_FONT:                return tr("Font"); break;
        case TTEXT_FONT_SIZE:           return tr("Font Size"); break;
        case TTEXT_TEXT:                return tr("Text"); break;
        case TTEXT_TEXT_JUSTIFICATION:  return tr("Text Justification"); break;
        case TTEXT_TEXT_EFFECT:         return tr("Text Effect"); break;
        case TTEXT_WORD_WRAP:           return tr("Word Wrap"); break;
        case TTEXT_BORDER_NAME:         return tr("Border Name"); break;
        case TTEXT_BORDER_COLOR:        return tr("Border Color"); break;
        case TTEXT_OVERALL_OPACITY:     return tr("Overall Opacity"); break;
        case TTEXT_CHAMELEON_IMAGE:     return tr("Chameleon Image"); break;   // Only if there is no border
        case TTEXT_SOUND:               return tr("Sound"); break;
        case TTEXT_FILL_GRADIENT_COLORS: return tr("Gradient Colors"); break;
        case TTEXT_GRADIENT_RADIUS:     return tr("Gradient Radius"); break;
        case TTEXT_GRADIENT_CENTER_X:   return tr("Gradient Center X%"); break;
        case TTEXT_GRADIENT_CENTER_Y:   return tr("Gradient Center Y%"); break;
        case TTEXT_TEXT_POSITION_X:     return tr("Absolut Position X"); break;
        case TTEXT_TEXT_POSITION_Y:     return tr("Absolut Position Y"); break;
    }

    return "???";
}

TElementBorderName *TPropertiesStates::makeBorderName(const QString& border, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeBorderName(const QString& border, const QString& name)");

    TElementBorderName *brd = new TElementBorderName(border, name, mParent);
    connect(brd, &TElementBorderName::borderChanged, this, &TPropertiesStates::onBorderNameChanged);
    return brd;
}

QComboBox *TPropertiesStates::makeFillType(const QString& ftype, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeFillType(int ftype, const QString& name)");

    QList<QString> list = {
        "solid", "radial", "sweep", "left to right",
        "top-left to bottom-right", "top to bottom",
        "top-right to bottom-left", "right to left",
        "bottom-right to top-left", "bottom to top"
    };

    QComboBox *cbox = new QComboBox;
    cbox->setObjectName(name);
    int idx = 0;

    for (QString txt : list)
    {
        cbox->addItem(txt);

        if (txt == ftype)
            cbox->setCurrentIndex(idx);

        idx++;
    }

    connect(cbox, &QComboBox::currentIndexChanged, [this, cbox](int index) {
        mBlocked = true;
        QString objName = cbox->objectName();
        QString item = cbox->itemText(index);
        addGradientLines(item, objName);
        setValue(objName, item);
        mBlocked = false;
    });

    return cbox;
}

QWidget *TPropertiesStates::makeColorSelector(const QColor& col, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeColorSelector(const QColor& col, const QString& name)");

    QWidget *widget = new QWidget;
    widget->setAccessibleIdentifier(col.name(QColor::HexArgb));
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    QLabel *label = new QLabel;
    label->setObjectName(name);
    label->setStyleSheet(QString("background-color: %1").arg(col.name()));
    layout->addWidget(label, 1);

    QPushButton *button = new QPushButton;
    button->setObjectName("PushButtonColorSelector");
    button->setText("...");
    button->setMaximumWidth(30);
    layout->addWidget(button);

    connect(button, &QPushButton::clicked, [this, label, col](bool) {
        mBlocked = true;
        QColor color = col;
        setColor(label, color);
        setValue(label->objectName(), color);
        mBlocked = false;
    });

    return widget;
}

QWidget *TPropertiesStates::makeVideoFill(const QString& vf, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeVideoFill(const QString& vf, const QString& name)");

    QList<QString> items = { "none", "streaming video", "MXA-MPL" };

    QComboBox *cbox = new QComboBox;
    cbox->setObjectName(name);
    int idx = 0;

    for (QString txt : items)
    {
        cbox->addItem(txt);

        if (txt == vf)
            cbox->setCurrentIndex(idx);
    }

    connect(cbox, &QComboBox::currentIndexChanged, [this, cbox](int index) {
        mBlocked = true;
        QString vf = cbox->itemText(index);
        setValue(cbox->objectName(), vf);
        mBlocked = false;
    });

    return cbox;
}

TElementBitmapSelector *TPropertiesStates::makeBitmapSelector(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeBitmapSelector(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name)");

    TElementBitmapSelector *bs = new TElementBitmapSelector(name, bitmaps, mParent);
    connect(bs, &TElementBitmapSelector::bitmapsChanged, this, &TPropertiesStates::onBitmapsChanged);
    return bs;
}

TElementWidgetFont *TPropertiesStates::makeFontSelector(const QString& fname, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeFontSelector(const QString& fname, const QString& name)");

    TElementWidgetFont *widget = new TElementWidgetFont(QFont(fname), name, mParent);
    connect(widget, &TElementWidgetFont::fontChanged, this, &TPropertiesStates::onFontChanged);
    return widget;
}

QSpinBox *TPropertiesStates::makeValueSelector(int value, const QString& name, int start, int max)
{
    DECL_TRACER("TPropertiesStates::makeValueSelector(int value, const QString& name)");

    QSpinBox *spin = new QSpinBox;
    spin->setObjectName(name);

    if (start < max)
        spin->setRange(start, max);

    spin->setValue(value);

    connect(spin, &QSpinBox::valueChanged, [this, name](int value) { setValue(name, value); });
    return spin;
}

QWidget *TPropertiesStates::makeTextValue(const QString& txt,  const QFont& font, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeTextValue(const QString& txt,  const QFont& font, const QString& name)");

    QWidget *widget = new QWidget;

    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    QLineEdit *line = new QLineEdit;
    line->setObjectName(name);
    line->setText(txt);

    QPushButton *button = new QPushButton;
    button->setText("...");
    button->setMaximumWidth(30);

    layout->addWidget(line);
    layout->addWidget(button);

    connect(line, &QLineEdit::textChanged, [this, line](const QString& text) {
        setValue(line->objectName(), text);
    });

    connect(button, &QPushButton::clicked, [this, line, font]() {
        mBlocked = true;
        TTextBoxDialog dialog(mParent);
        dialog.setTextFont(line->text(), font);

        if (dialog.exec() == QDialog::Rejected)
            return;

        line->setText(dialog.getText());
        setValue(line->objectName(), dialog.getText());
        mBlocked = false;
    });

    return widget;
}

TElementWidgetCombo *TPropertiesStates::makeTextJustification(ObjHandler::ORIENTATION ori, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeTextJustification(ObjHandler::ORIENTATION ori, const QString& name)");

    QList<QString> items = { "absolute", "top-left", "top-middle", "top-right",
                            "center-left", "center-middle", "center-right",
                            "bottom-left", "bottom-middle", "bottom-right" };
    QList<QVariant> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mParent);
    combo->addItems(items);
    combo->addData(data);
    combo->setCurrentIndex(ori);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesStates::onOrientationChanged);
    return combo;
}

TElementTextEffect *TPropertiesStates::makeTextEffect(int ef, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeTextEffect(int ef, const QString& name)");

    TElementTextEffect *eff = new TElementTextEffect(ef, name, mParent);
    connect(eff, &TElementTextEffect::effectChanged, this, &TPropertiesStates::onTextEffectChanged);
    return eff;
}

TElementWidgetCombo *TPropertiesStates::makeWordWrap(bool ww, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeWordWrap(bool ww, const QString& name)");

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

TElementGradientColors *TPropertiesStates::makeGradientColors(const QList<QColor>& color, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeGradientColors(const QList<QColor>& color, const QString& name)");

    TElementGradientColors *grad = new TElementGradientColors(color, name, mParent);
    connect(grad, &TElementGradientColors::gradientColorChanged, this, &TPropertiesStates::onGradientColorChanged);
    return grad;
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

    if (name == "PgFillType" || name == "PopupFillType")
        mPage.srPage.ft = value.toString();
    else if (name == "PgFillColor" || name == "PopupFillColor")
        mPage.srPage.cf = value.toString();
    else if (name == "PgTextColor" || name == "PopupTextColor")
        mPage.srPage.ct = value.toString();
    else if (name == "PgTextEffectColor" || name == "PopupTextEffectColor")
        mPage.srPage.ec = value.toString();
    else if (name == "PgVideoFill" || name == "PopupVideoFill")
        mPage.srPage.vf = value.toString();
    else if (name == "PgFontSelector" || name == "PopupFontSelector")
        mPage.srPage.ff = value.toString();
    else if (name == "PgFontSize" || name == "PopupFontSize")
        mPage.srPage.fs = value.toInt();
    else if (name == "PgText" || name == "PopupText")
        mPage.srPage.te = value.toString();
    else if (name == "PgTextOrientation" || name == "PopupTextOrientation")
        mPage.srPage.jt = static_cast<ObjHandler::ORIENTATION>(value.toInt());
    else if (name == "PgTextEffect" || name == "PopupTextEffect")
        mPage.srPage.et = value.toInt();
    else if (name == "PgWordWrap" || name == "PopupWordWrap")
        mPage.srPage.ww = (value.toBool() ? 1 : 0);
    else if (name.startsWith("PgGradientRadius") || name.startsWith("PopupGradientRadius"))
        mPage.srPage.gr = value.toInt();
    else if (name.startsWith("PgGradientCenterX") || name.startsWith("PopupGradientCenterX"))
        mPage.srPage.gx = value.toInt();
    else if (name.startsWith("PgGradientCenterY") || name.startsWith("PopupGradientCenterY"))
        mPage.srPage.gy = value.toInt();
    else if (name == "PgTextPositionX" || name == "PopupTextPositionX")
        mPage.srPage.tx = value.toInt();
    else if (name == "PgTextPositionY" || name == "PopupTextPositionY")
        mPage.srPage.ty = value.toInt();

    if (name == "PopupBorderName")
        mPage.srPage.bs = value.toString();
    else if (name == "PopupBorderColor")
        mPage.srPage.cb = value.toString();
    else if (name == "PopupOverallOpacity")
        mPage.srPage.oo = value.toInt();

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

    Q_UNUSED(gradient);
    Q_UNUSED(init);

    QTreeWidgetItem *root = mTreeWidget->invisibleRootItem();
    QTreeWidgetItem *top = root->child(0);
    QTableWidget *widget = static_cast<QTableWidget *>(mTreeWidget->itemWidget(top->child(0), 0));

    if (!widget)
    {
        MSG_WARNING("Couldn't get the table widget!");
        return;
    }

    if (name == "PgFillType")
        adjustTablePage(widget, gradient);
    else if (name == "PopupFillType")
        adjustTablePopup(widget, gradient);
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

    if (name == "PgTextOrientation")
        adjustTablePage(widget);
    else if (name == "PopupTextOrientation")
        adjustTablePopup(widget);

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

    if (name.startsWith("PgFillGradientColors") || name.startsWith("PopupFillGradientColors"))
        mPage.srPage.gradientColors = colors;

    // TODO: Add code to save it to stages

    saveChangedData(&mPage, TBL_STATES);
    mChanged = false;
    requestRedraw(&mPage);
}
