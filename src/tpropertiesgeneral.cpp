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
#include "tconfmain.h"
#include "ttexteditdialog.h"
#include "terror.h"

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

void TPropertiesGeneral::setGeneralPage(const QString& name)
{
    DECL_TRACER("TPropertiesGeneral::setPage(const QString& name)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_GENERIC);

    mChanged = false;
    mPage = Page::PAGE_t();
    mPage = TPageHandler::Current().getPage(name);
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

    if (!loaded && mPage.pageID > 0 && mChanged)
    {
        saveChangedData(&mPage, TBL_GENERIC);
        mPage = Page::PAGE_t();
    }

    mChanged = false;

    if (!loaded)
        mPage = TPageHandler::Current().getPage(id);

    if (mPage.pageID <= 0)
        return;

    mInitialized = false;
    QBrush brush;
    brush.setColor(Qt::GlobalColor::lightGray);
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

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_GENERIC);

    mChanged = false;
    mPage = Page::PAGE_t();
    mPage = TPageHandler::Current().getPage(name);
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

    if (!loaded && mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_GENERIC);

    mChanged = false;

    if (!loaded)
        mPage = TPageHandler::Current().getPage(id);

    if (mPage.pageID <= 0)
        return;

    mInitialized = false;
    QBrush brush;
    brush.setColor(Qt::GlobalColor::lightGray);
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

QWidget* TPropertiesGeneral::makeLabelTool(const QString& text, int id)
{
    DECL_TRACER("TPropertiesGeneral::makeLabelTool(const QString& text, int id)");
    QWidget *widget = new QWidget;
    widget->setObjectName(QString("Widget_%1").arg(id));

    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setObjectName(QString("Layout_%1").arg(id));
    layout->setContentsMargins(0, 0, 0, 0);

    QLineEdit *line = new QLineEdit(text);
    line->setObjectName(QString("lineEdit_%1").arg(id));

    QSizePolicy policy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(0);
    policy.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
    line->setSizePolicy(policy);
    mLineDescription = line;
    connect(line, &QLineEdit::textChanged, this, &TPropertiesGeneral::onLineEditTextChanged);

    QSizePolicy policyBt(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
    policyBt.setHorizontalStretch(0);
    policyBt.setVerticalStretch(0);
    policyBt.setHeightForWidth(true);

    QPushButton *button = new QPushButton;
    button->setObjectName(QString("ToolButton_%1").arg(id));
    button->setSizePolicy(policyBt);
    button->setText("...");
    connect(button, &QPushButton::clicked, this, &TPropertiesGeneral::onToolButtonClicked);

    layout->addWidget(line);
    layout->addWidget(button);
    return widget;
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

/**
 * @brief TPropertiesGeneral::onLineEditTextChanged
 * This method is called whenever the only edit line changes. Because there
 * is only one, it is allways the description.
 *
 * @param text  The changed text
 */
void TPropertiesGeneral::onLineEditTextChanged(const QString& text)
{
    DECL_TRACER("TPropertiesGeneral::onLineEditTextChanged(const QString& text)");

    if (mPage.popupType == Page::PT_PAGE || mPage.popupType == Page::PT_POPUP)
    {
        mPage.description = text;
        saveChangedData(&mPage, TBL_GENERIC);
        mChanged = true;
    }
}

/**
 * @brief TPropertiesGeneral::onToolButtonClicked
 * This method can be activated only if the button, the only one, is clicked.
 * This means, that the description may have changed.
 */
void TPropertiesGeneral::onToolButtonClicked()
{
    DECL_TRACER("TPropertiesGeneral::onToolButtonClicked()");

    if (mPage.popupType == Page::PT_PAGE || mPage.popupType == Page::PT_POPUP)
    {
        TTextEditDialog dialog(mParent);
        dialog.setCaption(tr("Description"));
        dialog.setDescription(mPage.description);

        if (dialog.exec() == QDialog::Rejected)
            return;

        mPage.description = dialog.getDescription();

        if (mLineDescription)
            mLineDescription->setText(mPage.description);

        saveChangedData(&mPage, TBL_GENERIC);
        mChanged = false;
    }
}

void TPropertiesGeneral::onComboPopupTypeChanged(int index)
{
    DECL_TRACER("TPropertiesGeneral::onComboPopupTypeChanged(int index)");

    mPage.popupType = static_cast<Page::PAGE_TYPE>(mComboPopupType->itemData(index).toInt());
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
