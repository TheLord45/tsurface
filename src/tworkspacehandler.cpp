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
#include "tworkspacehandler.h"
#include "terror.h"

TWorkSpaceHandler *TWorkSpaceHandler::mCurrent{nullptr};

TWorkSpaceHandler::TWorkSpaceHandler()
    : TPageTree(nullptr, nullptr),
    TPropertiesGeneral(nullptr)
{
    DECL_TRACER("TWorkSpaceHandler::TWorkSpaceHandler()");
}

TWorkSpaceHandler::TWorkSpaceHandler(QTreeView *tree, QTableWidget *general, QWidget *parent)
    : TPageTree(tree, parent),
      TPropertiesGeneral(general)
{
    DECL_TRACER("TWorkSpaceHandler::TWorkSpaceHandler(QTreeView *tree, QTableWidget *general, QWidget *parent)");

    TPropertiesGeneral::setParent(parent);
}

TWorkSpaceHandler::~TWorkSpaceHandler()
{
    DECL_TRACER("TWorkSpaceHandler::~TWorkSpaceHandler()");
}

TWorkSpaceHandler& TWorkSpaceHandler::Current()
{
    DECL_TRACER("TWorkSpaceHandler::Current()");

    if (!mCurrent)
        mCurrent = new TWorkSpaceHandler(nullptr, nullptr);

    return *mCurrent;
}

TWorkSpaceHandler& TWorkSpaceHandler::Current(QTreeView *tree, QTableWidget *general, QWidget *parent)
{
    DECL_TRACER("TWorkSpaceHandler::Current(QTreeView *tree, QTableWidget *general, QWidget *parent)");

    if (!mCurrent)
        mCurrent = new TWorkSpaceHandler(tree, general, parent);

    return *mCurrent;
}

void TWorkSpaceHandler::setParent(QWidget *widget)
{
    DECL_TRACER("TWorkSpaceHandler::setParent(QWidget *widget)");

    mParent = widget;
    TPageTree::setParent(widget);
    TPropertiesGeneral::setParent(widget);
}

void TWorkSpaceHandler::setWorkspacePagesWidget(QTreeView *tree)
{
    DECL_TRACER("TWorkSpaceHandler::setWorkspacePagesWidget(QTreeView *tree)");

    TPageTree::setTreeView(tree);
}

void TWorkSpaceHandler::setPropertiesGeneralWidget(QTableWidget *widget)
{
    DECL_TRACER("TWorkSpaceHandler::setPropertiesGeneralWidget(QTableWidget *widget)");

    TPropertiesGeneral::setWidget(widget);
}
