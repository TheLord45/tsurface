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
#include <QTreeView>
#include <QStandardItemModel>
#include <QJsonObject>
#include <QJsonDocument>

#include "tconfmain.h"
#include "tpaneltypes.h"
#include "terror.h"

using namespace ConfigMain;

TConfMain *TConfMain::mCurrent{nullptr};

TConfMain::TConfMain()
{
    DECL_TRACER("TConfMain::TConfMain()");
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
 *
 * @param file      The file name of the project
 * @param pname     The name of the project
 */
void TConfMain::createNew(const QString& file, const QString& pname, const QString& project)
{
    DECL_TRACER("TConfMain::createNew(const QString& file, const QString& pname, const QString& project)");

    mFileName = file;
    mJobName = pname;

    if (mConfMain)
        delete mConfMain;

    mConfMain = new CONFMAIN_t;

}

void TConfMain::setProjectInfo(const PROJECTINFO_t& pi)
{
    DECL_TRACER("TConfMain::setProjectInfo(const PROJECTINFO_t& pi)");

    if (!mConfMain)
        return;

    mConfMain->projectInfo = pi;
}

void TConfMain::setSetup(const SETUP_t& setup)
{
    DECL_TRACER("TConfMain::setSetup(const SETUP_t& setup)");

    if (!mConfMain)
        return;

    mConfMain->setup = setup;
}

bool TConfMain::readMain(const QString& path)
{
    DECL_TRACER("TConfMain::readMain(const QString& path)");

    return false;
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
