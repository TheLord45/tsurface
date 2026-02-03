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
#ifndef TELEMENTWIDGETFONT_H
#define TELEMENTWIDGETFONT_H

#include <QWidget>

class QLineEdit;
class QPushButton;

class TElementWidgetFont : public QWidget
{
    Q_OBJECT

    public:
        TElementWidgetFont(const QFont& font, const QString& name, QWidget *parent);
        ~TElementWidgetFont();

        QFont &getFont() { return mFont; }
        QString& getName() { return mName; }

    signals:
        void fontChanged(const QFont& font, const QString& name);

    protected:
        // Callbacks
        void onLineEditTextChanged(const QString& text);
        void onPushButtonClicked();

    private:
        QFont mFont;
        QString mName;
        QLineEdit *mLine{nullptr};
        QPushButton *mButton{nullptr};
        bool mBlocked{false};
};

#endif // TELEMENTWIDGETFONT_H
