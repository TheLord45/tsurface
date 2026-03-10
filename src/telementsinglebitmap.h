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
#ifndef TELEMENTSINGLEBITMAP_H
#define TELEMENTSINGLEBITMAP_H

#include <QWidget>

class QLineEdit;
class QToolButton;

class TElementSingleBitmap : public QWidget
{
    Q_OBJECT

    public:
        TElementSingleBitmap(const QString& name, QWidget *parent=nullptr);
        TElementSingleBitmap(const QString& bm, const QString& name, QWidget *parent=nullptr);
        ~TElementSingleBitmap();

        void setPixmapName(const QString& bm);
        QString& getPixmapName() { return mPixmapName; }
        void setInstance(int inst) { mInstance = inst; }
        int getInstance() { return mInstance; }

    signals:
        void bitmapChanged(const QString& bm, const QString& name);
        void bitmapChangedInst(const QString& bm, const QString& name, int instance);

    protected:
        void init();

        void onLineEditTextChanged(const QString& text);
        void onPushButtonClicked();

    private:
        QString mPixmapName;
        QString mName;
        int mInstance{0};
        QLineEdit *mLine{nullptr};
        QToolButton *mButton{nullptr};
};

#endif // TELEMENTSINGLEBITMAP_H
