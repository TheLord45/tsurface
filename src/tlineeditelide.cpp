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
#include "tlineeditelide.h"
#include "terror.h"

TLineEditElide::TLineEditElide(QWidget *parent)
    : QLineEdit(parent)
{
    DECL_TRACER("TLineEditElide::TLineEditElide(QWidget *parent)");

    connect(this, &QLineEdit::textChanged, this, &TLineEditElide::onTextChanged);
}

TLineEditElide::TLineEditElide(const QString& string, QWidget *parent)
    : QLineEdit(string, parent)
{
    DECL_TRACER("TLineEditElide::TLineEditElide(const QString& string, QWidget *parent)");

    mText = string;
    connect(this, &QLineEdit::textChanged, this, &TLineEditElide::onTextChanged);
}

TLineEditElide::~TLineEditElide()
{
    DECL_TRACER("TLineEditElide::~TLineEditElide()");

    disconnect(this, &QLineEdit::textChanged, this, &TLineEditElide::onTextChanged);
}

void TLineEditElide::TLineEditElide::onTextChanged(const QString& text)
{
    DECL_TRACER("TLineEditElide::TLineEditElide::onTextChanged(const QString& text)");

    mText = text;
    emit textChanged(text, elideWithThreeDots(text));
}

void TLineEditElide::setText(const QString& text)
{
    DECL_TRACER("TLineEditElide::setText(const QString& text)");

    mText = text;

    if (mElideEnabled)
        QLineEdit::setText(elideWithThreeDots(mText));
    else
        QLineEdit::setText(text);

    setCursorPosition(0);
}

void TLineEditElide::setElide(bool state, const QString& el)
{
    DECL_TRACER("TLineEditElide::setElide(bool state, const QString& el)");

    mElideEnabled = state;

    if (state && !isReadOnly())
        setReadOnly(true);

    if (el.length() > 3)
        mElideString = el.left(3);
    else
        mElideString = el;

    if (state && !mText.isEmpty())
        QLineEdit::setText(elideWithThreeDots(mText));
}

QString TLineEditElide::elideWithThreeDots(const QString& s)
{
    const int width = contentsRect().width();
    const QString dots = mElideString + " ";
    const QFontMetrics fm(font());
    const int dotsW = fm.horizontalAdvance(dots);

    if (fm.horizontalAdvance(s) <= width)
        return s;
    // If even "..." is wider than the available width, still return "..."
    // (QLineEdit will clip it visually).
    if (dotsW >= width)
        return mElideString;

    int lo = 0, hi = s.size();

    while (lo < hi)
    {
        int mid = (lo + hi + 1) / 2;

        if (fm.horizontalAdvance(s.left(mid) + dots) <= width)
            lo = mid;
        else
            hi = mid - 1;
    }

    return s.left(lo) + dots;
}
