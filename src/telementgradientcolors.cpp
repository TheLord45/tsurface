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
#include <QToolButton>

#include "telementgradientcolors.h"
#include "tgradientcolordialog.h"
#include "terror.h"

TElementGradientColors::TElementGradientColors(const QList<QColor>& colors, const QString& name, QWidget *parent)
    : QWidget(parent),
      mGradients(colors),
      mName(name)
{
    DECL_TRACER("TElementGradientColors::TElementGradientColors(const QList<QColor>& colors, const QString& name, QWidget *parent)");

    if (!name.isEmpty())
        setObjectName(name);

    if (colors.empty())
    {
        mGradients.append(Qt::gray);
        mGradients.append(Qt::white);
    }
    else if (colors.size() < 2)
    {
        mGradients = colors;
        mGradients.append(Qt::white);
    }
    else
        mGradients = colors;

    setContentsMargins(0, 0, 0, 0);
    createLine();
}

void TElementGradientColors::setGradientColors(const QList<QColor>& colors)
{
    DECL_TRACER("TElementGradientColors::setGradientColors(const QList<QColor>& colors)");

    mGradients = colors;
    QSignalBlocker sigBlocker(this);
    createLine();
}

void TElementGradientColors::onPushButtonClicked()
{
    DECL_TRACER("TElementGradientColors::onPushButtonClicked()");

    TGradientColorDialog dialog(mGradients, this);

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

    bool haveLayout = false;

    if (mLayout)
    {
        haveLayout = true;
        // Delete all widget contained by the layout
        QList<QWidget*> childs = findChildren<QWidget*>(Qt::FindDirectChildrenOnly);
        MSG_DEBUG("Found " << childs.size() << " childs");

        for (QWidget *item : childs)
        {
            if (!item)
                continue;

            QString oName = item->objectName();
            MSG_DEBUG("Found object \"" << oName.toStdString() << "\"");

            if (oName == "PushButton" || oName.startsWith("Color_"))
            {
                MSG_DEBUG("Removing object " << oName.toStdString());

                if (oName == "PushButton")
                    mButton = nullptr;

                item->deleteLater();
                mLayout->removeWidget(item);
                delete item;
            }
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

    mLayout->addStretch();

    if (!mButton)
    {
        mButton = new QToolButton;
        mButton->setObjectName("PushButton");
        mButton->setText("...");
        connect(mButton, &QToolButton::clicked, this, &TElementGradientColors::onPushButtonClicked);
    }

    mLayout->addWidget(mButton);
}
