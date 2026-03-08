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
#include "tgradientcolordialog.h"
#include "terror.h"

TElementGradientColors::TElementGradientColors(const QList<QColor>& colors, const QString& name, QWidget *parent)
    : QWidget(parent),
      mGradients(colors),
      mName(name),
      mParent(parent)
{
    DECL_TRACER("TElementGradientColors::TElementGradientColors(const QList<QColor>& colors, const QString& name, QWidget *parent)");

    if (!name.isEmpty())
        setObjectName(name);

    if (colors.empty())
    {
        mGradients.append(Qt::gray);
        mGradients.append(Qt::white);
    }

    setContentsMargins(0, 0, 0, 0);
    createLine();
}

void TElementGradientColors::setGradientColors(const QList<QColor>& colors)
{
    DECL_TRACER("TElementGradientColors::setGradientColors(const QList<QColor>& colors)");

    mGradients = colors;
    createLine();
}

void TElementGradientColors::onPushButtonClicked()
{
    DECL_TRACER("TElementGradientColors::onPushButtonClicked()");

    TGradientColorDialog dialog(mGradients, mParent);

    if (dialog.exec() == QDialog::Rejected)
        return;

    mGradients = dialog.getColors();
    // Insert all labels again.
    createLine();
    emit gradientColorChanged(mGradients, mName);
    emit gradientColorChangedInst(mGradients, mName, mInstance);
}

void TElementGradientColors::createLine()
{
    DECL_TRACER("TElementGradientColors::createLine()");

    QSignalBlocker sigBlock(this);
    bool haveLayout = false;

    if (mLayout)
    {
        haveLayout = true;
        // Delete all widget contained by the layout
        QList<QWidget*> childs = findChildren<QWidget*>();
        MSG_DEBUG("Found " << childs.size() << " childs");

        for (QWidget *item : childs)
        {
            MSG_DEBUG("Try to remove object \"" << item->objectName().toStdString() << "\"");

            mLayout->removeWidget(item);
            delete item;
        }
    }
    else
        mLayout = new QHBoxLayout(this);

    if (!haveLayout)
    {
        mLayout->setObjectName("Layout");
        mLayout->setContentsMargins(0, 0, 0, 0);
    }

    int itemNumber = 0;

    for (QColor color : mGradients)
    {
        QLabel *item = new QLabel;
        item->setMinimumWidth(35);
        item->setObjectName(QString("Color_%1_%2").arg(itemNumber).arg(color.name(QColor::HexArgb)));
        item->setFrameShape(QFrame::Box);
        item->setMargin(0);
        item->setStyleSheet(QString("background-color: %1").arg(color.name(QColor::HexArgb)));
        mLayout->addWidget(item);
        itemNumber++;
    }

    if (!haveLayout)
    {
        mLayout->addStretch();
        mButton = new QPushButton("...");
        mButton->setFixedWidth(30);
        mLayout->addWidget(mButton);
    }

    if (!mConnected)
    {
        connect(mButton, &QPushButton::clicked, this, &TElementGradientColors::onPushButtonClicked);
        mConnected = true;
    }
}
