/*
 * Copyright (C) 2026 by Andreas Theofilu <andreas@theosys.at>
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
#ifndef TOBJECTHANDLER_H
#define TOBJECTHANDLER_H

#include <QString>
#include <QColor>

#include "tresizablewidget.h"

class TCanvasWidget;
class QLabel;

namespace ObjHandler
{
    typedef enum ORIENTATION
    {
        ORI_ABSOLUT,
        ORI_TOP_LEFT,
        ORI_TOP_MIDDLE,
        ORI_TOP_RIGHT,
        ORI_CENTER_LEFT,
        ORI_CENTER_MIDDLE,      // default
        ORI_CENTER_RIGHT,
        ORI_BOTTOM_LEFT,
        ORI_BOTTOM_MIDDLE,
        ORI_BOTTOM_RIGHT,
        ORI_SCALE_FIT,          // G5 scale to fit
        ORI_SCALE_ASPECT        // G5 scale maintain aspect ratio
    }ORIENTATION;

    typedef enum TEXT_EFFECT
    {
        EFFECT_NONE,
        EFFECT_OUTLINE_S,
        EFFECT_OUTLINE_M,
        EFFECT_OUTLINE_L,
        EFFECT_OUTLINE_X,
        EFFECT_GLOW_S,
        EFFECT_GLOW_M,
        EFFECT_GLOW_L,
        EFFECT_GLOW_X,
        EFFECT_SOFT_DROP_SHADOW_1,
        EFFECT_SOFT_DROP_SHADOW_2,
        EFFECT_SOFT_DROP_SHADOW_3,
        EFFECT_SOFT_DROP_SHADOW_4,
        EFFECT_SOFT_DROP_SHADOW_5,
        EFFECT_SOFT_DROP_SHADOW_6,
        EFFECT_SOFT_DROP_SHADOW_7,
        EFFECT_SOFT_DROP_SHADOW_8,
        EFFECT_MEDIUM_DROP_SHADOW_1,
        EFFECT_MEDIUM_DROP_SHADOW_2,
        EFFECT_MEDIUM_DROP_SHADOW_3,
        EFFECT_MEDIUM_DROP_SHADOW_4,
        EFFECT_MEDIUM_DROP_SHADOW_5,
        EFFECT_MEDIUM_DROP_SHADOW_6,
        EFFECT_MEDIUM_DROP_SHADOW_7,
        EFFECT_MEDIUM_DROP_SHADOW_8,
        EFFECT_HARD_DROP_SHADOW_1,
        EFFECT_HARD_DROP_SHADOW_2,
        EFFECT_HARD_DROP_SHADOW_3,
        EFFECT_HARD_DROP_SHADOW_4,
        EFFECT_HARD_DROP_SHADOW_5,
        EFFECT_HARD_DROP_SHADOW_6,
        EFFECT_HARD_DROP_SHADOW_7,
        EFFECT_HARD_DROP_SHADOW_8,
        EFFECT_SOFT_DROP_SHADOW_1_WITH_OUTLINE,
        EFFECT_SOFT_DROP_SHADOW_2_WITH_OUTLINE,
        EFFECT_SOFT_DROP_SHADOW_3_WITH_OUTLINE,
        EFFECT_SOFT_DROP_SHADOW_4_WITH_OUTLINE,
        EFFECT_SOFT_DROP_SHADOW_5_WITH_OUTLINE,
        EFFECT_SOFT_DROP_SHADOW_6_WITH_OUTLINE,
        EFFECT_SOFT_DROP_SHADOW_7_WITH_OUTLINE,
        EFFECT_SOFT_DROP_SHADOW_8_WITH_OUTLINE,
        EFFECT_MEDIUM_DROP_SHADOW_1_WITH_OUTLINE,
        EFFECT_MEDIUM_DROP_SHADOW_2_WITH_OUTLINE,
        EFFECT_MEDIUM_DROP_SHADOW_3_WITH_OUTLINE,
        EFFECT_MEDIUM_DROP_SHADOW_4_WITH_OUTLINE,
        EFFECT_MEDIUM_DROP_SHADOW_5_WITH_OUTLINE,
        EFFECT_MEDIUM_DROP_SHADOW_6_WITH_OUTLINE,
        EFFECT_MEDIUM_DROP_SHADOW_7_WITH_OUTLINE,
        EFFECT_MEDIUM_DROP_SHADOW_8_WITH_OUTLINE,
        EFFECT_HARD_DROP_SHADOW_1_WITH_OUTLINE,
        EFFECT_HARD_DROP_SHADOW_2_WITH_OUTLINE,
        EFFECT_HARD_DROP_SHADOW_3_WITH_OUTLINE,
        EFFECT_HARD_DROP_SHADOW_4_WITH_OUTLINE,
        EFFECT_HARD_DROP_SHADOW_5_WITH_OUTLINE,
        EFFECT_HARD_DROP_SHADOW_6_WITH_OUTLINE,
        EFFECT_HARD_DROP_SHADOW_7_WITH_OUTLINE,
        EFFECT_HARD_DROP_SHADOW_8_WITH_OUTLINE
    }TEXT_EFFECT;

    typedef enum DRAW_ORDER
    {
        ORD_ELEM_NONE,
        ORD_ELEM_FILL,
        ORD_ELEM_BITMAP,
        ORD_ELEM_ICON,
        ORD_ELEM_TEXT,
        ORD_ELEM_BORDER
    }DRAW_ORDER;

    typedef enum FEEDBACK
    {
        FB_NONE,
        FB_CHANNEL,
        FB_INV_CHANNEL,     // inverted channel
        FB_ALWAYS_ON,
        FB_MOMENTARY,
        FB_BLINK
    } FEEDBACK;

    typedef enum SUBVIEW_POSITION_t
    {
        SVP_CENTER,
        SVP_LEFT_TOP,
        SVP_RIGHT_BOTTOM
    }SUBVIEW_POSITION_t;

    typedef enum BUTTON_ACTION_t
    {
        BT_ACTION_LAUNCH,
        BT_ACTION_PGFLIP,
        BT_ACTION_COMMAND
    }BUTTON_ACTION_t;

    /**
     * @brief G5 button events
     * This defines the possible event types on a button.
     */
    typedef enum BUTTON_EVENT_t
    {
        EVENT_NONE,                 // No event
        EVENT_PRESS,                // Event on button press
        EVENT_RELEASE,              // Event on button release
        EVENT_GUESTURE_ANY,         // Event on any guesture
        EVENT_GUESTURE_UP,          // Event on 1 finger up guesture
        EVENT_GUESTURE_DOWN,        // Event on 1 finger down guesture
        EVENT_GUESTURE_LEFT,        // Event on 1 finger left guesture
        EVENT_GUESTURE_RIGHT,       // Event on 1 finger right guesture
        EVENT_GUESTURE_DBLTAP,      // Event on double tap
        EVENT_GUESTURE_2FUP,        // Event on 2 finger up guesture
        EVENT_GUESTURE_2FDN,        // Event on 2 finger down guesture
        EVENT_GUESTURE_2FLT,        // Event on 2 finger left guesture
        EVENT_GUESTURE_2FRT         // Event on 2 finger right guesture
    }BUTTON_EVENT_t;

    typedef enum
    {
        GRAD_SOLID,                 //  0: Solid; No gradient!
        GRAD_SWEEP,                 //  1: Sweep
        GRAD_RADIAL,                //  2: Radial
        GRAD_CLCR,                  //  3: Left to right
        GRAD_TLBR,                  //  4: Top-left to Bottom-right
        GRAD_CTCB,                  //  5: Top to bottom
        GRAD_TRBL,                  //  6: Top-right to Bottom-left
        GRAD_CRCL,                  //  7: Right to left
        GRAD_BRTL,                  //  8: Bottom-right to top-left
        GRAD_CBCT,                  //  9: Bottom to top
        GRAD_BLTR                   // 10: Bottom-left to top-right
    }GRAD_TYPE_t;

    typedef enum BUTTONTYPE
    {
        NONE,
        GENERAL,
        MULTISTATE_GENERAL,
        BARGRAPH,
        MULTISTATE_BARGRAPH,
        JOYSTICK,
        TEXT_INPUT,
        LISTBOX,
        COMPUTER_CONTROL,
        TAKE_NOTE,
        SUBPAGE_VIEW
    } BUTTONTYPE;

    /**
     * Justification values:
     *    0 = absolut
     *    1 = top right
     *    2 = top middle
     *    3 = top right
     *    4 = center left
     *    5 = center middle (default)
     *    6 = center right
     *    7 = bottom left
     *    8 = bottom middle
     *    9 = bottom right
     *   10 = scale to fit (ignore aspect ration)
     *   11 = scale maintain aspect ratio
     */
    typedef struct BITMAPS_t    // G5 bitmap entry
    {
        QString fileName;       // file name of the bitmap (replaces icons)
        bool dynamic{false};    // Marks an image as a dynamic image (Video, camera, ...)
        int index{-1};          // The bitmap index number
        ORIENTATION justification{ORI_CENTER_MIDDLE};   // Justification of bitmap
        int offsetX{0};         // Absolut X position (only if justification is 0)
        int offsetY{0};         // Absolut Y position (only if justification is 0)
        int width{0};           // Internal: The width of the image
        int height{0};          // Internal: The height of the image
    }BITMAPS_t;

    typedef struct SR_T
    {
        int number{0};
        QString _do;            // Order on how to show a multistate bargraph (010203...)
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
        QList<BITMAPS_t> bitmaps;   // G5 table of bitmaps. Limited to 5 max.
        QList<QString> gradientColors;  // G5 optional gradient colors
        int gr{15};             // G5 Gradient radius
        int gx{50};             // G5 Gradient center X in percent
        int gy{50};             // G5 Gradient center Y in percent
        QString sd;             // Sound file to play
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
        ORIENTATION jt{ORI_CENTER_MIDDLE}; // Text orientation
        int tx{0};              // Text X position
        int ty{0};              // Text Y position
        QString ff;             // G5 font file name
        int fs{0};              // G5 font size
        int ww{0};              // line break when 1
        int et{0};              // Text effect (^TEF)
        int oo{-1};             // Over all opacity
        int md{0};              // Marquee type: 1 = scroll left, 2 = scroll right, 3 = ping pong, 4 = scroll up, 5 = scroll down
        int mr{0};              // Marquee enabled: 1 = enabled, 0 = disabled
        int ms{1};              // Marquee speed: Range: 1 to 10 (look for command ^MSP to see of how to set this)
        QString vf;             // G5: Video fill; Marks the button reserved for video (100 = video stream, 101 = MXA-MPL)
        QString dv;             // G5: Streaming source; Only valid if "vf" is 100!
    } SR_T;

    typedef struct EXTBUTTON_t
    {
        BUTTONTYPE type;
        int bi{0};              // button ID
        QString na;         // name
        int lt{0};              // pixel from left
        int tp{0};              // pixel from top
        int wt{0};              // width
        int ht{0};              // height
        int zo{0};              // Z-Order
        QString hs;         // bounding, ...
        QString bs;         // Border style (circle, ...)
        FEEDBACK fb{FB_NONE};   // Feedback type (momentary, ...)
        int ap{1};              // Address port (default: 1)
        int ad{0};              // Address channel
        int lp{1};              // Level port (default: 1)
        int lv{0};              // Level code
        QString dr;         // Level "horizontal" or "vertical"
        int lu{0};              // Animate time up (Bargraph)
        int ld{0};              // Animate time down (Bargraph)
        int rl{0};              // Range low
        int rh{0};              // Range high
        int rn{0};              // Bargraph: Range drag increment
        QString sc;         // Color of slider (for bargraph)
        QList<SR_T> sr;
    }EXTBUTTON_t;

    typedef struct PUSH_FUNC
    {
        int item{0};            // G5: Item number
        QString pfType;     // command to execute when button was pushed
        QString pfAction;   // G5: Action; Used for launching external apps
        QString pfName;     // Name of popup
        BUTTON_ACTION_t action{BT_ACTION_PGFLIP};   // G5: Button action (page flip, ...)
        int ID{0};              // G5: An ID for launch buttons
        BUTTON_EVENT_t event{EVENT_NONE};   // G5: Type of event
    }PUSH_FUNC_T;

    typedef enum CENTER_CODE
    {
        SC_ICON = 0,
        SC_BITMAP,
        SC_TEXT
    }CENTER_CODE;

    typedef struct POSITION_t
    {
        int width{0};
        int height{0};
        int left{1};
        int top{1};
        bool overflow{false};
        bool valid{false};
    }POSITION_t;

    typedef struct POINT_t
    {
        int x{0};
        int y{0};
    }POINT_t;

    typedef struct IMAGE_SIZE_t
    {
        int width{0};
        int height{0};
    }IMAGE_SIZE_t;

    typedef struct BITMAP_t
    {
        unsigned char *buffer{nullptr};
        int left{0};
        int top{0};
        int width{0};
        int height{0};
        size_t rowBytes{0};
    }BITMAP_t;

