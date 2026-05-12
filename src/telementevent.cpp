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
#include <QToolButton>
#include <QHBoxLayout>

#include "telementevent.h"
#include "teventactionsdialog.h"
#include "tlineeditelide.h"
#include "terror.h"

TElementEvent::TElementEvent(const QString& name, int instance, QWidget *widget)
    : QWidget(widget),
      mName(name),
      mInstance(instance)
{
    DECL_TRACER("TElementEvent::TElementEvent(const QString& name, int instance, QWidget *widget)");

    init();
}

TElementEvent::TElementEvent(ObjHandler::BUTTON_EVENT_t event, const QString& name, int instance, QWidget *widget)
    : QWidget(widget),
      mName(name),
      mInstance(instance),
      mEventType(event)
{
    DECL_TRACER("TElementEvent::TElementEvent(ObjHandler::BUTTON_EVENT_t event, const QString& name, int instance, QWidget *widget)");

    init();
}

void TElementEvent::init()
{
    DECL_TRACER("TElementEvent::init()");

    mLine = new TLineEditElide;
    mLine->setReadOnly(true);
    mLine->setElide(true);

    mButton = new QToolButton;
    mButton->setText("...");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mLine);
    layout->addWidget(mButton);

    connect(mButton, &QToolButton::clicked, this, &TElementEvent::onClicked);
}

void TElementEvent::setFuncs(const QList<ObjHandler::PUSH_FUNC_T>& funcs)
{
    DECL_TRACER("TElementEvent::setFuncs(const QList<ObjHandler::PUSH_FUNC_T>& funcs)");

    mFuncs = funcs;
    setTextLine();
}

void TElementEvent::setTextLine()
{
    DECL_TRACER("TElementEvent::setTextLine()");

    QSignalBlocker sigBlock(mLine);

    if (mFuncs.size() > 0)
    {
        QString text;

        if (mFuncs[0].action == ObjHandler::BT_ACTION_PGFLIP)
        {
            QStringList commands = { "sStan", "sPrev", "sShow", "sHide", "sToggle", "ClearG", "scPage", "scPanel" };
            QStringList list = { "standard page", "previous page", "show popup", "hide popup", "toggle popup", "hide popup group", "hide popup on page", "hide all popups" };
            QString cmdText;
            int pos = 0;

            for (QString cmd : commands)
            {
                if (mFuncs[0].pfType.contains(cmd, Qt::CaseInsensitive))
                {
                    cmdText = list[pos];
                    break;
                }

                pos++;
            }

            text = QString("[%1] %2").arg(cmdText).arg(mFuncs[0].pfName);
        }
        else if (mFuncs[0].action == ObjHandler::BT_ACTION_LAUNCH)
        {
            QStringList commands = { "show", "close", "close_all", "status_show", "status_hide" };
            QStringList list = { "show", "close", "close all", "show status", "hide status" };
            int idx = commands.indexOf(mFuncs[0].pfAction);

            if (idx >= 0)
                text = QString("[launch] %1").arg(list[idx]);
            else
                text = QString("[launch] %1").arg(mFuncs[0].pfAction);
        }
        else if (mFuncs[0].action == ObjHandler::BT_ACTION_CUSTOM)
            text = QString("[custom] Port:%1, ID:%2, Type:%3, Flag:%4, Val1:%5, Val2:%6, Val3:%7")
                   .arg(mFuncs[0].port)
                   .arg(mFuncs[0].ID)
                   .arg(mFuncs[0].type)
                   .arg(mFuncs[0].flag)
                   .arg(mFuncs[0].value1)
                   .arg(mFuncs[0].value2)
                   .arg(mFuncs[0].value3);
        else if (mFuncs[0].action == ObjHandler::BT_ACTION_COMMAND)
            text = "command";
        else
            text = "string";

        if (mFuncs.size() > 1)
            text.append(" +");

        mLine->setText(text);
        mLine->setCursorPosition(0);
    }
    else
        mLine->clear();
}

bool TElementEvent::hasChanged(const QList<ObjHandler::PUSH_FUNC_T>& funcs)
{
    DECL_TRACER("TElementEvent::hasChanged(const QList<ObjHandler::PUSH_FUNC_T>& funcs)");

    if (funcs.size() != mFuncs.size())
        return true;

    QList<ObjHandler::PUSH_FUNC_T>::ConstIterator iter;
    int pos = 0;

    for (iter = funcs.constBegin(); iter != funcs.constEnd(); ++iter)
    {
        if (iter->ID != mFuncs[pos].ID ||
            iter->action != mFuncs[pos].action ||
            iter->pfAction != mFuncs[pos].pfAction ||
            iter->encode != mFuncs[pos].encode ||
            iter->event != mFuncs[pos].event ||
            iter->flag != mFuncs[pos].flag ||
            iter->item != mFuncs[pos].item ||
            iter->key != mFuncs[pos].key ||
            iter->name != mFuncs[pos].name ||
            iter->port != mFuncs[pos].port ||
            iter->pfType != mFuncs[pos].pfType ||
            iter->pfName != mFuncs[pos].pfName ||
            iter->value1 != mFuncs[pos].value1 ||
            iter->value2 != mFuncs[pos].value2 ||
            iter->value3 != mFuncs[pos].value3 ||
            iter->text != mFuncs[pos].text)
            return true;
    }

    return false;
}

void TElementEvent::onClicked()
{
    DECL_TRACER("TElementEvent::onClicked()");

    TEventActionsDialog dlg(this);
    dlg.setFuncs(mFuncs);
    dlg.setEventType(mEventType);

    if (dlg.exec() == QDialog::Rejected)
        return;

    QList<ObjHandler::PUSH_FUNC_T> funcs = dlg.getFuncs();

    if (hasChanged(funcs))
    {
        mFuncs = funcs;
        setTextLine();
        emit eventChanged(mFuncs, mEventType, mName, mInstance);
    }
}
