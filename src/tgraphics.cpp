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
#include <filesystem>

#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include "terror.h"
#include "tgraphics.h"

TGraphics *TGraphics::mCurrent{nullptr};

namespace fs = std::filesystem;
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
    initBorderStyle();
    initBorderData();
    initCursorFamily();
    initCursorStyle();
    initSliderFamily();
    initSliderStyle();
    initEffectFamily();
    initEffectStyle();
    initPopupEffect();
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
    style.name = "Fuzzy";
    style.off = "Fuzzy";
    style.on = "Fuzzy";
    style.drag = "Fuzzy";
    style.drop = "Fuzzy";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 15";
    style.off = "Circle 15";
    style.on = "Circle 15";
    style.drag = "Circle 15";
    style.drop = "Circle 15";
    style.g3Equiv.push_back(5);
    style.g3Equiv.push_back(6);
    style.g3Equiv.push_back(22);
    style.g3Equiv.push_back(23);
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 25";
    style.off = "Circle 25";
    style.on = "Circle 25";
    style.drag = "Circle 25";
    style.drop = "Circle 25";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 35";
    style.off = "Circle 35";
    style.on = "Circle 35";
    style.drag = "Circle 35";
    style.drop = "Circle 35";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 45";
    style.off = "Circle 45";
    style.on = "Circle 45";
    style.drag = "Circle 45";
    style.drop = "Circle 45";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 55";
    style.off = "Circle 55";
    style.on = "Circle 55";
    style.drag = "Circle 55";
    style.drop = "Circle 55";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 65";
    style.off = "Circle 65";
    style.on = "Circle 65";
    style.drag = "Circle 65";
    style.drop = "Circle 65";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 75";
    style.off = "Circle 75";
    style.on = "Circle 75";
    style.drag = "Circle 75";
    style.drop = "Circle 75";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 85";
    style.off = "Circle 85";
    style.on = "Circle 85";
    style.drag = "Circle 85";
    style.drop = "Circle 85";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 95";
    style.off = "Circle 95";
    style.on = "Circle 95";
    style.drag = "Circle 95";
    style.drop = "Circle 95";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 105";
    style.off = "Circle 105";
    style.on = "Circle 105";
    style.drag = "Circle 105";
    style.drop = "Circle 105";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 115";
    style.off = "Circle 115";
    style.on = "Circle 115";
    style.drag = "Circle 115";
    style.drop = "Circle 115";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 125";
    style.off = "Circle 125";
    style.on = "Circle 125";
    style.drag = "Circle 125";
    style.drop = "Circle 125";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 135";
    style.off = "Circle 135";
    style.on = "Circle 135";
    style.drag = "Circle 135";
    style.drop = "Circle 135";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 145";
    style.off = "Circle 145";
    style.on = "Circle 145";
    style.drag = "Circle 145";
    style.drop = "Circle 145";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 155";
    style.off = "Circle 155";
    style.on = "Circle 155";
    style.drag = "Circle 155";
    style.drop = "Circle 155";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 165";
    style.off = "Circle 165";
    style.on = "Circle 165";
    style.drag = "Circle 165";
    style.drop = "Circle 165";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 175";
    style.off = "Circle 175";
    style.on = "Circle 175";
    style.drag = "Circle 175";
    style.drop = "Circle 1735";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 185";
    style.off = "Circle 185";
    style.on = "Circle 185";
    style.drag = "Circle 185";
    style.drop = "Circle 185";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Circle 195";
    style.off = "Circle 195";
    style.on = "Circle 195";
    style.drag = "Circle 195";
    style.drop = "Circle 195";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Rounded Spacer -Vertical";
    style.off = "Menu Rounded Vertical";
    style.on = "Menu Rounded Vertical";
    style.drag = "Menu Rounded Vertical";
    style.drop = "Menu Rounded Vertical";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Rounded Spacer -Horizontal";
    style.off = "Menu Rounded Horizontal";
    style.on = "Menu Rounded Horizontal";
    style.drag = "Menu Rounded Horizontal";
    style.drop = "Menu Rounded Horizontal";
    mDraw.borderStyles.push_back(style);

    style .init();
    style.name = "Oval H 60x30";
    style.off = "Oval H 60x30";
    style.on = "Oval H 60x30";
    style.drag = "Oval H 60x30";
    style.drop = "Oval H 60x30";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Oval H 100x50";
    style.off = "Oval H 100x50";
    style.on = "Oval H 100x50";
    style.drag = "Oval H 100x50";
    style.drop = "Oval H 100x50";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Oval H 150x75";
    style.off = "Oval H 150x75";
    style.on = "Oval H 150x75";
    style.drag = "Oval H 150x75";
    style.drop = "Oval H 150x75";
    mDraw.borderStyles.push_back(style);

    style .init();
    style.name = "Oval H 200x100";
    style.off = "Oval H 200x100";
    style.on = "Oval H 200x100";
    style.drag = "Oval H 200x100";
    style.drop = "Oval H 200x100";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Oval V 30x60";
    style.off = "Oval V 30x60";
    style.on = "Oval V 30x60";
    style.drag = "Oval V 30x60";
    style.drop = "Oval V 30x60";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Oval V 50x100";
    style.off = "Oval V 50x100";
    style.on = "Oval V 50x100";
    style.drag = "Oval V 50x100";
    style.drop = "Oval V 50x100";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Oval V 75x150";
    style.off = "Oval V 75x150";
    style.on = "Oval V 75x150";
    style.drag = "Oval V 75x150";
    style.drop = "Oval V 75x150";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Oval V 100x200";
    style.off = "Oval V 100x200";
    style.on = "Oval V 100x200";
    style.drag = "Oval V 100x200";
    style.drop = "Oval V 100x200";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 15";
    style.off = "Diamond 15";
    style.on = "Diamond 15";
    style.drag = "Diamond 15";
    style.drop = "Diamond 15";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 25";
    style.off = "Diamond 25";
    style.on = "Diamond 25";
    style.drag = "Diamond 25";
    style.drop = "Diamond 25";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 35";
    style.off = "Diamond 35";
    style.on = "Diamond 35";
    style.drag = "Diamond 35";
    style.drop = "Diamond 35";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 45";
    style.off = "Diamond 45";
    style.on = "Diamond 45";
    style.drag = "Diamond 45";
    style.drop = "Diamond 45";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 55";
    style.off = "Diamond 55";
    style.on = "Diamond 55";
    style.drag = "Diamond 55";
    style.drop = "Diamond 55";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 65";
    style.off = "Diamond 65";
    style.on = "Diamond 65";
    style.drag = "Diamond 65";
    style.drop = "Diamond 65";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 75";
    style.off = "Diamond 75";
    style.on = "Diamond 75";
    style.drag = "Diamond 75";
    style.drop = "Diamond 75";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 85";
    style.off = "Diamond 85";
    style.on = "Diamond 85";
    style.drag = "Diamond 85";
    style.drop = "Diamond 85";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 95";
    style.off = "Diamond 95";
    style.on = "Diamond 95";
    style.drag = "Diamond 95";
    style.drop = "Diamond 95";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 105";
    style.off = "Diamond 105";
    style.on = "Diamond 105";
    style.drag = "Diamond 105";
    style.drop = "Diamond 105";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 115";
    style.off = "Diamond 115";
    style.on = "Diamond 115";
    style.drag = "Diamond 115";
    style.drop = "Diamond 115";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 125";
    style.off = "Diamond 125";
    style.on = "Diamond 125";
    style.drag = "Diamond 125";
    style.drop = "Diamond 125";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 135";
    style.off = "Diamond 135";
    style.on = "Diamond 135";
    style.drag = "Diamond 135";
    style.drop = "Diamond 135";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 145";
    style.off = "Diamond 145";
    style.on = "Diamond 145";
    style.drag = "Diamond 145";
    style.drop = "Diamond 145";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 155";
    style.off = "Diamond 155";
    style.on = "Diamond 155";
    style.drag = "Diamond 155";
    style.drop = "Diamond 155";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 165";
    style.off = "Diamond 165";
    style.on = "Diamond 165";
    style.drag = "Diamond 165";
    style.drop = "Diamond 165";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 175";
    style.off = "Diamond 175";
    style.on = "Diamond 175";
    style.drag = "Diamond 175";
    style.drop = "Diamond 175";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 185";
    style.off = "Diamond 185";
    style.on = "Diamond 185";
    style.drag = "Diamond 185";
    style.drop = "Diamond 185";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Diamond 195";
    style.off = "Diamond 195";
    style.on = "Diamond 195";
    style.drag = "Diamond 195";
    style.drop = "Diamond 195";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "AMX Elite -L";
    style.off = "AMX Elite Raised -L";
    style.on = "AMX Elite Inset -L";
    style.drag = "AMX Elite Inset -L";
    style.drop = "AMX Elite Inset -L";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "AMX Elite -M";
    style.off = "AMX Elite Raised -M";
    style.on = "AMX Elite Inset -M";
    style.drag = "AMX Elite Inset -M";
    style.drop = "AMX Elite Inset -M";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "AMX Elite -S";
    style.off = "AMX Elite Raised -S";
    style.on = "AMX Elite Inset -S";
    style.drag = "AMX Elite Inset -S";
    style.drop = "AMX Elite Inset -S";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Bevel -L";
    style.off = "Bevel Raised -L";
    style.on = "Bevel Inset -L";
    style.drag = "Bevel Inset -L";
    style.drop = "Bevel Inset -L";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Bevel -M";
    style.off = "Bevel Raised -M";
    style.on = "Bevel Inset -M";
    style.drag = "Bevel Inset -M";
    style.drop = "Bevel Inset -M";
    style.g3Equiv.push_back(21);
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Bevel -S";
    style.off = "Bevel Raised -S";
    style.on = "Bevel Inset -S";
    style.drag = "Bevel Inset -S";
    style.drop = "Bevel Inset -S";
    style.g3Equiv.push_back(20);
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Double Bevel -L";
    style.off = "Double Bevel Raised -L";
    style.on = "Double Bevel Inset -L";
    style.drag = "Double Bevel Inset -L";
    style.drop = "Double Bevel Inset -L";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Double Bevel -M";
    style.off = "Double Bevel Raised -M";
    style.on = "Double Bevel Inset -M";
    style.drag = "Double Bevel Inset -M";
    style.drop = "Double Bevel Inset -M";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Double Bevel -S";
    style.off = "Double Bevel Raised -S";
    style.on = "Double Bevel Inset -S";
    style.drag = "Double Bevel Inset -S";
    style.drop = "Double Bevel Inset -S";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Neon Active -S";
    style.off = "Neon Active Off -S";
    style.on = "Neon Active On -S";
    style.drag = "Neon Active On -S";
    style.drop = "Neon Active On -S";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Neon Active -L";
    style.off = "Neon Active Off -L";
    style.on = "Neon Active On -L";
    style.drag = "Neon Active On -L";
    style.drop = "Neon Active On -L";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Neon Inactive -S";
    style.off = "Neon Inactive -S";
    style.on = "Neon Inactive -S";
    style.drag = "Neon Inactive -S";
    style.drop = "Neon Inactive -S";
    style.g3Equiv.push_back(24);
    style.g3Equiv.push_back(25);
    style.g3Equiv.push_back(26);
    style.g3Equiv.push_back(27);
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Neon Inactive -L";
    style.off = "Neon Inactive -L";
    style.on = "Neon Inactive -L";
    style.drag = "Neon Inactive -L";
    style.drop = "Neon Inactive -L";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Glow -L";
    style.off = "Glow -L";
    style.on = "Glow -L";
    style.drag = "Glow -L";
    style.drop = "Glow -L";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Glow -S";
    style.off = "Glow -S";
    style.on = "Glow -S";
    style.drag = "Glow -S";
    style.drop = "Glow -S";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Cursor Top";
    style.off = "Cursor Top";
    style.on = "Cursor Top";
    style.drag = "Cursor Top";
    style.drop = "Cursor Top";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Cursor Bottom";
    style.off = "Cursor Bottom";
    style.on = "Cursor Bottom";
    style.drag = "Cursor Bottom";
    style.drop = "Cursor Bottom";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Cursor Left";
    style.off = "Cursor Left";
    style.on = "Cursor Left";
    style.drag = "Cursor Left";
    style.drop = "Cursor Left";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Cursor Right";
    style.off = "Cursor Right";
    style.on = "Cursor Right";
    style.drag = "Cursor Right";
    style.drop = "Cursor Right";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Cursor Top with Hole";
    style.off = "Cursor Top with Hole";
    style.on = "Cursor Top with Hole";
    style.drag = "Cursor Top with Hole";
    style.drop = "Cursor Top with Hole";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Cursor Bottom with Hole";
    style.off = "Cursor Bottom with Hole";
    style.on = "Cursor Bottom with Hole";
    style.drag = "Cursor Bottom with Hole";
    style.drop = "Cursor Bottom with Hole";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Cursor Left with Hole";
    style.off = "Cursor Left with Hole";
    style.on = "Cursor Left with Hole";
    style.drag = "Cursor Left with Hole";
    style.drop = "Cursor Left with Hole";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Cursor Right with Hole";
    style.off = "Cursor Right with Hole";
    style.on = "Cursor Right with Hole";
    style.drag = "Cursor Right with Hole";
    style.drop = "Cursor Right with Hole";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Custom Frame";
    style.off = "Custom Frame";
    style.on = "Custom Frame";
    style.drag = "Custom Frame";
    style.drop = "Custom Frame";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Windows Style Popup (Status Bar)";
    style.off = "Windows Style Popup (Status Bar)";
    style.on = "Windows Style Popup (Status Bar)";
    style.drag = "Windows Style Popup (Status Bar)";
    style.drop = "Windows Style Popup (Status Bar)";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Windows Style Popup";
    style.off = "Windows Style Popup";
    style.on = "Windows Style Popup";
    style.drag = "Windows Style Popup";
    style.drop = "Windows Style Popup";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Help Down";
    style.off = "Help Down";
    style.on = "Help Down";
    style.drag = "Help Down";
    style.drop = "Help Down";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Help Down Reversed";
    style.off = "Help Down Reversed";
    style.on = "Help Down Reversed";
    style.drag = "Help Down Reversed";
    style.drop = "Help Down Reversed";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 15";
    style.off = "Menu Left Rounded 15";
    style.on = "Menu Left Rounded 15";
    style.drag = "Menu Left Rounded 15";
    style.drop = "Menu Left Rounded 15";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 25";
    style.off = "Menu Left Rounded 25";
    style.on = "Menu Left Rounded 25";
    style.drag = "Menu Left Rounded 25";
    style.drop = "Menu Left Rounded 25";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 35";
    style.off = "Menu Left Rounded 35";
    style.on = "Menu Left Rounded 35";
    style.drag = "Menu Left Rounded 35";
    style.drop = "Menu Left Rounded 35";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 45";
    style.off = "Menu Left Rounded 45";
    style.on = "Menu Left Rounded 45";
    style.drag = "Menu Left Rounded 45";
    style.drop = "Menu Left Rounded 45";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 55";
    style.off = "Menu Left Rounded 55";
    style.on = "Menu Left Rounded 55";
    style.drag = "Menu Left Rounded 55";
    style.drop = "Menu Left Rounded 55";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 65";
    style.off = "Menu Left Rounded 65";
    style.on = "Menu Left Rounded 65";
    style.drag = "Menu Left Rounded 65";
    style.drop = "Menu Left Rounded 65";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 75";
    style.off = "Menu Left Rounded 75";
    style.on = "Menu Left Rounded 75";
    style.drag = "Menu Left Rounded 75";
    style.drop = "Menu Left Rounded 75";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 85";
    style.off = "Menu Left Rounded 85";
    style.on = "Menu Left Rounded 85";
    style.drag = "Menu Left Rounded 85";
    style.drop = "Menu Left Rounded 85";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 95";
    style.off = "Menu Left Rounded 95";
    style.on = "Menu Left Rounded 95";
    style.drag = "Menu Left Rounded 95";
    style.drop = "Menu Left Rounded 95";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 105";
    style.off = "Menu Left Rounded 105";
    style.on = "Menu Left Rounded 105";
    style.drag = "Menu Left Rounded 105";
    style.drop = "Menu Left Rounded 105";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 115";
    style.off = "Menu Left Rounded 115";
    style.on = "Menu Left Rounded 115";
    style.drag = "Menu Left Rounded 115";
    style.drop = "Menu Left Rounded 115";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 125";
    style.off = "Menu Left Rounded 125";
    style.on = "Menu Left Rounded 125";
    style.drag = "Menu Left Rounded 125";
    style.drop = "Menu Left Rounded 125";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 135";
    style.off = "Menu Left Rounded 135";
    style.on = "Menu Left Rounded 135";
    style.drag = "Menu Left Rounded 135";
    style.drop = "Menu Left Rounded 135";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 145";
    style.off = "Menu Left Rounded 145";
    style.on = "Menu Left Rounded 145";
    style.drag = "Menu Left Rounded 145";
    style.drop = "Menu Left Rounded 145";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 155";
    style.off = "Menu Left Rounded 155";
    style.on = "Menu Left Rounded 155";
    style.drag = "Menu Left Rounded 155";
    style.drop = "Menu Left Rounded 155";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 165";
    style.off = "Menu Left Rounded 165";
    style.on = "Menu Left Rounded 165";
    style.drag = "Menu Left Rounded 165";
    style.drop = "Menu Left Rounded 165";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 175";
    style.off = "Menu Left Rounded 175";
    style.on = "Menu Left Rounded 175";
    style.drag = "Menu Left Rounded 175";
    style.drop = "Menu Left Rounded 175";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 185";
    style.off = "Menu Left Rounded 185";
    style.on = "Menu Left Rounded 185";
    style.drag = "Menu Left Rounded 185";
    style.drop = "Menu Left Rounded 185";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Left Rounded 195";
    style.off = "Menu Left Rounded 195";
    style.on = "Menu Left Rounded 195";
    style.drag = "Menu Left Rounded 195";
    style.drop = "Menu Left Rounded 195";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 15";
    style.off = "Menu Right Rounded 15";
    style.on = "Menu Right Rounded 15";
    style.drag = "Menu Right Rounded 15";
    style.drop = "Menu Right Rounded 15";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 25";
    style.off = "Menu Right Rounded 25";
    style.on = "Menu Right Rounded 25";
    style.drag = "Menu Right Rounded 25";
    style.drop = "Menu Right Rounded 25";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 35";
    style.off = "Menu Right Rounded 35";
    style.on = "Menu Right Rounded 35";
    style.drag = "Menu Right Rounded 35";
    style.drop = "Menu Right Rounded 35";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 45";
    style.off = "Menu Right Rounded 45";
    style.on = "Menu Right Rounded 45";
    style.drag = "Menu Right Rounded 45";
    style.drop = "Menu Right Rounded 45";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 55";
    style.off = "Menu Right Rounded 55";
    style.on = "Menu Right Rounded 55";
    style.drag = "Menu Right Rounded 55";
    style.drop = "Menu Right Rounded 55";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 65";
    style.off = "Menu Right Rounded 65";
    style.on = "Menu Right Rounded 65";
    style.drag = "Menu Right Rounded 65";
    style.drop = "Menu Right Rounded 65";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 75";
    style.off = "Menu Right Rounded 75";
    style.on = "Menu Right Rounded 75";
    style.drag = "Menu Right Rounded 75";
    style.drop = "Menu Right Rounded 75";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 85";
    style.off = "Menu Right Rounded 85";
    style.on = "Menu Right Rounded 85";
    style.drag = "Menu Right Rounded 85";
    style.drop = "Menu Right Rounded 85";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 95";
    style.off = "Menu Right Rounded 95";
    style.on = "Menu Right Rounded 95";
    style.drag = "Menu Right Rounded 95";
    style.drop = "Menu Right Rounded 95";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 105";
    style.off = "Menu Right Rounded 105";
    style.on = "Menu Right Rounded 105";
    style.drag = "Menu Right Rounded 105";
    style.drop = "Menu Right Rounded 105";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 115";
    style.off = "Menu Right Rounded 115";
    style.on = "Menu Right Rounded 115";
    style.drag = "Menu Right Rounded 115";
    style.drop = "Menu Right Rounded 115";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 125";
    style.off = "Menu Right Rounded 125";
    style.on = "Menu Right Rounded 125";
    style.drag = "Menu Right Rounded 125";
    style.drop = "Menu Right Rounded 125";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 135";
    style.off = "Menu Right Rounded 135";
    style.on = "Menu Right Rounded 135";
    style.drag = "Menu Right Rounded 135";
    style.drop = "Menu Right Rounded 135";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 145";
    style.off = "Menu Right Rounded 145";
    style.on = "Menu Right Rounded 145";
    style.drag = "Menu Right Rounded 145";
    style.drop = "Menu Right Rounded 145";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 155";
    style.off = "Menu Right Rounded 155";
    style.on = "Menu Right Rounded 155";
    style.drag = "Menu Right Rounded 155";
    style.drop = "Menu Right Rounded 155";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 165";
    style.off = "Menu Right Rounded 165";
    style.on = "Menu Right Rounded 165";
    style.drag = "Menu Right Rounded 165";
    style.drop = "Menu Right Rounded 165";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 175";
    style.off = "Menu Right Rounded 175";
    style.on = "Menu Right Rounded 175";
    style.drag = "Menu Right Rounded 175";
    style.drop = "Menu Right Rounded 175";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 185";
    style.off = "Menu Right Rounded 185";
    style.on = "Menu Right Rounded 185";
    style.drag = "Menu Right Rounded 185";
    style.drop = "Menu Right Rounded 185";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Right Rounded 195";
    style.off = "Menu Right Rounded 195";
    style.on = "Menu Right Rounded 195";
    style.drag = "Menu Right Rounded 195";
    style.drop = "Menu Right Rounded 195";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 15";
    style.off = "Menu Top Rounded 15";
    style.on = "Menu Top Rounded 15";
    style.drag = "Menu Top Rounded 15";
    style.drop = "Menu Top Rounded 15";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 25";
    style.off = "Menu Top Rounded 25";
    style.on = "Menu Top Rounded 25";
    style.drag = "Menu Top Rounded 25";
    style.drop = "Menu Top Rounded 25";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 35";
    style.off = "Menu Top Rounded 35";
    style.on = "Menu Top Rounded 35";
    style.drag = "Menu Top Rounded 35";
    style.drop = "Menu Top Rounded 35";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 45";
    style.off = "Menu Top Rounded 45";
    style.on = "Menu Top Rounded 45";
    style.drag = "Menu Top Rounded 45";
    style.drop = "Menu Top Rounded 45";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 55";
    style.off = "Menu Top Rounded 55";
    style.on = "Menu Top Rounded 55";
    style.drag = "Menu Top Rounded 55";
    style.drop = "Menu Top Rounded 55";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 65";
    style.off = "Menu Top Rounded 65";
    style.on = "Menu Top Rounded 65";
    style.drag = "Menu Top Rounded 65";
    style.drop = "Menu Top Rounded 65";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 75";
    style.off = "Menu Top Rounded 75";
    style.on = "Menu Top Rounded 75";
    style.drag = "Menu Top Rounded 75";
    style.drop = "Menu Top Rounded 75";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 85";
    style.off = "Menu Top Rounded 85";
    style.on = "Menu Top Rounded 85";
    style.drag = "Menu Top Rounded 85";
    style.drop = "Menu Top Rounded 85";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 95";
    style.off = "Menu Top Rounded 95";
    style.on = "Menu Top Rounded 95";
    style.drag = "Menu Top Rounded 95";
    style.drop = "Menu Top Rounded 95";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 105";
    style.off = "Menu Top Rounded 105";
    style.on = "Menu Top Rounded 105";
    style.drag = "Menu Top Rounded 105";
    style.drop = "Menu Top Rounded 105";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 115";
    style.off = "Menu Top Rounded 115";
    style.on = "Menu Top Rounded 115";
    style.drag = "Menu Top Rounded 115";
    style.drop = "Menu Top Rounded 115";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 125";
    style.off = "Menu Top Rounded 125";
    style.on = "Menu Top Rounded 125";
    style.drag = "Menu Top Rounded 125";
    style.drop = "Menu Top Rounded 125";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 135";
    style.off = "Menu Top Rounded 135";
    style.on = "Menu Top Rounded 135";
    style.drag = "Menu Top Rounded 135";
    style.drop = "Menu Top Rounded 135";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 145";
    style.off = "Menu Top Rounded 145";
    style.on = "Menu Top Rounded 145";
    style.drag = "Menu Top Rounded 145";
    style.drop = "Menu Top Rounded 145";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 155";
    style.off = "Menu Top Rounded 155";
    style.on = "Menu Top Rounded 155";
    style.drag = "Menu Top Rounded 155";
    style.drop = "Menu Top Rounded 155";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 165";
    style.off = "Menu Top Rounded 165";
    style.on = "Menu Top Rounded 165";
    style.drag = "Menu Top Rounded 165";
    style.drop = "Menu Top Rounded 165";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 175";
    style.off = "Menu Top Rounded 175";
    style.on = "Menu Top Rounded 175";
    style.drag = "Menu Top Rounded 175";
    style.drop = "Menu Top Rounded 175";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 185";
    style.off = "Menu Top Rounded 185";
    style.on = "Menu Top Rounded 185";
    style.drag = "Menu Top Rounded 185";
    style.drop = "Menu Top Rounded 185";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Top Rounded 195";
    style.off = "Menu Top Rounded 195";
    style.on = "Menu Top Rounded 195";
    style.drag = "Menu Top Rounded 195";
    style.drop = "Menu Top Rounded 195";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 15";
    style.off = "Menu Bottom Rounded 15";
    style.on = "Menu Bottom Rounded 15";
    style.drag = "Menu Bottom Rounded 15";
    style.drop = "Menu Bottom Rounded 15";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 25";
    style.off = "Menu Bottom Rounded 25";
    style.on = "Menu Bottom Rounded 25";
    style.drag = "Menu Bottom Rounded 25";
    style.drop = "Menu Bottom Rounded 25";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 35";
    style.off = "Menu Bottom Rounded 35";
    style.on = "Menu Bottom Rounded 35";
    style.drag = "Menu Bottom Rounded 35";
    style.drop = "Menu Bottom Rounded 35";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 45";
    style.off = "Menu Bottom Rounded 45";
    style.on = "Menu Bottom Rounded 45";
    style.drag = "Menu Bottom Rounded 45";
    style.drop = "Menu Bottom Rounded 45";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 55";
    style.off = "Menu Bottom Rounded 55";
    style.on = "Menu Bottom Rounded 55";
    style.drag = "Menu Bottom Rounded 55";
    style.drop = "Menu Bottom Rounded 55";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 65";
    style.off = "Menu Bottom Rounded 65";
    style.on = "Menu Bottom Rounded 65";
    style.drag = "Menu Bottom Rounded 65";
    style.drop = "Menu Bottom Rounded 65";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 75";
    style.off = "Menu Bottom Rounded 75";
    style.on = "Menu Bottom Rounded 75";
    style.drag = "Menu Bottom Rounded 75";
    style.drop = "Menu Bottom Rounded 75";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 85";
    style.off = "Menu Bottom Rounded 85";
    style.on = "Menu Bottom Rounded 85";
    style.drag = "Menu Bottom Rounded 85";
    style.drop = "Menu Bottom Rounded 85";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 95";
    style.off = "Menu Bottom Rounded 95";
    style.on = "Menu Bottom Rounded 95";
    style.drag = "Menu Bottom Rounded 95";
    style.drop = "Menu Bottom Rounded 95";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 105";
    style.off = "Menu Bottom Rounded 105";
    style.on = "Menu Bottom Rounded 105";
    style.drag = "Menu Bottom Rounded 105";
    style.drop = "Menu Bottom Rounded 105";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 115";
    style.off = "Menu Bottom Rounded 115";
    style.on = "Menu Bottom Rounded 115";
    style.drag = "Menu Bottom Rounded 115";
    style.drop = "Menu Bottom Rounded 115";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 125";
    style.off = "Menu Bottom Rounded 125";
    style.on = "Menu Bottom Rounded 125";
    style.drag = "Menu Bottom Rounded 125";
    style.drop = "Menu Bottom Rounded 125";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 135";
    style.off = "Menu Bottom Rounded 135";
    style.on = "Menu Bottom Rounded 135";
    style.drag = "Menu Bottom Rounded 135";
    style.drop = "Menu Bottom Rounded 135";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 145";
    style.off = "Menu Bottom Rounded 145";
    style.on = "Menu Bottom Rounded 145";
    style.drag = "Menu Bottom Rounded 145";
    style.drop = "Menu Bottom Rounded 145";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 155";
    style.off = "Menu Bottom Rounded 155";
    style.on = "Menu Bottom Rounded 155";
    style.drag = "Menu Bottom Rounded 155";
    style.drop = "Menu Bottom Rounded 155";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 165";
    style.off = "Menu Bottom Rounded 165";
    style.on = "Menu Bottom Rounded 165";
    style.drag = "Menu Bottom Rounded 165";
    style.drop = "Menu Bottom Rounded 165";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 175";
    style.off = "Menu Bottom Rounded 175";
    style.on = "Menu Bottom Rounded 175";
    style.drag = "Menu Bottom Rounded 175";
    style.drop = "Menu Bottom Rounded 175";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 185";
    style.off = "Menu Bottom Rounded 185";
    style.on = "Menu Bottom Rounded 185";
    style.drag = "Menu Bottom Rounded 185";
    style.drop = "Menu Bottom Rounded 185";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Menu Bottom Rounded 195";
    style.off = "Menu Bottom Rounded 195";
    style.on = "Menu Bottom Rounded 195";
    style.drag = "Menu Bottom Rounded 195";
    style.drop = "Menu Bottom Rounded 195";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Smart Button Bubbled Very Large";
    style.off = "Smart Button Bubbled Very Large";
    style.on = "Smart Button Bubbled Very Large";
    style.drag = "Smart Button Bubbled Very Large";
    style.drop = "Smart Button Bubbled Very Large";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Smart Button Bubbled Large";
    style.off = "Smart Button Bubbled Large";
    style.on = "Smart Button Bubbled Large";
    style.drag = "Smart Button Bubbled Large";
    style.drop = "Smart Button Bubbled Large";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Smart Button Bubbled Medium";
    style.off = "Smart Button Bubbled Medium";
    style.on = "Smart Button Bubbled Medium";
    style.drag = "Smart Button Bubbled Medium";
    style.drop = "Smart Button Bubbled Medium";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Smart Button Bubbled Small";
    style.off = "Smart Button Bubbled Small";
    style.on = "Smart Button Bubbled Small";
    style.drag = "Smart Button Bubbled Small";
    style.drop = "Smart Button Bubbled Small";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "News Header";
    style.off = "News Header";
    style.on = "News Header";
    style.drag = "News Header";
    style.drop = "News Header";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "News Header Right";
    style.off = "News Header Right";
    style.on = "News Header Right";
    style.drag = "News Header Right";
    style.drop = "News Header Right";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "News Header Left";
    style.off = "News Header Left";
    style.on = "News Header Left";
    style.drag = "News Header Left";
    style.drop = "News Header Left";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Smart Button Squared Large";
    style.off = "Smart Button Squared Large";
    style.on = "Smart Button Squared Large";
    style.drag = "Smart Button Squared Large";
    style.drop = "Smart Button Squared Large";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Smart Button Squared Medium";
    style.off = "Smart Button Squared Medium";
    style.on = "Smart Button Squared Medium";
    style.drag = "Smart Button Squared Medium";
    style.drop = "Smart Button Squared Medium";
    mDraw.borderStyles.push_back(style);

    style.init();
    style.name = "Smart Button Squared Small";
    style.off = "Smart Button Squared Small";
    style.on = "Smart Button Squared Small";
    style.drag = "Smart Button Squared Small";
    style.drop = "Smart Button Squared Small";
    mDraw.borderStyles.push_back(style);
}

