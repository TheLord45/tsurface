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
#include <QPushButton>
#include <QHBoxLayout>

#include "telementbitmapselector.h"
#include "tbitmapdialog.h"
#include "tbitmapselectdialog.h"
#include "terror.h"

TElementBitmapSelector::TElementBitmapSelector(QWidget *parent)
    : QWidget(parent)
{
    DECL_TRACER("TElementBitmapSelector::TElementBitmapSelector(QWidget *parent)");
}

TElementBitmapSelector::TElementBitmapSelector(const QString& name, const ObjHandler::BITMAPS_t& bm, QWidget *parent)
    : QWidget(parent),
      mName(name)
{
    DECL_TRACER("TElementBitmapSelector::TElementBitmapSelector(const QString& name, const QList<ObjHandler::BITMAPS_t>& bm, QWidget *parent)");

    mBitmaps.insert(0, bm);
    mSingleBitmap = true;
    init();
}

TElementBitmapSelector::TElementBitmapSelector(const QString& name, const QList<ObjHandler::BITMAPS_t>& bm, QWidget *parent)
    : QWidget(parent),
      mName(name),
      mBitmaps(bm)
{
    DECL_TRACER("TElementBitmapSelector::TElementBitmapSelector(const QString& name, const QList<ObjHandler::BITMAPS_t>& bm, QWidget *parent)");

    mSingleBitmap = false;
    init();
}

TElementBitmapSelector::~TElementBitmapSelector()
{
    DECL_TRACER("TElementBitmapSelector::~TElementBitmapSelector()");
}

void TElementBitmapSelector::start()
{
    DECL_TRACER("TElementBitmapSelector::start()");

    if (mName.isEmpty() && !objectName().isEmpty())
        mName = objectName();

    init();
}

void TElementBitmapSelector::init()
{
    DECL_TRACER("TElementBitmapSelector::init()");

    if (!mName.isEmpty() && mName != objectName())
        setObjectName(mName);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    mLine = new QLineEdit;

    if (!mBitmaps.empty())
        mLine->setText(mBitmaps[0].fileName);

    layout->addWidget(mLine, 1);

    mButton = new QPushButton;
    mButton->setText("...");
    mButton->setMaximumWidth(30);
    layout->addWidget(mButton);

    connect(mLine, &QLineEdit::textChanged, this, &TElementBitmapSelector::onLineEditTextChanged);
    connect(mButton, &QPushButton::clicked, this, &TElementBitmapSelector::onPushButtonClicked);
}

void TElementBitmapSelector::onLineEditTextChanged(const QString& text)
{
    DECL_TRACER("TElementBitmapSelector::onLineEditTextChanged(const QString& text)");

    if (mSingleBitmap)
    {
        ObjHandler::BITMAPS_t bm;
        bm.fileName = text;
        bm.dynamic = false;
        mBitmaps.clear();
        mBitmaps.append(bm);
    }
    else if (!mBitmaps.empty())
    {
        mBitmaps[0].fileName = text;
        mBitmaps[0].dynamic = false;
    }
    else
    {
        ObjHandler::BITMAPS_t bm;
        bm.fileName = text;
        bm.dynamic = false;
        mBitmaps.append(bm);
    }

    emit bitmapsChanged(mBitmaps, mName);
}

void TElementBitmapSelector::onPushButtonClicked()
{
    DECL_TRACER("TElementBitmapSelector::onPushButtonClicked()");

    if (mSingleBitmap)
    {
        TBitmapSelectDialog bmDialog(this);
        bmDialog.setTemporaryPath(TConfMain::Current().getPathTemporary());
        bmDialog.setSingleSelect(mSingleBitmap);

        if (bmDialog.exec() == QDialog::Rejected)
            return;

        QList<ObjHandler::BITMAPS_t> bm = bmDialog.getSelected();

        if (bm.empty())
            return;

        mLine->setText(bm[0].fileName);
        mBitmaps.clear();
        mBitmaps.append(bm[0]);
    }
    else
    {
        TBitmapDialog bmDialog(this);
        bmDialog.setBitmaps(mBitmaps);

        if (bmDialog.exec() == QDialog::Rejected)
            return;

        mBitmaps = bmDialog.getBitmaps();

        if (!mBitmaps.empty())
        {
            if (mBitmaps[0].dynamic)
                mLine->setText(mBitmaps[0].fileName + " (dynamic)");
            else
                mLine->setText(mBitmaps[0].fileName);
        }
        else
            mLine->clear();
    }

    emit bitmapsChanged(mBitmaps, mName);
}
