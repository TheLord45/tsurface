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
#include <QtEnvironmentVariables>

#include "tsurface.h"
#include "tsurfacereader.h"
#include "tsurfacewriter.h"
#include "tconfmain.h"
#include "tnewprojectdialog.h"
#include "tprojectproperties.h"
#include "tpaneltypes.h"
#include "tpagetree.h"
#include "tpagehandler.h"
#include "taddpagedialog.h"
#include "taddpopupdialog.h"
#include "tresourcedialog.h"
#include "tgraphics.h"
#include "tfonts.h"
#include "tconfig.h"
#include "terror.h"
#include "tmisc.h"
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

    TFonts::releaseFontConfig();
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
    QString file = QFileDialog::getOpenFileName(this, tr("Open TSurface file"), mLastOpenPath, tr("TSurface (*.tsf);;AMX (*.TP4 *.TP5);;All (*)"));
    TConfig::Current().setLastDirectory(pathname(file));

    if (file.isEmpty())
        return;

    if (fs::exists(file.toStdString()) && fs::is_regular_file(file.toStdString()))
    {
        mLastOpenPath = pathname(file);                                     // The path from where the file was opened
        mPathTemporary = createTemporaryPath(basename(file));               // The path of the temporary directory with all files
        TSurfaceReader sreader(file, mPathTemporary);                       // Unzip file and write the contents into temporary direcory
        TConfMain::Current().setPathTemporary(mPathTemporary);              // Initialize class with path to temporary directory
        TConfMain::Current().readProject(mPathTemporary + "/prj_.json");    // Read the main project file
        TConfMain::Current().setFileName(file);                             // Initialize the class with the filename of the opened file
        TPageHandler::Current().setPathTemporary(mPathTemporary);           // Initialize class with path to temporary directory
        QStringList pages = TConfMain::Current().getAllPages();             // Get names of all pages
        QStringList popups = TConfMain::Current().getAllPopups();           // Get names of all popups
        pages.append(popups);                                               // Merge the lists
        TPageHandler::Current().readPages(pages);                           // Read all pages fron their files
        TPageTree::Current().setParent(this);                               // Initialize the class with the pointer to this class
        TPageTree::Current().createTree(m_ui->treeViewPages, TConfMain::Current().getJobName(), TConfMain::Current().getPanelType());   // Create the basic tree
        connect(&TPageTree::Current(), &TPageTree::clicked, this, &TSurface::onClickedPageTree);    // Connect to get informed about clicks
        QList<int> pageNumbers = TPageHandler::Current().getPageNumbers();  // Get a list of all page numbers
        QList<int>::Iterator iter;

        for (iter = pageNumbers.begin(); iter != pageNumbers.end(); ++iter) // Iterate through the page numbers
        {
            Page::PAGE_t pg = TPageHandler::Current().getPage(*iter);       // Get the page

            if (pg.pageID <= 0)                                             // Should never be true, but who knows ...
            {
                MSG_ERROR("Can't find page number " << *iter);
                continue;
            }

            MSG_DEBUG("Popuptype: " << pg.popupType);

            if (pg.popupType == Page::PT_PAGE)                              // Is it a page?
                TPageTree::Current().addPage(pg.name, pg.pageID);           // Yes, then add it to page part of tree
            else if (pg.popupType == Page::PT_POPUP)                        // Is it a popup?
                TPageTree::Current().addPopup(pg.name, pg.pageID);          // Yes, then add it to popup part of tree
        }

        mHaveProject = true;                                                // Set mark to indicate that we've a project
        mIsSaved = true;                                                    // Set mark that there exists already a file.
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
        int button = QMessageBox::question(this, tr("New project"), tr("<b>There are unsaved changes in this project!</b><br>Do you want to save the project?"));

        if (button == QMessageBox::Yes)
        {
            if (!mIsSaved)
            {
                if (!saveAs())
                    return;
            }
            else
            {
                if (!saveNormal())
                    return;
            }
        }

        m_ui->mdiArea->closeAllSubWindows();
        std::error_code ec;
        fs::remove_all(mPathTemporary.toStdString(), ec);
        TConfMain::Current().reset();
        mProjectChanged = false;
    }
    else if (mHaveProject)
    {
        int button = QMessageBox::question(this, tr("New project"), tr("Do you want to close the current project?"));

        if (button == QMessageBox::No)
            return;

        m_ui->mdiArea->closeAllSubWindows();
        std::error_code ec;
        fs::remove_all(mPathTemporary.toStdString(), ec);
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
    // Create new file structure
    mPathTemporary = createTemporaryPath(npd.getFileName());
    TConfMain::Current().setPathTemporary(mPathTemporary);
    createNewFileStructure();
    TGraphics::Current().writeSystemFiles(Graphics::FT_THEOSYS, mPathTemporary);
    // Store the selected font
    QFont font = npd.getFontName();
    QString ffile = TFonts::getFontFile(font);
    TFonts::addFontFile(ffile);
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

void TSurface::on_actionClose_triggered()
{
    DECL_TRACER("TSurface::on_actionClose_triggered()");

    if (!mHaveProject)
        return;

    if (!closeRequest())
        return;

    if (!mPageWidgets.empty())
    {
        mPageWidgets.clear();
    }

    m_ui->mdiArea->closeAllSubWindows();
    TPageTree::Current().reset();
    TConfMain::Current().reset();
    TPageHandler::Current().reset();
    mPathTemporary.clear();
    mProjectChanged = false;
    mHaveProject = false;
}

void TSurface::on_actionSave_triggered()
{
    DECL_TRACER("TSurface::on_actionSave_triggered()");

    if (!mIsSaved)
        saveAs();
    else
        saveNormal();
}

void TSurface::on_actionSave_as_triggered()
{
    DECL_TRACER("TSurface::on_actionSave_as_triggered()");

    saveAs();
}

void TSurface::on_actionExit_triggered()
{
    DECL_TRACER("TSurface::on_actionExit_triggered()");

    if (!closeRequest())
        return;

    close();
}


void TSurface::on_actionAdd_page_triggered()
{
    DECL_TRACER("TSurface::on_actionAdd_page_triggered()");

    onAddNewPage();
}

void TSurface::on_actionAdd_popup_page_triggered()
{
    DECL_TRACER("TSurface::on_actionAdd_popup_page_triggered()");

    onAddNewPopup();
}

void TSurface::on_actionSelection_tool_triggered(bool checked)
{
    DECL_TRACER("TSurface::on_actionSelection_tool_triggered(bool checked)");

    if (checked)
    {
        m_ui->actionButton_draw_tool->setChecked(false);
        m_ui->actionPopup_draw_tool->setChecked(false);
    }
}

void TSurface::on_actionButton_draw_tool_triggered(bool checked)
{
    DECL_TRACER("TSurface::on_actionButton_draw_tool_triggered(bool checked)");

    if (checked)
    {
        m_ui->actionSelection_tool->setChecked(false);
        m_ui->actionPopup_draw_tool->setChecked(false);
    }
}

void TSurface::on_actionPopup_draw_tool_triggered(bool checked)
{
    DECL_TRACER("TSurface::on_actionPopup_draw_tool_triggered(bool checked)");

    if (checked)
    {
        m_ui->actionButton_draw_tool->setChecked(false);
        m_ui->actionSelection_tool->setChecked(false);
    }
}

void TSurface::on_actionCut_triggered()
{

}
void TSurface::on_actionCopy_triggered()
{

}
void TSurface::on_actionInsert_triggered()
{

}
void TSurface::on_actionAdd_Application_Window_triggered()
{

}
void TSurface::on_actionEdit_Sub_Page_Sets_triggered()
{

}
void TSurface::on_actionEdit_Drop_Target_Groups_triggered()
{

}

void TSurface::on_actionResource_Manager_triggered()
{
    DECL_TRACER("TSurface::on_actionResource_Manager_triggered()");

    TResourceDialog rsDialog(this);
    rsDialog.setPathTemporary(mPathTemporary);
    rsDialog.exec();
}

void TSurface::on_actionRefresh_Dynamic_Images_triggered()
{

}
void TSurface::on_actionEdit_Palettes_triggered()
{

}

void TSurface::on_actionExport_Page_Images_triggered()
{

}
void TSurface::on_actionVerify_Function_Maps_triggered()
{

}
void TSurface::on_actionVerify_Event_Actions_triggered()
{

}
void TSurface::on_actionSynchronize_Fonts_triggered()
{

}
void TSurface::on_actionView_Conversion_Log_triggered()
{

}
void TSurface::on_actionDelete_Conversion_Log_triggered()
{

}
void TSurface::on_actionComma_Separated_Format_triggered()
{

}
void TSurface::on_actionText_Only_Format_triggered()
{

}
void TSurface::on_actionWeb_Page_Format_triggered()
{

}
void TSurface::on_actionShow_Popup_Page_triggered()
{

}
void TSurface::on_actionHide_Popup_Page_triggered()
{

}
void TSurface::on_actionHide_All_Popup_Pages_and_Application_Windows_triggered()
{

}
void TSurface::on_actionShow_Application_Window_triggered()
{

}
void TSurface::on_actionHide_Application_Window_triggered()
{

}
void TSurface::on_actionShow_External_Controls_triggered()
{

}
void TSurface::on_actionCopy_Image_to_Clipboard_triggered()
{

}
void TSurface::on_actionReset_Display_triggered()
{

}
void TSurface::on_actionDisplay_Previous_State_triggered()
{

}
void TSurface::on_actionDisplay_Next_State_triggered()
{

}
void TSurface::on_actionChoose_Display_State_triggered()
{

}
void TSurface::on_actionAdd_States_triggered()
{

}
void TSurface::on_actionInsert_States_triggered()
{

}
void TSurface::on_actionAnimation_Wizard_triggered()
{

}
void TSurface::on_actionPower_Assign_triggered()
{

}
void TSurface::on_actionPaste_Controls_triggered()
{

}
void TSurface::on_actionEnable_Disable_triggered()
{

}
void TSurface::on_actionSound_on_off_triggered()
{

}
void TSurface::on_actionPush_triggered()
{

}
void TSurface::on_actionBorder_Color_triggered()
{

}
void TSurface::on_actionFill_Color_triggered()
{

}
void TSurface::on_actionBorder_Color_2_triggered()
{

}
void TSurface::on_actionFill_Color_2_triggered()
{

}
void TSurface::on_actionText_Color_2_triggered()
{

}
void TSurface::on_actionText_Effect_Color_triggered()
{

}
void TSurface::on_actionAll_Colors_triggered()
{

}
void TSurface::on_actionOverall_Opacity_triggered()
{

}
void TSurface::on_actionBitmap_Position_triggered()
{

}
void TSurface::on_actionText_Position_triggered()
{

}
void TSurface::on_actionAll_Positions_triggered()
{

}
void TSurface::on_actionText_Size_triggered()
{

}
void TSurface::on_actionBring_to_Front_triggered()
{

}
void TSurface::on_actionSend_to_Back_triggered()
{

}
void TSurface::on_actionShift_Button_Up_triggered()
{

}
void TSurface::on_actionShift_Button_Down_triggered()
{

}
void TSurface::on_actionSize_to_Image_triggered()
{

}
void TSurface::on_actionAlignment_Sizing_triggered()
{

}
void TSurface::on_actionLeft_triggered()
{

}
void TSurface::on_actionHorizontal_Center_triggered()
{

}
void TSurface::on_actionRight_triggered()
{

}
void TSurface::on_actionTop_triggered()
{

}
void TSurface::on_actionVertical_Center_triggered()
{

}
void TSurface::on_actionBottom_triggered()
{

}
void TSurface::on_actionHorizontal_triggered()
{

}
void TSurface::on_actionVertical_triggered()
{

}
void TSurface::on_actionWidth_triggered()
{

}
void TSurface::on_actionHeight_triggered()
{

}
void TSurface::on_actionBoth_triggered()
{

}
void TSurface::on_action4_3_Video_triggered()
{

}
void TSurface::on_action16_9_triggered()
{

}
void TSurface::on_action16_9_Anamorphic_Video_triggered()
{

}
void TSurface::on_action1_85_1_Letterbox_Video_triggered()
{

}
void TSurface::on_action1_85_1_Anamorphic_Video_triggered()
{

}
void TSurface::on_action2_35_1_Letterbox_Video_triggered()
{

}
void TSurface::on_action2_35_1_Anamorphic_Video_triggered()
{

}
void TSurface::on_actionEqual_triggered()
{

}
void TSurface::on_actionIncrease_triggered()
{

}
void TSurface::on_actionDecrease_triggered()
{

}
void TSurface::on_actionRemove_triggered()
{

}
void TSurface::on_actionEqual_2_triggered()
{

}
void TSurface::on_actionIncrease_2_triggered()
{

}
void TSurface::on_actionDecrease_2_triggered()
{

}
void TSurface::on_actionRemove_2_triggered()
{

}
void TSurface::on_actionConnect_triggered()
{

}
void TSurface::on_actionDisconnect_from_Panel_triggered()
{

}
void TSurface::on_actionSend_to_Panel_triggered()
{

}
void TSurface::on_actionSend_File_to_Panel_triggered()
{

}
void TSurface::on_actionReceive_from_Panel_triggered()
{

}
void TSurface::on_actionRedo_last_Transfer_triggered()
{

}
void TSurface::on_actionCancel_Transfer_triggered()
{

}
void TSurface::on_actionCancel_all_Pending_Transfers_triggered()
{

}
void TSurface::on_actionClear_Transfer_triggered()
{

}
void TSurface::on_actionClear_All_Completed_Transfers_triggered()
{

}
void TSurface::on_actionClose_Status_When_Empty_triggered()
{

}
void TSurface::on_actionState_Manager_triggered()
{

}
void TSurface::on_actionButton_Preview_triggered()
{

}
void TSurface::on_actionMagnifier_triggered()
{

}
void TSurface::on_actionTransfer_Status_triggered()
{

}
void TSurface::on_actionStatus_Bar_triggered()
{

}
void TSurface::on_actionProperty_Painter_triggered()
{

}
void TSurface::on_actionDisplay_Function_State_Overlay_triggered()
{

}
void TSurface::on_actionError_Warnings_Report_triggered()
{

}
void TSurface::on_actionMain_Toolbar_triggered()
{

}
void TSurface::on_actionTransfer_Toolbar_triggered()
{

}
void TSurface::on_actionView_Toolbar_triggered()
{

}
void TSurface::on_actionSelection_Drawing_Tools_triggered()
{

}
void TSurface::on_actionButton_Displays_triggered()
{

}
void TSurface::on_actionZoom_Toolbar_triggered()
{

}
void TSurface::on_actionDrawing_Toolbar_triggered()
{

}
void TSurface::on_actionOrder_Assist_Toolbar_triggered()
{

}
void TSurface::on_actionPosition_Assist_Toolbar_triggered()
{

}
void TSurface::on_actionSize_Assist_Toolbar_triggered()
{

}

void TSurface::on_actionView_ReadMe_triggered()
{

}
void TSurface::on_actionPaint_triggered()
{

}
void TSurface::on_actionContents_triggered()
{

}
void TSurface::on_actionKeyboard_Map_triggered()
{

}
void TSurface::on_actionUpdate_triggered()
{

}

void TSurface::on_actionAbout_TSurface_triggered()
{
    DECL_TRACER("TSurface::on_actionAbout_TSurface_triggered()");

    QString line1 = QString("TSurface v%1<br>").arg(VERSION_STRING());
    QString line2("Copyright © 2026 by Andreas Theofilu<br>");
    QString line3("License: GPL v3");
    QMessageBox::about(this, tr("TSurface"), line1 + line2 + line3);
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
    bool visible = TPageHandler::Current().isVisible(num);
    MSG_DEBUG("Window is " << (visible ? "visible" : "not visible") << ", widget is " << (widget == nullptr ? "not available" : "available"));

    if (widget && visible)
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
    else if (!visible)
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
        TPageHandler::Current().setWidget(widget, num);

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
    QString ffile = TFonts::getFontFile(font);
    TFonts::addFont(font, ffile);
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

    pg.popupType = Page::PT_POPUP;
    pg.srPage.cf = popupDialog.getColorPageBackground();
    pg.srPage.ct = popupDialog.getColorText();
    pg.srPage.cb = popupDialog.getColorBorder();
    QFont font = popupDialog.getFont();
    QString ffile = TFonts::getFontFile(font);
    TFonts::addFont(font, ffile);
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

void TSurface::closeEvent(QCloseEvent *event)
{
    DECL_TRACER("TSurface::closeEvent(QCloseEvent *event)");

    if (!closeRequest())
    {
        event->ignore();
        return;
    }

    event->accept();
}

bool TSurface::closeRequest()
{
    DECL_TRACER("TSurface::closeRequest()");

    if (mHaveProject && mProjectChanged)
    {
        int ret = QMessageBox::question(this, tr("TSurface"), tr("There are unsaved changes in the current project!<br>Do you want to save the project?"));

        if (ret == QMessageBox::Yes)
        {
            QString fileName = TConfMain::Current().getFileName();
            QString basefile = basename(fileName);

            if (mIsSaved && fs::is_regular_file(fileName.toStdString()))
            {
                std::error_code ec;

                if (QMessageBox::question(this, "TSurface", tr("The file %1 exists!<br>Do you wan't to overwrite it?").arg(basefile)) == QMessageBox::Yes)
                {
                    fs::remove(fileName.toStdString(), ec);

                    if (ec)
                    {
                        MSG_ERROR("Unable to remove old project file: " << ec.message());
                        QMessageBox::critical(this, tr("TSurface error"), tr("Can't save project!<br><b>%1</b>").arg(ec.message()));
                        return false;
                    }
                }
            }

            if (mIsSaved && !saveNormal())
                return false;
            else if (!mIsSaved && !saveAs())
                return false;
            else
                return false;
        }

        mProjectChanged = false;
    }

    // If there's a project open, we close it and deleting the temporary files
    if (mHaveProject)
    {
        std::error_code ec;
        fs::remove_all(mPathTemporary.toStdString(), ec);

        if (ec)
        {
            MSG_ERROR("Error closing project: " << ec.message());
            return false;
        }
    }

    return true;
}

QString TSurface::createTemporaryPath(const QString& name)
{
    DECL_TRACER("TSurface::createTemporaryPath(const QString& name)");

    // Determine the temporary path of the OS
    QString temp;
    bool hidden = false;

    if (fs::is_directory("/tmp"))
        temp = "/tmp";
    else
    {
        temp = qEnvironmentVariable("TMP");

        if (temp.isEmpty())
        {
            temp = qEnvironmentVariable("TEMP");

            if (temp.isEmpty())
            {
                temp = qEnvironmentVariable("HOME");

                if (temp.isEmpty())
                    temp = QString::fromStdString(fs::current_path());
                else
                    hidden = true;
            }
        }
    }

    if (hidden)
        temp.append("/.");
    else
        temp.append("/");

    temp.append(name);

    if (!name.endsWith(".tsf"))
        temp.append(".tsf");

    MSG_PROTOCOL("Using temporary path: " << temp.toStdString());

    return temp;
}

bool TSurface::createNewFileStructure()
{
    DECL_TRACER("TSurface::createNewFileStructure()");

    QString fName = mPathTemporary;

    if (!fName.endsWith(".tsf"))
        fName.append(".tsf");

    try
    {
        fs::create_directories(fName.toStdString());
        fs::create_directory(fName.toStdString() + "/fonts");
        fs::create_directory(fName.toStdString() + "/images");
        fs::create_directory(fName.toStdString() + "/sounds");
        fs::create_directories(fName.toStdString() + "/__system/graphics/borders");
        fs::create_directories(fName.toStdString() + "/__system/graphics/cursors");
        fs::create_directories(fName.toStdString() + "/__system/graphics/fonts");
        fs::create_directories(fName.toStdString() + "/__system/graphics/images");
        fs::create_directories(fName.toStdString() + "/__system/graphics/sliders");
        fs::create_directories(fName.toStdString() + "/__system/graphics/sounds");
    }
    catch(std::exception& e)
    {
        MSG_ERROR("Error creating directories: " << e.what());
        return false;
    }

    return true;
}

bool TSurface::saveAs()
{
    DECL_TRACER("TSurface::saveAs()");

    if (!mHaveProject)
        return true;

    QString path;

    if (!mLastOpenPath.isEmpty())
        path = mLastOpenPath;
    else
        path = QString::fromStdString(fs::current_path());

    QFileDialog fileDialog(this, tr("Save As"), path, "Surface (*.tsf)");
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);

    if (mIsSaved)       // Was this saved already previously?
    {                   // Yes, then get the old file name and suggest it.
        QString file = basename(TConfMain::Current().getFileName());
        MSG_DEBUG("Using file: " << file.toStdString());
        fileDialog.selectFile(file);
    }
    else    // Get the predefined file name and suggest it.
    {
        QString file = basename(TConfMain::Current().getFileName());

        if (!file.endsWith(".tsf"))
            file.append(".tsf");

        fileDialog.selectFile(file);
    }
    // There is no need to check afterwards if the file already exists, because
    // the dialog function does this already. In case the user don't want to
    // overwrite an existing file, an empty file name is returned.
    if (fileDialog.exec() == QDialog::Rejected)
        return false;

    QStringList selectedFiles = fileDialog.selectedFiles();

    if (selectedFiles.empty())
        return false;

    QString file = selectedFiles[0];

    if (file.isEmpty())
        return false;

    if (file != TConfMain::Current().getFileName())
        TConfMain::Current().setFileName(file);

    mLastOpenPath = pathname(file);

    if (!file.endsWith(".tsf"))
        file.append(".tsf");

    TConfig::Current().setLastDirectory(mLastOpenPath);
    TConfMain::Current().setPathTemporary(mPathTemporary);
    TConfMain::Current().saveProject();
    TPageHandler::Current().setPathTemporary(mPathTemporary);
    TPageHandler::Current().saveAllPages();
    TSurfaceWriter prjSave(mPathTemporary, file);

    if (prjSave.haveError())
    {
        QMessageBox::critical(this, "TSurface error", "Error saving project!");
        return false;
    }

    mProjectChanged = false;
    mIsSaved = true;
    return true;
}

bool TSurface::saveNormal()
{
    DECL_TRACER("TSurface::saveNormal()");

    if (!mHaveProject || !mProjectChanged || mLastOpenPath.isEmpty())
        return true;

    QString file = TConfMain::Current().getFileName();
    TConfMain::Current().saveProject();
    TPageHandler::Current().setPathTemporary(mPathTemporary);
    TPageHandler::Current().saveAllPages();
    TSurfaceWriter prjSave(mPathTemporary, file);

    if (prjSave.haveError())
    {
        QMessageBox::critical(this, "TSurface error", "Error saving project!");
        return false;
    }

    mProjectChanged = false;
    mIsSaved = true;
    return true;
}
