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
#include "tpaneltypes.h"
#include "terror.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

TPanelType::TPanelType()
{
    DECL_TRACER("TPanelType::TPanelType()");

    // VARIA-SL50
    RESOLUTION_t res;
    res.image = "varia-sl50.png";
    res.description = "AMX Varia 5.5\" ultra-slim wall mount touch panel";
    res.size.push_back(QSize(720, 1280));
    mResolutions.insert(pair<string, RESOLUTION_t>("VARIA-SL50", res));

    // VARIA-SL80
    res.clear();
    res.image = "varia-sl80.png";
    res.description = "AMX Varia 8\" ultra-slim wall mount touch panel";
    res.size.push_back(QSize(1280, 800));
    mResolutions.insert(pair<string, RESOLUTION_t>("VARIA-SL80", res));

    // VARIA-80
    res.clear();
    res.image = "varia-80.png";
    res.description = "AMX Varia 8\" touch panel";
    res.size.push_back(QSize(1280, 800));
    mResolutions.insert(pair<string, RESOLUTION_t>("VARIA-80", res));

    // VARIA-100
    res.clear();
    res.image = "varia-100.png";
    res.description = "AMX Varia 10.1\" touch panel";
    res.size.push_back(QSize(1920, 1200));
    mResolutions.insert(pair<string, RESOLUTION_t>("VARIA-100", res));

    // VARIA-150
    res.clear();
    res.image = "varia-150.png";
    res.description = "AMX Varia 15.6\" touch panel";
    res.size.push_back(QSize(1920, 1024));
    mResolutions.insert(pair<string, RESOLUTION_t>("VARIA-150", res));

    // MT-702
    res.clear();
    res.image = "mt-702.png";
    res.description = "Modero 7\" tabletop touch panel";
    res.size.push_back(QSize(1280, 800));
    mResolutions.insert(pair<string, RESOLUTION_t>("MT-702", res));

    // MD-702
    res.clear();
    res.image = "md-702.png";
    res.description = "Modero 7\" wall-mount touch panel";
    res.size.push_back(QSize(1280, 800));
    mResolutions.insert(pair<string, RESOLUTION_t>("MD-702", res));

    // MT-1002
    res.clear();
    res.image = "mt-1002.png";
    res.description = "Modero 10\" tabletop touch panel";
    res.size.push_back(QSize(1280, 800));
    mResolutions.insert(pair<string, RESOLUTION_t>("MT-1002", res));

    // MD-1002
    res.clear();
    res.image = "md-1002.png";
    res.description = "Modero 10\" wall-mount touch panel";
    res.size.push_back(QSize(1280, 800));
    mResolutions.insert(pair<string, RESOLUTION_t>("MD-1002", res));

    // MT-2002
    res.clear();
    res.image = "mt-2002.png";
    res.description = "Modero 20\" Panoramic tabletop touch panel";
    res.size.push_back(QSize(1980, 800));
    mResolutions.insert(pair<string, RESOLUTION_t>("MT-2002", res));

    // MXT-701
    res.clear();
    res.image = "mxt-701.png";
    res.description = "Modero X 7\" tabletop touch panel";
    res.size.push_back(QSize(1024, 600));
    mResolutions.insert(pair<string, RESOLUTION_t>("MXT-701", res));

    // MXD-701
    res.clear();
    res.image = "mxd-701.png";
    res.description = "Modero X 7\" wall-mount touch panel";
    res.size.push_back(QSize(1024, 600));
    mResolutions.insert(pair<string, RESOLUTION_t>("MXD-701", res));

    // MXT-1001
    res.clear();
    res.image = "mxt-1001.png";
    res.description = "Modero X 10\" tabletop touch panel";
    res.size.push_back(QSize(1280, 800));
    mResolutions.insert(pair<string, RESOLUTION_t>("MXT-1001", res));

    // MXD-1001
    res.clear();
    res.image = "mxd-1001.png";
    res.description = "Modero X 10\" wall-mount touch panel";
    res.size.push_back(QSize(1280, 800));
    res.size.push_back(QSize(800, 1280));
    mResolutions.insert(pair<string, RESOLUTION_t>("MXD-1001", res));

    // MXR-1001
    res.clear();
    res.image = "mxr-1001.png";
    res.description = "Modero X 10\" Retractable touch panel";
    res.size.push_back(QSize(1280, 800));
    mResolutions.insert(pair<string, RESOLUTION_t>("MXR-1001", res));

    // MXT-1901-PAN
    res.clear();
    res.image = "mxt-1901-pan.png";
    res.description = "Modero X 19\" Panoramic tabletop touch panel";
    res.size.push_back(QSize(1920, 530));
    mResolutions.insert(pair<string, RESOLUTION_t>("MXT-1901-PAN", res));

    // MXD-1901-PAN
    res.clear();
    res.image = "mxd-1901-pan.png";
    res.description = "Modero X 19\" Panoramic wall-mount touch panel";
    res.size.push_back(QSize(1920, 530));
    res.size.push_back(QSize(530, 1920));
    mResolutions.insert(pair<string, RESOLUTION_t>("MXD-1901-PAN", res));

    // MXT-2001-PAN
    res.clear();
    res.image = "mxt-2001-pan.png";
    res.description = "Modero X 20\" Panoramic tabletop touch panel";
    res.size.push_back(QSize(1920, 800));
    mResolutions.insert(pair<string, RESOLUTION_t>("MXT-2001-PAN", res));

    // MXD-2001-PAN
    res.clear();
    res.image = "mxd-2001-pan.png";
    res.description = "Modero X 20\" Panoramic wall-mount touch panel";
    res.size.push_back(QSize(1920, 800));
    res.size.push_back(QSize(800, 1920));
    mResolutions.insert(pair<string, RESOLUTION_t>("MXD-2001-PAN", res));

    // TheoSys
    res.clear();
    res.image = "TheoSys.png";
    res.description = "Different resolutions to serve different kind of display formats.";
    res.size.push_back(QSize(480, 1170));
    res.size.push_back(QSize(960, 640));
    res.size.push_back(QSize(640, 960));
    res.size.push_back(QSize(1024, 768));
    res.size.push_back(QSize(768, 1024));
    res.size.push_back(QSize(1136, 640));
    res.size.push_back(QSize(640, 1136));
    res.size.push_back(QSize(1200, 800));
    res.size.push_back(QSize(800, 1200));
    res.size.push_back(QSize(1280, 800));
    res.size.push_back(QSize(800, 1280));
    res.size.push_back(QSize(1334, 750));
    res.size.push_back(QSize(750, 1334));
    res.size.push_back(QSize(480, 1170));
    res.size.push_back(QSize(1920, 1080));
    res.size.push_back(QSize(1080, 1920));
    res.size.push_back(QSize(1920, 1280));
    res.size.push_back(QSize(1280, 1920));
    res.size.push_back(QSize(2048, 1536));
    res.size.push_back(QSize(1536, 2048));
    res.size.push_back(QSize(2732, 2048));
    res.size.push_back(QSize(2048, 2732));
    res.size.push_back(QSize(2436, 1125));
    res.size.push_back(QSize(1125, 2436));
    res.size.push_back(QSize(2560, 1440));
    res.size.push_back(QSize(1440, 2560));
    res.size.push_back(QSize(3840, 2160));
    res.size.push_back(QSize(2160, 3840));
    mResolutions.insert(pair<string, RESOLUTION_t>("TheoSys", res));
}

