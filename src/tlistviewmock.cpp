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
#include <QPixmap>
#include <QPainter>

#include "tlistviewmock.h"
#include "tobjecthandler.h"
#include "tfonts.h"
#include "terror.h"

TListViewMock::TListViewMock()
{
    DECL_TRACER("TListViewMock::TListViewMock()");
}

TListViewMock::TListViewMock(int width, int height, QPixmap *bm)
    : mPixmap(bm),
      mWidth(width),
      mHeight(height)
{
    DECL_TRACER("TListViewMock::TListViewMock(QPixmap *bm)");
}

TListViewMock::~TListViewMock()
{
    DECL_TRACER("TListViewMock::~TListViewMock()");

    if (mInternPixmap && mPixmap)
        delete mPixmap;
}

void TListViewMock::drawListview()
{
    DECL_TRACER("TListViewMock::drawListview()");

    if (!mWidth || !mHeight)
    {
        MSG_ERROR("Invalid width or height!");
        return;
    }

    if (!mPixmap)
    {
        mPixmap = new QPixmap(mWidth, mHeight);
        mPixmap->fill(Qt::transparent);
        mInternPixmap = true;
    }

    if (mPixmap->isNull())
    {
        MSG_ERROR("Invalid pixmap!");
        return;
    }

    QPixmap pxSearch(":images/system-search.svg");
    QPainter painter(mPixmap);
    QPen frame(mBorderColor);
    frame.setStyle(Qt::SolidLine);
    frame.setWidth(2);
    int yPos = 0;
    // If there is a filter defined, draw it first on top of list
    if (mLvs)
    {
        painter.setPen(frame);
        painter.drawRect(0, 0, -1, mLsh);
        painter.drawPixmap(3, 3, mLsh - 6, mLsh - 6, pxSearch);
        mFont.setPixelSize(mLsh - 6);
        painter.setFont(mFont);
        painter.drawText(QRect(6 + mLsh, 3, mWidth - mLsh - 6, mLsh - 6), "Search", QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
        yPos = mLsh;
    }

    while (yPos < mHeight)
    {
        yPos = drawItem(yPos, &painter);
    }

    painter.end();
}

int TListViewMock::drawItem(int start, QPainter *painter)
{
    DECL_TRACER("TListViewMock::drawItem(int start, QPainter *painter)");

    QFont font = mFont;
    font.setPointSize(mFontSize);

    QPen frame(mBorderColor);
    frame.setStyle(Qt::SolidLine);
    frame.setWidth(1);

    QPen text(mTextColor);
    text.setStyle(Qt::SolidLine);
    text.setWidth(1);

    painter->setPen(frame);
    painter->setFont(font);
    painter->drawRect(0, start, mWidth, mLvh);
    MSG_DEBUG("mLvc: " << mLvc);

    if (mLvc & LIST_TEXT_PRIMARY)
    {
        int l = 3;
        int height1 = static_cast<double>(mLvh) / 100.0 * static_cast<double>(mLhp);

        if (mLvc & LIST_IMAGE_CELL)
            l = static_cast<double>(mWidth) / 100.0 * static_cast<double>(mLhp);

        MSG_DEBUG("Drawing 1st text in rect: " << l << ", " << (start + 3) << ", " << mWidth << ", " << height1);
        painter->setPen(text);
        painter->drawText(QRect(l, start + 3, mWidth, height1), "Primary Text", QTextOption(Qt::AlignLeft | Qt::AlignVCenter));

        if (mLvc & LIST_TEXT_SECONDARY)
        {
            int height2 = mLvh - height1;
            MSG_DEBUG("Drawing 2nd text in rect: " << l << ", " << (start + height1 + 1) << ", " << mWidth << ", " << height2);
            painter->drawText(QRect(l, start + height1 + 1, mWidth, height2), "Secondary Text", QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
        }
    }

    if (mLvc & LIST_IMAGE_CELL)
    {
        int w = static_cast<double>(mWidth) / 100.0 * static_cast<double>(mLhp);
        int h = static_cast<double>(mLvh) / 100.0 * static_cast<double>(mLhp);

        QPixmap px(":images/user-mowgli.svg");
        px = px.scaled(QSize(w, qMin(h * 2, mLvh - 2)), Qt::KeepAspectRatio);
        QRect iArea((w - px.width()) / 2, start + 2 + ((mLvh - px.height()) / 2), px.width(), px.height());
        MSG_DEBUG("Start: " << start << ", iArea: " << iArea.x() << ", " << iArea.y() << ", " << iArea.width() << ", " << iArea.height());
        painter->drawPixmap(iArea, px);
    }

    return start + mLvh;
}

// Getter/Setter
QPixmap *TListViewMock::pixmap() const
{
    DECL_TRACER("TListViewMock::pixmap() const");

    return mPixmap;
}

void TListViewMock::setPixmap(QPixmap *newPixmap)
{
    DECL_TRACER("TListViewMock::setPixmap(QPixmap *newPixmap)");

    if (mInternPixmap && mPixmap)
    {
        delete mPixmap;
        mInternPixmap = false;
    }

    mPixmap = newPixmap;
}

void TListViewMock::setFillColor1(const QColor& col)
{
    DECL_TRACER("TListViewMock::setFillColor1(const QColor& col)");

    if (col == mFillColor2)
        return;

    mFillColor1 = col;
}

void TListViewMock::setFillColor2(const QColor& col)
{
    DECL_TRACER("TListViewMock::setFillColor2(const QColor& col)");

    if (col == mFillColor1)
        return;

    mFillColor2 = col;
}

void TListViewMock::setFontSize(int pt)
{
    DECL_TRACER("TListViewMock::setFontSize(int pt)");

    if (pt < 1)
        return;

    mFontSize = pt;
}

int TListViewMock::getLvc() const
{
    DECL_TRACER("TListViewMock::getLvc() const");

    return mLvc;
}

void TListViewMock::setLvc(int newLvc)
{
    DECL_TRACER("TListViewMock::setLvc(int newLvc)");

    mLvc = newLvc;
}

int TListViewMock::getLvh() const
{
    DECL_TRACER("TListViewMock::getLvh() const");

    return mLvh;
}

void TListViewMock::setLvh(int newLvh)
{
    DECL_TRACER("TListViewMock::setLvh(int newLvh)");
    mLvh = newLvh;
}

int TListViewMock::getLvl() const
{
    DECL_TRACER("TListViewMock::getLvl() const");
    return mLvl;
}

void TListViewMock::setLvl(int newLvl)
{
    DECL_TRACER("TListViewMock::setLvl(int newLvl)");

    mLvl = newLvl;
}

int TListViewMock::getLvg() const
{
    DECL_TRACER("TListViewMock::getLvg() const");

    return mLvg;
}

void TListViewMock::setLvg(int newLvg)
{
    DECL_TRACER("TListViewMock::setLvg(int newLvg)");

    mLvg = newLvg;
}

int TListViewMock::getLhp() const
{
    DECL_TRACER("TListViewMock::getLhp() const");

    return mLhp;
}

void TListViewMock::setLhp(int newLhp)
{
    DECL_TRACER("TListViewMock::setLhp(int newLhp)");

    mLhp = newLhp;
}

int TListViewMock::getLvp() const
{
    DECL_TRACER("TListViewMock::getLvp() const");

    return mLvp;
}

void TListViewMock::setLvp(int newLvp)
{
    DECL_TRACER("TListViewMock::setLvp(int newLvp)");

    mLvp = newLvp;
}

int TListViewMock::getLvs() const
{
    DECL_TRACER("TListViewMock::getLvs() const");

    return mLvs;
}

void TListViewMock::setLvs(int newLvs)
{
    DECL_TRACER("TListViewMock::setLvs(int newLvs)");

    mLvs = newLvs;
}

int TListViewMock::getLsh() const
{
    DECL_TRACER("TListViewMock::getLsh() const");

    return mLsh;
}

void TListViewMock::setLsh(int newLsh)
{
    DECL_TRACER("TListViewMock::setLsh(int newLsh)");

    mLsh = newLsh;
}

int TListViewMock::getLva() const
{
    DECL_TRACER("TListViewMock::getLva() const");

    return mLva;
}

void TListViewMock::setLva(int newLva)
{
    mLva = newLva;
}
