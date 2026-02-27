/*
 * Copyright (C) 2025, 2026 by Andreas Theofilu <andreas@theosys.at>
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
#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QMdiArea>
#include <QMdiSubWindow>

#include "tpagehandler.h"
#include "tcanvaswidget.h"
#include "tconfmain.h"
#include "tmisc.h"
#include "terror.h"

using namespace Page;
using std::vector;

TPageHandler* TPageHandler::mCurrent{nullptr};

TPageHandler::TPageHandler()
{
    DECL_TRACER("TPageHandler::TPageHandler(QObject *parent)");
}

TPageHandler& TPageHandler::Current()
{
    DECL_TRACER("TPageHandler::Current()");

    if (!mCurrent)
        mCurrent = new TPageHandler;

    return *mCurrent;
}

/**
 * @brief TPageHandler::createPage
 * This method creates a new page or popup and initializes all internal structures
 * to contain the correct information. If there exists already a page or popup
 * with the \b name, the contents of the page or popup will be updated with the
 * given parameters.
 * On success the method returns the new page or popup number.
 *
 * @param w         A pointer to a widget in the MDI area
 * @param pt        The type of the page/popup to create
 * @param name      The name of the page/popup to create. This must be a unique name.
 * @param geom      The geometry of a popup. If this is a page, the left and top
 * dimensions are ignored.
 *
 * @return  On success the new page or popup number is returned. On error it returns 0.
 */
int TPageHandler::createPage(TCanvasWidget *w, Page::PAGE_TYPE pt, const QString& name, const QRect& geom, Page::PAGE_t *rpage)
{
    DECL_TRACER("TPageHandler::createPage(QWidget *w, Page::PAGE_TYPE pt, const QString& name, const QRect& geom, Page::PAGE_t *rpage)");

    return createPage(w, pt, name, geom.width(), geom.height(), geom.left(), geom.top(), rpage);
}

/**
 * @brief TPageHandler::createPage
 * This method creates a new page or popup and initializes all internal structures
 * to contain the correct information. If there exists already a page or popup
 * with the \b name, the contents of the page or popup will be updated with the
 * given parameters.
 * On success the method returns the new page or popup number.
 *
 * @param w         A pointer to a widget in the MDI area
 * @param pt        The type of the page/popup to create
 * @param name      The name of the page/popup to create. This must be a unique name.
 * @param width     The width of the page/popup
 * @param height    The height of the page/popup
 * @param left      If it is a popup, this is the left number of pixels (x).
 * @param top       If it is a popup, this is the top number of pixels (y).
 *
 * @return  On success the new page or popup number is returned. On error it returns 0.
 */
int TPageHandler::createPage(TCanvasWidget *w, Page::PAGE_TYPE pt, const QString& name, int width, int height, int left, int top, Page::PAGE_t *rpage)
{
    DECL_TRACER("TPageHandler::createPage(QWidget *w, Page::PAGE_TYPE pt, const QString& name, int width, int height, int left, int top, Page::PAGE_t *rpage=nullptr)");

    PAGE_t page;

    if (pt == PT_PAGE)
        page.pageID = getNextPageNumber();
    else if (pt == PT_POPUP)
        page.pageID = getNextPopupNumber();
    else
        return 0;

    page.popupType = pt;
    page.name = name;
    page.width = width;
    page.height = height;

    if (pt == PT_POPUP)
    {
        page.left = left;
        page.height = height;
    }

    page.baseObject.widget = w;

    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->name == name)
        {
            iter->popupType = pt;
            iter->name = name;
            iter->width = width;
            iter->height = height;

            if (pt == PT_POPUP)
            {
                iter->left = left;
                iter->top = top;
            }

            if (iter->baseObject.widget && iter->baseObject.widget != w)
                iter->baseObject.widget = w;

            return iter->pageID;
        }
    }

    mPages.append(page);

    if (rpage)
        *rpage = page;

    return page.pageID;
}

void TPageHandler::reset()
{
    DECL_TRACER("TPageHandler::reset()");

    mPages.clear();
    mPathTemporary.clear();
    mMaxPageNumber = 0;
    mMaxPopupNumber = 500;
}

void TPageHandler::setVisible(int number, bool visible)
{
    DECL_TRACER("TPageHandler::setVisible(int number, bool visible)");

    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->pageID == number)
        {
            iter->visible = visible;
            break;
        }
    }
}

bool TPageHandler::isVisible(int number)
{
    DECL_TRACER("TPageHandler::isVisible(int number)");

    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->pageID == number)
            return iter->visible;
    }

    return false;
}

bool TPageHandler::isGridVisible(int number)
{
    DECL_TRACER("TPageHandler::isGridVisible(int number)");

    Page::PAGE_t *page = getPagePointer(number);

    if (!page)
        return false;

    return page->gridVisible;
}

void TPageHandler::setGridVisible(int number, bool state)
{
    DECL_TRACER("TPageHandler::setGridVisible(int number, bool state)");

    Page::PAGE_t *page = getPagePointer(number);

    if (!page)
        return;

    page->gridVisible = state;
}

bool TPageHandler::isSnapToGrid(int number)
{
    DECL_TRACER("TPageHandler::isSnapToGrid(int number)");

    PAGE_t *page = getPagePointer(number);

    if (!page)
        return false;

    return page->snapToGrid;
}

void TPageHandler::setSnapToGrid(int number, bool state)
{
    DECL_TRACER("TPageHandler::setSnapToGrid(int number, bool state)");

    PAGE_t *page = getPagePointer(number);

    if (!page)
        return;

    page->snapToGrid = state;
}

PAGE_t TPageHandler::getCurrentPage(QMdiArea *area)
{
    DECL_TRACER("TPageHandler::getCurrentPage(QMdiArea *area)");

    if (area && (!mMdiArea || mMdiArea != area))
        mMdiArea = area;

    if (!mMdiArea)
        return PAGE_t();

    QMdiSubWindow *subWin = mMdiArea->currentSubWindow();

    if (!subWin)
    {
        MSG_DEBUG("No open subwindow found");
        return PAGE_t();
    }

    int id = getObjectID(subWin->objectName(), "SubWindow_");

    if (id <= 0)
        return PAGE_t();

    for (PAGE_t page : mPages)
    {
        if (page.pageID == id)
            return page;
    }

    return PAGE_t();
}

