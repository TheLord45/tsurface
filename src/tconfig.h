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
#ifndef TCONFIG_H
#define TCONFIG_H

#include <string>
#include <vector>

#define V_MAJOR     1
#define V_MINOR     0
#define V_PATCH     0

#define TSURFACE_VERSION      ((V_MAJOR * 0x10000) + (V_MINOR * 0x100) + V_PATCH)

#define VERSION_STRING() _GET_X_VERSION(V_MAJOR, V_MINOR, V_PATCH)
#define _GET_X_VERSION(a, b, c) _GET_VERSION(a, b, c)
#define _GET_VERSION(a, b, c) ( #a "." #b "." #c )          // Release version

class TConfig
{
    public:
        TConfig();

        static TConfig& Current();
        void saveConfig();

        typedef struct WINSIZE_t
        {
            int left{0};
            int top{0};
            int width{600};
            int height{400};
        }WINSIZE_t;

        std::string& getLastDirectory() { return mLastDirectory; }
        void setLastDirectory(const std::string& dir) { mLastDirectory = dir; }
        WINSIZE_t& getLastPosition() { return mPosition; }
        void setLastPosition(const WINSIZE_t& ws) { mPosition = ws; }
        bool isInitialized() { return mInitialized; }
        bool isLongFormat() { return mLongFormat; }
        std::string getLogFile() { return mLogFile; }
        std::string getLogLevel() { return mLogLevel; }
        bool getProfiling() { return mProfiling; }
        void setProgName(const std::string& name) { mProgName = name; }
        std::string& getProgName() { return mProgName; }

    protected:
        void init();
        void initDefaults();
        bool readConfig();
        void setLogging();

    private:
        static TConfig *mCurrent;           // Pointer to self.

        std::string mProgName;              // The real name of this program
        bool mInitialized{false};           // TRUE = Configuration was initialized
        std::string mConfigFile;            // The path and name of the configuration file
        std::string mLastDirectory;         // The last directory a file was loaded from
        WINSIZE_t mPosition;                // The last position and size of the main window
        // Debugging options
        std::string mLogFile;               // An optional logfile
        std::string mLogLevel;              // The loglevels
        bool mLongFormat{false};            // TRUE = Logging is written in long format (recomended)
        bool mProfiling{false};             // TRUE = Profiling information is logged for each method
};

#endif // TCONFIG_H
