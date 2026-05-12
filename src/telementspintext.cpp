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
#include <QSpinBox>
#include <QLineEdit>
#include <QHBoxLayout>

#include "telementspintext.h"
#include "terror.h"

TElementSpinText::TElementSpinText(int port, const QString& cmd, const QString& name, QWidget *parent)
    : QWidget(parent),
      mPort(port),
      mCommand(cmd),
      mName(name)
{
    DECL_TRACER("TElementSpinText::TElementSpinText(int port, const QString& cmd, QWidget *parent)");

    setContentsMargins(0, 0, 0, 0);

    mSpinPort = new QSpinBox;
    mSpinPort->setRange(0, 999999);
    mSpinPort->setValue(port);

    mLineEdit = new QLineEdit;

    if (cmd.isEmpty())
        mLineEdit->setText("none");
    else
        mLineEdit->setText(cmd);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mSpinPort);
    layout->addWidget(mLineEdit);
    connect(mSpinPort, &QSpinBox::valueChanged, this, &TElementSpinText::onValueChanged);
    connect(mLineEdit, &QLineEdit::textEdited, this, &TElementSpinText::onTextChanged);
}

void TElementSpinText::setRange(int start, int end)
{
    DECL_TRACER("TElementSpinText::setRange(int start, int end)");

    if (start > end || start == end)
        return;

    QSignalBlocker sigBlock(mSpinPort);
    mSpinPort->setRange(start, end);
}

void TElementSpinText::setContent(int port, const QString& cmd)
{
    DECL_TRACER("TElementSpinText::setContent(int port, const QString& cmd)");

    mPort = port;
    mCommand = cmd;

    QSignalBlocker sigSpin(mSpinPort);
    QSignalBlocker sigLine(mLineEdit);

    mSpinPort->setValue(port);
    mLineEdit->setText(cmd);
}

void TElementSpinText::onValueChanged(int value)
{
    DECL_TRACER("TElementSpinText::onValueChanged(int value)");

    if (mBlock || mPort == value)
        return;

    mPort = value;
    emit contentChanged(mPort, mCommand, mName);
}

void TElementSpinText::onTextChanged(const QString& text)
{
    DECL_TRACER("TElementSpinText::onTextChanged(const QString& text)");

    if (mBlock || mCommand == text)
        return;

    mCommand = text;

    if (mCommand.isEmpty())
    {
        mCommand = "none";
        QSignalBlocker sigBlock(mLineEdit);
        mLineEdit->setText(mCommand);
    }

    emit contentChanged(mPort, mCommand, mName);
}
