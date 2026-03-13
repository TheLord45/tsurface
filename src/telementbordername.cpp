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

    const QModelIndexList matches = mModel->match(mModel->index(0, 0), Qt::DisplayRole, border, 1, Qt::MatchExactly | Qt::MatchRecursive);

    if (!matches.isEmpty())
    {
        const QModelIndex modelIdx = matches.first();
        // Temporarily set the root to the parent to allow row-based selection
        mCombo->setRootModelIndex(modelIdx.parent());
        mCombo->setCurrentIndex(modelIdx.row());
        // Restore the original root to show the full tree
        mCombo->setRootModelIndex(QModelIndex());
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