void TGraphics::initBorderData()
{
    DECL_TRACER("TGraphics::initBorderData();");

    BORDER_DATA_t bd;
    bd.name = "Smart Button Squared Large";
    bd.baseFile = "sbSquaredLarge";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 149;
    bd.idealHeight = 149;
    bd.minHeight = 149;
    bd.minWidth = 149;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Smart Button Squared Medium";
    bd.baseFile = "sbSquaredMed";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 75;
    bd.idealHeight = 75;
    bd.minHeight = 75;
    bd.minWidth = 75;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Smart Button Squared Small";
    bd.baseFile = "sbSquaredSmall";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 43;
    bd.idealHeight = 43;
    bd.minHeight = 43;
    bd.minWidth = 43;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "News Header";
    bd.baseFile = "newsHeader";
    bd.multiColor = 1;
    bd.fillTop = 3;
    bd.fillLeft = 3;
    bd.fillBottom = 3;
    bd.fillRight = 3;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 53;
    bd.idealHeight = 53;
    bd.minHeight = 53;
    bd.minWidth = 53;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "News Header Right";
    bd.baseFile = "newsHeaderRight";
    bd.multiColor = 1;
    bd.fillTop = 3;
    bd.fillLeft = 3;
    bd.fillBottom = 3;
    bd.fillRight = 3;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 53;
    bd.idealHeight = 53;
    bd.minHeight = 53;
    bd.minWidth = 53;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "News Header Left";
    bd.baseFile = "newsHeaderLeft";
    bd.multiColor = 1;
    bd.fillTop = 3;
    bd.fillLeft = 3;
    bd.fillBottom = 3;
    bd.fillRight = 3;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 53;
    bd.idealHeight = 53;
    bd.minHeight = 53;
    bd.minWidth = 53;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Smart Button Bubbled Small";
    bd.baseFile = "aqua";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 43;
    bd.idealHeight = 43;
    bd.minHeight = 43;
    bd.minWidth = 43;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Smart Button Bubbled Medium";
    bd.baseFile = "aquaSmall";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 75;
    bd.idealHeight = 75;
    bd.minHeight = 75;
    bd.minWidth = 75;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Smart Button Bubbled Large";
    bd.baseFile = "aquaMed";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 149;
    bd.idealHeight = 149;
    bd.minHeight = 149;
    bd.minWidth = 149;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Smart Button Bubbled Very Large";
    bd.baseFile = "aquaLarge";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 296;
    bd.idealHeight = 296;
    bd.minHeight = 296;
    bd.minWidth = 296;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Rounded Vertical";
    bd.baseFile = "MenuVerticalRounded";
    bd.multiColor = 1;
    bd.fillTop = 1;
    bd.fillLeft = 1;
    bd.fillBottom = 1;
    bd.fillRight = 1;
    bd.textTop = 1;
    bd.textLeft = 1;
    bd.textBottom = 1;
    bd.textRight = 1;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 3;
    bd.minWidth = 3;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Rounded Horizontal";
    bd.baseFile = "MenuHorizontalRounded";
    bd.multiColor = 1;
    bd.fillTop = 1;
    bd.fillLeft = 1;
    bd.fillBottom = 1;
    bd.fillRight = 1;
    bd.textTop = 1;
    bd.textLeft = 1;
    bd.textBottom = 1;
    bd.textRight = 1;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 3;
    bd.minWidth = 3;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Single Line";
    bd.baseFile = "Line1";
    bd.multiColor = 1;
    bd.fillTop = 1;
    bd.fillLeft = 1;
    bd.fillBottom = 1;
    bd.fillRight = 1;
    bd.textTop = 1;
    bd.textLeft = 1;
    bd.textBottom = 1;
    bd.textRight = 1;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 3;
    bd.minWidth = 3;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Double Line";
    bd.baseFile = "Line2";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 5;
    bd.minWidth = 5;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Quad Line";
    bd.baseFile = "Line4";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 4;
    bd.fillBottom = 4;
    bd.fillRight = 4;
    bd.textTop = 4;
    bd.textLeft = 4;
    bd.textBottom = 4;
    bd.textRight = 4;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 9;
    bd.minWidth = 9;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Picture Frame";
    bd.baseFile = "PF";
    bd.multiColor = 1;
    bd.fillTop = 5;
    bd.fillLeft = 5;
    bd.fillBottom = 5;
    bd.fillRight = 5;
    bd.textTop = 5;
    bd.textLeft = 5;
    bd.textBottom = 5;
    bd.textRight = 5;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 11;
    bd.minWidth = 11;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 15";
    bd.baseFile = "circle15";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 15;
    bd.idealHeight = 15;
    bd.minHeight = 15;
    bd.minWidth = 15;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 25";
    bd.baseFile = "circle25";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 25;
    bd.idealHeight = 25;
    bd.minHeight = 25;
    bd.minWidth = 25;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 35";
    bd.baseFile = "circle35";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 35;
    bd.idealHeight = 35;
    bd.minHeight = 35;
    bd.minWidth = 35;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 45";
    bd.baseFile = "circle45";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 45;
    bd.idealHeight = 45;
    bd.minHeight = 45;
    bd.minWidth = 45;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 55";
    bd.baseFile = "circle55";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 55;
    bd.idealHeight = 55;
    bd.minHeight = 55;
    bd.minWidth = 55;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 65";
    bd.baseFile = "circle65";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 65;
    bd.idealHeight = 65;
    bd.minHeight = 65;
    bd.minWidth = 65;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 75";
    bd.baseFile = "circle75";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 75;
    bd.idealHeight = 75;
    bd.minHeight = 75;
    bd.minWidth = 75;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 85";
    bd.baseFile = "circle85";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 85;
    bd.idealHeight = 85;
    bd.minHeight = 85;
    bd.minWidth = 85;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 95";
    bd.baseFile = "circle95";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 95;
    bd.idealHeight = 95;
    bd.minHeight = 95;
    bd.minWidth = 95;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 105";
    bd.baseFile = "circle105";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 105;
    bd.idealHeight = 105;
    bd.minHeight = 105;
    bd.minWidth = 105;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 115";
    bd.baseFile = "circle115";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 115;
    bd.idealHeight = 115;
    bd.minHeight = 115;
    bd.minWidth = 115;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 125";
    bd.baseFile = "circle125";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 125;
    bd.idealHeight = 125;
    bd.minHeight = 125;
    bd.minWidth = 125;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 135";
    bd.baseFile = "circle135";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 135;
    bd.idealHeight = 135;
    bd.minHeight = 135;
    bd.minWidth = 135;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 145";
    bd.baseFile = "circle145";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 145;
    bd.idealHeight = 145;
    bd.minHeight = 145;
    bd.minWidth = 145;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 155";
    bd.baseFile = "circle155";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 155;
    bd.idealHeight = 155;
    bd.minHeight = 155;
    bd.minWidth = 155;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 165";
    bd.baseFile = "circle165";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 165;
    bd.idealHeight = 165;
    bd.minHeight = 165;
    bd.minWidth = 165;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 175";
    bd.baseFile = "circle175";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 175;
    bd.idealHeight = 175;
    bd.minHeight = 175;
    bd.minWidth = 175;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 185";
    bd.baseFile = "circle185";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 185;
    bd.idealHeight = 185;
    bd.minHeight = 185;
    bd.minWidth = 185;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Circle 195";
    bd.baseFile = "circle195";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 195;
    bd.idealHeight = 195;
    bd.minHeight = 195;
    bd.minWidth = 195;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Oval H 60x30";
    bd.baseFile = "HOval60x30";
    bd.multiColor = 1;
    bd.fillTop = 3;
    bd.fillLeft = 3;
    bd.fillBottom = 3;
    bd.fillRight = 3;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 60;
    bd.idealHeight = 30;
    bd.minHeight = 60;
    bd.minWidth = 30;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Oval H 100x50";
    bd.baseFile = "HOval100x50";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 4;
    bd.fillBottom = 3;
    bd.fillRight = 3;
    bd.textTop = 4;
    bd.textLeft = 4;
    bd.textBottom = 4;
    bd.textRight = 4;
    bd.idealWidth = 100;
    bd.idealHeight = 50;
    bd.minHeight = 100;
    bd.minWidth = 50;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Oval H 150x75";
    bd.baseFile = "HOval150x75";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 4;
    bd.fillBottom = 4;
    bd.fillRight = 5;
    bd.textTop = 4;
    bd.textLeft = 4;
    bd.textBottom = 4;
    bd.textRight = 5;
    bd.idealWidth = 150;
    bd.idealHeight = 75;
    bd.minHeight = 150;
    bd.minWidth = 75;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Oval H 200x100";
    bd.baseFile = "HOval200x100";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 3;
    bd.fillBottom = 3;
    bd.fillRight = 4;
    bd.textTop = 4;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 4;
    bd.idealWidth = 200;
    bd.idealHeight = 100;
    bd.minHeight = 200;
    bd.minWidth = 100;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Oval V 30x60";
    bd.baseFile = "VOval30x60";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 3;
    bd.fillBottom = 3;
    bd.fillRight = 3;
    bd.textTop = 4;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 30;
    bd.idealHeight = 60;
    bd.minHeight = 30;
    bd.minWidth = 60;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Oval V 50x100";
    bd.baseFile = "VOval50x100";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 4;
    bd.fillBottom = 4;
    bd.fillRight = 4;
    bd.textTop = 4;
    bd.textLeft = 4;
    bd.textBottom = 4;
    bd.textRight = 4;
    bd.idealWidth = 50;
    bd.idealHeight = 100;
    bd.minHeight = 50;
    bd.minWidth = 100;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Oval V 75x150";
    bd.baseFile = "VOval75x150";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 4;
    bd.fillBottom = 3;
    bd.fillRight = 4;
    bd.textTop = 4;
    bd.textLeft = 4;
    bd.textBottom = 3;
    bd.textRight = 4;
    bd.idealWidth = 75;
    bd.idealHeight = 150;
    bd.minHeight = 75;
    bd.minWidth = 150;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Oval V 100x200";
    bd.baseFile = "VOval100x200";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 4;
    bd.fillBottom = 3;
    bd.fillRight = 3;
    bd.textTop = 4;
    bd.textLeft = 4;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 100;
    bd.idealHeight = 200;
    bd.minHeight = 100;
    bd.minWidth = 200;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 15";
    bd.baseFile = "diamond15";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 15;
    bd.idealHeight = 15;
    bd.minHeight = 15;
    bd.minWidth = 15;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 25";
    bd.baseFile = "diamond25";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 25;
    bd.idealHeight = 25;
    bd.minHeight = 25;
    bd.minWidth = 25;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 35";
    bd.baseFile = "diamond35";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 35;
    bd.idealHeight = 35;
    bd.minHeight = 35;
    bd.minWidth = 35;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 45";
    bd.baseFile = "diamond45";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 45;
    bd.idealHeight = 45;
    bd.minHeight = 45;
    bd.minWidth = 45;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 55";
    bd.baseFile = "diamond55";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 55;
    bd.idealHeight = 55;
    bd.minHeight = 55;
    bd.minWidth = 55;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 65";
    bd.baseFile = "diamond65";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 65;
    bd.idealHeight = 65;
    bd.minHeight = 65;
    bd.minWidth = 65;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 75";
    bd.baseFile = "diamond75";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 75;
    bd.idealHeight = 75;
    bd.minHeight = 75;
    bd.minWidth = 75;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 85";
    bd.baseFile = "diamond85";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 85;
    bd.idealHeight = 85;
    bd.minHeight = 85;
    bd.minWidth = 85;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 95";
    bd.baseFile = "diamond95";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 95;
    bd.idealHeight = 95;
    bd.minHeight = 95;
    bd.minWidth = 95;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 105";
    bd.baseFile = "diamond105";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 105;
    bd.idealHeight = 105;
    bd.minHeight = 105;
    bd.minWidth = 105;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 115";
    bd.baseFile = "diamond115";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 115;
    bd.idealHeight = 115;
    bd.minHeight = 115;
    bd.minWidth = 115;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 125";
    bd.baseFile = "diamond125";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 125;
    bd.idealHeight = 125;
    bd.minHeight = 125;
    bd.minWidth = 125;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 135";
    bd.baseFile = "diamond135";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 135;
    bd.idealHeight = 135;
    bd.minHeight = 135;
    bd.minWidth = 135;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 145";
    bd.baseFile = "diamond145";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 145;
    bd.idealHeight = 145;
    bd.minHeight = 145;
    bd.minWidth = 145;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 155";
    bd.baseFile = "diamond155";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 155;
    bd.idealHeight = 155;
    bd.minHeight = 155;
    bd.minWidth = 155;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 165";
    bd.baseFile = "diamond165";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 165;
    bd.idealHeight = 165;
    bd.minHeight = 165;
    bd.minWidth = 165;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 175";
    bd.baseFile = "diamond175";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 175;
    bd.idealHeight = 175;
    bd.minHeight = 175;
    bd.minWidth = 175;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 185";
    bd.baseFile = "diamond185";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 185;
    bd.idealHeight = 185;
    bd.minHeight = 185;
    bd.minWidth = 185;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Diamond 195";
    bd.baseFile = "diamond195";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 195;
    bd.idealHeight = 195;
    bd.minHeight = 195;
    bd.minWidth = 195;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "AMX Elite Raised -L";
    bd.baseFile = "AMXeliteL-off";
    bd.multiColor = 1;
    bd.fillTop = 12;
    bd.fillLeft = 12;
    bd.fillBottom = 12;
    bd.fillRight = 12;
    bd.textTop = 12;
    bd.textLeft = 12;
    bd.textBottom = 12;
    bd.textRight = 12;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 25;
    bd.minWidth = 25;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "AMX Elite Inset -L";
    bd.baseFile = "AMXeliteL-on";
    bd.multiColor = 1;
    bd.fillTop = 12;
    bd.fillLeft = 12;
    bd.fillBottom = 12;
    bd.fillRight = 12;
    bd.textTop = 12;
    bd.textLeft = 12;
    bd.textBottom = 12;
    bd.textRight = 12;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 25;
    bd.minWidth = 25;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "AMX Elite Raised -M";
    bd.baseFile = "AMXeliteM-off";
    bd.multiColor = 1;
    bd.fillTop = 6;
    bd.fillLeft = 6;
    bd.fillBottom = 6;
    bd.fillRight = 6;
    bd.textTop = 6;
    bd.textLeft = 6;
    bd.textBottom = 6;
    bd.textRight = 6;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 13;
    bd.minWidth = 13;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "AMX Elite Inset -M";
    bd.baseFile = "AMXeliteM-on";
    bd.multiColor = 1;
    bd.fillTop = 6;
    bd.fillLeft = 6;
    bd.fillBottom = 6;
    bd.fillRight = 6;
    bd.textTop = 6;
    bd.textLeft = 6;
    bd.textBottom = 6;
    bd.textRight = 6;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 13;
    bd.minWidth = 13;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "AMX Elite Raised -S";
    bd.baseFile = "AMXeliteS-off";
    bd.multiColor = 1;
    bd.fillTop = 3;
    bd.fillLeft = 3;
    bd.fillBottom = 3;
    bd.fillRight = 3;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 7;
    bd.minWidth = 7;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "AMX Elite Inset -S";
    bd.baseFile = "AMXeliteS-on";
    bd.multiColor = 1;
    bd.fillTop = 3;
    bd.fillLeft = 3;
    bd.fillBottom = 3;
    bd.fillRight = 3;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 7;
    bd.minWidth = 7;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Bevel Raised -L";
    bd.baseFile = "bevelL-off";
    bd.multiColor = 1;
    bd.fillTop = 10;
    bd.fillLeft = 10;
    bd.fillBottom = 10;
    bd.fillRight = 10;
    bd.textTop = 10;
    bd.textLeft = 10;
    bd.textBottom = 10;
    bd.textRight = 10;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 21;
    bd.minWidth = 21;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Bevel Inset -L";
    bd.baseFile = "bevelL-on";
    bd.multiColor = 1;
    bd.fillTop = 10;
    bd.fillLeft = 10;
    bd.fillBottom = 10;
    bd.fillRight = 10;
    bd.textTop = 10;
    bd.textLeft = 10;
    bd.textBottom = 10;
    bd.textRight = 10;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 21;
    bd.minWidth = 21;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Bevel Raised -M";
    bd.baseFile = "bevelM-off";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 4;
    bd.fillBottom = 4;
    bd.fillRight = 4;
    bd.textTop = 4;
    bd.textLeft = 4;
    bd.textBottom = 4;
    bd.textRight = 4;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 9;
    bd.minWidth = 9;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Bevel Inset -M";
    bd.baseFile = "bevelM-on";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 4;
    bd.fillBottom = 4;
    bd.fillRight = 4;
    bd.textTop = 4;
    bd.textLeft = 4;
    bd.textBottom = 4;
    bd.textRight = 4;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 9;
    bd.minWidth = 9;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Bevel Raised -S";
    bd.baseFile = "bevel-off";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 5;
    bd.minWidth = 5;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Bevel Inset -S";
    bd.baseFile = "bevel-on";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 2;
    bd.textLeft = 2;
    bd.textBottom = 2;
    bd.textRight = 2;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 5;
    bd.minWidth = 5;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Double Bevel Raised -L";
    bd.baseFile = "dbevelL-off";
    bd.multiColor = 1;
    bd.fillTop = 14;
    bd.fillLeft = 14;
    bd.fillBottom = 14;
    bd.fillRight = 14;
    bd.textTop = 14;
    bd.textLeft = 14;
    bd.textBottom = 14;
    bd.textRight = 14;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 27;
    bd.minWidth = 27;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Double Bevel Inset -L";
    bd.baseFile = "dbevelL-on";
    bd.multiColor = 1;
    bd.fillTop = 14;
    bd.fillLeft = 14;
    bd.fillBottom = 14;
    bd.fillRight = 14;
    bd.textTop = 14;
    bd.textLeft = 14;
    bd.textBottom = 14;
    bd.textRight = 14;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 27;
    bd.minWidth = 27;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Double Bevel Raised -M";
    bd.baseFile = "dbevelM-off";
    bd.multiColor = 1;
    bd.fillTop = 8;
    bd.fillLeft = 8;
    bd.fillBottom = 8;
    bd.fillRight = 8;
    bd.textTop = 8;
    bd.textLeft = 8;
    bd.textBottom = 8;
    bd.textRight = 8;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 17;
    bd.minWidth = 17;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Double Bevel Inset -M";
    bd.baseFile = "dbevelM-on";
    bd.multiColor = 1;
    bd.fillTop = 8;
    bd.fillLeft = 8;
    bd.fillBottom = 8;
    bd.fillRight = 8;
    bd.textTop = 8;
    bd.textLeft = 8;
    bd.textBottom = 8;
    bd.textRight = 8;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 17;
    bd.minWidth = 17;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Double Bevel Raised -S";
    bd.baseFile = "dbevelS-off";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 4;
    bd.fillBottom = 4;
    bd.fillRight = 4;
    bd.textTop = 4;
    bd.textLeft = 4;
    bd.textBottom = 4;
    bd.textRight = 4;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 9;
    bd.minWidth = 9;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Double Bevel Inset -S";
    bd.baseFile = "dbevelS-on";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 4;
    bd.fillBottom = 4;
    bd.fillRight = 4;
    bd.textTop = 4;
    bd.textLeft = 4;
    bd.textBottom = 4;
    bd.textRight = 4;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 9;
    bd.minWidth = 9;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Neon Inactive -L";
    bd.baseFile = "pipe100";
    bd.multiColor = 1;
    bd.fillTop = 3;
    bd.fillLeft = 3;
    bd.fillBottom = 3;
    bd.fillRight = 3;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 100;
    bd.idealHeight = 100;
    bd.minHeight = 100;
    bd.minWidth = 100;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Neon Inactive -S";
    bd.baseFile = "pipe50";
    bd.multiColor = 1;
    bd.fillTop = 3;
    bd.fillLeft = 3;
    bd.fillBottom = 3;
    bd.fillRight = 3;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 50;
    bd.minWidth = 50;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Neon Active Off -L";
    bd.baseFile = "neon150-f";
    bd.multiColor = 1;
    bd.fillTop = 37;
    bd.fillLeft = 37;
    bd.fillBottom = 35;
    bd.fillRight = 35;
    bd.textTop = 37;
    bd.textLeft = 37;
    bd.textBottom = 35;
    bd.textRight = 35;
    bd.idealWidth = 150;
    bd.idealHeight = 150;
    bd.minHeight = 150;
    bd.minWidth = 150;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Neon Active On -L";
    bd.baseFile = "neon150-n";
    bd.multiColor = 1;
    bd.fillTop = 37;
    bd.fillLeft = 37;
    bd.fillBottom = 35;
    bd.fillRight = 35;
    bd.textTop = 37;
    bd.textLeft = 37;
    bd.textBottom = 35;
    bd.textRight = 35;
    bd.idealWidth = 150;
    bd.idealHeight = 150;
    bd.minHeight = 150;
    bd.minWidth = 150;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Neon Active Off -S";
    bd.baseFile = "neon75-f";
    bd.multiColor = 1;
    bd.fillTop = 18;
    bd.fillLeft = 18;
    bd.fillBottom = 19;
    bd.fillRight = 19;
    bd.textTop = 18;
    bd.textLeft = 18;
    bd.textBottom = 19;
    bd.textRight = 19;
    bd.idealWidth = 75;
    bd.idealHeight = 75;
    bd.minHeight = 75;
    bd.minWidth = 75;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Neon Active On -S";
    bd.baseFile = "neon75-n";
    bd.multiColor = 1;
    bd.fillTop = 18;
    bd.fillLeft = 18;
    bd.fillBottom = 19;
    bd.fillRight = 19;
    bd.textTop = 18;
    bd.textLeft = 18;
    bd.textBottom = 19;
    bd.textRight = 19;
    bd.idealWidth = 75;
    bd.idealHeight = 75;
    bd.minHeight = 75;
    bd.minWidth = 75;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Glow -L";
    bd.baseFile = "Glow50";
    bd.multiColor = 1;
    bd.fillTop = 22;
    bd.fillLeft = 22;
    bd.fillBottom = 22;
    bd.fillRight = 22;
    bd.textTop = 22;
    bd.textLeft = 22;
    bd.textBottom = 22;
    bd.textRight = 22;
    bd.idealWidth = 100;
    bd.idealHeight = 100;
    bd.minHeight = 45;
    bd.minWidth = 45;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Glow -S";
    bd.baseFile = "Glow25";
    bd.multiColor = 1;
    bd.fillTop = 12;
    bd.fillLeft = 12;
    bd.fillBottom = 12;
    bd.fillRight = 12;
    bd.textTop = 12;
    bd.textLeft = 12;
    bd.textBottom = 12;
    bd.textRight = 12;
    bd.idealWidth = 50;
    bd.idealHeight = 50;
    bd.minHeight = 25;
    bd.minWidth = 25;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Fuzzy";
    bd.baseFile = "fuzzyBorder";
    bd.multiColor = 1;
    bd.fillTop = 29;
    bd.fillLeft = 29;
    bd.fillBottom = 29;
    bd.fillRight = 29;
    bd.textTop = 35;
    bd.textLeft = 35;
    bd.textBottom = 35;
    bd.textRight = 35;
    bd.idealWidth = 150;
    bd.idealHeight = 150;
    bd.minHeight = 150;
    bd.minWidth = 150;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Cursor Top";
    bd.baseFile = "top_cursor";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 5;
    bd.fillBottom = 4;
    bd.fillRight = 5;
    bd.textTop = 4;
    bd.textLeft = 5;
    bd.textBottom = 4;
    bd.textRight = 5;
    bd.idealWidth = 135;
    bd.idealHeight = 96;
    bd.minHeight = 135;
    bd.minWidth = 96;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Cursor Bottom";
    bd.baseFile = "bottom_cursor";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 5;
    bd.fillBottom = 4;
    bd.fillRight = 5;
    bd.textTop = 4;
    bd.textLeft = 5;
    bd.textBottom = 4;
    bd.textRight = 5;
    bd.idealWidth = 135;
    bd.idealHeight = 96;
    bd.minHeight = 135;
    bd.minWidth = 96;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Cursor Left";
    bd.baseFile = "left_cursor";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 4;
    bd.fillBottom = 4;
    bd.fillRight = 4;
    bd.textTop = 4;
    bd.textLeft = 4;
    bd.textBottom = 4;
    bd.textRight = 4;
    bd.idealWidth = 96;
    bd.idealHeight = 135;
    bd.minHeight = 96;
    bd.minWidth = 135;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Cursor Right";
    bd.baseFile = "right_cursor";
    bd.multiColor = 1;
    bd.fillTop = 4;
    bd.fillLeft = 4;
    bd.fillBottom = 4;
    bd.fillRight = 4;
    bd.textTop = 4;
    bd.textLeft = 4;
    bd.textBottom = 4;
    bd.textRight = 4;
    bd.idealWidth = 96;
    bd.idealHeight = 135;
    bd.minHeight = 96;
    bd.minWidth = 135;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Custom Frame";
    bd.baseFile = "CustomFrame";
    bd.multiColor = 1;
    bd.fillTop = 14;
    bd.fillLeft = 14;
    bd.fillBottom = 14;
    bd.fillRight = 14;
    bd.textTop = 14;
    bd.textLeft = 14;
    bd.textBottom = 14;
    bd.textRight = 14;
    bd.idealWidth = 96;
    bd.idealHeight = 96;
    bd.minHeight = 45;
    bd.minWidth = 45;
    bd.incHeight = 17;
    bd.incWidth = 17;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Cursor Bottom with Hole";
    bd.baseFile = "cursorHoleDown";
    bd.multiColor = 1;
    bd.fillTop = 14;
    bd.fillLeft = 14;
    bd.fillBottom = 14;
    bd.fillRight = 14;
    bd.textTop = 14;
    bd.textLeft = 14;
    bd.textBottom = 14;
    bd.textRight = 14;
    bd.idealWidth = 141;
    bd.idealHeight = 69;
    bd.minHeight = 69;
    bd.minWidth = 141;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Cursor Top with Hole";
    bd.baseFile = "cursorHoleUp";
    bd.multiColor = 1;
    bd.fillTop = 14;
    bd.fillLeft = 14;
    bd.fillBottom = 14;
    bd.fillRight = 14;
    bd.textTop = 14;
    bd.textLeft = 14;
    bd.textBottom = 14;
    bd.textRight = 14;
    bd.idealWidth = 141;
    bd.idealHeight = 69;
    bd.minHeight = 69;
    bd.minWidth = 141;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Cursor Right with Hole";
    bd.baseFile = "cursorHoleRight";
    bd.multiColor = 1;
    bd.fillTop = 14;
    bd.fillLeft = 14;
    bd.fillBottom = 14;
    bd.fillRight = 14;
    bd.textTop = 14;
    bd.textLeft = 14;
    bd.textBottom = 14;
    bd.textRight = 14;
    bd.idealWidth = 69;
    bd.idealHeight = 141;
    bd.minHeight = 141;
    bd.minWidth = 69;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Cursor Left with Hole";
    bd.baseFile = "cursorHoleLeft";
    bd.multiColor = 1;
    bd.fillTop = 14;
    bd.fillLeft = 14;
    bd.fillBottom = 14;
    bd.fillRight = 14;
    bd.textTop = 14;
    bd.textLeft = 14;
    bd.textBottom = 14;
    bd.textRight = 14;
    bd.idealWidth = 69;
    bd.idealHeight = 141;
    bd.minHeight = 141;
    bd.minWidth = 69;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Windows Style Popup (Status Bar)";
    bd.baseFile = "WindowsPopupStatus";
    bd.multiColor = 1;
    bd.fillTop = 25;
    bd.fillLeft = 6;
    bd.fillBottom = 26;
    bd.fillRight = 7;
    bd.textTop = 25;
    bd.textLeft = 6;
    bd.textBottom = 26;
    bd.textRight = 7;
    bd.idealWidth = 100;
    bd.idealHeight = 100;
    bd.minHeight = 52;
    bd.minWidth = 24;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Windows Style Popup";
    bd.baseFile = "WindowsPopup";
    bd.multiColor = 1;
    bd.fillTop = 25;
    bd.fillLeft = 6;
    bd.fillBottom = 6;
    bd.fillRight = 6;
    bd.textTop = 25;
    bd.textLeft = 6;
    bd.textBottom = 6;
    bd.textRight = 6;
    bd.idealWidth = 100;
    bd.idealHeight = 100;
    bd.minHeight = 52;
    bd.minWidth = 24;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Help Down";
    bd.baseFile = "HelpDown";
    bd.multiColor = 1;
    bd.fillTop = 25;
    bd.fillLeft = 6;
    bd.fillBottom = 6;
    bd.fillRight = 6;
    bd.textTop = 25;
    bd.textLeft = 6;
    bd.textBottom = 6;
    bd.textRight = 6;
    bd.idealWidth = 150;
    bd.idealHeight = 150;
    bd.minHeight = 85;
    bd.minWidth = 63;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Help Down Reversed";
    bd.baseFile = "HelpDown2";
    bd.multiColor = 1;
    bd.fillTop = 6;
    bd.fillLeft = 6;
    bd.fillBottom = 25;
    bd.fillRight = 6;
    bd.textTop = 6;
    bd.textLeft = 6;
    bd.textBottom = 25;
    bd.textRight = 6;
    bd.idealWidth = 150;
    bd.idealHeight = 150;
    bd.minHeight = 85;
    bd.minWidth = 63;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 15";
    bd.baseFile = "leftMenuRounded15";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 9;
    bd.idealHeight = 15;
    bd.minHeight = 9;
    bd.minWidth = 15;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 25";
    bd.baseFile = "leftMenuRounded25";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 14;
    bd.idealHeight = 25;
    bd.minHeight = 14;
    bd.minWidth = 25;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 35";
    bd.baseFile = "leftMenuRounded35";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 19;
    bd.idealHeight = 35;
    bd.minHeight = 19;
    bd.minWidth = 35;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 45";
    bd.baseFile = "leftMenuRounded45";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 24;
    bd.idealHeight = 45;
    bd.minHeight = 24;
    bd.minWidth = 45;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 55";
    bd.baseFile = "leftMenuRounded55";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 29;
    bd.idealHeight = 55;
    bd.minHeight = 29;
    bd.minWidth = 55;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 65";
    bd.baseFile = "leftMenuRounded65";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 34;
    bd.idealHeight = 65;
    bd.minHeight = 34;
    bd.minWidth = 65;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 75";
    bd.baseFile = "leftMenuRounded75";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 39;
    bd.idealHeight = 75;
    bd.minHeight = 39;
    bd.minWidth = 75;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 85";
    bd.baseFile = "leftMenuRounded85";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 44;
    bd.idealHeight = 85;
    bd.minHeight = 44;
    bd.minWidth = 85;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 95";
    bd.baseFile = "leftMenuRounded95";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 49;
    bd.idealHeight = 95;
    bd.minHeight = 49;
    bd.minWidth = 95;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 105";
    bd.baseFile = "leftMenuRounded105";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 54;
    bd.idealHeight = 105;
    bd.minHeight = 54;
    bd.minWidth = 105;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 115";
    bd.baseFile = "leftMenuRounded115";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 59;
    bd.idealHeight = 115;
    bd.minHeight = 59;
    bd.minWidth = 115;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 125";
    bd.baseFile = "leftMenuRounded125";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 64;
    bd.idealHeight = 125;
    bd.minHeight = 64;
    bd.minWidth = 125;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 135";
    bd.baseFile = "leftMenuRounded135";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 69;
    bd.idealHeight = 135;
    bd.minHeight = 69;
    bd.minWidth = 135;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 145";
    bd.baseFile = "leftMenuRounded145";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 74;
    bd.idealHeight = 145;
    bd.minHeight = 74;
    bd.minWidth = 145;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 155";
    bd.baseFile = "leftMenuRounded155";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 79;
    bd.idealHeight = 155;
    bd.minHeight = 79;
    bd.minWidth = 155;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 165";
    bd.baseFile = "leftMenuRounded165";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 84;
    bd.idealHeight = 165;
    bd.minHeight = 84;
    bd.minWidth = 165;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 175";
    bd.baseFile = "leftMenuRounded175";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 89;
    bd.idealHeight = 175;
    bd.minHeight = 89;
    bd.minWidth = 175;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 185";
    bd.baseFile = "leftMenuRounded185";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 94;
    bd.idealHeight = 185;
    bd.minHeight = 94;
    bd.minWidth = 185;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Left Rounded 195";
    bd.baseFile = "leftMenuRounded195";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 99;
    bd.idealHeight = 195;
    bd.minHeight = 99;
    bd.minWidth = 195;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 15";
    bd.baseFile = "rightMenuRounded15";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 9;
    bd.idealHeight = 15;
    bd.minHeight = 9;
    bd.minWidth = 15;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 25";
    bd.baseFile = "rightMenuRounded25";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 14;
    bd.idealHeight = 25;
    bd.minHeight = 14;
    bd.minWidth = 25;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 35";
    bd.baseFile = "rightMenuRounded35";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 19;
    bd.idealHeight = 35;
    bd.minHeight = 19;
    bd.minWidth = 35;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 45";
    bd.baseFile = "rightMenuRounded45";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 24;
    bd.idealHeight = 45;
    bd.minHeight = 24;
    bd.minWidth = 45;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 55";
    bd.baseFile = "rightMenuRounded55";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 29;
    bd.idealHeight = 55;
    bd.minHeight = 29;
    bd.minWidth = 55;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 65";
    bd.baseFile = "rightMenuRounded65";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 34;
    bd.idealHeight = 65;
    bd.minHeight = 34;
    bd.minWidth = 65;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 75";
    bd.baseFile = "rightMenuRounded75";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 39;
    bd.idealHeight = 75;
    bd.minHeight = 39;
    bd.minWidth = 75;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 85";
    bd.baseFile = "rightMenuRounded85";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 44;
    bd.idealHeight = 85;
    bd.minHeight = 44;
    bd.minWidth = 85;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 95";
    bd.baseFile = "rightMenuRounded95";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 49;
    bd.idealHeight = 95;
    bd.minHeight = 49;
    bd.minWidth = 95;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 105";
    bd.baseFile = "rightMenuRounded105";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 54;
    bd.idealHeight = 105;
    bd.minHeight = 54;
    bd.minWidth = 105;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 115";
    bd.baseFile = "rightMenuRounded115";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 59;
    bd.idealHeight = 115;
    bd.minHeight = 59;
    bd.minWidth = 115;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 125";
    bd.baseFile = "rightMenuRounded125";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 64;
    bd.idealHeight = 125;
    bd.minHeight = 64;
    bd.minWidth = 125;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 135";
    bd.baseFile = "rightMenuRounded135";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 69;
    bd.idealHeight = 135;
    bd.minHeight = 69;
    bd.minWidth = 135;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 145";
    bd.baseFile = "rightMenuRounded145";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 74;
    bd.idealHeight = 145;
    bd.minHeight = 74;
    bd.minWidth = 145;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 155";
    bd.baseFile = "rightMenuRounded155";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 79;
    bd.idealHeight = 155;
    bd.minHeight = 79;
    bd.minWidth = 155;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 165";
    bd.baseFile = "rightMenuRounded165";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 84;
    bd.idealHeight = 165;
    bd.minHeight = 84;
    bd.minWidth = 165;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 175";
    bd.baseFile = "rightMenuRounded175";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 89;
    bd.idealHeight = 175;
    bd.minHeight = 89;
    bd.minWidth = 175;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 185";
    bd.baseFile = "rightMenuRounded185";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 94;
    bd.idealHeight = 185;
    bd.minHeight = 94;
    bd.minWidth = 185;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Right Rounded 195";
    bd.baseFile = "rightMenuRounded195";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 99;
    bd.idealHeight = 195;
    bd.minHeight = 99;
    bd.minWidth = 195;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 15";
    bd.baseFile = "topMenuRounded15";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 15;
    bd.idealHeight = 9;
    bd.minHeight = 15;
    bd.minWidth = 9;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 25";
    bd.baseFile = "topMenuRounded25";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 25;
    bd.idealHeight = 14;
    bd.minHeight = 25;
    bd.minWidth = 14;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 35";
    bd.baseFile = "topMenuRounded35";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 35;
    bd.idealHeight = 19;
    bd.minHeight = 35;
    bd.minWidth = 19;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 45";
    bd.baseFile = "topMenuRounded45";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 45;
    bd.idealHeight = 24;
    bd.minHeight = 45;
    bd.minWidth = 24;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 55";
    bd.baseFile = "topMenuRounded55";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 55;
    bd.idealHeight = 29;
    bd.minHeight = 55;
    bd.minWidth = 29;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 65";
    bd.baseFile = "topMenuRounded65";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 65;
    bd.idealHeight = 34;
    bd.minHeight = 65;
    bd.minWidth = 34;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 75";
    bd.baseFile = "topMenuRounded75";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 75;
    bd.idealHeight = 39;
    bd.minHeight = 75;
    bd.minWidth = 39;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 85";
    bd.baseFile = "topMenuRounded85";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 85;
    bd.idealHeight = 44;
    bd.minHeight = 85;
    bd.minWidth = 44;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 95";
    bd.baseFile = "topMenuRounded95";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 95;
    bd.idealHeight = 49;
    bd.minHeight = 95;
    bd.minWidth = 49;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 105";
    bd.baseFile = "topMenuRounded105";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 105;
    bd.idealHeight = 54;
    bd.minHeight = 105;
    bd.minWidth = 54;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 115";
    bd.baseFile = "topMenuRounded115";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 115;
    bd.idealHeight = 59;
    bd.minHeight = 115;
    bd.minWidth = 59;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 125";
    bd.baseFile = "topMenuRounded125";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 125;
    bd.idealHeight = 64;
    bd.minHeight = 125;
    bd.minWidth = 64;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 135";
    bd.baseFile = "topMenuRounded135";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 135;
    bd.idealHeight = 69;
    bd.minHeight = 135;
    bd.minWidth = 69;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 145";
    bd.baseFile = "topMenuRounded145";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 145;
    bd.idealHeight = 74;
    bd.minHeight = 145;
    bd.minWidth = 74;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 155";
    bd.baseFile = "topMenuRounded155";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 155;
    bd.idealHeight = 79;
    bd.minHeight = 155;
    bd.minWidth = 79;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 165";
    bd.baseFile = "topMenuRounded165";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 165;
    bd.idealHeight = 84;
    bd.minHeight = 165;
    bd.minWidth = 84;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 175";
    bd.baseFile = "topMenuRounded175";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 175;
    bd.idealHeight = 89;
    bd.minHeight = 175;
    bd.minWidth = 89;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 185";
    bd.baseFile = "topMenuRounded185";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 185;
    bd.idealHeight = 94;
    bd.minHeight = 185;
    bd.minWidth = 94;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Top Rounded 195";
    bd.baseFile = "topMenuRounded195";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 195;
    bd.idealHeight = 99;
    bd.minHeight = 195;
    bd.minWidth = 99;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 15";
    bd.baseFile = "bottomMenuRounded15";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 15;
    bd.idealHeight = 9;
    bd.minHeight = 15;
    bd.minWidth = 9;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 25";
    bd.baseFile = "bottomMenuRounded25";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 25;
    bd.idealHeight = 14;
    bd.minHeight = 25;
    bd.minWidth = 14;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 35";
    bd.baseFile = "bottomMenuRounded35";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 35;
    bd.idealHeight = 19;
    bd.minHeight = 35;
    bd.minWidth = 19;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 45";
    bd.baseFile = "bottomMenuRounded45";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 45;
    bd.idealHeight = 24;
    bd.minHeight = 45;
    bd.minWidth = 24;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 55";
    bd.baseFile = "bottomMenuRounded55";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 55;
    bd.idealHeight = 29;
    bd.minHeight = 55;
    bd.minWidth = 29;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 65";
    bd.baseFile = "bottomMenuRounded65";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 65;
    bd.idealHeight = 34;
    bd.minHeight = 65;
    bd.minWidth = 34;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 75";
    bd.baseFile = "bottomMenuRounded75";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 75;
    bd.idealHeight = 39;
    bd.minHeight = 75;
    bd.minWidth = 39;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 85";
    bd.baseFile = "bottomMenuRounded85";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 85;
    bd.idealHeight = 44;
    bd.minHeight = 85;
    bd.minWidth = 44;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 95";
    bd.baseFile = "bottomMenuRounded95";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 95;
    bd.idealHeight = 49;
    bd.minHeight = 95;
    bd.minWidth = 49;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 105";
    bd.baseFile = "bottomMenuRounded105";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 105;
    bd.idealHeight = 54;
    bd.minHeight = 105;
    bd.minWidth = 54;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 115";
    bd.baseFile = "bottomMenuRounded115";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 115;
    bd.idealHeight = 59;
    bd.minHeight = 115;
    bd.minWidth = 59;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 125";
    bd.baseFile = "bottomMenuRounded125";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 125;
    bd.idealHeight = 64;
    bd.minHeight = 125;
    bd.minWidth = 64;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 135";
    bd.baseFile = "bottomMenuRounded135";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 135;
    bd.idealHeight = 69;
    bd.minHeight = 135;
    bd.minWidth = 69;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 145";
    bd.baseFile = "bottomMenuRounded145";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 145;
    bd.idealHeight = 74;
    bd.minHeight = 145;
    bd.minWidth = 74;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 155";
    bd.baseFile = "bottomMenuRounded155";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 155;
    bd.idealHeight = 79;
    bd.minHeight = 155;
    bd.minWidth = 79;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 165";
    bd.baseFile = "bottomMenuRounded165";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 165;
    bd.idealHeight = 84;
    bd.minHeight = 165;
    bd.minWidth = 84;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 175";
    bd.baseFile = "bottomMenuRounded175";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 175;
    bd.idealHeight = 89;
    bd.minHeight = 175;
    bd.minWidth = 89;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 185";
    bd.baseFile = "bottomMenuRounded185";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 185;
    bd.idealHeight = 94;
    bd.minHeight = 185;
    bd.minWidth = 94;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);

    bd.init();
    bd.name = "Menu Bottom Rounded 195";
    bd.baseFile = "bottomMenuRounded195";
    bd.multiColor = 1;
    bd.fillTop = 2;
    bd.fillLeft = 2;
    bd.fillBottom = 2;
    bd.fillRight = 2;
    bd.textTop = 3;
    bd.textLeft = 3;
    bd.textBottom = 3;
    bd.textRight = 3;
    bd.idealWidth = 195;
    bd.idealHeight = 99;
    bd.minHeight = 195;
    bd.minWidth = 99;
    bd.incHeight = 1;
    bd.incWidth = 1;
    mDraw.borderData.push_back(bd);
}

