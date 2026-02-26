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
#include "tcanvaswidget.h"
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
    TPropertiesStates::setParent(parent);
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
    TPropertiesProgramming::setParent(widget);
    TPropertiesStates::setParent(widget);
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

    if (TPropertiesGeneral::isChanged() || TPropertiesProgramming::isChanged() || TPropertiesStates::isChanged())
        return true;

    return false;
}

void TWorkSpaceHandler::setObjectGeometry(int pageID, int bi, const QRect& geom)
{
    DECL_TRACER("TWorkSpaceHandler::setObjectGeometry(int bi, const QRect& geom)");

    Q_UNUSED(pageID);

    TPropertiesStates::setGeometry(bi, geom);
    TPropertiesGeneral::update();
}

void TWorkSpaceHandler::setActualObject(TObjectHandler *object, int index)
{
    DECL_TRACER("TWorkSpaceHandler::setActualObject(TObjectHandler *object, int index)");

    mObject = object;
    mObjectIndex = index;

    if (!mObject)
        return;

    TPropertiesGeneral::update();
}

void TWorkSpaceHandler::clear()
{
    DECL_TRACER("TWorkSpaceHandler::clear()");

    TPropertiesGeneral::clear();
    TPropertiesProgramming::clear();
    TPropertiesStates::clear();
}

void TWorkSpaceHandler::setPage(const QString& name)
{
    DECL_TRACER("TWorkSpaceHandler::setPage(const QString& name)");

    Page::PAGE_t page = TPageHandler::Current().getPage(name);
    setPage(page.pageID, false, page);
}

void TWorkSpaceHandler::setPage(int id, bool load, const Page::PAGE_t& rpage)
{
    DECL_TRACER("TWorkSpaceHandler::setPage(int id, bool load, const Page::PAGE_t& rpage)");

    Page::PAGE_t page = rpage;

    if (load)
        page = *TPageHandler::Current().getPage(id);

    setGeneralPage(page, STATE_PAGE);
    TPropertiesProgramming::setPage(page);
    TPropertiesStates::setPage(page);
}

void TWorkSpaceHandler::setPopup(const QString& name)
{
    DECL_TRACER("TWorkSpaceHandler::setPopup(const QString& name)");

    Page::PAGE_t page = TPageHandler::Current().getPage(name);
    setPopup(page.pageID, false, page);
}

void TWorkSpaceHandler::setPopup(int id, bool load, const Page::PAGE_t& rpage)
{
    DECL_TRACER("TWorkSpaceHandler::setPopup(int id, bool load, const Page::PAGE_t& rpage)");

    Page::PAGE_t page = rpage;

    if (load)
        page = *TPageHandler::Current().getPage(id);

    setGeneralPage(page, STATE_POPUP);
    TPropertiesProgramming::setPage(page);
    TPropertiesStates::setPage(page);
}

void TWorkSpaceHandler::setAllProperties(Page::PAGE_t& page, STATE_TYPE stype, int objectID)
{
    DECL_TRACER("TWorkSpaceHandler::setAllProperties(Page::PAGE_t& page, STATE_TYPE stype, int objectID)");

    if (page.pageID <= 0)
        return;

    setGeneralPage(page, stype, objectID);

    if (stype == STATE_PAGE)
        setProgrammingPage(page.pageID, false);
    else if (stype == STATE_POPUP)
        setProgrammingPopup(page.pageID, false);

    setStatesPage(page.pageID, false);
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

    Page::PAGE_t *pg = TPageHandler::Current().getPage(page->pageID);

    switch(prop)
    {
        case TBL_GENERAL:
        {
            pg->popupType = page->popupType;
            pg->name = page->name;
            pg->description = page->description;
            pg->left = page->left;
            pg->top = page->top;
            pg->width = page->width;
            pg->height = page->height;
            pg->resetPos = page->resetPos;
            pg->group = page->group;
            pg->timeout = page->timeout;
            pg->modal = page->modal;
            pg->showEffect = page->showEffect;
            pg->hideEffect = page->hideEffect;
            pg->collapseDirection = page->collapseDirection;

            int index = TPropertiesGeneral::getActObjectIndex();
            ObjHandler::TOBJECT_t obj = TPropertiesGeneral::getActObject();

            if (index >= 0 && index < pg->objects.size())
                pg->objects[index]->setObject(obj);
        }
        break;

        case TBL_PROGRAM:
            pg->ap = page->ap;
            pg->ad = page->ap;
            pg->cp = page->cp;
            pg->ch = page->ch;
        break;

        case TBL_STATES:
            pg->srPage = page->srPage;
            pg->objects = page->objects;
        break;

        default:
            return;
    }

    _markDirty();
//    _dataChanged(pg);
}

void TWorkSpaceHandler::markChanged()
{
    DECL_TRACER("TWorkSpaceHandler::markChanged()");

    if (_markDirty)
        _markDirty();
}

ObjHandler::TOBJECT_t TWorkSpaceHandler::getActualObject(const Page::PAGE_t& page)
{
    DECL_TRACER("TWorkSpaceHandler::getActualObject(const Page::PAGE_t& page)");

    if (mObject && mObject->getObjectWidget() == page.baseObject.widget)
        return mObject->getObject();

    if (!page.baseObject.widget)
        return ObjHandler::TOBJECT_t();

    TResizableWidget *widget = page.baseObject.widget->currentSelectedWidget();

    if (!widget || page.pageID != widget->getPageId())
        return ObjHandler::TOBJECT_t();

    int bi = widget->getId();

    for (TObjectHandler *obj : page.objects)
    {
        if (obj->getButtonIndex() == bi)
            return obj->getObject();
    }

    return ObjHandler::TOBJECT_t();
}

Page::PAGE_t TWorkSpaceHandler::getCurrentPage()
{
    DECL_TRACER("TWorkSpaceHandler::getCurrentPage()");

    return TPageHandler::Current().getCurrentPage(nullptr);
}

void TWorkSpaceHandler::requestRedraw(Page::PAGE_t *page)
{
    DECL_TRACER("TWorkSpaceHandler::requestRedraw(Page::PAGE_t *page)");

    if (_requestDraw)
        _requestDraw(page);
}