TPanelType::~TPanelType()
{
    DECL_TRACER("TPanelType::~TPanelType()");

    mResolutions.clear();
}

vector<QSize> TPanelType::getResolutions(const string& name)
{
    DECL_TRACER("TPanelType::getResolutions(const string& name)");

    map<string, RESOLUTION_t>::iterator iter;

    if ((iter = mResolutions.find(name)) != mResolutions.end())
        return iter->second.size;

    return vector<QSize>();
}

string TPanelType::getImageName(const string& name)
{
    DECL_TRACER("TPanelType::getImageName(const string& name)");

    map<string, RESOLUTION_t>::iterator iter;

    if ((iter = mResolutions.find(name)) != mResolutions.end())
        return iter->second.image;

    return string();
}

string TPanelType::getDescription(const string& name)
{
    DECL_TRACER("TPanelType::getDescription(const string& name)");

    map<string, RESOLUTION_t>::iterator iter;

    if ((iter = mResolutions.find(name)) != mResolutions.end())
        return iter->second.description;

    return string();
}

QString TPanelType::getPanelName(PANELTYPE_t pn)
{
    DECL_TRACER("TPanelType::getPanelName(PANELTYPE_t pn)");

    switch(pn)
    {
        case PAN_VARIA_SL50:    return "VARIA-SL50";
        case PAN_VARIA_SL80:    return "VARIA-SL80";
        case PAN_VARIA_80:      return "VARIA-80";
        case PAN_VARIA_100:     return "VARIA-100";
        case PAN_VARIA_150:     return "VARIA-150";
        case PAN_MT_702:        return "MT-702";
        case PAN_MD_702:        return "MD-702";
        case PAN_MT_1002:       return "MT-1002";
        case PAN_MD_1002:       return "MD-1002";
        case PAN_MT_2002:       return "MT-1002";
        case PAN_MXT_701:       return "MXT-701";
        case PAN_MXD_701:       return "MXD-701";
        case PAN_MXT_1001:      return "MXT-1001";
        case PAN_MXD_1001:      return "MXD-1001";
        case PAN_MXR_1001:      return "MXR-1001";
        case PAN_MXT_1901_PAN:  return "MXT-1901-PAN";
        case PAN_MXD_1901_PAN:  return "MXD-1901-PAN";
        case PAN_MXT_2001_PAN:  return "MXT-2001-PAN";
        case PAN_MXD_2001_PAN:  return "MXD-2001-PAN";
        case PAN_THEOSYS:       return "THEOSYS";
    }

    return QString();
}
