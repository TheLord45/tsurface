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
#include <functional>

#include <QFileDialog>
#include <QResizeEvent>
#include <QMessageBox>
#include <QMdiSubWindow>
#include <QtEnvironmentVariables>

#include "tsurface.h"
#include "tresizablewidget.h"
#include "tcanvaswidget.h"
#include "tsurfacereader.h"
#include "tsurfacewriter.h"
#include "tconfmain.h"
#include "tworkspacehandler.h"
#include "tnewprojectdialog.h"
#include "tprojectproperties.h"
#include "tpaneltypes.h"
#include "tpagehandler.h"
#include "taddpagedialog.h"
#include "taddpopupdialog.h"
#include "tresourcedialog.h"
#include "tpreferencesdialog.h"
#include "tdrawimage.h"
#include "tdrawtext.h"
#include "tgraphics.h"
#include "tfonts.h"
#include "tconfig.h"
#include "terror.h"
#include "tmisc.h"
#include "ui_tsurface.h"
#include "moc_tsurface.cpp"

namespace fs = std::filesystem;
using namespace ObjHandler;
using std::cout;
using std::endl;
using std::bind;

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
            int id = getObjectID(objName);
            MSG_DEBUG("ID: " << id);

            if (id > 0)
            {
                TPageHandler::Current().setVisible(id, false);
                TWorkSpaceHandler::Current().clear();
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

    m_ui->mdiArea->setStyleSheet(QString("QMdiSubWindow{selection-background-color: %1;}").arg(TConfig::Current().getGutterColor().name()));

    TWorkSpaceHandler::Current(m_ui->treeViewPages,
                               m_ui->tableWidgetGeneral,
                               m_ui->tableWidgetProgramming,
                               m_ui->treeWidgetStates,
                               this);

    int splitt = TConfig::Current().getSplitterPosition();
    MSG_DEBUG("Setting Splitter to " << splitt)
    QList<int> splittSize = { splitt, (TConfig::Current().getLastPosition().width() - splitt) };
    m_ui->splitter->setSizes(splittSize);

    m_ui->tableWidgetGeneral->horizontalHeader()->hide();
    m_ui->tableWidgetGeneral->verticalHeader()->hide();
    // Disable Menu points
    m_ui->actionResource_Manager->setDisabled(true);
    m_ui->actionAdd_page->setDisabled(true);
    m_ui->actionAdd_popup_page->setDisabled(true);
    m_ui->actionAdd_Application_Window->setDisabled(true);
    // Get the last position and size of the main window and set it.
    QRect ws = TConfig::Current().getLastPosition();
    setGeometry(ws);
    mCloseEater = new winCloseEater;
    // Add the application icon
    setWindowIcon(QIcon(":images/tsurface_512.png"));
    setUnifiedTitleAndToolBarOnMac(true);
    // Add symbols to the toolbar
    initToolbar();
    // Initialize the callbacks
    connect(&TWorkSpaceHandler::Current(), &TWorkSpaceHandler::addNewTreePage, this, &TSurface::onAddNewPage);
    connect(&TWorkSpaceHandler::Current(), &TWorkSpaceHandler::addNewTreePopup, this, &TSurface::onAddNewPopup);
    connect(&TWorkSpaceHandler::Current(), &TWorkSpaceHandler::windowToFront, this, &TSurface::onItemToFront);
    TWorkSpaceHandler::Current().regDataChanged(bind(&TSurface::onDataChanged, this, std::placeholders::_1));
    TWorkSpaceHandler::Current().regMarkDirty(bind(&TSurface::onMarkDirty, this));
    TWorkSpaceHandler::Current().regRequestDraw(bind(&TSurface::onRedrawRequest, this, std::placeholders::_1));

    connect(m_ui->splitter, &QSplitter::splitterMoved, this, &TSurface::onSplitterMoved);
    connect(m_ui->mdiArea, &QMdiArea::subWindowActivated, this, &TSurface::onSubWindowActivated);
}

TSurface::~TSurface()
{
    DECL_TRACER("TSurface::~TSurface()");

    TConfig::Current().setLastPosition(geometry());
    TConfig::Current().saveConfig();

    if (mCloseEater)
        delete mCloseEater;

    TFonts::releaseFontConfig();
}

void TSurface::initToolbar()
{
    DECL_TRACER("TSurface::initToolbar()");

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

    m_ui->mainToolBar->addSeparator();

    mActionShowHideGrid = new QAction(QIcon(":images/view-grid.svg"), tr("Show/Hide grid"));
    mActionShowHideGrid->setCheckable(true);
    mActionShowHideGrid->setChecked(false);
    connect(mActionShowHideGrid, &QAction::triggered, this, &TSurface::onActionShowHideGrid);
    m_ui->mainToolBar->addAction(mActionShowHideGrid);

    mActionSnapToGrid = new QAction(QIcon(":images/snap-bounding-box-corners.svg"), tr("Snap to grid"));
    mActionSnapToGrid->setCheckable(true);
    mActionSnapToGrid->setChecked(false);
    connect(mActionSnapToGrid, &QAction::triggered, this, &TSurface::onActionSnapToGrid);
    m_ui->mainToolBar->addAction(mActionSnapToGrid);

    m_ui->mainToolBar->addSeparator();

    mActionToolSelect = new QAction(QIcon(":/images/edit-node.png"), tr("Selection tool"));
    mActionToolSelect->setCheckable(true);
    mActionToolSelect->setChecked(true);
    mSelectedTool = TOOL_SELECT;
    m_ui->actionSelection_tool->setChecked(true);
    connect(mActionToolSelect, &QAction::triggered, this, &TSurface::on_actionSelection_tool_triggered);
    m_ui->mainToolBar->addAction(mActionToolSelect);

    mActionToolDraw = new QAction(QIcon(":/images/document-edit.svg"), tr("Button draw tool"));
    mActionToolDraw->setCheckable(true);
    connect(mActionToolDraw, &QAction::triggered, this, &TSurface::on_actionButton_draw_tool_triggered);
    m_ui->mainToolBar->addAction(mActionToolDraw);

    mActionToolPopup = new QAction(QIcon(":/images/document-edit.png"), tr("Popup draw tool"));
    mActionToolPopup->setCheckable(true);
    connect(mActionToolPopup, &QAction::triggered, this, &TSurface::on_actionPopup_draw_tool_triggered);
    m_ui->mainToolBar->addAction(mActionToolPopup);
}

