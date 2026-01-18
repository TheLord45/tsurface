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
#ifndef TCOMBOBOXITEMDELEGATE_H
#define TCOMBOBOXITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class TComboBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        TComboBoxItemDelegate(QObject *parent = nullptr);
        ~TComboBoxItemDelegate();

        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
        void setEditorData(QWidget *editor, const QModelIndex &index) const override;
        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

        typedef struct CBENTRY_t
        {
            QString entry;
            QVariant data;
        }CBENTRY_t;

        void setBoxEntries(QList<CBENTRY_t>& list) { mEntries = list; };

    private:
        QList<CBENTRY_t> mEntries;
};

#endif // TCOMBOBOXITEMDELEGATE_H