bool TPageHandler::bringToFront(int number)
{
    DECL_TRACER("TPageHandler::bringToFront(int number)");

    MSG_DEBUG("Searing for window number: " << number);
    Page::PAGE_t *page = getPagePointer(number);

    if (!page || !page->visible)
        return false;

    if (page->baseObject.widget && page->baseObject.widget->parentWidget())
    {
        page->baseObject.widget->parentWidget()->raise();
        page->baseObject.widget->parentWidget()->show();
        return true;
    }

    return false;
}

TCanvasWidget *TPageHandler::getWidget(int number)
{
    DECL_TRACER("TPageHandler::getWidget(int number)");

    Page::PAGE_t *page = getPagePointer(number);

    if (!page)
        return nullptr;

    return page->baseObject.widget;
}

void TPageHandler::setWidget(TCanvasWidget *w, int number)
{
    DECL_TRACER("TPageHandler::setWidget(QWidget *w, int number)");

    if (!w || number < 1 || mPages.empty())
        return;

    Page::PAGE_t *page = getPagePointer(number);

    if (!page)
        return;

    page->baseObject.widget = w;
}

QList<int> TPageHandler::getPageNumbers()
{
    DECL_TRACER("TPageHandler::getPageNumbers()");

    if (mPages.empty())
        return QList<int>();

    QList<int> list;
    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        list.append(iter->pageID);
        MSG_DEBUG("Added page " << iter->pageID << " (" << iter->name.toStdString() << ")");
    }

    return list;
}

PAGE_t *TPageHandler::getPage(int number)
{
    DECL_TRACER("TPageHandler::getPage(int number)");

    if (number <= 0)
    {
        MSG_ERROR("Invalid page number " << number);
        return nullptr;
    }

    MSG_DEBUG("Having " << mPages.size() << " pages.");

    if (mPages.empty())
        return nullptr;

    return getPagePointer(number);
}

PAGE_t TPageHandler::getPage(const QString& name)
{
    DECL_TRACER("TPageHandler::getPage(const QString& name)");

    if (name.isEmpty())
    {
        MSG_ERROR("No page name!");
        return PAGE_t();
    }

    if (mPages.empty())
        return PAGE_t();

    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->popupType == PT_PAGE && iter->name == name)
            return *iter;
    }

    return PAGE_t();
}

PAGE_t TPageHandler::getPopup(const QString& name)
{
    DECL_TRACER("TPageHandler::getPopup(const QString& name)");

    if (name.isEmpty())
    {
        MSG_ERROR("No popup name!");
        return PAGE_t();
    }

    if (mPages.empty())
        return PAGE_t();

    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->popupType == PT_POPUP && iter->name == name)
            return *iter;
    }

    return PAGE_t();
}

void TPageHandler::setPage(PAGE_t& page)
{
    DECL_TRACER("TPageHandler::setPage(PAGE_t& page)");

    if (page.pageID <= 0 || page.name.isEmpty())
        return;

    Page::PAGE_t *pg = getPagePointer(page.pageID);

    if (!pg)
        return;

    *pg = page;
}

void TPageHandler::setPageBgColor(int number, QColor& col)
{
    DECL_TRACER("TPageHandler::setPageBgColor(int number, QColor& col)");

    Page::PAGE_t *page = getPagePointer(number);

    if (!page)
        return;

    page->srPage.cf = col;
}

void TPageHandler::setPageTextColor(int number, QColor& col)
{
    DECL_TRACER("TPageHandler::setPageTextColor(int number, QColor& col)");

    Page::PAGE_t *page = getPagePointer(number);

    if (!page)
        return;

    page->srPage.ct = col;
}

QStringList TPageHandler::getGroupNames()
{
    DECL_TRACER("TPageHandler::getGroupNames()");

    QStringList list;
    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->popupType == Page::PT_POPUP)
        {
            if (!list.contains(iter->group))
                list.append(iter->group);
        }
    }

    list.sort();
    return list;
}

void TPageHandler::changePageName(int id, const QString& name)
{
    DECL_TRACER("TPageHandler::changePageName(int id, const QString& name)");

    Page::PAGE_t *page = getPagePointer(id);

    if (!page)
        return;

    if (id > 0 && id < 500)
        TConfMain::Current().renamePage(id, name);
    else if (id > 500 && id < 1000)
        TConfMain::Current().renamePopup(id, name);

    page->name = name;
}

ObjHandler::TOBJECT_t TPageHandler::initNewObject(int bi, const QString& name)
{
    DECL_TRACER("TPageHandler::initNewObject(int bi, const QString& name)");

    if (bi < 1 || name.isEmpty())
        return ObjHandler::TOBJECT_t();

    ObjHandler::TOBJECT_t object;
    ObjHandler::SR_T sr;

    object.bi = bi;
    object.na = name;

    sr.number = 1;
    sr.cb = qRgb(255, 161, 0);  // Border color (#ffa100)
    sr.cf = qRgb(151, 190, 13); // Fill color (#97be0d)
    sr.ct = qRgb(0, 0, 0);      // Text color
    object.sr.push_back(sr);

    sr.number = 2;
    object.sr.push_back(sr);

    return object;
}

int TPageHandler::appendObject(int pageID, TObjectHandler *object)
{
    DECL_TRACER("TPageHandler::appendObject(int pageID, TObjectHandler *object)");

    if (pageID <= 0 || !object || object->getButtonIndex() <= 0)
        return -1;

    PAGE_t *page = getPagePointer(pageID);

    if (!page)
        return -1;

    // Check if object is unique
    for (TObjectHandler *o : page->objects)
    {
        if (o->getButtonIndex() == object->getButtonIndex())
        {
            MSG_WARNING("Attempt to insert duplicate button with button index " << o->getButtonIndex() << " for page " << pageID);
            return -1;
        }
    }

    page->objects.append(object);
    return page->objects.size() - 1;
}

void TPageHandler::setObject(int num, ObjHandler::TOBJECT_t& object)
{
    DECL_TRACER("TPageHandler::addObject(int num, ObjHandler::TOBJECT_t& object)");

    Page::PAGE_t *page = getPagePointer(num);

    if (!page)
        return;

    // Make sure the object does not already exist
    QList<TObjectHandler *>::Iterator iter;

    for (iter = page->objects.begin(); iter != page->objects.end(); ++iter)
    {
        TObjectHandler *obj = *iter;

        if (obj->getButtonIndex() == object.bi)
        {
            obj->setObject(object);
            return;
        }
    }

    TObjectHandler *o = new TObjectHandler;
    o->setObject(object);
    o->setZOrder(page->objects.size());
    page->objects.append(o);
}