void TSurface::drawBackgroundColor(const Page::PAGE_t& page)
{
    DECL_TRACER("TSurface::drawBackgroundColor(const Page::PAGE_t& page)");

    if (!page.baseObject.widget)
    {
        MSG_WARNING("Have no validwidget to draw to!");
        return;
    }

    QList<QColor> gradients = page.srPage.gradientColors;

    if (gradients.empty() || gradients.size() < 2)
    {
        MSG_WARNING("No gradient colors! Will use default colors!");
        gradients = { Qt::gray, Qt::white };
    }

    if (page.srPage.ft == "solid")
    {
        page.baseObject.widget->setSolidColor(page.srPage.cf);
        return;
    }

    int entity = qMax(page.width, page.height);
    qreal stop = static_cast<qreal>(entity) / static_cast<qreal>(gradients.size()) / static_cast<qreal>(entity);
    QGradientStops gstops;
    qreal point = 0.0;

    if (gradients.size() == 2)
        stop = 1.0;

    for (QColor col : gradients)
    {
        gstops.append(std::pair<qreal, QColor>(point, col));
        point += stop;
    }

    gstops.last() = std::pair<qreal, QColor>(1.0, gradients.last());

    if (page.srPage.ft == "radial")    // Colors in circles
    {
        QRadialGradient radial;
        // Calculate center point;
        qreal rx = static_cast<qreal>(page.width) / 100.0 * static_cast<qreal>(page.srPage.gx);
        qreal ry = static_cast<qreal>(page.height) / 100.0 * static_cast<qreal>(page.srPage.gy);
        radial.setCenter(rx, ry);
        radial.setFocalPoint(rx, ry);
        radial.setRadius(static_cast<qreal>(page.srPage.gr));
        radial.setStops(gstops);
        page.baseObject.widget->setRadialGradient(radial);
        return;
    }
    else if (page.srPage.ft == "sweep")
    {
        QConicalGradient grad;
        grad.setAngle(0.0);
        grad.setCenter(page.baseObject.widget->rect().toRectF().center());
        grad.setStops(gstops);
        page.baseObject.widget->setConicGradient(grad);
        return;
    }

    QLinearGradient linear;
    linear.setStops(gstops);

    if (page.srPage.ft == "linearCLCR")
    {
        linear.setStart(0.0, static_cast<qreal>(page.height / 2));
        linear.setFinalStop(static_cast<qreal>(page.width), static_cast<qreal>(page.height / 2));
    }
    else if (page.srPage.ft == "linearTLBR")
    {
        linear.setStart(0.0, 0.0);
        linear.setFinalStop(static_cast<qreal>(page.width), static_cast<qreal>(page.height));
    }
    else if (page.srPage.ft == "linearCTCB")
    {
        linear.setStart(static_cast<qreal>(page.width / 2), 0.0);
        linear.setFinalStop(static_cast<qreal>(page.width / 2), static_cast<qreal>(page.height));
    }
    else if (page.srPage.ft == "linearTRBL")
    {
        linear.setStart(static_cast<qreal>(page.width), 0.0);
        linear.setFinalStop(0.0, static_cast<qreal>(page.height));
    }
    else if (page.srPage.ft == "linearCRCL")
    {
        linear.setStart(static_cast<qreal>(page.width), static_cast<qreal>(page.height / 2));
        linear.setFinalStop(0.0, static_cast<qreal>(page.height / 2));
    }
    else if (page.srPage.ft == "linearBRTL")
    {
        linear.setStart(static_cast<qreal>(page.width), static_cast<qreal>(page.height));
        linear.setFinalStop(0.0, 0.0);
    }
    else if (page.srPage.ft == "linearCBCT")
    {
        linear.setStart(static_cast<qreal>(page.width / 2), static_cast<qreal>(page.height));
        linear.setFinalStop(static_cast<qreal>(page.width / 2), 0.0);
    }
    else if (page.srPage.ft == "linearBLTR")
    {
        linear.setStart(0.0, static_cast<qreal>(page.height));
        linear.setFinalStop(static_cast<qreal>(page.width), 0.0);
    }

    page.baseObject.widget->setLinearGradient(linear);
}

void TSurface::updateGridFromUI(TCanvasWidget *widget)
{
    DECL_TRACER("TSurface::updateGridFromUI(TCanvasWidget *widget)");

    if (!widget)
        return;

    widget->setGridSize(TConfig::Current().getGridSize(), TConfig::Current().getGridSize());
    applyGridToChildren(widget);
}

void TSurface::applyGridToChildren(TCanvasWidget *widget)
{
    DECL_TRACER("TSurface::applyGridToChildren(TCanvasWidget *widget)");

    // Kept for compatibility with earlier API; snapping uses Canvas settings during move.
    if (!widget)
        return;

    const QSize grid = widget->gridSize();
    const bool snap = widget->snapEnabled();
    const QList<TResizableWidget*> children = widget->findChildren<TResizableWidget*>();

    for (TResizableWidget *w : children)
    {
        w->setGridSize(grid);
        w->setSnapToGrid(snap);
    }
}

