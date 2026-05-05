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
#ifndef TELEMENTSPINTEXT_H
#define TELEMENTSPINTEXT_H

#include <QWidget>

class QSpinBox;
class QLineEdit;

class TElementSpinText : public QWidget
{
    Q_OBJECT

    public:
        TElementSpinText(int port, const QString& cmd, const QString& name, QWidget *parent=nullptr);

        void setRange(int start, int end);
        void setContent(int port, const QString& cmd);

    signals:
        void contentChanged(int port, const QString& text, const QString& name);

    protected:
        void onValueChanged(int value);
        void onTextChanged(const QString& text);

    private:
        int mPort{0};
        QString mCommand;
        QString mName;
        bool mBlock{false};

        QSpinBox *mSpinPort{nullptr};
        QLineEdit *mLineEdit{nullptr};
};

#endif // TELEMENTSPINTEXT_H
