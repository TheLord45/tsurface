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
#include <fstream>
#include <ios>

#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QImage>

#include "terror.h"
#include "tgraphics.h"

TGraphics *TGraphics::mCurrent{nullptr};

namespace fs = std::filesystem;
using namespace Graphics;
using std::string;
using std::vector;
using std::ofstream;

const vector<QString> borderResources = {
    "borders/AMXeliteL-off_b.png",
    "borders/AMXeliteL-off_bl.png",
    "borders/AMXeliteL-off_br.png",
    "borders/AMXeliteL-off_l.png",
    "borders/AMXeliteL-off_r.png",
    "borders/AMXeliteL-off_t.png",
    "borders/AMXeliteL-off_tl.png",
    "borders/AMXeliteL-off_tr.png",
    "borders/AMXeliteL-on_b.png",
    "borders/AMXeliteL-on_bl.png",
    "borders/AMXeliteL-on_br.png",
    "borders/AMXeliteL-on_l.png",
    "borders/AMXeliteL-on_r.png",
    "borders/AMXeliteL-on_t.png",
    "borders/AMXeliteL-on_tl.png",
    "borders/AMXeliteL-on_tr.png",
    "borders/AMXeliteM-off_b.png",
    "borders/AMXeliteM-off_bl.png",
    "borders/AMXeliteM-off_br.png",
    "borders/AMXeliteM-off_l.png",
    "borders/AMXeliteM-off_r.png",
    "borders/AMXeliteM-off_t.png",
    "borders/AMXeliteM-off_tl.png",
    "borders/AMXeliteM-off_tr.png",
    "borders/AMXeliteM-on_b.png",
    "borders/AMXeliteM-on_bl.png",
    "borders/AMXeliteM-on_br.png",
    "borders/AMXeliteM-on_l.png",
    "borders/AMXeliteM-on_r.png",
    "borders/AMXeliteM-on_t.png",
    "borders/AMXeliteM-on_tl.png",
    "borders/AMXeliteM-on_tr.png",
    "borders/AMXeliteS-off_b.png",
    "borders/AMXeliteS-off_bl.png",
    "borders/AMXeliteS-off_br.png",
    "borders/AMXeliteS-off_l.png",
    "borders/AMXeliteS-off_r.png",
    "borders/AMXeliteS-off_t.png",
    "borders/AMXeliteS-off_tl.png",
    "borders/AMXeliteS-off_tr.png",
    "borders/AMXeliteS-on_b.png",
    "borders/AMXeliteS-on_bl.png",
    "borders/AMXeliteS-on_br.png",
    "borders/AMXeliteS-on_l.png",
    "borders/AMXeliteS-on_r.png",
    "borders/AMXeliteS-on_t.png",
    "borders/AMXeliteS-on_tl.png",
    "borders/AMXeliteS-on_tr.png",
    "borders/aqua_b_alpha.png",
    "borders/aqua_b.png",
    "borders/aqua_bl_alpha.png",
    "borders/aqua_bl.png",
    "borders/aqua_br_alpha.png",
    "borders/aqua_br.png",
    "borders/aqua_l_alpha.png",
    "borders/aqua_l.png",
    "borders/aqua_r_alpha.png",
    "borders/aqua_r.png",
    "borders/aqua_t_alpha.png",
    "borders/aqua_t.png",
    "borders/aqua_tl_alpha.png",
    "borders/aqua_tl.png",
    "borders/aqua_tr_alpha.png",
    "borders/aqua_tr.png",
    "borders/aquaLarge_b_alpha.png",
    "borders/aquaLarge_b.png",
    "borders/aquaLarge_bl_alpha.png",
    "borders/aquaLarge_bl.png",
    "borders/aquaLarge_br_alpha.png",
    "borders/aquaLarge_br.png",
    "borders/aquaLarge_l_alpha.png",
    "borders/aquaLarge_l.png",
    "borders/aquaLarge_r_alpha.png",
    "borders/aquaLarge_r.png",
    "borders/aquaLarge_t_alpha.png",
    "borders/aquaLarge_t.png",
    "borders/aquaLarge_tl_alpha.png",
    "borders/aquaLarge_tl.png",
    "borders/aquaLarge_tr_alpha.png",
    "borders/aquaLarge_tr.png",
    "borders/aquaMed_b_alpha.png",
    "borders/aquaMed_b.png",
    "borders/aquaMed_bl_alpha.png",
    "borders/aquaMed_bl.png",
    "borders/aquaMed_br_alpha.png",
    "borders/aquaMed_br.png",
    "borders/aquaMed_l_alpha.png",
    "borders/aquaMed_l.png",
    "borders/aquaMed_r_alpha.png",
    "borders/aquaMed_r.png",
    "borders/aquaMed_t_alpha.png",
    "borders/aquaMed_t.png",
    "borders/aquaMed_tl_alpha.png",
    "borders/aquaMed_tl.png",
    "borders/aquaMed_tr_alpha.png",
    "borders/aquaMed_tr.png",
    "borders/aquaSmall_b_alpha.png",
    "borders/aquaSmall_b.png",
    "borders/aquaSmall_bl_alpha.png",
    "borders/aquaSmall_bl.png",
    "borders/aquaSmall_br_alpha.png",
    "borders/aquaSmall_br.png",
    "borders/aquaSmall_l_alpha.png",
    "borders/aquaSmall_l.png",
    "borders/aquaSmall_r_alpha.png",
    "borders/aquaSmall_r.png",
    "borders/aquaSmall_t_alpha.png",
    "borders/aquaSmall_t.png",
    "borders/aquaSmall_tl_alpha.png",
    "borders/aquaSmall_tl.png",
    "borders/aquaSmall_tr_alpha.png",
    "borders/aquaSmall_tr.png",
    "borders/Ball_b_alpha.png",
    "borders/Ball_b.png",
    "borders/Ball_bl_alpha.png",
    "borders/Ball_bl.png",
    "borders/Ball_br_alpha.png",
    "borders/Ball_br.png",
    "borders/Ball_l_alpha.png",
    "borders/Ball_l.png",
    "borders/Ball_r_alpha.png",
    "borders/Ball_r.png",
    "borders/Ball_t_alpha.png",
    "borders/Ball_t.png",
    "borders/Ball_tl_alpha.png",
    "borders/Ball_tl.png",
    "borders/Ball_tr_alpha.png",
    "borders/Ball_tr.png",
    "borders/bevel-off_b.png",
    "borders/bevel-off_bl.png",
    "borders/bevel-off_br.png",
    "borders/bevel-off_l.png",
    "borders/bevel-off_r.png",
    "borders/bevel-off_t.png",
    "borders/bevel-off_tl.png",
    "borders/bevel-off_tr.png",
    "borders/bevel-on_b.png",
    "borders/bevel-on_bl.png",
    "borders/bevel-on_br.png",
    "borders/bevel-on_l.png",
    "borders/bevel-on_r.png",
    "borders/bevel-on_t.png",
    "borders/bevel-on_tl.png",
    "borders/bevel-on_tr.png",
    "borders/bevelL-off_b.png",
    "borders/bevelL-off_bl.png",
    "borders/bevelL-off_br.png",
    "borders/bevelL-off_l.png",
    "borders/bevelL-off_r.png",
    "borders/bevelL-off_t.png",
    "borders/bevelL-off_tl.png",
    "borders/bevelL-off_tr.png",
    "borders/bevelL-on_b.png",
    "borders/bevelL-on_bl.png",
    "borders/bevelL-on_br.png",
    "borders/bevelL-on_l.png",
    "borders/bevelL-on_r.png",
    "borders/bevelL-on_t.png",
    "borders/bevelL-on_tl.png",
    "borders/bevelL-on_tr.png",
    "borders/bevelM-off_b.png",
    "borders/bevelM-off_bl.png",
    "borders/bevelM-off_br.png",
    "borders/bevelM-off_l.png",
    "borders/bevelM-off_r.png",
    "borders/bevelM-off_t.png",
    "borders/bevelM-off_tl.png",
    "borders/bevelM-off_tr.png",
    "borders/bevelM-on_b.png",
    "borders/bevelM-on_bl.png",
    "borders/bevelM-on_br.png",
    "borders/bevelM-on_l.png",
    "borders/bevelM-on_r.png",
    "borders/bevelM-on_t.png",
    "borders/bevelM-on_tl.png",
    "borders/bevelM-on_tr.png",
    "borders/bottom_cursor_b_alpha.png",
    "borders/bottom_cursor_bl_alpha.png",
    "borders/bottom_cursor_br_alpha.png",
    "borders/bottom_cursor_l_alpha.png",
    "borders/bottom_cursor_r_alpha.png",
    "borders/bottom_cursor_t_alpha.png",
    "borders/bottom_cursor_tl_alpha.png",
    "borders/bottom_cursor_tr_alpha.png",
    "borders/bottomMenuRounded15_b_alpha.png",
    "borders/bottomMenuRounded15_bl_alpha.png",
    "borders/bottomMenuRounded15_br_alpha.png",
    "borders/bottomMenuRounded15_l_alpha.png",
    "borders/bottomMenuRounded15_r_alpha.png",
    "borders/bottomMenuRounded15_t_alpha.png",
    "borders/bottomMenuRounded15_tl_alpha.png",
    "borders/bottomMenuRounded15_tr_alpha.png",
    "borders/bottomMenuRounded25_b_alpha.png",
    "borders/bottomMenuRounded25_bl_alpha.png",
    "borders/bottomMenuRounded25_br_alpha.png",
    "borders/bottomMenuRounded25_l_alpha.png",
    "borders/bottomMenuRounded25_r_alpha.png",
    "borders/bottomMenuRounded25_t_alpha.png",
    "borders/bottomMenuRounded25_tl_alpha.png",
    "borders/bottomMenuRounded25_tr_alpha.png",
    "borders/bottomMenuRounded35_b_alpha.png",
    "borders/bottomMenuRounded35_bl_alpha.png",
    "borders/bottomMenuRounded35_br_alpha.png",
    "borders/bottomMenuRounded35_l_alpha.png",
    "borders/bottomMenuRounded35_r_alpha.png",
    "borders/bottomMenuRounded35_t_alpha.png",
    "borders/bottomMenuRounded35_tl_alpha.png",
    "borders/bottomMenuRounded35_tr_alpha.png",
    "borders/bottomMenuRounded45_b_alpha.png",
    "borders/bottomMenuRounded45_bl_alpha.png",
    "borders/bottomMenuRounded45_br_alpha.png",
    "borders/bottomMenuRounded45_l_alpha.png",
    "borders/bottomMenuRounded45_r_alpha.png",
    "borders/bottomMenuRounded45_t_alpha.png",
    "borders/bottomMenuRounded45_tl_alpha.png",
    "borders/bottomMenuRounded45_tr_alpha.png",
    "borders/bottomMenuRounded55_b_alpha.png",
    "borders/bottomMenuRounded55_bl_alpha.png",
    "borders/bottomMenuRounded55_br_alpha.png",
    "borders/bottomMenuRounded55_l_alpha.png",
    "borders/bottomMenuRounded55_r_alpha.png",
    "borders/bottomMenuRounded55_t_alpha.png",
    "borders/bottomMenuRounded55_tl_alpha.png",
    "borders/bottomMenuRounded55_tr_alpha.png",
    "borders/bottomMenuRounded65_b_alpha.png",
    "borders/bottomMenuRounded65_bl_alpha.png",
    "borders/bottomMenuRounded65_br_alpha.png",
    "borders/bottomMenuRounded65_l_alpha.png",
    "borders/bottomMenuRounded65_r_alpha.png",
    "borders/bottomMenuRounded65_t_alpha.png",
    "borders/bottomMenuRounded65_tl_alpha.png",
    "borders/bottomMenuRounded65_tr_alpha.png",
    "borders/bottomMenuRounded75_b_alpha.png",
    "borders/bottomMenuRounded75_bl_alpha.png",
    "borders/bottomMenuRounded75_br_alpha.png",
    "borders/bottomMenuRounded75_l_alpha.png",
    "borders/bottomMenuRounded75_r_alpha.png",
    "borders/bottomMenuRounded75_t_alpha.png",
    "borders/bottomMenuRounded75_tl_alpha.png",
    "borders/bottomMenuRounded75_tr_alpha.png",
    "borders/bottomMenuRounded85_b_alpha.png",
    "borders/bottomMenuRounded85_bl_alpha.png",
    "borders/bottomMenuRounded85_br_alpha.png",
    "borders/bottomMenuRounded85_l_alpha.png",
    "borders/bottomMenuRounded85_r_alpha.png",
    "borders/bottomMenuRounded85_t_alpha.png",
    "borders/bottomMenuRounded85_tl_alpha.png",
    "borders/bottomMenuRounded85_tr_alpha.png",
    "borders/bottomMenuRounded95_b_alpha.png",
    "borders/bottomMenuRounded95_bl_alpha.png",
    "borders/bottomMenuRounded95_br_alpha.png",
    "borders/bottomMenuRounded95_l_alpha.png",
    "borders/bottomMenuRounded95_r_alpha.png",
    "borders/bottomMenuRounded95_t_alpha.png",
    "borders/bottomMenuRounded95_tl_alpha.png",
    "borders/bottomMenuRounded95_tr_alpha.png",
    "borders/bottomMenuRounded105_b_alpha.png",
    "borders/bottomMenuRounded105_bl_alpha.png",
    "borders/bottomMenuRounded105_br_alpha.png",
    "borders/bottomMenuRounded105_l_alpha.png",
    "borders/bottomMenuRounded105_r_alpha.png",
    "borders/bottomMenuRounded105_t_alpha.png",
    "borders/bottomMenuRounded105_tl_alpha.png",
    "borders/bottomMenuRounded105_tr_alpha.png",
    "borders/bottomMenuRounded115_b_alpha.png",
    "borders/bottomMenuRounded115_bl_alpha.png",
    "borders/bottomMenuRounded115_br_alpha.png",
    "borders/bottomMenuRounded115_l_alpha.png",
    "borders/bottomMenuRounded115_r_alpha.png",
    "borders/bottomMenuRounded115_t_alpha.png",
    "borders/bottomMenuRounded115_tl_alpha.png",
    "borders/bottomMenuRounded115_tr_alpha.png",
    "borders/bottomMenuRounded125_b_alpha.png",
    "borders/bottomMenuRounded125_bl_alpha.png",
    "borders/bottomMenuRounded125_br_alpha.png",
    "borders/bottomMenuRounded125_l_alpha.png",
    "borders/bottomMenuRounded125_r_alpha.png",
    "borders/bottomMenuRounded125_t_alpha.png",
    "borders/bottomMenuRounded125_tl_alpha.png",
    "borders/bottomMenuRounded125_tr_alpha.png",
    "borders/bottomMenuRounded135_b_alpha.png",
    "borders/bottomMenuRounded135_bl_alpha.png",
    "borders/bottomMenuRounded135_br_alpha.png",
    "borders/bottomMenuRounded135_l_alpha.png",
    "borders/bottomMenuRounded135_r_alpha.png",
    "borders/bottomMenuRounded135_t_alpha.png",
    "borders/bottomMenuRounded135_tl_alpha.png",
    "borders/bottomMenuRounded135_tr_alpha.png",
    "borders/bottomMenuRounded145_b_alpha.png",
    "borders/bottomMenuRounded145_bl_alpha.png",
    "borders/bottomMenuRounded145_br_alpha.png",
    "borders/bottomMenuRounded145_l_alpha.png",
    "borders/bottomMenuRounded145_r_alpha.png",
    "borders/bottomMenuRounded145_t_alpha.png",
    "borders/bottomMenuRounded145_tl_alpha.png",
    "borders/bottomMenuRounded145_tr_alpha.png",
    "borders/bottomMenuRounded155_b_alpha.png",
    "borders/bottomMenuRounded155_bl_alpha.png",
    "borders/bottomMenuRounded155_br_alpha.png",
    "borders/bottomMenuRounded155_l_alpha.png",
    "borders/bottomMenuRounded155_r_alpha.png",
    "borders/bottomMenuRounded155_t_alpha.png",
    "borders/bottomMenuRounded155_tl_alpha.png",
    "borders/bottomMenuRounded155_tr_alpha.png",
    "borders/bottomMenuRounded165_b_alpha.png",
    "borders/bottomMenuRounded165_bl_alpha.png",
    "borders/bottomMenuRounded165_br_alpha.png",
    "borders/bottomMenuRounded165_l_alpha.png",
    "borders/bottomMenuRounded165_r_alpha.png",
    "borders/bottomMenuRounded165_t_alpha.png",
    "borders/bottomMenuRounded165_tl_alpha.png",
    "borders/bottomMenuRounded165_tr_alpha.png",
    "borders/bottomMenuRounded175_b_alpha.png",
    "borders/bottomMenuRounded175_bl_alpha.png",
    "borders/bottomMenuRounded175_br_alpha.png",
    "borders/bottomMenuRounded175_l_alpha.png",
    "borders/bottomMenuRounded175_r_alpha.png",
    "borders/bottomMenuRounded175_t_alpha.png",
    "borders/bottomMenuRounded175_tl_alpha.png",
    "borders/bottomMenuRounded175_tr_alpha.png",
    "borders/bottomMenuRounded185_b_alpha.png",
    "borders/bottomMenuRounded185_bl_alpha.png",
    "borders/bottomMenuRounded185_br_alpha.png",
    "borders/bottomMenuRounded185_l_alpha.png",
    "borders/bottomMenuRounded185_r_alpha.png",
    "borders/bottomMenuRounded185_t_alpha.png",
    "borders/bottomMenuRounded185_tl_alpha.png",
    "borders/bottomMenuRounded185_tr_alpha.png",
    "borders/bottomMenuRounded195_b_alpha.png",
    "borders/bottomMenuRounded195_bl_alpha.png",
    "borders/bottomMenuRounded195_br_alpha.png",
    "borders/bottomMenuRounded195_l_alpha.png",
    "borders/bottomMenuRounded195_r_alpha.png",
    "borders/bottomMenuRounded195_t_alpha.png",
    "borders/bottomMenuRounded195_tl_alpha.png",
    "borders/bottomMenuRounded195_tr_alpha.png",
    "borders/BvlDblIM_b.png",
    "borders/BvlDblIM_bl.png",
    "borders/BvlDblIM_br.png",
    "borders/BvlDblIM_l.png",
    "borders/BvlDblIM_r.png",
    "borders/BvlDblIM_t.png",
    "borders/BvlDblIM_tl.png",
    "borders/BvlDblIM_tr.png",
    "borders/BvlDblIS_b.png",
    "borders/BvlDblIS_bl.png",
    "borders/BvlDblIS_br.png",
    "borders/BvlDblIS_l.png",
    "borders/BvlDblIS_r.png",
    "borders/BvlDblIS_t.png",
    "borders/BvlDblIS_tl.png",
    "borders/BvlDblIS_tr.png",
    "borders/BvlDblRM_b.png",
    "borders/BvlDblRM_bl.png",
    "borders/BvlDblRM_br.png",
    "borders/BvlDblRM_l.png",
    "borders/BvlDblRM_r.png",
    "borders/BvlDblRM_t.png",
    "borders/BvlDblRM_tl.png",
    "borders/BvlDblRM_tr.png",
    "borders/BvlIM_b.png",
    "borders/BvlIM_bl.png",
    "borders/BvlIM_br.png",
    "borders/BvlIM_l.png",
    "borders/BvlIM_r.png",
    "borders/BvlIM_t.png",
    "borders/BvlIM_tl.png",
    "borders/BvlIM_tr.png",
    "borders/BvlRM_b.png",
    "borders/BvlRM_bl.png",
    "borders/BvlRM_br.png",
    "borders/BvlRM_l.png",
    "borders/BvlRM_r.png",
    "borders/BvlRM_t.png",
    "borders/BvlRM_tl.png",
    "borders/BvlRM_tr.png",
    "borders/circle-down_b_alpha.png",
    "borders/circle-down_bl_alpha.png",
    "borders/circle-down_br_alpha.png",
    "borders/circle-down_l_alpha.png",
    "borders/circle-down_r_alpha.png",
    "borders/circle-down_t_alpha.png",
    "borders/circle-down_tl_alpha.png",
    "borders/circle-down_tr_alpha.png",
    "borders/circle-left_b_alpha.png",
    "borders/circle-left_bl_alpha.png",
    "borders/circle-left_br_alpha.png",
    "borders/circle-left_l_alpha.png",
    "borders/circle-left_r_alpha.png",
    "borders/circle-left_t_alpha.png",
    "borders/circle-left_tl_alpha.png",
    "borders/circle-left_tr_alpha.png",
    "borders/circle-right_b_alpha.png",
    "borders/circle-right_bl_alpha.png",
    "borders/circle-right_br_alpha.png",
    "borders/circle-right_l_alpha.png",
    "borders/circle-right_r_alpha.png",
    "borders/circle-right_t_alpha.png",
    "borders/circle-right_tl_alpha.png",
    "borders/circle-right_tr_alpha.png",
    "borders/circle-up_b_alpha.png",
    "borders/circle-up_bl_alpha.png",
    "borders/circle-up_br_alpha.png",
    "borders/circle-up_l_alpha.png",
    "borders/circle-up_r_alpha.png",
    "borders/circle-up_t_alpha.png",
    "borders/circle-up_tl_alpha.png",
    "borders/circle-up_tr_alpha.png",
    "borders/circle15_b_alpha.png",
    "borders/circle15_bl_alpha.png",
    "borders/circle15_br_alpha.png",
    "borders/circle15_l_alpha.png",
    "borders/circle15_r_alpha.png",
    "borders/circle15_t_alpha.png",
    "borders/circle15_tl_alpha.png",
    "borders/circle15_tr_alpha.png",
    "borders/circle25_b_alpha.png",
    "borders/circle25_bl_alpha.png",
    "borders/circle25_br_alpha.png",
    "borders/circle25_l_alpha.png",
    "borders/circle25_r_alpha.png",
    "borders/circle25_t_alpha.png",
    "borders/circle25_tl_alpha.png",
    "borders/circle25_tr_alpha.png",
    "borders/circle35_b_alpha.png",
    "borders/circle35_bl_alpha.png",
    "borders/circle35_br_alpha.png",
    "borders/circle35_l_alpha.png",
    "borders/circle35_r_alpha.png",
    "borders/circle35_t_alpha.png",
    "borders/circle35_tl_alpha.png",
    "borders/circle35_tr_alpha.png",
    "borders/circle45_b_alpha.png",
    "borders/circle45_bl_alpha.png",
    "borders/circle45_br_alpha.png",
    "borders/circle45_l_alpha.png",
    "borders/circle45_r_alpha.png",
    "borders/circle45_t_alpha.png",
    "borders/circle45_tl_alpha.png",
    "borders/circle45_tr_alpha.png",
    "borders/circle55_b_alpha.png",
    "borders/circle55_bl_alpha.png",
    "borders/circle55_br_alpha.png",
    "borders/circle55_l_alpha.png",
    "borders/circle55_r_alpha.png",
    "borders/circle55_t_alpha.png",
    "borders/circle55_tl_alpha.png",
    "borders/circle55_tr_alpha.png",
    "borders/circle65_b_alpha.png",
    "borders/circle65_bl_alpha.png",
    "borders/circle65_br_alpha.png",
    "borders/circle65_l_alpha.png",
    "borders/circle65_r_alpha.png",
    "borders/circle65_t_alpha.png",
    "borders/circle65_tl_alpha.png",
    "borders/circle65_tr_alpha.png",
    "borders/circle75_b_alpha.png",
    "borders/circle75_bl_alpha.png",
    "borders/circle75_br_alpha.png",
    "borders/circle75_l_alpha.png",
    "borders/circle75_r_alpha.png",
    "borders/circle75_t_alpha.png",
    "borders/circle75_tl_alpha.png",
    "borders/circle75_tr_alpha.png",
    "borders/circle85_b_alpha.png",
    "borders/circle85_bl_alpha.png",
    "borders/circle85_br_alpha.png",
    "borders/circle85_l_alpha.png",
    "borders/circle85_r_alpha.png",
    "borders/circle85_t_alpha.png",
    "borders/circle85_tl_alpha.png",
    "borders/circle85_tr_alpha.png",
    "borders/circle95_b_alpha.png",
    "borders/circle95_bl_alpha.png",
    "borders/circle95_br_alpha.png",
    "borders/circle95_l_alpha.png",
    "borders/circle95_r_alpha.png",
    "borders/circle95_t_alpha.png",
    "borders/circle95_tl_alpha.png",
    "borders/circle95_tr_alpha.png",
    "borders/circle105_b_alpha.png",
    "borders/circle105_bl_alpha.png",
    "borders/circle105_br_alpha.png",
    "borders/circle105_l_alpha.png",
    "borders/circle105_r_alpha.png",
    "borders/circle105_t_alpha.png",
    "borders/circle105_tl_alpha.png",
    "borders/circle105_tr_alpha.png",
    "borders/circle115_b_alpha.png",
    "borders/circle115_bl_alpha.png",
    "borders/circle115_br_alpha.png",
    "borders/circle115_l_alpha.png",
    "borders/circle115_r_alpha.png",
    "borders/circle115_t_alpha.png",
    "borders/circle115_tl_alpha.png",
    "borders/circle115_tr_alpha.png",
    "borders/circle125_b_alpha.png",
    "borders/circle125_bl_alpha.png",
    "borders/circle125_br_alpha.png",
    "borders/circle125_l_alpha.png",
    "borders/circle125_r_alpha.png",
    "borders/circle125_t_alpha.png",
    "borders/circle125_tl_alpha.png",
    "borders/circle125_tr_alpha.png",
    "borders/circle135_b_alpha.png",
    "borders/circle135_bl_alpha.png",
    "borders/circle135_br_alpha.png",
    "borders/circle135_l_alpha.png",
    "borders/circle135_r_alpha.png",
    "borders/circle135_t_alpha.png",
    "borders/circle135_tl_alpha.png",
    "borders/circle135_tr_alpha.png",
    "borders/circle145_b_alpha.png",
    "borders/circle145_bl_alpha.png",
    "borders/circle145_br_alpha.png",
    "borders/circle145_l_alpha.png",
    "borders/circle145_r_alpha.png",
    "borders/circle145_t_alpha.png",
    "borders/circle145_tl_alpha.png",
    "borders/circle145_tr_alpha.png",
    "borders/circle155_b_alpha.png",
    "borders/circle155_bl_alpha.png",
    "borders/circle155_br_alpha.png",
    "borders/circle155_l_alpha.png",
    "borders/circle155_r_alpha.png",
    "borders/circle155_t_alpha.png",
    "borders/circle155_tl_alpha.png",
    "borders/circle155_tr_alpha.png",
    "borders/circle165_b_alpha.png",
    "borders/circle165_bl_alpha.png",
    "borders/circle165_br_alpha.png",
    "borders/circle165_l_alpha.png",
    "borders/circle165_r_alpha.png",
    "borders/circle165_t_alpha.png",
    "borders/circle165_tl_alpha.png",
    "borders/circle165_tr_alpha.png",
    "borders/circle175_b_alpha.png",
    "borders/circle175_bl_alpha.png",
    "borders/circle175_br_alpha.png",
    "borders/circle175_l_alpha.png",
    "borders/circle175_r_alpha.png",
    "borders/circle175_t_alpha.png",
    "borders/circle175_tl_alpha.png",
    "borders/circle175_tr_alpha.png",
    "borders/circle185_b_alpha.png",
    "borders/circle185_bl_alpha.png",
    "borders/circle185_br_alpha.png",
    "borders/circle185_l_alpha.png",
    "borders/circle185_r_alpha.png",
    "borders/circle185_t_alpha.png",
    "borders/circle185_tl_alpha.png",
    "borders/circle185_tr_alpha.png",
    "borders/circle195_b_alpha.png",
    "borders/circle195_bl_alpha.png",
    "borders/circle195_br_alpha.png",
    "borders/circle195_l_alpha.png",
    "borders/circle195_r_alpha.png",
    "borders/circle195_t_alpha.png",
    "borders/circle195_tl_alpha.png",
    "borders/circle195_tr_alpha.png",
    "borders/CrclBevL_b_alpha.png",
    "borders/CrclBevL_b.png",
    "borders/CrclBevL_bl_alpha.png",
    "borders/CrclBevL_bl.png",
    "borders/CrclBevL_br_alpha.png",
    "borders/CrclBevL_br.png",
    "borders/CrclBevL_l_alpha.png",
    "borders/CrclBevL_l.png",
    "borders/CrclBevL_r_alpha.png",
    "borders/CrclBevL_r.png",
    "borders/CrclBevL_t_alpha.png",
    "borders/CrclBevL_t.png",
    "borders/CrclBevL_tl_alpha.png",
    "borders/CrclBevL_tl.png",
    "borders/CrclBevL_tr_alpha.png",
    "borders/CrclBevL_tr.png",
    "borders/cursorHoleDown_b_alpha.png",
    "borders/cursorHoleDown_bl_alpha.png",
    "borders/cursorHoleDown_br_alpha.png",
    "borders/cursorHoleDown_l_alpha.png",
    "borders/cursorHoleDown_r_alpha.png",
    "borders/cursorHoleDown_t_alpha.png",
    "borders/cursorHoleDown_tl_alpha.png",
    "borders/cursorHoleDown_tr_alpha.png",
    "borders/cursorHoleLeft_b_alpha.png",
    "borders/cursorHoleLeft_bl_alpha.png",
    "borders/cursorHoleLeft_br_alpha.png",
    "borders/cursorHoleLeft_l_alpha.png",
    "borders/cursorHoleLeft_r_alpha.png",
    "borders/cursorHoleLeft_t_alpha.png",
    "borders/cursorHoleLeft_tl_alpha.png",
    "borders/cursorHoleLeft_tr_alpha.png",
    "borders/cursorHoleRight_b_alpha.png",
    "borders/cursorHoleRight_bl_alpha.png",
    "borders/cursorHoleRight_br_alpha.png",
    "borders/cursorHoleRight_l_alpha.png",
    "borders/cursorHoleRight_r_alpha.png",
    "borders/cursorHoleRight_t_alpha.png",
    "borders/cursorHoleRight_tl_alpha.png",
    "borders/cursorHoleRight_tr_alpha.png",
    "borders/cursorHoleUp_b_alpha.png",
    "borders/cursorHoleUp_bl_alpha.png",
    "borders/cursorHoleUp_br_alpha.png",
    "borders/cursorHoleUp_l_alpha.png",
    "borders/cursorHoleUp_r_alpha.png",
    "borders/cursorHoleUp_t_alpha.png",
    "borders/cursorHoleUp_tl_alpha.png",
    "borders/cursorHoleUp_tr_alpha.png",
    "borders/CustomFrame_b.png",
    "borders/CustomFrame_bl.png",
    "borders/CustomFrame_br.png",
    "borders/CustomFrame_l.png",
    "borders/CustomFrame_r.png",
    "borders/CustomFrame_t.png",
    "borders/CustomFrame_tl.png",
    "borders/CustomFrame_tr.png",
    "borders/dbevelL-off_b.png",
    "borders/dbevelL-off_bl.png",
    "borders/dbevelL-off_br.png",
    "borders/dbevelL-off_l.png",
    "borders/dbevelL-off_r.png",
    "borders/dbevelL-off_t.png",
    "borders/dbevelL-off_tl.png",
    "borders/dbevelL-off_tr.png",
    "borders/dbevelL-on_b.png",
    "borders/dbevelL-on_bl.png",
    "borders/dbevelL-on_br.png",
    "borders/dbevelL-on_l.png",
    "borders/dbevelL-on_r.png",
    "borders/dbevelL-on_t.png",
    "borders/dbevelL-on_tl.png",
    "borders/dbevelL-on_tr.png",
    "borders/dbevelM-off_b.png",
    "borders/dbevelM-off_bl.png",
    "borders/dbevelM-off_br.png",
    "borders/dbevelM-off_l.png",
    "borders/dbevelM-off_r.png",
    "borders/dbevelM-off_t.png",
    "borders/dbevelM-off_tl.png",
    "borders/dbevelM-off_tr.png",
    "borders/dbevelM-on_b.png",
    "borders/dbevelM-on_bl.png",
    "borders/dbevelM-on_br.png",
    "borders/dbevelM-on_l.png",
    "borders/dbevelM-on_r.png",
    "borders/dbevelM-on_t.png",
    "borders/dbevelM-on_tl.png",
    "borders/dbevelM-on_tr.png",
    "borders/dbevelS-off_b.png",
    "borders/dbevelS-off_bl.png",
    "borders/dbevelS-off_br.png",
    "borders/dbevelS-off_l.png",
    "borders/dbevelS-off_r.png",
    "borders/dbevelS-off_t.png",
    "borders/dbevelS-off_tl.png",
    "borders/dbevelS-off_tr.png",
    "borders/dbevelS-on_b.png",
    "borders/dbevelS-on_bl.png",
    "borders/dbevelS-on_br.png",
    "borders/dbevelS-on_l.png",
    "borders/dbevelS-on_r.png",
    "borders/dbevelS-on_t.png",
    "borders/dbevelS-on_tl.png",
    "borders/dbevelS-on_tr.png",
    "borders/diamond15_b_alpha.png",
    "borders/diamond15_bl_alpha.png",
    "borders/diamond15_br_alpha.png",
    "borders/diamond15_l_alpha.png",
    "borders/diamond15_r_alpha.png",
    "borders/diamond15_t_alpha.png",
    "borders/diamond15_tl_alpha.png",
    "borders/diamond15_tr_alpha.png",
    "borders/diamond25_b_alpha.png",
    "borders/diamond25_bl_alpha.png",
    "borders/diamond25_br_alpha.png",
    "borders/diamond25_l_alpha.png",
    "borders/diamond25_r_alpha.png",
    "borders/diamond25_t_alpha.png",
    "borders/diamond25_tl_alpha.png",
    "borders/diamond25_tr_alpha.png",
    "borders/diamond35_b_alpha.png",
    "borders/diamond35_bl_alpha.png",
    "borders/diamond35_br_alpha.png",
    "borders/diamond35_l_alpha.png",
    "borders/diamond35_r_alpha.png",
    "borders/diamond35_t_alpha.png",
    "borders/diamond35_tl_alpha.png",
    "borders/diamond35_tr_alpha.png",
    "borders/diamond45_b_alpha.png",
    "borders/diamond45_bl_alpha.png",
    "borders/diamond45_br_alpha.png",
    "borders/diamond45_l_alpha.png",
    "borders/diamond45_r_alpha.png",
    "borders/diamond45_t_alpha.png",
    "borders/diamond45_tl_alpha.png",
    "borders/diamond45_tr_alpha.png",
    "borders/diamond55_b_alpha.png",
    "borders/diamond55_bl_alpha.png",
    "borders/diamond55_br_alpha.png",
    "borders/diamond55_l_alpha.png",
    "borders/diamond55_r_alpha.png",
    "borders/diamond55_t_alpha.png",
    "borders/diamond55_tl_alpha.png",
    "borders/diamond55_tr_alpha.png",
    "borders/diamond65_b_alpha.png",
    "borders/diamond65_bl_alpha.png",
    "borders/diamond65_br_alpha.png",
    "borders/diamond65_l_alpha.png",
    "borders/diamond65_r_alpha.png",
    "borders/diamond65_t_alpha.png",
    "borders/diamond65_tl_alpha.png",
    "borders/diamond65_tr_alpha.png",
    "borders/diamond75_b_alpha.png",
    "borders/diamond75_bl_alpha.png",
    "borders/diamond75_br_alpha.png",
    "borders/diamond75_l_alpha.png",
    "borders/diamond75_r_alpha.png",
    "borders/diamond75_t_alpha.png",
    "borders/diamond75_tl_alpha.png",
    "borders/diamond75_tr_alpha.png",
    "borders/diamond85_b_alpha.png",
    "borders/diamond85_bl_alpha.png",
    "borders/diamond85_br_alpha.png",
    "borders/diamond85_l_alpha.png",
    "borders/diamond85_r_alpha.png",
    "borders/diamond85_t_alpha.png",
    "borders/diamond85_tl_alpha.png",
    "borders/diamond85_tr_alpha.png",
    "borders/diamond95_b_alpha.png",
    "borders/diamond95_bl_alpha.png",
    "borders/diamond95_br_alpha.png",
    "borders/diamond95_l_alpha.png",
    "borders/diamond95_r_alpha.png",
    "borders/diamond95_t_alpha.png",
    "borders/diamond95_tl_alpha.png",
    "borders/diamond95_tr_alpha.png",
    "borders/diamond105_b_alpha.png",
    "borders/diamond105_bl_alpha.png",
    "borders/diamond105_br_alpha.png",
    "borders/diamond105_l_alpha.png",
    "borders/diamond105_r_alpha.png",
    "borders/diamond105_t_alpha.png",
    "borders/diamond105_tl_alpha.png",
    "borders/diamond105_tr_alpha.png",
    "borders/diamond115_b_alpha.png",
    "borders/diamond115_bl_alpha.png",
    "borders/diamond115_br_alpha.png",
    "borders/diamond115_l_alpha.png",
    "borders/diamond115_r_alpha.png",
    "borders/diamond115_t_alpha.png",
    "borders/diamond115_tl_alpha.png",
    "borders/diamond115_tr_alpha.png",
    "borders/diamond125_b_alpha.png",
    "borders/diamond125_bl_alpha.png",
    "borders/diamond125_br_alpha.png",
    "borders/diamond125_l_alpha.png",
    "borders/diamond125_r_alpha.png",
    "borders/diamond125_t_alpha.png",
    "borders/diamond125_tl_alpha.png",
    "borders/diamond125_tr_alpha.png",
    "borders/diamond135_b_alpha.png",
    "borders/diamond135_bl_alpha.png",
    "borders/diamond135_br_alpha.png",
    "borders/diamond135_l_alpha.png",
    "borders/diamond135_r_alpha.png",
    "borders/diamond135_t_alpha.png",
    "borders/diamond135_tl_alpha.png",
    "borders/diamond135_tr_alpha.png",
    "borders/diamond145_b_alpha.png",
    "borders/diamond145_bl_alpha.png",
    "borders/diamond145_br_alpha.png",
    "borders/diamond145_l_alpha.png",
    "borders/diamond145_r_alpha.png",
    "borders/diamond145_t_alpha.png",
    "borders/diamond145_tl_alpha.png",
    "borders/diamond145_tr_alpha.png",
    "borders/diamond155_b_alpha.png",
    "borders/diamond155_bl_alpha.png",
    "borders/diamond155_br_alpha.png",
    "borders/diamond155_l_alpha.png",
    "borders/diamond155_r_alpha.png",
    "borders/diamond155_t_alpha.png",
    "borders/diamond155_tl_alpha.png",
    "borders/diamond155_tr_alpha.png",
    "borders/diamond165_b_alpha.png",
    "borders/diamond165_bl_alpha.png",
    "borders/diamond165_br_alpha.png",
    "borders/diamond165_l_alpha.png",
    "borders/diamond165_r_alpha.png",
    "borders/diamond165_t_alpha.png",
    "borders/diamond165_tl_alpha.png",
    "borders/diamond165_tr_alpha.png",
    "borders/diamond175_b_alpha.png",
    "borders/diamond175_bl_alpha.png",
    "borders/diamond175_br_alpha.png",
    "borders/diamond175_l_alpha.png",
    "borders/diamond175_r_alpha.png",
    "borders/diamond175_t_alpha.png",
    "borders/diamond175_tl_alpha.png",
    "borders/diamond175_tr_alpha.png",
    "borders/diamond185_b_alpha.png",
    "borders/diamond185_bl_alpha.png",
    "borders/diamond185_br_alpha.png",
    "borders/diamond185_l_alpha.png",
    "borders/diamond185_r_alpha.png",
    "borders/diamond185_t_alpha.png",
    "borders/diamond185_tl_alpha.png",
    "borders/diamond185_tr_alpha.png",
    "borders/diamond195_b_alpha.png",
    "borders/diamond195_bl_alpha.png",
    "borders/diamond195_br_alpha.png",
    "borders/diamond195_l_alpha.png",
    "borders/diamond195_r_alpha.png",
    "borders/diamond195_t_alpha.png",
    "borders/diamond195_tl_alpha.png",
    "borders/diamond195_tr_alpha.png",
    "borders/down_b_alpha.png",
    "borders/down_bl_alpha.png",
    "borders/down_br_alpha.png",
    "borders/down_l_alpha.png",
    "borders/down_r_alpha.png",
    "borders/down_t_alpha.png",
    "borders/down_tl_alpha.png",
    "borders/down_tr_alpha.png",
    "borders/fuzzyBorder_b_alpha.png",
    "borders/fuzzyBorder_bl_alpha.png",
    "borders/fuzzyBorder_br_alpha.png",
    "borders/fuzzyBorder_l_alpha.png",
    "borders/fuzzyBorder_r_alpha.png",
    "borders/fuzzyBorder_t_alpha.png",
    "borders/fuzzyBorder_tl_alpha.png",
    "borders/fuzzyBorder_tr_alpha.png",
    "borders/Glow25_b_alpha.png",
    "borders/Glow25_bl_alpha.png",
    "borders/Glow25_br_alpha.png",
    "borders/Glow25_l_alpha.png",
    "borders/Glow25_r_alpha.png",
    "borders/Glow25_t_alpha.png",
    "borders/Glow25_tl_alpha.png",
    "borders/Glow25_tr_alpha.png",
    "borders/Glow50_b_alpha.png",
    "borders/Glow50_bl_alpha.png",
    "borders/Glow50_br_alpha.png",
    "borders/Glow50_l_alpha.png",
    "borders/Glow50_r_alpha.png",
    "borders/Glow50_t_alpha.png",
    "borders/Glow50_tl_alpha.png",
    "borders/Glow50_tr_alpha.png",
    "borders/HelpDown_b_alpha.png",
    "borders/HelpDown_bl_alpha.png",
    "borders/HelpDown_br_alpha.png",
    "borders/HelpDown_l_alpha.png",
    "borders/HelpDown_r_alpha.png",
    "borders/HelpDown_t_alpha.png",
    "borders/HelpDown_tl_alpha.png",
    "borders/HelpDown_tr_alpha.png",
    "borders/HelpDown2_b_alpha.png",
    "borders/HelpDown2_bl_alpha.png",
    "borders/HelpDown2_br_alpha.png",
    "borders/HelpDown2_l_alpha.png",
    "borders/HelpDown2_r_alpha.png",
    "borders/HelpDown2_t_alpha.png",
    "borders/HelpDown2_tl_alpha.png",
    "borders/HelpDown2_tr_alpha.png",
    "borders/HOval60x30_b_alpha.png",
    "borders/HOval60x30_bl_alpha.png",
    "borders/HOval60x30_br_alpha.png",
    "borders/HOval60x30_l_alpha.png",
    "borders/HOval60x30_r_alpha.png",
    "borders/HOval60x30_t_alpha.png",
    "borders/HOval60x30_tl_alpha.png",
    "borders/HOval60x30_tr_alpha.png",
    "borders/HOval100x50_b_alpha.png",
    "borders/HOval100x50_bl_alpha.png",
    "borders/HOval100x50_br_alpha.png",
    "borders/HOval100x50_l_alpha.png",
    "borders/HOval100x50_r_alpha.png",
    "borders/HOval100x50_t_alpha.png",
    "borders/HOval100x50_tl_alpha.png",
    "borders/HOval100x50_tr_alpha.png",
    "borders/HOval150x75_b_alpha.png",
    "borders/HOval150x75_bl_alpha.png",
    "borders/HOval150x75_br_alpha.png",
    "borders/HOval150x75_l_alpha.png",
    "borders/HOval150x75_r_alpha.png",
    "borders/HOval150x75_t_alpha.png",
    "borders/HOval150x75_tl_alpha.png",
    "borders/HOval150x75_tr_alpha.png",
    "borders/HOval200x100_b_alpha.png",
    "borders/HOval200x100_bl_alpha.png",
    "borders/HOval200x100_br_alpha.png",
    "borders/HOval200x100_l_alpha.png",
    "borders/HOval200x100_r_alpha.png",
    "borders/HOval200x100_t_alpha.png",
    "borders/HOval200x100_tl_alpha.png",
    "borders/HOval200x100_tr_alpha.png",
    "borders/left_b_alpha.png",
    "borders/left_bl_alpha.png",
    "borders/left_br_alpha.png",
    "borders/left_cursor_b_alpha.png",
    "borders/left_cursor_bl_alpha.png",
    "borders/left_cursor_br_alpha.png",
    "borders/left_cursor_l_alpha.png",
    "borders/left_cursor_r_alpha.png",
    "borders/left_cursor_t_alpha.png",
    "borders/left_cursor_tl_alpha.png",
    "borders/left_cursor_tr_alpha.png",
    "borders/left_l_alpha.png",
    "borders/left_r_alpha.png",
    "borders/left_t_alpha.png",
    "borders/left_tl_alpha.png",
    "borders/left_tr_alpha.png",
    "borders/leftMenuRounded15_b_alpha.png",
    "borders/leftMenuRounded15_bl_alpha.png",
    "borders/leftMenuRounded15_br_alpha.png",
    "borders/leftMenuRounded15_l_alpha.png",
    "borders/leftMenuRounded15_r_alpha.png",
    "borders/leftMenuRounded15_t_alpha.png",
    "borders/leftMenuRounded15_tl_alpha.png",
    "borders/leftMenuRounded15_tr_alpha.png",
    "borders/leftMenuRounded25_b_alpha.png",
    "borders/leftMenuRounded25_bl_alpha.png",
    "borders/leftMenuRounded25_br_alpha.png",
    "borders/leftMenuRounded25_l_alpha.png",
    "borders/leftMenuRounded25_r_alpha.png",
    "borders/leftMenuRounded25_t_alpha.png",
    "borders/leftMenuRounded25_tl_alpha.png",
    "borders/leftMenuRounded25_tr_alpha.png",
    "borders/leftMenuRounded35_b_alpha.png",
    "borders/leftMenuRounded35_bl_alpha.png",
    "borders/leftMenuRounded35_br_alpha.png",
    "borders/leftMenuRounded35_l_alpha.png",
    "borders/leftMenuRounded35_r_alpha.png",
    "borders/leftMenuRounded35_t_alpha.png",
    "borders/leftMenuRounded35_tl_alpha.png",
    "borders/leftMenuRounded35_tr_alpha.png",
    "borders/leftMenuRounded45_b_alpha.png",
    "borders/leftMenuRounded45_bl_alpha.png",
    "borders/leftMenuRounded45_br_alpha.png",
    "borders/leftMenuRounded45_l_alpha.png",
    "borders/leftMenuRounded45_r_alpha.png",
    "borders/leftMenuRounded45_t_alpha.png",
    "borders/leftMenuRounded45_tl_alpha.png",
    "borders/leftMenuRounded45_tr_alpha.png",
    "borders/leftMenuRounded55_b_alpha.png",
    "borders/leftMenuRounded55_bl_alpha.png",
    "borders/leftMenuRounded55_br_alpha.png",
    "borders/leftMenuRounded55_l_alpha.png",
    "borders/leftMenuRounded55_r_alpha.png",
    "borders/leftMenuRounded55_t_alpha.png",
    "borders/leftMenuRounded55_tl_alpha.png",
    "borders/leftMenuRounded55_tr_alpha.png",
    "borders/leftMenuRounded65_b_alpha.png",
    "borders/leftMenuRounded65_bl_alpha.png",
    "borders/leftMenuRounded65_br_alpha.png",
    "borders/leftMenuRounded65_l_alpha.png",
    "borders/leftMenuRounded65_r_alpha.png",
    "borders/leftMenuRounded65_t_alpha.png",
    "borders/leftMenuRounded65_tl_alpha.png",
    "borders/leftMenuRounded65_tr_alpha.png",
    "borders/leftMenuRounded75_b_alpha.png",
    "borders/leftMenuRounded75_bl_alpha.png",
    "borders/leftMenuRounded75_br_alpha.png",
    "borders/leftMenuRounded75_l_alpha.png",
    "borders/leftMenuRounded75_r_alpha.png",
    "borders/leftMenuRounded75_t_alpha.png",
    "borders/leftMenuRounded75_tl_alpha.png",
    "borders/leftMenuRounded75_tr_alpha.png",
    "borders/leftMenuRounded85_b_alpha.png",
    "borders/leftMenuRounded85_bl_alpha.png",
    "borders/leftMenuRounded85_br_alpha.png",
    "borders/leftMenuRounded85_l_alpha.png",
    "borders/leftMenuRounded85_r_alpha.png",
    "borders/leftMenuRounded85_t_alpha.png",
    "borders/leftMenuRounded85_tl_alpha.png",
    "borders/leftMenuRounded85_tr_alpha.png",
    "borders/leftMenuRounded95_b_alpha.png",
    "borders/leftMenuRounded95_bl_alpha.png",
    "borders/leftMenuRounded95_br_alpha.png",
    "borders/leftMenuRounded95_l_alpha.png",
    "borders/leftMenuRounded95_r_alpha.png",
    "borders/leftMenuRounded95_t_alpha.png",
    "borders/leftMenuRounded95_tl_alpha.png",
    "borders/leftMenuRounded95_tr_alpha.png",
    "borders/leftMenuRounded105_b_alpha.png",
    "borders/leftMenuRounded105_bl_alpha.png",
    "borders/leftMenuRounded105_br_alpha.png",
    "borders/leftMenuRounded105_l_alpha.png",
    "borders/leftMenuRounded105_r_alpha.png",
    "borders/leftMenuRounded105_t_alpha.png",
    "borders/leftMenuRounded105_tl_alpha.png",
    "borders/leftMenuRounded105_tr_alpha.png",
    "borders/leftMenuRounded115_b_alpha.png",
    "borders/leftMenuRounded115_bl_alpha.png",
    "borders/leftMenuRounded115_br_alpha.png",
    "borders/leftMenuRounded115_l_alpha.png",
    "borders/leftMenuRounded115_r_alpha.png",
    "borders/leftMenuRounded115_t_alpha.png",
    "borders/leftMenuRounded115_tl_alpha.png",
    "borders/leftMenuRounded115_tr_alpha.png",
    "borders/leftMenuRounded125_b_alpha.png",
    "borders/leftMenuRounded125_bl_alpha.png",
    "borders/leftMenuRounded125_br_alpha.png",
    "borders/leftMenuRounded125_l_alpha.png",
    "borders/leftMenuRounded125_r_alpha.png",
    "borders/leftMenuRounded125_t_alpha.png",
    "borders/leftMenuRounded125_tl_alpha.png",
    "borders/leftMenuRounded125_tr_alpha.png",
    "borders/leftMenuRounded135_b_alpha.png",
    "borders/leftMenuRounded135_bl_alpha.png",
    "borders/leftMenuRounded135_br_alpha.png",
    "borders/leftMenuRounded135_l_alpha.png",
    "borders/leftMenuRounded135_r_alpha.png",
    "borders/leftMenuRounded135_t_alpha.png",
    "borders/leftMenuRounded135_tl_alpha.png",
    "borders/leftMenuRounded135_tr_alpha.png",
    "borders/leftMenuRounded145_b_alpha.png",
    "borders/leftMenuRounded145_bl_alpha.png",
    "borders/leftMenuRounded145_br_alpha.png",
    "borders/leftMenuRounded145_l_alpha.png",
    "borders/leftMenuRounded145_r_alpha.png",
    "borders/leftMenuRounded145_t_alpha.png",
    "borders/leftMenuRounded145_tl_alpha.png",
    "borders/leftMenuRounded145_tr_alpha.png",
    "borders/leftMenuRounded155_b_alpha.png",
    "borders/leftMenuRounded155_bl_alpha.png",
    "borders/leftMenuRounded155_br_alpha.png",
    "borders/leftMenuRounded155_l_alpha.png",
    "borders/leftMenuRounded155_r_alpha.png",
    "borders/leftMenuRounded155_t_alpha.png",
    "borders/leftMenuRounded155_tl_alpha.png",
    "borders/leftMenuRounded155_tr_alpha.png",
    "borders/leftMenuRounded165_b_alpha.png",
    "borders/leftMenuRounded165_bl_alpha.png",
    "borders/leftMenuRounded165_br_alpha.png",
    "borders/leftMenuRounded165_l_alpha.png",
    "borders/leftMenuRounded165_r_alpha.png",
    "borders/leftMenuRounded165_t_alpha.png",
    "borders/leftMenuRounded165_tl_alpha.png",
    "borders/leftMenuRounded165_tr_alpha.png",
    "borders/leftMenuRounded175_b_alpha.png",
    "borders/leftMenuRounded175_bl_alpha.png",
    "borders/leftMenuRounded175_br_alpha.png",
    "borders/leftMenuRounded175_l_alpha.png",
    "borders/leftMenuRounded175_r_alpha.png",
    "borders/leftMenuRounded175_t_alpha.png",
    "borders/leftMenuRounded175_tl_alpha.png",
    "borders/leftMenuRounded175_tr_alpha.png",
    "borders/leftMenuRounded185_b_alpha.png",
    "borders/leftMenuRounded185_bl_alpha.png",
    "borders/leftMenuRounded185_br_alpha.png",
    "borders/leftMenuRounded185_l_alpha.png",
    "borders/leftMenuRounded185_r_alpha.png",
    "borders/leftMenuRounded185_t_alpha.png",
    "borders/leftMenuRounded185_tl_alpha.png",
    "borders/leftMenuRounded185_tr_alpha.png",
    "borders/leftMenuRounded195_b_alpha.png",
    "borders/leftMenuRounded195_bl_alpha.png",
    "borders/leftMenuRounded195_br_alpha.png",
    "borders/leftMenuRounded195_l_alpha.png",
    "borders/leftMenuRounded195_r_alpha.png",
    "borders/leftMenuRounded195_t_alpha.png",
    "borders/leftMenuRounded195_tl_alpha.png",
    "borders/leftMenuRounded195_tr_alpha.png",
    "borders/Line1_b.png",
    "borders/Line1_bl.png",
    "borders/Line1_br.png",
    "borders/Line1_l.png",
    "borders/Line1_r.png",
    "borders/Line1_t.png",
    "borders/Line1_tl.png",
    "borders/Line1_tr.png",
    "borders/Line2_b.png",
    "borders/Line2_bl.png",
    "borders/Line2_br.png",
    "borders/Line2_l.png",
    "borders/Line2_r.png",
    "borders/Line2_t.png",
    "borders/Line2_tl.png",
    "borders/Line2_tr.png",
    "borders/Line4_b.png",
    "borders/Line4_bl.png",
    "borders/Line4_br.png",
    "borders/Line4_l.png",
    "borders/Line4_r.png",
    "borders/Line4_t.png",
    "borders/Line4_tl.png",
    "borders/Line4_tr.png",
    "borders/MenuHorizontalRounded_b_alpha.png",
    "borders/MenuHorizontalRounded_bl_alpha.png",
    "borders/MenuHorizontalRounded_br_alpha.png",
    "borders/MenuHorizontalRounded_l_alpha.png",
    "borders/MenuHorizontalRounded_r_alpha.png",
    "borders/MenuHorizontalRounded_t_alpha.png",
    "borders/MenuHorizontalRounded_tl_alpha.png",
    "borders/MenuHorizontalRounded_tr_alpha.png",
    "borders/MenuVerticalRounded_b_alpha.png",
    "borders/MenuVerticalRounded_bl_alpha.png",
    "borders/MenuVerticalRounded_br_alpha.png",
    "borders/MenuVerticalRounded_l_alpha.png",
    "borders/MenuVerticalRounded_r_alpha.png",
    "borders/MenuVerticalRounded_t_alpha.png",
    "borders/MenuVerticalRounded_tl_alpha.png",
    "borders/MenuVerticalRounded_tr_alpha.png",
    "borders/neon75-f_b_alpha.png",
    "borders/neon75-f_b.png",
    "borders/neon75-f_bl_alpha.png",
    "borders/neon75-f_bl.png",
    "borders/neon75-f_br_alpha.png",
    "borders/neon75-f_br.png",
    "borders/neon75-f_l_alpha.png",
    "borders/neon75-f_l.png",
    "borders/neon75-f_r_alpha.png",
    "borders/neon75-f_r.png",
    "borders/neon75-f_t_alpha.png",
    "borders/neon75-f_t.png",
    "borders/neon75-f_tl_alpha.png",
    "borders/neon75-f_tl.png",
    "borders/neon75-f_tr_alpha.png",
    "borders/neon75-f_tr.png",
    "borders/neon75-n_b_alpha.png",
    "borders/neon75-n_b.png",
    "borders/neon75-n_bl_alpha.png",
    "borders/neon75-n_bl.png",
    "borders/neon75-n_br_alpha.png",
    "borders/neon75-n_br.png",
    "borders/neon75-n_l_alpha.png",
    "borders/neon75-n_l.png",
    "borders/neon75-n_r_alpha.png",
    "borders/neon75-n_r.png",
    "borders/neon75-n_t_alpha.png",
    "borders/neon75-n_t.png",
    "borders/neon75-n_tl_alpha.png",
    "borders/neon75-n_tl.png",
    "borders/neon75-n_tr_alpha.png",
    "borders/neon75-n_tr.png",
    "borders/neon150-f_b_alpha.png",
    "borders/neon150-f_b.png",
    "borders/neon150-f_bl_alpha.png",
    "borders/neon150-f_bl.png",
    "borders/neon150-f_br_alpha.png",
    "borders/neon150-f_br.png",
    "borders/neon150-f_l_alpha.png",
    "borders/neon150-f_l.png",
    "borders/neon150-f_r_alpha.png",
    "borders/neon150-f_r.png",
    "borders/neon150-f_t_alpha.png",
    "borders/neon150-f_t.png",
    "borders/neon150-f_tl_alpha.png",
    "borders/neon150-f_tl.png",
    "borders/neon150-f_tr_alpha_r1_c4.png",
    "borders/neon150-f_tr_alpha_r2_c2.png",
    "borders/neon150-f_tr_alpha_r4_c1.png",
    "borders/neon150-f_tr_alpha.png",
    "borders/neon150-f_tr.png",
    "borders/neon150-n_b_alpha.png",
    "borders/neon150-n_b.png",
    "borders/neon150-n_bl_alpha.png",
    "borders/neon150-n_bl.png",
    "borders/neon150-n_br_alpha.png",
    "borders/neon150-n_br.png",
    "borders/neon150-n_l_alpha.png",
    "borders/neon150-n_l.png",
    "borders/neon150-n_r_alpha.png",
    "borders/neon150-n_r.png",
    "borders/neon150-n_t_alpha.png",
    "borders/neon150-n_t.png",
    "borders/neon150-n_tl_alpha.png",
    "borders/neon150-n_tl.png",
    "borders/neon150-n_tr_alpha.png",
    "borders/neon150-n_tr.png",
    "borders/newsHeader_b_alpha.png",
    "borders/newsHeader_bl_alpha.png",
    "borders/newsHeader_br_alpha.png",
    "borders/newsHeader_l_alpha.png",
    "borders/newsHeader_r_alpha.png",
    "borders/newsHeader_t_alpha.png",
    "borders/newsHeader_tl_alpha.png",
    "borders/newsHeader_tr_alpha.png",
    "borders/newsHeaderLeft_b_alpha.png",
    "borders/newsHeaderLeft_bl_alpha.png",
    "borders/newsHeaderLeft_br_alpha.png",
    "borders/newsHeaderLeft_l_alpha.png",
    "borders/newsHeaderLeft_r_alpha.png",
    "borders/newsHeaderLeft_t_alpha.png",
    "borders/newsHeaderLeft_tl_alpha.png",
    "borders/newsHeaderLeft_tr_alpha.png",
    "borders/newsHeaderRight_b_alpha.png",
    "borders/newsHeaderRight_bl_alpha.png",
    "borders/newsHeaderRight_br_alpha.png",
    "borders/newsHeaderRight_l_alpha.png",
    "borders/newsHeaderRight_r_alpha.png",
    "borders/newsHeaderRight_t_alpha.png",
    "borders/newsHeaderRight_tl_alpha.png",
    "borders/newsHeaderRight_tr_alpha.png",
    "borders/PF_b_alpha.png",
    "borders/PF_bl_alpha.png",
    "borders/PF_br_alpha.png",
    "borders/PF_l_alpha.png",
    "borders/PF_r_alpha.png",
    "borders/PF_t_alpha.png",
    "borders/PF_tl_alpha.png",
    "borders/PF_tr_alpha.png",
    "borders/pipe50_b_alpha.png",
    "borders/pipe50_b.png",
    "borders/pipe50_bl_alpha.png",
    "borders/pipe50_bl.png",
    "borders/pipe50_br_alpha.png",
    "borders/pipe50_br.png",
    "borders/pipe50_l_alpha.png",
    "borders/pipe50_l.png",
    "borders/pipe50_r_alpha.png",
    "borders/pipe50_r.png",
    "borders/pipe50_t_alpha.png",
    "borders/pipe50_t.png",
    "borders/pipe50_tl_alpha.png",
    "borders/pipe50_tl.png",
    "borders/pipe50_tr_alpha.png",
    "borders/pipe50_tr.png",
    "borders/pipe100_b_alpha.png",
    "borders/pipe100_b.png",
    "borders/pipe100_bl_alpha.png",
    "borders/pipe100_bl.png",
    "borders/pipe100_br_alpha.png",
    "borders/pipe100_br.png",
    "borders/pipe100_l_alpha.png",
    "borders/pipe100_l.png",
    "borders/pipe100_r_alpha.png",
    "borders/pipe100_r.png",
    "borders/pipe100_t_alpha.png",
    "borders/pipe100_t.png",
    "borders/pipe100_tl_alpha.png",
    "borders/pipe100_tl.png",
    "borders/pipe100_tr_alpha.png",
    "borders/pipe100_tr.png",
    "borders/right_b_alpha.png",
    "borders/right_bl_alpha.png",
    "borders/right_br_alpha.png",
    "borders/right_cursor_b_alpha.png",
    "borders/right_cursor_bl_alpha.png",
    "borders/right_cursor_br_alpha.png",
    "borders/right_cursor_l_alpha.png",
    "borders/right_cursor_r_alpha.png",
    "borders/right_cursor_t_alpha.png",
    "borders/right_cursor_tl_alpha.png",
    "borders/right_cursor_tr_alpha.png",
    "borders/right_l_alpha.png",
    "borders/right_r_alpha.png",
    "borders/right_t_alpha.png",
    "borders/right_tl_alpha.png",
    "borders/right_tr_alpha.png",
    "borders/rightMenuRounded15_b_alpha.png",
    "borders/rightMenuRounded15_bl_alpha.png",
    "borders/rightMenuRounded15_br_alpha.png",
    "borders/rightMenuRounded15_l_alpha.png",
    "borders/rightMenuRounded15_r_alpha.png",
    "borders/rightMenuRounded15_t_alpha.png",
    "borders/rightMenuRounded15_tl_alpha.png",
    "borders/rightMenuRounded15_tr_alpha.png",
    "borders/rightMenuRounded25_b_alpha.png",
    "borders/rightMenuRounded25_bl_alpha.png",
    "borders/rightMenuRounded25_br_alpha.png",
    "borders/rightMenuRounded25_l_alpha.png",
    "borders/rightMenuRounded25_r_alpha.png",
    "borders/rightMenuRounded25_t_alpha.png",
    "borders/rightMenuRounded25_tl_alpha.png",
    "borders/rightMenuRounded25_tr_alpha.png",
    "borders/rightMenuRounded35_b_alpha.png",
    "borders/rightMenuRounded35_bl_alpha.png",
    "borders/rightMenuRounded35_br_alpha.png",
    "borders/rightMenuRounded35_l_alpha.png",
    "borders/rightMenuRounded35_r_alpha.png",
    "borders/rightMenuRounded35_t_alpha.png",
    "borders/rightMenuRounded35_tl_alpha.png",
    "borders/rightMenuRounded35_tr_alpha.png",
    "borders/rightMenuRounded45_b_alpha.png",
    "borders/rightMenuRounded45_bl_alpha.png",
    "borders/rightMenuRounded45_br_alpha.png",
    "borders/rightMenuRounded45_l_alpha.png",
    "borders/rightMenuRounded45_r_alpha.png",
    "borders/rightMenuRounded45_t_alpha.png",
    "borders/rightMenuRounded45_tl_alpha.png",
    "borders/rightMenuRounded45_tr_alpha.png",
    "borders/rightMenuRounded55_b_alpha.png",
    "borders/rightMenuRounded55_bl_alpha.png",
    "borders/rightMenuRounded55_br_alpha.png",
    "borders/rightMenuRounded55_l_alpha.png",
    "borders/rightMenuRounded55_r_alpha.png",
    "borders/rightMenuRounded55_t_alpha.png",
    "borders/rightMenuRounded55_tl_alpha.png",
    "borders/rightMenuRounded55_tr_alpha.png",
    "borders/rightMenuRounded65_b_alpha.png",
    "borders/rightMenuRounded65_bl_alpha.png",
    "borders/rightMenuRounded65_br_alpha.png",
    "borders/rightMenuRounded65_l_alpha.png",
    "borders/rightMenuRounded65_r_alpha.png",
    "borders/rightMenuRounded65_t_alpha.png",
    "borders/rightMenuRounded65_tl_alpha.png",
    "borders/rightMenuRounded65_tr_alpha.png",
    "borders/rightMenuRounded75_b_alpha.png",
    "borders/rightMenuRounded75_bl_alpha.png",
    "borders/rightMenuRounded75_br_alpha.png",
    "borders/rightMenuRounded75_l_alpha.png",
    "borders/rightMenuRounded75_r_alpha.png",
    "borders/rightMenuRounded75_t_alpha.png",
    "borders/rightMenuRounded75_tl_alpha.png",
    "borders/rightMenuRounded75_tr_alpha.png",
    "borders/rightMenuRounded85_b_alpha.png",
    "borders/rightMenuRounded85_bl_alpha.png",
    "borders/rightMenuRounded85_br_alpha.png",
    "borders/rightMenuRounded85_l_alpha.png",
    "borders/rightMenuRounded85_r_alpha.png",
    "borders/rightMenuRounded85_t_alpha.png",
    "borders/rightMenuRounded85_tl_alpha.png",
    "borders/rightMenuRounded85_tr_alpha.png",
    "borders/rightMenuRounded95_b_alpha.png",
    "borders/rightMenuRounded95_bl_alpha.png",
    "borders/rightMenuRounded95_br_alpha.png",
    "borders/rightMenuRounded95_l_alpha.png",
    "borders/rightMenuRounded95_r_alpha.png",
    "borders/rightMenuRounded95_t_alpha.png",
    "borders/rightMenuRounded95_tl_alpha.png",
    "borders/rightMenuRounded95_tr_alpha.png",
    "borders/rightMenuRounded105_b_alpha.png",
    "borders/rightMenuRounded105_bl_alpha.png",
    "borders/rightMenuRounded105_br_alpha.png",
    "borders/rightMenuRounded105_l_alpha.png",
    "borders/rightMenuRounded105_r_alpha.png",
    "borders/rightMenuRounded105_t_alpha.png",
    "borders/rightMenuRounded105_tl_alpha.png",
    "borders/rightMenuRounded105_tr_alpha.png",
    "borders/rightMenuRounded115_b_alpha.png",
    "borders/rightMenuRounded115_bl_alpha.png",
    "borders/rightMenuRounded115_br_alpha.png",
    "borders/rightMenuRounded115_l_alpha.png",
    "borders/rightMenuRounded115_r_alpha.png",
    "borders/rightMenuRounded115_t_alpha.png",
    "borders/rightMenuRounded115_tl_alpha.png",
    "borders/rightMenuRounded115_tr_alpha.png",
    "borders/rightMenuRounded125_b_alpha.png",
    "borders/rightMenuRounded125_bl_alpha.png",
    "borders/rightMenuRounded125_br_alpha.png",
    "borders/rightMenuRounded125_l_alpha.png",
    "borders/rightMenuRounded125_r_alpha.png",
    "borders/rightMenuRounded125_t_alpha.png",
    "borders/rightMenuRounded125_tl_alpha.png",
    "borders/rightMenuRounded125_tr_alpha.png",
    "borders/rightMenuRounded135_b_alpha.png",
    "borders/rightMenuRounded135_bl_alpha.png",
    "borders/rightMenuRounded135_br_alpha.png",
    "borders/rightMenuRounded135_l_alpha.png",
    "borders/rightMenuRounded135_r_alpha.png",
    "borders/rightMenuRounded135_t_alpha.png",
    "borders/rightMenuRounded135_tl_alpha.png",
    "borders/rightMenuRounded135_tr_alpha.png",
    "borders/rightMenuRounded145_b_alpha.png",
    "borders/rightMenuRounded145_bl_alpha.png",
    "borders/rightMenuRounded145_br_alpha.png",
    "borders/rightMenuRounded145_l_alpha.png",
    "borders/rightMenuRounded145_r_alpha.png",
    "borders/rightMenuRounded145_t_alpha.png",
    "borders/rightMenuRounded145_tl_alpha.png",
    "borders/rightMenuRounded145_tr_alpha.png",
    "borders/rightMenuRounded155_b_alpha.png",
    "borders/rightMenuRounded155_bl_alpha.png",
    "borders/rightMenuRounded155_br_alpha.png",
    "borders/rightMenuRounded155_l_alpha.png",
    "borders/rightMenuRounded155_r_alpha.png",
    "borders/rightMenuRounded155_t_alpha.png",
    "borders/rightMenuRounded155_tl_alpha.png",
    "borders/rightMenuRounded155_tr_alpha.png",
    "borders/rightMenuRounded165_b_alpha.png",
    "borders/rightMenuRounded165_bl_alpha.png",
    "borders/rightMenuRounded165_br_alpha.png",
    "borders/rightMenuRounded165_l_alpha.png",
    "borders/rightMenuRounded165_r_alpha.png",
    "borders/rightMenuRounded165_t_alpha.png",
    "borders/rightMenuRounded165_tl_alpha.png",
    "borders/rightMenuRounded165_tr_alpha.png",
    "borders/rightMenuRounded175_b_alpha.png",
    "borders/rightMenuRounded175_bl_alpha.png",
    "borders/rightMenuRounded175_br_alpha.png",
    "borders/rightMenuRounded175_l_alpha.png",
    "borders/rightMenuRounded175_r_alpha.png",
    "borders/rightMenuRounded175_t_alpha.png",
    "borders/rightMenuRounded175_tl_alpha.png",
    "borders/rightMenuRounded175_tr_alpha.png",
    "borders/rightMenuRounded185_b_alpha.png",
    "borders/rightMenuRounded185_bl_alpha.png",
    "borders/rightMenuRounded185_br_alpha.png",
    "borders/rightMenuRounded185_l_alpha.png",
    "borders/rightMenuRounded185_r_alpha.png",
    "borders/rightMenuRounded185_t_alpha.png",
    "borders/rightMenuRounded185_tl_alpha.png",
    "borders/rightMenuRounded185_tr_alpha.png",
    "borders/rightMenuRounded195_b_alpha.png",
    "borders/rightMenuRounded195_bl_alpha.png",
    "borders/rightMenuRounded195_br_alpha.png",
    "borders/rightMenuRounded195_l_alpha.png",
    "borders/rightMenuRounded195_r_alpha.png",
    "borders/rightMenuRounded195_t_alpha.png",
    "borders/rightMenuRounded195_tl_alpha.png",
    "borders/rightMenuRounded195_tr_alpha.png",
    "borders/sbSquaredLarge_b_alpha.png",
    "borders/sbSquaredLarge_b.png",
    "borders/sbSquaredLarge_bl_alpha.png",
    "borders/sbSquaredLarge_bl.png",
    "borders/sbSquaredLarge_br_alpha.png",
    "borders/sbSquaredLarge_br.png",
    "borders/sbSquaredLarge_l_alpha.png",
    "borders/sbSquaredLarge_l.png",
    "borders/sbSquaredLarge_r_alpha.png",
    "borders/sbSquaredLarge_r.png",
    "borders/sbSquaredLarge_t_alpha.png",
    "borders/sbSquaredLarge_t.png",
    "borders/sbSquaredLarge_tl_alpha.png",
    "borders/sbSquaredLarge_tl.png",
    "borders/sbSquaredLarge_tr_alpha.png",
    "borders/sbSquaredLarge_tr.png",
    "borders/sbSquaredMed_b_alpha.png",
    "borders/sbSquaredMed_b.png",
    "borders/sbSquaredMed_bl_alpha.png",
    "borders/sbSquaredMed_bl.png",
    "borders/sbSquaredMed_br_alpha.png",
    "borders/sbSquaredMed_br.png",
    "borders/sbSquaredMed_l_alpha.png",
    "borders/sbSquaredMed_l.png",
    "borders/sbSquaredMed_r_alpha.png",
    "borders/sbSquaredMed_r.png",
    "borders/sbSquaredMed_t_alpha.png",
    "borders/sbSquaredMed_t.png",
    "borders/sbSquaredMed_tl_alpha.png",
    "borders/sbSquaredMed_tl.png",
    "borders/sbSquaredMed_tr_alpha.png",
    "borders/sbSquaredMed_tr.png",
    "borders/sbSquaredSmall_b_alpha.png",
    "borders/sbSquaredSmall_b.png",
    "borders/sbSquaredSmall_bl_alpha.png",
    "borders/sbSquaredSmall_bl.png",
    "borders/sbSquaredSmall_br_alpha.png",
    "borders/sbSquaredSmall_br.png",
    "borders/sbSquaredSmall_l_alpha.png",
    "borders/sbSquaredSmall_l.png",
    "borders/sbSquaredSmall_r_alpha.png",
    "borders/sbSquaredSmall_r.png",
    "borders/sbSquaredSmall_t_alpha.png",
    "borders/sbSquaredSmall_t.png",
    "borders/sbSquaredSmall_tl_alpha.png",
    "borders/sbSquaredSmall_tl.png",
    "borders/sbSquaredSmall_tr_alpha.png",
    "borders/sbSquaredSmall_tr.png",
    "borders/top_cursor_b_alpha.png",
    "borders/top_cursor_bl_alpha.png",
    "borders/top_cursor_br_alpha.png",
    "borders/top_cursor_l_alpha.png",
    "borders/top_cursor_r_alpha.png",
    "borders/top_cursor_t_alpha.png",
    "borders/top_cursor_tl_alpha.png",
    "borders/top_cursor_tr_alpha.png",
    "borders/topMenuRounded15_b_alpha.png",
    "borders/topMenuRounded15_bl_alpha.png",
    "borders/topMenuRounded15_br_alpha.png",
    "borders/topMenuRounded15_l_alpha.png",
    "borders/topMenuRounded15_r_alpha.png",
    "borders/topMenuRounded15_t_alpha.png",
    "borders/topMenuRounded15_tl_alpha.png",
    "borders/topMenuRounded15_tr_alpha.png",
    "borders/topMenuRounded25_b_alpha.png",
    "borders/topMenuRounded25_bl_alpha.png",
    "borders/topMenuRounded25_br_alpha.png",
    "borders/topMenuRounded25_l_alpha.png",
    "borders/topMenuRounded25_r_alpha.png",
    "borders/topMenuRounded25_t_alpha.png",
    "borders/topMenuRounded25_tl_alpha.png",
    "borders/topMenuRounded25_tr_alpha.png",
    "borders/topMenuRounded35_b_alpha.png",
    "borders/topMenuRounded35_bl_alpha.png",
    "borders/topMenuRounded35_br_alpha.png",
    "borders/topMenuRounded35_l_alpha.png",
    "borders/topMenuRounded35_r_alpha.png",
    "borders/topMenuRounded35_t_alpha.png",
    "borders/topMenuRounded35_tl_alpha.png",
    "borders/topMenuRounded35_tr_alpha.png",
    "borders/topMenuRounded45_b_alpha.png",
    "borders/topMenuRounded45_bl_alpha.png",
    "borders/topMenuRounded45_br_alpha.png",
    "borders/topMenuRounded45_l_alpha.png",
    "borders/topMenuRounded45_r_alpha.png",
    "borders/topMenuRounded45_t_alpha.png",
    "borders/topMenuRounded45_tl_alpha.png",
    "borders/topMenuRounded45_tr_alpha.png",
    "borders/topMenuRounded55_b_alpha.png",
    "borders/topMenuRounded55_bl_alpha.png",
    "borders/topMenuRounded55_br_alpha.png",
    "borders/topMenuRounded55_l_alpha.png",
    "borders/topMenuRounded55_r_alpha.png",
    "borders/topMenuRounded55_t_alpha.png",
    "borders/topMenuRounded55_tl_alpha.png",
    "borders/topMenuRounded55_tr_alpha.png",
    "borders/topMenuRounded65_b_alpha.png",
    "borders/topMenuRounded65_bl_alpha.png",
    "borders/topMenuRounded65_br_alpha.png",
    "borders/topMenuRounded65_l_alpha.png",
    "borders/topMenuRounded65_r_alpha.png",
    "borders/topMenuRounded65_t_alpha.png",
    "borders/topMenuRounded65_tl_alpha.png",
    "borders/topMenuRounded65_tr_alpha.png",
    "borders/topMenuRounded75_b_alpha.png",
    "borders/topMenuRounded75_bl_alpha.png",
    "borders/topMenuRounded75_br_alpha.png",
    "borders/topMenuRounded75_l_alpha.png",
    "borders/topMenuRounded75_r_alpha.png",
    "borders/topMenuRounded75_t_alpha.png",
    "borders/topMenuRounded75_tl_alpha.png",
    "borders/topMenuRounded75_tr_alpha.png",
    "borders/topMenuRounded85_b_alpha.png",
    "borders/topMenuRounded85_bl_alpha.png",
    "borders/topMenuRounded85_br_alpha.png",
    "borders/topMenuRounded85_l_alpha.png",
    "borders/topMenuRounded85_r_alpha.png",
    "borders/topMenuRounded85_t_alpha.png",
    "borders/topMenuRounded85_tl_alpha.png",
    "borders/topMenuRounded85_tr_alpha.png",
    "borders/topMenuRounded95_b_alpha.png",
    "borders/topMenuRounded95_bl_alpha.png",
    "borders/topMenuRounded95_br_alpha.png",
    "borders/topMenuRounded95_l_alpha.png",
    "borders/topMenuRounded95_r_alpha.png",
    "borders/topMenuRounded95_t_alpha.png",
    "borders/topMenuRounded95_tl_alpha.png",
    "borders/topMenuRounded95_tr_alpha.png",
    "borders/topMenuRounded105_b_alpha.png",
    "borders/topMenuRounded105_bl_alpha.png",
    "borders/topMenuRounded105_br_alpha.png",
    "borders/topMenuRounded105_l_alpha.png",
    "borders/topMenuRounded105_r_alpha.png",
    "borders/topMenuRounded105_t_alpha.png",
    "borders/topMenuRounded105_tl_alpha.png",
    "borders/topMenuRounded105_tr_alpha.png",
    "borders/topMenuRounded115_b_alpha.png",
    "borders/topMenuRounded115_bl_alpha.png",
    "borders/topMenuRounded115_br_alpha.png",
    "borders/topMenuRounded115_l_alpha.png",
    "borders/topMenuRounded115_r_alpha.png",
    "borders/topMenuRounded115_t_alpha.png",
    "borders/topMenuRounded115_tl_alpha.png",
    "borders/topMenuRounded115_tr_alpha.png",
    "borders/topMenuRounded125_b_alpha.png",
    "borders/topMenuRounded125_bl_alpha.png",
    "borders/topMenuRounded125_br_alpha.png",
    "borders/topMenuRounded125_l_alpha.png",
    "borders/topMenuRounded125_r_alpha.png",
    "borders/topMenuRounded125_t_alpha.png",
    "borders/topMenuRounded125_tl_alpha.png",
    "borders/topMenuRounded125_tr_alpha.png",
    "borders/topMenuRounded135_b_alpha.png",
    "borders/topMenuRounded135_bl_alpha.png",
    "borders/topMenuRounded135_br_alpha.png",
    "borders/topMenuRounded135_l_alpha.png",
    "borders/topMenuRounded135_r_alpha.png",
    "borders/topMenuRounded135_t_alpha.png",
    "borders/topMenuRounded135_tl_alpha.png",
    "borders/topMenuRounded135_tr_alpha.png",
    "borders/topMenuRounded145_b_alpha.png",
    "borders/topMenuRounded145_bl_alpha.png",
    "borders/topMenuRounded145_br_alpha.png",
    "borders/topMenuRounded145_l_alpha.png",
    "borders/topMenuRounded145_r_alpha.png",
    "borders/topMenuRounded145_t_alpha.png",
    "borders/topMenuRounded145_tl_alpha.png",
    "borders/topMenuRounded145_tr_alpha.png",
    "borders/topMenuRounded155_b_alpha.png",
    "borders/topMenuRounded155_bl_alpha.png",
    "borders/topMenuRounded155_br_alpha.png",
    "borders/topMenuRounded155_l_alpha.png",
    "borders/topMenuRounded155_r_alpha.png",
    "borders/topMenuRounded155_t_alpha.png",
    "borders/topMenuRounded155_tl_alpha.png",
    "borders/topMenuRounded155_tr_alpha.png",
    "borders/topMenuRounded165_b_alpha.png",
    "borders/topMenuRounded165_bl_alpha.png",
    "borders/topMenuRounded165_br_alpha.png",
    "borders/topMenuRounded165_l_alpha.png",
    "borders/topMenuRounded165_r_alpha.png",
    "borders/topMenuRounded165_t_alpha.png",
    "borders/topMenuRounded165_tl_alpha.png",
    "borders/topMenuRounded165_tr_alpha.png",
    "borders/topMenuRounded175_b_alpha.png",
    "borders/topMenuRounded175_bl_alpha.png",
    "borders/topMenuRounded175_br_alpha.png",
    "borders/topMenuRounded175_l_alpha.png",
    "borders/topMenuRounded175_r_alpha.png",
    "borders/topMenuRounded175_t_alpha.png",
    "borders/topMenuRounded175_tl_alpha.png",
    "borders/topMenuRounded175_tr_alpha.png",
    "borders/topMenuRounded185_b_alpha.png",
    "borders/topMenuRounded185_bl_alpha.png",
    "borders/topMenuRounded185_br_alpha.png",
    "borders/topMenuRounded185_l_alpha.png",
    "borders/topMenuRounded185_r_alpha.png",
    "borders/topMenuRounded185_t_alpha.png",
    "borders/topMenuRounded185_tl_alpha.png",
    "borders/topMenuRounded185_tr_alpha.png",
    "borders/topMenuRounded195_b_alpha.png",
    "borders/topMenuRounded195_bl_alpha.png",
    "borders/topMenuRounded195_br_alpha.png",
    "borders/topMenuRounded195_l_alpha.png",
    "borders/topMenuRounded195_r_alpha.png",
    "borders/topMenuRounded195_t_alpha.png",
    "borders/topMenuRounded195_tl_alpha.png",
    "borders/topMenuRounded195_tr_alpha.png",
    "borders/up_b_alpha.png",
    "borders/up_bl_alpha.png",
    "borders/up_br_alpha.png",
    "borders/up_l_alpha.png",
    "borders/up_r_alpha.png",
    "borders/up_t_alpha.png",
    "borders/up_tl_alpha.png",
    "borders/up_tr_alpha.png",
    "borders/VOval30x60_b_alpha.png",
    "borders/VOval30x60_bl_alpha.png",
    "borders/VOval30x60_br_alpha.png",
    "borders/VOval30x60_l_alpha.png",
    "borders/VOval30x60_r_alpha.png",
    "borders/VOval30x60_t_alpha.png",
    "borders/VOval30x60_tl_alpha.png",
    "borders/VOval30x60_tr_alpha.png",
    "borders/VOval50x100_b_alpha.png",
    "borders/VOval50x100_bl_alpha.png",
    "borders/VOval50x100_br_alpha.png",
    "borders/VOval50x100_l_alpha.png",
    "borders/VOval50x100_r_alpha.png",
    "borders/VOval50x100_t_alpha.png",
    "borders/VOval50x100_tl_alpha.png",
    "borders/VOval50x100_tr_alpha.png",
    "borders/VOval75x150_b_alpha.png",
    "borders/VOval75x150_bl_alpha.png",
    "borders/VOval75x150_br_alpha.png",
    "borders/VOval75x150_l_alpha.png",
    "borders/VOval75x150_r_alpha.png",
    "borders/VOval75x150_t_alpha.png",
    "borders/VOval75x150_tl_alpha.png",
    "borders/VOval75x150_tr_alpha.png",
    "borders/VOval100x200_b_alpha.png",
    "borders/VOval100x200_bl_alpha.png",
    "borders/VOval100x200_br_alpha.png",
    "borders/VOval100x200_l_alpha.png",
    "borders/VOval100x200_r_alpha.png",
    "borders/VOval100x200_t_alpha.png",
    "borders/VOval100x200_tl_alpha.png",
    "borders/VOval100x200_tr_alpha.png",
    "borders/WindowsPopup_b_alpha.png",
    "borders/WindowsPopup_b.png",
    "borders/WindowsPopup_bl_alpha.png",
    "borders/WindowsPopup_bl.png",
    "borders/WindowsPopup_br_alpha.png",
    "borders/WindowsPopup_br.png",
    "borders/WindowsPopup_l_alpha.png",
    "borders/WindowsPopup_l.png",
    "borders/WindowsPopup_r_alpha.png",
    "borders/WindowsPopup_r.png",
    "borders/WindowsPopup_t_alpha.png",
    "borders/WindowsPopup_t.png",
    "borders/WindowsPopup_tl_alpha.png",
    "borders/WindowsPopup_tl.png",
    "borders/WindowsPopup_tr_alpha.png",
    "borders/WindowsPopup_tr.png",
    "borders/WindowsPopupStatus_b_alpha.png",
    "borders/WindowsPopupStatus_b.png",
    "borders/WindowsPopupStatus_bl_alpha.png",
    "borders/WindowsPopupStatus_bl.png",
    "borders/WindowsPopupStatus_br_alpha.png",
    "borders/WindowsPopupStatus_br.png",
    "borders/WindowsPopupStatus_l_alpha.png",
    "borders/WindowsPopupStatus_l.png",
    "borders/WindowsPopupStatus_r_alpha.png",
    "borders/WindowsPopupStatus_r.png",
    "borders/WindowsPopupStatus_t_alpha.png",
    "borders/WindowsPopupStatus_t.png",
    "borders/WindowsPopupStatus_tl_alpha.png",
    "borders/WindowsPopupStatus_tl.png",
    "borders/WindowsPopupStatus_tr_alpha.png",
    "borders/WindowsPopupStatus_tr.png"
};

