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

    QStandardItem *parentItem = mModel->invisibleRootItem();
    QStandardItem *top = new QStandardItem("none");
    top->setData(SEL_NONE);
    parentItem->appendRow(top);

    for (FAMILY_t border : borders)
    {
        top = new QStandardItem(border.name);
        top->setData(SEL_TOP);
        top->setFlags(top->flags() & ~Qt::ItemIsSelectable);
        parentItem->appendRow(top);

        for (QString member : border.member)
        {
            int e = TGraphics::Current().getEffectStyleNumber(member);
            QStandardItem *item = new QStandardItem(member);
            item->setData(e);
            top->appendRow(item);
        }
    }

    mCombo->setModel(mModel);
    mCombo->setModelColumn(0);

    if (!brd.isEmpty())
        mCombo->setCurrentText(brd);

    connect(mCombo, &QComboBox::currentTextChanged, this, &TElementBorderName::onComboTextChanged);
}

TElementBorderName::~TElementBorderName()
{
    DECL_TRACER("TElementBorderName::~TElementBorderName()");
}

void TElementBorderName::onComboTextChanged(const QString& text)
{
    DECL_TRACER("TElementBorderName::onComboTextChanged(const QString& text)");

    mBorder = text;
    emit borderChanged(text, mName);
    mTreeView->collapseAll();
}
