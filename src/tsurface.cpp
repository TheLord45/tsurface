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
#include <filesystem>

#include <QFileDialog>
#include <QResizeEvent>

#include "tsurface.h"
#include "tsurfacereader.h"
#include "tconfmain.h"
#include "tnewprojectdialog.h"
#include "tpaneltypes.h"
#include "tconfig.h"
#include "terror.h"
#include "ui_tsurface.h"
#include "moc_tsurface.cpp"

namespace fs = std::filesystem;
using std::cout;
using std::endl;

TSurface::TSurface(QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::tsurface)
{
    DECL_TRACER("TSurface::TSurface(QWidget *parent)");

    m_ui->setupUi(this);
    // Get the last position and size of the main window and set it.
    TConfig::WINSIZE_t ws = TConfig::Current().getLastPosition();
    setGeometry(QRect(ws.left, ws.top, ws.width, ws.height));
}

TSurface::~TSurface()
{
    DECL_TRACER("TSurface::~TSurface()");

    TConfig::WINSIZE_t ws;
    ws.left = geometry().x();
    ws.top = geometry().y();
    ws.width = geometry().width();
    ws.height = geometry().height();
    TConfig::Current().setLastPosition(ws);
    TConfig::Current().saveConfig();
}

void TSurface::on_actionOpen_triggered()
{
    DECL_TRACER("TSurface::on_actionOpen_triggered()");

    mLastOpenPath = QString::fromStdString(TConfig::Current().getLastDirectory());
    QString file = QFileDialog::getOpenFileName(this, tr("Open Logfile"), mLastOpenPath, tr("Surface (*.tsf);;AMX (*.TP4 *.TP5);;All (*)"));
    qsizetype pos = file.lastIndexOf("/");

    if (pos > 0)
        TConfig::Current().setLastDirectory(file.left(pos).toStdString());

    if (file.isEmpty())
        return;

    if (fs::exists(file.toStdString()) && fs::is_regular_file(file.toStdString()))
    {
        TSurfaceReader sreader(file.toStdString());
    }
}

void TSurface::on_actionNew_triggered()
{
    DECL_TRACER("TSurface::on_actionNew_triggered()");

    TNewProjectDialog npd(this);
    int ret = npd.exec();

    if (ret == QDialog::Rejected)
        return;

    TConfMain& cmain = TConfMain::Current();
    cmain.setTreeView(m_ui->treeViewPages);
    cmain.createNew(npd.getFileName(), npd.getPageName());
    ConfigMain::PROJECTINFO_t projectInfo;
    TPanelType tpType;
    projectInfo.panelType = tpType.getPanelName(npd.getPanelType());
    projectInfo.comment = npd.getComment();
    projectInfo.company = "TheoSys";
    projectInfo.dealer = npd.getDealer();
    projectInfo.copyright = "(C) by TheoSys";
    projectInfo.date = QDateTime::currentDateTime();
    projectInfo.lastDate = QDateTime::currentDateTime();
    projectInfo.revision = npd.getRevision();
    projectInfo.logLevel = "PROTOCOL";
    projectInfo.protection = false;
    cmain.setProjectInfo(projectInfo);
}

void TSurface::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QSize statusbar = m_ui->statusBar->size();
    QSize menubar = m_ui->menuBar->size();
    QSize toolbar = m_ui->mainToolBar->size();
    QSize size(event->size().width(), event->size().height() - statusbar.height() - menubar.height() - toolbar.height());
    m_ui->splitter->resize(size);
}

