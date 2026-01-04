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
#include <QStringList>

#include "terror.h"
#include "tgraphics.h"

TGraphics *TGraphics::mCurrent{nullptr};

using namespace Graphics;
using std::vector;

TGraphics::TGraphics()
{
    DECL_TRACER("TBorders::TBorders()");

    initialize();
}

TGraphics& TGraphics::Current()
{
    DECL_TRACER("TBorders::Current()");

    if (!mCurrent)
        mCurrent = new TGraphics;

    return *mCurrent;
}

void TGraphics::initialize()
{
    DECL_TRACER("TGraphics::initialize()");

    initBorderFamily();
}

void TGraphics::initBorderFamily()
{
    DECL_TRACER("TGraphics::initBorderFamily();");

    if (mInitialized)
        return;

    FAMILY_t family;
    family.name = "2D Standard";
    family.member.push_back("Single Line");
    family.member.push_back("Double Line");
    family.member.push_back("Quad Line");
    family.member.push_back("Picture Frame");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Circles";
    family.member.push_back("Circle 15");
    family.member.push_back("Circle 25");
    family.member.push_back("Circle 35");
    family.member.push_back("Circle 45");
    family.member.push_back("Circle 55");
    family.member.push_back("Circle 65");
    family.member.push_back("Circle 75");
    family.member.push_back("Circle 85");
    family.member.push_back("Circle 95");
    family.member.push_back("Circle 105");
    family.member.push_back("Circle 115");
    family.member.push_back("Circle 125");
    family.member.push_back("Circle 135");
    family.member.push_back("Circle 145");
    family.member.push_back("Circle 155");
    family.member.push_back("Circle 165");
    family.member.push_back("Circle 175");
    family.member.push_back("Circle 185");
    family.member.push_back("Circle 195");
    mDraw.borders.push_back(family);


    family.member.clear();
    family.name = "Ovals -Horizontal";
    family.member.push_back("Oval H 60x30");
    family.member.push_back("Oval H 100x50");
    family.member.push_back("Oval H 150x75");
    family.member.push_back("Oval H 200x100");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Ovals -Vertical";
    family.member.push_back("Oval V 30x60");
    family.member.push_back("Oval V 50x100");
    family.member.push_back("Oval V 75x150");
    family.member.push_back("Oval V 100x200");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Diamonds";
    family.member.push_back("Diamond 15");
    family.member.push_back("Diamond 25");
    family.member.push_back("Diamond 35");
    family.member.push_back("Diamond 45");
    family.member.push_back("Diamond 55");
    family.member.push_back("Diamond 65");
    family.member.push_back("Diamond 75");
    family.member.push_back("Diamond 85");
    family.member.push_back("Diamond 95");
    family.member.push_back("Diamond 105");
    family.member.push_back("Diamond 115");
    family.member.push_back("Diamond 125");
    family.member.push_back("Diamond 135");
    family.member.push_back("Diamond 145");
    family.member.push_back("Diamond 155");
    family.member.push_back("Diamond 165");
    family.member.push_back("Diamond 175");
    family.member.push_back("Diamond 185");
    family.member.push_back("Diamond 195");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "AMX Elite";
    family.member.push_back("AMX Elite -L");
    family.member.push_back("AMX Elite -M");
    family.member.push_back("AMX Elite -S");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Bevel";
    family.member.push_back("Bevel -L");
    family.member.push_back("Bevel -M");
    family.member.push_back("Bevel -S");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Double Bevel";
    family.member.push_back("Double Bevel -L");
    family.member.push_back("Double Bevel -M");
    family.member.push_back("Double Bevel -S");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Neon";
    family.member.push_back("Neon Inactive -S");
    family.member.push_back("Neon Inactive -L");
    family.member.push_back("Neon Active -S");
    family.member.push_back("Neon Active -L");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Glow";
    family.member.push_back("Glow -S");
    family.member.push_back("Glow -L");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Fuzzy";
    family.member.push_back("Fuzzy");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Popups";
    family.member.push_back("Windows Style Popup");
    family.member.push_back("Windows Style Popup (Status Bar)");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Cursor Controls";
    family.member.push_back("Cursor Top");
    family.member.push_back("Cursor Right");
    family.member.push_back("Cursor Left");
    family.member.push_back("Cursor Bottom");
    family.member.push_back("Cursor Top with Hole");
    family.member.push_back("Cursor Right with Hole");
    family.member.push_back("Cursor Left with Hole");
    family.member.push_back("Cursor Bottom with Hole");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Misc";
    family.member.push_back("Custom Frame");
    family.member.push_back("Help Down");
    family.member.push_back("Help Down Reversed");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Smart Button Bubbled";
    family.member.push_back("Smart Button Bubbled Very Large");
    family.member.push_back("Smart Button Bubbled Large");
    family.member.push_back("Smart Button Bubbled Medium");
    family.member.push_back("Smart Button Bubbled Small");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Smart Button Squared";
    family.member.push_back("Smart Button Squared Large");
    family.member.push_back("Smart Button Squared Medium");
    family.member.push_back("Smart Button Squared Small");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "News Header";
    family.member.push_back("News Header");
    family.member.push_back("News Header Right");
    family.member.push_back("News Header Left");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Menu Rounded Spacers";
    family.member.push_back("Menu Rounded Spacer -Vertical");
    family.member.push_back("Menu Rounded Spacer -Horizontal");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Menu Rounded -Left";
    family.member.push_back("Menu Left Rounded 15");
    family.member.push_back("Menu Left Rounded 25");
    family.member.push_back("Menu Left Rounded 35");
    family.member.push_back("Menu Left Rounded 45");
    family.member.push_back("Menu Left Rounded 55");
    family.member.push_back("Menu Left Rounded 65");
    family.member.push_back("Menu Left Rounded 75");
    family.member.push_back("Menu Left Rounded 85");
    family.member.push_back("Menu Left Rounded 95");
    family.member.push_back("Menu Left Rounded 105");
    family.member.push_back("Menu Left Rounded 115");
    family.member.push_back("Menu Left Rounded 125");
    family.member.push_back("Menu Left Rounded 135");
    family.member.push_back("Menu Left Rounded 145");
    family.member.push_back("Menu Left Rounded 155");
    family.member.push_back("Menu Left Rounded 165");
    family.member.push_back("Menu Left Rounded 175");
    family.member.push_back("Menu Left Rounded 185");
    family.member.push_back("Menu Left Rounded 195");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Menu Rounded -Right";
    family.member.push_back("Menu Right Rounded 15");
    family.member.push_back("Menu Right Rounded 25");
    family.member.push_back("Menu Right Rounded 35");
    family.member.push_back("Menu Right Rounded 45");
    family.member.push_back("Menu Right Rounded 55");
    family.member.push_back("Menu Right Rounded 65");
    family.member.push_back("Menu Right Rounded 75");
    family.member.push_back("Menu Right Rounded 85");
    family.member.push_back("Menu Right Rounded 95");
    family.member.push_back("Menu Right Rounded 105");
    family.member.push_back("Menu Right Rounded 115");
    family.member.push_back("Menu Right Rounded 125");
    family.member.push_back("Menu Right Rounded 135");
    family.member.push_back("Menu Right Rounded 145");
    family.member.push_back("Menu Right Rounded 155");
    family.member.push_back("Menu Right Rounded 165");
    family.member.push_back("Menu Right Rounded 175");
    family.member.push_back("Menu Right Rounded 185");
    family.member.push_back("Menu Right Rounded 195");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Menu Rounded -Top";
    family.member.push_back("Menu Top Rounded 15");
    family.member.push_back("Menu Top Rounded 25");
    family.member.push_back("Menu Top Rounded 35");
    family.member.push_back("Menu Top Rounded 45");
    family.member.push_back("Menu Top Rounded 55");
    family.member.push_back("Menu Top Rounded 65");
    family.member.push_back("Menu Top Rounded 75");
    family.member.push_back("Menu Top Rounded 85");
    family.member.push_back("Menu Top Rounded 95");
    family.member.push_back("Menu Top Rounded 105");
    family.member.push_back("Menu Top Rounded 115");
    family.member.push_back("Menu Top Rounded 125");
    family.member.push_back("Menu Top Rounded 135");
    family.member.push_back("Menu Top Rounded 145");
    family.member.push_back("Menu Top Rounded 155");
    family.member.push_back("Menu Top Rounded 165");
    family.member.push_back("Menu Top Rounded 175");
    family.member.push_back("Menu Top Rounded 185");
    family.member.push_back("Menu Top Rounded 195");
    mDraw.borders.push_back(family);

    family.member.clear();
    family.name = "Menu Rounded -Bottom";
    family.member.push_back("Menu Bottom Rounded 15");
    family.member.push_back("Menu Bottom Rounded 25");
    family.member.push_back("Menu Bottom Rounded 35");
    family.member.push_back("Menu Bottom Rounded 45");
    family.member.push_back("Menu Bottom Rounded 55");
    family.member.push_back("Menu Bottom Rounded 65");
    family.member.push_back("Menu Bottom Rounded 75");
    family.member.push_back("Menu Bottom Rounded 85");
    family.member.push_back("Menu Bottom Rounded 95");
    family.member.push_back("Menu Bottom Rounded 105");
    family.member.push_back("Menu Bottom Rounded 115");
    family.member.push_back("Menu Bottom Rounded 125");
    family.member.push_back("Menu Bottom Rounded 135");
    family.member.push_back("Menu Bottom Rounded 145");
    family.member.push_back("Menu Bottom Rounded 155");
    family.member.push_back("Menu Bottom Rounded 165");
    family.member.push_back("Menu Bottom Rounded 175");
    family.member.push_back("Menu Bottom Rounded 185");
    family.member.push_back("Menu Bottom Rounded 195");
    mDraw.borders.push_back(family);
}