void TSurface::addObject(int id, QPoint pt)
{
    DECL_TRACER("TSurface::addObject(int id, QPoint pt)");

    Page::PAGE_t *page = TPageHandler::Current().getPage(id);

    if (!page || page->pageID <= 0 || !page->baseObject.widget || !page->visible)
        return;

    QList<ObjHandler::TOBJECT_t> olist = TPageHandler::Current().getObjectList(*page);
    int btNumber = getNextObjectNumber(olist);
    ObjHandler::TOBJECT_t object = TPageHandler::Current().initNewObject(btNumber, QString("Button %1").arg(btNumber));
    QWidget* content = new QWidget(page->baseObject.widget);
    QString objName = QString("Object_%1").arg(btNumber);
    MSG_DEBUG("Adding object \"" << objName.toStdString() << "\" at position " << pt.x() << ", " << pt.y());
    content->setObjectName(objName);
    content->setStyleSheet(QString("background: %1").arg(object.sr[0].cf.name()));
    page->baseObject.widget->clearSelection();

    TResizableWidget* wrap = new TResizableWidget(content, page->baseObject.widget);
    wrap->setMinimumSize(40, 40);
    wrap->setGridSize(page->baseObject.widget->gridSize());
    wrap->setSnapToGrid(page->baseObject.widget->snapEnabled());
    wrap->setGeometry(pt.x(), pt.y(), 40, 40);
    wrap->show();
    wrap->setId(btNumber);
    wrap->setPageId(page->pageID);
    wrap->setSelected(false);
    connect(wrap, &TResizableWidget::selectChanged, this, &TSurface::onObjectSelectChanged);
    connect(wrap, &TResizableWidget::objectSizeChanged, this, &TSurface::onObjectSizeChanged);
    connect(wrap, &TResizableWidget::objectMoved, this, &TSurface::onObjectMoved);
    // Add to list
    TObjectHandler *o = new TObjectHandler(ObjHandler::GENERAL, btNumber, objName);
    o->setObject(page->baseObject.widget);
    o->setObject(object);
    o->setSize(wrap->geometry());
    int index = TPageHandler::Current().appendObject(page->pageID, o);
    TWorkSpaceHandler::Current().setObject(o);
    TWorkSpaceHandler::Current().setAllProperties(*page, STATE_BUTTON, index);
}

/**
 * @brief TSurface::drawObject
 * This method is called whenever ab object on a page or popup should be redrawn.
 * It takes a pointer to a page or popup and the index of the object to draw.
 * Mainly the method is called from the callback onRedrawRequest().
 *
 * @param page      A pointer to a page or popup
 * @param objIndex  The index to the object to draw
 */
void TSurface::drawObject(Page::PAGE_t *page, int objIndex)
{
    DECL_TRACER("TSurface::drawObject(Page::PAGE_t *page, int objIndex)");

    if (!page || page->pageID <= 0 || objIndex < 0 || objIndex >= page->objects.size() || !page->baseObject.widget)
        return;

    TObjectHandler *pobject = page->objects[objIndex];
    ObjHandler::TOBJECT_t object = pobject->getObject();

    if (object.sr.empty())
    {
        MSG_ERROR("The object " << object.bi << " on page " << page->pageID << " has no SR section!");
        return;
    }

    QWidget* content = new QWidget(page->baseObject.widget);
    QString objName = QString("Object_%1").arg(object.bi);
    QPoint pt(object.lt, object.tp);
    MSG_DEBUG("Drawing object \"" << objName.toStdString() << "\" at position " << pt.x() << ", " << pt.y());
    content->setObjectName(objName);
    content->setStyleSheet(QString("background: %1").arg(object.sr[0].cf.name(QColor::HexArgb)));
    page->baseObject.widget->clearSelection();

    TResizableWidget* wrap = new TResizableWidget(content, page->baseObject.widget);
    wrap->setGridSize(page->baseObject.widget->gridSize());
    wrap->setSnapToGrid(page->baseObject.widget->snapEnabled());
    wrap->setGeometry(pt.x(), pt.y(), object.wt, object.ht);
    wrap->show();
    wrap->setId(object.bi);
    wrap->setPageId(page->pageID);
    wrap->setSelected(false);
    connect(wrap, &TResizableWidget::selectChanged, this, &TSurface::onObjectSelectChanged);
    connect(wrap, &TResizableWidget::objectSizeChanged, this, &TSurface::onObjectSizeChanged);
    connect(wrap, &TResizableWidget::objectMoved, this, &TSurface::onObjectMoved);

    TWorkSpaceHandler::Current().setObject(pobject);
    STATE_TYPE stype = getStateFromPageType(page->popupType);
    TWorkSpaceHandler::Current().setAllProperties(*page, stype, objIndex);
}

int TSurface::getNextObjectNumber(QList<ObjHandler::TOBJECT_t>& objects)
{
    DECL_TRACER("TSurface::getNextObjectNumber(QList<ObjHandler::TOBJECT_t>& objects)");

    if (objects.empty())
        return 1;

    int btNumber = 0;
    QList<ObjHandler::TOBJECT_t>::Iterator iter;

    for (iter = objects.begin(); iter != objects.end(); ++iter)
    {
        if (iter->bi > btNumber)
            btNumber = iter->bi;
    }

    return btNumber + 1;
}

