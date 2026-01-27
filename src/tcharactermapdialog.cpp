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
#include <QCheckBox>
#include <QFontComboBox>
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QClipboard>

#include "tcharactermapdialog.h"
#include "tcharacterwidget.h"
#include "terror.h"

TCharacterMapDialog::TCharacterMapDialog(QWidget *parent)
    : QDialog(parent)
{
    DECL_TRACER("TCharacterMapDialog::TCharacterMapDialog(QWidget *parent)");

    mFont = font();
    mFont.setPointSize(12);
    init();
}

TCharacterMapDialog::TCharacterMapDialog(const QFont& font, QWidget *parent)
    : QDialog(parent)
{
    DECL_TRACER("TCharacterMapDialog::TCharacterMapDialog(const QFont& font, QWidget *parent)");

    mFont = font;
    init();
}

void TCharacterMapDialog::init()
{
    DECL_TRACER("TCharacterMapDialog::init()");

    QLabel *fontLabel = new QLabel(tr("Font:"));
    mLineEditFont = new QLineEdit;
    mLineEditFont->setReadOnly(true);
    mLineEditFont->setText(mFont.family());

    QLabel *sizeLabel = new QLabel(tr("Size:"));
    mLineEditFontSize = new QLineEdit;
    mLineEditFontSize->setReadOnly(true);
    mLineEditFontSize->setText(QString("%1pt").arg(mFont.pointSize()));

    mScrollArea = new QScrollArea;
    mCharacterWidget = new TCharacterWidget;
    mScrollArea->setWidget(mCharacterWidget);

    mLineEdit = new QLineEdit;
    QPushButton *clipboardButton = new QPushButton(tr("&To clipboard"));
    QPushButton *buttonOk = new QPushButton(tr("Ok"));
    QPushButton *buttonCancel = new QPushButton(tr("Cancel"));

    connect(mCharacterWidget, &TCharacterWidget::characterSelected, this, &TCharacterMapDialog::insertCharacter);
    connect(clipboardButton, &QPushButton::clicked, this, &TCharacterMapDialog::updateClipboard);

    connect(buttonOk, &QPushButton::clicked, [this]() { done(QDialog::Accepted); });
    connect(buttonCancel, &QPushButton::clicked, [this]() { done(QDialog::Rejected); });

    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(fontLabel);
    controlsLayout->addWidget(mLineEditFont, 1);
    controlsLayout->addWidget(sizeLabel);
    controlsLayout->addWidget(mLineEditFontSize, 1);
    controlsLayout->addStretch(1);

    QHBoxLayout *lineLayout = new QHBoxLayout;
    lineLayout->addWidget(mLineEdit, 1);
    lineLayout->addSpacing(12);
    lineLayout->addWidget(clipboardButton);
    lineLayout->addSpacing(12);
    lineLayout->addWidget(buttonOk);
    lineLayout->addWidget(buttonCancel);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addLayout(controlsLayout);
    centralLayout->addWidget(mScrollArea, 1);
    centralLayout->addSpacing(4);
    centralLayout->addLayout(lineLayout);
    setLayout(centralLayout);

    setWindowTitle(tr("Character Map"));
}

TCharacterMapDialog::~TCharacterMapDialog()
{
    DECL_TRACER("TCharacterMapDialog::~TCharacterMapDialog()");

    delete mCharacterWidget;
}

void TCharacterMapDialog::setFont(const QFont& font)
{
    DECL_TRACER("TCharacterMapDialog::setFont(const QFont& font)");

    mFont = font;

    if (mLineEditFont)
    {
        mLineEditFont->setFont(font);
        mLineEditFont->setText(font.family());
    }

    if (mLineEditFontSize)
        mLineEditFontSize->setText(QString("%1pt").arg(font.pointSize()));

    if (mCharacterWidget)
        mCharacterWidget->updateFont(font);
}

void TCharacterMapDialog::insertCharacter(const QString &character)
{
    DECL_TRACER("TCharacterMapDialog::insertCharacter(const QString &character)");

    mLineEdit->insert(character);
    mActualCharacter = mLineEdit->text();
}

void TCharacterMapDialog::updateClipboard()
{
    mClipboard->setText(mLineEdit->text(), QClipboard::Clipboard);
    mClipboard->setText(mLineEdit->text(), QClipboard::Selection);
}