void TGraphics::initBorderStyle()
{
    DECL_TRACER("TGraphics::initBorderStyle()");

    BORDER_STYLE_t style;
    style.name = "Single Line";
    style.off = "Single Line";
    style.on = "Single Line";
    style.drag = "Single Line";
    style.drop = "Single Line";
    style.g3Equiv.push_back(2);
    style.g3Equiv.push_back(7);
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Double Line";
    style.off = "Double Line";
    style.on = "Double Line";
    style.drag = "Double Line";
    style.drop = "Double Line";
    style.g3Equiv.push_back(3);
    style.g3Equiv.push_back(8);
    style.g3Equiv.push_back(12);
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Quad Line";
    style.off = "Quad Line";
    style.on = "Quad Line";
    style.drag = "Quad Line";
    style.drop = "Quad Line";
    style.g3Equiv.push_back(4);
    style.g3Equiv.push_back(9);
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Picture Frame";
    style.off = "Picture Frame";
    style.on = "Picture Frame";
    style.drag = "Picture Frame";
    style.drop = "Picture Frame";
    style.g3Equiv.push_back(10);
    style.g3Equiv.push_back(11);
    mDraw.borderStyles.push_back(style);

    style.init();
}

QStringList TGraphics::getBorderNames()
{
    DECL_TRACER("TGraphics::getBorderNames()");

    QStringList list;
    vector<FAMILY_t>::iterator iter;

    for (iter = mDraw.borders.begin(); iter != mDraw.borders.end(); ++iter)
    {
        vector<QString>::iterator iterMember;

        for (iterMember = iter->member.begin(); iterMember != iter->member.end(); ++iterMember)
            list.append(*iterMember);
    }

    return list;
}
