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
#ifndef TLISTVIEWMOCK_H
#define TLISTVIEWMOCK_H

#include <QColor>
#include <QFont>

class QPixmap;
class QPainter;

class TListViewMock
{
    public:
        TListViewMock();
        TListViewMock(int width, int height, QPixmap *bm);
        ~TListViewMock();

        void drawListview();
        // Getter/Setter
        void setWidth(int width);
        void setHeight(int height);
        QPixmap *pixmap() const;
        void setPixmap(QPixmap *newPixmap);
        void setFillColor1(const QColor& col);
        void setFillColor2(const QColor& col);
        void setTextColor(const QColor& col) { mTextColor = col; }
        void setFont(const QFont& font) { mFont = font; }
        void setFontSize(int pt);
        int getLvc() const;
        void setLvc(int newLvc);
        int getLvh() const;
        void setLvh(int newLvh);
        int getLvl() const;
        void setLvl(int newLvl);
        int getLvg() const;
        void setLvg(int newLvg);
        int getLhp() const;
        void setLhp(int newLhp);
        int getLvp() const;
        void setLvp(int newLvp);
        int getLvs() const;
        void setLvs(int newLvs);
        int getLsh() const;
        void setLsh(int newLsh);
        int getLva() const;
        void setLva(int newLva);

    protected:
        int drawItem(int start, QPainter *painter);

    private:
        QPixmap *mPixmap{nullptr};
        bool mInternPixmap{false};

        int mWidth{0};
        int mHeight{0};
        QColor mBorderColor{Qt::white};
        QColor mFillColor1{qRgb(0xff, 0xa1, 0)};
        QColor mFillColor2{qRgb(0x97, 0xbe, 0x0d)};
        QColor mTextColor{Qt::white};
        QFont mFont;
        int mFontSize{10};
        int mLvc{0};             // G5 Listview: Listview components? [ ORed values: (2 = Primary Text; 4 = Primary+Secondary Text; 1 = Image only)]
        int mLvh{48};            // G5 Listview: Item height
        int mLvl{1};             // G5 Listview: Item layout --> 4 = vertical, image top; 2 = horizontal, image right; 1 = horizontal, image left
        int mLvg{0};             // G5 Listview: Listview columns
        int mLhp{5};             // G5 Listview: Primary Partition (%)
        int mLvp{95};            // G5 Listview: Secondary Partition (%)
        int mLvs{0};             // G5 Listview: Filter enabled; 1 = TRUE --> Search input line with magnifying glass on top
        int mLsh{24};            // G5 Listview: Filter height
        int mLva{0};             // G5 Listview: Alphabet scrollbar; 1 = TRUE
};

#endif // TLISTVIEWMOCK_H
