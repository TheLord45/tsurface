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
#include <QSettings>

#include "tconfig.h"
#include "terror.h"

using std::endl;

TConfig *TConfig::mCurrent = nullptr;

TConfig::TConfig()
{
    mSettings = new QSettings("TheoSys", "TSurface");
}

TConfig::~TConfig()
{
    if (mSettings)
        delete mSettings;
}

/**
 * @brief TConfig::Current - Initialize singleton class
 * This method creates a new instance of this class, which is a
 * singleton class. This makes sure that it exists only one time.
 *
 * @return A pointer to this class.
 */
TConfig &TConfig::Current()
{
    if (!mCurrent)
    {
        mCurrent = new TConfig;
        mCurrent->init();
    }

    return *mCurrent;
}

void TConfig::init()
{
    initDefaults();

    if (!readConfig())
    {
        initDefaults();
        saveConfig();
    }
    else
        setLogging();

    mInitialized = true;
}

void TConfig::initDefaults()
{
    char *home = getenv("HOME");
    QString sHome;

    if (home)
        sHome = QString(home);
    else
        sHome = "/";

    mLastDirectory = sHome;
    mPosition.setLeft(10);
    mPosition.setTop(10);
    mPosition.setWidth(1100);
    mPosition.setHeight(800);
    mSplitterPosition = 260;

    setLogging();
}

void TConfig::setLogging()
{
    if (!mInitialized)
    {
        mLogLevel = "PROTOCOL";
        char *logfile = getenv("TSLOGFILE");

        if (logfile)
        {
            mLogFile = logfile;
            TStreamError::setLogFile(mLogFile.toStdString());
        }

        char *loglevel = getenv("TSLOGLEVEL");

        if (loglevel)
        {
            mLogLevel = loglevel;
            TStreamError::setLogLevel(mLogLevel.toStdString());
        }
        else
            TStreamError::setLogLevel(mLogLevel.toStdString());
    }
    else
    {
        if (!mLogFile.isEmpty())
            TStreamError::setLogFile(mLogFile.toStdString());

        if (!mLogLevel.isEmpty())
            TStreamError::setLogLevel(mLogLevel.toStdString());
        else
        {
            TStreamError::setLogLevel("PROTOCOL");
            mLogLevel = "PROTOCOL";
        }
    }
}

void TConfig::saveConfig()
{
    DECL_TRACER("TConfig::saveConfig()");

    if (!mSettings)
        return;

    mSettings->setValue("lastDirectory", mLastDirectory);
    mSettings->setValue("position", mPosition);
    mSettings->setValue("logLevel", mLogLevel);
    mSettings->setValue("logFile", mLogFile);
    mSettings->setValue("splitter", mSplitterPosition);
    mSettings->sync();
#ifdef QT_DEBUG
        MSG_DEBUG("Configuration options:" << endl
               << "Last directory:  " << mLastDirectory.toStdString() << endl
               << "Position left:   " << mPosition.left() << endl
               << "Position top:    " << mPosition.top() << endl
               << "Position width:  " << mPosition.width() << endl
               << "Position height: " << mPosition.height() << endl
               << "Log level:       " << mLogLevel.toStdString() << endl
               << "Log file:        " << mLogFile.toStdString() << endl
               << "Splitter pos:    " << mSplitterPosition);
#endif
}

bool TConfig::readConfig()
{
    DECL_TRACER("TConfig::readConfig()");

    if (!mSettings)
    {
        MSG_WARNING("Configuration not initialized!");
        return false;
    }

    mLastDirectory = mSettings->value("lastDirectory").toString();
    mPosition = mSettings->value("position").toRect();
    mLogLevel = mSettings->value("logLevel").toString();
    mLogFile = mSettings->value("logFile").toString();
    mSplitterPosition = mSettings->value("splitter").toInt();

#ifdef QT_DEBUG
    MSG_DEBUG("Configuration options read:" << endl
              << "Last directory:  " << mLastDirectory.toStdString() << endl
              << "Position left:   " << mPosition.left() << endl
              << "Position top:    " << mPosition.top() << endl
              << "Position width:  " << mPosition.width() << endl
              << "Position height: " << mPosition.height() << endl
              << "Log level:       " << mLogLevel.toStdString() << endl
              << "Log file:        " << mLogFile.toStdString() << endl
              << "Splitter pos:    " << mSplitterPosition);
#endif

    if (mPosition.width() == 0)
        return false;

    return true;
}
