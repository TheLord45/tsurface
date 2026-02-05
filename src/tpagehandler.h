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
#ifndef TPAGEHANDLER_H
#define TPAGEHANDLER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QColor>

#include "tobjecthandler.h"
#include "tmisc.h"

class TCanvasWidget;
class QMdiArea;

namespace Page
{
    enum PAGE_TYPE
    {
        PT_UNKNOWN,
        PT_PAGE,
        PT_POPUP,
        PT_SUBPAGE
    };

    enum SHOWEFFECT
    {
        SE_NONE,
        SE_FADE,
        SE_SLIDE_LEFT,
        SE_SLIDE_RIGHT,
        SE_SLIDE_TOP,
        SE_SLIDE_BOTTOM,
        SE_SLIDE_LEFT_FADE,
        SE_SLIDE_RIGHT_FADE,
        SE_SLIDE_TOP_FADE,
        SE_SLIDE_BOTTOM_FADE
    };

    // The following enum is for G5 only
    typedef enum
    {
        EV_NONE,                        // Invalid event
        EV_PGFLIP,                      // Do a page flip
        EV_COMMAND,                     // Execute a command (can be a self feed command)
        EV_LAUNCH                       // Launch an external application.
    }EVENT_TYPE_t;

    typedef struct EVENT_t
    {
        EVENT_TYPE_t evType{EV_NONE};
        int item{0};                    // The position; Lines are sorted by this number
        QString evAction;               // The action to take on type EV_PGFLIP (sShow, ...).
        QString name;                   // The page/program to take the action on if type is EV_PGFLIP or EV_LAUNCH
        int ID{0};                      // The ID of an application to launch
        int port{0};                    // The port number if the type is EV_COMMAND.
    }EVENT_t;

    typedef enum
    {
        COL_CLOSED,
        COL_SMALL,
        COL_FULL
    }COLLAPS_STATE_t;
    // Collapse direction (animation)
    typedef enum
    {
        COLDIR_NONE,                    // Not collapsible
        COLDIR_LEFT,                    // Collapse to left
        COLDIR_RIGHT,                   // Collapse to right
        COLDIR_UP,                      // Collapse up
        COLDIR_DOWN                     // Collapse down
    }COLDIR_t;

    /**
     * @brief struct SR_t
     * This structure is for a page or popup only and defines the background.
     * Therefore it is shortened and does not contain all features an object
     * may contain.
     */
    typedef struct SR_t
    {
        QString bs;             // Frame type (circle, ...)
        QString mi;             // Chameleon image
        int mi_width{0};        // Width of image
        int mi_height{0};       // Height of image
        QColor cb;              // Border color
        QString ft;             // G5: Fill type for gradient colors.
        QColor cf;              // Fill color
        QColor ct;              // Text Color
        QColor ec;              // Text effect color
        QString bm;             // bitmap file name
        QList<ObjHandler::BITMAPS_t> bitmaps;   // G5 table of bitmaps. Limited to 5 max.
        QList<QColor> gradientColors;  // G5 optional gradient colors
        int gr{15};             // G5 Gradient radius
        int gx{50};             // G5 Gradient center X in percent
        int gy{50};             // G5 Gradient center Y in percent
        int bm_width{0};        // Width of image
        int bm_height{0};       // Height of image
        bool dynamic{false};    // TRUE = moving image
        int sb{0};              // Index to external graphics download
        int ii{0};              // Icon index number
        int ix{0};              // Icon X position
        int iy{0};              // Icon Y position
        int ji{5};              // Icon style / position like "jt", default 5 = center+middle
        int jb{5};              // Image position (center, left, ...), default 5 = center+middle
        int bx{0};              // Absolute image position x
        int by{0};              // Absolute image position y
        int fi{0};              // Font index
        QString te;             // Text
        ObjHandler::ORIENTATION jt{ObjHandler::ORI_CENTER_MIDDLE}; // Text orientation
        int tx{0};              // Text X position
        int ty{0};              // Text Y position
        QString ff;             // G5 font file name
        int fs{0};              // G5 font size
        int ww{0};              // line break when 1
        int et{0};              // Text effect (^TEF)
        int oo{-1};             // Over all opacity
        QString vf;             // G5: Video fill; Marks the button reserved for video (100 = video stream, 101 = MXA-MPL)
        QString dv;             // G5: Streaming source; Only valid if "vf" is 100!
    } SR_t;

