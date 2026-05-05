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
#ifndef TCOMBOBUTTON_H
#define TCOMBOBUTTON_H

#include <QPushButton>

class QListWidget;
class QListWidgetItem;

class TComboButton : public QPushButton
{
    Q_OBJECT

    public:
        explicit TComboButton(QWidget* parent = nullptr);
        TComboButton(const QString& text, QWidget* parent = nullptr);
        TComboButton(const QString& text, const QStringList& items, QWidget* parent = nullptr);
        ~TComboButton() override;

        void setItems(const QStringList& items);
        void setData(const QList<QVariant>& data);
        void enableItem(int index, bool state);
        void disableItem(int index, bool state);
        int currentIndex() const { return mCurrentIndex; }
        QString currentText() const { return text(); }
        void setCurrentIndex(int index);
        QString getText(int index);
        bool isEnabled(int index);
        bool isDisabled(int index);

    signals:
        void currentIndexChanged(int index);
        void currentTextChanged(const QString& text);

    protected:
        void resizeEvent(QResizeEvent* e) override;
        bool eventFilter(QObject* obj, QEvent* ev) override;
        void onItemSelected(QListWidgetItem* item);

    private:
        void init();
        void showPopup();
        void hidePopup();
        void updatePopupGeometry() const;

        QListWidget *mList = nullptr;   // non-modal, top-level list
        int mCurrentIndex = -1;
        QStringList mItems;
        QList<int> mDisabled;
        QString mText;
};

#endif // COMBO_BUTTON_H
