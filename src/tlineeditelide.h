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
#ifndef TLINEEDITELIDE_H
#define TLINEEDITELIDE_H

#include <QLineEdit>

class TLineEditElide : public QLineEdit
{
    Q_OBJECT
    public:
        TLineEditElide(QWidget *parent=nullptr);
        TLineEditElide(const QString& string, QWidget *parent=nullptr);
        ~TLineEditElide();

        void setText(const QString& text);
        void setElide(bool state, const QString& el="...");

    signals:
        void textChanged(const QString& text, const QString& elided);

    protected:
        void onTextChanged(const QString& text);

    private:
        QString elideWithThreeDots(const QString& s);

        bool mElideEnabled{false};
        QString mElideString{"..."};
        QString mText;
};

#endif // TLINEEDITELIDE_H
