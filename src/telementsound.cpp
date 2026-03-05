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
#include <QLabel>
#include <QToolButton>

#include "tsoundselectdialog.h"
#include "telementsound.h"
#include "terror.h"

TElementSound::TElementSound(QWidget *parent)
    : QWidget(parent)
{
    DECL_TRACER("TElementSound::TElementSound(QWidget *parent)");

    init();
}

TElementSound::TElementSound(const QString& sound, const QString &name, QWidget *parent)
    : QWidget(parent),
      mSound(sound),
      mName(name)
{
    DECL_TRACER("TElementSound::TElementSound(const QString& sound, const QString &name, QWidget *parent)");

    init();
}

void TElementSound::init()
{
    DECL_TRACER("TElementSound::init()");

    if (!mName.isEmpty() && mName != objectName())
        setObjectName(mName);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    mLine = new QLabel;
    mLine->setText(mSound);
    layout->addWidget(mLine, 1);

    mButton = new QToolButton;
    layout->addWidget(mButton);

    connect(mButton, &QToolButton::clicked, this, &TElementSound::onPushButtonClicked);
}

QString TElementSound::getSound() const
{
    DECL_TRACER("TElementSound::getSound()");

    return mSound;
}

void TElementSound::setSound(const QString& sound)
{
    DECL_TRACER("TElementSound::setSound(const QString &sound)");

    mSound = sound;
    mLine->setText(sound);
}

void TElementSound::onPushButtonClicked()
{
    DECL_TRACER("TElementSound::onPushButtonClicked()");

    // TODO: Call sound select dialog
    TSoundSelectDialog ss(this);

    if (ss.exec() == QDialog::Rejected)
        return;

    mSound = ss.getSound();
    mLine->setText(mSound);
    emit soundFileChanged(mSound, mName);
}
