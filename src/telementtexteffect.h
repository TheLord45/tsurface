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
#ifndef TELEMENTTEXTEFFECT_H
#define TELEMENTTEXTEFFECT_H

#include <QWidget>

class QTreeView;
class QStandardItemModel;
class QModelIndex;
class QComboBox;

class TElementTextEffect : public QWidget
{
    Q_OBJECT

    public:
        TElementTextEffect(int eff, const QString& name, QWidget *parent=nullptr);
        ~TElementTextEffect();

        QString& getEffect() { return mEffect; }

    signals:
        void effectChanged(int number, const QString& effect, const QString& name);

    protected:
        void onComboTextChanged(const QString& text);

    private:
        const int SEL_NONE{-1};
        const int SEL_TOP{0};

        QComboBox *mCombo{nullptr};
        QTreeView *mTreeView{nullptr};
        QStandardItemModel *mModel{nullptr};
        QString mName;
        QString mEffect;
};

#endif // TELEMENTTEXTEFFECT_H
