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

#include "tpagehandler.h"
#include "terror.h"

using namespace Page;

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
int TPageHandler::createPage(QWidget *w, Page::PAGE_TYPE pt, const QString& name, const QRect& geom)
{
    DECL_TRACER("TPageHandler::createPage(QWidget *w, Page::PAGE_TYPE pt, const QString& name, const QRect& geom)");

    return createPage(w, pt, name, geom.width(), geom.height(), geom.left(), geom.top());
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
int TPageHandler::createPage(QWidget *w, Page::PAGE_TYPE pt, const QString& name, int width, int height, int left, int top)
{
    DECL_TRACER("TPageHandler::createPage(QWidget *w, Page::PAGE_TYPE pt, const QString& name, int width, int height, int left, int top)");

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

    page.widget = w;

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

            if (iter->widget && iter->widget != w)
                iter->widget = w;

            return iter->pageID;
        }
    }

    mPages.append(page);
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

void TPageHandler::bringToFront(int number)
{
    DECL_TRACER("TPageHandler::bringToFront(int number)");

    MSG_DEBUG("Searing for window number: " << number);
    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->pageID == number && iter->visible)
        {
            if (iter->widget && iter->widget->parentWidget())
            {
                iter->widget->parentWidget()->raise();
                iter->widget->parentWidget()->show();
                return;
            }
        }
    }
}

QWidget *TPageHandler::getWidget(int number)
{
    DECL_TRACER("TPageHandler::getWidget(int number)");

    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->pageID == number)
            return iter->widget;
    }

    return nullptr;
}

void TPageHandler::setWidget(QWidget *w, int number)
{
    DECL_TRACER("TPageHandler::setWidget(QWidget *w, int number)");

    if (!w || number < 1 || mPages.empty())
        return;

    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->pageID == number)
        {
            iter->widget = w;
            break;
        }
    }
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

PAGE_t TPageHandler::getPage(int number)
{
    DECL_TRACER("TPageHandler::getPage(int number)");

    if (number <= 0)
    {
        MSG_ERROR("Invalid page number " << number);
        return PAGE_t();
    }

    MSG_DEBUG("Having " << mPages.size() << " pages.");

    if (mPages.empty())
        return PAGE_t();

    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->pageID == number)
            return *iter;
    }

    return PAGE_t();
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

    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->pageID == page.pageID)
        {
            *iter = page;
            break;
        }
    }
}

void TPageHandler::setPageBgColor(int number, QColor& col)
{
    DECL_TRACER("TPageHandler::setPageBgColor(int number, QColor& col)");

    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->pageID == number)
        {
            iter->srPage.cf = col;
            break;
        }
    }
}