void TGraphics::initCursorFamily()
{
    DECL_TRACER("TGraphics::initCursorFamily()");

    FAMILY_t cursor;
    cursor.name = "Cursors";
    cursor.member.push_back("Target");
    cursor.member.push_back("Ball");
    cursor.member.push_back("Circle");
    cursor.member.push_back("Gunsight");
    cursor.member.push_back("Spiral");
    cursor.member.push_back("Hand");
    cursor.member.push_back("Crosshairs");
    cursor.member.push_back("ViewFinder");
    cursor.member.push_back("Metal");
    cursor.member.push_back("Smart Button Bubbled");
    mDraw.cursors.push_back(cursor);
}

void TGraphics::initCursorStyle()
{
    DECL_TRACER("TGraphics::initCursorStyle()");

    CURSOR_STYLE_t style;
    style.name = "Target";
    style.baseFile = "Target";
    style.multiColor = 1;
    style.g3Equiv.push_back(0);
    mDraw.cursorStyles.push_back(style);

    style.init();
    style.name = "Smart Button Bubbled";
    style.baseFile = "cursor_aqua";
    style.multiColor = 1;
    mDraw.cursorStyles.push_back(style);

    style.init();
    style.name = "Crosshairs";
    style.baseFile = "Crosshairs";
    style.multiColor = 1;
    style.g3Equiv.push_back(1);
    mDraw.cursorStyles.push_back(style);

    style.init();
    style.name = "View Finder";
    style.baseFile = "ViewFinder";
    style.multiColor = 1;
    mDraw.cursorStyles.push_back(style);

    style.init();
    style.name = "Circle";
    style.baseFile = "Circle";
    style.multiColor = 1;
    mDraw.cursorStyles.push_back(style);

    style.init();
    style.name = "Ball";
    style.baseFile = "Ball";
    style.multiColor = 1;
    mDraw.cursorStyles.push_back(style);

    style.init();
    style.name = "Gunsight";
    style.baseFile = "Gunsight";
    style.multiColor = 1;
    mDraw.cursorStyles.push_back(style);

    style.init();
    style.name = "Spiral";
    style.baseFile = "Spiral";
    style.multiColor = 1;
    mDraw.cursorStyles.push_back(style);

    style.init();
    style.name = "Hand";
    style.baseFile = "Hand";
    style.multiColor = 0;
    mDraw.cursorStyles.push_back(style);

    style.init();
    style.name = "Arrow";
    style.baseFile = "Arrow";
    style.multiColor = 0;
    mDraw.cursorStyles.push_back(style);

    style.init();
    style.name = "Metal";
    style.baseFile = "metal";
    style.multiColor = 0;
    mDraw.cursorStyles.push_back(style);
}

