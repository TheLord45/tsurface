#include "maploader.h"
#include "mapglobal.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPixmapCache>
#include <QApplication>
#include <QFileInfo>
#include <QPixmap>
#include <QDebug>
#include <QFile>
#include <QUrl>
#include <QDir>

#include "terror.h"

struct MapLoader::MapLoaderPrivate
{
    MapGlobal &settings = MapGlobal::instance();
    QNetworkAccessManager *netAccessManager;
    QVector<QNetworkReply*> replies;
    QString cachePath = "/z%1/%2/x%3/%4/y%5.png";
    QString currentUrl;
};

MapLoader::MapLoader(QObject *parent, bool sslIgnore)
    : QObject(parent),
      d(new MapLoaderPrivate),
      mSslIgnore(sslIgnore)
{
    DECL_TRACER("MapLoader::MapLoader(QObject *parent, bool sslIgnore)");

    d->netAccessManager = new QNetworkAccessManager;
    d->netAccessManager->setStrictTransportSecurityEnabled(sslIgnore ? false : true);
    QPixmapCache::setCacheLimit(100000); //100MB
}

MapLoader::~MapLoader()
{
    DECL_TRACER("MapLoader::~MapLoader()");

    foreach (QNetworkReply *reply, d->replies)
    {
        reply->disconnect();
        reply->abort();
        delete reply;
    }

    d->replies.clear();


    delete d->netAccessManager;
    delete d;
}

void MapLoader::update()
{
    foreach (QNetworkReply *reply, d->replies)
    {
        reply->disconnect();
        reply->abort();
        delete reply;
    }

    d->replies.clear();
}

void MapLoader::loadTile(const QPoint &pos)
{
    DECL_TRACER("MapLoader::loadTile(const QPoint &pos)");

    if (d->settings.providerName().isEmpty()) return;

    QString cache = createCachePath(pos);
    QPixmap pix;

    if(QPixmapCache::find(cache, &pix))
    {
        emit loaded(pos, pix);
    }

    if(loadFile(cache, pix))
    {
        emit loaded(pos, pix);
    }
    else
    {
        d->settings.calculateUrl(pos.x(), pos.y(), d->settings.zoom() - 1, d->currentUrl);
        QNetworkRequest request = QNetworkRequest(QUrl(d->currentUrl));

        if (mSslIgnore)
        {
            if (d->netAccessManager)
                d->netAccessManager->setStrictTransportSecurityEnabled(false);
#ifndef __EMSCRIPTEN__
            QSslConfiguration sslConfig;
            sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            sslConfig.setMissingCertificateIsFatal(false);
            request.setSslConfiguration(sslConfig);
#endif
        }

#ifndef __EMSCRIPTEN__
        request.setRawHeader("User-Agent", "Mozilla/5.0 (PC; U; Intel; Linux; en) AppleWebKit/420+ (KHTML, like Gecko)");
#endif
        if (!d->netAccessManager)   // should never be true! This satisfies the C++ compiler only.
        {
            MSG_ERROR("Network manager was not initialized!");
            return;
        }

        QNetworkReply *reply = d->netAccessManager->get(request);

        if (!reply)
        {
            MSG_ERROR("Got no valid reply from request for a map tile!");
            return;
        }

        d->replies.append(reply);

        reply->setProperty("id", pos);
        connect(reply, &QNetworkReply::finished, this, &MapLoader::requestFinished);
    }
}

void MapLoader::requestFinished()
{
    DECL_TRACER("MapLoader::requestFinished()");

    QNetworkReply *reply = dynamic_cast<QNetworkReply*>(sender());
    d->replies.removeOne(reply);

    if(reply->error() != QNetworkReply::NoError)
    {
        MSG_ERROR("Network reply error: " << reply->errorString().toStdString());
        return;
    }

    QByteArray data = reply->readAll();
    const QPoint pos = reply->property("id").toPoint();

    QPixmap pix;
    pix.loadFromData(data);
    emit loaded(pos, pix);
    reply->disconnect();
    // For Emscripten (WASM) this must be commented out because otherwise the
    // app stops here. The CPU usage goes to a high level and the browser stops
    // to react.
    // BUT: This is a memory leake if disabled!!
#ifndef __EMSCRIPTEN__
    delete reply;
#endif
    QString cache = createCachePath(pos);
    QPixmapCache::insert(cache, pix);
    saveFile(cache, data);
}

QString MapLoader::createCachePath(const QPoint &pos)
{
    DECL_TRACER("MapLoader::createCachePath(const QPoint &pos)");

    return d->settings.cachePathSuffix() + d->cachePath.arg(d->settings.zoom()).
            arg(pos.x()/1024).arg(pos.x()).arg(pos.y()/1024).arg(pos.y());
}

void MapLoader::saveFile(const QString &path, const QByteArray &data)
{
    DECL_TRACER("MapLoader::saveFile(const QString &path, const QByteArray &data)");


    if (d->settings.cachePath().isEmpty()) return;

    QFileInfo fInfo(d->settings.cachePath() + path);
    if (!fInfo.dir().exists())
    {
        QDir dir = fInfo.dir();
        dir.mkpath(".");
    }

    QFile file(fInfo.filePath());
    if (!file.open(QIODevice::WriteOnly)) return;

    file.write(data);
    file.close();
}

bool MapLoader::loadFile(const QString &path, QPixmap &pix)
{
    DECL_TRACER("MapLoader::loadFile(const QString &path, QPixmap &pix)");

    if (d->settings.cachePath().isEmpty()) return false;
    QFile file(d->settings.cachePath() + path);

    if (!file.exists()) return false;
    if (!file.open(QIODevice::ReadOnly)) return false;

    bool result = pix.loadFromData(file.readAll());
    file.close();

    return result;
}
