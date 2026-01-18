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

#include "tcomboboxitemdelegate.h"
#include "terror.h"

TComboBoxItemDelegate::TComboBoxItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    DECL_TRACER("TComboBoxItemDelegate::TComboBoxItemDelegate(QObject *parent)");
}

TComboBoxItemDelegate::~TComboBoxItemDelegate()
{
    DECL_TRACER("TComboBoxItemDelegate::~TComboBoxItemDelegate()");
}

QWidget *TComboBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    DECL_TRACER("TComboBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)");

    // Create the combobox and populate it
    QComboBox *cb = new QComboBox(parent);

    if (!mEntries.empty())
    {
        QList<CBENTRY_t>::ConstIterator iter;

        for (iter = mEntries.cbegin(); iter != mEntries.cend(); ++iter)
            cb->addItem(iter->entry, iter->data);
    }

    return cb;
}

void TComboBoxItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    DECL_TRACER("TComboBoxItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const");

    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    // get the index of the text in the combobox that matches the current value of the item
    const QString currentText = index.data(Qt::EditRole).toString();
    const int cbIndex = cb->findText(currentText);
    // if it is valid, adjust the combobox
    if (cbIndex >= 0)
        cb->setCurrentIndex(cbIndex);
}

void TComboBoxItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    DECL_TRACER("TComboBoxItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const");

    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    model->setData(index, cb->currentText(), Qt::EditRole);
}
