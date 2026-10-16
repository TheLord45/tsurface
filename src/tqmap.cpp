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
#include <QEventLoop>
#include <QFile>
#include <QImage>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QSaveFile>
#include <QtMath>

#include "tqmap.h"
#include "tconfig.h"
#include "terror.h"

namespace
{
    constexpr int TileSize = 256;
    constexpr double MaxMercatorLatitude = 85.05112878;
}

TQMap::TQMap(QObject *parent)
    : QObject(parent),
      network_(this)
{
    DECL_TRACER("TQMap::TQMap(QObject *parent)");

    cacheDirectory_ = getTileCache();
    init();
}

TQMap::TQMap(const QString &cacheDirectory, QObject *parent)
    : QObject(parent),
      cacheDirectory_(cacheDirectory),
      network_(this)
{
    DECL_TRACER("TQMap::TQMap(const QString &cacheDirectory, QObject *parent)");

    init();
}

void TQMap::init()
{
    DECL_TRACER("TQMap::init()");

    QDir().mkpath(cacheDirectory_);
    googleUrlTemplate_ = "https://mt0.google.com/vt/lyrs=m&x={x}&y={y}&z={z}";
}

void TQMap::setProvider(Provider provider)
{
    DECL_TRACER("TQMap::setProvider(Provider provider)");

    provider_ = provider;
}

void TQMap::setGoogleUrlTemplate(const QString &urlTemplate)
{
    DECL_TRACER("TQMap::setGoogleUrlTemplate(const QString &urlTemplate)");

    googleUrlTemplate_ = urlTemplate;
}

QPixmap TQMap::map(double latitude, double longitude, int zoom, const QSize &pixelSize, QString *errorMessage)
{
    DECL_TRACER("QPixmap TQMap::map(double latitude, double longitude, int zoom, const QSize &pixelSize, QString *errorMessage)");

    if (errorMessage)
        errorMessage->clear();

    if (zoom < 0 || zoom > 22 || pixelSize.isEmpty())
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Invalid zoom level or image size.");

        MSG_ERROR("Invalid zoom level or image size! [Zoom: " << zoom << ", Size: " << pixelSize.width() << "x" << pixelSize.height() << "]");

        return {};
    }

    latitude = qBound(-MaxMercatorLatitude, latitude, MaxMercatorLatitude);
    longitude = qBound(-180.0, longitude, 180.0);

    const double centerX = longitudeToPixelX(longitude, zoom);
    const double centerY = latitudeToPixelY(latitude, zoom);

    const double left = centerX - pixelSize.width() / 2.0;
    const double top = centerY - pixelSize.height() / 2.0;

    const int firstTileX = qFloor(left / TileSize);
    const int firstTileY = qFloor(top / TileSize);
    const int lastTileX = qFloor((left + pixelSize.width() - 1) / TileSize);
    const int lastTileY = qFloor((top + pixelSize.height() - 1) / TileSize);

    QImage result(pixelSize, QImage::Format_ARGB32_Premultiplied);
    result.fill(Qt::lightGray);

    QPainter painter(&result);
    const int tilesPerAxis = 1 << zoom;

    for (int tileY = firstTileY; tileY <= lastTileY; ++tileY)
    {
        if (tileY < 0 || tileY >= tilesPerAxis)
            continue;

        for (int tileX = firstTileX; tileX <= lastTileX; ++tileX)
        {
            const int wrappedX = wrapTileX(tileX, zoom);

            QString tileError;
            const QImage image = tile(wrappedX, tileY, zoom, &tileError);

            if (image.isNull())
            {
                if (errorMessage && errorMessage->isEmpty())
                    *errorMessage = tileError;

                continue;
            }

            const double drawX = tileX * TileSize - left;
            const double drawY = tileY * TileSize - top;

            painter.drawImage(QPointF(drawX, drawY), image);
        }
    }

    painter.end();
    return QPixmap::fromImage(result);
}