    typedef struct PAGE_t
    {
        ObjHandler::TBASEOBJ_t baseObject;      // Internal use; Contains pointers for drawing
        PAGE_TYPE popupType{PT_UNKNOWN};        // The type of the popup (popup only)
        int pageID{0};                          // Unique ID of popup/page
        QString name;                           // The name of the popup/page
        QString description;                    // Optional description of page
        bool visible{false};                    // Internal use: TRUE = The page/popup is visible as MDI window
        bool gridVisible{false};                // Internal use: TRUE = Grid is visible
        bool snapToGrid{true};                  // Internal use: FALSE = No snap to grid
        int ap{0};                              // Default: 0; Address port
        int ad{1};                              // Default: 1; Address code
        int cp{0};                              // Default: 0; Channel port
        int ch{1};                              // Default: 1; Channel code
        int left{0};                            // Left position of popup (always 0 for pages)
        int top{0};                             // Top position of popup (always 0 for pages)
        int width{0};                           // Width of popup
        int height{0};                          // Height of popup
        int modal{0};                           // 0 = Popup/Page = non modal
        int showLockX{0};                       // G5 ?
        COLDIR_t collapseDirection{COLDIR_NONE};// G5: The direction the popup should move on collapse
        int collapseOffset{0};                  // G5: The offset to collapse to.
        bool collapsible{false};                // G5: Internal use: TRUE = popup is collapsible.
        COLLAPS_STATE_t colState{COL_CLOSED};   // G5: Internal use: Defines the state of a collapsable popup.
        QString group;                          // Name of the group the popup belongs (popup only)
        int timeout{0};                         // Time after the popup hides in 1/10 seconds (popup only)
        SHOWEFFECT showEffect{SE_NONE};         // The effect when the popup is shown (popup only)
        int showTime{0};                        // The time reserved for the show effect (popup only)
        int showX{0};                           // End of show effect position (by default "left+width"); (popup only)
        int showY{0};                           // End of show effect position (by default "top+height"); (popup only)
        SHOWEFFECT hideEffect{SE_NONE};         // The effect when the popup hides (popup only)
        int hideTime{0};                        // The time reserved for the hide effect (popup only)
        int hideX{0};                           // End of hide effect position (by default "left"); (popup only)
        int hideY{0};                           // End of hide effect position (by default "top"); (popup only)
        int resetPos{0};                        // If this is 1 the popup is reset to it's original position and size on open
        SR_t srPage;                            // The configuration for the page itself
        QList<EVENT_t> eventShow;               // G5: Events to start showing
        QList<EVENT_t> eventHide;               // G5: Events to start hiding
        QList<TObjectHandler *> objects;        // The objects belonging to the page/popup
    }PAGE_t;
}

/**
 * @brief The TPageHandler class
 * This class handles all pages and popups. It is filled at the moment
 * a file is read or during the user creates new pages, popups and
 * objects on them.
 * The class handles also the reading and saving of individual pages or
 * popups.
 */
class TPageHandler : public QObject
{
    Q_OBJECT

    public:
        explicit TPageHandler();
        static TPageHandler& Current();

        int createPage(TCanvasWidget *w, Page::PAGE_TYPE pt, const QString& name, int width, int height, int left=0, int top=0);
        int createPage(TCanvasWidget *w, Page::PAGE_TYPE pt, const QString& name, const QRect& geom);
        void reset();
        void setVisible(int number, bool visible);
        Page::PAGE_t getCurrentPage(QMdiArea *area);
        bool bringToFront(int number);
        int getNextPageNumber() { mMaxPageNumber++; return mMaxPageNumber; }
        int getNextPopupNumber() { mMaxPopupNumber++; return mMaxPopupNumber; }
        bool isVisible(int number);
        bool isGridVisible(int number);
        bool isSnapToGrid(int number);
        TCanvasWidget *getWidget(int number);
        void setWidget(TCanvasWidget *w, int number);
        bool saveAllPages();
        bool readPages(const QStringList& list);
        ObjHandler::TOBJECT_t initNewObject(int bi, const QString& name);
        void appendObject(int pageID, TObjectHandler *object);
        // Getter/Setter
        Page::PAGE_t getPage(int number);
        Page::PAGE_t getPage(const QString& name);
        Page::PAGE_t getPopup(const QString& name);
        QList<int> getPageNumbers();
        void setPage(Page::PAGE_t& page);
        void setPageBgColor(int number, QColor& col);
        void setPageTextColor(int number, QColor& col);
        QStringList getGroupNames();
        void setPathTemporary(const QString& path) { mPathTemporary = path; }
        void changePageName(int id, const QString& name);
        void setGridVisible(int number, bool state);
        void setSnapToGrid(int number, bool state);
        void setObject(int num, ObjHandler::TOBJECT_t& object);
        ObjHandler::TOBJECT_t getObject(int page, int bi);
        TObjectHandler *getObjectHandler(int page, int bi);
        void setCurrentState(STATE_TYPE s) { mCurrentState = s; }
        STATE_TYPE getCurrentState() { return mCurrentState; }
        void setSelectedToolToAllPages(TOOL t);
        QList<ObjHandler::TOBJECT_t> getObjectList(const Page::PAGE_t& page);
        void setObjectGeometry(int pageID, int bi, const QRect& geom);

    protected:
        bool savePage(const Page::PAGE_t& page);
        bool savePopup(const Page::PAGE_t& popup);
        QJsonObject getSr(Page::PAGE_TYPE pt, const Page::SR_t& sr, int number=0);
        void parsePage(const QJsonObject& page);
        void parseObjects(Page::PAGE_t *page, const QJsonArray& obj);
        QJsonArray getObjects(const QList<TObjectHandler *>& objects);
        Page::PAGE_t *getPagePointer(int num);

    private:
        static TPageHandler *mCurrent;
        QString mPathTemporary;
        QList<Page::PAGE_t> mPages;
        int mMaxPageNumber{0};
        int mMaxPopupNumber{500};
        STATE_TYPE mCurrentState{STATE_UNKNOWN};
};

#endif // TPAGEHANDLER_H
