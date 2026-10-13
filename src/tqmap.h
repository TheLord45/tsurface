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

#ifndef TQMAP_H
#define TQMAP_H

#include <QWidget>

class MapView;
class QResizeEvent;
class MapItem;
class QPointF;

class TQMap : public QWidget
{
    Q_OBJECT

    public:
        typedef enum
        {
            PROV_GOOGLE_MAP,    // Google: normal map
            PROV_GOOGLE_SAT,    // Google: Satellite map
            PROV_GOOGLE_LAND,   // Google: Land
            PROV_BING_SAT,      // Bing: Satellite
            PROV_BING_ROADS,    // Bing: Road map
            PROV_OSTREETMAP     // Open Streetmap
        }PROVIDER_t;

        TQMap(QWidget *parent);
        ~TQMap();

        void drawMap(const QSize& size, double lat, double lon, int zoom, bool sslIgnore=false);
        void setProvider(PROVIDER_t prov);

    protected:
        void resizeEvent(QResizeEvent *event);

    private:
        QString getTileCache();
        QString getProvider(PROVIDER_t prov);

        QWidget *mParent{nullptr};
        MapView *mMapView{nullptr};
        double mCenterLat{0.0};
        double mCenterLon{0.0};
        double mMapWidth{0.0};
        double mMapHeight{0.0};
        int mZoomLevel{0};
        PROVIDER_t mProvider{PROV_GOOGLE_MAP};
};

#endif // TQMAP_H
