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
#include <QMessageBox>
#include <QMdiSubWindow>

#include "tsurface.h"
#include "tsurfacereader.h"
#include "tconfmain.h"
#include "tnewprojectdialog.h"
#include "tpaneltypes.h"
#include "tpagetree.h"
#include "tpagehandler.h"
#include "tconfig.h"
#include "terror.h"
#include "ui_tsurface.h"
#include "moc_tsurface.cpp"

namespace fs = std::filesystem;
using std::cout;
using std::endl;

bool winCloseEater::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
        case QEvent::Close:
        {
            MSG_DEBUG("Close event triggered ...");
            QMdiSubWindow *subWindow = dynamic_cast<QMdiSubWindow*>(obj);

            if (subWindow == nullptr)
                return QObject::eventFilter(obj, event);

            // Get widget and substract number fron obhect name
            QWidget *widget = subWindow->widget();
            MSG_DEBUG("Grabbing widget from subwindow ...");

            if (widget == nullptr)
                return QObject::eventFilter(obj, event);

            QString objName = widget->objectName();
            MSG_DEBUG("Got object name: " << objName.toStdString());
            qsizetype pos = objName.lastIndexOf('_');

            if (pos > 0)
            {
                MSG_DEBUG("Found _ at: " << pos);
                int id = objName.remove(0, pos).toInt();
                MSG_DEBUG("ID: " << id);

                if (id > 0)
                    TPageHandler::Current().setVisible(id, false);
            }
        }
        break;

        default:
            qt_noop();
    }

    return QObject::eventFilter(obj, event);
}

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

//
// Menu callback methods
//
/**
 * @brief TSurface::on_actionOpen_triggered
 * This method is triggered when the user pressed in the menu @bold File on the
 * point @bold Open.
 * It shows a file dialog where the user can select a file. If the file is one
 * of the supported formats, then it is opened and the structures are filled.
 */
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

/**
 * @brief TSurface::on_actionNew_triggered
 * Opens a dialog box to enter the data for a new project. If there's
 * already a project open, it will be closed. In case the open project
 * has unsaved state, the user is asked whether it should be saved or
 * not.
 */
void TSurface::on_actionNew_triggered()
{
    DECL_TRACER("TSurface::on_actionNew_triggered()");

    if (mProjectChanged)
    {
        int button = QMessageBox::question(this, tr("Unsaved changes"), tr("<b>There are unsaved changes in this project!</b><br>Do you want to save the project?"));

        if (button == QMessageBox::Yes)
            TConfMain::Current().saveProject();

        m_ui->mdiArea->closeAllSubWindows();
        TConfMain::Current().reset();
        mProjectChanged = false;
    }
    else if (mHaveProject)
    {
        int button = QMessageBox::question(this, tr("Open project"), tr("Do you want to close the current project?"));

        if (button == QMessageBox::No)
            return;

        m_ui->mdiArea->closeAllSubWindows();
        TConfMain::Current().reset();
        mHaveProject = false;
    }

    mPageWidgets.clear();
    TNewProjectDialog npd(this);
    int ret = npd.exec();

    if (ret == QDialog::Rejected)
        return;

    TConfMain& cmain = TConfMain::Current();
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
    // Create tree menu
    TPageTree::Current().createNewTree(m_ui->treeViewPages, npd.getProjectName(), npd.getPageName(), npd.getPanelName());
    connect(&TPageTree::Current(), &TPageTree::clicked, this, &TSurface::onClickedPageTree);
    // Add main page to MDI
    winCloseEater *wce = new winCloseEater;
    QWidget *widget = new QWidget;
    widget->setWindowTitle(npd.getPageName());
    widget->setFixedSize(npd.getResolution());
    widget->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    widget->setStyleSheet("background-color: " + npd.getColorBackground().name() + ";color: " + npd.getColorText().name()+ ";");
    widget->installEventFilter(wce);
    int id = TPageHandler::Current().createPage(widget, Page::PT_PAGE, npd.getPageName(), npd.getResolution().width(), npd.getResolution().height());
    TPageHandler::Current().setPageBgColor(id, npd.getColorBackground());
    TPageHandler::Current().setPageTextColor(id, npd.getColorText());
    QString objName("QWidgetMDI_");
    objName.append(QString::number(id));
    QMdiSubWindow *page = new QMdiSubWindow;
    page->setWidget(widget);
    page->setAttribute(Qt::WA_DeleteOnClose);
    page->installEventFilter(wce);
    m_ui->mdiArea->addSubWindow(page);
    widget->activateWindow();
    widget->show();
    TPageHandler::Current().setVisible(id, true);
    mHaveProject = true;
}

void TSurface::onClickedPageTree(const TPageTree::WINTYPE_t wt, int num, const QString& name)
{
    DECL_TRACER("TSurface::onClickedPageTree(const TPageTree::WINTYPE_t wt, int num, const QString& name)");

    MSG_DEBUG("Toggle: Type: " << wt << ", Number: " << num << ", Name: " << name.toStdString());
    QWidget *widget = TPageHandler::Current().getWidget(num);

    if (!widget)
        return;

    if (TPageHandler::Current().isVisible(num))
    {
        QList<QMdiSubWindow *> swList = m_ui->mdiArea->subWindowList();
        QList<QMdiSubWindow *>::Iterator iter;

        for (iter = swList.begin(); iter != swList.end(); ++iter)
        {
            QMdiSubWindow *sw = *iter;

            if (sw->widget() == widget)
            {
                m_ui->mdiArea->setActiveSubWindow(sw);
                m_ui->mdiArea->closeActiveSubWindow();
                TPageHandler::Current().setVisible(num, false);
                break;
            }
        }
    }
    else
    {
        Page::PAGE_t pg = TPageHandler::Current().getPage(num);
        widget = new QWidget;
        widget->setWindowTitle(name);
        widget->setFixedSize(QSize(pg.width, pg.height));
        widget->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
        widget->setStyleSheet("background-color: " + pg.srPage.cf.name() + ";color: " + pg.srPage.ct.name()+ ";");
        int id = TPageHandler::Current().createPage(widget, Page::PT_PAGE, name, pg.width, pg.height);
        QString objName("QWidgetMDI_");
        objName.append(QString::number(id));
        widget->setObjectName(objName);

        QMdiSubWindow *page = new QMdiSubWindow;
        page->setWidget(widget);
        page->setAttribute(Qt::WA_DeleteOnClose);
        m_ui->mdiArea->addSubWindow(page);
        widget->activateWindow();
        widget->show();
        TPageHandler::Current().setVisible(num, true);
    }
}

void TSurface::onCloseEvent(QCloseEvent *event)
{
    DECL_TRACER("TSurface::onCloseEvent(QCloseEvent *event)");

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