//
// Menu callback methods
//
void TSurface::onSplitterMoved(int pos, int index)
{
    DECL_TRACER("TSurface::onSplitterMoved(int pos, int index)");

    Q_UNUSED(index);
    TConfig::Current().setSplitterPosition(pos);
}

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

    mLastOpenPath = TConfig::Current().getLastDirectory();
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
        TWorkSpaceHandler::Current().createTree(TConfMain::Current().getJobName(), TConfMain::Current().getPanelType());   // Create the basic tree
        connect(&TWorkSpaceHandler::Current(), &TPageTree::clicked, this, &TSurface::onClickedPageTree);    // Connect to get informed about clicks

        QList<int> pageNumbers = TPageHandler::Current().getPageNumbers();  // Get a list of all page numbers
        QList<int>::Iterator iter;

        for (iter = pageNumbers.begin(); iter != pageNumbers.end(); ++iter) // Iterate through the page numbers
        {
            Page::PAGE_t *pg = TPageHandler::Current().getPage(*iter);      // Get the page

            if (!pg || pg->pageID <= 0)                                     // Should never be true, but who knows ...
            {
                MSG_ERROR("Can't find page number " << *iter);
                continue;
            }

            MSG_DEBUG("Popuptype: " << pg->popupType);

            if (pg->popupType == Page::PT_PAGE)                              // Is it a page?
                TWorkSpaceHandler::Current().addTreePage(pg->name, pg->pageID);   // Yes, then add it to page part of tree
            else if (pg->popupType == Page::PT_POPUP)                        // Is it a popup?
                TWorkSpaceHandler::Current().addTreePopup(pg->name, pg->pageID);  // Yes, then add it to popup part of tree
            else if (pg->popupType == Page::PT_SUBPAGE)
                TWorkSpaceHandler::Current().addTreeSubPage(pg->name, pg->pageID);
        }

        mHaveProject = true;                                                // Set mark to indicate that we've a project
        mIsSaved = true;                                                    // Set mark that there exists already a file.
        enableBaseMenus();                                                  // Enable the menus
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

    if (mProjectChanged)        // If there was already a project loaded and if it was changed, ask to save it.
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

        m_ui->mdiArea->closeAllSubWindows();                                    // Close all subwindows
        std::error_code ec;                                                     // Used to get the error code. This avoids the need of an exception.
        fs::remove_all(mPathTemporary.toStdString(), ec);                       // Delete all temporary files and directories.
        TConfMain::Current().reset();                                           // Reset the configuration settings
        mProjectChanged = false;                                                // Mark the project as untached.
    }
    else if (mHaveProject)          // If there is a project loaded but has no changes, ask if the project should really be closed.
    {
        int button = QMessageBox::question(this, tr("New project"), tr("Do you want to close the current project?"));

        if (button == QMessageBox::No)
            return;

        m_ui->mdiArea->closeAllSubWindows();                                    // Close all subwindows
        std::error_code ec;                                                     // Used to get the error code. This avoids the need of an exception.
        fs::remove_all(mPathTemporary.toStdString(), ec);                       // Delete all temporary files and directories.
        TConfMain::Current().reset();                                           // Reset the configuration settings
        mHaveProject = false;                                                   // Mark the project as untached.
    }

    mPageWidgets.clear();                                                       // Clear the list of widgets
    TNewProjectDialog npd(this);                                                // Show a dialog to get the base parameters of the new project
    int ret = npd.exec();                                                       // Execute the dialog and wait until it returns

    if (ret == QDialog::Rejected)                                               // If the user clicked on "Cancel", we're done here
        return;

    TConfMain& cmain = TConfMain::Current();                                    // Get the pointer to the instance of class TConfMain
    ConfigMain::PROJECTINFO_t projectInfo;                                      // The project information structure
    ConfigMain::SETUP_t setupInfo;                                              // The project setup structure
    TPanelType tpType;                                                          // The management of known panel types
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
    TWorkSpaceHandler::Current().createNewTree(npd.getProjectName(), npd.getPageName(), npd.getPanelName());
    connect(&TWorkSpaceHandler::Current(), &TPageTree::clicked, this, &TSurface::onClickedPageTree);
    // Add main page to MDI
    TCanvasWidget *widget = new TCanvasWidget;
    widget->setWindowTitle(npd.getPageName());
    widget->setFixedSize(npd.getResolution());
    widget->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    widget->setStyleSheet("background-color: " + npd.getColorBackground().name() + ";color: " + npd.getColorText().name()+ ";");
    widget->installEventFilter(mCloseEater);
    int id = TPageHandler::Current().createPage(widget, Page::PT_PAGE, npd.getPageName(), npd.getResolution().width(), npd.getResolution().height());
    QString objName(QString("Canvas_%1").arg(id));                              // Create a name for the object
    widget->setObjectName(objName);
    TPageHandler::Current().setPageBgColor(id, npd.getColorBackground());       // Set the background color
    TPageHandler::Current().setPageTextColor(id, npd.getColorText());           // Set the text color
    QMdiSubWindow *page = new QMdiSubWindow;
    objName = QString("SubWindow_%1").arg(id);                                  // Create a name for the object
    page->setObjectName(objName);
    page->setWidget(widget);
    page->setAttribute(Qt::WA_DeleteOnClose);
    page->installEventFilter(mCloseEater);
    page->setWindowIcon(QIcon(":images/tsurface_512.png"));
    m_ui->mdiArea->addSubWindow(page);
    widget->activateWindow();
    updateGridFromUI(widget);                                                   // Set the grid
    widget->show();

    TPageHandler::Current().setVisible(id, true);                               // Mark the page as visible
    onActionShowHideGrid(false);                                                // Hide the grid
    onActionSnapToGrid(false);                                                  // Deactivate snap to grid.
    mHaveProject = true;
    // Register callbacks
    connect(widget, &TCanvasWidget::gridChanged, [this, id](const QSize&) { applyGridToChildren(TPageHandler::Current().getWidget(id)); });
    connect(widget, &TCanvasWidget::snapChanged, [this, id](bool) { applyGridToChildren(TPageHandler::Current().getWidget(id)); });
    connect(widget, &TCanvasWidget::failedClickAt, this, &TSurface::onFailedClickAt);
    // Create new file structure
    mPathTemporary = createTemporaryPath(npd.getFileName());
    TConfMain::Current().setPathTemporary(mPathTemporary);
    createNewFileStructure();
    TGraphics::Current().writeSystemFiles(Graphics::FT_THEOSYS, mPathTemporary);
    // Store the selected font
    QFont font = npd.getFontName();
    QString ffile = TFonts::getFontFile(font);
    TFonts::addFontFile(ffile);
    // Enable Menus
    enableBaseMenus();
    // Draw the window with all known data
    Page::PAGE_t *pg = TPageHandler::Current().getPage(id);

    if (pg)
    {
        pg->baseObject.widget = widget;
        onRedrawRequest(pg);                                                   // Draw the components of the page
    }
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
    TWorkSpaceHandler::Current().resetTree();
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

    mForceClose = true;
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
        mActionToolDraw->setChecked(false);
        mActionToolPopup->setChecked(false);
        mSelectedTool = TOOL_SELECT;
        TPageHandler::Current().setSelectedToolToAllPages(mSelectedTool);
    }

    if (!mActionToolSelect->isChecked())
        mActionToolSelect->setChecked(true);
    else if (!m_ui->actionSelection_tool->isChecked())
        m_ui->actionSelection_tool->setChecked(true);
}

