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
#ifndef TMAPS_H
#define TMAPS_H

#include <QString>

namespace Maps
{
    typedef enum MAP_TYPE
    {
        TYPE_CM = 1,        // ON / OFF
        TYPE_AM,            // TXT, ...
        TYPE_LM             // Bargraphs
    }MAP_TYPE;

    typedef struct MAP_T
    {
        int p{0};           // port number
        int c{0};           // channel number
        int ax{0};          // Joystick? (Couldn't find out the real meaning)
        int pg{0};          // page number
        int bt{0};          // button number
        QString pn;         // page name
        QString bn;         // button name
    }MAP_T;

    // Images
    typedef struct MAP_BM_T
    {
        QString i;          // image file name
        QString src;        // Internally used to hold the source of the image
        int id{0};          // 1 = Resource (camera, ...)
        int rt{0};          // ? always 0
        int pg{0};          // page number
        int bt{0};          // button number
        int st{0};          // button instance
        int sl{0};          // ? G4 only?
        QString pn;         // page name
        QString bn;         // button name
        int rc{0};          // ? Only G5!
    }MAP_BM_T;

    typedef struct MAP_PM_T     // Only TP4
    {
        int a{0};
        QString t;          // Group name
        int pg{0};          // page number
        int bt{0};          // button number
        QString pn;         // page name
        QString bn;         // button name
    }MAP_PM_T;

    typedef struct MAP_EVPF_T   // Only G5
    {
        int a{0};
        QString t;          // Group name
        int pg{0};          // page number
        int bt{0};          // button number
        QString ev;
        int ai{0};
    }MAP_EVPF_T;

    typedef struct MAPS_T
    {
        std::vector<MAP_T> map_cm;
        std::vector<MAP_T> map_am;
        std::vector<MAP_T> map_lm;
        std::vector<MAP_BM_T> map_bm;       // Images
        std::vector<QString> map_sm;        // sound file names
        std::vector<MAP_T> map_strm;        // System resources
        std::vector<MAP_PM_T> map_pm;       // Button -> text
        std::vector<MAP_EVPF_T> map_evpf;   // Only G5
    }MAPS_T;
};

class TMaps
{
    public:
        TMaps();
        static TMaps& Current();

        void addButton(Maps::MAP_TYPE type, int port, int channel, int bt, const QString& btName, int pgnum);
        void addBitmap(const QString& file, int id, int page, int bt, int st);
        void removeBitmap(const QString& file);
        void renameBitmap(const QString& ori, const QString& tgt);
        bool isBitmapUsed(const QString& file);
        void addDynamicResource(const Maps::MAP_T& bm);
        void addSound(const QString& file);
        Maps::MAP_T getButtonByNumber(int num, int page);
        QStringList getAllImageFiles();
        void setPathTemporary(const QString& path) { mPathTemporary = path; }

        bool readMaps(const QString& path, const QString& file);
        bool writeMaps(const QString& path, const QString& file);

    protected:
        bool buttonExist(Maps::MAP_TYPE type, int port, int channel, int pgnum, int *btIndex=nullptr);

    private:
        static TMaps *mCurrent;
        Maps::MAPS_T mMap;
        bool mInitialized{false};
        QString mPathTemporary;
};

#endif // TMAPS_H