ObjHandler::TOBJECT_t TPageHandler::getObject(int page, int bi)
{
    DECL_TRACER("TPageHandler::getObject(int page, int bi)");

    PAGE_t *pg = getPage(page);

    if (!pg || pg->pageID <= 0)
        return ObjHandler::TOBJECT_t();

    for (TObjectHandler *obj : pg->objects)
    {
        if (obj->getObject().bi == bi)
            return obj->getObject();
    }

    return ObjHandler::TOBJECT_t();
}

TObjectHandler *TPageHandler::getObjectHandler(int page, int bi)
{
    DECL_TRACER("TPageHandler::getObjectHandler(int page, int bi)");

    PAGE_t *pg = getPage(page);

    if (!pg || pg->pageID <= 0)
        return nullptr;

    for (TObjectHandler *obj : pg->objects)
    {
        if (obj->getObject().bi == bi)
            return obj;
    }

    return nullptr;
}

int TPageHandler::getObjectIndex(const PAGE_t& page, int bi)
{
    DECL_TRACER("TPageHandler::getObjectIndex(const PAGE_t& page, int bi)");

    if (page.pageID <= 0)
        return -1;

    int idx = 0;

    for (TObjectHandler *obj : page.objects)
    {
        if (obj->getObject().bi == bi)
            return idx;

        idx++;
    }

    return idx;
}

void TPageHandler::setSelectedToolToAllPages(TOOL t)
{
    DECL_TRACER("TPageHandler::setSelectedToolToAllPages(TOOL t)");

    if (mPages.empty())
        return;

    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->baseObject.widget && iter->visible)
            iter->baseObject.widget->setCurrentTool(t);
    }
}

QList<ObjHandler::TOBJECT_t> TPageHandler::getObjectList(const Page::PAGE_t& page)
{
    DECL_TRACER("TPageHandler::getObjectList(const Page::PAGE_t& page)");

    QList<ObjHandler::TOBJECT_t> objects;

    for (TObjectHandler *obj : page.objects)
        objects.append(obj->getObject());

    return objects;
}

void TPageHandler::setObjectGeometry(int pageID, int bi, const QRect& geom)
{
    DECL_TRACER("TPageHandler::setObjectGeometry(int pageID, int bi, const QRect& geom)");

    PAGE_t *page = getPagePointer(pageID);

    if (!page)
        return;

    for (TObjectHandler *obj : page->objects)
    {
        if (obj->getButtonIndex() == bi)
        {
            obj->setSize(geom);
            break;
        }
    }
}

bool TPageHandler::saveAllPages()
{
    DECL_TRACER("TPageHandler::saveAllPages()");

    QList<Page::PAGE_t>::Iterator pageIter;

    for (pageIter = mPages.begin(); pageIter != mPages.end(); ++pageIter)
    {
        if (pageIter->popupType == PT_PAGE)
        {
            if (!savePage(*pageIter))
                return false;
        }
        else if (pageIter->popupType == PT_POPUP)
        {
            if (!savePopup(*pageIter))
                return false;
        }
    }

    return true;
}