void TSurface::on_actionButton_draw_tool_triggered(bool checked)
{
    DECL_TRACER("TSurface::on_actionButton_draw_tool_triggered(bool checked)");

    if (checked)
    {
        m_ui->actionSelection_tool->setChecked(false);
        m_ui->actionPopup_draw_tool->setChecked(false);
        mActionToolSelect->setChecked(false);
        mActionToolPopup->setChecked(false);
        mSelectedTool = TOOL_DRAW;
        TPageHandler::Current().setSelectedToolToAllPages(mSelectedTool);
    }

    if (!mActionToolDraw->isChecked())
        mActionToolDraw->setChecked(true);
    else if (!m_ui->actionButton_draw_tool->isChecked())
        m_ui->actionButton_draw_tool->setChecked(true);
}

void TSurface::on_actionPopup_draw_tool_triggered(bool checked)
{
    DECL_TRACER("TSurface::on_actionPopup_draw_tool_triggered(bool checked)");

    if (checked)
    {
        m_ui->actionButton_draw_tool->setChecked(false);
        m_ui->actionSelection_tool->setChecked(false);
        mActionToolSelect->setChecked(false);
        mActionToolDraw->setChecked(false);
        mSelectedTool = TOOL_DRAW;
        TPageHandler::Current().setSelectedToolToAllPages(mSelectedTool);
    }

    if (!mActionToolPopup->isChecked())
        mActionToolPopup->setChecked(true);
    else if (!m_ui->actionPopup_draw_tool->isChecked())
        m_ui->actionPopup_draw_tool->setChecked(true);
}

void TSurface::on_actionGrab_Properties_Tool_triggered(bool checked)
{

}

void TSurface::on_actionPaint_Properties_Tool_triggered(bool checked)
{

}

void TSurface::on_actionShow_Grid_triggered(bool checked)
{
    DECL_TRACER("TSurface::on_actionShow_Grid_triggered(bool checked)");

    mFromExtern = true;
    onActionShowHideGrid(checked);
    mFromExtern = false;
}

void TSurface::on_actionSnap_To_Grid(bool checked)
{
    DECL_TRACER("TSurface::on_actionSnap_To_Grid(bool checked)");

    mFromExtern = true;
    onActionSnapToGrid(checked);
    mFromExtern = false;
}

void TSurface::on_actionCurrent_Property_triggered()
{

}

void TSurface::on_actionDisabled_triggered()
{

}

void TSurface::on_actionText_triggered()
{

}

void TSurface::on_actionUndo_Button_Set_Dynamic_Data_Source_triggered()
{

}

void TSurface::on_actionRedo_triggered()
{

}

void TSurface::on_actionCut_triggered()
{

}
void TSurface::on_actionCopy_triggered()
{

}
void TSurface::on_actionPaste_triggered()
{

}

void TSurface::on_actionReplace_triggered()
{

}

void TSurface::on_actionDelete_triggered()
{

}

void TSurface::on_actionRename_triggered()
{

}

void TSurface::on_actionFind_triggered()
{

}

void TSurface::on_actionFind_Replace_triggered()
{

}

void TSurface::on_actionSelect_All_triggered()
{

}

void TSurface::on_actionApply_To_All_triggered()
{

}

void TSurface::on_actionEdit_Previous_triggered()
{

}

void TSurface::on_actionEdit_Next_triggered()
{

}

void TSurface::on_actionPreferences_triggered()
{
    DECL_TRACER("TSurface::on_actionPreferences_triggered()");

    // TODO add preferences dialog
    TPreferencesDialog dialog(this);

    if (dialog.exec() == QDialog::Rejected)
        return;

    TConfig::Current().setLastDirectory(TConfig::Current().getFilesPanels());
    TConfig::Current().saveConfig();
    // TODO: Apply settings
    mLastOpenPath = TConfig::Current().getLastDirectory();
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
    rsDialog.setLastOpenPath(mLastOpenPath);
    rsDialog.exec();

    mLastOpenPath = rsDialog.getLastOpenPath();

    if (rsDialog.haveChanged())
        mProjectChanged = true;
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

    int pageID = id;

    if (!TPageHandler::Current().bringToFront(pageID))
    {
        QMdiSubWindow *w = m_ui->mdiArea->currentSubWindow();

        if (!w)
        {
            MSG_DEBUG("Have no active subwindow");
            TWorkSpaceHandler::Current().clear();
            return;
        }
        else
        {
            pageID = getObjectID(w->objectName());

            if (pageID <= 0 || !TPageHandler::Current().isVisible(pageID))
            {
                MSG_DEBUG("Page " << pageID << " is not visible");
                TWorkSpaceHandler::Current().clear();
                return;
            }
        }

        MSG_DEBUG("Showing data of page " << pageID);
    }
    // Set the properties
    if (pageID < 500)
        TWorkSpaceHandler::Current().setPage(pageID);
    else if (pageID < 1000)
        TWorkSpaceHandler::Current().setPopup(pageID);
}

