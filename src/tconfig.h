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

#include <QString>
#include <QRect>
#include <QColor>

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

        bool getSystemGeneratedName() { return mSystemGeneratedName; }
        void setSystemGeneratedName(bool name) { mSystemGeneratedName = name; }
        bool getReloadLastWorkspace() { return mReloadLastWorkspace; }
        void setReloadLastWorkspace(bool ws) { mReloadLastWorkspace = ws; }
        bool getExpandWorkspaceNavigator() { return mExpandWorkspaceNavigator; }
        void setExpandWorkspaceNavigator(bool navi) { mExpandWorkspaceNavigator = navi; }
        bool getWarnOnDeletingSources() { return mWarnOnDeletingSources; }
        void setWarnOnDeletingSources(bool s) { mWarnOnDeletingSources = s; }
        bool getFlushClosedPages() { return mFlushClosedPages; }
        void setFlushClosedPages(bool p) { mFlushClosedPages = p; }
        bool getCreateBackup() { return mCreateBackup; }
        void setCreateBackup(bool b) { mCreateBackup = b; }
        bool getUsePostfix() { return mUsePostfix; }
        void setUsePostfix(bool p) { mUsePostfix = p; }
        bool getRetainSelectedTool() { return mRetainSelectedTool; }
        void setRetainSelectedTool(bool t) { mRetainSelectedTool = t; }
        int getImageCacheSize() { return mImageCacheSize; }
        void setImageCacheSize(int i) { mImageCacheSize = i; }

        int getInitialZoom() { return mInitialZoom; }
        void setInitialZoom(int zoom) { mInitialZoom = zoom; }
        qreal getVisibleSize() { return mVisibleSize; }
        void setVisibleSize(qreal size) { mVisibleSize = size; }
        QColor getGutterColor() { return mGutterColor; }
        void setGutterColor(const QColor& col) { mGutterColor = col; }
        bool getStackWindows() { return mStackWindows; }
        void setStackWindows(bool stack) { mStackWindows = stack; }
        int getGridStyle() { return mGridStyle; }
        void setGridStyle(int style) { mGridStyle = style; }
        int getGridSize() { return mGridSize; }
        void setGridSize(int size) { mGridSize = size; }
        int getSnapTolerance() { return mSnapTolerance; }
        void setSnapTolerance(int t) { mSnapTolerance = t; }
        QColor getGridColor() { return mGridColor; }
        void setGridColor(const QColor& col) { mGridColor = col; }
        int getTransparencyStyle() { return mTransparencyStyle; }
        void setTransparencyStyle(int style) { mTransparencyStyle = style; }
        int getTransparencySize() { return mTransparencySize; }
        void setTransparencySize(int t) { mTransparencySize = t; }
        QColor getTransparencyCustomColor1() { return mTransparencyCustomColor1; }
        void setTransparencyCustomColor1(const QColor& col) { mTransparencyCustomColor1 = col; }
        QColor getTransparencyCustomColor2() { return mTransparencyCustomColor2; }
        void setTransparencyCustomColor2(const QColor& col) { mTransparencyCustomColor2 = col; }

        QString& getFilesPanels() { return mFilesPanels; }
        void setFilesPanels(const QString& file) { mFilesPanels = file; }
        QString& getFilesBackups() { return mFilesBackups; }
        void setFilesBackups(const QString& file) { mFilesBackups = file; }
        QString& getFilesTemp() { return mFilesTemp; }
        void setFilesTemp(const QString& file) { mFilesTemp = file; }

        QStringList& getEditorsImage() { return mEditorsImage; }
        void setEditorsImage(const QStringList& list) { mEditorsImage = list; }
        int getEditorImageSelected() { return mEditorImageSelected; }
        void setEditorImageSelected(int idx) { mEditorImageSelected = idx; }
        QStringList& getEditorsSound() { return mEditorsSound; }
        void setEditorsSound(const QStringList& list) { mEditorsSound = list; }
        int getEditorSoundSelected() { return mEditorSoundSelected; }
        void setEditorSoundSelected(int idx) { mEditorSoundSelected = idx; }

        bool getEnableUndoSystem() { return mEnableUndoSystem; }
        void setEnableUndoSystem(bool undo) { mEnableUndoSystem = undo; }
        int getUndoLevels() { return mUndoLevels; }
        void setUndoLevels(int level) { mUndoLevels = level; }
        bool getUndoShowAffectedPages() { return mUndoShowAffectedPages; }
        void setUndoShowAffectedPages(bool b) { mUndoShowAffectedPages = b; }
        bool getUndoChangeSelection() { return mUndoChangeSelection; }
        void setUndoChangeSelection(bool b) { mUndoChangeSelection = b; }
        bool getRedoEnableSystem() { return mRedoEnableSystem; }
        void setRedoEnableSystem(bool b) { mRedoEnableSystem = b; }

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
        // Preferences: Application
        bool mSystemGeneratedName{false};
        bool mReloadLastWorkspace{false};
        bool mExpandWorkspaceNavigator{true};
        bool mWarnOnDeletingSources{true};
        bool mFlushClosedPages{true};
        bool mCreateBackup{true};
        bool mUsePostfix{true};
        bool mRetainSelectedTool{true};
        qsizetype mImageCacheSize{8};       // Mib
        // Preferences: Appearance
        int mInitialZoom{100};              // Percent
        qreal mVisibleSize{0.0};            // Inches
        QColor mGutterColor{qRgb(0, 0, 0)};
        bool mStackWindows{true};
        int mGridStyle{1};
        int mGridSize{8};
        int mSnapTolerance{8};
        QColor mGridColor{Qt::gray};
        int mTransparencyStyle{0};
        int mTransparencySize{1};
        QColor mTransparencyCustomColor1{Qt::white};
        QColor mTransparencyCustomColor2{Qt::gray};
        // Preferences: Directories
        QString mFilesPanels;
        QString mFilesBackups;
        QString mFilesTemp;
        // Preferences: Editors
        QStringList mEditorsImage;
        QStringList mEditorsSound;
        int mEditorImageSelected{0};
        int mEditorSoundSelected{0};
        // Preferences: Undo/Redo
        bool mEnableUndoSystem{false};
        int mUndoLevels{500};
        bool mUndoShowAffectedPages{false};
        bool mUndoChangeSelection{false};
        bool mRedoEnableSystem{false};
};

#endif // TCONFIG_H