const vector<QString> sliderResources = {
    "sliders/aqua_b_alpha.png",
    "sliders/aqua_b.png",
    "sliders/aqua_h_alpha.png",
    "sliders/aqua_h.png",
    "sliders/aqua_l_alpha.png",
    "sliders/aqua_l.png",
    "sliders/aqua_r_alpha.png",
    "sliders/aqua_r.png",
    "sliders/aqua_t_alpha.png",
    "sliders/aqua_t.png",
    "sliders/aqua_v_alpha.png",
    "sliders/aqua_v.png",
    "sliders/aquaS_b_alpha.png",
    "sliders/aquaS_b.png",
    "sliders/aquaS_h_alpha.png",
    "sliders/aquaS_h.png",
    "sliders/aquaS_l_alpha.png",
    "sliders/aquaS_l.png",
    "sliders/aquaS_r_alpha.png",
    "sliders/aquaS_r.png",
    "sliders/aquaS_t_alpha.png",
    "sliders/aquaS_t.png",
    "sliders/aquaS_v_alpha.png",
    "sliders/aquaS_v.png",
    "sliders/Ball_b_alpha.png",
    "sliders/Ball_b.png",
    "sliders/Ball_h_alpha.png",
    "sliders/Ball_h.png",
    "sliders/Ball_l_alpha.png",
    "sliders/Ball_l.png",
    "sliders/Ball_r_alpha.png",
    "sliders/Ball_r.png",
    "sliders/Ball_t_alpha.png",
    "sliders/Ball_t.png",
    "sliders/Ball_v_alpha.png",
    "sliders/Ball_v.png",
    "sliders/CircleL_b_alpha.png",
    "sliders/CircleL_h_alpha.png",
    "sliders/CircleL_l_alpha.png",
    "sliders/CircleL_r_alpha.png",
    "sliders/CircleL_t_alpha.png",
    "sliders/CircleL_v_alpha.png",
    "sliders/CircleM_b_alpha.png",
    "sliders/CircleM_h_alpha.png",
    "sliders/CircleM_l_alpha.png",
    "sliders/CircleM_r_alpha.png",
    "sliders/CircleM_t_alpha.png",
    "sliders/CircleM_v_alpha.png",
    "sliders/CircleS_b_alpha.png",
    "sliders/CircleS_h_alpha.png",
    "sliders/CircleS_l_alpha.png",
    "sliders/CircleS_r_alpha.png",
    "sliders/CircleS_t_alpha.png",
    "sliders/CircleS_v_alpha.png",
    "sliders/LineL_b.png",
    "sliders/LineL_h.png",
    "sliders/LineL_l.png",
    "sliders/LineL_r.png",
    "sliders/LineL_t.png",
    "sliders/LineL_v.png",
    "sliders/LineM_b.png",
    "sliders/LineM_h.png",
    "sliders/LineM_l.png",
    "sliders/LineM_r.png",
    "sliders/LineM_t.png",
    "sliders/LineM_v.png",
    "sliders/LineS_b.png",
    "sliders/LineS_h.png",
    "sliders/LineS_l.png",
    "sliders/LineS_r.png",
    "sliders/LineS_t.png",
    "sliders/LineS_v.png",
    "sliders/prec_b_alpha.png",
    "sliders/prec_h_alpha.png",
    "sliders/prec_l_alpha.png",
    "sliders/prec_r_alpha.png",
    "sliders/prec_t_alpha.png",
    "sliders/prec_v_alpha.png",
    "sliders/windowActive_b_alpha.png",
    "sliders/windowActive_b.png",
    "sliders/windowActive_h_alpha.png",
    "sliders/windowActive_h.png",
    "sliders/windowActive_l_alpha.png",
    "sliders/windowActive_l.png",
    "sliders/windowActive_r_alpha.png",
    "sliders/windowActive_r.png",
    "sliders/windowActive_t_alpha.png",
    "sliders/windowActive_t.png",
    "sliders/windowActive_v_alpha.png",
    "sliders/windowActive_v.png",
    "sliders/windows_b.png",
    "sliders/windows_h.png",
    "sliders/windows_l.png",
    "sliders/windows_r.png",
    "sliders/windows_t.png",
    "sliders/windows_v.png"
};

