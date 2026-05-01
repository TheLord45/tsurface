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
#ifndef TELEMENTEVENT_H
#define TELEMENTEVENT_H

#include <QWidget>

#include "tobjecthandler.h"

class QLineEdit;
class QToolButton;

class TElementEvent : public QWidget
{
    Q_OBJECT

    public:
        TElementEvent(const QString& name, int instance, QWidget *widget = nullptr);
        TElementEvent(ObjHandler::BUTTON_EVENT_t event, const QString& name, int instance, QWidget *widget = nullptr);

        void setFuncs(const QList<ObjHandler::PUSH_FUNC_T>& funcs) { mFuncs = funcs; }
        void setEventType(ObjHandler::BUTTON_EVENT_t event) { mEventType = event; }
        ObjHandler::BUTTON_EVENT_t getEventType() { return mEventType; }

    signals:
        void eventChanged(const QList<ObjHandler::PUSH_FUNC_T>& funcs, const QString& name, int instance);

    protected:
        void onClicked();

    private:
        void init();

        QLineEdit *mLine;
        QToolButton *mButton;
        QString mName;
        int mInstance{-1};
        ObjHandler::BUTTON_EVENT_t mEventType{ObjHandler::EVENT_NONE};
        QList<ObjHandler::PUSH_FUNC_T> mFuncs;
};

#endif // TELEMENTEVENT_H
