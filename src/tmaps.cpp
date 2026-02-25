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
#include <vector>
#include <filesystem>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

#include "tmaps.h"
#include "tpagehandler.h"
#include "terror.h"
#include "tmisc.h"

TMaps *TMaps::mCurrent{nullptr};

using std::vector;
using namespace Maps;
namespace fs = std::filesystem;

TMaps::TMaps()
{
    DECL_TRACER("TMaps::TMaps()");
}

TMaps& TMaps::Current()
{
    DECL_TRACER("TMaps::Current()");

    if (!mCurrent)
        mCurrent = new TMaps;

    return *mCurrent;
}

void TMaps::addButton(Maps::MAP_TYPE type, int port, int channel, int bt, const QString& btName, int pgnum)
{
    DECL_TRACER("TMaps::addButton(Maps::MAP_TYPE type, int port, int channel, int bt, const QString& btName, int pgnum)");

    if (!bt || btName.isEmpty() || !pgnum)
    {
        MSG_ERROR("Missing one or more parameters!");
        return;
    }

    if (buttonExist(type, port, channel, pgnum, &bt))
        return;

    Page::PAGE_t *page = TPageHandler::Current().getPage(pgnum);

    if (!page || page->pageID <= 0)
    {
        MSG_ERROR("Couln't find page " << pgnum << "!");
        return;
    }

    MAP_T map;
    map.bt = bt;
    map.c = channel;
    map.p = port;
    map.pg = pgnum;
    map.bn = btName;
    map.pn = page->name;

    switch(type)
    {
        case TYPE_AM: mMap.map_am.push_back(map); break;
        case TYPE_CM: mMap.map_cm.push_back(map); break;
        case TYPE_LM: mMap.map_lm.push_back(map); break;
    }
}

bool TMaps::buttonExist(Maps::MAP_TYPE type, int port, int channel, int pgnum, int *btIndex)
{
    DECL_TRACER("TMaps::buttonExist(Maps::MAP_TYPE type, int port, int channel, int pgnum, int *btIndex)");

    if (btIndex)
        *btIndex = -1;

    vector<MAP_T> localMap;

    switch (type)
    {
        case TYPE_AM:   localMap = mMap.map_am; break;
        case TYPE_CM:   localMap = mMap.map_cm; break;
        case TYPE_LM:   localMap = mMap.map_lm; break;
    }

    if (localMap.empty())
    {
        MSG_WARNING("The internal list of elements is empty!")
        return false;
    }

    vector<MAP_T>::iterator mapIter;

    for (mapIter = localMap.begin(); mapIter != localMap.end(); ++mapIter)
    {
        if (mapIter->p == port && mapIter->c == channel && mapIter->pg == pgnum)
        {
            if (btIndex)
                *btIndex = mapIter->bt;

            return true;
        }
    }

    return false;
}

void TMaps::addBitmap(const QString& file, int id, int page, int bt, int st)
{
    DECL_TRACER("TMaps::addBitmap(const QString& file, int id, int page, int bt, int st)");

    // Check if entry exists already
    vector<MAP_BM_T>::iterator iter;

    for (iter = mMap.map_bm.begin(); iter != mMap.map_bm.end(); ++iter)
    {
        if (iter->i == file && iter->bt == bt && iter->pg == page && iter->st == st)
            return;
    }

    MAP_BM_T bm;

    if (id == 1)    // resource?
    {
        bm.i = basename(file);
        bm.id = id;
        mMap.map_bm.push_back(bm);
        return;
    }

    if (!id && !page && !bt && !st)
    {
        bm.i = basename(file);
        mMap.map_bm.push_back(bm);

        QFile f(file);
        QString target = mPathTemporary + "/images/" + bm.i;

        if (!f.copy(target))
        {
            MSG_ERROR("Couldn't copy file " << file.toStdString() << " to " << target.toStdString());
            MSG_ERROR("File copy error: " << f.errorString().toStdString());
        }

        return;
    }

    Page::PAGE_t *pg = TPageHandler::Current().getPage(page);

    if (!pg || pg->pageID <= 0)
    {
        MSG_ERROR("Couln't find page " << page << "!");
        return;
    }

    MAP_T button = getButtonByNumber(bt, page);

    if (button.bt <= 0)
    {
        MSG_ERROR("Couldn't find Button " << bt << " on page " << page << "!");
        return;
    }

    bm.i = basename(file);
    bm.rt = 0;
    bm.pg = page;
    bm.bt = bt;
    bm.st = st;
    bm.pn = pg->name;
    bm.bn = button.bn;
    mMap.map_bm.push_back(bm);
}

