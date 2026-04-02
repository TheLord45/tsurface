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
#include <QPixmap>
#include <QFile>
#include <QStringConverter>

#include <sstream>
#include <iomanip>
#include <pwd.h>
#include <unistd.h>

#include "tmisc.h"
#include "terror.h"
#include "tobjecthandler.h"
#include "tpagehandler.h"

using std::stringstream;
using std::hex;
using std::setw;
using std::setfill;
using namespace ObjHandler;

QString basename(const QString& path)
{
    qsizetype pos = path.lastIndexOf('/');

    if (pos < 0)
        return path;

    return path.mid(pos + 1);
}

QString pathname(const QString& path)
{
    qsizetype pos = path.lastIndexOf('/');

    if (pos < 0)
        return ".";

    return path.left(pos);
}

int getObjectID(const QString& name, const QString& hint)
{
    MSG_DEBUG("Searching for object " << name.toStdString() << " with hint " << hint.toStdString());

    if (!name.contains("_"))
        return -1;

    if (!hint.isEmpty() && !name.startsWith(hint))
        return -1;

    QString s = name;
    qsizetype pos = s.lastIndexOf('_');
    return s.remove(0, pos+1).toInt();
}

QString wcharToUnicodeString(const QChar& ch)
{
    char16_t letter = ch.unicode();
    stringstream ss;
    ss << setw(4) << setfill('0') << hex << letter;
    return QString::fromStdString(ss.str());
}

QPixmap makePixmapFromString(const QString& str, int width)
{
    QString byte;
    int pos = 1;
    int x = 0, y = 0;
    QPixmap px;
    QImage img;

    for (QChar c : str)
    {
        if ((pos % 2) == 0)
        {
            uint pixel = byte.toUInt(nullptr, 16);
            byte.clear();
            img.setPixel(x, y, pixel);
            x++;

            if (x >= width)
            {
                x = 0;
                y++;
            }
            else
                x++;
        }

        byte.append(c);
    }

    px = QPixmap::fromImage(img);
    return px;
}

STATE_TYPE getStateFromButtonType(int bt)
{
    BUTTONTYPE t = static_cast<BUTTONTYPE>(bt);
    MSG_DEBUG("Got button type: " << bt);

    switch(t)
    {
        case GENERAL:
        case MULTISTATE_GENERAL:    return STATE_BUTTON;

        case BARGRAPH:
        case MULTISTATE_BARGRAPH:   return STATE_BARGRAPH;

        case TEXT_INPUT:            return STATE_INPUT;
        case SUBPAGE_VIEW:          return STATE_SUBPAGE;

        default:
            return STATE_UNKNOWN;
    }

    return STATE_UNKNOWN;
}

STATE_TYPE getStateFromPageType(int pt)
{
    switch(pt)
    {
        case Page::PT_PAGE:     return STATE_PAGE;
        case Page::PT_POPUP:    return STATE_POPUP;
        case Page::PT_SUBPAGE:  return STATE_SUBPAGE;
    }

    return STATE_UNKNOWN;
}

QPixmap sizeImage(const QSize& size, const QString& file, QSize *ori)
{
    QPixmap pm(file);

    if (ori)
        *ori = pm.size();

    return QPixmap(pm.scaled(size, Qt::KeepAspectRatio));
}

QString convertToUTF8(const QString& filename, bool fake)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
    {
        MSG_ERROR("convertToUTF8: Cannot open file:" << filename.toStdString() << "! Error: " << file.errorString().toStdString());
        return QString();
    }

    QByteArray buffer = file.readAll();
    file.close();

    if (fake)
        return buffer;

    auto toUtf8 = QStringDecoder(QStringDecoder::Latin1);
    return toUtf8(buffer);
}

QString getStrLogLevel(int level)
{
    QString sl;

    if ((level & HLOG_ERROR) > 0)
    {
        if (!sl.isEmpty())
            sl += "|ERROR";
        else
            sl = "ERROR";
    }

    if ((level & HLOG_WARNING))
    {
        if (!sl.isEmpty())
            sl += "|WARNING";
        else
            sl = "WARNING";
    }

    if ((level & HLOG_INFO))
    {
        if (!sl.isEmpty())
            sl += "|INFO";
        else
            sl = "INFO";
    }

    if ((level & HLOG_DEBUG))
    {
        if (!sl.isEmpty())
            sl += "|DEBUG";
        else
            sl = "DEBUG";
    }

    if ((level & HLOG_TRACE))
    {
        if (!sl.isEmpty())
            sl += "|TRACE";
        else
            sl = "TRACE";
    }

    return sl;
}

int getLogLevelFromString(const QString& level)
{
    int l = 0;

    if (level.contains("ERROR", Qt::CaseInsensitive))
        l |= HLOG_ERROR;

    if (level.contains("WARNING", Qt::CaseInsensitive))
        l |= HLOG_ERROR;

    if (level.contains("INFO", Qt::CaseInsensitive))
        l |= HLOG_ERROR;

    if (level.contains("DEBUG", Qt::CaseInsensitive))
        l |= HLOG_ERROR;

    if (level.contains("TRACE", Qt::CaseInsensitive))
        l |= HLOG_ERROR;

    if (level.contains("PROTOCOL", Qt::CaseInsensitive))
        l |= HLOG_PROTOCOL;

    if (level.contains("ALL", Qt::CaseInsensitive))
        l |= HLOG_ALL;

    return l;
}

OS detectOS()
{
#if defined(__APPLE__) && defined(__MACH__)
    return OS::MacOS;
#elif defined(__linux__)
    return OS::Linux;
#else
    return OS::Unknown;
#endif
}

/**
 * @brief getHomeDir
 * This method tries to find the home directory on a *NIX system. If there is
 * the environment variable @b HOME set, it takes the content from it and
 * returns it.
 * If the environment varialbe is not available, it looks in the file
 * /usr/passwd. There the current user should have been defined and there
 * should be a home directory. It retrieves the user ID and tries to find the
 * entry for the user. If it was successfully it returns the path which was
 * found.
 * If it can't find the user it throws an exception.
 *
 * @return The path of the home directory. In case the home directory was not
 * found, it throws an exception.
 */
QString getHomeDir()
{
    QString home = qEnvironmentVariable("HOME");

    if (!home.isEmpty())
        return home;

    struct passwd* pw = getpwuid(getuid());

    if (pw)
        return QString(pw->pw_dir);

    throw std::runtime_error("Cannot determine home directory");
}