void TGraphics::initSliderFamily()
{
    DECL_TRACER("TGraphics::initSliderFamily()");

    FAMILY_t slider;
    slider.name = "Sliders";
    slider.member.push_back("Ball");
    slider.member.push_back("Line -S");
    slider.member.push_back("Line -M");
    slider.member.push_back("Line -L");
    slider.member.push_back("Circle -S");
    slider.member.push_back("Circle -M");
    slider.member.push_back("Circle -L");
    slider.member.push_back("Precision");
    slider.member.push_back("Windows");
    slider.member.push_back("Smart Button Bubbled Large");
    slider.member.push_back("Smart Button Bubbled Small");
    mDraw.sliders.push_back(slider);
}

void TGraphics::initSliderStyle()
{
    DECL_TRACER("TGraphics::initSliderStyle()");

    SLIDER_STYLE_t style;
    style.name = "Ball";
    style.baseFile = "Ball";
    style.multiColor = 1;
    style.incRepeat = 1;
    style.minSize = 15;
    style.fixedSize = 15;
    mDraw.sliderStyles.push_back(style);

    style.init();
    style.name = "Smart Button Bubbled Large";
    style.baseFile = "aqua";
    style.multiColor = 1;
    style.incRepeat = 1;
    style.minSize = 25;
    style.fixedSize = 25;
    mDraw.sliderStyles.push_back(style);

    style.init();
    style.name = "Smart Button Bubbled Small";
    style.baseFile = "aquaS";
    style.multiColor = 1;
    style.incRepeat = 1;
    style.minSize = 15;
    style.fixedSize = 15;
    mDraw.sliderStyles.push_back(style);

    style.init();
    style.name = "Rectangle -S";
    style.baseFile = "LineS";
    style.multiColor = 1;
    style.incRepeat = 1;
    style.minSize = 3;
    style.fixedSize = 5;
    style.g3Equiv.push_back(1);
    mDraw.sliderStyles.push_back(style);

    style.init();
    style.name = "Rectangle -M";
    style.baseFile = "LineM";
    style.multiColor = 1;
    style.incRepeat = 1;
    style.minSize = 3;
    style.fixedSize = 9;
    mDraw.sliderStyles.push_back(style);

    style.init();
    style.name = "Rectangle -L";
    style.baseFile = "LineL";
    style.multiColor = 1;
    style.incRepeat = 1;
    style.minSize = 3;
    style.fixedSize = 15;
    mDraw.sliderStyles.push_back(style);

    style.init();
    style.name = "Circle -S";
    style.baseFile = "CircleS";
    style.multiColor = 1;
    style.incRepeat = 1;
    style.minSize = 5;
    style.fixedSize = 5;
    mDraw.sliderStyles.push_back(style);

    style.init();
    style.name = "Circle -M";
    style.baseFile = "CircleM";
    style.multiColor = 1;
    style.incRepeat = 1;
    style.minSize = 9;
    style.fixedSize = 9;
    mDraw.sliderStyles.push_back(style);

    style.init();
    style.name = "Circle -L";
    style.baseFile = "CircleL";
    style.multiColor = 1;
    style.incRepeat = 1;
    style.minSize = 15;
    style.fixedSize = 15;
    mDraw.sliderStyles.push_back(style);

    style.init();
    style.name = "Precision";
    style.baseFile = "prec";
    style.multiColor = 1;
    style.incRepeat = 1;
    style.minSize = 28;
    style.fixedSize = 28;
    mDraw.sliderStyles.push_back(style);

    style.init();
    style.name = "Windows";
    style.baseFile = "windows";
    style.multiColor = 0;
    style.incRepeat = 1;
    style.minSize = 16;
    style.fixedSize = 16;
    mDraw.sliderStyles.push_back(style);

    style.init();
    style.name = "Windows Active";
    style.baseFile = "windowActive";
    style.multiColor = 0;
    style.incRepeat = 1;
    style.minSize = 7;
    style.fixedSize = 11;
    mDraw.sliderStyles.push_back(style);
}