/**
 * @brief TSurface::onDataChanged
 * This is a callback method. It is called out of one of the subclasses of
 * "TWorkSpaceHandler". Beside this the project is marked as changed.
 *
 * @param page  A pointer to the structure containing all page data. This
 * structure is send to class "TPageHandler" where it is put into the array
 * of pages.
 */
void TSurface::onDataChanged(Page::PAGE_t *page)
{
    DECL_TRACER("TSurface::onDataChanged(Page::PAGE_t *page)");

    mProjectChanged = true;
    TPageHandler::Current().setPage(*page);
}

void TSurface::onMarkDirty()
{
    DECL_TRACER("TSurface::onMarkDirty()");

    mProjectChanged = true;
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

void TSurface::onActionShowHideGrid(bool checked)
{
    DECL_TRACER("TSurface::onActionShowHideGrid(bool checked)");

    if (mActionBlock)
        return;

    if (!mFromExtern)
        m_ui->actionShow_Grid->setChecked(checked);

    Page::PAGE_t page = TPageHandler::Current().getCurrentPage(m_ui->mdiArea);

    if (page.pageID <= 0)
        return;

    page.baseObject.widget->setShowGrid(checked);
    TPageHandler::Current().setGridVisible(page.pageID, checked);

    if (mActionShowHideGrid->isChecked() != checked)
    {
        mActionBlock = true;
        mActionShowHideGrid->setChecked(checked);
        mActionBlock = false;
    }
}

void TSurface::onActionSnapToGrid(bool checked)
{
    DECL_TRACER("TSurface::onActionSnapToGrid(bool checked)");

    if (mActionBlock)
        return;

    if (!mFromExtern)
        m_ui->actionSnap_To_Grid->setChecked(checked);

    Page::PAGE_t page = TPageHandler::Current().getCurrentPage(m_ui->mdiArea);

    if (page.pageID <= 0)
        return;

    page.baseObject.widget->setSnapEnabled(checked);
    applyGridToChildren(page.baseObject.widget);
    TPageHandler::Current().setSnapToGrid(page.pageID, checked);

    if (mActionSnapToGrid->isChecked() != checked)
    {
        mActionBlock = true;
        mActionSnapToGrid->setChecked(checked);
        mActionBlock = false;
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
    TCanvasWidget *widget = TPageHandler::Current().getWidget(num);
    bool visible = TPageHandler::Current().isVisible(num);
    MSG_DEBUG("Window is " << (visible ? "visible" : "not visible") << ", widget is " << (widget == nullptr ? "not available" : "available"));

    if (widget && visible)          // If the window is visible, hide it.
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
                TWorkSpaceHandler::Current().clear();
                break;
            }
        }
    }
    else if (!visible)          // If the window is invisible (not created yet), create it.
    {
        MSG_DEBUG("Window is not visible. Generating it ...")
        Page::PAGE_t *pg = TPageHandler::Current().getPage(num);                // Get the whole page (structure)

        if (!pg)
            return;

        widget = new TCanvasWidget(this);                                       // Create a new canvas widget --> subwindow in Mdi
        widget->setWindowTitle(name);                                           // Set the title of the window
        widget->setFixedSize(QSize(pg->width, pg->height));                     // Set the size
        MSG_DEBUG("Window was set to size: " << pg->width << " x " << pg->height);
        widget->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);    // Frame decorations
        widget->setStyleSheet("background-color: " + pg->srPage.cf.name() + ";color: " + pg->srPage.ct.name()+ ";");  // Set the background color
        QString objName(QString("Canvas_%1").arg(pg->pageID));                  // Create a name for the object
        widget->setObjectName(objName);                                         // set the object name
        widget->installEventFilter(mCloseEater);                                // Set an event filter to cache the click on the close button
        MSG_DEBUG("Object name: " << objName.toStdString());
        pg->baseObject.widget = widget;                                         // Add the widget to our local copy of the page structure

        QMdiSubWindow *page = new QMdiSubWindow;                                // Create a new subwindow in the QMdiArea
        objName = QString("SubWindow_%1").arg(pg->pageID);                      // Create a name for the object
        page->setObjectName(objName);                                           // Set the name
        page->setContentsMargins(0, 0, 0, 0);                                   // We remove the margins
        page->setWidget(widget);                                                // Add the previous created widget to the subwindow
        page->resize(page->minimumSizeHint());                                  // This is necessary to get the correct size of the window
        page->setAttribute(Qt::WA_DeleteOnClose);                               // Request that it should be deleted automatically on close of subwindow
        page->installEventFilter(mCloseEater);                                  // Set an event filter to cache the click on the close button
        page->setWindowIcon(QIcon(":images/tsurface_512.png"));                 // Give the window an icon
        m_ui->mdiArea->addSubWindow(page);                                      // Add it to the QMdiArea
        widget->activateWindow();                                               // Activate it
        widget->show();                                                         // Make it visible
        updateGridFromUI(widget);                                               // Set the grid
        int id = pg->pageID;                                                    // Necessary to be able to give the page id to the lamda function
        connect(widget, &TCanvasWidget::gridChanged, [this, id](const QSize&) { applyGridToChildren(TPageHandler::Current().getWidget(id)); });
        connect(widget, &TCanvasWidget::snapChanged, [this, id](bool) { applyGridToChildren(TPageHandler::Current().getWidget(id)); });
        connect(widget, &TCanvasWidget::failedClickAt, this, &TSurface::onFailedClickAt);

        TPageHandler::Current().setVisible(num, true);                          // Mark the page as visible
        onActionShowHideGrid(TPageHandler::Current().isGridVisible(pg->pageID));// Show or hide the grid
        onActionSnapToGrid(TPageHandler::Current().isSnapToGrid(pg->pageID));   // Activate or deactivate snap to grid. This independable from the visibility of the grid.
        // TODO: Add code to draw all objects
        onRedrawRequest(pg);                                                    // Draw the components of the page
    }
}

