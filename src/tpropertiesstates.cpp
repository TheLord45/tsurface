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

#include "tpropertiesstates.h"
#include "tconfmain.h"
#include "terror.h"

using namespace Page;

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

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_STATES);

    mChanged = false;
    mPage = TPageHandler::Current().getPage(name);
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

    if (!loaded && mPage.pageID > 0 && mChanged)
    {
        saveChangedData(&mPage);
        mPage = Page::PAGE_t();
    }

    mChanged = false;

    if (!loaded)
        mPage = TPageHandler::Current().getPage(id);

    if (mPage.pageID <= 0)
        return;

    createPage();
}

void TPropertiesStates::createPage()
{
    DECL_TRACER("TPropertiesStates::createPage()");

    int setupPort = TConfMain::Current().getSetupPort();
    mInitialized = false;
    QBrush brush;
    brush.setColor(Qt::GlobalColor::lightGray);
    mTreeWidget->clear();
    mTreeWidget->setColumnCount(1);

    if (mPage.popupType == PT_PAGE)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, tr("Off"));
    }
}

QWidget *TPropertiesStates::createTableWidget(STATE_TYPE stype)
{
    DECL_TRACER("TPropertiesStates::createTableWidget(STATE_TYPE stype)");

    QBrush brush;
    brush.setColor(Qt::GlobalColor::lightGray);
    QTableWidget *table = new QTableWidget;
    table->setColumnCount(2);
    int rows = 0;

    if (stype == STATE_PAGE)
        rows = 12;
    else if (stype == STATE_POPUP)
        rows = 15;
    else if (stype == STATE_BUTTON)
        rows = 17;
    else if (stype == STATE_BARGRAPH)
        rows = 16;

    table->setRowCount(rows);

    for (int row = 0; row < rows; ++row)
    {
        QTableWidgetItem *col0 = new QTableWidgetItem;
        col0->setBackground(brush);
        col0->setText(getLeftColText(stype, 0, row));

        switch(row)
        {
            case 0:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeFillType());
            break;

            case 1:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeColorSelector(mPage.srPage.cf, "FillColor"));
            break;

            case 2:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeColorSelector(mPage.srPage.cf, "TextColor"));
            break;

            case 3:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeColorSelector(mPage.srPage.cf, "TextEffectColor"));
            break;

            case 4:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeVideoFill());
            break;

            case 5:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeBitmapSelector(mPage.srPage.bitmaps[0].fileName, "BitmapSelector"));
            break;
        }
    }

    return table;
}

QString TPropertiesStates::getLeftColText(STATE_TYPE stype, int state, int line)
{
    DECL_TRACER("TPropertiesStates::getLeftColText(Page::STATE_TYPE stype, int state, int line)");

    if (stype == STATE_PAGE)
    {
        switch(line)
        {
            case 0: return tr("Fill Type");
            case 1: return tr("Fill Color");
            case 2: return tr("Text Color");
            case 3: return tr("Text Effect Color");
            case 4: return tr("Video Fill");
            case 5: return tr("Bitmaps");
            case 6: return tr("Font");
            case 7: return tr("Font Size");
            case 8: return tr("Text");
            case 9: return tr("Text Justification");
            case 10: return tr("Text Effect");
            case 11: return tr("Word Wrap");
        }
    }
    else if (stype == STATE_POPUP)
    {
        switch(line)
        {
            case 0: return tr("Border Name");
            case 1: return tr("Border Color");
            case 2: return tr("Fill Type");
            case 3: return tr("Fill Color");
            case 4: return tr("Text Color");
            case 5: return tr("Text Effect Color");
            case 6: return tr("Overall Opacity");
            case 7: return tr("Video Fill");
            case 8: return tr("Bitmaps");
            case 9: return tr("Font");
            case 10: return tr("Font Size");
            case 11: return tr("Text");
            case 12: return tr("Text Justification");
            case 13: return tr("Text Effect");
            case 14: return tr("Word Wrap");
        }
    }
    else if (stype == STATE_BUTTON)   // All states
    {
        switch(line)
        {
            case 0: return tr("Border Name");
            case 1: return tr("Chameleon Image");   // Only if there is no border
            case 2: return tr("Border Color");
            case 3: return tr("Fill Type");
            case 4: return tr("Fill Color");
            case 5: return tr("Text Color");
            case 6: return tr("Text Effect Color");
            case 7: return tr("Overall Opacity");
            case 8: return tr("Video Fill");
            case 9: return tr("Bitmaps");
            case 10: return tr("Font");
            case 11: return tr("Font Size");
            case 12: return tr("Text");
            case 13: return tr("Text Justification");
            case 14: return tr("Text Effect");
            case 15: return tr("Word Wrap");
            case 16: return tr("Sound");
        }
    }
    else if (stype == STATE_BARGRAPH)   // All states
    {
        switch(line)
        {
            case 0: return tr("Border Name");
            case 1: return tr("Chameleon Image");   // Only if there is no border
            case 2: return tr("Border Color");
            case 3: return tr("Fill Type");
            case 4: return tr("Fill Color");
            case 5: return tr("Text Color");
            case 6: return tr("Text Effect Color");
            case 7: return tr("Overall Opacity");
            case 8: return tr("Video Fill");
            case 9: return tr("Bitmaps");
            case 10: return tr("Font");
            case 11: return tr("Font Size");
            case 12: return tr("Text");
            case 13: return tr("Text Justification");
            case 14: return tr("Text Effect");
            case 15: return tr("Word Wrap");
        }
    }

    return "";
}

