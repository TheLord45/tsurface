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
#include <filesystem>

#include <QFileDialog>
#include <QResizeEvent>
#include <QMessageBox>
#include <QMdiSubWindow>

#include "tsurface.h"
#include "tsurfacereader.h"
#include "tconfmain.h"
#include "tnewprojectdialog.h"
#include "tprojectproperties.h"
#include "tpaneltypes.h"
#include "tpagetree.h"
#include "tpagehandler.h"
#include "taddpagedialog.h"
#include "taddpopupdialog.h"
#include "tfonts.h"
#include "tconfig.h"
#include "terror.h"
#include "ui_tsurface.h"
#include "moc_tsurface.cpp"

namespace fs = std::filesystem;
using std::cout;
using std::endl;

/**
 * @brief winCloseEater::eventFilter
 * This is a class to catch the close event of an MDI window. The MDI window
 * is identified by it's object name. From it the window number is extracted
 * and used to mark the window closed.
 * Every close of the window means that it is deleted from the MDI space. To
 * display it again it must be completely redefined.
 *
 * @param obj       The pointer to the object received a close event
 * @param event     The type of the event. We catch only the close event.
 *
 * @return The event is forwarded to the original object so that Qt cat do
 * its work. The result of this forwarding is returned.
 */
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
                int id = objName.remove(0, pos+1).toInt();
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
    mCloseEater = new winCloseEater;
    // Add the application icon
    setWindowIcon(QIcon(":images/tsurface_512.png"));
    setUnifiedTitleAndToolBarOnMac(true);
    // Add symbols to the toolbar
    mActionStateManager = new QAction(QIcon(":images/statemanager.png"), tr("State manager"));
    m_ui->mainToolBar->addAction(mActionStateManager);

    mActionPlay = new QAction(QIcon(":images/play_screen.png"), tr("Play"));
    m_ui->mainToolBar->addAction(mActionPlay);

    mActionSearch = new QAction(QIcon(":images/search_screen.png"), tr("Search"));
    m_ui->mainToolBar->addAction(mActionSearch);

    mActionChannels = new QAction(QIcon(":images/show_channels.png"), tr("Hide channels"));
    connect(mActionChannels, &QAction::triggered, this, &TSurface::onActionShowChannels);
    mToggleChannels = true;     // Mark channels as visible
    m_ui->mainToolBar->addAction(mActionChannels);

    m_ui->mainToolBar->addSeparator();

    mActionConnectionState = new QAction(QIcon(":images/disconnected.svg"), tr("Disconnected"));
    m_ui->mainToolBar->addAction(mActionConnectionState);

    connect(&TPageTree::Current(), &TPageTree::addNewPage, this, &TSurface::onAddNewPage);
    connect(&TPageTree::Current(), &TPageTree::addNewPopup, this, &TSurface::onAddNewPopup);
    connect(&TPageTree::Current(), &TPageTree::toFront, this, &TSurface::onItemToFront);
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

    if (mCloseEater)
        delete mCloseEater;
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
    ConfigMain::SETUP_t setupInfo;
    TPanelType tpType;
    projectInfo.jobName = npd.getProjectName();
    projectInfo.panelType = npd.getPanelName();
    projectInfo.panelSize = npd.getResolution();
    projectInfo.comment = npd.getComment();
    projectInfo.company = "TheoSys";
    projectInfo.dealer = npd.getDealer();
    projectInfo.designer = npd.getDesigner();
    projectInfo.copyright = "(C) by TheoSys";
    projectInfo.date = QDateTime::currentDateTime();
    projectInfo.lastDate = QDateTime::currentDateTime();
    projectInfo.revision = npd.getRevision();
    projectInfo.logLevel = "PROTOCOL";
    projectInfo.protection = false;
    setupInfo.screenHeight = projectInfo.panelSize.height();
    setupInfo.screenWidth = projectInfo.panelSize.width();
    // The following settings are the base settings for this project.
    cmain.createNew(npd.getFileName(), npd.getPageName(), npd.getProjectName());
    cmain.setProjectInfo(projectInfo);
    cmain.setFileNameAuto(npd.getFileNameAuto());
    cmain.setColorBackground(npd.getColorBackground());
    cmain.setColorText(npd.getColorText());
    cmain.setFontBase(npd.getFontName());
    cmain.setFontBaseSize(npd.getFontSize());
    // Create tree menu
    TPageTree::Current().setParent(this);
    TPageTree::Current().createNewTree(m_ui->treeViewPages, npd.getProjectName(), npd.getPageName(), npd.getPanelName());
    connect(&TPageTree::Current(), &TPageTree::clicked, this, &TSurface::onClickedPageTree);
    // Add main page to MDI
    QWidget *widget = new QWidget;
    widget->setWindowTitle(npd.getPageName());
    widget->setFixedSize(npd.getResolution());
    widget->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    widget->setStyleSheet("background-color: " + npd.getColorBackground().name() + ";color: " + npd.getColorText().name()+ ";");
    widget->installEventFilter(mCloseEater);
    int id = TPageHandler::Current().createPage(widget, Page::PT_PAGE, npd.getPageName(), npd.getResolution().width(), npd.getResolution().height());
    TPageHandler::Current().setPageBgColor(id, npd.getColorBackground());
    TPageHandler::Current().setPageTextColor(id, npd.getColorText());
    QString objName("QWidgetMDI_");
    objName.append(QString::number(id));
    widget->setObjectName(objName);
    QMdiSubWindow *page = new QMdiSubWindow;
    page->setWidget(widget);
    page->setAttribute(Qt::WA_DeleteOnClose);
    page->installEventFilter(mCloseEater);
    page->setWindowIcon(QIcon(":images/tsurface_512.png"));
    m_ui->mdiArea->addSubWindow(page);
    widget->activateWindow();
    widget->show();
    TPageHandler::Current().setVisible(id, true);
    mHaveProject = true;
}

