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
#include <QComboBox>
#include <QTreeView>
#include <QStandardItemModel>
#include <QLineEdit>

#include "telementbordername.h"
#include "tgraphics.h"
#include "terror.h"

using namespace Graphics;

TElementBorderName::TElementBorderName(const QString& brd, const QString& name, QWidget *parent)
    : QWidget(parent),
      mName(name)
{
    DECL_TRACER("TElementBorderName::TElementBorderName(int eff, const QString& name, QWidget *parent)");

    QList<FAMILY_t> borders = TGraphics::Current().getBorders();

    QSignalBlocker sigBlock(this);
    mCombo = new QComboBox(this);
    mTreeView = new QTreeView(mCombo);
    mTreeView->setHeaderHidden(true);
    mTreeView->setRootIsDecorated(true);
    mTreeView->setExpandsOnDoubleClick(true);
    mTreeView->setUniformRowHeights(true);
    mCombo->setView(mTreeView);

    mModel = new QStandardItemModel(mCombo);
    mModel->setColumnCount(1);
    mModel->setHorizontalHeaderLabels(QStringList() << "Items");

    mParentItem = mModel->invisibleRootItem();
    QStandardItem *top = new QStandardItem("none");
    top->setData(SEL_NONE);
    mParentItem->appendRow(top);
    mSelected = 0;

    for (FAMILY_t border : borders)
    {
        top = new QStandardItem(border.name);
        top->setData(SEL_TOP);
        top->setFlags(top->flags() & ~Qt::ItemIsSelectable);
        mParentItem->appendRow(top);

        for (QString member : border.member)
        {
            BORDER_STYLE_t style = TGraphics::Current().getBorderStyle(member);
            QStandardItem *item = new QStandardItem(member);
            item->setData(style.number);
            top->appendRow(item);

            if (!brd.isEmpty() && member == brd)
                mSelected = style.number;
        }
    }

    mCombo->setModel(mModel);
    mCombo->setModelColumn(0);

    if (!brd.isEmpty())
        setBorder(brd);

    connect(mCombo, &QComboBox::currentTextChanged, this, &TElementBorderName::onComboTextChanged);
    connect(mCombo, &QComboBox::currentIndexChanged, this, &TElementBorderName::onComboIndexChanged);
}

TElementBorderName::~TElementBorderName()
{
    DECL_TRACER("TElementBorderName::~TElementBorderName()");
}

void TElementBorderName::setBorder(const QString& border)
{
    DECL_TRACER("TElementBorderName::setBorder(const QString& border)");

    QSignalBlocker sigBlock(this);
    mBorder = border;

    if (!border.isEmpty())
    {
        QLineEdit *line = mCombo->lineEdit();

        if (!line)
        {
            MSG_WARNING("ComboBox has no line edit!");
            return;
        }

        line->setText(border);
/*        bool found = false;
        Graphics::BORDER_STYLE_t style = TGraphics::Current().getBorderStyle(border);
        MSG_DEBUG("Searching for border " << border.toStdString() << " with number " << style.number);

        if (style.number > 0)
        {
            int index = 0;

            for (int row = 0; row < mParentItem->rowCount(); ++row)
            {
                QStandardItem *item = mParentItem->child(row, 0);
                MSG_DEBUG("Item " << item->text().toStdString() << " with data " << item->data().toInt());

                if (item->data().toInt() <= 0)
                {
                    for (int r = 0; r < item->rowCount(); ++r)
                    {
                        QStandardItem *itemDetail = item->child(r, 0);
                        MSG_DEBUG("Item detail " << itemDetail->text().toStdString() << " with data " << itemDetail->data().toInt());

                        if (itemDetail && itemDetail->data().toInt() == style.number)
                        {
                            QModelIndex modIndex = mModel->indexFromItem(itemDetail);
                            MSG_DEBUG("Using Index: " << index << " (item row: " << itemDetail->row() << ", model index row: " << modIndex.row() << ")");
                            mCombo->setCurrentIndex(modIndex.row());
//                            mTreeView->setCurrentIndex(itemDetail->index());
//                            QItemSelectionModel *selModel = mCombo->view()->selectionModel();
//                            selModel->setCurrentIndex(itemDetail->index(), QItemSelectionModel::Current);
                            found = true;
                            break;
                        }

//                        index++;
                    }
                }

                if (found)
                    break;

                index++;
            }
        } */
    }
}

void TElementBorderName::onComboTextChanged(const QString& text)
{
    DECL_TRACER("TElementBorderName::onComboTextChanged(const QString& text)");

    mBorder = text;
    emit borderChanged(text, mName);
    emit borderChangedInst(text, mName, mInstance);
    mTreeView->collapseAll();
}

void TElementBorderName::onComboIndexChanged(int index)
{
    DECL_TRACER("TElementBorderName::onComboIndexChanged(int index)");

    int number = mCombo->itemData(index).toInt();
    MSG_DEBUG("Data changed: " << number << " at index " << index);

    if (number > 0)
        emit borderDataChanged(number, mName, mInstance);
}