const vector<QString> fontResources = {
    "fonts/amxbold_.ttf",
    "fonts/arial.ttf",
    "fonts/arialbd.ttf",
    "fonts/cour.ttf"
};

const vector<QString> soundResources = {
    "sounds/audioTest.wav",
    "sounds/docked.mp3",
    "sounds/doubleBeep.wav",
    "sounds/doubleBeep01.wav",
    "sounds/doubleBeep02.wav",
    "sounds/doubleBeep03.wav",
    "sounds/doubleBeep04.wav",
    "sounds/doubleBeep05.wav",
    "sounds/doubleBeep06.wav",
    "sounds/doubleBeep07.wav",
    "sounds/doubleBeep08.wav",
    "sounds/doubleBeep09.wav",
    "sounds/doubleBeep10.wav",
    "sounds/ringback.wav",
    "sounds/ringtone.wav",
    "sounds/singleBeep.wav",
    "sounds/singleBeep01.wav",
    "sounds/singleBeep02.wav",
    "sounds/singleBeep03.wav",
    "sounds/singleBeep04.wav",
    "sounds/singleBeep05.wav",
    "sounds/singleBeep06.wav",
    "sounds/singleBeep07.wav",
    "sounds/singleBeep08.wav",
    "sounds/singleBeep09.wav",
    "sounds/singleBeep10.wav"
};

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