void TSurface::onFailedClickAt(const QPoint& pt)
{
    DECL_TRACER("TSurface::onFailedClickAt(const QPoint& pt)");

    if (mSelectedTool == TOOL_DRAW)
    {
        Page::PAGE_t page = TPageHandler::Current().getCurrentPage(m_ui->mdiArea);

        if (page.pageID <= 0)
            return;

        addObject(page.pageID, pt);
    }
}

void TSurface::onObjectSelectChanged(TResizableWidget *w, bool selected)
{
    DECL_TRACER("TSurface::onObjectSelectChanged(TResizableWidget *w, bool selected)");

    MSG_DEBUG("Page: " << w->getPageId() << " with object: " << w->getId());
    Page::PAGE_t *page = TPageHandler::Current().getPage(w->getPageId());

    if (!page)
        return;

    if (selected)
    {
        TWorkSpaceHandler::Current().setStateType(STATE_BUTTON);
        TObjectHandler *object = TPageHandler::Current().getObjectHandler(w->getPageId(), w->getId());

        if (!object)
            return;

        TWorkSpaceHandler::Current().setActualObject(object, TPageHandler::Current().getObjectIndex(*page, w->getId()));
    }
    else
    {
        // Find actual selected object
        if (page->visible && page->baseObject.widget)
        {
            TResizableWidget *widget = page->baseObject.widget->currentSelectedWidget();

            if (!widget)
                TWorkSpaceHandler::Current().setStateType(page->popupType == Page::PT_PAGE ? STATE_PAGE : STATE_POPUP);
            else
            {
                TObjectHandler *object = TPageHandler::Current().getObjectHandler(widget->getPageId(), widget->getId());

                if (!object)
                    return;

                TWorkSpaceHandler::Current().setActualObject(object, TPageHandler::Current().getObjectIndex(*page, w->getId()));
            }
        }
    }
}

void TSurface::onObjectMoved(TResizableWidget *w, QPoint pt)
{
    DECL_TRACER("TSurface::onObjectMoved(TResizableWidget *w, QPoint pt)");

    MSG_DEBUG("Page: " << w->getPageId() << " with object: " << w->getId() << ", point: " << pt.x() << ", " << pt.y());
    QRect geom = w->geometry();
    geom.setLeft(pt.x());
    geom.setTop(pt.y());
    Page::PAGE_t *page = TPageHandler::Current().getPage(w->getPageId());

    if (!page)
        return;

    TObjectHandler *object = TPageHandler::Current().getObjectHandler(w->getPageId(), w->getId());

    if (!object)
        return;

    object->setSize(geom);
    TWorkSpaceHandler::Current().setActualObject(object, TPageHandler::Current().getObjectIndex(*page, w->getId()));
}

