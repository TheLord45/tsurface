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
#include <filesystem>

#include <QTreeView>
#include <QStandardItemModel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QGuiApplication>

#include "tconfmain.h"
#include "tpaneltypes.h"
#include "terror.h"

using namespace ConfigMain;
namespace fs = std::filesystem;
using std::string;

TConfMain *TConfMain::mCurrent{nullptr};

TConfMain::TConfMain()
{
    DECL_TRACER("TConfMain::TConfMain()");

    mFontBase = QGuiApplication::font();
}

TConfMain::~TConfMain()
{
    DECL_TRACER("TConfMain::~TConfMain()");

    if (mConfMain)
    {
        delete mConfMain;
        mConfMain = nullptr;
    }
}

TConfMain& TConfMain::Current()
{
    if (!mCurrent)
        mCurrent = new TConfMain;

    return *mCurrent;
}

/**
 * @brief TConfMain::createNew
 * Creates a new, empty project. It initializes the tree in the Pages tree view.
 * If there exists already a project it is closed silently and a new project is
 * allocated.
 * This method must be called only once to create the main page. If this is
 * called and there exists already a project, the project is deleted and a new
 * one will be created.
 *
 * @param file      The file name of the project
 * @param pname     The name of the main page
 * @param project   The project name (job name)
 */
void TConfMain::createNew(const QString& file, const QString& pname, const QString& project)
{
    DECL_TRACER("TConfMain::createNew(const QString& file, const QString& pname, const QString& project)");

    mFileName = file;
    mJobName = project;

    if (mConfMain)
        delete mConfMain;

    mConfMain = new CONFMAIN_t;
    mConfMain->fileName = file;
    mConfMain->projectInfo.jobName = project;
    PAGEENTRY_t page;
    page.name = pname;
    page.pageID = 1;
    page.file = QString("%1.json").arg(pname);
    page.popupType = PN_PAGE;
    mConfMain->pageList.append(page);
}

/**
 * @brief TConfMain::addPage
 * This method adds a new page to the tree of pages. The name must be unique!
 *
 * @param name  The name of the new page
 * @param num   The number of the new page
 */
void TConfMain::addPage(const QString& name, int num)
{
    DECL_TRACER("TConfMain::addPage(const QString& name, int num)");

    if (num < 1 || num > 50)
    {
        MSG_ERROR("Can't add page " << name.toStdString() << " because number " << num << " is out of range (1 - 50)");
        return;
    }
    // Check if the name and number is unique
    QList<PAGEENTRY_t>::Iterator iter;

    for (iter = mConfMain->pageList.begin(); iter != mConfMain->pageList.end(); ++iter)
    {
        if (iter->popupType == PN_PAGE && iter->name == name)
            return;

        if (iter->popupType == PN_PAGE && iter->pageID == num)
            return;
    }

    PAGEENTRY_t page;
    page.name = name;
    page.pageID = num;
    page.file = QString("%1.json").arg(name);
    page.popupType = PN_PAGE;
    mConfMain->pageList.append(page);
}

/**
 * @brief TConfMain::addPopup
 * This method adds a new popup to the tree of pages. The name must be unique!
 *
 * @param name  The name of the new popup
 * @param num   The number of the new popup
 */
void TConfMain::addPopup(const QString& name, int num)
{
    DECL_TRACER("TConfMain::addPopup(const QString& name, int num)");

    if (num < 500 || num >= 1000)
    {
        MSG_ERROR("Can't add popup page " << name.toStdString() << " because number " << num << " is out of range (500 - 1000)");
        return;
    }
    // Check if the name and number is unique
    QList<PAGEENTRY_t>::Iterator iter;

    for (iter = mConfMain->pageList.begin(); iter != mConfMain->pageList.end(); ++iter)
    {
        if (iter->popupType == PN_POPUP && iter->name == name)
            return;

        if (iter->popupType == PN_POPUP && iter->pageID == num)
            return;
    }

    PAGEENTRY_t page;
    page.name = name;
    page.pageID = num;
    page.file = QString("%1.json").arg(name);
    page.popupType = PN_POPUP;
    mConfMain->pageList.append(page);
}

void TConfMain::renamePage(int num, const QString& name)
{
    DECL_TRACER("TConfMain::renamePage(int num, const QString& name)");

    QList<PAGEENTRY_t>::Iterator iter;

    for (iter = mConfMain->pageList.begin(); iter != mConfMain->pageList.end(); ++iter)
    {
        if (iter->pageID == num)
        {
            string fname = QString("%1/%2").arg(mBaseDirectory).arg(iter->file).toStdString();

            if (fs::exists(fname))
                fs::remove(fname);

            iter->name = name;
            iter->file = QString("%1.json").arg(name);
            break;
        }
    }
}

