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
#include <QToolButton>
#include <QHBoxLayout>

#include "telementevent.h"
#include "teventactionsdialog.h"
#include "terror.h"

TElementEvent::TElementEvent(const QString& name, int instance, QWidget *widget)
    : QWidget(widget),
      mName(name),
      mInstance(instance)
{
    DECL_TRACER("TElementEvent::TElementEvent(const QString& name, int instance, QWidget *widget)");

    init();
}

TElementEvent::TElementEvent(ObjHandler::BUTTON_EVENT_t event, const QString& name, int instance, QWidget *widget)
    : QWidget(widget),
      mName(name),
      mInstance(instance),
      mEventType(event)
{
    DECL_TRACER("TElementEvent::TElementEvent(ObjHandler::BUTTON_EVENT_t event, const QString& name, int instance, QWidget *widget)");

    init();
}

void TElementEvent::init()
{
    DECL_TRACER("TElementEvent::init()");

    mLine = new QLineEdit;
    mLine->setReadOnly(true);

    mButton = new QToolButton;
    mButton->setText("...");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(mLine);
    layout->addWidget(mButton);

    connect(mButton, &QToolButton::clicked, this, &TElementEvent::onClicked);
}

void TElementEvent::onClicked()
{
    DECL_TRACER("TElementEvent::onClicked()");

    TEventActionsDialog dlg(this);
    dlg.setFuncs(mFuncs);

    if (dlg.exec() == QDialog::Rejected)
        return;

    mFuncs = dlg.getFuncs();
    emit eventChanged(mFuncs, mName, mInstance);
}
