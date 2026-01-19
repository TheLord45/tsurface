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
#include <QJsonArray>
#include <QJsonDocument>
#include <QGuiApplication>
#include <QMessageBox>
#include <QFile>

#include "tconfmain.h"
#include "tpaneltypes.h"
#include "tfonts.h"
#include "tmaps.h"
#include "tmisc.h"
#include "terror.h"

using namespace ConfigMain;
namespace fs = std::filesystem;
using std::string;

const int FILE_VERSION = 1;

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

    initConfig(true);
    mConfMain->fileName = basename(file);
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

    for (iter = mConfMain->popupList.begin(); iter != mConfMain->popupList.end(); ++iter)
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
    mConfMain->popupList.append(page);
}

void TConfMain::renamePage(int num, const QString& name)
{
    DECL_TRACER("TConfMain::renamePage(int num, const QString& name)");

    QList<PAGEENTRY_t>::Iterator iter;

    for (iter = mConfMain->pageList.begin(); iter != mConfMain->pageList.end(); ++iter)
    {
        if (iter->pageID == num)
        {
            string fname = QString("%1/%2").arg(mPathTemporary).arg(iter->file).toStdString();
            MSG_DEBUG("Renaming file \"" << iter->file.toStdString() << "\" to \"" << name.toStdString() << ".json\"");
            MSG_DEBUG("Source: " << fname);

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

    for (iter = mConfMain->popupList.begin(); iter != mConfMain->popupList.end(); ++iter)
    {
        if (iter->pageID == num)
        {
            string fname = QString("%1/%2").arg(mPathTemporary).arg(iter->file).toStdString();
            MSG_DEBUG("Renaming file \"" << iter->file.toStdString() << "\" to \"" << name.toStdString() << ".json\"");
            MSG_DEBUG("Source: " << fname);

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
            string fname = QString("%1/%2").arg(mPathTemporary).arg(iter->file).toStdString();

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

    for (iter = mConfMain->popupList.begin(); iter != mConfMain->popupList.end(); ++iter)
    {
        if (iter->popupType == PN_POPUP && iter->name == name)
        {
            string fname = QString("%1/%2").arg(mPathTemporary).arg(iter->file).toStdString();

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

    initConfig();
    mConfMain->projectInfo = pi;
}

void TConfMain::setSetup(const SETUP_t& setup)
{
    DECL_TRACER("TConfMain::setSetup(const SETUP_t& setup)");

    initConfig();
    mConfMain->setup = setup;
}

bool TConfMain::readProject(const QString& path)
{
    DECL_TRACER("TConfMain::readMain(const QString& path)");

    QFile project(path);

    if (!project.exists())
    {
        MSG_ERROR("The project file " << path.toStdString() << " was not found!");
        return false;
    }

    if (!project.open(QIODevice::ReadOnly))
    {
        MSG_ERROR("Error reading file \"" << path.toStdString() << "\": " << project.errorString().toStdString());
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(project.readAll());
    project.close();

    initConfig(true);

    if (mPathTemporary.isEmpty())
        mPathTemporary = basename(path);

    QJsonObject root = doc.object();
    QJsonObject versionInfo = root.value("versionInfo").toObject();

    mConfMain->fileName = versionInfo.value("fileName").toString();
    mConfMain->fileVersion = versionInfo.value("fileVersion").toInt(0);

    if (mConfMain->fileVersion != FILE_VERSION)
    {
        MSG_ERROR("Invalid file version " << mConfMain->fileVersion);
        return false;
    }

    QJsonObject projectInfo = root.value("projectInfo").toObject();
    mConfMain->projectInfo.protection = projectInfo.value("protection").toBool(false);
    mConfMain->projectInfo.password = projectInfo.value("password").toString();
    mConfMain->projectInfo.jobName = projectInfo.value("jobName").toString(mConfMain->fileName);
    mConfMain->projectInfo.panelType = projectInfo.value("panelType").toString();
    mConfMain->projectInfo.panelSize = QSize(projectInfo.value("panelWidth").toInt(), projectInfo.value("panelHeight").toInt());
    mConfMain->projectInfo.revision = projectInfo.value("revision").toString();
    mConfMain->projectInfo.designer = projectInfo.value("designer").toString();
    mConfMain->projectInfo.dealer = projectInfo.value("dealer").toString();
    mConfMain->projectInfo.company = projectInfo.value("company").toString();
    mConfMain->projectInfo.copyright = projectInfo.value("copyright").toString();
    mConfMain->projectInfo.comment = projectInfo.value("comment").toString();
    mConfMain->projectInfo.date = QDateTime::fromString(projectInfo.value("date").toString());
    mConfMain->projectInfo.lastDate = QDateTime::fromString(projectInfo.value("lastDate").toString());
    mConfMain->projectInfo.logLevel = projectInfo.value("logLevel").toString();
    mConfMain->projectInfo.logFile = projectInfo.value("logFile").toString();

    QJsonObject fileList = root.value("logFile").toObject();
    mConfMain->fileList.mapFile = fileList.value("mapFile").toString("maps_.json");
    mConfMain->fileList.colorFile = fileList.value("colorFile").toString("palette_.json");
    mConfMain->fileList.fontFile = fileList.value("fontFile").toString("fonts_.json");
    mConfMain->fileList.themeFile = fileList.value("themeFile").toString("theme_.json");
    mConfMain->fileList.buttonFile = fileList.value("buttonFile").toString("buttons_.json");
    mConfMain->fileList.appFile = fileList.value("appFile").toString("apps_.json");

    QJsonObject setup = root.value("setup").toObject();
    mConfMain->setup.portCount = setup.value("portCount").toInt();
    mConfMain->setup.setupPort = setup.value("setupPort").toInt();
    mConfMain->setup.powerUpPage = setup.value("powerUpPage").toString();
    QJsonArray powerUpPopups = setup.value("powerUpPopups").toArray();

    for (int i = 0; i < powerUpPopups.count(); ++i)
        mConfMain->setup.powerUpPopups.append(powerUpPopups[i].toString());

    mConfMain->setup.startupString = setup.value("startupString").toString();
    mConfMain->setup.wakeupString = setup.value("wakeupString").toString();
    mConfMain->setup.sleepString = setup.value("sleepString").toString();
    mConfMain->setup.shutdownString = setup.value("shutdownString").toString();
    mConfMain->setup.idlePage = setup.value("idlePage").toString();
    mConfMain->setup.inactivityPage = setup.value("inactivityPage").toString();
    mConfMain->setup.idleTimeout = setup.value("idleTimeout").toInt(0);
    mConfMain->setup.screenWidth = setup.value("screenWidth").toInt(1920);
    mConfMain->setup.screenHeight = setup.value("screenHeight").toInt(1024);
    mConfMain->setup.screenRotate = setup.value("screenRotate").toBool(false);
    mConfMain->setup.batteryLevelPort = setup.value("batteryLevelPort").toInt(0);
    mConfMain->setup.batteryLevelCode = setup.value("batteryLevelCode").toInt(100);
    mConfMain->setup.marqeeSpeed = setup.value("marqeeSpeed").toInt(1);

    QJsonArray pageList = root.value("pageList").toArray();

    for (int i = 0; i < pageList.count(); ++i)
    {
        QJsonObject entry = pageList[i].toObject();
        PAGEENTRY_t pe;
        pe.name = entry.value("name").toString();
        pe.pageID = entry.value("pageID").toInt(0);
        pe.file = entry.value("file").toString();
        pe.isValid = entry.value("isValid").toBool(true);
        pe.popupType = static_cast<PANELTYPE_t>(entry.value("popupType").toInt(PN_PAGE));
        mConfMain->pageList.append(pe);
    }

    QJsonArray popupList = root.value("popupList").toArray();

    for (int i = 0; i < popupList.count(); ++i)
    {
        QJsonObject entry = popupList[i].toObject();
        PAGEENTRY_t pe;
        pe.name = entry.value("name").toString();
        pe.pageID = entry.value("pageID").toInt(0);
        pe.file = entry.value("file").toString();
        pe.isValid = entry.value("isValid").toBool(true);
        pe.popupType = static_cast<PANELTYPE_t>(entry.value("popupType").toInt(PN_POPUP));
        mConfMain->popupList.append(pe);
    }

    QJsonArray resourceList = root.value("resourceList").toArray();

    for (int i = 0; i < resourceList.count(); ++i)
    {
        QJsonObject entry = resourceList[i].toObject();
        RESOURCE_t res;
        res.name = entry.value("name").toString();
        res.protocol = entry.value("protocol").toString();
        res.host = entry.value("host").toString();
        res.path = entry.value("path").toString();
        res.file = entry.value("file").toString();
        res.password = entry.value("password").toString();
        res.user = entry.value("user").toString();
        res.refreshRate = entry.value("refresh").toInt(0);
        res.refreshStart = entry.value("dynamo").toBool(false);
        mConfMain->resourceList.append(res);
    }

    QJsonArray dataSourceList = root.value("dataSourceList").toArray();

    for (int i = 0; i < dataSourceList.count(); ++i)
    {
        QJsonObject entry = dataSourceList[i].toObject();
        DATASOURCE_t dres;
        dres.name = entry.value("name").toString();
        dres.protocol = entry.value("protocol").toString();
        dres.host = entry.value("host").toString();
        dres.path = entry.value("path").toString();
        dres.file = entry.value("file").toString();
        dres.password = entry.value("password").toString();
        dres.user = entry.value("user").toString();
        dres.refreshRate = entry.value("refresh").toInt(0);
        dres.force = entry.value("force").toBool(false);

        dres.format = entry.value("format").toString();
        dres.sort = static_cast<SORTMAP_t>(entry.value("sort").toInt(SORT_NONE));
        dres.mapIdT1 = entry.value("mapIdT1").toString();
        dres.mapIdT2 = entry.value("mapIdT2").toString();
        dres.mapIdI1 = entry.value("mapIdI1").toString();
        dres.sortQuery = entry.value("sortAdv").toString();
        dres.delimiter = entry.value("delimiter").toString(";");
        dres.quoted = entry.value("quoted").toBool(false);
        dres.headlines = entry.value("headlines").toInt(0);

        QJsonArray sortOrder = entry.value("sortList").toArray();

        for (int j = 0; j < sortOrder.count(); ++j)
            dres.sortOrder.append(sortOrder[j].toString());

        mConfMain->dataSourceList.append(dres);
    }

    if (!TFonts::readFontFile(mPathTemporary, mConfMain->fileList.fontFile))
        return false;

    TMaps::Current().setPathTemporary(mPathTemporary);

    if (!TMaps::Current().readMaps(mPathTemporary, mConfMain->fileList.mapFile))
        return false;

    return true;
}

void TConfMain::setFileName(const QString& fn)
{
    DECL_TRACER("TConfMain::setFileName(const QString& fn)");

    if (!mConfMain)
        return;

    mFileName = fn;
    mConfMain->fileName = basename(fn);
}

QString TConfMain::getFileName()
{
    DECL_TRACER("TConfMain::getFileName()");

    if (!mConfMain)
        return QString();

    return mFileName;
}

void TConfMain::setDynamicResource(const RESOURCE_t& res)
{
    DECL_TRACER("TConfMain::setDynamicResource(const RESOURCE_t& res)");

    if (!mConfMain)
        return;

    if (res.name.isEmpty() || res.host.isEmpty() || res.file.isEmpty())
    {
        MSG_ERROR("Incomplete resource data! Ignoring resource.");
        return;
    }

    QList<RESOURCE_t>::Iterator iter;

    for (iter = mConfMain->resourceList.begin(); iter != mConfMain->resourceList.end(); ++iter)
    {
        if (iter->name == res.name)
        {
            iter->protocol = res.protocol;
            iter->host = res.host;
            iter->path = res.path;
            iter->file = res.file;
            iter->user = res.user;
            iter->password = res.password;
            iter->refreshRate = res.refreshRate;
            iter->refreshStart = res.refreshStart;
            return;
        }
    }

    mConfMain->resourceList.append(res);
}

RESOURCE_t TConfMain::getDynamicResource(const QString& name)
{
    DECL_TRACER("getDynamicResource(const QString& name)");

    if (name.isEmpty() || !mConfMain)
        return RESOURCE_t();

    QList<RESOURCE_t>::Iterator iter;

    for (iter = mConfMain->resourceList.begin(); iter != mConfMain->resourceList.end(); ++iter)
    {
        if (iter->name == name)
            return *iter;
    }

    return RESOURCE_t();
}

void TConfMain::removeDynamicImage(const QString& name)
{
    DECL_TRACER("TConfMain::removeDynamicImage(const QString& name)");

    if (name.isEmpty() || !mConfMain)
        return;

    QList<RESOURCE_t>::Iterator iter;

    for (iter = mConfMain->resourceList.begin(); iter != mConfMain->resourceList.end(); ++iter)
    {
        if (iter->name == name)
        {
            mConfMain->resourceList.erase(iter);
            break;
        }
    }
}

bool TConfMain::haveDynamicResource(const QString& name)
{
    DECL_TRACER("TConfMain::haveDynamicResource(const QString& name)");

    if (name.isEmpty() || !mConfMain)
        return false;

    QList<RESOURCE_t>::Iterator iter;

    for (iter = mConfMain->resourceList.begin(); iter != mConfMain->resourceList.end(); ++iter)
    {
        if (iter->name == name)
            return true;
    }

    return false;
}

void TConfMain::setDynamicData(const DATASOURCE_t& data)
{
    DECL_TRACER("TConfMain::setDynamicData(const DATASOURCE_t& data)");

    if (!mConfMain)
        return;

    if (!mConfMain->dataSourceList.empty())
    {
        QList<DATASOURCE_t>::Iterator iter;

        for (iter = mConfMain->dataSourceList.begin(); iter != mConfMain->dataSourceList.end(); ++iter)
        {
            if (iter->name == data.name)
            {
                iter->protocol = data.protocol;
                iter->host = data.host;
                iter->path = data.path;
                iter->file = data.file;
                iter->user = data.user;
                iter->password = data.password;
                iter->refreshRate = data.refreshRate;
                iter->force = data.force;
                iter->format = data.format;
                iter->mapIdT1 = data.mapIdT1;
                iter->mapIdT2 = data.mapIdT2;
                iter->mapIdI1 = data.mapIdI1;
                iter->sort = data.sort;
                iter->sortOrder = data.sortOrder;
                iter->sortQuery = data.sortQuery;
                iter->delimiter = data.delimiter;
                iter->quoted = data.quoted;
                iter->headlines = data.headlines;
                iter->live = data.live;
                return;
            }
        }
    }

    mConfMain->dataSourceList.append(data);
}

DATASOURCE_t TConfMain::getDynamicData(const QString& name)
{
    DECL_TRACER("TConfMain::getDynamicData(const QString& name)");

    QList<DATASOURCE_t>::Iterator iter;

    for (iter = mConfMain->dataSourceList.begin(); iter != mConfMain->dataSourceList.end(); ++iter)
    {
        if (iter->name == name)
            return *iter;
    }

    return DATASOURCE_t();
}

void TConfMain::removeDynamicData(const QString& name)
{
    DECL_TRACER("TConfMain::removeDynamicData(const QString& name)");

    QList<DATASOURCE_t>::Iterator iter;

    for (iter = mConfMain->dataSourceList.begin(); iter != mConfMain->dataSourceList.end(); ++iter)
    {
        if (iter->name == name)
        {
            mConfMain->dataSourceList.erase(iter);
            return;
        }
    }
}

QList<DATASOURCE_t>& TConfMain::getAllDynamicData()
{
    DECL_TRACER("TConfMain::getAllDynamicData()");

    return mConfMain->dataSourceList;
}

void TConfMain::saveProject()
{
    DECL_TRACER("TConfMain::saveProject()");

    QJsonObject root;
    QJsonObject versionInfo;
    versionInfo.insert("fileVersion", mConfMain->fileVersion);
    versionInfo.insert("fileName", mConfMain->fileName);
    root.insert("versionInfo", versionInfo);

    QJsonObject projectInfo;
    projectInfo.insert("protection", mConfMain->projectInfo.protection);
    projectInfo.insert("password", mConfMain->projectInfo.password);
    projectInfo.insert("jobName", mConfMain->projectInfo.jobName);
    projectInfo.insert("panelType", mConfMain->projectInfo.panelType);
    projectInfo.insert("panelWidth", mConfMain->projectInfo.panelSize.width());
    projectInfo.insert("panelHeight", mConfMain->projectInfo.panelSize.height());
    projectInfo.insert("revision", mConfMain->projectInfo.revision);
    projectInfo.insert("designer", mConfMain->projectInfo.designer);
    projectInfo.insert("dealer", mConfMain->projectInfo.dealer);
    projectInfo.insert("company", mConfMain->projectInfo.company);
    projectInfo.insert("copyright", mConfMain->projectInfo.copyright);
    projectInfo.insert("comment", mConfMain->projectInfo.comment);
    projectInfo.insert("date", mConfMain->projectInfo.date.toString());
    projectInfo.insert("lastDate", mConfMain->projectInfo.lastDate.toString());
    projectInfo.insert("logLevel", mConfMain->projectInfo.logLevel);
    projectInfo.insert("logFile", mConfMain->projectInfo.logFile);
    root.insert("projectInfo", projectInfo);

    QJsonObject fileInfo;
    fileInfo.insert("mapFile", mConfMain->fileList.mapFile);
    fileInfo.insert("colorFile", mConfMain->fileList.colorFile);
    fileInfo.insert("fontFile", mConfMain->fileList.fontFile);
    fileInfo.insert("themeFile", mConfMain->fileList.themeFile);
    fileInfo.insert("buttonFile", mConfMain->fileList.buttonFile);
    fileInfo.insert("appFile", mConfMain->fileList.appFile);
    root.insert("fileInfo", fileInfo);

    QJsonObject setup;
    setup.insert("portCount", mConfMain->setup.portCount);
    setup.insert("setupPort", mConfMain->setup.setupPort);
    setup.insert("powerUpPage", mConfMain->setup.powerUpPage);
    QJsonArray pwrPopups;
    QList<QString>::Iterator pwrIter;

    for (pwrIter = mConfMain->setup.powerUpPopups.begin(); pwrIter != mConfMain->setup.powerUpPopups.end(); ++pwrIter)
        pwrPopups.append(*pwrIter);

    setup.insert("powerUpPopups", pwrPopups);
    setup.insert("startupString", mConfMain->setup.startupString);
    setup.insert("wakeupString", mConfMain->setup.wakeupString);
    setup.insert("sleepString", mConfMain->setup.sleepString);
    setup.insert("shutdownString", mConfMain->setup.shutdownString);
    setup.insert("idlePage", mConfMain->setup.idlePage);
    setup.insert("inactivityPage", mConfMain->setup.inactivityPage);
    setup.insert("idleTimeout", mConfMain->setup.idleTimeout);
    setup.insert("screenWidth", mConfMain->setup.screenWidth);
    setup.insert("screenHeight", mConfMain->setup.screenHeight);
    setup.insert("screenRotate", mConfMain->setup.screenRotate);
    setup.insert("batteryLevelPort", mConfMain->setup.batteryLevelPort);
    setup.insert("batteryLevelCode", mConfMain->setup.batteryLevelCode);
    setup.insert("marqeeSpeed", mConfMain->setup.marqeeSpeed);
    root.insert("setup", setup);

    QJsonArray pgList;
    QList<PAGEENTRY_t>::Iterator pglIter;

    for (pglIter = mConfMain->pageList.begin(); pglIter != mConfMain->pageList.end(); ++pglIter)
    {
        QJsonObject entry;
        entry.insert("name", pglIter->name);
        entry.insert("pageID", pglIter->pageID);
        entry.insert("file", pglIter->file);
        entry.insert("isValid", pglIter->isValid);
        entry.insert("popupType", pglIter->popupType);
        pgList.append(entry);
    }

    root.insert("pageList", pgList);

    QJsonArray popupList;

    for (pglIter = mConfMain->popupList.begin(); pglIter != mConfMain->popupList.end(); ++pglIter)
    {
        QJsonObject entry;
        entry.insert("name", pglIter->name);
        entry.insert("pageID", pglIter->pageID);
        entry.insert("file", pglIter->file);
        entry.insert("isValid", pglIter->isValid);
        entry.insert("popupType", pglIter->popupType);
        popupList.append(entry);
    }

    root.insert("popupList", popupList);

    QJsonArray resourceList;
    QList<RESOURCE_t>::Iterator resIter;

    for (resIter = mConfMain->resourceList.begin(); resIter != mConfMain->resourceList.end(); ++resIter)
    {
        QJsonObject entry;
        entry.insert("name", resIter->name);
        entry.insert("protocol", resIter->protocol);
        entry.insert("host", resIter->host);
        entry.insert("path", resIter->path);
        entry.insert("file", resIter->file);
        entry.insert("user", resIter->user);
        entry.insert("password", resIter->password);
        entry.insert("refresh", resIter->refreshRate);
        entry.insert("dynamo", resIter->refreshStart);
        resourceList.append(entry);
    }

    root.insert("resourceList", resourceList);

    QJsonArray dataSourceList;
    QList<DATASOURCE_t>::Iterator dresIter;

    for (dresIter = mConfMain->dataSourceList.begin(); dresIter != mConfMain->dataSourceList.end(); ++dresIter)
    {
        QJsonObject entry;
        entry.insert("name", dresIter->name);
        entry.insert("protocol", dresIter->protocol);
        entry.insert("host", dresIter->host);
        entry.insert("path", dresIter->path);
        entry.insert("file", dresIter->file);
        entry.insert("user", dresIter->user);
        entry.insert("password", dresIter->password);
        entry.insert("refresh", dresIter->refreshRate);
        entry.insert("force", dresIter->force);
        entry.insert("format", dresIter->format);
        entry.insert("sort", dresIter->sort);
        entry.insert("mapIdT1", dresIter->mapIdT1);
        entry.insert("mapIdT2", dresIter->mapIdT2);
        entry.insert("mapIdI1", dresIter->mapIdI1);
        entry.insert("sortAdv", dresIter->sortQuery);
        entry.insert("delimiter", dresIter->delimiter);
        entry.insert("quoted", dresIter->quoted);
        entry.insert("headlines", dresIter->headlines);

        if (!dresIter->sortOrder.empty())
        {
            QStringList::Iterator sortIter;
            QJsonArray sortOrder;

            for (sortIter = dresIter->sortOrder.begin(); sortIter != dresIter->sortOrder.end(); ++sortIter)
                sortOrder.append(*sortIter);

            entry.insert("sortList", sortOrder);
        }

        dataSourceList.append(entry);
    }

    root.insert("dataSourceList", dataSourceList);

    QJsonDocument doc;
    doc.setObject(root);
    QString metaFile = mPathTemporary + "/prj_.json";
    QFile file(metaFile);

    if(!file.open(QIODevice::WriteOnly))
    {
        MSG_ERROR("Error opening file \"" << metaFile.toStdString() << "\" for writing!");
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    // Save all used fonts
    TFonts::writeFontFile(mPathTemporary, mConfMain->fileList.fontFile);
    // Save the maps
    TMaps::Current().writeMaps(mPathTemporary, mConfMain->fileList.mapFile);
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

void TConfMain::initConfig(bool force)
{
    DECL_TRACER("TConfMain::initConfig(bool force)");

    if (!mConfMain)
        mConfMain = new CONFMAIN_t;
    else if (force)
    {
        delete mConfMain;
        mConfMain = new CONFMAIN_t;
    }

    mConfMain->fileList.fontFile = "fonts_.json";
    mConfMain->fileList.appFile = "apps_.json";
    mConfMain->fileList.colorFile = "palette_.json";
    mConfMain->fileList.mapFile = "maps_.json";
    mConfMain->fileList.themeFile = "theme_.json";
    mConfMain->fileList.buttonFile = "buttons_.json";

    if (!mFileName.isEmpty())
        mConfMain->fileName = basename(mFileName);
}
