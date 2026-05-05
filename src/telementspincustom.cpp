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
#include <QPushButton>
#include <QHBoxLayout>

#include "telementspincustom.h"
#include "tcustomactiondialog.h"
#include "terror.h"

TElementSpinCustom::TElementSpinCustom(const ObjHandler::PUSH_FUNC_T& pf, const QString& name, QWidget *parent)
    : QWidget(parent),
      mFunc(pf),
      mName(name)
{
    DECL_TRACER("TElementSpinCustom::TElementSpinCustom(const ObjHandler::PUSH_FUNC_T& pf, const QString& name, QWidget *parent)");

    setContentsMargins(0, 0, 0, 0);

    mSpinBox = new QSpinBox;
    mSpinBox->setRange(1, 999999);
    mSpinBox->setValue(pf.port);

    mButton = new QPushButton;
    mButton->setText(tr("Edit Custom ..."));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mSpinBox);
    layout->addWidget(mButton);

    connect(mSpinBox, &QSpinBox::valueChanged, this, &TElementSpinCustom::onValueChanged);
    connect(mButton, &QPushButton::clicked, this, &TElementSpinCustom::onButtonClicked);
}

void TElementSpinCustom::setFunc(const ObjHandler::PUSH_FUNC_T& pf)
{
    DECL_TRACER("TElementSpinCustom::setFunc(const ObjHandler::PUSH_FUNC_T& pf)");

    mFunc = pf;
    QSignalBlocker sigBlock(mSpinBox);
    mSpinBox->setValue(pf.port);
}

void TElementSpinCustom::onValueChanged(int value)
{
    DECL_TRACER("TElementSpinCustom::onValueChanged(int value)");

    mFunc.port = value;
    emit contentChanged(mFunc, mName);
}

void TElementSpinCustom::onButtonClicked()
{
    DECL_TRACER("TElementSpinCustom::onButtonClicked()");

    TCustomActionDialog *dlg = new TCustomActionDialog(this);
    dlg->setFunc(mFunc);

    if (dlg->exec() == QDialog::Rejected)
        return;

    mFunc = dlg->getFunc();
    emit contentChanged(mFunc, mName);
}
