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
#ifndef TGRAPHICS_H
#define TGRAPHICS_H

#include <QString>

#include <vector>

class QJsonObject;

namespace Graphics
{
    typedef enum SLIDER_GRTYPE_t
    {
        SGR_TOP,
        SGR_BOTTOM,
        SGR_LEFT,
        SGR_RIGHT,
        SGR_HORIZONTAL,
        SGR_VERTICAL
    }SLIDER_GRTYPE_t;

    typedef enum LINE_TYPE_t
    {
        LT_OFF,
        LT_ON,
        LT_DRAG,
        LT_DROP
    }LINE_TYPE_t;

    typedef enum XELEMENTS_t
    {
        X_NONE,
        X_BORDER,
        X_BORDER_DATA,
        X_BORDER_FAMILY,
        X_BORDER_STYLE,
        X_CURSOR_DATA,
        X_CURSOR_FAMILY,
        X_CURSOR_STYLE,
        X_SLIDER_DATA,
        X_SLIDER_FAMILY,
        X_SLIDER_STYLE,
        X_EFFECT_DATA,
        X_EFFECT_FAMILY,
        X_EFFECT_STYLE,
        X_POPUP_EFFECT_DATA,
        X_POPUP_EFFECT
    }XELEMENTS_t;

    typedef enum
    {
        FT_UNKNOWN,     // Invalid, error
        FT_THEOSYS,     // JSON format internal
        FT_AMXG5,       // Only for reading!
        FT_AMXG4        // Only for reading!
    }FILE_TYPE_t;

    typedef struct FAMILY_t
    {
        QString name;
        std::vector<QString> member;
    }FAMILY_t;

    typedef struct BORDER_STYLE_t
    {
        QString name;
        QString off;
        QString on;
        QString drag;
        QString drop;
        std::vector<int> g3Equiv;

        void init()
        {
            name.clear();
            off.clear();
            on.clear();
            drag.clear();
            drop.clear();
            g3Equiv.clear();
        }
    }BORDER_STYLE_t;

    typedef struct BORDER_DATA_t
    {
        QString name;
        QString baseFile;
        int multiColor;
        int fillTop;
        int fillLeft;
        int fillBottom;
        int fillRight;
        int textTop;
        int textLeft;
        int textBottom;
        int textRight;
        int idealWidth;
        int idealHeight;
        int minHeight;
        int minWidth;
        int incHeight;
        int incWidth;

        void init()
        {
            multiColor = 0;
            fillTop = 0;
            fillLeft = 0;
            fillBottom = 0;
            fillRight = 0;
            textTop = 0;
            textLeft = 0;
            textBottom = 0;
            textRight = 0,
            idealWidth = 0;
            idealHeight = 0;
            minHeight = 0;
            minWidth = 0;
            incHeight = 0;
            incWidth = 0;
        }
    }BORDER_DATA_t;

    typedef struct CURSOR_STYLE_t
    {
        QString name;
        QString baseFile;
        int multiColor;
        std::vector<int> g3Equiv;

        void init()
        {
            name.clear();
            baseFile.clear();
            multiColor = 0;
            g3Equiv.clear();
        }
    }CURSOR_STYLE_t;

    typedef struct SLIDER_STYLE_t
    {
        QString name;
        QString baseFile;
        int multiColor;
        int incRepeat;
        int minSize;
        int fixedSize;
        std::vector<int> g3Equiv;

        void init()
        {
            name.clear();
            baseFile.clear();
            multiColor = 0;
            incRepeat = 0;
            minSize = 0;
            fixedSize = 0;
            g3Equiv.clear();
        }
    }SLIDER_STYLE_t;

    typedef struct EFFECT_STYLE_t
    {
        QString name;
        int number;
        int startx;
        int starty;
        int height;
        int width;
        int cutout;
        QString pixelMap;

        void init()
        {
            number = 0;
            startx = 0;
            starty = 0;
            height = 0;
            width = 0;
            cutout = 0;
        }
    }EFFECT_STYLE_t;

    typedef struct POPUP_EFFECT_t
    {
        QString name;
        int number;
        int valueUsed;

        void init()
        {
            number = 0;
            valueUsed = 0;
        }
    }POPUP_EFFECT_t;

    typedef struct DRAW_t
    {
        std::vector<FAMILY_t> borders;
        std::vector<BORDER_STYLE_t> borderStyles;
        std::vector<BORDER_DATA_t> borderData;
        std::vector<FAMILY_t> cursors;
        std::vector<CURSOR_STYLE_t> cursorStyles;
        std::vector<FAMILY_t> sliders;
        std::vector<SLIDER_STYLE_t> sliderStyles;
        std::vector<FAMILY_t> effects;
        std::vector<EFFECT_STYLE_t> effectStyles;
        std::vector<POPUP_EFFECT_t> popupEffects;
    }DRAW_t;

    typedef struct BORDER_t
    {
        QString bl;         // bottom left corner
        QString b;          // bottom
        QString br;         // bottom right corner
        QString r;          // right
        QString tr;         // top right corner
        QString t;          // top
        QString tl;         // top left corner
        QString l;          // left
        QString bl_alpha;   // bottom left corner
        QString b_alpha;    // bottom
        QString br_alpha;   // bottom right corner
        QString r_alpha;    // right
        QString tr_alpha;   // top right corner
        QString t_alpha;    // top
        QString tl_alpha;   // top left corner
        QString l_alpha;    // left
        BORDER_DATA_t border;   // Border data
        BORDER_STYLE_t bdStyle; // The border style data
    }BORDER_t;

    typedef struct SLIDER_t
    {
        SLIDER_GRTYPE_t type;   //<! The type of the file the path is pointing to
        QString path;           //<! The path and file name of the graphics mask file.
        QString pathAlpha;      //<! The path and file name of the graphics file containing the alpha part of the image.
    }SLIDER_t;

    typedef struct CURSOR_t
    {
        QString imageBase;  // The base image file.
        QString imageAlpha; // The alpha image file.
    }CURSOR_t;


}

class TGraphics
{
    public:
        TGraphics();

        static TGraphics& Current();
        QStringList getBorderNames();
        bool writeSystemFiles(Graphics::FILE_TYPE_t ft, const QString& basePath);

    protected:
        void initialize();
        void initBorderFamily();
        void initBorderStyle();
        void initBorderData();
        void initCursorFamily();
        void initCursorStyle();
        void initSliderFamily();
        void initSliderStyle();
        void initEffectFamily();
        void initEffectStyle();
        void initPopupEffect();
        QJsonObject writeSystemBordersJson();
        QJsonObject writeSystemCursorsJson();
        QJsonObject writeSystemSlidersJson();
        QJsonObject writeSystemEffectsJson();
        QJsonObject writeSystemPopupsJson();
        bool writeBorderResources(const QString& path);
        bool writeSliderResources(const QString& path);
        bool writeFontResources(const QString& path);
        bool writeSoundResources(const QString& path);

    private:
        static TGraphics *mCurrent;

        bool mInitialized{false};
        Graphics::DRAW_t mDraw;
};

#endif // TGRAPHICS_H