void TGraphics::initEffectFamily()
{
    DECL_TRACER("TGraphics::initEffectFamily()");

    FAMILY_t effect;
    effect.name = "Outline";
    effect.member.push_back("Outline -S");
    effect.member.push_back("Outline -M");
    effect.member.push_back("Outline -L");
    effect.member.push_back("Outline -X");
    mDraw.effects.push_back(effect);

    effect.member.clear();
    effect.name = "Glow";
    effect.member.push_back("Glow -S");
    effect.member.push_back("Glow -M");
    effect.member.push_back("Glow -L");
    effect.member.push_back("Glow -X");
    mDraw.effects.push_back(effect);

    effect.member.clear();
    effect.name = "Soft Drop Shadow";
    effect.member.push_back("Soft Drop Shadow 1");
    effect.member.push_back("Soft Drop Shadow 2");
    effect.member.push_back("Soft Drop Shadow 3");
    effect.member.push_back("Soft Drop Shadow 4");
    effect.member.push_back("Soft Drop Shadow 5");
    effect.member.push_back("Soft Drop Shadow 6");
    effect.member.push_back("Soft Drop Shadow 7");
    effect.member.push_back("Soft Drop Shadow 8");
    mDraw.effects.push_back(effect);

    effect.member.clear();
    effect.name = "Medium Drop Shadow";
    effect.member.push_back("Medium Drop Shadow 1");
    effect.member.push_back("Medium Drop Shadow 2");
    effect.member.push_back("Medium Drop Shadow 3");
    effect.member.push_back("Medium Drop Shadow 4");
    effect.member.push_back("Medium Drop Shadow 5");
    effect.member.push_back("Medium Drop Shadow 6");
    effect.member.push_back("Medium Drop Shadow 7");
    effect.member.push_back("Medium Drop Shadow 8");
    mDraw.effects.push_back(effect);

    effect.member.clear();
    effect.name = "Hard Drop Shadow";
    effect.member.push_back("Hard Drop Shadow 1");
    effect.member.push_back("Hard Drop Shadow 2");
    effect.member.push_back("Hard Drop Shadow 3");
    effect.member.push_back("Hard Drop Shadow 4");
    effect.member.push_back("Hard Drop Shadow 5");
    effect.member.push_back("Hard Drop Shadow 6");
    effect.member.push_back("Hard Drop Shadow 7");
    effect.member.push_back("Hard Drop Shadow 8");
    mDraw.effects.push_back(effect);

    effect.member.clear();
    effect.name = "Soft Drop Shadow with Outline";
    effect.member.push_back("Soft Drop Shadow 1 with Outline");
    effect.member.push_back("Soft Drop Shadow 2 with Outline");
    effect.member.push_back("Soft Drop Shadow 3 with Outline");
    effect.member.push_back("Soft Drop Shadow 4 with Outline");
    effect.member.push_back("Soft Drop Shadow 5 with Outline");
    effect.member.push_back("Soft Drop Shadow 6 with Outline");
    effect.member.push_back("Soft Drop Shadow 7 with Outline");
    effect.member.push_back("Soft Drop Shadow 8 with Outline");
    mDraw.effects.push_back(effect);

    effect.member.clear();
    effect.name = "Medium Drop Shadow with Outline";
    effect.member.push_back("Medium Drop Shadow 1 with Outline");
    effect.member.push_back("Medium Drop Shadow 2 with Outline");
    effect.member.push_back("Medium Drop Shadow 3 with Outline");
    effect.member.push_back("Medium Drop Shadow 4 with Outline");
    effect.member.push_back("Medium Drop Shadow 5 with Outline");
    effect.member.push_back("Medium Drop Shadow 6 with Outline");
    effect.member.push_back("Medium Drop Shadow 7 with Outline");
    effect.member.push_back("Medium Drop Shadow 8 with Outline");
    mDraw.effects.push_back(effect);

    effect.member.clear();
    effect.name = "Hard Drop Shadow with Outline";
    effect.member.push_back("Hard Drop Shadow 1 with Outline");
    effect.member.push_back("Hard Drop Shadow 2 with Outline");
    effect.member.push_back("Hard Drop Shadow 3 with Outline");
    effect.member.push_back("Hard Drop Shadow 4 with Outline");
    effect.member.push_back("Hard Drop Shadow 5 with Outline");
    effect.member.push_back("Hard Drop Shadow 6 with Outline");
    effect.member.push_back("Hard Drop Shadow 7 with Outline");
    effect.member.push_back("Hard Drop Shadow 8 with Outline");
    mDraw.effects.push_back(effect);
}

