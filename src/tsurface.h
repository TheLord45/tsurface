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
#ifndef TSURFACE_H
#define TSURFACE_H

#include <QMainWindow>

#include <memory>
#include <vector>

#include "tpagetree.h"
#include "tpagehandler.h"

namespace Ui {
class tsurface;
}

class winCloseEater : public QObject
{
    Q_OBJECT
    public:
        winCloseEater() {}

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;
};

class TSurface : public QMainWindow
{
    Q_OBJECT

    public:
        explicit TSurface(QWidget *parent = nullptr);
        ~TSurface() override;

    protected:
        void resizeEvent(QResizeEvent *event) override;
        void closeEvent(QCloseEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;

        void updateGridFromUI(TCanvasWidget *widget);
        void applyGridToChildren(TCanvasWidget *widget);
        void addObject(int id, QPoint pt);
        int getNextObjectNumber(QList<ObjHandler::TOBJECT_t>& objects);

        void onClickedPageTree(const TPageTree::WINTYPE_t wt, int num, const QString& name);
        void onAddNewPage();
        void onAddNewPopup();
        void onActionShowChannels(bool checked);
        void onItemToFront(int id);
        void onDataChanged(Page::PAGE_t *page);
        void onMarkDirty();

    private slots:
        // Splitter
        void onSplitterMoved(int pos, int index);
        // Menu: File
        void on_actionOpen_triggered();
        void on_actionNew_triggered();
        void on_actionProject_properties_triggered();
        void on_actionClose_triggered();
        void on_actionSave_triggered();
        void on_actionSave_as_triggered();
        void on_actionExit_triggered();
        // Menu: Edit
        void on_actionButton_draw_tool_triggered(bool checked);
        void on_actionSelection_tool_triggered(bool checked);
        void on_actionPopup_draw_tool_triggered(bool checked);
        void on_actionCut_triggered();
        void on_actionCopy_triggered();
        void on_actionInsert_triggered();
        // Menu: Panel
        void on_actionAdd_page_triggered();
        void on_actionAdd_popup_page_triggered();
        void on_actionAdd_Application_Window_triggered();
        void on_actionEdit_Sub_Page_Sets_triggered();
        void on_actionEdit_Drop_Target_Groups_triggered();
        void on_actionResource_Manager_triggered();
        void on_actionRefresh_Dynamic_Images_triggered();
        void on_actionEdit_Palettes_triggered();
        void on_actionExport_Page_Images_triggered();
        void on_actionVerify_Function_Maps_triggered();
        void on_actionVerify_Event_Actions_triggered();
        void on_actionSynchronize_Fonts_triggered();
        void on_actionView_Conversion_Log_triggered();
        void on_actionDelete_Conversion_Log_triggered();
        // Menu: Panel->Generate Programmer's Report
        void on_actionComma_Separated_Format_triggered();
        void on_actionText_Only_Format_triggered();
        void on_actionWeb_Page_Format_triggered();
        // Menu: Page
        void on_actionShow_Popup_Page_triggered();
        void on_actionHide_Popup_Page_triggered();
        void on_actionHide_All_Popup_Pages_and_Application_Windows_triggered();
        void on_actionShow_Application_Window_triggered();
        void on_actionHide_Application_Window_triggered();
        void on_actionShow_External_Controls_triggered();
        void on_actionCopy_Image_to_Clipboard_triggered();
        // Menu: Button
        void on_actionAdd_States_triggered();
        void on_actionAnimation_Wizard_triggered();
        void on_actionChoose_Display_State_triggered();
        void on_actionDisplay_Previous_State_triggered();
        void on_actionDisplay_Next_State_triggered();
        void on_actionInsert_States_triggered();
        void on_actionPower_Assign_triggered();
        void on_actionPaste_Controls_triggered();
        void on_actionReset_Display_triggered();
        // Menu: Button->preview
        void on_actionEnable_Disable_triggered();
        void on_actionSound_on_off_triggered();
        void on_actionPush_triggered();
        // Menu: States
        void on_actionBorder_Color_triggered();
        void on_actionFill_Color_triggered();
        // Menu: States->Tweeners
        void on_actionBorder_Color_2_triggered();
        void on_actionFill_Color_2_triggered();
        void on_actionText_Color_2_triggered();
        void on_actionText_Effect_Color_triggered();
        void on_actionAll_Colors_triggered();
        void on_actionOverall_Opacity_triggered();
        void on_actionBitmap_Position_triggered();
        void on_actionText_Position_triggered();
        void on_actionAll_Positions_triggered();
        void on_actionText_Size_triggered();
        // Menu: Layout
        void on_actionBring_to_Front_triggered();
        void on_actionSend_to_Back_triggered();
        void on_actionShift_Button_Up_triggered();
        void on_actionShift_Button_Down_triggered();
        void on_actionSize_to_Image_triggered();
        void on_actionAlignment_Sizing_triggered();
        // Menu: Layout->Align
        void on_actionLeft_triggered();
        void on_actionHorizontal_Center_triggered();
        void on_actionRight_triggered();
        void on_actionTop_triggered();
        void on_actionVertical_Center_triggered();
        void on_actionBottom_triggered();
        // Menu: Layout->Center in Page
        void on_actionHorizontal_triggered();
        void on_actionVertical_triggered();
        // Menu: Layout->Horizontal Spacing
        void on_actionEqual_triggered();
        void on_actionIncrease_triggered();
        void on_actionDecrease_triggered();
        void on_actionRemove_triggered();
        // Menu: Layout->Make same Size
        void on_actionWidth_triggered();
        void on_actionHeight_triggered();
        void on_actionBoth_triggered();
        // Menu: Layout->Size for Video
        void on_action4_3_Video_triggered();
        void on_action16_9_triggered();
        void on_action16_9_Anamorphic_Video_triggered();
        void on_action1_85_1_Letterbox_Video_triggered();
        void on_action1_85_1_Anamorphic_Video_triggered();
        void on_action2_35_1_Letterbox_Video_triggered();
        void on_action2_35_1_Anamorphic_Video_triggered();
        // Menu: Layout->Vertical Spacing
        void on_actionEqual_2_triggered();
        void on_actionIncrease_2_triggered();
        void on_actionDecrease_2_triggered();
        void on_actionRemove_2_triggered();
        // Menu: Transfer
        void on_actionConnect_triggered();
        void on_actionDisconnect_from_Panel_triggered();
        void on_actionSend_to_Panel_triggered();
        void on_actionSend_File_to_Panel_triggered();
        void on_actionReceive_from_Panel_triggered();
        void on_actionRedo_last_Transfer_triggered();
        void on_actionCancel_Transfer_triggered();
        void on_actionCancel_all_Pending_Transfers_triggered();
        void on_actionClear_Transfer_triggered();
        void on_actionClear_All_Completed_Transfers_triggered();
        void on_actionClose_Status_When_Empty_triggered();
        // Menu: View
        void on_actionState_Manager_triggered();
        void on_actionButton_Preview_triggered();
        void on_actionMagnifier_triggered();
        void on_actionTransfer_Status_triggered();
        void on_actionStatus_Bar_triggered();
        void on_actionProperty_Painter_triggered();
        void on_actionDisplay_Function_State_Overlay_triggered();
        void on_actionError_Warnings_Report_triggered();
        // Menu: View->Toolbars
        void on_actionMain_Toolbar_triggered();
        void on_actionTransfer_Toolbar_triggered();
        void on_actionView_Toolbar_triggered();
        void on_actionSelection_Drawing_Tools_triggered();
        void on_actionButton_Displays_triggered();
        void on_actionZoom_Toolbar_triggered();
        void on_actionDrawing_Toolbar_triggered();
        void on_actionOrder_Assist_Toolbar_triggered();
        void on_actionPosition_Assist_Toolbar_triggered();
        void on_actionSize_Assist_Toolbar_triggered();
        // Menu: Tools
        void on_actionView_ReadMe_triggered();
        void on_actionPaint_triggered();
        // Menu: Help
        void on_actionContents_triggered();
        void on_actionKeyboard_Map_triggered();
        void on_actionUpdate_triggered();
        void on_actionAbout_TSurface_triggered();

    private:
        QString createTemporaryPath(const QString& name);
        bool createNewFileStructure();
        bool saveAs();
        bool saveNormal();
        bool closeRequest();
        void enableBaseMenus();

        const std::unique_ptr<Ui::tsurface> m_ui;
        bool mForceClose{false};
        QString mLastOpenPath;
        bool mProjectChanged{false};
        bool mHaveProject{false};
        bool mIsSaved{false};
        std::vector<QWidget *> mPageWidgets;
        winCloseEater *mCloseEater{nullptr};
        QAction *mActionStateManager{nullptr};
        QAction *mActionPlay{nullptr};
        QAction *mActionSearch{nullptr};
        QAction *mActionChannels{nullptr};
        bool mToggleChannels{false};                // TRUE = Channels are visible
        QAction *mActionConnectionState{nullptr};
        // Basic information about active project
        QString mPathTemporary;         // The temporary path were all files are plain
};

#endif // TSURFACE_H