bool TPageHandler::savePage(const PAGE_t& page)
{
    DECL_TRACER("TPageHandler::savePage(const PAGE_t& page)");

    int setupPort = TConfMain::Current().getSetupPort();

    QJsonObject root;
    root.insert("type", page.popupType);
    root.insert("pageID", page.pageID);
    root.insert("name", page.name);
    INSERTJ(root, "description", page.description, "");
    root.insert("width", page.width);
    root.insert("height", page.height);
    INSERTJ(root, "collapseDirection", page.collapseDirection, 0);
    INSERTJ(root, "collapseOffset", page.collapseOffset, 0);
    INSERTJ(root, "ap", page.ap, setupPort);
    INSERTJ(root, "ad", page.ad, 1);
    INSERTJ(root, "cp", page.cp, setupPort);
    INSERTJ(root, "ch", page.ch, 1);

    QJsonArray objects = getObjects(page.objects);
    root.insert("objects", objects);

    QJsonObject sr = getSr(page.popupType, page.srPage);
    root.insert("sr", sr);

    QJsonDocument doc;
    doc.setObject(root);
    QString metaFile = mPathTemporary + "/" + page.name + ".json";
    QFile file(metaFile);

    if(!file.open(QIODevice::WriteOnly))
    {
        MSG_ERROR("Error opening file \"" << metaFile.toStdString() << "\" for writing!");
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool TPageHandler::savePopup(const PAGE_t& popup)
{
    DECL_TRACER("TPageHandler::savePopup(const PAGE_t& popup)");

    QJsonObject root;
    root.insert("type", popup.popupType);
    root.insert("pageID", popup.pageID);
    root.insert("name", popup.name);
    INSERTJ(root, "description", popup.description, "");
    root.insert("width", popup.width);
    root.insert("height", popup.height);
    root.insert("left", popup.left);
    root.insert("top", popup.top);
    root.insert("modal", popup.modal);
    root.insert("collapseDirection", popup.collapseDirection);
    root.insert("collapseOffset", popup.collapseOffset);
    INSERTJ(root, "group", popup.group, "");
    root.insert("timeout", popup.timeout);
    root.insert("showEffect", popup.showEffect);
    root.insert("showTime", popup.showTime);
    root.insert("showX", popup.showX);
    root.insert("showY", popup.showY);
    root.insert("hideEffect", popup.hideEffect);
    root.insert("hideTime", popup.hideTime);
    root.insert("hideX", popup.hideX);
    root.insert("hideY", popup.hideY);

    if (!popup.eventShow.empty())
    {
        QJsonArray eventShow;
        QList<EVENT_t>::ConstIterator iter;

        for (iter = popup.eventShow.constBegin(); iter != popup.eventShow.constEnd(); ++iter)
        {
            QJsonObject entry;
            entry.insert("evType", iter->evType);
            entry.insert("item", iter->item);
            entry.insert("evAction", iter->evAction);
            entry.insert("name", iter->name);
            entry.insert("ID", iter->ID);
            entry.insert("port", iter->port);
            eventShow.append(entry);
        }

        root.insert("eventShow", eventShow);
    }

    if (!popup.eventHide.empty())
    {
        QJsonArray eventHide;
        QList<EVENT_t>::ConstIterator iter;

        for (iter = popup.eventHide.constBegin(); iter != popup.eventHide.constEnd(); ++iter)
        {
            QJsonObject entry;
            entry.insert("evType", iter->evType);
            entry.insert("item", iter->item);
            entry.insert("evAction", iter->evAction);
            entry.insert("name", iter->name);
            entry.insert("ID", iter->ID);
            entry.insert("port", iter->port);
            eventHide.append(entry);
        }

        root.insert("eventHide", eventHide);
    }

    root.insert("objects", getObjects(popup.objects));

    QJsonObject sr = getSr(popup.popupType, popup.srPage);
    root.insert("sr", sr);

    QJsonDocument doc;
    doc.setObject(root);
    QString metaFile = mPathTemporary + "/" + popup.name + ".json";
    QFile file(metaFile);

    if(!file.open(QIODevice::WriteOnly))
    {
        MSG_ERROR("Error opening file \"" << metaFile.toStdString() << "\" for writing!");
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

QJsonArray TPageHandler::getObjects(const QList<TObjectHandler *>& objects)
{
    DECL_TRACER("TPageHandler::getObjects(const QList<TObjectHandler*>& objects)");

    int setupPort = TConfMain::Current().getSetupPort();
    QJsonArray obj;

    for (TObjectHandler *o : objects)
    {
        ObjHandler::TOBJECT_t e = o->getObject();
        QJsonObject bt;
        bt.insert("type", e.type);
        bt.insert("bi", e.bi);
        bt.insert("na", e.na);
        INSERTJ(bt, "li", e.li, false);
        INSERTJ(bt, "bd", e.bd, "");
        INSERTJ(bt, "lt", e.lt, 0);
        INSERTJ(bt, "tp", e.tp, 0);
        INSERTJ(bt, "wt", e.wt, 0);
        INSERTJ(bt, "ht", e.ht, 0);
        INSERTJ(bt, "zo", e.zo, 0);
        INSERTJ(bt, "hs", e.hs, "");
        INSERTJ(bt, "bs", e.bs, "");
        INSERTJ(bt, "fb", e.fb, 0);
        INSERTJ(bt, "ap", e.ap, 1);
        INSERTJ(bt, "ad", e.ad, setupPort);
        INSERTJ(bt, "ch", e.ch, setupPort);
        INSERTJ(bt, "cp", e.cp, 1);
        INSERTJ(bt, "lp", e.lp, setupPort);
        INSERTJ(bt, "lv", e.lv, setupPort);
        INSERTJ(bt, "ta", e.ta, 0);
        INSERTJ(bt, "ti", e.ti, 0);
        INSERTJ(bt, "tr", e.tr, 0);
        INSERTJ(bt, "tc", e.tc, 0);
        INSERTJ(bt, "tj", e.tj, 0);
        INSERTJ(bt, "tk", e.tk, 0);
        INSERTJ(bt, "of", e.of, 0);
        INSERTJ(bt, "tg", e.tg, 0);
        INSERTJ(bt, "so", e.so, 1);
        INSERTJ(bt, "co", e.co, 1);
        INSERTJ(bt, "dr", e.dr, "");
        INSERTJ(bt, "va", e.va, 0);

        if (e.type == ObjHandler::LISTVIEW)
        {
            INSERTJ(bt, "lvc", e.lvc, 0);   // G5 Listbox: Listview components? [ ORed values: (2 = Primary Text; 4 = Primary+Secondary Text; 1 = Image only)]
            INSERTJ(bt, "lvh", e.lvh, 0);   // G5 Listbox: Item height
            INSERTJ(bt, "lvl", e.lvl, 0);   // G5 Listbox: Listview columns
            INSERTJ(bt, "lhp", e.lhp, 0);   // G5 Listbox: Primary Partition (%)
            INSERTJ(bt, "lvp", e.lvp, 0);   // G5 Listbox: Secondary Partition (%)
            INSERTJ(bt, "lvs", e.lvs, 0);   // G5 Listbox: Filter enabled; 1 = TRUE
            INSERTJ(bt, "lsh", e.lsh, 0);   // G5 Listbox: Filter height
            INSERTJ(bt, "lva", e.lva, 0);   // G5 Listbox: Alphabet scrollbar; 1 = TRUE
            INSERTJ(bt, "lds", e.lds, "");  // G5 Listbox: Dynamic data source
            bt.insert("ldm", e.ldm);        // G5 Listbox: Internal distinct name?
        }

        INSERTJ(bt, "stateCount", e.stateCount, 0);
        INSERTJ(bt, "ddt", e.ddt, "");
        INSERTJ(bt, "rm", e.rm, 0);
        INSERTJ(bt, "nu", e.nu, 2);
        INSERTJ(bt, "nd", e.nd, 2);
        INSERTJ(bt, "ar", e.ar, 0);
        INSERTJ(bt, "ru", e.ru, 2);
        INSERTJ(bt, "rd", e.rd, 2);
        INSERTJ(bt, "lu", e.lu, 2);
        INSERTJ(bt, "ld", e.ld, 2);
        INSERTJ(bt, "rv", e.rv, 0);
        INSERTJ(bt, "rl", e.rl, 0);
        INSERTJ(bt, "rh", e.rh, 0);
        INSERTJ(bt, "ri", e.ri, 0);
        INSERTJ(bt, "ji", e.ji, 0);
        INSERTJ(bt, "rn", e.rn, 0);
        INSERTJ(bt, "ac", e.ac_di, 0);
        INSERTJ(bt, "hd", e.hd, 0);
        INSERTJ(bt, "da", e.da, 0);
        INSERTJ(bt, "pp", e.pp, 0);
        INSERTJ(bt, "lf", e.lf, "");
        INSERTJ(bt, "sd", e.sd, "");
        INSERTJ(bt, "vt", e.vt, "");
        INSERTJ(bt, "cd", e.cd, "");
        INSERTJ(bt, "sc", e.sc.name(), "");
        INSERTJ(bt, "cc", e.cc.name(), "");
        INSERTJ(bt, "mt", e.mt, 0);
        INSERTJ(bt, "dt", e.dt, "");
        INSERTJ(bt, "im", e.im, "");
        INSERTJ(bt, "st", e.st, 0);
        INSERTJ(bt, "ws", e.ws, 0);
        INSERTJ(bt, "on", e.on, "");
        INSERTJ(bt, "sa", e.sa, 0);
        INSERTJ(bt, "dy", e.dy, 0);
        INSERTJ(bt, "rs", e.rs, 0);
        INSERTJ(bt, "ba", e.ba, 0);
        INSERTJ(bt, "bo", e.bo, 0);
        INSERTJ(bt, "sw", e.sw, 1);
        INSERTJ(bt, "we", e.we, "");
        INSERTJ(bt, "pc", e.pc, "");
        INSERTJ(bt, "op", e.op, "");

        QJsonArray cm;

        for (QString s : e.cm)
            cm.append(s);

        bt.insert("cm", cm);

        QJsonArray pushFunc;

        for (ObjHandler::PUSH_FUNC_T f : e.pushFunc)
        {
            QJsonObject pf;
            pf.insert("item", f.item);
            pf.insert("pfType", f.pfType);
            pf.insert("pfAction", f.pfAction);
            pf.insert("pfName", f.pfName);
            pf.insert("action", f.action);
            pf.insert("id", f.ID);
            pf.insert("event", f.event);
            pushFunc.append(pf);
        }

        bt.insert("pushFunc", pushFunc);

        QJsonArray sr;

        for (ObjHandler::SR_T s : e.sr)
        {
            QJsonObject jsr;
            jsr.insert("number", s.number);
            INSERTJ(jsr, "do", s._do, "");
            INSERTJ(jsr, "bs", s.bs, "");                   // Frame type (circle, ...)
            INSERTJ(jsr, "mi", s.mi, "");                   // Chameleon image
            jsr.insert("cb", s.cb.name(QColor::HexArgb));   // Border color
            jsr.insert("ft", s.ft);                         // G5: Fill type for gradient colors.
            jsr.insert("cf", s.cf.name(QColor::HexArgb));   // Fill color
            jsr.insert("ct", s.ct.name(QColor::HexArgb));   // Text Color
            jsr.insert("ec", s.ec.name(QColor::HexArgb));   // Text effect color
            INSERTJ(jsr, "bm", s.bm, "");                   // bitmap file name
            QJsonArray bitmaps;
            int idx = 0;

            for (ObjHandler::BITMAPS_t bitmap : s.bitmaps)
            {
                QJsonObject bm;

                bm.insert("index", idx);
                bm.insert("fileName", bitmap.fileName);
                bm.insert("dynamic", bitmap.dynamic);
                bm.insert("justification", bitmap.justification);
                bm.insert("offsetX", bitmap.offsetX);
                bm.insert("offsetY", bitmap.offsetY);
                bitmaps.append(bm);
                idx++;
            }

            if (!s.bitmaps.empty())
                jsr.insert("bitmapEntries", bitmaps);

            if (!s.gradientColors.empty())
            {
                QJsonArray gradientColors;
                QList<QString>::const_iterator iter;

                for (iter = s.gradientColors.cbegin(); iter != s.gradientColors.cend(); ++iter)
                    gradientColors.append(*iter);

                jsr.insert("gradientColors", gradientColors);
            }

            INSERTJ(jsr, "gr", s.gr, 0);
            INSERTJ(jsr, "gx", s.gx, 0);
            INSERTJ(jsr, "gy", s.gy, 0);
            INSERTJ(jsr, "sd", s.sd, "");
            INSERTJ(jsr, "dynamic", s.dynamic, false);
            INSERTJ(jsr, "sb", s.sb, 0);
            INSERTJ(jsr, "jb", s.jb, 5);
            INSERTJ(jsr, "bx", s.bx, 0);
            INSERTJ(jsr, "by", s.by, 0);
            INSERTJ(jsr, "fi", s.fi, 0);
            INSERTJ(jsr, "te", s.te, "");
            INSERTJ(jsr, "jt", s.jt, ObjHandler::ORI_CENTER_MIDDLE);
            INSERTJ(jsr, "tx", s.tx, 0);
            INSERTJ(jsr, "ty", s.ty, 0);
            INSERTJ(jsr, "ff", s.ff, "");
            INSERTJ(jsr, "fs", s.fs, 0);
            INSERTJ(jsr, "ww", s.ww, 0);
            INSERTJ(jsr, "et", s.et, 0);

            if (s.oo >= 0)
                jsr.insert("oo", s.oo);

            INSERTJ(jsr, "md", s.md, 0);
            INSERTJ(jsr, "mr", s.mr, 0);
            INSERTJ(jsr, "ms", s.ms, 1);
            INSERTJ(jsr, "vf", s.vf, "");

            sr.append(jsr);
        }

        bt.insert("sr", sr);
        obj.append(bt);
    }

    return obj;
}

QJsonObject TPageHandler::getSr(PAGE_TYPE pt, const SR_t& srPage, int number)
{
    DECL_TRACER("TPageHandler::getSr(PAGE_TYPE pt, const SR_t& srPage, int number)");

    QJsonObject sr;

    if (pt == PT_POPUP)
        sr.insert("number", number);

    INSERTJ(sr, "bs", srPage.bs, "");                         // Frame type (circle, ...)
    INSERTJ(sr, "mi", srPage.mi, "");                         // Chameleon image
    sr.insert("cb", srPage.cb.name(QColor::HexArgb));   // Border color
    sr.insert("ft", srPage.ft);                         // G5: Fill type for gradient colors.
    sr.insert("cf", srPage.cf.name(QColor::HexArgb));   // Fill color
    sr.insert("ct", srPage.ct.name(QColor::HexArgb));   // Text Color
    sr.insert("ec", srPage.ec.name(QColor::HexArgb));   // Text effect color
    INSERTJ(sr, "bm", srPage.bm, "");                         // bitmap file name
    QJsonArray bitmaps;
    int idx = 0;

    for (ObjHandler::BITMAPS_t bitmap : srPage.bitmaps)
    {
        QJsonObject bm;
        bm.insert("index", idx);
        bm.insert("fileName", bitmap.fileName);
        bm.insert("dynamic", bitmap.dynamic);
        bm.insert("justification", bitmap.justification);
        bm.insert("offsetX", bitmap.offsetX);
        bm.insert("offsetY", bitmap.offsetY);
        bitmaps.append(bm);
        idx++;
    }

    if (!srPage.bitmaps.empty())
        sr.insert("bitmaps", bitmaps);

    if (!srPage.gradientColors.empty())
    {
        QJsonArray gradientColors;
        QList<QColor>::ConstIterator iter;

        for (iter = srPage.gradientColors.begin(); iter != srPage.gradientColors.end(); ++iter)
            gradientColors.append(iter->name(QColor::HexArgb));

        sr.insert("gradientColors", gradientColors);
    }

    INSERTJ(sr, "gr", srPage.gr, 0);
    INSERTJ(sr, "gx", srPage.gx, 0);
    INSERTJ(sr, "gy", srPage.gy, 0);
    sr.insert("dynamic", srPage.dynamic);
    INSERTJ(sr, "jb", srPage.jb, 5);
    INSERTJ(sr, "bx", srPage.bx, 0);
    INSERTJ(sr, "by", srPage.by, 0);
    INSERTJ(sr, "fi", srPage.fi, 0);
    INSERTJ(sr, "te", srPage.te, "");
    INSERTJ(sr, "jt", srPage.jt, 5);
    INSERTJ(sr, "tx", srPage.tx, 0);
    INSERTJ(sr, "ty", srPage.ty, 0);
    INSERTJ(sr, "ff", srPage.ff, "");
    INSERTJ(sr, "fs", srPage.fs, 0);
    INSERTJ(sr, "ww", srPage.ww, 0);
    INSERTJ(sr, "et", srPage.et, 0);

    if (srPage.oo >= 0)
        sr.insert("oo", srPage.oo);

    return sr;
}

bool TPageHandler::readPages(const QStringList& list)
{
    DECL_TRACER("TPageHandler::readPages(const QStringList& list)");

    if (list.empty())
        return true;

    if (!mPages.empty())
        reset();

    QStringList::ConstIterator iter;

    for (iter = list.constBegin(); iter != list.constEnd(); ++iter)
    {
        QFile pageFile(mPathTemporary + "/" + *iter + ".json");
        MSG_DEBUG("Reading page " << pageFile.fileName().toStdString());

        if (!pageFile.exists())
        {
            MSG_ERROR("The page file " << iter->toStdString() << " was not found!");
            return false;
        }

        if (!pageFile.open(QIODevice::ReadOnly))
        {
            MSG_ERROR("Error reading file " << iter->toStdString());
            return false;
        }

        QJsonDocument doc = QJsonDocument::fromJson(pageFile.readAll());
        pageFile.close();
        parsePage(doc.object());
    }

    return true;
}

Page::PAGE_t *TPageHandler::getPagePointer(int num)
{
    DECL_TRACER("TPageHandler::getPagePointer(int num)");

    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->pageID == num)
            return &(*iter);
    }

    return nullptr;
}

void TPageHandler::parsePage(const QJsonObject& page)
{
    DECL_TRACER("TPageHandler::parsePage(const QJsonObject& page)");

    int setupPort = TConfMain::Current().getSetupPort();
    PAGE_t pg;
    pg.popupType = static_cast<PAGE_TYPE>(page.value("type").toInt(PT_UNKNOWN));
    pg.pageID = page.value("pageID").toInt(0);
    pg.name = page.value("name").toString();
    pg.description = page.value("description").toString();
    pg.ap = page.value("ap").toInt(setupPort);
    pg.ad = page.value("ad").toInt(1);
    pg.cp = page.value("cp").toInt(setupPort);
    pg.ch = page.value("ch").toInt(1);
    pg.left = page.value("left").toInt(0);
    pg.top = page.value("top").toInt(0);
    pg.width = page.value("width").toInt(0);
    pg.height = page.value("height").toInt(0);
    pg.modal = page.value("modal").toInt(0);
    pg.collapseDirection = static_cast<COLDIR_t>(page.value("collapseDirection").toInt(COLDIR_NONE));
    pg.collapseOffset = page.value("collapseOffset").toInt(0);
    pg.collapsible = page.value("collapsible").toBool(false);
    pg.colState = static_cast<COLLAPS_STATE_t>(page.value("colState").toInt(COL_CLOSED));
    pg.group = page.value("group").toString();
    pg.timeout = page.value("timeout").toInt(0);
    pg.showEffect = static_cast<SHOWEFFECT>(page.value("showEffect").toInt(SE_NONE));
    pg.showTime = page.value("showTime").toInt(0);
    pg.showX = page.value("showX").toInt(0);
    pg.showY = page.value("showY").toInt(0);
    pg.hideEffect = static_cast<SHOWEFFECT>(page.value("hideEffect").toInt(SE_NONE));
    pg.hideTime = page.value("hideTime").toInt(0);
    pg.hideX = page.value("hideX").toInt(0);
    pg.hideY = page.value("hideY").toInt(0);

    QJsonObject srPage = page.value("sr").toObject();
    pg.srPage.bs = srPage.value("bs").toString();
    pg.srPage.mi = srPage.value("mi").toString();
    pg.srPage.cb = QColor::fromString(srPage.value("cb").toString("#ffff0000"));
    pg.srPage.ft = srPage.value("ft").toString();
    pg.srPage.cf = QColor::fromString(srPage.value("cf").toString("#ffffffff"));
    pg.srPage.ct = QColor::fromString(srPage.value("ct").toString("#ff000000"));
    pg.srPage.ec = QColor::fromString(srPage.value("ec").toString("#ff808080"));
    pg.srPage.bm = srPage.value("bm").toString();

    QJsonArray bitmaps = srPage.value("bitmaps").toArray();

    for (int i = 0; i < bitmaps.count(); ++i)
    {
        QJsonObject entry = bitmaps[i].toObject();
        ObjHandler::BITMAPS_t bm;
        bm.fileName = entry.value("fileName").toString();
        bm.dynamic = entry.value("dynamic").toBool(false);
        bm.justification = static_cast<ObjHandler::ORIENTATION>(entry.value("justification").toInt(ObjHandler::ORI_CENTER_MIDDLE));
        bm.offsetX = entry.value("offsetX").toInt(0);
        bm.offsetY = entry.value("offsetY").toInt(0);
        pg.srPage.bitmaps.append(bm);
    }

    QJsonArray gradientColors = srPage.value("gradientColors").toArray();

    for (int i = 0; i < gradientColors.count(); ++i)
        pg.srPage.gradientColors.append(QColor::fromString(gradientColors[i].toString()));

    pg.srPage.gr = srPage.value("gr").toInt(0);
    pg.srPage.gx = srPage.value("gx").toInt(0);
    pg.srPage.gy = srPage.value("gy").toInt(0);
    pg.srPage.dynamic = srPage.value("dynamic").toBool(false);
    pg.srPage.sb = srPage.value("sb").toInt(0);
    pg.srPage.jb = srPage.value("jb").toInt(5);
    pg.srPage.bx = srPage.value("bx").toInt(0);
    pg.srPage.by = srPage.value("by").toInt(0);
    pg.srPage.fi = srPage.value("fi").toInt(0);
    pg.srPage.te = srPage.value("te").toString();
    pg.srPage.jt = static_cast<ObjHandler::ORIENTATION>(srPage.value("jt").toInt(ObjHandler::ORI_CENTER_MIDDLE));
    pg.srPage.tx = srPage.value("tx").toInt(0);
    pg.srPage.ty = srPage.value("ty").toInt(0);
    pg.srPage.ff = srPage.value("ff").toString();
    pg.srPage.fs = srPage.value("fs").toInt(0);
    pg.srPage.ww = srPage.value("ww").toInt(0);
    pg.srPage.et = srPage.value("et").toInt(0);
    pg.srPage.oo = srPage.value("oo").toInt(0);

    QJsonArray eventShow = page.value("eventShow").toArray();

    for (int i = 0; i < eventShow.count(); ++i)
    {
        QJsonObject entry = eventShow[i].toObject();
        EVENT_t event;
        event.evType = static_cast<EVENT_TYPE_t>(entry.value("evType").toInt(EV_NONE));
        event.item = entry.value("item").toInt(0);
        event.evAction = entry.value("evAction").toString();
        event.name = entry.value("name").toString();
        event.ID = entry.value("ID").toInt(0);
        event.port = entry.value("port").toInt(0);
        pg.eventShow.append(event);
    }

    QJsonArray eventHide = page.value("eventHide").toArray();

    for (int i = 0; i < eventHide.count(); ++i)
    {
        QJsonObject entry = eventHide[i].toObject();
        EVENT_t event;
        event.evType = static_cast<EVENT_TYPE_t>(entry.value("evType").toInt(EV_NONE));
        event.item = entry.value("item").toInt(0);
        event.evAction = entry.value("evAction").toString();
        event.name = entry.value("name").toString();
        event.ID = entry.value("ID").toInt(0);
        event.port = entry.value("port").toInt(0);
        pg.eventHide.append(event);
    }

    parseObjects(&pg, page.value("objects").toArray());
    mPages.append(pg);
}

void TPageHandler::parseObjects(PAGE_t *page, const QJsonArray& obj)
{
    DECL_TRACER("TPageHandler::parseObjects(PAGE_t *page, const QJsonArray& obj)");

    int setupPort = TConfMain::Current().getSetupPort();

    for (int i = 0; i < obj.count(); ++i)
    {
        QJsonObject jo = obj[i].toObject();
        ObjHandler::TOBJECT_t object;
        object.type = static_cast<ObjHandler::BUTTONTYPE>(jo.value("type").toInt(ObjHandler::BUTTONTYPE::NONE));
        object.bi = jo.value("bi").toInt(0);
        object.na = jo.value("na").toString();
        object.bd = jo.value("bd").toString();
        object.li = jo.value("li").toBool(false);
        object.lt = jo.value("lt").toInt(0);
        object.tp = jo.value("tp").toInt(0);
        object.wt = jo.value("wt").toInt(0);
        object.ht = jo.value("ht").toInt(0);
        object.zo = jo.value("zo").toInt(0);
        object.hs = jo.value("hs").toString();
        object.bs = jo.value("bs").toString();
        object.fb = static_cast<ObjHandler::FEEDBACK>(jo.value("fb").toInt(ObjHandler::FEEDBACK::FB_NONE));
        object.ap = jo.value("ap").toInt(1);
        object.ad = jo.value("ad").toInt(setupPort);
        object.ch = jo.value("ch").toInt(setupPort);
        object.cp = jo.value("cp").toInt(1);
        object.lp = jo.value("lp").toInt(1);
        object.lv = jo.value("lv").toInt(setupPort);
        object.ta = jo.value("ta").toInt(0);
        object.ti = jo.value("ti").toInt(0);
        object.tr = jo.value("tr").toInt(0);
        object.tc = jo.value("tc").toInt(0);
        object.tj = jo.value("tj").toInt(0);
        object.tk = jo.value("tk").toInt(0);
        object.of = jo.value("of").toInt(0);
        object.tg = jo.value("tg").toInt(0);
        object.so = jo.value("so").toInt(1);
        object.co = jo.value("co").toInt(1);
        object.lvc = jo.value("lvc").toInt(0);      // G5 Listbox: Listview components? [ ORed values: (2 = Primary Text; 4 = Primary+Secondary Text; 1 = Image only)]
        object.lvh = jo.value("lvh").toInt(48);     // G5 Listbox: Item height
        object.lvl = jo.value("lvl").toInt(1);      // G5 Listbox: Listview columns
        object.lhp = jo.value("lhp").toInt(5);      // G5 Listbox: Primary Partition (%)
        object.lvp = jo.value("lvp").toInt(95);     // G5 Listbox: Secondary Partition (%)
        object.lvs = jo.value("lvs").toInt(0);      // G5 Listbox: Filter enabled; 1 = TRUE
        object.lsh = jo.value("lsh").toInt(24);     // G5 Listbox: Filter height
        object.lva = jo.value("lva").toInt(0);      // G5 Listbox: Alphabet scrollbar; 1 = TRUE
        object.lds = jo.value("lds").toString();    // G5 Listbox: Dynamic data source
        object.ldm = jo.value("ldm").toString();    // G5 Listbox: Internal distinct name?
        object.ddt = jo.value("ddt").toString();

        QJsonArray cm = jo.value("cm").toArray();

        for (int j = 0; j < cm.count(); ++j)
            object.cm.push_back(cm[j].toString());

        object.dr = jo.value("dr").toString();
        object.va = jo.value("va").toInt(0);
        object.stateCount = jo.value("stateCount").toInt(0);
        object.rm = jo.value("rm").toInt(0);
        object.nu = jo.value("nu").toInt(2);
        object.nd = jo.value("nd").toInt(2);
        object.ar = jo.value("ar").toInt(0);
        object.ru = jo.value("ru").toInt(2);
        object.rd = jo.value("rd").toInt(2);
        object.lu = jo.value("lu").toInt(2);
        object.ld = jo.value("ld").toInt(2);
        object.rv = jo.value("rv").toInt(0);
        object.rl = jo.value("rl").toInt(0);
        object.rh = jo.value("rh").toInt(0);
        object.ri = jo.value("ri").toInt(0);
        object.ji = jo.value("ji").toInt(0);
        object.rn = jo.value("rn").toInt(0);
        object.ac_di = jo.value("ac").toInt(0);
        object.hd = jo.value("hd").toInt(0);
        object.da = jo.value("da").toInt(0);
        object.pp = jo.value("pp").toInt(0);
        object.lf = jo.value("lf").toString();
        object.sd = jo.value("sd").toString();
        object.vt = jo.value("vt").toString();
        object.cd = jo.value("cd").toString();
        object.sc = QColor(jo.value("sc").toString());
        object.cc = QColor(jo.value("cc").toString());
        object.mt = jo.value("mt").toInt(0);
        object.dt = jo.value("dt").toString();
        object.im = jo.value("im").toString();
        object.st = jo.value("st").toInt(0);
        object.ws = jo.value("ws").toInt(0);
        object.on = jo.value("on").toString();
        object.sa = jo.value("sa").toInt(0);
        object.dy = jo.value("dy").toInt(0);
        object.rs = jo.value("rs").toInt(0);
        object.ba = jo.value("ba").toInt(0);
        object.bo = jo.value("bo").toInt(0);
        object.sw = jo.value("sw").toInt(1);
        object.we = jo.value("we").toString();
        object.pc = jo.value("pc").toString();
        object.op = jo.value("op").toString();

        QJsonArray pushFunc = jo.value("pushFunc").toArray();

        for (int j = 0; j < pushFunc.count(); ++j)
        {
            ObjHandler::PUSH_FUNC_T pf;
            QJsonObject jpf = pushFunc[j].toObject();
            pf.item = jpf.value("item").toInt(0);
            pf.pfType = jpf.value("pfType").toString();
            pf.pfAction = jpf.value("pfAction").toString();
            pf.pfName = jpf.value("pfName").toString();
            pf.action = static_cast<ObjHandler::BUTTON_ACTION_t>(jpf.value("action").toInt(ObjHandler::BUTTON_ACTION_t::BT_ACTION_PGFLIP));
            pf.ID = jpf.value("id").toInt(0);
            pf.event = static_cast<ObjHandler::BUTTON_EVENT_t>(jpf.value("event").toInt(ObjHandler::BUTTON_EVENT_t::EVENT_NONE));
            object.pushFunc.push_back(pf);
        }

        QJsonArray sr = jo.value("sr").toArray();

        for (int j = 0; j < sr.count(); ++j)
        {
            QJsonObject jsr = sr[j].toObject();
            ObjHandler::SR_T s;
            s.number = jsr.value("number").toInt(0);
            s._do = jsr.value("do").toString();
            s.bs = jsr.value("bs").toString();
            s.mi = jsr.value("mi").toString();
            s.cb = QColor(jsr.value("cb").toString());      // Border color
            s.ft = jsr.value("ft").toString();
            s.cf = QColor(jsr.value("cf").toString());      // Fill color
            s.ct = QColor(jsr.value("ct").toString());      // Text color
            s.ec = QColor(jsr.value("ec").toString());      // Text effect color
            s.bm = jsr.value("bm").toString();
            QJsonArray bitmaps = jsr.value("bitmaps").toArray();

            for (int k = 0; k < bitmaps.count(); ++k)
            {
                QJsonObject bm = bitmaps[k].toObject();
                ObjHandler::BITMAPS_t m;
                m.fileName = bm.value("fileName").toString();
                m.index = bm.value("index").toInt(-1);
                m.dynamic = bm.value("dynamic").toBool(false);
                m.justification = static_cast<ObjHandler::ORIENTATION>(bm.value("justification").toInt(ObjHandler::ORIENTATION::ORI_CENTER_MIDDLE));
                m.offsetX = bm.value("offsetX").toInt(0);
                m.offsetY = bm.value("offsetY").toInt(0);
                s.bitmaps.append(m);
            }

            QJsonArray gradients = jsr.value("gradientColors").toArray();

            for (int k = 0; k < gradients.count(); ++k)
                s.gradientColors.push_back(gradients[k].toString());

            s.gr = jsr.value("gr").toInt(15);
            s.gx = jsr.value("gx").toInt(50);
            s.gy = jsr.value("gy").toInt(50);
            s.sd = jsr.value("sd").toString();
            s.dynamic = jsr.value("dynamic").toBool(false);
            s.sb = jsr.value("sb").toInt(0);
            s.jb = jsr.value("jb").toInt(5);
            s.bx = jsr.value("bx").toInt(0);
            s.by = jsr.value("by").toInt(0);
            s.fi = jsr.value("fi").toInt(0);
            s.te = jsr.value("te").toString();
            s.jt = static_cast<ObjHandler::ORIENTATION>(jsr.value("jt").toInt(ObjHandler::ORIENTATION::ORI_CENTER_MIDDLE));
            s.tx = jsr.value("tx").toInt(0);
            s.ty = jsr.value("ty").toInt(0);
            s.ff = jsr.value("ff").toString();
            s.fs = jsr.value("fs").toInt(0);
            s.ww = jsr.value("ww").toInt(0);
            s.et = jsr.value("et").toInt(0);
            s.oo = jsr.value("oo").toInt(-1);
            s.md = jsr.value("md").toInt(0);
            s.mr = jsr.value("mr").toInt(0);
            s.ms = jsr.value("ms").toInt(1);
            s.vf = jsr.value("vf").toString();
            object.sr.push_back(s);
        }

        TObjectHandler *o = new TObjectHandler(object.type, object.bi, object.na);
        o->setObject(object);
        page->objects.append(o);
    }
}
