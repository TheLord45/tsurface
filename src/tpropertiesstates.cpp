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
#include <QTreeWidget>
#include <QBrush>

#include "tpropertiesstates.h"
#include "tconfmain.h"
#include "terror.h"

TPropertiesStates::TPropertiesStates(QTreeWidget *widget)
    : mTreeWidget(widget)
{
    DECL_TRACER("TPropertiesStates::TPropertiesStates(QTreeWidget *widget)");
}

TPropertiesStates::~TPropertiesStates()
{
    DECL_TRACER("TPropertiesStates::~TPropertiesStates()");
}

void TPropertiesStates::setStatesPage(const QString& name)
{
    DECL_TRACER("TPropertiesStates::setStatesPage(const QString& name)");

    if (!mTreeWidget)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTreeWidget\"!");
        return;
    }

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_STATES);

    mChanged = false;
    mPage = TPageHandler::Current().getPage(name);
    setStatesPage(mPage.pageID, true);
}

void TPropertiesStates::setStatesPage(int id, bool loaded)
{
    DECL_TRACER("TPropertiesStates::setStatesPage(int id, bool loaded)");

    if (!mTreeWidget)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTreeWidget\"!")
        return;
    }

    if (!loaded && mPage.pageID > 0 && mChanged)
    {
        saveChangedData(&mPage);
        mPage = Page::PAGE_t();
    }

    mChanged = false;

    if (!loaded)
        mPage = TPageHandler::Current().getPage(id);

    if (mPage.pageID <= 0)
        return;

    createPage();
}

void TPropertiesStates::createPage()
{
    DECL_TRACER("TPropertiesStates::createPage()");

    int setupPort = TConfMain::Current().getSetupPort();
    mInitialized = false;
    QBrush brush;
    brush.setColor(Qt::GlobalColor::lightGray);
    mTreeWidget->clear();

}
