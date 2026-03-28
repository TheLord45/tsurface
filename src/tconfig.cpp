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

#include <filesystem>

#include "tconfig.h"
#include "tmisc.h"
#include "terror.h"

using std::endl;
namespace fs = std::filesystem;

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
    mLogLevel = "PROTOCOL";
    char *home = getenv("HOME");
    QString sHome;

    if (home)
        sHome = QString(home);
    else
        sHome = "/";

    bool addDir = false;
    mFilesTemp = QString::fromStdString(fs::temp_directory_path());

    if (fs::exists(QString("%1/%2").arg(sHome).arg("Documents").toStdString()))
    {
        mLastDirectory = sHome + "/Documents/Panels";
        mFilesBackups = sHome + "/Documents";
        addDir = true;
    }
    else if (fs::exists(QString("%1/%2").arg(sHome).arg("Dokumente").toStdString()))
    {
        mLastDirectory = sHome + "/Dokumente/Panels";
        mFilesBackups = sHome + "/Dokumente";
        addDir = true;
    }
    else
        mLastDirectory = sHome;

    mPosition.setLeft(10);
    mPosition.setTop(10);
    mPosition.setWidth(1100);
    mPosition.setHeight(800);
    mSplitterPosition = 260;

    if (addDir)
    {
        fs::create_directories(mLastDirectory.toStdString());
        mFilesPanels = mLastDirectory;
    }

    if (mFilesTemp.isEmpty())
    {
        if (fs::exists("/tmp"))
            mFilesTemp = "/tmp";
        else if (fs::exists("/temp"))
            mFilesTemp = "/temp";
        else
            mFilesTemp = sHome;
    }

    setLogging();
}