void TMaps::removeBitmap(const QString& file)
{
    DECL_TRACER("TMaps::removeBitmap(const QString& file)");

    MSG_DEBUG("Try to remove file \"" << file.toStdString() << "\" ...");
    bool rem = false;
    vector<MAP_BM_T>::iterator iter;

    do
    {
        rem = false;

        for (iter = mMap.map_bm.begin(); iter != mMap.map_bm.end(); ++iter)
        {
            if (iter->i == file)
            {
                QFile f(mPathTemporary + "/images/" + file);

                if (!f.remove())
                {
                    MSG_ERROR("Error removing file \"" << f.fileName().toStdString() << "\": " << f.errorString().toStdString());
                }

                mMap.map_bm.erase(iter);
                rem = true;
                break;
            }
        }
    }
    while (rem);
}

void TMaps::renameBitmap(const QString& ori, const QString& tgt)
{
    DECL_TRACER("TMaps::renameBitmap(const QString& ori, const QString& tgt)");

    vector<MAP_BM_T>::iterator iter;

    for (iter = mMap.map_bm.begin(); iter != mMap.map_bm.end(); ++iter)
    {
        if (iter->i == ori)
            iter->i = tgt;
    }

    QFile f(mPathTemporary + "/images/" + ori);

    if (!f.rename(mPathTemporary + "/images/" + tgt))
    {
        MSG_ERROR("Error renaming file \"" << f.fileName().toStdString() << "\": " << f.errorString().toStdString());
    }
}

bool TMaps::isBitmapUsed(const QString& file)
{
    DECL_TRACER("TMaps::isBitmapUsed(const QString& file)");

    vector<MAP_BM_T>::iterator iter;

    for (iter = mMap.map_bm.begin(); iter != mMap.map_bm.end(); ++iter)
    {
        if (iter->i == file)
        {
            if (iter->pg > 0 || iter->bt > 0)
                return true;
        }
    }

    return false;
}

void TMaps::addDynamicResource(const Maps::MAP_T& res)
{
    DECL_TRACER("TMaps::addDynamicResource(const Maps::MAP_T& res)");

    if (res.bt <= 0 || res.pg <= 0)
        return;

    vector<MAP_T>::iterator iter;

    for (iter = mMap.map_am.begin(); iter != mMap.map_am.end(); ++iter)
    {
        if (iter->bt == res.bt && iter->pg == res.pg)
        {
            iter->bn = res.bn;
            iter->c = res.c;
            iter->p = res.p;
            iter->pn = res.pn;
            return;
        }
    }

    mMap.map_am.push_back(res);
}

bool TMaps::addSound(const QString& file)
{
    DECL_TRACER("TMaps::addSound(const QString& file)");

    if (file.isEmpty())
        return false;

    QFile f(file);

    if (!f.exists())
    {
        MSG_ERROR("File \"" << file.toStdString() << "\" doesn't exist!");
        return false;
    }

    if (!f.copy(mPathTemporary + "/sounds/" + basename(file)))
    {
        MSG_ERROR("Error copying file \"" << file.toStdString() << "\" to temporary folder!");
        return false;
    }

    mMap.map_sm.push_back(basename(file));
    return true;
}

bool TMaps::removeSound(const QString& file)
{
    DECL_TRACER("TMaps::removeSound(const QString& file)");

    vector<QString>::iterator iter;

    for (iter = mMap.map_sm.begin(); iter != mMap.map_sm.end(); ++iter)
    {
        if (*iter == file)
        {
            QFile f(mPathTemporary + "/sounds/" + file);

            if (!f.remove())
            {
                MSG_ERROR("Couldn't remove file \"" << file.toStdString() << "\"!");
                return false;
            }

            mMap.map_sm.erase(iter);
            return true;
        }
    }

    return false;
}

