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

    // Structure to define access to devices like cameras etc.
    typedef struct RESOURCE_t
    {
        QString name;
        QString protocol;
        QString host;
        QString path;
        QString file;
        QString user;
        QString password;
        int refreshRate{0};
        bool refreshStart{false};
    }RESOURCE_t;

    typedef enum
    {
        DM_UNKOWN,
        DM_LISTVIEW
    }TYPEDATAMAP_t;

    typedef enum
    {
        SORT_NONE,
        SORT_ASC,
        SORT_DESC,
        SORT_ADVANCED
    }SORTMAP_t;

    typedef struct DATASOURCE_t
    {
        QString name;
        QString protocol;       // HTTP, HTTPS
        QString host;
        QString path;
        QString file;
        QString user;
        QString password;
        int refreshRate{0};
        bool force{false};      // True data is forced
        QString format;         // xport-s | csv | csv-headers
        // Datamap
        TYPEDATAMAP_t type{DM_UNKOWN};
        SORTMAP_t sort{SORT_NONE};
        QString mapIdT1;        // Column name?
        QString mapIdT2;        // Column name?
        QString mapIdI1;        // Image to show (optional)
        QStringList sortOrder;
        QString sortQuery;      // Only if sort order = SORT_ADVANCED
        bool live{false};       // Internal use; TRUE = there was a successfull live data request.
        // For CSV specifying
        QString delimiter;      // The delimiter for the columns of a CSV
        bool quoted{false};     // TRUE = The string is quoted with a character
        int headlines{0};       // The number of headlines
    }DATASOURCE_t;

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
        QList<DATASOURCE_t> dataSourceList;
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
        bool readProject(const QString& path);
        void saveProject();
        void reset();
        void removeDynamicImage(const QString& name);
        void removeDynamicData(const QString& name);

        // Setter
        void setDefaultPanelType(TPanelType::PANELTYPE_t pt) { mPanType = pt; }
        void setProjectInfo(const ConfigMain::PROJECTINFO_t& pi);
        void setSetup(const ConfigMain::SETUP_t& setup);
        void setFileName(const QString& fn);
        void setPathTemporary(const QString& path) { mPathTemporary = path; }
        void setFileNameAuto(bool a) { mFileNameAuto = a; }
        void setColorBackground(const QColor& col) { mColorBackground = col; }
        void setColorText(const QColor& col) { mColorText = col; }
        void setFontBase(const QFont& font) { mFontBase = font; };
        void setFontBaseSize(int size) { mFontBaseSize = size; }
        void setDynamicResource(const ConfigMain::RESOURCE_t& res);
        void setDynamicData(const ConfigMain::DATASOURCE_t& data);
        // Getter
        QString getPanelType() { if (mConfMain) return mConfMain->projectInfo.panelType; else return QString(); }
        TPanelType::PANELTYPE_t getDefaultPanelType() { return mPanType; }
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
        int getFontBaseSize() { return mFontBaseSize; }
        QString getFontFile() { if (mConfMain) return mConfMain->fileList.fontFile; else return QString(); }
        QString getPathTemporary() { return mPathTemporary; }
        QString getMapsFile() { if (mConfMain) return mConfMain->fileList.mapFile; else return QString(); }
        ConfigMain::RESOURCE_t getDynamicResource(const QString& name);
        QList<ConfigMain::RESOURCE_t> getAllDynamicResources() { if (mConfMain) return mConfMain->resourceList; else return QList<ConfigMain::RESOURCE_t>(); }
        ConfigMain::DATASOURCE_t getDynamicData(const QString& name);
        QList<ConfigMain::DATASOURCE_t>& getAllDynamicData();
        int getSetupPort() { if (mConfMain) return mConfMain->setup.setupPort; else return 0; }

        bool haveDynamicResource(const QString& name);

    protected:
        void initConfig(bool force=false);

    private:
        static TConfMain *mCurrent;

        ConfigMain::CONFMAIN_t *mConfMain{nullptr};
        QString mFileName;          // The path and file name of the project
        QString mPathTemporary;     // Directory which contains the plain files
        QString mJobName;           // Also called project name
        QString mPageName;          // Main page
        TPanelType::PANELTYPE_t mPanType{TPanelType::PAN_VARIA_SL50};
        QColor mColorBackground{qRgb(0xff, 0xff, 0xff)};
        QColor mColorText{qRgb(0, 0, 0)};
        QFont mFontBase;            // The default font
        int mFontBaseSize{10};      // The default font size
        bool mFileNameAuto{false};  // TRUE = File name is auto generated
};

#endif // TCONFMAIN_H
