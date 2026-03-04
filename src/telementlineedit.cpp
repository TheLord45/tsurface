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
#include <QLineEdit>

#include "telementlineedit.h"
#include "terror.h"

TElementLineEdit::TElementLineEdit(const QString& text, const QString& name, QWidget *parent)
    : QWidget(parent),
      mName(name),
      mText(text)
{
    DECL_TRACER("TElementLineEdit::TElementLineEdit(const QString& text, const QString& name, QWidget *parent)");

    mLine = new QLineEdit(this);
    mLine->setText(text);
    connect(mLine, &QLineEdit::textEdited, this, &TElementLineEdit::onTextChanged);
}

TElementLineEdit::~TElementLineEdit()
{
    DECL_TRACER("TElementLineEdit::~TElementLineEdit()");
}

void TElementLineEdit::setText(const QString& text)
{
    DECL_TRACER("TElementLineEdit::setText(const QString& text)");

    mText = text;
    QSignalBlocker block(this);
    mLine->setText(text);
}

void TElementLineEdit::onTextChanged(const QString& text)
{
    DECL_TRACER("TElementLineEdit::onTextChanged(const QString& text)");

    mText = text;
    emit inputTextChanged(text, mName);
}