void TConfMain::renamePopup(int num, const QString& name)
{
    DECL_TRACER("TConfMain::renamePopup(int num, const QString& name)");

    QList<PAGEENTRY_t>::Iterator iter;

    for (iter = mConfMain->pageList.begin(); iter != mConfMain->pageList.end(); ++iter)
    {
        if (iter->pageID == num)
        {
            string fname = QString("%1/%2").arg(mBaseDirectory).arg(iter->file).toStdString();

            if (fs::exists(fname))
                fs::remove(fname);

            iter->name = name;
            iter->file = QString("%1.json").arg(name);
            break;
        }
    }
}

void TConfMain::deletePage(const QString& name)
{
    DECL_TRACER("TConfMain::deletePage(const QString& name)");

    QList<PAGEENTRY_t>::Iterator iter;

    for (iter = mConfMain->pageList.begin(); iter != mConfMain->pageList.end(); ++iter)
    {
        if (iter->popupType == PN_PAGE && iter->name == name)
        {
            string fname = QString("%1/%2").arg(mBaseDirectory).arg(iter->file).toStdString();

            if (fs::exists(fname))
                fs::remove(fname);

            mConfMain->pageList.erase(iter);
            break;
        }
    }
}

void TConfMain::deletePopup(const QString& name)
{
    DECL_TRACER("TConfMain::deletePopup(const QString& name)");

    QList<PAGEENTRY_t>::Iterator iter;

    for (iter = mConfMain->pageList.begin(); iter != mConfMain->pageList.end(); ++iter)
    {
        if (iter->popupType == PN_POPUP && iter->name == name)
        {
            string fname = QString("%1/%2").arg(mBaseDirectory).arg(iter->file).toStdString();

            if (fs::exists(fname))
                fs::remove(fname);

            mConfMain->pageList.erase(iter);
            break;
        }
    }
}

void TConfMain::setProjectInfo(const PROJECTINFO_t& pi)
{
    DECL_TRACER("TConfMain::setProjectInfo(const PROJECTINFO_t& pi)");

    if (!mConfMain)
        mConfMain = new CONFMAIN_t;

    mConfMain->projectInfo = pi;
}

void TConfMain::setSetup(const SETUP_t& setup)
{
    DECL_TRACER("TConfMain::setSetup(const SETUP_t& setup)");

    if (!mConfMain)
        mConfMain = new CONFMAIN_t;

    mConfMain->setup = setup;
}

bool TConfMain::readMain(const QString& path)
{
    DECL_TRACER("TConfMain::readMain(const QString& path)");

    return false;
}

void TConfMain::setFileName(const QString& fn)
{
    DECL_TRACER("TConfMain::setFileName(const QString& fn)");

    if (!mConfMain)
        return;

    mFileName = fn;
    mConfMain->fileName = fn;
}

QString TConfMain::getFileName()
{
    DECL_TRACER("TConfMain::getFileName()");

    if (!mConfMain)
        return QString();

    return mConfMain->fileName;
}

void TConfMain::saveProject()
{
    DECL_TRACER("TConfMain::saveProject()");
}

void TConfMain::reset()
{
    DECL_TRACER("TConfMain::reset()");

    if (!mConfMain)
        return;

    delete mConfMain;
    mConfMain = nullptr;
}

QSize TConfMain::getPanelSize()
{
    DECL_TRACER("TConfMain::getPanelSize()");

    if (!mConfMain)
        return QSize();

    return mConfMain->projectInfo.panelSize;
}

QList<QString> TConfMain::getAllPages()
{
    DECL_TRACER("TConfMain::getAllPages()");

    if (!mConfMain || mConfMain->pageList.isEmpty())
        return QList<QString>();

    QList<QString> list;
    QList<PAGEENTRY_t>::Iterator iter;

    for (iter = mConfMain->pageList.begin(); iter != mConfMain->pageList.end(); ++iter)
        list.append(iter->name);

    return list;
}

QList<QString> TConfMain::getAllPopups()
{
    DECL_TRACER("TConfMain::getAllPopups()");

    if (!mConfMain || mConfMain->popupList.isEmpty())
        return QList<QString>();

    QList<QString> list;
    QList<PAGEENTRY_t>::Iterator iter;

    for (iter = mConfMain->popupList.begin(); iter != mConfMain->popupList.end(); ++iter)
        list.append(iter->name);

    return list;
}
