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

#include <QObject>
#include <QNetworkAccessManager>
#include <QPixmap>
#include <QString>

class TQMap : public QObject
{
    public:
        enum class Provider
        {
            OpenStreetMap,
            Google
        };

        TQMap(QObject *parent = nullptr);
        explicit TQMap(const QString &cacheDirectory, QObject *parent = nullptr);
        void setProvider(Provider provider);

        // Example:
        // https://tile.googleapis.com/v1/2dtiles/{z}/{x}/{y}?session=SESSION&key=API_KEY
        //
        // The template must contain {x}, {y}, and {z}.
        void setGoogleUrlTemplate(const QString &urlTemplate);

        QPixmap map(double latitude,
                    double longitude,
                    int zoom,
                    const QSize &pixelSize,
                    QString *errorMessage = nullptr);

    protected:
        void init();
        QString getTileCache();

    private:
        QImage tile(int x, int y, int zoom, QString *errorMessage);
        QImage downloadTile(const QUrl &url, QString *errorMessage);

        QString tileUrl(int x, int y, int zoom) const;
        QString cacheFileName(int x, int y, int zoom) const;

        static double longitudeToPixelX(double longitude, int zoom);
        static double latitudeToPixelY(double latitude, int zoom);
        static int wrapTileX(int x, int zoom);

        Provider provider_ = Provider::OpenStreetMap;
        QString cacheDirectory_;
        QString googleUrlTemplate_;
        QNetworkAccessManager network_;
};

#endif // TQMAP_H