QWidget *TPropertiesStates::makeFillType()
{
    DECL_TRACER("TPropertiesStates::makeFillType()");

    mComboBoxFillType = new QComboBox;
    mComboBoxFillType->addItem("solid");
    mComboBoxFillType->addItem("radial");
    mComboBoxFillType->addItem("sweep");
    mComboBoxFillType->addItem("left to right");
    mComboBoxFillType->addItem("top-left to bottom-right");
    mComboBoxFillType->addItem("top to bottom");
    mComboBoxFillType->addItem("top-right to bottom-left");
    mComboBoxFillType->addItem("right to left");
    mComboBoxFillType->addItem("bottom-right to top-left");
    mComboBoxFillType->addItem("bottom to top");
    connect(mComboBoxFillType, &QComboBox::currentIndexChanged, this, &TPropertiesStates::onComboBoxFillTypeIndex);
    return mComboBoxFillType;
}

QWidget *TPropertiesStates::makeColorSelector(const QColor& col, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeColorSelector(const QColor& col, const QString& name)");

    mWidgetColorSelector = new QWidget;
    mWidgetColorSelector->setObjectName(name);
    mWidgetColorSelector->setAccessibleIdentifier(col.name(QColor::HexArgb));
    QHBoxLayout *layout = new QHBoxLayout(mWidgetColorSelector);
    layout->setContentsMargins(0, 0, 0, 0);

    QLabel *label = new QLabel;
    label->setObjectName("LabelColorSelector");
    QSizePolicy policy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(0);
    label->setSizePolicy(policy);
    label->setStyleSheet(QString("background-color: %1").arg(col.name()));
    layout->addWidget(label);

    QPushButton *button = new QPushButton;
    button->setObjectName("PushButtonColorSelector");
    button->setText("...");
    connect(button, &QPushButton::clicked, this, &TPropertiesStates::onPushButtonColorSelector);
    layout->addWidget(button);
    return mWidgetColorSelector;
}

QWidget *TPropertiesStates::makeVideoFill()
{
    DECL_TRACER("TPropertiesStates::makeVideoFill()");

    mComboBoxVideoFill = new QComboBox;
    mComboBoxVideoFill->addItem("none");
    mComboBoxVideoFill->addItem("streaming video");
    mComboBoxVideoFill->addItem("MXA-MPL");     // Not supported by TPanel!
    connect(mComboBoxVideoFill, &QComboBox::currentIndexChanged, this, &TPropertiesStates::onComboBoxVideoFill);
    return mComboBoxVideoFill;
}

QWidget *TPropertiesStates::makeBitmapSelector(const QString& bitmap, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeBitmapSelector(const QString& bitmap, const QString& name)");

    mWidgetBitmapSelector = new QWidget;
    mWidgetBitmapSelector->setObjectName(name);

    QHBoxLayout *layout = new QHBoxLayout(mWidgetBitmapSelector);
    layout->setContentsMargins(0, 0, 0, 0);

    QLineEdit *line = new QLineEdit;
    QSizePolicy policy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(0);
    line->setSizePolicy(policy);
    line->setText(bitmap);
    layout->addWidget(line);

    QPushButton *button = new QPushButton;
    button->setText("...");
    connect(button, &QPushButton::clicked, this, &TPropertiesStates::onPushButtonBitmapSelector);
    layout->addWidget(button);
    return mWidgetBitmapSelector;
}

// Callbacks

void TPropertiesStates::onComboBoxFillTypeIndex(int index)
{

}

void TPropertiesStates::onPushButtonColorSelector(bool checked)
{
    DECL_TRACER("TPropertiesStates::onPushButtonColorSelector(bool checked)");

    QColorDialog colDialog(mParent);
    colDialog.setCurrentColor(mWidgetColorSelector->accessibleIdentifier());

    if (colDialog.exec() == QDialog::Rejected)
        return;

    QColor col = colDialog.selectedColor();
    QString objName = mWidgetColorSelector->objectName();

    QObjectList childs = mWidgetColorSelector->children();

    for (QObject *obj : childs)
    {
        QString name = obj->objectName();

        if (name == "LabelColorSelector")
        {
            QLabel *label = static_cast<QLabel*>(obj);
            label->setStyleSheet(QString("background-color: %1").arg(col.name(QColor::HexArgb)));
            label->setAccessibleIdentifier(col.name(QColor::HexArgb));
            mChanged = true;
            break;
        }
    }

    if (objName == "FillColor")
        mPage.srPage.cf = col;
}


void TPropertiesStates::onComboBoxVideoFill(int index)
{

}

void TPropertiesStates::onPushButtonBitmapSelector(bool checked)
{

}

