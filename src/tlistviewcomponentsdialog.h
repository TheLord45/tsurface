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
#ifndef TLISTVIEWCOMPONENTSDIALOG_H
#define TLISTVIEWCOMPONENTSDIALOG_H

#include <QDialog>

namespace Ui {
    class TListViewComponentsDialog;
}

class TListViewComponentsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit TListViewComponentsDialog(QWidget *parent = nullptr);
        ~TListViewComponentsDialog();


        bool primaryText() const;
        void setPrimaryText(bool newPrimaryText);
        bool secondaryText() const;
        void setSecondaryText(bool newSecondaryText);
        bool image() const;
        void setImage(bool newImage);

    protected:
        void accept() override;

    private slots:
        void on_checkBoxPrimaryText_checkStateChanged(const Qt::CheckState &arg1);
        void on_checkBoxSecondaryText_checkStateChanged(const Qt::CheckState &arg1);
        void on_checkBoxImage_checkStateChanged(const Qt::CheckState &arg1);

    private:
        Ui::TListViewComponentsDialog *ui;

        bool mPrimaryText{false};
        bool mSecondaryText{false};
        bool mImage{false};
};

#endif // TLISTVIEWCOMPONENTSDIALOG_H
