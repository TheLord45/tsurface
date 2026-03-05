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

/**
 * @brief TObjectHandler::getObjectCommon
 * The method takes the first element in the @bold sr array and compares
 * every value with the other entries in the array. Every value contained
 * in the other instances too is copied into a virtual object.
 * Values which are different are left blank or on default value.
 *
 * @return A virtual object containing only the common values of all entries
 * in the array @bold sr.
 */
ObjHandler::TOBJECT_t TObjectHandler::getObjectCommon()
{
    DECL_TRACER("TObjectHandler::getObjectCommon()");

    TOBJECT_t obj = mObject;

    for (int i = 1; i < mObject.sr.size(); ++i)
    {
        if (obj.sr[0]._do != mObject.sr[i]._do)
            obj.sr[0]._do.clear();

        if (obj.sr[0].bs != mObject.sr[i].bs)
            obj.sr[0].bs.clear();

        // TODO: Add code for all other values
    }

    return obj;
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