void TSurface::onObjectSizeChanged(TResizableWidget *w, QSize size)
{
    DECL_TRACER("TSurface::onObjectSizeChanged(TResizableWidget *w, QSize size)");

    MSG_DEBUG("Page: " << w->getPageId() << " with object: " << w->getId());
    QRect geom = w->geometry();
    geom.setWidth(size.width());
    geom.setHeight(size.height());
    Page::PAGE_t *page = TPageHandler::Current().getPage(w->getPageId());

    if (!page)
        return;

    TObjectHandler *object = TPageHandler::Current().getObjectHandler(w->getPageId(), w->getId());

    if (!object)
        return;

    object->setSize(geom);
    TWorkSpaceHandler::Current().setActualObject(object, TPageHandler::Current().getObjectIndex(*page, w->getId()));
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
    TCanvasWidget *widget = new TCanvasWidget;
    widget->setWindowTitle(pageDialog.getPageName());
    widget->setFixedSize(QSize(pgSize.width(), pgSize.height()));
    widget->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    widget->setStyleSheet("background-color: " + pageDialog.getColorBackground().name() + ";color: " + pageDialog.getColorText().name()+ ";");
    Page::PAGE_t pg;
    int id = TPageHandler::Current().createPage(widget, Page::PT_PAGE, pageDialog.getPageName(), pgSize.width(), pgSize.height(), 0, 0, &pg);
    QString objName("QWidgetMDI_");
    objName.append(QString::number(id));
    widget->setObjectName(objName);
    widget->setPageID(id);
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
    TWorkSpaceHandler::Current().addTreePage(pageDialog.getPageName(), id);
    TWorkSpaceHandler::Current().invalidateObject();
    TConfMain::Current().addPage(pageDialog.getPageName(), id);
    // Here we set everyting of the page we currently know
//    Page::PAGE_t *pg = TPageHandler::Current().getPage(id);

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
    TCanvasWidget *widget = new TCanvasWidget;
    widget->setWindowTitle(popupDialog.getPopupName());
    widget->setFixedSize(QSize(pgSize.width(), pgSize.height()));
    widget->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    widget->setStyleSheet("background-color: " + popupDialog.getColorPageBackground().name() + ";color: " + popupDialog.getColorText().name()+ ";");
    QRect geom(popupDialog.getPositionLeft(), popupDialog.getPositionTop(), popupDialog.getSizeWidth(), popupDialog.getSizeHeight());
    Page::PAGE_t pg;
    int id = TPageHandler::Current().createPage(widget, Page::PT_POPUP, popupDialog.getPopupName(), geom, &pg);
    QString objName("QWidgetMDI_");
    objName.append(QString::number(id));
    widget->setObjectName(objName);
    widget->setPageID(id);
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
    TWorkSpaceHandler::Current().addTreePopup(popupDialog.getPopupName(), id);
    TWorkSpaceHandler::Current().invalidateObject();
    TConfMain::Current().addPopup(popupDialog.getPopupName(), id);
    // Here we set everyting of the page we currently know
//    Page::PAGE_t *pg = TPageHandler::Current().getPage(id);

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

void TSurface::onSubWindowActivated(QMdiSubWindow *window)
{
    DECL_TRACER("TSurface::onSubWindowActivated(QMdiSubWindow *window)");

    if (!window)
        return;

    QString objName = window->objectName();
    int id = getObjectID(objName);

    if (id <= 0)
        return;

    Page::PAGE_t *page = TPageHandler::Current().getPage(id);

    if (!page || page->pageID <= 0)
        return;

    MSG_DEBUG("Requested page ID: " << id << ", received page ID: " << page->pageID);
    TPageHandler::Current().setVisible(id, true);

    if (page->popupType == Page::PT_PAGE)
        TWorkSpaceHandler::Current().setPage(id, false, *page);
    else
        TWorkSpaceHandler::Current().setPopup(id, false, *page);

    TWorkSpaceHandler::Current().setFocus(page->pageID);
    // TODO: Add code to check for a selected object. If there is
    // one and only one, it is the actual one.
    // If there are more than one selected the page data should
    // be shown.
    if (page->baseObject.widget && page->baseObject.widget->selectedCount() == 1)
    {
        TResizableWidget *rw = page->baseObject.widget->currentSelectedWidget();

        if (rw && rw->getPageId() == page->pageID)
        {
            TObjectHandler *obj = TPageHandler::Current().getObjectHandler(page->pageID, rw->getId());
            TWorkSpaceHandler::Current().setActualObject(obj);
        }
    }
}

void TSurface::onRedrawRequest(Page::PAGE_t *page)
{
    DECL_TRACER("TSurface::onRedrawRequest(Page::PAGE_t *page)");

    if (!page || page->pageID <= 0)
        return;
    // First draw everything on the background
    //----------------------------------------
    // Background color
    drawBackgroundColor(*page);

    // Draw bitmaps on background
    if (!page->srPage.bitmaps.empty())
    {
        TDrawImage drawImage(page->srPage.bitmaps, &page->baseObject);
        drawImage.draw();
    }
    // Draw Text on background
    if (!page->srPage.te.isEmpty())
    {
        TDrawText dt(&page->baseObject, page->srPage.te, page->srPage.ff, page->srPage.jt, page->srPage.tx, page->srPage.ty);
        dt.setFontSize(page->srPage.fs);
        dt.setTextColor(page->srPage.ct);
        dt.setTextEffect(page->srPage.et, page->srPage.ec);

        if (!dt.draw())
        {
            MSG_WARNING("Couldn't draw a background text!");
            return;
        }
    }

    // Draw the objects
    for (int idx = 0; idx < page->objects.size(); ++idx)
        drawObject(page, idx);
    // TODO: Add code to draw objects
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

    if (mForceClose)
    {
        event->accept();
        return;
    }

    if (!closeRequest())
    {
        event->ignore();
        return;
    }

    mForceClose = true;
    event->accept();
}

void TSurface::keyPressEvent(QKeyEvent *event)
{
    DECL_TRACER("TSurface::keyPressEvent(QKeyEvent *event)");

    if (event->key() == Qt::Key_Delete)     // Delete all selected objects if key Delete is pressed
    {
        Page::PAGE_t page = TPageHandler::Current().getCurrentPage(m_ui->mdiArea);

        if (page.pageID <= 0 || !page.baseObject.widget)
            return;

        page.baseObject.widget->removeSelected();
        event->accept();
    }
}

bool TSurface::closeRequest()
{
    DECL_TRACER("TSurface::closeRequest()");

    if (mForceClose)
        return true;

    bool changed = TWorkSpaceHandler::Current().isChanged();

    if (mHaveProject && (mProjectChanged || changed))
    {
        if (changed)
        {
            Page::PAGE_t pg = TWorkSpaceHandler::Current().getActualPage();
            TPageHandler::Current().setPage(pg);
        }

        int ret = QMessageBox::question(this, tr("TSurface"), tr("There are unsaved changes in the current project!<br>Do you want to save the project?"));

        if (ret == QMessageBox::Yes)
        {
            QString fileName = TConfMain::Current().getFileName();
            QString basefile = basename(fileName);

            if (mIsSaved && fs::is_regular_file(fileName.toStdString()) && fileName != TConfMain::Current().getFileName())
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
    QString temp = TConfig::Current().getFilesTemp();
    bool hidden = false;
    bool haveTemp = false;

    if (temp.isEmpty())
        temp = QString::fromStdString(fs::temp_directory_path());

    if (!temp.isEmpty() && fs::is_directory(temp.toStdString()))
        haveTemp = true;
    else if (fs::is_directory("/tmp"))
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

    if (!haveTemp)
    {
        TConfig::Current().setFilesTemp(temp);
        TConfig::Current().saveConfig();
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

void TSurface::enableBaseMenus()
{
    DECL_TRACER("TSurface::enableBaseMenus()");

    m_ui->actionResource_Manager->setDisabled(false);
    m_ui->actionAdd_page->setDisabled(false);
    m_ui->actionAdd_popup_page->setDisabled(false);
    m_ui->actionAdd_Application_Window->setDisabled(false);
}
