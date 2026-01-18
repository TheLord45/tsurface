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

    mPage = Page::PAGE_t();
    mPage = TPageHandler::Current().getPage(name);
    setGeneralPage(mPage.pageID);
}

void TPropertiesGeneral::setGeneralPage(int id)
{
    DECL_TRACER("TPropertiesGeneral::setGeneralPage(int id)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    if (mPage.pageID <= 0)
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

    mPage = Page::PAGE_t();
    mPage = TPageHandler::Current().getPage(name);
    setGeneralPopup(mPage.pageID);
}

void TPropertiesGeneral::setGeneralPopup(int id)
{
    DECL_TRACER("TPropertiesGeneral::setGeneralPopup(int id)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    if (mPage.pageID < 500 || mPage.pageID >= 1000)
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
        QTableWidgetItem *cell2 = new QTableWidgetItem;

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
                delete cell2;
                cell2 = nullptr;
                mTable->setCellWidget(i, 1, mComboPopupType);
            }
            break;

            case 1:
                cell1->setText(tr("Name"));
                cell2->setText(mPage.name);
            break;

            case 2:
                cell1->setText(tr("Description"));
                delete cell2;
                cell2 = nullptr;
                mTable->setCellWidget(i, 1, makeLabelTool(mPage.description, i));
            break;

            case 3:
            {
                cell1->setText(tr("Left"));
                delete cell2;
                cell2 = nullptr;
                QSpinBox *spBox = new QSpinBox;
                spBox->setMinimum(0);
                spBox->setMaximum(panelSize.width());
                spBox->setValue(mPage.left);
                mTable->setCellWidget(i, 1, spBox);
            }
            break;

            case 4:
            {
                cell1->setText(tr("Top"));
                delete cell2;
                cell2 = nullptr;
                QSpinBox *spBox = new QSpinBox;
                spBox->setMinimum(0);
                spBox->setMaximum(panelSize.height());
                spBox->setValue(mPage.top);
                mTable->setCellWidget(i, 1, spBox);
            }
            break;

            case 5:
            {
                cell1->setText(tr("Width"));
                delete cell2;
                cell2 = nullptr;
                QSpinBox *spBox = new QSpinBox;
                spBox->setMinimum(0);
                spBox->setMaximum(panelSize.width());
                spBox->setValue(mPage.width);
                mTable->setCellWidget(i, 1, spBox);
            }
            break;

            case 6:
            {
                cell1->setText(tr("Height"));
                delete cell2;
                cell2 = nullptr;
                QSpinBox *spBox = new QSpinBox;
                spBox->setMinimum(0);
                spBox->setMaximum(panelSize.height());
                spBox->setValue(mPage.height);
                mTable->setCellWidget(i, 1, spBox);
            }
            break;

            case 7:
            {
                cell1->setText(tr("Reset pos on show"));
                QComboBox *cbBox = new QComboBox;
                cbBox->addItem(tr("No"), false);
                cbBox->addItem(tr("Yes"), true);
                delete cell2;
                cell2 = nullptr;
                mTable->setCellWidget(i, 1, cbBox);
            }
            break;

            case 8:
            {
                cell1->setText(tr("Group"));
                QComboBox *cbBox = new QComboBox;
                cbBox->setEditable(true);
                QStringList groups = TPageHandler::Current().getGroupNames();
                QStringList::Iterator iter;
                cbBox->addItem("");

                for (iter = groups.begin(); iter != groups.end(); ++iter)
                    cbBox->addItem(*iter);

                delete cell2;
                cell2 = nullptr;
                mTable->setCellWidget(i, 1, cbBox);
            }
            break;

            case 9:
            {
                cell1->setText(tr("Timeout"));
                delete cell2;
                cell2 = nullptr;
                QSpinBox *spBox = new QSpinBox;
                spBox->setMinimum(0);
                spBox->setMaximum(300);
                spBox->setValue(mPage.timeout);
                mTable->setCellWidget(i, 1, spBox);
            }
            break;

            case 10:
            {
                cell1->setText(tr("Modal"));
                QComboBox *cbBox = new QComboBox;
                cbBox->addItem(tr("No"), false);
                cbBox->addItem(tr("Yes"), true);
                delete cell2;
                cell2 = nullptr;
                mTable->setCellWidget(i, 1, cbBox);
            }
            break;

            case 11:
            {
                cell1->setText(tr("Show effect"));
                QComboBox *cbBox = new QComboBox;
                cbBox->addItem(tr("None"), Page::SE_NONE);
                cbBox->addItem(tr("Fade"), Page::SE_FADE);
                cbBox->addItem(tr("Slide from left"), Page::SE_SLIDE_LEFT);
                cbBox->addItem(tr("Slide from right"), Page::SE_SLIDE_RIGHT);
                cbBox->addItem(tr("Slide from top"), Page::SE_SLIDE_TOP);
                cbBox->addItem(tr("Slide from bottom"), Page::SE_SLIDE_BOTTOM);
                cbBox->addItem(tr("Slide from left fade"), Page::SE_SLIDE_LEFT_FADE);
                cbBox->addItem(tr("Slide from right fade"), Page::SE_SLIDE_RIGHT_FADE);
                cbBox->addItem(tr("Slide from top fade"), Page::SE_SLIDE_TOP_FADE);
                cbBox->addItem(tr("Slide from bottom fade"), Page::SE_SLIDE_BOTTOM_FADE);
                delete cell2;
                cell2 = nullptr;
                mTable->setCellWidget(i, 1, cbBox);
            }
            break;

            case 12:
            {
                cell1->setText(tr("Hide effect"));
                QComboBox *cbBox = new QComboBox;
                cbBox->addItem(tr("None"), Page::SE_NONE);
                cbBox->addItem(tr("Fade"), Page::SE_FADE);
                cbBox->addItem(tr("Slide to left"), Page::SE_SLIDE_LEFT);
                cbBox->addItem(tr("Slide to right"), Page::SE_SLIDE_RIGHT);
                cbBox->addItem(tr("Slide to top"), Page::SE_SLIDE_TOP);
                cbBox->addItem(tr("Slide to bottom"), Page::SE_SLIDE_BOTTOM);
                cbBox->addItem(tr("Slide to left fade"), Page::SE_SLIDE_LEFT_FADE);
                cbBox->addItem(tr("Slide to right fade"), Page::SE_SLIDE_RIGHT_FADE);
                cbBox->addItem(tr("Slide to top fade"), Page::SE_SLIDE_TOP_FADE);
                cbBox->addItem(tr("Slide to bottom fade"), Page::SE_SLIDE_BOTTOM_FADE);
                delete cell2;
                cell2 = nullptr;
                mTable->setCellWidget(i, 1, cbBox);
            }
            break;

            case 13:
            {
                cell1->setText(tr("Collapse direction"));
                QComboBox *cbBox = new QComboBox;
                cbBox->addItem(tr("None"), Page::COLDIR_NONE);
                cbBox->addItem(tr("Left"), Page::COLDIR_LEFT);
                cbBox->addItem(tr("Right"), Page::COLDIR_RIGHT);
                cbBox->addItem(tr("Up"), Page::COLDIR_UP);
                cbBox->addItem(tr("Down"), Page::COLDIR_DOWN);
                delete cell2;
                cell2 = nullptr;
                mTable->setCellWidget(i, 1, cbBox);
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
    mLineDescription = line;
    connect(line, &QLineEdit::textChanged, this, &TPropertiesGeneral::onLineEditTextChanged);

    QPushButton *button = new QPushButton;
    button->setObjectName(QString("ToolButton_%1").arg(id));
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
            emit dataChanged(&mPage);
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
            emit dataChanged(&mPage);
            mChanged = true;
        }
    }
}

