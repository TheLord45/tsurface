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
#include <fstream>
#include <iomanip>
#include <filesystem>

#include "tconfig.h"
#include "terror.h"

#define CONFIGFILE  "tsurface.conf"

namespace  fs = std::filesystem;
using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::cout;
using std::endl;
using std::to_string;
using std::to_integer;

TConfig *TConfig::mCurrent = nullptr;

TConfig::TConfig()
{
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
    // Search for configuration file
    char *home = getenv("HOME");
    string sHome;

    if (home)
        sHome = string(home);
    else
        sHome = "/";

    string path = sHome + "/.config";
    mInitialized = true;

    try
    {
        if (!fs::exists(path))
        {
            fs::create_directories(path);
            saveConfig();
            return;
        }

        if (!readConfig())
        {
            initDefaults();
            saveConfig();
        }
        else
            setLogging();
    }
    catch(std::exception& e)
    {
        MSG_ERROR("Config error: " << e.what());
        return;
    }
}

void TConfig::initDefaults()
{
    char *home = getenv("HOME");
    string sHome;

    if (home)
        sHome = string(home);
    else
        sHome = "/";

    mConfigFile = sHome + "/.config/" + CONFIGFILE;
    mLastDirectory = sHome;
    mPosition.left = 10;
    mPosition.top = 10;
    mPosition.width = 1100;
    mPosition.height = 800;

    setLogging();
}

void TConfig::setLogging()
{
    mLogLevel = "PROTOCOL";
    char *logfile = getenv("TSLOGFILE");

    if (logfile)
    {
        mLogFile = logfile;
        TStreamError::setLogFile(mLogFile);
    }

    char *loglevel = getenv("TSLOGLEVEL");

    if (loglevel)
    {
        mLogLevel = loglevel;
        TStreamError::setLogLevel(mLogLevel);
    }
    else
        TStreamError::setLogLevel(mLogLevel);
}

void TConfig::saveConfig()
{
    DECL_TRACER("TConfig::saveConfig()");

    ofstream file;

    try
    {
        file.open(mConfigFile, ios::trunc | ios::out);
        MSG_DEBUG("Last directory:  " << mLastDirectory << endl
               << "Position left:   " << to_string(mPosition.left) << endl
               << "Position top:    " << to_string(mPosition.top) << endl
               << "Position width:  " << to_string(mPosition.width) << endl
               << "Position height: " << to_string(mPosition.height) << endl
               << "Log level:       " << mLogLevel << endl
               << "Log file:        " << mLogFile);

        file << mLastDirectory << endl
             << to_string(mPosition.left) << endl
             << to_string(mPosition.top) << endl
             << to_string(mPosition.width) << endl
             << to_string(mPosition.height) << endl
             << mLogLevel << endl
             << mLogFile << endl;
        file.close();
    }
    catch (std::exception& e)
    {
        MSG_ERROR("Error writing config file: " << e.what());

        if (file.is_open())
            file.close();
    }
}

bool TConfig::readConfig()
{
    DECL_TRACER("TConfig::readConfig()");

    ifstream file;

    try
    {
        file.open(mConfigFile, ios::in);
        int zeile = 0;

        for (string line; getline(file, line);)
        {
            zeile++;

            switch(zeile)
            {
                case 1: mLastDirectory   = line; break;
                case 2: mPosition.left   = atoi(line.c_str()); break;
                case 3: mPosition.top    = atoi(line.c_str()); break;
                case 4: mPosition.width  = atoi(line.c_str()); break;
                case 5: mPosition.height = atoi(line.c_str()); break;
                case 6: mLogLevel        = line; break;
                case 7: mLogFile         = line; break;
            }
        }

        file.close();
    }
    catch(std::exception& e)
    {
        MSG_ERROR("Error reading config file: " << e.what());

        if (file.is_open())
            file.close();

        return false;
    }

    return true;
}