void TGraphics::initEffectStyle()
{
    DECL_TRACER("TGraphics::initEffectStyle()");

    EFFECT_STYLE_t style;
    style.name = "Outline -S";
    style.number = 1;
    style.startx = 2;
    style.starty = 2;
    style.height = 3;
    style.width = 3;
    style.cutout = 1;
    style.pixelMap = "80FF80FFFFFF80FF80";
    mDraw.effectStyles.push_back(style);

    style.name = "Outline -M";
    style.number = 2;
    style.startx = 3;
    style.starty = 3;
    style.height = 5;
    style.width = 5;
    style.cutout = 1;
    style.pixelMap = "0080FF800080FFFFFF80FFFFFFFFFF80FFFFFF800080FF8000";
    mDraw.effectStyles.push_back(style);

    style.name = "Outline -L";
    style.number = 3;
    style.startx = 4;
    style.starty = 4;
    style.height = 7;
    style.width = 7;
    style.cutout = 1;
    style.pixelMap = "000080FF8000000080FFFFFF800080FFFFFFFFFF80FFFFFFFFFFFFFF80FFFFFFFFFF800080FFFFFF8000000080FF800000";
    mDraw.effectStyles.push_back(style);

    style.name = "Outline -X";
    style.number = 4;
    style.startx = 5;
    style.starty = 5;
    style.height = 9;
    style.width = 9;
    style.cutout = 1;
    style.pixelMap = "00000080FF80000000000080FFFFFF8000000080FFFFFFFFFF800080FFFFFFFFFFFFFF80FFFFFFFFFFFFFFFFFF80FFFFFFFFFFFFFF800080FFFFFFFFFF8000000080FFFFFF80000000000080FF80000000";
    mDraw.effectStyles.push_back(style);

    style.name = "Glow -S";
    style.number = 5;
    style.startx = 3;
    style.starty = 3;
    style.height = 5;
    style.width = 5;
    style.cutout = 0;
    style.pixelMap = "555555555555AAAAAA5555AAFFAA5555AAAAAA555555555555";
    mDraw.effectStyles.push_back(style);

    style.name = "Glow -M";
    style.number = 6;
    style.startx = 5;
    style.starty = 5;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "323232323232323232326464646464646432326496969696966432326496C8C8C8966432326496C8FAC8966432326496C8C8C8966432326496969696966432326464646464646432323232323232323232";
    mDraw.effectStyles.push_back(style);

    style.name = "Glow -L";
    style.number = 7;
    style.startx = 7;
    style.starty = 7;
    style.height = 13;
    style.width = 13;
    style.cutout = 0;
    style.pixelMap = "242424242424242424242424242448484848484848484848482424486C6C6C6C6C6C6C6C6C482424486C909090909090906C482424486C90B4B4B4B4B4906C482424486C90B4D8D8D8B4906C482424486C90B4D8FFD8B4906C482424486C90B4D8D8D8B4906C482424486C90B4B4B4B4B4906C482424486C909090909090906C482424486C6C6C6C6C6C6C6C6C48242448484848484848484848482424242424242424242424242424";
    mDraw.effectStyles.push_back(style);

    style.name = "Glow -X";
    style.number = 8;
    style.startx = 8;
    style.starty = 8;
    style.height = 15;
    style.width = 15;
    style.cutout = 0;
    style.pixelMap = "20202020202020202020202020202020404040404040404040404040402020406060606060606060606060402020406080808080808080808060402020406080A0A0A0A0A0A0A08060402020406080A0C0C0C0C0C0A08060402020406080A0C0E0E0E0C0A08060402020406080A0C0E0FFE0C0A08060402020406080A0C0E0E0E0C0A08060402020406080A0C0C0C0C0C0A08060402020406080A0A0A0A0A0A0A080604020204060808080808080808080604020204060606060606060606060604020204040404040404040404040404020202020202020202020202020202020";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 1";
    style.number = 9;
    style.startx = 4;
    style.starty = 4;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "323232323232323232326464646464646432326496969696966432326496C8C8C8966432326496C8FAC8966432326496C8C8C8966432326496969696966432326464646464646432323232323232323232";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 2";
    style.number = 10;
    style.startx = 3;
    style.starty = 3;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "323232323232323232326464646464646432326496969696966432326496C8C8C8966432326496C8FAC8966432326496C8C8C8966432326496969696966432326464646464646432323232323232323232";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 3";
    style.number = 11;
    style.startx = 2;
    style.starty = 2;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "323232323232323232326464646464646432326496969696966432326496C8C8C8966432326496C8FAC8966432326496C8C8C8966432326496969696966432326464646464646432323232323232323232";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 4";
    style.number = 12;
    style.startx = 1;
    style.starty = 1;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "323232323232323232326464646464646432326496969696966432326496C8C8C8966432326496C8FAC8966432326496C8C8C8966432326496969696966432326464646464646432323232323232323232";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 5";
    style.number = 13;
    style.startx = 2;
    style.starty = 2;
    style.height = 11;
    style.width = 11;
    style.cutout = 0;
    style.pixelMap = "000000000000000000000000000000000000000000000000323232323232323232000032646464646464643200003264969696969664320000326496C8C8C89664320000326496C8FAC89664320000326496C8C8C8966432000032649696969696643200003264646464646464320000323232323232323232";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 6";
    style.number = 14;
    style.startx = 1;
    style.starty = 1;
    style.height = 11;
    style.width = 11;
    style.cutout = 0;
    mDraw.effectStyles.push_back(style);

    style.pixelMap = "000000000000000000000000000000000000000000000000323232323232323232000032646464646464643200003264969696969664320000326496C8C8C89664320000326496C8FAC89664320000326496C8C8C8966432000032649696969696643200003264646464646464320000323232323232323232";
    style.name = "Soft Drop Shadow 7";
    style.number = 15;
    style.startx = 2;
    style.starty = 2;
    style.height = 13;
    style.width = 13;
    style.cutout = 0;
    style.pixelMap = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000323232323232323232000000003264646464646464320000000032649696969696643200000000326496C8C8C896643200000000326496C8FAC896643200000000326496C8C8C8966432000000003264969696969664320000000032646464646464643200000000323232323232323232";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 8";
    style.number = 16;
    style.startx = 1;
    style.starty = 1;
    style.height = 13;
    style.width = 13;
    style.cutout = 0;
    style.pixelMap = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000323232323232323232000000003264646464646464320000000032649696969696643200000000326496C8C8C896643200000000326496C8FAC896643200000000326496C8C8C8966432000000003264969696969664320000000032646464646464643200000000323232323232323232";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 1";
    style.number = 17;
    style.startx = 2;
    style.starty = 2;
    style.height = 5;
    style.width = 5;
    style.cutout = 0;
    style.pixelMap = "555555555555AAAAAA5555AAFFAA5555AAAAAA555555555555";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 2";
    style.number = 18;
    style.startx = 1;
    style.starty = 1;
    style.height = 5;
    style.width = 5;
    style.cutout = 0;
    style.pixelMap = "555555555555AAAAAA5555AAFFAA5555AAAAAA555555555555";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 3";
    style.number = 19;
    style.startx = 2;
    style.starty = 2;
    style.height = 7;
    style.width = 7;
    style.cutout = 0;
    style.pixelMap = "000000000000000000000000000000005555555555000055AAAAAA55000055AAFFAA55000055AAAAAA5500005555555555";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 4";
    style.number = 20;
    style.startx = 1;
    style.starty = 1;
    style.height = 7;
    style.width = 7;
    style.cutout = 0;
    style.pixelMap = "000000000000000000000000000000005555555555000055AAAAAA55000055AAFFAA55000055AAAAAA5500005555555555";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 5";
    style.number = 21;
    style.startx = 2;
    style.starty = 2;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "0000000000000000000000000000000000000000000000000000000000000000000000000000000055555555550000000055AAAAAA550000000055AAFFAA550000000055AAAAAA55000000005555555555";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 6";
    style.number = 22;
    style.startx = 1;
    style.starty = 1;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "0000000000000000000000000000000000000000000000000000000000000000000000000000000055555555550000000055AAAAAA550000000055AAFFAA550000000055AAAAAA55000000005555555555";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 7";
    style.number = 23;
    style.startx = 2;
    style.starty = 2;
    style.height = 11;
    style.width = 11;
    style.cutout = 0;
    style.pixelMap = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000555555555500000000000055AAAAAA5500000000000055AAFFAA5500000000000055AAAAAA550000000000005555555555";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 8";
    style.number = 24;
    style.startx = 1;
    style.starty = 1;
    style.height = 11;
    style.width = 11;
    style.cutout = 0;
    style.pixelMap = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000555555555500000000000055AAAAAA5500000000000055AAFFAA5500000000000055AAAAAA550000000000005555555555";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 1";
    style.number = 25;
    style.startx = 2;
    style.starty = 2;
    style.height = 3;
    style.width = 3;
    style.cutout = 0;
    style.pixelMap = "0000000000000000FF";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 2";
    style.number = 26;
    style.startx = 1;
    style.starty = 1;
    style.height = 3;
    style.width = 3;
    style.cutout = 0;
    style.pixelMap = "0000000000000000FF";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 3";
    style.number = 27;
    style.startx = 2;
    style.starty = 2;
    style.height = 5;
    style.width = 5;
    style.cutout = 0;
    style.pixelMap = "000000000000000000000000000000000000000000000000FF";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 4";
    style.number = 28;
    style.startx = 1;
    style.starty = 1;
    style.height = 5;
    style.width = 5;
    style.cutout = 0;
    style.pixelMap = "000000000000000000000000000000000000000000000000FF";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 5";
    style.number = 29;
    style.startx = 2;
    style.starty = 2;
    style.height = 7;
    style.width = 7;
    style.cutout = 0;
    style.pixelMap = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000FF";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 6";
    style.number = 30;
    style.startx = 1;
    style.starty = 1;
    style.height = 7;
    style.width = 7;
    style.cutout = 0;
    style.pixelMap = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000FF";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 7";
    style.number = 31;
    style.startx = 2;
    style.starty = 2;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000FF";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 8";
    style.number = 32;
    style.startx = 1;
    style.starty = 1;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000FF";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 1 with Outline";
    style.number = 33;
    style.startx = 4;
    style.starty = 4;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "323232323232323232326464646464646432326496FF96969664323264FFFFFFC8966432326496FFFAC8966432326496C8C8C8966432326496969696966432326464646464646432323232323232323232";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 2 with Outline";
    style.number = 34;
    style.startx = 3;
    style.starty = 3;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "3232323232323232323280FF80646464643232FFFFFF96969664323280FFC8C8C8966432326496C8FAC8966432326496C8C8C8966432326496969696966432326464646464646432323232323232323232";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 3 with Outline";
    style.number = 35;
    style.startx = 2;
    style.starty = 2;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "80FF80323232323232FFFFFF64646464643280FF96969696966432326496C8C8C8966432326496C8FAC8966432326496C8C8C8966432326496969696966432326464646464646432323232323232323232";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 4 with Outline";
    style.number = 36;
    style.startx = 2;
    style.starty = 2;
    style.height = 11;
    style.width = 11;
    style.cutout = 0;
    style.pixelMap = "80FF800000000000000000FFFFFF323232323232320080FF806464646464643200003264969696969664320000326496C8C8C89664320000326496C8FAC89664320000326496C8C8C8966432000032649696969696643200003264646464646464320000323232323232323232000000000000000000000000";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 5 with Outline";
    style.number = 37;
    style.startx = 2;
    style.starty = 2;
    style.height = 11;
    style.width = 11;
    style.cutout = 0;
    style.pixelMap = "80FF800000000000000000FFFFFF000000000000000080FF803232323232323232000032646464646464643200003264969696969664320000326496C8C8C89664320000326496C8FAC89664320000326496C8C8C8966432000032649696969696643200003264646464646464320000323232323232323232";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 6 with Outline";
    style.number = 38;
    style.startx = 2;
    style.starty = 2;
    style.height = 13;
    style.width = 13;
    style.cutout = 0;
    style.pixelMap = "80FF8000000000000000000000FFFFFF0000000000000000000080FF8000000000000000000000000000323232323232323232000000003264646464646464320000000032649696969696643200000000326496C8C8C896643200000000326496C8FAC896643200000000326496C8C8C89664320000000032649696969696643200000000326464646464646432000000003232323232323232320000000000000000000000000000";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 7 with Outline";
    style.number = 39;
    style.startx = 2;
    style.starty = 2;
    style.height = 13;
    style.width = 13;
    style.cutout = 0;
    style.pixelMap = "80FF8000000000000000000000FFFFFF0000000000000000000080FF80000000000000000000000000000000000000000000000000000000323232323232323232000000003264646464646464320000000032649696969696643200000000326496C8C8C896643200000000326496C8FAC896643200000000326496C8C8C8966432000000003264969696969664320000000032646464646464643200000000323232323232323232";
    mDraw.effectStyles.push_back(style);

    style.name = "Soft Drop Shadow 8 with Outline";
    style.number = 40;
    style.startx = 2;
    style.starty = 2;
    style.height = 15;
    style.width = 15;
    style.cutout = 0;
    style.pixelMap = "80FF80000000000000000000000000FFFFFF00000000000000000000000080FF800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000323232323232323232000000000000326464646464646432000000000000326496969696966432000000000000326496C8C8C8966432000000000000326496C8FAC8966432000000000000326496C8C8C896643200000000000032649696969696643200000000000032646464646464643200000000000032323232323232323200000000000000000000000000000000";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 1 with Outline";
    style.number = 41;
    style.startx = 2;
    style.starty = 2;
    style.height = 5;
    style.width = 5;
    style.cutout = 0;
    style.pixelMap = "80FF805555FFFFFFAA5580FFFFAA5555AAAAAA555555555555";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 2 with Outline";
    style.number = 42;
    style.startx = 2;
    style.starty = 2;
    style.height = 7;
    style.width = 7;
    style.cutout = 0;
    style.pixelMap = "80FF8000000000FFFFFF5555550080FFAAAAAA55000055AAFFAA55000055AAAAAA55000055555555550000000000000000";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 3 with Outline";
    style.number = 43;
    style.startx = 2;
    style.starty = 2;
    style.height = 7;
    style.width = 7;
    style.cutout = 0;
    style.pixelMap = "80FF8000000000FFFFFF0000000080FF8055555555000055AAAAAA55000055AAFFAA55000055AAAAAA5500005555555555";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 4 with Outline";
    style.number = 44;
    style.startx = 2;
    style.starty = 2;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "80FF80000000000000FFFFFF00000000000080FF8000000000000000000055555555550000000055AAAAAA550000000055AAFFAA550000000055AAAAAA5500000000555555555500000000000000000000";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 5 with Outline";
    style.number = 45;
    style.startx = 2;
    style.starty = 2;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "80FF80000000000000FFFFFF00000000000080FF800000000000000000000000000000000000000055555555550000000055AAAAAA550000000055AAFFAA550000000055AAAAAA55000000005555555555";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 6 with Outline";
    style.number = 46;
    style.startx = 2;
    style.starty = 2;
    style.height = 11;
    style.width = 11;
    style.cutout = 0;
    style.pixelMap = "80FF800000000000000000FFFFFF000000000000000080FF800000000000000000000000000000000000000000000000000000000000000000000000555555555500000000000055AAAAAA5500000000000055AAFFAA5500000000000055AAAAAA550000000000005555555555000000000000000000000000";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 7 with Outline";
    style.number = 47;
    style.startx = 2;
    style.starty = 2;
    style.height = 11;
    style.width = 11;
    style.cutout = 0;
    style.pixelMap = "80FF800000000000000000FFFFFF000000000000000080FF800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000555555555500000000000055AAAAAA5500000000000055AAFFAA5500000000000055AAAAAA550000000000005555555555";
    mDraw.effectStyles.push_back(style);

    style.name = "Medium Drop Shadow 8 with Outline";
    style.number = 48;
    style.startx = 2;
    style.starty = 2;
    style.height = 13;
    style.width = 13;
    style.cutout = 0;
    style.pixelMap = "80FF8000000000000000000000FFFFFF0000000000000000000080FF800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005555555555000000000000000055AAAAAA55000000000000000055AAFFAA55000000000000000055AAAAAA55000000000000000055555555550000000000000000000000000000";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 1 with Outline";
    style.number = 49;
    style.startx = 2;
    style.starty = 2;
    style.height = 3;
    style.width = 3;
    style.cutout = 0;
    style.pixelMap = "80FF80FFFFFF80FFFF";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 2 with Outline";
    style.number = 50;
    style.startx = 2;
    style.starty = 2;
    style.height = 5;
    style.width = 5;
    style.cutout = 0;
    style.pixelMap = "80FF800000FFFFFF000080FF800000000000FF000000000000";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 3 with Outline";
    style.number = 51;
    style.startx = 2;
    style.starty = 2;
    style.height = 5;
    style.width = 5;
    style.cutout = 0;
    style.pixelMap = "80FF800000FFFFFF000080FF800000000000000000000000FF";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 4 with Outline";
    style.number = 52;
    style.startx = 2;
    style.starty = 2;
    style.height = 7;
    style.width = 7;
    style.cutout = 0;
    style.pixelMap = "80FF8000000000FFFFFF0000000080FF800000000000000000000000000000000000000000000000FF0000000000000000";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 5 with Outline";
    style.number = 53;
    style.startx = 2;
    style.starty = 2;
    style.height = 7;
    style.width = 7;
    style.cutout = 0;
    style.pixelMap = "80FF8000000000FFFFFF0000000080FF8000000000000000000000000000000000000000000000000000000000000000FF";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 6 with Outline";
    style.number = 54;
    style.startx = 2;
    style.starty = 2;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "80FF80000000000000FFFFFF00000000000080FF8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000FF00000000000000000000";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 7 with Outline";
    style.number = 55;
    style.startx = 2;
    style.starty = 2;
    style.height = 9;
    style.width = 9;
    style.cutout = 0;
    style.pixelMap = "80FF80000000000000FFFFFF00000000000080FF800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000FF";
    mDraw.effectStyles.push_back(style);

    style.name = "Hard Drop Shadow 8 with Outline";
    style.number = 56;
    style.startx = 2;
    style.starty = 2;
    style.height = 11;
    style.width = 11;
    style.cutout = 0;
    style.pixelMap = "80FF800000000000000000FFFFFF000000000000000080FF800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000FF000000000000000000000000";
    mDraw.effectStyles.push_back(style);
}