void TPropertiesGeneral::extractChangedData(int id, QWidget *item)
{
    DECL_TRACER("TPropertiesGeneral::extractChangedData(int id, QWidget *item)");

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
        emit dataChanged(&mPage);
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

        emit dataChanged(&mPage);
        mChanged = true;
    }
}

void TPropertiesGeneral::onComboPopupTypeChanged(int index)
{
    DECL_TRACER("TPropertiesGeneral::onComboPopupTypeChanged(int index)");

    mPage.popupType = static_cast<Page::PAGE_TYPE>(mComboPopupType->itemData(index).toInt());
}

void TPropertiesGeneral::onSpinLeftValue(int value)
{

}

void TPropertiesGeneral::onSpinTopValue(int value)
{

}

void TPropertiesGeneral::onSpinWidthValue(int value)
{

}

void TPropertiesGeneral::onSpinHeightValue(int value)
{

}

void TPropertiesGeneral::onComboResetPosChanged(int index)
{

}

void TPropertiesGeneral::onComboGroupTextChanged(const QString& text)
{

}

void TPropertiesGeneral::onSpinTimeoutValue(int value)
{

}

void TPropertiesGeneral::onComboModalChanged(int index)
{

}

void TPropertiesGeneral::onComboShowChanged(int index)
{

}

void TPropertiesGeneral::onComboHideChanged(int index)
{

}

void TPropertiesGeneral::onComboColapseChange(int value)
{

}
