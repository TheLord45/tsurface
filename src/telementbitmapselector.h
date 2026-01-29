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

class TElementBitmapSelector : public QWidget
{
    Q_OBJECT

    public:
        TElementBitmapSelector(QWidget *parent=nullptr);
        TElementBitmapSelector(const QString& name, const ObjHandler::BITMAPS_t& bm, QWidget *parent=nullptr);

        void start();
        ObjHandler::BITMAPS_t& getBitmap() { return mBitmap; }
        QString& getName() { return mName; }
        void setBitmap(const ObjHandler::BITMAPS_t& bm) { mBitmap = bm; }
        void setName(const QString& name) { mName = name; }

    protected:
        void init();
        // Callbacks
        void onLineEditTextChanged(const QString& txt);
        void onPushButtonClicked();

    private:
        QLineEdit *mLine{nullptr};
        QPushButton *mButton{nullptr};
        QString mName;
        ObjHandler::BITMAPS_t mBitmap;
};

#endif // TELEMENTBITMAPSELECTOR_H
