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
      TPropertiesGeneral(nullptr),
      TPropertiesStates(nullptr)
{
    DECL_TRACER("TWorkSpaceHandler::TWorkSpaceHandler()");
}

TWorkSpaceHandler::TWorkSpaceHandler(QTreeView *tree, QTableWidget *general, QTableWidget *prog, QTreeWidget *states, QWidget *parent)
    : TPageTree(tree, parent),
      TPropertiesGeneral(general),
      TPropertiesProgramming(prog),
      TPropertiesStates(states)
{
    DECL_TRACER("TWorkSpaceHandler::TWorkSpaceHandler(QTreeView *tree, QTableWidget *general, QTableWidget *prog, QTreeWidget *states, QWidget *parent)");

    TPropertiesGeneral::setParent(parent);
    TPropertiesProgramming::setParent(parent);
}

TWorkSpaceHandler::~TWorkSpaceHandler()
{
    DECL_TRACER("TWorkSpaceHandler::~TWorkSpaceHandler()");
}

TWorkSpaceHandler& TWorkSpaceHandler::Current()
{
    DECL_TRACER("TWorkSpaceHandler::Current()");

    if (!mCurrent)
        mCurrent = new TWorkSpaceHandler(nullptr, nullptr, nullptr, nullptr);

    return *mCurrent;
}

TWorkSpaceHandler& TWorkSpaceHandler::Current(QTreeView *tree, QTableWidget *general, QTableWidget *prog, QTreeWidget *states, QWidget *parent)
{
    DECL_TRACER("TWorkSpaceHandler::Current(QTreeView *tree, QTableWidget *general, QTableWidget *prog, QTreeWidget *states, QWidget *parent)");

    if (!mCurrent)
        mCurrent = new TWorkSpaceHandler(tree, general, prog, states, parent);

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

bool TWorkSpaceHandler::isChanged()
{
    DECL_TRACER("TWorkSpaceHandler::isChanged()");

    if (TPropertiesGeneral::isChanged())
        return true;

    return false;
}

void TWorkSpaceHandler::setPage(const QString& name)
{
    DECL_TRACER("TWorkSpaceHandler::setPage(const QString& name)");

    setGeneralPage(name);
    setProgrammingPage(name);
}

void TWorkSpaceHandler::setPage(int id)
{
    DECL_TRACER("TWorkSpaceHandler::setPage(int id)");

    setGeneralPage(id);
    setProgrammingPage(id);
}

void TWorkSpaceHandler::setPopup(const QString& name)
{
    DECL_TRACER("TWorkSpaceHandler::setPopup(const QString& name)");

    setGeneralPopup(name);
    setProgrammingPopup(name);
}

void TWorkSpaceHandler::setPopup(int id)
{
    DECL_TRACER("TWorkSpaceHandler::setPopup(int id)");

    setGeneralPopup(id);
    setProgrammingPopup(id);
}

void TWorkSpaceHandler::pageNameChanged(int id, const QString& name)
{
    DECL_TRACER("TWorkSpaceHandler::onPageNameChanged(int id, const QString& name)");

    if (id >= 1 && id < 500)
        TPageTree::updatePageName(id, name);
    else if (id >= 500 && id < 1000)
        TPageTree::updatePopupName(id, name);

    TPageHandler::Current().changePageName(id, name);
}

void TWorkSpaceHandler::saveChangedData(Page::PAGE_t *page, PROPERTIES_t prop)
{
    DECL_TRACER("TWorkSpaceHandler::saveChangedData(Page::PAGE_t *page, PROPERTIES_t prop)");

    if (!_dataChanged)
        return;

    Page::PAGE_t pg = TPageHandler::Current().getPage(page->pageID);

    switch(prop)
    {
        case TBL_GENERIC:
            pg.popupType = page->popupType;
            pg.name = page->name;
            pg.description = page->description;
            pg.left = page->left;
            pg.top = page->top;
            pg.width = page->width;
            pg.height = page->height;
            pg.resetPos = page->resetPos;
            pg.group = page->group;
            pg.timeout = page->timeout;
            pg.modal = page->modal;
            pg.showEffect = page->showEffect;
            pg.hideEffect = page->hideEffect;
            pg.collapseDirection = page->collapseDirection;
        break;

        case TBL_PROGRAM:
            pg.ap = page->ap;
            pg.ad = page->ap;
            pg.cp = page->cp;
            pg.ch = page->ch;
        break;

        default:
            return;
    }

    _dataChanged(&pg);
}

void TWorkSpaceHandler::markChanged()
{
    DECL_TRACER("TWorkSpaceHandler::markChanged()");

    if (_markDirty)
        _markDirty();
}