MAP_T TMaps::getButtonByNumber(int num, int page)
{
    DECL_TRACER("TMaps::getButtonByNumber(int num)");

    vector<MAP_T>::iterator iter;

    for (iter = mMap.map_cm.begin(); iter != mMap.map_cm.end(); ++iter)
    {
        if (iter->bt == num && iter->pg == page)
            return *iter;
    }

    return MAP_T();
}

QStringList TMaps::getAllImageFiles()
{
    DECL_TRACER("TMaps::getAllImageFiles()");

    if (mMap.map_bm.empty())
        return QStringList();

    QStringList list;
    vector<MAP_BM_T>::iterator iter;

    for (iter = mMap.map_bm.begin(); iter != mMap.map_bm.end(); ++iter)
    {
        // Check if image is already in list
        if (!list.empty())
        {
            QStringList::Iterator listIter;
            bool haveImage = false;

            for (listIter = list.begin(); listIter != list.end(); ++listIter)
            {
                if (*listIter == iter->i)
                {
                    haveImage = true;
                    break;
                }
            }

            if (haveImage)
                continue;
        }

        list.append(iter->i);
    }

    return list;
}

QStringList TMaps::getAllSoundFiles()
{
    DECL_TRACER("TMaps::getAllSoundFiles()");

    if (mMap.map_sm.empty())
        return QStringList();

    vector<QString>::iterator iter;
    QStringList list;

    for (iter = mMap.map_sm.begin(); iter != mMap.map_sm.end(); ++iter)
        list.append(*iter);

    return list;
}