void TConfig::setLogging()
{
    if (!mInitialized)
    {
        char *logfile = getenv("TSLOGFILE");

        if (logfile)
        {
            mLogFile = logfile;
            TError::Current()->setLogFile(mLogFile.toStdString());
        }

        char *loglevel = getenv("TSLOGLEVEL");

        if (loglevel)
            mLogLevel = loglevel;

        TError::Current()->setLogLevel(mLogLevel.toStdString());
    }
    else
    {
        if (!mLogFile.isEmpty() && mDebugFile)
            TError::Current()->setLogFile(mLogFile.toStdString());

        int level = 0;

        if (mDebugError)
            level |= HLOG_ERROR;

        if (mDebugWarning)
            level |= HLOG_WARNING;

        if (mDebugInfo)
            level |= HLOG_INFO;

        if (mDebugDebug)
            level |= HLOG_DEBUG;

        if (mDebugTrace)
            level |= HLOG_TRACE;

        mLogLevel = getStrLogLevel(level);
        TError::Current()->setLogLevel(level);
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

    mSettings->beginGroup("Preferences");
    mSettings->setValue("SystemGeneratedName", mSystemGeneratedName);
    mSettings->setValue("ReloadLastWorkspace", mReloadLastWorkspace);;
    mSettings->setValue("ExpandWorkspaceNavigator", mExpandWorkspaceNavigator);
    mSettings->setValue("WarnOnDeletingSources", mWarnOnDeletingSources);
    mSettings->setValue("FlushClosedPages", mFlushClosedPages);
    mSettings->setValue("CreateBackup", mCreateBackup);
    mSettings->setValue("UsePostfix", mUsePostfix);
    mSettings->setValue("RetainSelectedTool", mRetainSelectedTool);
    mSettings->setValue("ImageCacheSize", mImageCacheSize);
    mSettings->setValue("InitialZoom", mInitialZoom);
    mSettings->setValue("VisibleSize", mVisibleSize);
    mSettings->setValue("GutterColor", mGutterColor.name(QColor::HexArgb));
    mSettings->setValue("StackWindows", mStackWindows);
    mSettings->setValue("GridStyle", mGridStyle);
    mSettings->setValue("GridSize", mGridSize);
    mSettings->setValue("SnapTolerance", mSnapTolerance);
    mSettings->setValue("GridColor", mGridColor.name(QColor::HexArgb));
    mSettings->setValue("TransparencyStyle", mTransparencyStyle);
    mSettings->setValue("TransparencySize", mTransparencySize);
    mSettings->setValue("TransparencyCustomColor1", mTransparencyCustomColor1.name(QColor::HexArgb));
    mSettings->setValue("TransparencyCustomColor2", mTransparencyCustomColor2.name(QColor::HexArgb));
    mSettings->setValue("FilesPanels", mFilesPanels);
    mSettings->setValue("FilesBackups", mFilesBackups);
    mSettings->setValue("FilesTemp", mFilesTemp);
    mSettings->setValue("EditorsImage", mEditorsImage);
    mSettings->setValue("EditorsSound", mEditorsSound);
    mSettings->setValue("EditorImageSelected", mEditorImageSelected);
    mSettings->setValue("EditorSoundSelected", mEditorSoundSelected);
    mSettings->setValue("EnableUndoSystem", mEnableUndoSystem);
    mSettings->setValue("UndoLevels", mUndoLevels);
    mSettings->setValue("UndoShowAffectedPages", mUndoShowAffectedPages);
    mSettings->setValue("UndoChangeSelection", mUndoChangeSelection);
    mSettings->setValue("RedoEnableSystem", mRedoEnableSystem);
    mSettings->endGroup();

    mSettings->beginGroup("Debug");
    mSettings->setValue("Errors", mDebugError);
    mSettings->setValue("Warnings", mDebugWarning);
    mSettings->setValue("Infos", mDebugInfo);
    mSettings->setValue("Debug", mDebugDebug);
    mSettings->setValue("Trace", mDebugTrace);
    mSettings->setValue("WriteLogFile", mDebugFile);
    mSettings->setValue("Profile", mLongFormat);
    mSettings->endGroup();

    mSettings->sync();
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

    mSettings->beginGroup("Preferences");
    mSystemGeneratedName = mSettings->value("SystemGeneratedName", false).toBool();
    mReloadLastWorkspace = mSettings->value("ReloadLastWorkspace", false).toBool();
    mExpandWorkspaceNavigator = mSettings->value("ExpandWorkspaceNavigator", true).toBool();
    mWarnOnDeletingSources = mSettings->value("WarnOnDeletingSources", true).toBool();
    mFlushClosedPages = mSettings->value("FlushClosedPages", true).toBool();
    mCreateBackup = mSettings->value("CreateBackup", true).toBool();
    mUsePostfix = mSettings->value("UsePostfix", true).toBool();
    mRetainSelectedTool = mSettings->value("RetainSelectedTool", true).toBool();
    mImageCacheSize = mSettings->value("ImageCacheSize", 8).toInt();
    mInitialZoom = mSettings->value("InitialZoom", 100).toInt();
    mVisibleSize = mSettings->value("VisibleSize", 0.0).toReal();
    mGutterColor = mSettings->value("GutterColor", QColor(qRgb(0, 0, 0)).name(QColor::HexArgb)).toString();
    mStackWindows = mSettings->value("StackWindows", true).toBool();
    mGridStyle = mSettings->value("GridStyle", 1).toInt();
    mGridSize = mSettings->value("GridSize", 8).toInt();
    mSnapTolerance = mSettings->value("SnapTolerance", 8).toInt();
    mGridColor = mSettings->value("GridColor", QColor(Qt::gray).name(QColor::HexArgb)).toString();
    mTransparencyStyle = mSettings->value("TransparencyStyle", 0).toInt();
    mTransparencySize = mSettings->value("TransparencySize", 1).toInt();
    mTransparencyCustomColor1 = mSettings->value("TransparencyCustomColor1", QColor(Qt::white).name(QColor::HexArgb)).toString();
    mTransparencyCustomColor2 = mSettings->value("TransparencyCustomColor2", QColor(Qt::gray).name(QColor::HexArgb)).toString();
    mFilesPanels = mSettings->value("FilesPanels").toString();
    mFilesBackups = mSettings->value("FilesBackups").toString();
    mFilesTemp = mSettings->value("FilesTemp").toString();
    mEditorsImage = mSettings->value("EditorsImage").toStringList();
    mEditorsSound = mSettings->value("EditorsSound").toStringList();
    mEditorImageSelected = mSettings->value("EditorImageSelected").toInt();
    mEditorSoundSelected = mSettings->value("EditorSoundSelected").toInt();
    mEnableUndoSystem = mSettings->value("EnableUndoSystem", false).toBool();
    mUndoLevels = mSettings->value("UndoLevels", 500).toInt();
    mUndoShowAffectedPages = mSettings->value("UndoShowAffectedPages", false).toBool();
    mUndoChangeSelection = mSettings->value("UndoChangeSelection", false).toBool();
    mRedoEnableSystem = mSettings->value("RedoEnableSystem", false).toBool();
    mSettings->endGroup();

    mSettings->beginGroup("Debug");
    mDebugError = mSettings->value("Errors").toBool();
    mDebugWarning = mSettings->value("Warnings").toBool();
    mDebugInfo = mSettings->value("Infos").toBool();
    mDebugDebug = mSettings->value("Debug").toBool();
    mDebugTrace = mSettings->value("Trace").toBool();
    mDebugFile = mSettings->value("WriteLogFile").toBool();
    mLongFormat = mSettings->value("Profile").toBool();
    mSettings->endGroup();

    if (mDebugError && mDebugWarning && mDebugInfo)
        mDebugProtocol = true;
    else
        mDebugProtocol = false;

    int level = 0;
    mLogLevel.clear();

    if (!mDebugProtocol)
    {
        if (mDebugError)
            level |= HLOG_ERROR;

        if (mDebugWarning)
            level |= HLOG_WARNING;

        if (mDebugInfo)
            level |= HLOG_INFO;
    }
    else
        level = HLOG_PROTOCOL;

    if (mDebugDebug)
        level |= HLOG_DEBUG;

    if (mDebugTrace)
        level |= HLOG_TRACE;

    mLogLevel = getStrLogLevel(level);
    std::cout << "Constructed log level: " << mLogLevel.toStdString() << std::endl;

    TError::Current()->setLogFileEnabled(mDebugFile);
    TError::Current()->setLogLevel(level);

    if (mDebugFile && !mLogFile.isEmpty())
        TError::Current()->setLogFile(mLogFile.toStdString());

    if (mPosition.width() == 0)
        return false;

    return true;
}
