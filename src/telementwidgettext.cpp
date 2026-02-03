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

#include "telementwidgettext.h"
#include "ttexteditdialog.h"
#include "terror.h"

TElementWidgetText::TElementWidgetText(const QString& text, const QString& name, QWidget *parent)
    : QWidget(parent),
      mText(text),
      mName(name)
{
    DECL_TRACER("TElementWidgetText::TElementWidgetText(const QString& text, QWidget *parent)");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    mLine = new QLineEdit;
    mLine->setObjectName(mName);
    mLine->setText(text);

    mButton = new QPushButton;
    mButton->setText("...");
    mButton->setMaximumWidth(30);

    layout->addWidget(mLine);
    layout->addWidget(mButton);

    connect(mLine, &QLineEdit::textChanged, this, &TElementWidgetText::onLineEditTextChanged);
    connect(mButton, &QPushButton::clicked, this, &TElementWidgetText::onPushButtonClicked);
}

TElementWidgetText::~TElementWidgetText()
{
    DECL_TRACER("TElementWidgetText::~TElementWidgetText()");
}

void TElementWidgetText::setName(const QString& name)
{
    DECL_TRACER("TElementWidgetText::setName(const QString& name)");

    mName = name;

    if (!name.isEmpty())
        setObjectName(name);
}

void TElementWidgetText::onLineEditTextChanged(const QString& text)
{
    DECL_TRACER("TElementWidgetText::onLineEditTextChanged(const QString& text)");

    mText = text;
    emit textChanged(text, mName);
}

void TElementWidgetText::onPushButtonClicked()
{
    DECL_TRACER("TElementWidgetText::onPushButtonClicked()");

    TTextEditDialog dialog(this);
    dialog.setCaption(tr("Enter text"));
    dialog.setDescription(mText);

    if (dialog.exec() == QDialog::Rejected)
        return;

    mText = dialog.getDescription();
    mLine->setText(mText);
}
