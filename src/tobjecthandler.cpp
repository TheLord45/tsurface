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
#include "tobjecthandler.h"
#include "terror.h"

using namespace ObjHandler;

TObjectHandler::TObjectHandler()
{
    DECL_TRACER("TObjectHandler::TObjectHandler()");
}

TObjectHandler::TObjectHandler(ObjHandler::BUTTONTYPE bt, int num, const QString& name)
{
    DECL_TRACER("TObjectHandler::TObjectHandler(ObjHandler::BUTTONTYPE bt, int num, const QString& name)");

    mObject.type = bt;
    mObject.bi = num;
    mObject.na = name;
}

ObjHandler::SR_T TObjectHandler::getSr(int number)
{
    DECL_TRACER("TObjectHandler::getSr(int number)");

    for (SR_T sr : mObject.sr)
    {
        if (sr.number == number)
            return sr;
    }

    return SR_T();
}

int TObjectHandler::getButtonTypeIndex(BUTTONTYPE bt)
{
    DECL_TRACER("TObjectHandler::getButtonTypeIndex(BUTTONTYPE bt)");

    // This is the index for G5 type panels!
    switch(bt)
    {
        case GENERAL:               return 0;
        case MULTISTATE_GENERAL:    return 1;
        case BARGRAPH:              return 2;
        case MULTISTATE_BARGRAPH:   return 3;
        case TEXT_INPUT:            return 4;
        case SUBPAGE_VIEW:          return 5;
        case LISTVIEW:              return 6;

        default:
            return 0;
    }

    return 0;
}