QList<FAMILY_t> TGraphics::getBorders()
{
    DECL_TRACER("TGraphics::getBorders()");

    QList<FAMILY_t> borders;

    for (FAMILY_t family : mDraw.borders)
        borders.append(family);

    // Sort with simple bubble sort in ascending order
    bool changed = false;

    do
    {
        changed = false;

        for (qsizetype i = 0; i < borders.count(); ++i)
        {
            if (i == 0)
                continue;

            if (borders[i].name < borders[i-1].name)
            {
                borders.swapItemsAt(i, i-1);
                changed = true;
            }
        }
    }while (changed);

    return borders;
}

QList<FAMILY_t> TGraphics::getEfects()
{
    DECL_TRACER("TGraphics::getEfects()");

    QList<FAMILY_t> eff;
    vector<FAMILY_t> effects = mDraw.effects;

    for (FAMILY_t family : effects)
        eff.append(family);

    // Sort with simple bubble sort in ascending order
    bool changed = false;

    do
    {
        changed = false;

        for (qsizetype i = 0; i < eff.count(); ++i)
        {
            if (i == 0)
                continue;

            if (eff[i].name < eff[i-1].name)
            {
                eff.swapItemsAt(i, i-1);
                changed = true;
            }
        }
    }while (changed);

    return eff;
}

