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
#ifndef TGRADIENTCOLORDIALOG_H
#define TGRADIENTCOLORDIALOG_H

#include <QDialog>

namespace Ui {
    class TGradientColorDialog;
}

class TGradientColorDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TGradientColorDialog(const QList<QColor>& colors, QWidget *parent = nullptr);
        ~TGradientColorDialog();

        QList<QColor>& getColors() { return mColors; }

    protected:
        QWidget *makeColorSelector(const QColor& color, int row);
        void updateRow(const QColor& color, int row);
        void createTable();
        void deselectAll();

    private slots:
        void on_pushButtonAdd_clicked();
        void on_pushButtonDelete_clicked();
        void on_pushButtonMoveUp_clicked();
        void on_pushButtonMoveDown_clicked();
        void on_tableWidgetColors_cellActivated(int row, int column);

    private:
        Ui::TGradientColorDialog *ui;
        QList<QColor> mColors;
        int mSelectedRow{-1};
};

#endif // TGRADIENTCOLORDIALOG_H
