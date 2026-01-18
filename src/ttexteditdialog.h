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
#ifndef TTEXTEDITDIALOG_H
#define TTEXTEDITDIALOG_H

#include <QDialog>

namespace Ui {
class TTextEditDialog;
}

class TTextEditDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TTextEditDialog(QWidget *parent = nullptr);
        ~TTextEditDialog();

        void setDescription(const QString& text);
        QString& getDescription() { return mText; }
        void setCaption(const QString& text);

    private slots:
        void on_plainTextEditDescription_textChanged();

    private:
        Ui::TTextEditDialog *ui;
        QString mText;
};

#endif // TTEXTEDITDIALOG_H
