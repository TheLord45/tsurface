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
#ifndef TTEXTBOXDIALOG_H
#define TTEXTBOXDIALOG_H

#include <QDialog>

namespace Ui {
    class TTextBoxDialog;
}

class TTextBoxDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TTextBoxDialog(QWidget *parent = nullptr);
        ~TTextBoxDialog();

        void setTextFont(const QString& text, const QFont& font);
        QString& getText() { return mText; }

    private slots:
        void on_textEditText_textChanged();
        void on_checkBoxPreview_checkStateChanged(const Qt::CheckState &arg1);
        void on_pushButtonCharacterMap_clicked();

    private:
        Ui::TTextBoxDialog *ui;
        QString mText;
        QFont mFont;
        QFont mDefaultFont;
};

#endif // TTEXTBOXDIALOG_H
