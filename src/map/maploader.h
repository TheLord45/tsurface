#pragma once

#include <QObject>

class MapLoader : public QObject
{
    Q_OBJECT
public:
    explicit MapLoader(QObject *parent = Q_NULLPTR, bool sslIgnore = false);
    ~MapLoader();

    void setSslIgnore(bool b) { mSslIgnore = b; }

signals:
    void loaded(const QPoint &pos, const QPixmap &pix);

public slots:
    void update();
    void loadTile(const QPoint &pos);

private:
    void requestFinished();
    QString createCachePath(const QPoint &pos);

    void saveFile(const QString &path, const QByteArray &data);
    bool loadFile(const QString &path, QPixmap &pix);

    struct MapLoaderPrivate;
    MapLoaderPrivate * const d;
    bool mSslIgnore{false};
};
