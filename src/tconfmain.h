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
#ifndef TCONFMAIN_H
#define TCONFMAIN_H

#include <QString>
#include <QDateTime>
#include <QList>
#include <QColor>
#include <QFont>

#include "tpaneltypes.h"

namespace ConfigMain
{
    typedef struct PROJECTINFO_t
    {
        bool protection{false};
        QString password;
        QString jobName;
        QString panelType;
        QSize panelSize;
        QString revision;
        QString designer;
        QString dealer;
        QString company;
        QString copyright;
        QString comment;
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
        QList<QString> powerUpPopups;
        QString startupString;
        QString wakeupString;
        QString sleepString;
        QString shutdownString;
        QString idlePage;
        QString inactivityPage;
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
        PANELTYPE_t popupType{PN_UNDEFINED};
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
        QString fileName;
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

        void createNew(const QString& file, const QString& pname, const QString& project);
        void addPage(const QString& name, int num);
        void addPopup(const QString& name, int num);
        void renamePage(int num, const QString& name);
        void renamePopup(int num, const QString& name);
        void deletePage(const QString& name);
        void deletePopup(const QString& name);
        bool readMain(const QString& path);
        void saveProject();
        void reset();

        // Setter
        void setProjectInfo(const ConfigMain::PROJECTINFO_t& pi);
        void setSetup(const ConfigMain::SETUP_t& setup);
        void setFileName(const QString& fn);
        void setFileNameAuto(bool a) { mFileNameAuto = a; }
        void setColorBackground(const QColor& col) { mColorBackground = col; }
        void setColorText(const QColor& col) { mColorText = col; }
        void setFontBase(const QFont& font);
        // Getter
        QString getPanelType() { if (mConfMain) return mConfMain->projectInfo.panelType; else return QString(); }
        QString getJobName() { if (mConfMain) return mConfMain->projectInfo.jobName; else return QString(); }
        QString getFileName();
        QSize getPanelSize();
        ConfigMain::PROJECTINFO_t getProjectInfo() { if (mConfMain) return mConfMain->projectInfo; else return ConfigMain::PROJECTINFO_t(); }
        bool getFileNameAuto() { return mFileNameAuto; }
        ConfigMain::SETUP_t getSetup() { if (mConfMain) return mConfMain->setup; else return ConfigMain::SETUP_t(); }
        QList<QString> getAllPages();
        QList<QString> getAllPopups();
        QColor& getColorBackground() { return mColorBackground; }
        QColor& getColorText() { return mColorText; }
        QFont& getFontBase() { return mFontBase; }

    private:
        static TConfMain *mCurrent;

        ConfigMain::CONFMAIN_t *mConfMain{nullptr};
        QString mFileName;
        QString mJobName;
        QString mPageName;
        QString mBaseDirectory;     // Directory which contains the plain files
        TPanelType::PANELTYPE_t mPanType{TPanelType::PAN_VARIA_SL50};
        QColor mColorBackground{qRgb(0xff, 0xff, 0xff)};
        QColor mColorText{qRgb(0, 0, 0)};
        QFont mFontBase;
        bool mFileNameAuto{false};
};

#endif // TCONFMAIN_H