void TPageHandler::setPageTextColor(int number, QColor& col)
{
    DECL_TRACER("TPageHandler::setPageTextColor(int number, QColor& col)");

    QList<PAGE_t>::Iterator iter;

    for (iter = mPages.begin(); iter != mPages.end(); ++iter)
    {
        if (iter->pageID == number)
        {
            iter->srPage.ct = col;
            break;
        }
    }
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

    QJsonObject root;
    root.insert("type", page.popupType);
    root.insert("pageID", page.pageID);
    root.insert("name", page.name);
    root.insert("description", page.description);
    root.insert("width", page.width);
    root.insert("height", page.height);
    root.insert("collapseDirection", page.collapseDirection);
    root.insert("collapseOffset", page.collapseOffset);
    QJsonObject sr = getSr(page.popupType, page.srPage);
    root.insert("sr", sr);

    // TODO: Add objects on page here

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
    root.insert("description", popup.description);
    root.insert("width", popup.width);
    root.insert("height", popup.height);
    root.insert("left", popup.left);
    root.insert("top", popup.top);
    root.insert("modal", popup.modal);
    root.insert("collapseDirection", popup.collapseDirection);
    root.insert("collapseOffset", popup.collapseOffset);
    root.insert("group", popup.group);
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

    // TODO: Add objects on popup here

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

QJsonObject TPageHandler::getSr(PAGE_TYPE pt, const SR_t& srPage, int number)
{
    DECL_TRACER("TPageHandler::getSr(PAGE_TYPE pt, const SR_t& srPage, int number)");

    QJsonObject sr;

    if (pt == PT_POPUP)
        sr.insert("number", number);

    sr.insert("bs", srPage.bs);                         // Frame type (circle, ...)
    sr.insert("mi", srPage.mi);                         // Chameleon image
    sr.insert("cb", srPage.cb.name(QColor::HexArgb));   // Border color
    sr.insert("ft", srPage.ft);                         // G5: Fill type for gradient colors.
    sr.insert("cf", srPage.cf.name(QColor::HexArgb));   // Fill color
    sr.insert("ct", srPage.ct.name(QColor::HexArgb));   // Text Color
    sr.insert("ec", srPage.ec.name(QColor::HexArgb));   // Text effect color
    sr.insert("bm", srPage.bm);                         // bitmap file name
    QJsonArray bitmaps;
    bool haveBitmap = false;

    for (int i = 0; i < 5; ++i)
    {
        QJsonObject bm;

        if (!srPage.bitmaps[i].fileName.isEmpty())
        {
            bm.insert("index", i);
            bm.insert("fileName", srPage.bitmaps[i].fileName);
            bm.insert("dynamic", srPage.bitmaps[i].dynamic);
            bm.insert("justification", srPage.bitmaps[i].justification);
            bm.insert("offsetX", srPage.bitmaps[i].offsetX);
            bm.insert("offsetY", srPage.bitmaps[i].offsetY);
            bm.insert("width", srPage.bitmaps[i].width);
            bm.insert("height", srPage.bitmaps[i].height);
            bitmaps.append(bm);
            haveBitmap = true;
        }
    }

    if (haveBitmap)
        sr.insert("bitmapEntries", bitmaps);

    if (!srPage.gradientColors.empty())
    {
        QJsonArray gradientColors;
        QList<QColor>::ConstIterator iter;

        for (iter = srPage.gradientColors.begin(); iter != srPage.gradientColors.end(); ++iter)
            gradientColors.append(iter->name(QColor::HexArgb));

        sr.insert("gradientColors", gradientColors);
    }

    sr.insert("gr", srPage.gr);
    sr.insert("gx", srPage.gx);
    sr.insert("gy", srPage.gy);
    sr.insert("dynamic", srPage.dynamic);
    sr.insert("jb", srPage.jb);
    sr.insert("bx", srPage.bx);
    sr.insert("by", srPage.by);
    sr.insert("fi", srPage.fi);
    sr.insert("te", srPage.te);
    sr.insert("jt", srPage.jt);
    sr.insert("tx", srPage.tx);
    sr.insert("ty", srPage.ty);
    sr.insert("ff", srPage.ff);
    sr.insert("fs", srPage.fs);
    sr.insert("ww", srPage.ww);
    sr.insert("et", srPage.et);

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

void TPageHandler::parsePage(const QJsonObject& page)
{
    DECL_TRACER("TPageHandler::parsePage(const QJsonObject& page)");

    PAGE_t pg;
    pg.popupType = static_cast<PAGE_TYPE>(page.value("type").toInt(PT_UNKNOWN));
    pg.pageID = page.value("pageID").toInt(0);
    pg.name = page.value("name").toString();
    pg.description = page.value("description").toString();
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
        int idx = entry.value("index").toInt(-1);

        if (idx < 0)
            continue;

        pg.srPage.bitmaps[idx].fileName = entry.value("fileName").toString();
        pg.srPage.bitmaps[idx].dynamic = entry.value("dynamic").toBool(false);
        pg.srPage.bitmaps[idx].justification = static_cast<ObjHandler::ORIENTATION>(entry.value("justification").toInt(ObjHandler::ORI_CENTER_MIDDLE));
        pg.srPage.bitmaps[idx].offsetX = entry.value("offsetX").toInt(0);
        pg.srPage.bitmaps[idx].offsetY = entry.value("offsetY").toInt(0);
        pg.srPage.bitmaps[idx].width = entry.value("width").toInt(0);
        pg.srPage.bitmaps[idx].height = entry.value("height").toInt(0);
    }

    QJsonArray gradientColors = srPage.value("gradientColors").toArray();

    for (int i = 0; i < gradientColors.count(); ++i)
        pg.srPage.gradientColors.append(QColor::fromString(gradientColors[i].toString()));

    pg.srPage.gr = srPage.value("gr").toInt(0);
    pg.srPage.gx = srPage.value("gx").toInt();
    pg.srPage.gy = srPage.value("gy").toInt();
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

    // TODO: Add code to parse the objects
    mPages.append(pg);
}
