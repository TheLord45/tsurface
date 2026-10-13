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
// SPDX-FileCopyrightText: 2026 Andreas Theofilu <andreas@theosys.at>
// SPDX-License-Identifier: GPL-3.0-or-later
#include <QDir>
#include <QBrush>
#include <QResizeEvent>

#include "tqmap.h"
#include "map/mapview.h"
#include "map/mapglobal.h"
#include "map/mapitem.h"
#include "terror.h"

namespace fs = std::filesystem;

TQMap::TQMap(QWidget *parent)
    : QWidget(parent),
      mParent(parent)
{
    DECL_TRACER("TQMap::TQMap(QWidget *parent)");
}

TQMap::~TQMap()
{
    DECL_TRACER("TQMap::~TQMap()");

    if (mMapView)
        delete mMapView;
}

void TQMap::drawMap(const QSize& size, double lat, double lon, int zoom, bool sslIgnore)
{
    DECL_TRACER("TQMap::drawMap(const QSize& size, double lat, double lon, int zoom, bool sslIgnore)");

    if (!mMapView)
    {
        mMapView = new MapView(mParent, sslIgnore);
        mMapView->resize(size);
        mMapView->show();
    }

    mCenterLat = lat;
    mCenterLon = lon;
    mZoomLevel = (zoom > 0 && zoom < 20 ? zoom : 10);

    QDir cpath(getTileCache());

    if (!cpath.exists())
        cpath.mkdir(cpath.absolutePath());

    mMapView->setCachePath(cpath.absolutePath());
    mMapView->setProvider(getProvider(mProvider));
    mMapView->setCenterOn(QPointF(lon, lat));
    mMapView->setZoom(zoom);
    // Set a visible object to mark the center
    MapItem *item = mMapView->createItem();
    item->move(QPointF(lon, lat));
    item->setPen(QPen(QColor(0, 255, 0), 3), MapItemState::AllState);
    item->setBrush(QBrush(QColor(0, 255, 0)), MapItemState::AllState);
    item->setEllipse(QSize(14, 14));
}

void TQMap::setProvider(PROVIDER_t prov)
{
    DECL_TRACER("TQMap::setProvider(PROVIDER_t prov)");

    mProvider = prov;

    if (mMapView)
        mMapView->setProvider(getProvider(mProvider));
}

void TQMap::resizeEvent(QResizeEvent* event)
{
    DECL_TRACER("TQMap::resizeEvent(QResizeEvent* event)");

    QWidget::resizeEvent(event);
    mMapView->resize(event->size());
    mMapView->setCenterOn(QPointF(mCenterLon, mCenterLat));
}

QString TQMap::getTileCache()
{
    DECL_TRACER("TQMap::getTileCache()");

    char *home = getenv("HOME");

    if (!home)
        return QString(".tiles");

    QString p(home);
    p += "/.tiles";
    return p;
}

QString TQMap::getProvider(PROVIDER_t prov)
{
    DECL_TRACER("TQMap::getProvider(PROVIDER_t prov)");

    switch(prov)
    {
        case PROV_GOOGLE_MAP:   return ProviderGoogleMap;
        case PROV_GOOGLE_SAT:   return ProviderGoogleSat;
        case PROV_GOOGLE_LAND:  return ProviderGoogleLand;
        case PROV_BING_SAT:     return ProviderBingSat;
        case PROV_BING_ROADS:   return ProviderBingRoads;
        case PROV_OSTREETMAP:   return ProviderOsmMap;
    }
}