QImage TQMap::tile(int x, int y, int zoom, QString *errorMessage)
{
    DECL_TRACER("TQMap::tile(int x, int y, int zoom, QString *errorMessage)");

    const QString fileName = cacheFileName(x, y, zoom);

    QFile cachedFile(fileName);

    if (cachedFile.open(QIODevice::ReadOnly))
    {
        QImage image;
        image.loadFromData(cachedFile.readAll());

        if (!image.isNull())
            return image;

        cachedFile.close();
        QFile::remove(fileName);
    }

    const QString urlText = tileUrl(x, y, zoom);

    if (urlText.isEmpty())
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("No tile URL has been configured.");

        MSG_ERROR("No tile URL has been configured.")
        return {};
    }

    const QImage image = downloadTile(QUrl(urlText), errorMessage);

    if (image.isNull())
        return {};

    QDir().mkpath(QFileInfo(fileName).absolutePath());
    QSaveFile output(fileName);

    if (output.open(QIODevice::WriteOnly))
    {
        image.save(&output, "PNG");
        output.commit();
    }

    return image;
}

QImage TQMap::downloadTile(const QUrl &url, QString *errorMessage)
{
    DECL_TRACER("TQMap::downloadTile(const QUrl &url, QString *errorMessage)");

    QNetworkRequest request(url);

    request.setHeader(
        QNetworkRequest::UserAgentHeader,
        QStringLiteral("TheoSys TSurface/%1").arg(VERSION_STRING()));

    MSG_DEBUG("Using version header: TheoSys TSurface/" << VERSION_STRING());
    QNetworkReply *reply = network_.get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Tile download failed: %1").arg(reply->errorString());

        MSG_ERROR("Tile download failed: " << reply->errorString().toStdString());
        reply->deleteLater();
        return {};
    }

    const QByteArray data = reply->readAll();
    reply->deleteLater();

    QImage image;

    if (!image.loadFromData(data))
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Downloaded tile is not a valid image.");

        MSG_ERROR("Downloaded tile is not a valid image.");
        return {};
    }

    return image;
}

QString TQMap::tileUrl(int x, int y, int zoom) const
{
    DECL_TRACER("TQMap::tileUrl(int x, int y, int zoom) const");

    QString url;

    switch (provider_)
    {
        case Provider::OpenStreetMap:
            url = QStringLiteral("https://tile.openstreetmap.org/{z}/{x}/{y}.png");
        break;

        case Provider::Google:
            url = googleUrlTemplate_;
        break;
    }

    url.replace(QStringLiteral("{x}"), QString::number(x));
    url.replace(QStringLiteral("{y}"), QString::number(y));
    url.replace(QStringLiteral("{z}"), QString::number(zoom));

    return url;
}

QString TQMap::cacheFileName(int x, int y, int zoom) const
{
    const QString providerName =
        provider_ == Provider::OpenStreetMap
            ? QStringLiteral("openstreetmap")
            : QStringLiteral("google");

    return QDir(cacheDirectory_).filePath(
        QStringLiteral("%1/%2/%3/%4.png")
            .arg(providerName)
            .arg(zoom)
            .arg(x)
            .arg(y));
}

double TQMap::longitudeToPixelX(double longitude, int zoom)
{
    const double mapSize = TileSize * static_cast<double>(1 << zoom);
    return (longitude + 180.0) / 360.0 * mapSize;
}

double TQMap::latitudeToPixelY(double latitude, int zoom)
{
    const double mapSize = TileSize * static_cast<double>(1 << zoom);
    const double latitudeRadians = qDegreesToRadians(latitude);

    const double mercator =
        qLn(qTan(M_PI_4 + latitudeRadians / 2.0));

    return (1.0 - mercator / M_PI) / 2.0 * mapSize;
}

int TQMap::wrapTileX(int x, int zoom)
{
    const int tileCount = 1 << zoom;
    return ((x % tileCount) + tileCount) % tileCount;
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