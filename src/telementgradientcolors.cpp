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
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

#include "telementgradientcolors.h"
#include "terror.h"

TElementGradientColors::TElementGradientColors(const QList<QColor>& colors, const QString& name, QWidget *parent)
    : QObject(parent),
      mGradients(colors),
      mName(name),
      mParent(parent)
{
    DECL_TRACER("TElementGradientColors::TElementGradientColors(const QList<QColor>& colors, const QString& name, QWidget *parent)");

    if (!name.isEmpty())
        setObjectName(name);

    mBase = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(mBase);
    int itemNumber = 0;

    for (QColor color : colors)
    {
        QLabel *item = new QLabel;
        item->setMinimumWidth(35);
        item->setObjectName(QString("Color_%1_%2").arg(itemNumber).arg(color.name(QColor::HexArgb)));
        item->setStyleSheet(QString("background-color: %1").arg(color.name(QColor::HexArgb)));
        layout->addWidget(item);
    }

    layout->addStretch();
    QPushButton *button = new QPushButton("...");
    button->setFixedWidth(30);
    layout->addWidget(button);

    connect(button, &QPushButton::clicked, this, &TElementGradientColors::onPushButtonClicked);
}

void TElementGradientColors::onPushButtonClicked()
{
    DECL_TRACER("TElementGradientColors::onPushButtonClicked()");
}
