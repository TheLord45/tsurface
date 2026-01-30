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
#ifndef TBITMAPDIALOG_H
#define TBITMAPDIALOG_H

#include <QDialog>
#include "tobjecthandler.h"

namespace Ui {
    class TBitmapDialog;
}

class QTableWidget;
class QTreeWidgetItem;
class QLineEdit;
class QSpinBox;

class TElementWidgetCombo;
class TElementBitmapSelector;

class TBitmapDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TBitmapDialog(QWidget *parent = nullptr);
        ~TBitmapDialog();

        void setBitmaps(const QList<ObjHandler::BITMAPS_t>& bm);
        QList<ObjHandler::BITMAPS_t>& getBitmaps() { return mBitmaps; }

    protected:
        QTableWidget *createTable(const ObjHandler::BITMAPS_t& bm);
        TElementBitmapSelector *makeBitmapSelector(const ObjHandler::BITMAPS_t& bitmap, const QString& name);
        TElementWidgetCombo *makeTextJustification(ObjHandler::ORIENTATION ori, const QString& name);
        QSpinBox *makeValueSelector(int value, const QString& name);
        void setValue(const QString& name, const QVariant& value);
        void addBitmap(const ObjHandler::BITMAPS_t& bm);
        // Callbacks
        void onTreeWidgetItemActivated(QTreeWidgetItem *item, int column);
        void onBitmapsChanged(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name);
        void onOrientationChanged(const QString& text, const QVariant& data, const QString& name);

    private slots:
        void on_pushButtonAdd_clicked();
        void on_pushButtonDelete_clicked();
        void on_pushButtonMoveUp_clicked();
        void on_pushButtonMoveDown_clicked();
        void on_pushButtonClear_clicked();

    private:
        Ui::TBitmapDialog *ui;
        int mRowCount{0};
        int mSelected{-1};
        QList<ObjHandler::BITMAPS_t> mBitmaps;
};

#endif // TBITMAPDIALOG_H