#   define LIST_IMAGE_CELL     1
#   define LIST_TEXT_PRIMARY   2
#   define LIST_TEXT_SECONDARY 4

    typedef enum LIST_SORT
    {
        LIST_SORT_NONE,
        LIST_SORT_ASC,
        LIST_SORT_DESC,
        LIST_SORT_OVERRIDE
    }LIST_SORT;

    typedef struct TBASEOBJ_t
    {
        TCanvasWidget *widget{nullptr};
        QLabel *qobject{nullptr};
        QPixmap pixmap;
    }TBASEOBJ_t;

    typedef struct TOBJECT_t
    {
        TBASEOBJ_t baseObject;  // Internal use: Pointer to object.
        BUTTONTYPE type;        // The type of the button
        int bi{0};              // button ID
        QString na;             // name
        QString bd;             // Description --> ignored
        int lt{0};              // pixel from left
        int tp{0};              // pixel from top
        int wt{0};              // width
        int ht{0};              // height
        int zo{0};              // Z-Order
        QString hs;             // bounding, ...
        QString bs;             // Border style (circle, ...)
        FEEDBACK fb{FB_NONE};   // Feedback type (momentary, ...)
        int ap{1};              // Address port (default: 1)
        int ad{0};              // Address channel
        int ch{0};              // Channel number
        int cp{1};              // Channel port (default: 1)
        int lp{1};              // Level port (default: 1)
        int lv{0};              // Level code
        int ta{0};              // Listbox table channel
        int ti{0};              // Listbox table index number of rows (all rows have this number in "cp")
        int tr{0};              // Listbox number of rows
        int tc{0};              // Listbox number of columns
        int tj{0};              // Listbox row height
        int tk{0};              // Listbox preferred row height
        int of{0};              // Listbox list offset: 0=disabled/1=enabled
        int tg{0};              // Listbox managed: 0=no/1=yes
        int so{1};              // String output port
        int co{1};              // Command port
        QList<QString> cm;      // Commands to send on each button hit
        QString dr;             // Level "horizontal" or "vertical"
        int va{0};              // Level control value
        int stateCount{0};      // State count with multistate buttons (number of states)
        int rm{0};              // State count with multistate buttons?
        int nu{2};              // Animate time up
        int nd{2};              // Animate time down
        int ar{0};              // Auto repeat (1 = true)
        int ru{2};              // Animate time up (bargraph)
        int rd{2};              // Animate time down (bargraph)
        int lu{2};              // Animate time up (Bargraph)
        int ld{2};              // Animate time down (Bargraph)
        int rv{0};              // Level control repeat time
        int rl{0};              // Range low
        int rh{0};              // Range high
        int ri{0};              // Bargraph inverted (0 = normal, 1 = inverted)
        int ji{0};              // Joystick aux inverted (0 = normal, 1 = inverted)
        int rn{0};              // Bargraph: Range drag increment
        int ac_di{0};           // (guess) Direction of text: 0 = left to right (default); 1 = right to left
        int hd{0};              // 1 = Hidden, 0 = Normal visible
        int da{0};              // 1 = Disabled, 0 = Normal active
        int pp{0};              // >0 = password protected; Range 1 to 4
        QString lf;             // Bargraph function: empty = display only, active, active centering, drag, drag centering
        QString sd;             // Name/Type of slider for a bargraph
        QString vt;             // Level control type (rel = relative, abs = absolute)
        QString cd;             // Name of cursor for a joystick
        QString sc;             // Color of slider (for bargraph)
        QString cc;             // Color of cursor (for joystick)
        int mt{0};              // Length of text area (0 = 2000)
        QString dt;             // "multiple" textarea has multiple lines, else single line
        QString im;             // Input mask of a text area
        int st{0};              // SubPageView: ID of subview list
        int ws{0};              // SubPageView: Wrap subpages; 1 = YES
        QString on;             // SubPageView: direction: vert = vertical, if empty: horizontal which is default
        int sa{0};              // SubPageView: Percent of space between items in list
        int dy{0};              // SubPageView: Allow dynamic reordering; 1 = YES --> A user can reorder subview pages (currently not supported)
        int rs{0};              // SubPageView: Reset view on show; 1 = YES
        int ba{0};              // SubPageView: 1 = Scrollbar is visible, 0 = No scrollbar visible
        int bo{0};              // SubPageView: Scrollbar offset in pixels; Only valid if "ba" > 0
        int sw{1};              // SubPageView: G5: Show subpages; 1 = YES (default), 0 = NO
        QString we;             // SubPageView: Anchor position: Empty = Center, "l/t" = left/top, "r/b" = right/bottom
        QString pc;             // Password character for text area
        QString op;             // String the button send
        bool visible{true};     // TRUE=Button is visible
        QList<PUSH_FUNC_T> pushFunc;  // Push functions: This are executed on button press
        QList<SR_T> sr;         // The elements the button consists of
    }TOBJECT_t;
};

class TObjectHandler
{
    public:
        TObjectHandler();
        TObjectHandler(ObjHandler::BUTTONTYPE bt, int num, const QString& name);

        int getButtonIndex() { return mObject.bi; }
        void setObject(TCanvasWidget *w) { mObject.baseObject.widget = w; }
        void setObject(const ObjHandler::TOBJECT_t& object) { mObject = object; }
        ObjHandler::TOBJECT_t& getObject() { return mObject; }
        TCanvasWidget *getObjectWidget() { return mObject.baseObject.widget; }

        inline void setSize(const QRect& rect)
        {
            mObject.lt = rect.left();
            mObject.tp = rect.top();
            mObject.wt = rect.width();
            mObject.ht = rect.height();
        }

        ObjHandler::SR_T getSr(int number);
        QList<ObjHandler::SR_T>& getSrList() { return mObject.sr; }

    private:
        ObjHandler::TOBJECT_t mObject;
};

#endif // TOBJECTHANDLER_H
