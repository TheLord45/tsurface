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
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QFile>
#include <QUrl>

#include "tdownloader.h"
#include "terror.h"

TDownloader::TDownloader(QObject *parent)
{
    DECL_TRACER("TDownloader::TDownloader(QObject *parent)");

    mManager = new QNetworkAccessManager(parent);
    connect(mManager, &QNetworkAccessManager::finished, this, &TDownloader::onResult);
}

TDownloader::~TDownloader()
{
    DECL_TRACER("TDownloader::~TDownloader()");

    if (!mManager)
        return;

    disconnect(mManager, &QNetworkAccessManager::finished, this, &TDownloader::onResult);
    delete mManager;
}
void TDownloader::getData(const QUrl& url)
{
    DECL_TRACER("TDownloader::getData(const QUrl& url)");

    MSG_DEBUG("Trying to load from " << url.toString().toStdString());

    if (!url.isValid())
    {
        MSG_ERROR("Invalid URL!");
        emit onError(url.errorString());
        return;
    }

    QNetworkRequest request;
    request.setUrl(url);
    mManager->get(request);
}

void TDownloader::onResult(QNetworkReply *reply)
{
    DECL_TRACER("TDownloader::onResult(QNetworkReply *reply)");

    if(reply->error())
    {
        MSG_ERROR(reply->errorString().toStdString());
        emit onError("Download failed!<br>" + reply->errorString());
    }
    else
    {
        mBuffer = reply->readAll();
        MSG_DEBUG("Downloading is completed");
        emit onReady();
    }
}
