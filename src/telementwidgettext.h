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
#ifndef TELEMENTWIDGETTEXT_H
#define TELEMENTWIDGETTEXT_H

#include <QWidget>

class QLineEdit;
class QPushButton;

class TElementWidgetText : public QWidget
{
    Q_OBJECT

    public:
        TElementWidgetText(const QString& text, const QString& name, QWidget *parent=nullptr);
        ~TElementWidgetText();

        QString& getText() { return mText; }
        void setText(const QString& text);
        QString& getName() { return mName; }
        void setName(const QString& name);
        void setInstance(int inst) { mInstance = inst; }
        int getInstance() { return mInstance; }

    signals:
        void textChanged(const QString& text, const QString& name);
        void textChangedInst(const QString& text, const QString& name, int instance);

    protected:
        // Callbacks
        void onLineEditTextChanged(const QString& text);
        void onPushButtonClicked();

    private:
        QString mText;
        QString mName;
        QLineEdit *mLine{nullptr};
        QPushButton *mButton{nullptr};
        int mInstance{-1};
};

#endif // TELEMENTWIDGETTEXT_H
