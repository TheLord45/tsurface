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

#include "telementtexteffect.h"
#include "tgraphics.h"
#include "terror.h"

using namespace Graphics;

TElementTextEffect::TElementTextEffect(int eff, const QString& name, QWidget *parent)
    : QWidget(parent),
      mName(name)
{
    DECL_TRACER("TElementTextEffect::TElementTextEffect(int eff, const QString& name, QWidget *parent)");

    QList<FAMILY_t> effects = TGraphics::Current().getEfects();

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
    QString text;

    if (eff > 0)
        text = TGraphics::Current().getEffectStyleName(eff);

    for (FAMILY_t effect : effects)
    {
        top = new QStandardItem(effect.name);
        top->setData(SEL_TOP);
        top->setFlags(top->flags() & ~Qt::ItemIsSelectable);
        parentItem->appendRow(top);

        for (QString member : effect.member)
        {
            int e = TGraphics::Current().getEffectStyleNumber(member);
            QStandardItem *item = new QStandardItem(member);
            item->setData(e);
            top->appendRow(item);
        }
    }

    mCombo->setModel(mModel);
    mCombo->setModelColumn(0);

    if (eff > 0 && !text.isEmpty())
    {
        const QModelIndexList matches = mModel->match(mModel->index(0, 0), Qt::DisplayRole, text, 1, Qt::MatchExactly | Qt::MatchRecursive);

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

    connect(mCombo, &QComboBox::currentTextChanged, this, &TElementTextEffect::onComboTextChanged);
}

TElementTextEffect::~TElementTextEffect()
{
    DECL_TRACER("TElementTextEffect::~TElementTextEffect()");
}

void TElementTextEffect::onComboTextChanged(const QString& text)
{
    DECL_TRACER("TElementTextEffect::onComboTextChanged(const QString& text)");

    int eff = TGraphics::Current().getEffectStyleNumber(text);
    emit effectChanged(eff, text, mName);
    mTreeView->collapseAll();
}