void TGraphics::initPopupEffect()
{
    DECL_TRACER("TGraphics::initPopupEffect()");

    POPUP_EFFECT_t popup;
    popup.name = "Fade";
    popup.number = 1;
    popup.valueUsed = 0;
    mDraw.popupEffects.push_back(popup);

    popup.name = "Slide %FT Left";
    popup.number = 2;
    popup.valueUsed = 1;
    mDraw.popupEffects.push_back(popup);

    popup.name = "Slide %FT Right";
    popup.number = 3;
    popup.valueUsed = 1;
    mDraw.popupEffects.push_back(popup);

    popup.name = "Slide %FT Top";
    popup.number = 4;
    popup.valueUsed = 2;
    mDraw.popupEffects.push_back(popup);

    popup.name = "Slide %FT Bottom";
    popup.number = 5;
    popup.valueUsed = 2;
    mDraw.popupEffects.push_back(popup);

    popup.name = "Slide %FT Left Fade";
    popup.number = 6;
    popup.valueUsed = 1;
    mDraw.popupEffects.push_back(popup);

    popup.name = "Slide %FT Right Fade";
    popup.number = 7;
    popup.valueUsed = 1;
    mDraw.popupEffects.push_back(popup);

    popup.name = "Slide %FT Top Fade";
    popup.number = 8;
    popup.valueUsed = 2;
    mDraw.popupEffects.push_back(popup);

    popup.name = "Slide %FT Bottom Fade";
    popup.number = 9;
    popup.valueUsed = 2;
    mDraw.popupEffects.push_back(popup);
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

bool TGraphics::writeSystemFiles(Graphics::FILE_TYPE_t ft, const QString& basePath)
{
    DECL_TRACER("TGraphics::writeSystemFiles(Graphics::FILE_TYPE_t ft, const QString& basePath)");

    if (!fs::is_directory(basePath.toStdString()))
    {
        MSG_ERROR("The path \"" << basePath.toStdString() << "\" is not a directory!");
        return false;
    }

    if (ft == Graphics::FT_THEOSYS)
    {
        QJsonObject root;
        QJsonObject borders = writeSystemBordersJson();
        root.insert("borders", borders);
        QJsonObject cursors = writeSystemCursorsJson();
        root.insert("cursors", cursors);
        QJsonObject sliders = writeSystemSlidersJson();
        root.insert("sliders", sliders);
        QJsonObject effects = writeSystemEffectsJson();
        root.insert("effects", effects);
        QJsonObject popups = writeSystemPopupsJson();
        root.insert("popups", popups);

        QJsonDocument doc;
        doc.setObject(root);
        QString metaFile = basePath + "/__system/graphics/draw.json";
        QFile file(metaFile);

        if(!file.open(QIODevice::WriteOnly))
        {
            MSG_ERROR("Error opening file \"" << metaFile.toStdString() << "\" for writing!");
            return false;
        }

        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        return true;
    }

    return false;
}

QJsonObject TGraphics::writeSystemBordersJson()
{
    DECL_TRACER("TGraphics::writeSystemBordersJson(const QString& basePath)");

    // Border family
    QJsonObject borders;
    QJsonArray borderFamily;

    vector<FAMILY_t>::iterator iter;

    for (iter = mDraw.borders.begin(); iter != mDraw.borders.end(); ++iter)
    {
        QJsonObject borderData;
        borderData.insert("name", iter->name);
        QJsonArray family;

        vector<QString>::iterator bsIter;

        for (bsIter = iter->member.begin(); bsIter != iter->member.end(); bsIter++)
            family.append(QJsonValue(*bsIter));

        borderData.insert("members", family);
        borderFamily.append(borderData);
    }

    borders.insert("borderFamilies", borderFamily);
    // Border styles
    vector<BORDER_STYLE_t>::iterator bsIter;
    QJsonArray borderStyle;

    for (bsIter = mDraw.borderStyles.begin(); bsIter != mDraw.borderStyles.end(); ++bsIter)
    {
        QJsonObject style;
        style.insert("name", bsIter->name);
        style.insert("off", bsIter->off);
        style.insert("on", bsIter->on);
        style.insert("drag", bsIter->drag);
        style.insert("drop", bsIter->drop);

        if (!bsIter->g3Equiv.empty())
        {
            QJsonArray g3equiv;
            vector<int>::iterator eqIter;

            for (eqIter = bsIter->g3Equiv.begin(); eqIter != bsIter->g3Equiv.end(); ++eqIter)
                g3equiv.append(QJsonValue(*eqIter));

            style.insert("g3equiv", g3equiv);
        }

        borderStyle.append(style);
    }

    borders.insert("borderStyles", borderStyle);
    // Border data
    vector<BORDER_DATA_t>::iterator bdIter;
    QJsonArray borderData;

    for (bdIter = mDraw.borderData.begin(); bdIter != mDraw.borderData.end(); ++bdIter)
    {
        QJsonObject data;
        data.insert("name", bdIter->name);
        data.insert("baseFile", bdIter->baseFile);
        data.insert("multiColor", bdIter->multiColor);
        data.insert("fillTop", bdIter->fillTop);
        data.insert("fillLeft", bdIter->fillLeft);
        data.insert("fillBottom", bdIter->fillBottom);
        data.insert("fillRight", bdIter->fillRight);
        data.insert("textTop", bdIter->textTop);
        data.insert("textLeft", bdIter->textLeft);
        data.insert("textBottom", bdIter->textBottom);
        data.insert("textRight", bdIter->textRight);
        data.insert("idealWidth", bdIter->idealWidth);
        data.insert("idealHeight", bdIter->idealHeight);
        data.insert("minHeight", bdIter->minHeight);
        data.insert("minWidth", bdIter->minWidth);
        data.insert("incHeight", bdIter->incHeight);
        data.insert("incWidth", bdIter->incWidth);
        borderData.append(data);
    }

    borders.insert("border", borderData);
    return borders;
}

QJsonObject TGraphics::writeSystemCursorsJson()
{
    DECL_TRACER("TGraphics::writeSystemCursorsJson()");

    // Cursor family
    QJsonObject cursors;
    QJsonArray cursorFamily;

    vector<FAMILY_t>::iterator iter;

    for (iter = mDraw.cursors.begin(); iter != mDraw.cursors.end(); ++iter)
    {
        QJsonObject cursorData;
        cursorData.insert("name", iter->name);
        QJsonArray family;

        vector<QString>::iterator bsIter;

        for (bsIter = iter->member.begin(); bsIter != iter->member.end(); bsIter++)
            family.append(QJsonValue(*bsIter));

        cursorData.insert("members", family);
        cursorFamily.append(cursorData);
    }

    cursors.insert("cursorFamilies", cursorFamily);
    // Cursor styles
    vector<CURSOR_STYLE_t>::iterator csIter;
    QJsonArray cursorStyle;

    for (csIter = mDraw.cursorStyles.begin(); csIter != mDraw.cursorStyles.end(); ++csIter)
    {
        QJsonObject style;
        style.insert("name", csIter->name);
        style.insert("baseFile", csIter->baseFile);
        style.insert("multiColor", csIter->multiColor);

        if (!csIter->g3Equiv.empty())
        {
            QJsonArray g3equiv;
            vector<int>::iterator eqIter;

            for (eqIter = csIter->g3Equiv.begin(); eqIter != csIter->g3Equiv.end(); ++eqIter)
                g3equiv.append(QJsonValue(*eqIter));

            style.insert("g3equiv", g3equiv);
        }

        cursorStyle.append(style);
    }

    cursors.insert("curserStyles", cursorStyle);
    return cursors;
}

QJsonObject TGraphics::writeSystemSlidersJson()
{
    DECL_TRACER("TGraphics::writeSystemSlidersJson()");

    // Slider family
    QJsonObject sliders;
    QJsonArray sliderFamily;

    vector<FAMILY_t>::iterator iter;

    for (iter = mDraw.sliders.begin(); iter != mDraw.sliders.end(); ++iter)
    {
        QJsonObject sliderData;
        sliderData.insert("name", iter->name);
        QJsonArray family;

        vector<QString>::iterator bsIter;

        for (bsIter = iter->member.begin(); bsIter != iter->member.end(); bsIter++)
            family.append(QJsonValue(*bsIter));

        sliderData.insert("members", family);
        sliderFamily.append(sliderData);
    }

    sliders.insert("sliderFamilies", sliderFamily);
    // Slider styles
    vector<SLIDER_STYLE_t>::iterator slIter;
    QJsonArray sliderStyle;

    for (slIter = mDraw.sliderStyles.begin(); slIter != mDraw.sliderStyles.end(); ++slIter)
    {
        QJsonObject style;
        style.insert("name", slIter->name);
        style.insert("baseFile", slIter->baseFile);
        style.insert("multiColor", slIter->multiColor);
        style.insert("incRepeat", slIter->incRepeat);
        style.insert("minSize", slIter->minSize);
        style.insert("fixedSize", slIter->fixedSize);

        if (!slIter->g3Equiv.empty())
        {
            QJsonArray g3equiv;
            vector<int>::iterator eqIter;

            for (eqIter = slIter->g3Equiv.begin(); eqIter != slIter->g3Equiv.end(); ++eqIter)
                g3equiv.append(QJsonValue(*eqIter));

            style.insert("g3equiv", g3equiv);
        }

        sliderStyle.append(style);
    }

    sliders.insert("sliderStyles", sliderStyle);
    return sliders;
}

QJsonObject TGraphics::writeSystemEffectsJson()
{
    DECL_TRACER("TGraphics::writeSystemEffectsJson()");

    // Effect family
    QJsonObject effects;
    QJsonArray effectFamily;

    vector<FAMILY_t>::iterator iter;

    for (iter = mDraw.effects.begin(); iter != mDraw.effects.end(); ++iter)
    {
        QJsonObject effectData;
        effectData.insert("name", iter->name);
        QJsonArray family;

        vector<QString>::iterator efIter;

        for (efIter = iter->member.begin(); efIter != iter->member.end(); efIter++)
            family.append(QJsonValue(*efIter));

        effectData.insert("members", family);
        effectFamily.append(effectData);
    }

    effects.insert("effectFamilies", effectFamily);
    // Slider styles
    vector<EFFECT_STYLE_t>::iterator efIter;
    QJsonArray effectStyle;

    for (efIter = mDraw.effectStyles.begin(); efIter != mDraw.effectStyles.end(); ++efIter)
    {
        QJsonObject style;
        style.insert("name", efIter->name);
        style.insert("number", efIter->number);
        style.insert("startX", efIter->startx);
        style.insert("startY", efIter->starty);
        style.insert("height", efIter->height);
        style.insert("width", efIter->width);
        style.insert("cutout", efIter->cutout);
        style.insert("pixelMap", efIter->pixelMap);
        effectStyle.append(style);
    }

    effects.insert("effectStyles", effectStyle);
    return effects;
}

QJsonObject TGraphics::writeSystemPopupsJson()
{
    DECL_TRACER("TGraphics::writeSystemPopupsJson()");

    QJsonObject popups;
    QJsonArray popupEffect;

    vector<POPUP_EFFECT_t>::iterator iter;

    for (iter = mDraw.popupEffects.begin(); iter != mDraw.popupEffects.end(); ++iter)
    {
        QJsonObject popupData;
        popupData.insert("name", iter->name);
        popupData.insert("number", iter->number);
        popupData.insert("valueUsed", iter->valueUsed);
        popupEffect.append(popupData);
    }

    popups.insert("popups", popupEffect);
    return popups;
}
