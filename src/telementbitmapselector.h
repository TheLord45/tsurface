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
#ifndef TELEMENTBITMAPSELECTOR_H
#define TELEMENTBITMAPSELECTOR_H

#include <QWidget>

#include "tobjecthandler.h"

class QLineEdit;
class QPushButton;

/**
 * @brief The TElementBitmapSelector class
 * The class creates a widget with an input line and a push button. It is
 * designed to be displayed in a cell of a QTableWidget or a QTreeWidget.
 * The class allows to select between the selection of a single bitmap or
 * the selection up to 5 bitmaps.
 * if "mSingleBitmap" is TRUE, clicking on the push button displays a
 * dialog to select a single bitmap. This is used when this class is used
 * in the dialog class "TBitmapDialog", for example.
 * The default behavior is the selection of multiple bitmaps. Clicking on
 * the push button displays a dialog where the selected bitmaps are
 * visible. The dialog box allows to manage the bitmaps.
 */
class TElementBitmapSelector : public QWidget
{
    Q_OBJECT

    public:
        TElementBitmapSelector(QWidget *parent=nullptr);
        TElementBitmapSelector(const QString& name, const ObjHandler::BITMAPS_t& bm, QWidget *parent=nullptr);
        TElementBitmapSelector(const QString& name, const QList<ObjHandler::BITMAPS_t>& bm, QWidget *parent=nullptr);

        void start();
        ObjHandler::BITMAPS_t& getBitmap() { return mBitmaps[0]; }
        QList<ObjHandler::BITMAPS_t>& getBitmaps() { return mBitmaps; }
        QString& getName() { return mName; }
        void setBitmap(const ObjHandler::BITMAPS_t& bm) { mBitmaps.insert(0, bm); }
        void setBitmaps(const QList<ObjHandler::BITMAPS_t>& bm) { mBitmaps = bm; }
        void setName(const QString& name) { mName = name; }
        void setSingleBitmap(bool state) { mSingleBitmap = state; }

    signals:
        void bitmapsChanged(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name);

    protected:
        void init();
        // Callbacks
        void onLineEditTextChanged(const QString& txt);
        void onPushButtonClicked();

    private:
        QLineEdit *mLine{nullptr};
        QPushButton *mButton{nullptr};
        QString mName;
        QList<ObjHandler::BITMAPS_t> mBitmaps;
        bool mSingleBitmap{false};          // TRUE = Hitting the "mButton" displays the bitmap selection dialog
};

#endif // TELEMENTBITMAPSELECTOR_H
