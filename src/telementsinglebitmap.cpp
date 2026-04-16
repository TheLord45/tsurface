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
#include <QToolButton>

#include "telementsinglebitmap.h"
#include "tbitmapselectdialog.h"
#include "terror.h"

TElementSingleBitmap::TElementSingleBitmap(const QString& name, QWidget *parent)
    : QWidget(parent),
      mName(name)
{
    DECL_TRACER("TElementSingleBitmap::TElementSingleBitmap(QWidget *parent)");

    init();
}

TElementSingleBitmap::TElementSingleBitmap(const QString& bm, const QString& name, QWidget *parent)
    : QWidget(parent),
      mPixmapName(bm),
      mName(name)
{
    DECL_TRACER("TElementSingleBitmap::TElementSingleBitmap(const QPixmap& bm, QWidget *parent)");

    init();
}

TElementSingleBitmap::~TElementSingleBitmap()
{
    DECL_TRACER("TElementSingleBitmap::~TElementSingleBitmap()");
}

void TElementSingleBitmap::init()
{
    DECL_TRACER("TElementSingleBitmap::init()");

    QSignalBlocker sigBlock(this);

    if (!mName.isEmpty() && mName != objectName())
        setObjectName(mName);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    mLine = new QLineEdit;
    mLine->setText(mPixmapName);

    layout->addWidget(mLine, 1);

    mButton = new QToolButton;
    mButton->setText("...");
    layout->addWidget(mButton);

    connect(mLine, &QLineEdit::textEdited, this, &TElementSingleBitmap::onLineEditTextChanged);
    connect(mButton, &QToolButton::clicked, this, &TElementSingleBitmap::onPushButtonClicked);

}

void TElementSingleBitmap::setPixmapName(const QString& bm)
{
    DECL_TRACER("TElementSingleBitmap::setPixmapName(const QString& bm)");

    mPixmapName = bm;
    QSignalBlocker sigBlock(this);

    if (mLine)
        mLine->setText(bm);
}

void TElementSingleBitmap::onLineEditTextChanged(const QString& text)
{
    DECL_TRACER("TElementBitmapSelector::onLineEditTextChanged(const QString& text)");

    mPixmapName = text;

    emit bitmapChanged(mPixmapName, mName);
    emit bitmapChangedInst(mPixmapName, mName, mInstance);
}

void TElementSingleBitmap::onPushButtonClicked()
{
    DECL_TRACER("TElementSingleBitmap::onPushButtonClicked()");

    TBitmapSelectDialog bmDialog(this);
    bmDialog.setTemporaryPath(TConfMain::Current().getPathTemporary());
    bmDialog.setSingleSelect(true);

    if (!mPixmapName.isEmpty())
        bmDialog.setDefaultBitmap(mPixmapName);

    if (bmDialog.exec() == QDialog::Rejected)
        return;

    QList<ObjHandler::BITMAPS_t> bm = bmDialog.getSelected();

    if (bm.empty())
        return;

    mPixmapName = bm[0].fileName;
    mLine->setText(mPixmapName);

    emit bitmapChanged(mPixmapName, mName);
    emit bitmapChangedInst(mPixmapName, mName, mInstance);
}
