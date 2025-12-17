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
#ifndef TCONFMAIN_H
#define TCONFMAIN_H

#include <QString>
#include <QDateTime>
#include <QList>

namespace ConfigMain
{
    typedef struct PROJECTINFO_t
    {
        bool protection{false};
        QString password;
        QString panelType;
        QString designer;
        QString company;
        QString copyright;
        QDateTime date;
        QDateTime lastDate;
        QString logLevel;
        QString logFile;
    }PROJECTINFO_t;

    typedef struct FILELIST_t
    {
        QString mapFile;
        QString colorFile;
        QString fontFile;
        QString themeFile;
        QString buttonFile;
        QString appFile;
    }FILELIST_t;

    typedef struct SETUP_t
    {
        int portCount{21};
        int setupPort{0};
        QString powerUpPage;
        QString powerUpPopup;
        QString startupString;
        QString wakeupString;
        QString sleepString;
        QString shutdownString;
        QString idlePage;
        int idleTimeout{0};
        int screenWidth{1920};
        int screenHeight{1024};
        bool screenRotate{false};
        int batteryLevelPort{0};
        int batteryLevelCode{100};
        int marqeeSpeed{1};
    }SETUP_t;

    typedef struct PAGEENTRY_t
    {
        QString name;
        int pageID{0};
        QString file;
        bool isValid{true};
        int popupType{0};
    }PAGEENTRY_t;

    typedef struct RESOURCE_t
    {
        QString name;
        QString protocol;
        QString host;
        QString path;
        QString file;
    }RESOURCE_t;
    typedef struct CONFMAIN_t
    {
        int fileVersion{1};
        PROJECTINFO_t projectInfo;
        FILELIST_t fileList;
        SETUP_t setup;
        QList<PAGEENTRY_t> pageList;
        QList<PAGEENTRY_t> popupList;
        QList<RESOURCE_t> resourceList;
    }CONFMAIN_t;
};

class TConfMain
{
    public:
        TConfMain();
        ~TConfMain();

        static TConfMain& Current();

        void createNew();
        bool readMain(const QString& path);

        // Setter
        void setProjectInfo(const ConfigMain::PROJECTINFO_t& pi);
        void setSetup(const ConfigMain::SETUP_t& setup);
        // Getter

    private:
        static TConfMain *mCurrent;

        ConfigMain::CONFMAIN_t *mConfMain{nullptr};
};

#endif // TCONFMAIN_H
