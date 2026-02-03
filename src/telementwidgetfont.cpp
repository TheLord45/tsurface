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
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFontDialog>

#include "telementwidgetfont.h"
#include "terror.h"

TElementWidgetFont::TElementWidgetFont(const QFont& font, const QString& name, QWidget *parent)
    : QWidget(parent),
      mFont(font),
      mName(name)
{
    DECL_TRACER("TElementWidgetFont::TElementWidgetFont(const QFont& font, const QString& name, QWidget *parent)");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    mLine = new QLineEdit;
    mLine->setObjectName(name);
    mLine->setText(font.family());

    mButton = new QPushButton;
    mButton->setText("...");
    mButton->setMaximumWidth(30);

    layout->addWidget(mLine);
    layout->addWidget(mButton);

    connect(mLine, &QLineEdit::textChanged, this, &TElementWidgetFont::onLineEditTextChanged);
    connect(mButton, &QPushButton::clicked, this, &TElementWidgetFont::onPushButtonClicked);
}

TElementWidgetFont::~TElementWidgetFont()
{
    DECL_TRACER("TElementWidgetFont::~TElementWidgetFont()");
}

void TElementWidgetFont::onLineEditTextChanged(const QString& text)
{
    DECL_TRACER("TElementWidgetFont::onLineEditTextChanged(const QString& text)");

    if (mBlocked)
        return;

    mFont.setFamily(text);
    emit fontChanged(mFont, mName);
}

void TElementWidgetFont::onPushButtonClicked()
{
    DECL_TRACER("TElementWidgetFont::onPushButtonClicked()");

    bool ok = false;
    QFont f = QFontDialog::getFont(&ok, mFont, this, tr("Select font"));

    if (ok)
    {
        mFont = f;
        mBlocked = true;
        mLine->setText(mFont.family());
        emit fontChanged(mFont, mName);
        mBlocked = false;
    }
}