void TSurface::on_actionProject_properties_triggered()
{
    DECL_TRACER("TSurface::on_actionProject_properties_triggered()");

    TProjectProperties propDialog(this);
    TPanelType panType;
    QSize size = TConfMain::Current().getPanelSize();
    QString panelType = TConfMain::Current().getPanelType();
    MSG_DEBUG("Stored panel: " << panelType.toStdString());
    QString panelImage = panType.getImageName(panelType);
    ConfigMain::PROJECTINFO_t projectInfo = TConfMain::Current().getProjectInfo();
    ConfigMain::SETUP_t setupInfo = TConfMain::Current().getSetup();
    MSG_DEBUG("Panel name: " << panelType.toStdString() << ", Panel image: " << panelImage.toStdString() << ", Panel size: " << size.width() << "x" << size.height());
    propDialog.setPanelImage(QPixmap(QString(":images/%1").arg(panelImage)));
    propDialog.setPanelSize(size);
    propDialog.setPanelType(panelType);
    propDialog.setJobName(projectInfo.jobName);
    propDialog.setDealerID(projectInfo.dealer);
    propDialog.setDesignerID(projectInfo.designer);
    propDialog.setJobComments(projectInfo.comment);
    propDialog.setFileRevision(projectInfo.revision);
    propDialog.setSystemFileName(TConfMain::Current().getFileNameAuto());
    propDialog.setStartup(setupInfo.startupString);
    propDialog.setWakeup(setupInfo.wakeupString);
    propDialog.setSleep(setupInfo.sleepString);
    propDialog.setShutown(setupInfo.shutdownString);
    propDialog.setStandby(setupInfo.idlePage);
    propDialog.setInactivityPage(setupInfo.inactivityPage);
    propDialog.setPowerUpPopups(setupInfo.powerUpPopups);
    propDialog.setTimeCreation(projectInfo.date);
    propDialog.setTimeLastChange(projectInfo.lastDate);
    propDialog.setFileName(TConfMain::Current().getFileName());

    if (propDialog.exec() == QDialog::Rejected)
        return;

    projectInfo.comment = propDialog.getJobComments();
    projectInfo.dealer = propDialog.getDealerID();
    projectInfo.designer = propDialog.getDesignerID();
    projectInfo.jobName = propDialog.getJobName();
    projectInfo.revision = propDialog.getFileRevision();
    projectInfo.lastDate = QDateTime::currentDateTime();
    setupInfo.idlePage = propDialog.getStandby();
    setupInfo.inactivityPage = propDialog.getInactivityPage();
    setupInfo.powerUpPage = propDialog.getPowerUpPage();
    setupInfo.powerUpPopups = propDialog.getPopwerUpPopups();
    setupInfo.shutdownString = propDialog.getShutdown();
    setupInfo.sleepString = propDialog.getSleep();
    setupInfo.startupString = propDialog.getStartup();
    setupInfo.wakeupString = propDialog.getWakeup();

    TConfMain::Current().setFileNameAuto(propDialog.getSystemFileName());
    TConfMain::Current().setProjectInfo(projectInfo);
    TConfMain::Current().setSetup(setupInfo);
}

