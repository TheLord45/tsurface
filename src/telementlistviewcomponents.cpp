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
#include <QVBoxLayout>

#include "telementlistviewcomponents.h"
#include "tlistviewcomponentsdialog.h"

#include "terror.h"

TElementListviewComponents::TElementListviewComponents(int setting, const QString& name, QWidget *widget)
    : QWidget(widget),
      mName(name)
{
    DECL_TRACER("TElementListviewComponents::TElementListviewComponents(int setting, const QString& name, QWidget *widget=nullptr)");

    mLine = new QLineEdit;
    mLine->setReadOnly(true);

    mButton = new QToolButton;
    mButton->setText("...");
    connect(mButton, &QToolButton::clicked, this, &TElementListviewComponents::onButtonClicked);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    layout->addWidget(mLine);
    layout->addWidget(mButton);

    setProperty(setting);
}

void TElementListviewComponents::setProperty(int setting)
{
    DECL_TRACER("TElementListviewComponents::setProperty(int setting)");

    if (setting < 0 ||
        (!(setting & LISTVIEW_IMAGE_CELL) &&
         !(setting & LISTVIEW_TEXT_PRIMARY) &&
         !(setting & LISTVIEW_TEXT_SECONDARY)))
        return;

    mSetting = setting;
    QSignalBlocker sBlock(this);
    mLine->setText(makeText());
}

void TElementListviewComponents::onButtonClicked()
{
    DECL_TRACER("TElementListviewComponents::onButtonClicked()");

    TListViewComponentsDialog lvcd(this);
    lvcd.setImage(mSetting & LISTVIEW_IMAGE_CELL);
    lvcd.setPrimaryText(mSetting & LISTVIEW_TEXT_PRIMARY);
    lvcd.setSecondaryText(mSetting & LISTVIEW_TEXT_SECONDARY);

    if (lvcd.exec() == QDialog::Rejected)
        return;

    int setting = 0;

    if (lvcd.image())
        setting |= LISTVIEW_IMAGE_CELL;

    if (lvcd.primaryText())
        setting |= LISTVIEW_TEXT_PRIMARY;

    if (lvcd.secondaryText())
        setting |= LISTVIEW_TEXT_SECONDARY;

    if (setting > 0)
    {
        setProperty(setting);
        emit componentChanged(setting, mName);
    }
}

QString TElementListviewComponents::makeText()
{
    DECL_TRACER("TElementListviewComponents::makeText()");

    //                         0                    1                      2                 3
    QStringList msgs = { tr("image only"), tr("single-line text"), tr("two-line text"), " w/ image" };
    QString msg;

    if (!mSetting)
        return QString();

    if (mSetting & LISTVIEW_TEXT_PRIMARY && !(mSetting & LISTVIEW_TEXT_SECONDARY))
        msg = msgs[1];
    else if (mSetting & LISTVIEW_TEXT_PRIMARY && mSetting & LISTVIEW_TEXT_SECONDARY)
        msg = msgs[2];

    if (!msg.isEmpty() && mSetting & LISTVIEW_IMAGE_CELL)
        msg += msgs[3];
    else if (msg.isEmpty() && mSetting & LISTVIEW_IMAGE_CELL)
        msg += msgs[0];

    return msg;
}
