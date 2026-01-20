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
#ifndef TCONFIG_H
#define TCONFIG_H

//#include <string>
#include <QString>
#include <QRect>

#define V_MAJOR     1
#define V_MINOR     0
#define V_PATCH     0

#define TSURFACE_VERSION      ((V_MAJOR * 0x10000) + (V_MINOR * 0x100) + V_PATCH)

#define VERSION_STRING() _GET_X_VERSION(V_MAJOR, V_MINOR, V_PATCH)
#define _GET_X_VERSION(a, b, c) _GET_VERSION(a, b, c)
#define _GET_VERSION(a, b, c) ( #a "." #b "." #c )          // Release version

class QSettings;

class TConfig
{
    public:
        TConfig();
        ~TConfig();

        static TConfig& Current();
        void saveConfig();

        QString& getLastDirectory() { return mLastDirectory; }
        void setLastDirectory(const QString& dir) { mLastDirectory = dir; }
        QRect& getLastPosition() { return mPosition; }
        void setLastPosition(const QRect& ws) { mPosition = ws; }
        bool isInitialized() { return mInitialized; }
        bool isLongFormat() { return mLongFormat; }
        QString& getLogFile() { return mLogFile; }
        QString& getLogLevel() { return mLogLevel; }
        bool getProfiling() { return mProfiling; }
        void setProgName(const QString& name) { mProgName = name; }
        QString& getProgName() { return mProgName; }
        int getSplitterPosition() { return mSplitterPosition; }
        void setSplitterPosition(int pos) { mSplitterPosition = pos; }

    protected:
        void init();
        void initDefaults();
        bool readConfig();
        void setLogging();

    private:
        static TConfig *mCurrent;           // Pointer to self.
        QSettings *mSettings{nullptr};      // Abstraction class to store and retrieve basic settings

        QString mProgName;                  // The real name of this program
        bool mInitialized{false};           // TRUE = Configuration was initialized
        QString mLastDirectory;             // The last directory a file was loaded from
        QRect mPosition;                    // The last position and size of the main window
        // Debugging options
        QString mLogFile;                   // An optional logfile
        QString mLogLevel;                  // The loglevels
        bool mLongFormat{false};            // TRUE = Logging is written in long format (recomended)
        bool mProfiling{false};             // TRUE = Profiling information is logged for each method
        int mSplitterPosition{0};           // Remembers the last splitter position
};

#endif // TCONFIG_H