bool TMaps::readMaps(const QString& path, const QString& file)
{
    DECL_TRACER("TMaps::readMaps(const QString& path, const QString& file)");

    if (mInitialized)
        return true;

    QString f = path + "/" + file;

    if (!fs::is_regular_file(f.toStdString()))
    {
        MSG_ERROR("The file " << f.toStdString() << " doesn't exist or is not a regular file!");
        return false;
    }

    QFile map(f);

    if (!map.open(QIODevice::ReadOnly))
    {
        MSG_ERROR("Error reading file \"" << path.toStdString() << "\": " << map.errorString().toStdString());
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(map.readAll());
    map.close();
    QJsonObject root = doc.object();

    for (int x = 0; x < 3; ++x)
    {
        QJsonArray xm;

        switch (x)
        {
            case 0: xm = root.value("cm").toArray(); break;
            case 1: xm = root.value("am").toArray(); break;
            case 2: xm = root.value("lm").toArray(); break;
        }

        for (int i = 0; i < xm.count(); ++i)
        {
            QJsonObject me = xm[i].toObject();
            MAP_T m;
            m.p = me.value("p").toInt(0);
            m.c = me.value("c").toInt(0);
            m.pg = me.value("pg").toInt(0);
            m.bt = me.value("bt").toInt(0);
            m.pn = me.value("pn").toString();
            m.bn = me.value("bn").toString();
            m.ax = me.value("ax").toInt(0);

            switch(x)
            {
                case 0: mMap.map_cm.push_back(m); break;
                case 1: mMap.map_am.push_back(m); break;
                case 2: mMap.map_lm.push_back(m); break;
            }
        }
    }

    // Resources
    QJsonArray bm = root.value("bm").toArray();

    for (int i = 0; i < bm.count(); ++i)
    {
        QJsonObject me = bm[i].toObject();
        MAP_BM_T m;
        m.i = me.value("i").toString();
        m.id = me.value("id").toInt(0);
        m.rt = me.value("rt").toInt(0);
        m.pg = me.value("pg").toInt(0);
        m.bt = me.value("bt").toInt(0);
        m.st = me.value("st").toInt(0);
        m.pn = me.value("pn").toString();
        m.bn = me.value("bn").toString();
        m.rc = me.value("rc").toInt(1);
        mMap.map_bm.push_back(m);
    }

    // Sound files
    QJsonArray sm = root.value("sm").toArray();

    for (int i = 0; i < sm.count(); ++i)
        mMap.map_sm.push_back(sm[i].toString());

    // TODO: Add system resources

    // TODO: Add evpf data

    mInitialized = true;
    return true;
}

bool TMaps::writeMaps(const QString& path, const QString& file)
{
    DECL_TRACER("TMaps::writeMaps(const QString& path, const QString& file)");

    QJsonObject root;
    QJsonArray cm, am, lm;

    vector<MAP_T>::iterator iter;

    if (!mMap.map_cm.empty())
    {
        for (iter = mMap.map_cm.begin(); iter != mMap.map_cm.end(); ++iter)
        {
            QJsonObject me;
            me.insert("p", iter->p);
            me.insert("c", iter->c);
            me.insert("pg", iter->pg);
            me.insert("bt", iter->bt);
            me.insert("pn", iter->pn);
            me.insert("bn", iter->bn);
            cm.append(me);
        }
    }

    root.insert("cm", cm);

    if (!mMap.map_am.empty())
    {
        for (iter = mMap.map_am.begin(); iter != mMap.map_am.end(); ++iter)
        {
            QJsonObject me;
            me.insert("p", iter->p);
            me.insert("c", iter->c);
            me.insert("pg", iter->pg);
            me.insert("bt", iter->bt);
            me.insert("pn", iter->pn);
            me.insert("bn", iter->bn);
            am.append(me);
        }
    }

    root.insert("am", am);

    if (!mMap.map_lm.empty())
    {
        for (iter = mMap.map_lm.begin(); iter != mMap.map_lm.end(); ++iter)
        {
            QJsonObject me;
            me.insert("p", iter->p);
            me.insert("c", iter->c);
            me.insert("ax", iter->ax);
            me.insert("pg", iter->pg);
            me.insert("bt", iter->bt);
            me.insert("pn", iter->pn);
            me.insert("bn", iter->bn);
            lm.append(me);
        }
    }

    root.insert("lm", lm);
    // Resources
    QJsonArray im;

    if (!mMap.map_bm.empty())
    {
        vector<MAP_BM_T>::iterator bmIter;

        for (bmIter = mMap.map_bm.begin(); bmIter != mMap.map_bm.end(); ++bmIter)
        {
            QJsonObject me;

            if (bmIter->bt == 0 && bmIter->id == 0 && bmIter->pg == 0 && bmIter->st == 0)
            {
                me.insert("i", bmIter->i);
            }
            else if (bmIter->id == 1)
            {
                me.insert("i", bmIter->i);
                me.insert("id", bmIter->id);
            }
            else
            {
                me.insert("i", bmIter->i);
                me.insert("rt", bmIter->rt);
                me.insert("pg", bmIter->pg);
                me.insert("bt", bmIter->bt);
                me.insert("st", bmIter->st);
                me.insert("pn", bmIter->pn);
                me.insert("bn", bmIter->bn);
                me.insert("rc", bmIter->rc);
            }

            MSG_DEBUG("Adding image " << bmIter->i.toStdString());
            im.append(me);
        }
    }

    root.insert("bm", im);
    // Sounds
    QJsonArray sm;

    if (!mMap.map_sm.empty())
    {
        vector<QString>::iterator smIter;

        for (smIter = mMap.map_sm.begin(); smIter != mMap.map_sm.end(); ++smIter)
            sm.append(*smIter);
    }

    root.insert("sm", sm);

    // TODO: Add code to write system resources (do we need them?)

    // TODO: Add code to write G5 evpf structure (what is it for?)

    // Write JSON to file
    QJsonDocument doc;
    doc.setObject(root);
    QString metaFile = path + "/" + file;
    QFile f(metaFile);

    if(!f.open(QIODevice::WriteOnly))
    {
        MSG_ERROR("Error writing file \"" << metaFile.toStdString() << "\": " << f.errorString().toStdString());
        return false;
    }

    f.write(doc.toJson(QJsonDocument::Indented));
    f.close();
    return true;
}
