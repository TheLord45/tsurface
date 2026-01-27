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
#ifndef TCHARACTERMAPDIALOG_H
#define TCHARACTERMAPDIALOG_H

#include <QDialog>

class TCharacterWidget;
class QLineEdit;
class QScrollArea;

class TCharacterMapDialog : public QDialog
{
    Q_OBJECT

    public:
        TCharacterMapDialog(QWidget *parent=nullptr);
        TCharacterMapDialog(const QFont& font, QWidget *parent=nullptr);
        ~TCharacterMapDialog();

        QString& getActualCharacter() { return mActualCharacter; }
        void setFont(const QFont& font);

    public slots:
        void insertCharacter(const QString &character);
        void updateClipboard();

    protected:
        void init();

    private:
        TCharacterWidget *mCharacterWidget{nullptr};
        QClipboard *mClipboard{nullptr};
        QLineEdit *mLineEditFont{nullptr};
        QLineEdit *mLineEditFontSize{nullptr};
        QLineEdit *mLineEdit{nullptr};
        QScrollArea *mScrollArea{nullptr};

        QString mActualCharacter;
        QFont mFont;
};

#endif // TCHARACTERMAPDIALOG_H
