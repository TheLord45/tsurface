/*
 * Copyright (C) 2025 by Andreas Theofilu <andreas@theosys.at>
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
 * @param width     The width of the page/popup
 * @param height    The height of the page/popup
 *
 * @return  On success the new page or popup number is returned. On error it returns 0.
 */
int TPageHandler::createPage(QWidget *w, Page::PAGE_TYPE pt, const QString& name, int width, int height)
{
    DECL_TRACER("TPageHandler::createPage(QWidget *w, Page::PAGE_TYPE pt, const QString& name, int width, int height)");

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

            if (iter->widget && iter->widget != w)
                iter->widget = w;

            return iter->pageID;
        }
    }

    mPages.append(page);
    return page.pageID;
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

PAGE_t TPageHandler::getPage(int number)
{
    DECL_TRACER("TPageHandler::getPage(int number)");

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