int TGraphics::getEffectStyleNumber(const QString& name)
{
    DECL_TRACER("TGraphics::getEffectStyleNumber(const QString& name)");

    for (EFFECT_STYLE_t style : mDraw.effectStyles)
    {
        if (style.name == name)
            return style.number;
    }

    return 0;
}

QString TGraphics::getEffectStyleName(int number)
{
    DECL_TRACER("TGraphics::getEffectStyleName(int number)");

    for (EFFECT_STYLE_t style : mDraw.effectStyles)
    {
        if (style.number == number)
            return style.name;
    }

    return QString();
}

EFFECT_STYLE_t TGraphics::getEffectDetails(int number)
{
    DECL_TRACER("TGraphics::getEffectDetails(int number)");

    for (EFFECT_STYLE_t style : mDraw.effectStyles)
    {
        if (style.number == number)
            return style;
    }

    return EFFECT_STYLE_t();
}

QStringList TGraphics::getSliderNames()
{
    DECL_TRACER("TGraphics::getSliderNames()");

    QStringList sliders;

    for (FAMILY_t family : mDraw.sliders)
    {
        for (QString s : family.member)
            sliders.append(s);
    }

    return sliders;
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
        // Write the resources
        if (!writeBorderResources(basePath))
            return false;

        if (!writeSliderResources(basePath))
            return false;

        if (!writeFontResources(basePath))
            return false;

        if (!writeSoundResources(basePath))
            return false;

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

bool TGraphics::writeBorderResources(const QString& path)
{
    DECL_TRACER("TGraphics::writeBorderResources(const QString& path)");

    QString baseFile = QString("%1/__system/graphics/borders/").arg(path);
    vector<QString>::const_iterator iter;

    for (iter = borderResources.cbegin(); iter != borderResources.cend(); ++iter)
    {
        QString res = *iter;
        qsizetype pos = res.lastIndexOf('/');
        QString file = baseFile + res.mid(pos + 1);
        QImage *image = new QImage(QString(":/%1").arg(*iter));

        if (!image->save(file))
        {
            MSG_ERROR("Error saving file: " << file.toStdString());
            delete image;
            return false;
        }

        delete image;
    }

    return true;
}

bool TGraphics::writeSliderResources(const QString& path)
{
    DECL_TRACER("TGraphics::writeSliderResources(const QString& path)");

    QString baseFile = QString("%1/__system/graphics/sliders/").arg(path);
    vector<QString>::const_iterator iter;

    for (iter = sliderResources.cbegin(); iter != sliderResources.cend(); ++iter)
    {
        QString res = *iter;
        qsizetype pos = res.lastIndexOf('/');
        QString file = baseFile + res.mid(pos + 1);
        QImage *image = new QImage(QString(":/%1").arg(*iter));

        if (!image->save(file))
        {
            MSG_ERROR("Error saving file: " << file.toStdString());
            delete image;
            return false;
        }

        delete image;
    }

    return true;
}

bool TGraphics::writeFontResources(const QString& path)
{
    DECL_TRACER("TGraphics::writeFontResources(const QString& path)");

    QString baseFile = QString("%1/__system/graphics/fonts/").arg(path);
    vector<QString>::const_iterator iter;

    for (iter = fontResources.cbegin(); iter != fontResources.cend(); ++iter)
    {
        QString res = *iter;
        qsizetype pos = res.lastIndexOf('/');
        QString file = baseFile + res.mid(pos + 1);
        QFile *datei = new QFile(QString(":/%1").arg(*iter));

        if (!datei->copy(file))
        {
            MSG_ERROR("Error saving file: " << file.toStdString());
            delete datei;
            return false;
        }

        delete datei;
    }

    return true;
}

bool TGraphics::writeSoundResources(const QString& path)
{
    DECL_TRACER("TGraphics::writeSoundResources(const QString& path)");

    QString baseFile = QString("%1/__system/graphics/sounds/").arg(path);
    vector<QString>::const_iterator iter;

    for (iter = soundResources.cbegin(); iter != soundResources.cend(); ++iter)
    {
        QString res = *iter;
        qsizetype pos = res.lastIndexOf('/');
        QString file = baseFile + res.mid(pos + 1);
        QFile *datei = new QFile(QString(":/%1").arg(*iter));

        if (!datei->copy(file))
        {
            MSG_ERROR("Error saving file: " << file.toStdString());
            delete datei;
            return false;
        }

        delete datei;
    }

    return true;
}