//
// Other callbacks
//
void TSurface::onItemToFront(int id)
{
    DECL_TRACER("TSurface::onItemToFront(int id)");

    TPageHandler::Current().bringToFront(id);
}

/**
 * @brief TSurface::onActionShowChannels
 * This method is triggered by a click on the channels icon in the toolbar.
 * It toggles the icon, and with it the function, from showing the channels
 * on each object and hiding them. The icon presentation and the tool tip
 * text is toggled also.
 *
 * @param checked   This parameter is not used.
 */
void TSurface::onActionShowChannels(bool checked)
{
    DECL_TRACER("TSurface::onActionShowChannels(bool checked)");

    Q_UNUSED(checked);
    mToggleChannels = !mToggleChannels;

    if (mToggleChannels)
    {
        mActionChannels->setIcon(QIcon(":images/show_channels.png"));
        mActionChannels->setText(tr("Hide channels"));
    }
    else
    {
        mActionChannels->setIcon(QIcon(":images/hide_channels.png"));
        mActionChannels->setText(tr("Show channels"));
    }
}

/**
 * @brief TSurface::onClickedPageTree
 * This method is triggered on a double click on an item in the page tree.
 * It toggles the visible page or popup window between visible and closed.
 *
 * @param wt        The type of the window
 * @param num       The number of the window
 * @param name      The name of the window. This is used to show the name
 * in the title bar of a visible window.
 */
void TSurface::onClickedPageTree(const TPageTree::WINTYPE_t wt, int num, const QString& name)
{
    DECL_TRACER("TSurface::onClickedPageTree(const TPageTree::WINTYPE_t wt, int num, const QString& name)");

    MSG_DEBUG("Toggle: Type: " << wt << ", Number: " << num << ", Name: " << name.toStdString());
    QWidget *widget = TPageHandler::Current().getWidget(num);

    if (!widget)
        return;

    if (TPageHandler::Current().isVisible(num))
    {
        MSG_DEBUG("Window is visible. Closing it ...");
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
        MSG_DEBUG("Window is not visible. Generating it ...")
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
        widget->installEventFilter(mCloseEater);
        MSG_DEBUG("Object name: " << objName.toStdString());

        QMdiSubWindow *page = new QMdiSubWindow;
        page->setWidget(widget);
        page->setAttribute(Qt::WA_DeleteOnClose);
        page->installEventFilter(mCloseEater);
        page->setWindowIcon(QIcon(":images/tsurface_512.png"));
        m_ui->mdiArea->addSubWindow(page);
        widget->activateWindow();
        widget->show();
        TPageHandler::Current().setVisible(num, true);
    }
}

