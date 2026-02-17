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
#include <QColorDialog>

#include "telementcolorselector.h"
#include "terror.h"

TElementColorSelector::TElementColorSelector(QWidget *parent)
    : QWidget(parent)
{
    DECL_TRACER("TElementColorSelector::TElementColorSelector(QWidget *parent)");

    mColor = Qt::white;
    init();
}

TElementColorSelector::TElementColorSelector(const QString& name, QWidget *parent)
    : QWidget(parent),
      mName(name)
{
    DECL_TRACER("TElementColorSelector::TElementColorSelector(const QString& name, QWidget *parent)");

    mColor = Qt::white;
    init();
}

TElementColorSelector::TElementColorSelector(const QColor& color, const QString& name, bool noLayout, QWidget *parent)
    : QWidget(parent),
      mName(name),
      mColor(color)
{
    DECL_TRACER("TElementColorSelector::TElementColorSelector(const QColor& color, const QString& name, bool noLayout, QWidget *parent)");

    mNoLayout = noLayout;
    init();
}

TElementColorSelector::~TElementColorSelector()
{
    DECL_TRACER("TElementColorSelector::~TElementColorSelector()");
}

void TElementColorSelector::init()
{
    DECL_TRACER("TElementColorSelector::init()");

    QHBoxLayout *layout = nullptr;

    if (mNoLayout && mParent)
    {
        QHBoxLayout *lay = static_cast<QHBoxLayout *>(mParent->layout());

        if (!lay)
            layout = new QHBoxLayout(this);
    }
    else
        layout = new QHBoxLayout(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    mLabel = new QLabel;
    mLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mLabel->setMinimumWidth(50);
    mLabel->setStyleSheet(QString("background-color: %1").arg(mColor.name(QColor::HexArgb)));
    layout->addWidget(mLabel);

    QToolButton *button = new QToolButton;
    button->setText("...");
    layout->addWidget(button);

    connect(button, &QToolButton::clicked, this, &TElementColorSelector::onButtonClicked);
}

void TElementColorSelector::setColor(const QColor& color)
{
    DECL_TRACER("TElementColorSelector::setColor(const QColor& color)");

    mColor = color;
    mLabel->setStyleSheet(QString("background-color: %1").arg(mColor.name(QColor::HexArgb)));
}

void TElementColorSelector::onButtonClicked()
{
    DECL_TRACER("TElementColorSelector::onButtonClicked()");

    QColorDialog cdialog(mColor, this);

    if (cdialog.exec() == QDialog::Rejected)
        return;

    QColor col = cdialog.selectedColor();

    if (col != mColor)
    {
        mColor = col;
        mLabel->setStyleSheet(QString("background-color: %1").arg(mColor.name(QColor::HexArgb)));
        emit colorChanged(col, mName);
    }
}