void TSurface::onAddNewPage()
{
    DECL_TRACER("TSurface::onAddNewPage()");

    TAddPageDialog pageDialog(this);
    ConfigMain::PROJECTINFO_t prjInfo = TConfMain::Current().getProjectInfo();
    pageDialog.setFont(TConfMain::Current().getFontBase());
    pageDialog.setFontSize(TConfMain::Current().getFontBaseSize());
    pageDialog.setColorBackground(TConfMain::Current().getColorBackground());
    pageDialog.setColorText(TConfMain::Current().getColorText());

    if (pageDialog.exec() == QDialog::Rejected)
        return;

    mProjectChanged = true;
    QSize pgSize = TConfMain::Current().getPanelSize();
    QWidget *widget = new QWidget;
    widget->setWindowTitle(pageDialog.getPageName());
    widget->setFixedSize(QSize(pgSize.width(), pgSize.height()));
    widget->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    widget->setStyleSheet("background-color: " + pageDialog.getColorBackground().name() + ";color: " + pageDialog.getColorText().name()+ ";");
    int id = TPageHandler::Current().createPage(widget, Page::PT_PAGE, pageDialog.getPageName(), pgSize.width(), pgSize.height());
    QString objName("QWidgetMDI_");
    objName.append(QString::number(id));
    widget->setObjectName(objName);
    widget->installEventFilter(mCloseEater);

    QMdiSubWindow *page = new QMdiSubWindow;
    page->setWidget(widget);
    page->setAttribute(Qt::WA_DeleteOnClose);
    page->installEventFilter(mCloseEater);
    page->setWindowIcon(QIcon(":images/tsurface_512.png"));
    m_ui->mdiArea->addSubWindow(page);
    widget->activateWindow();
    widget->show();
    TPageHandler::Current().setVisible(id, true);
    TPageTree::Current().addPage(pageDialog.getPageName(), id);
    TConfMain::Current().addPage(pageDialog.getPageName(), id);
    // Here we set everyting of the page we currently know
    Page::PAGE_t pg = TPageHandler::Current().getPage(id);

    if (pg.pageID <= 0)
    {
        MSG_ERROR("Error getting whole page!");
        return;
    }

    pg.srPage.cf = pageDialog.getColorBackground();
    pg.srPage.ct = pageDialog.getColorText();
    QFont font = pageDialog.getFont();
    TFonts::getFontPath(font.family());
    TPageHandler::Current().setPage(pg);
}

void TSurface::onAddNewPopup()
{
    DECL_TRACER("TSurface::onAddNewPopup()");

    TAddPopupDialog popupDialog(this);
    ConfigMain::PROJECTINFO_t prjInfo = TConfMain::Current().getProjectInfo();
    popupDialog.setFont(TConfMain::Current().getFontBase());
    popupDialog.setFontSize(TConfMain::Current().getFontBaseSize());
    popupDialog.setColorPageBackground(TConfMain::Current().getColorBackground());
    popupDialog.setColorText(TConfMain::Current().getColorText());
    popupDialog.setPageSize(TConfMain::Current().getPanelSize());
    popupDialog.setGroupNames(TPageHandler::Current().getGroupNames());

    if (popupDialog.exec() == QDialog::Rejected)
        return;

    mProjectChanged = true;
    QSize pgSize = popupDialog.getPopupSize();
    QWidget *widget = new QWidget;
    widget->setWindowTitle(popupDialog.getPopupName());
    widget->setFixedSize(QSize(pgSize.width(), pgSize.height()));
    widget->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    widget->setStyleSheet("background-color: " + popupDialog.getColorPageBackground().name() + ";color: " + popupDialog.getColorText().name()+ ";");
    QRect geom(popupDialog.getPositionLeft(), popupDialog.getPositionTop(), popupDialog.getSizeWidth(), popupDialog.getSizeHeight());
    int id = TPageHandler::Current().createPage(widget, Page::PT_POPUP, popupDialog.getPopupName(), geom);
    QString objName("QWidgetMDI_");
    objName.append(QString::number(id));
    widget->setObjectName(objName);
    widget->installEventFilter(mCloseEater);

    QMdiSubWindow *page = new QMdiSubWindow;
    page->setWidget(widget);
    page->setAttribute(Qt::WA_DeleteOnClose);
    page->installEventFilter(mCloseEater);
    page->setWindowIcon(QIcon(":images/tsurface_512.png"));
    m_ui->mdiArea->addSubWindow(page);
    widget->activateWindow();
    widget->show();
    TPageHandler::Current().setVisible(id, true);
    TPageTree::Current().addPopup(popupDialog.getPopupName(), id);
    TConfMain::Current().addPopup(popupDialog.getPopupName(), id);
    // Here we set everyting of the page we currently know
    Page::PAGE_t pg = TPageHandler::Current().getPage(id);

    if (pg.pageID <= 0)
    {
        MSG_ERROR("Error getting whole page!");
        return;
    }

    pg.srPage.cf = popupDialog.getColorPageBackground();
    pg.srPage.ct = popupDialog.getColorText();
    pg.srPage.cb = popupDialog.getColorBorder();
//    QFont font = popupDialog.getFont();
//    TFonts::getFontPath(font.family());
    TPageHandler::Current().setPage(pg);
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

