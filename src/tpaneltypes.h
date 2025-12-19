/*
 * Copyright (C) 2025 by Andreas Theofilu <andreas@theosys.at>
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
#ifndef __TPANELTYPE_H__
#define __TPANELTYPE_H__

#include <QSize>

#include <string>
#include <map>

class TPanelType
{
    public:
        TPanelType();
        ~TPanelType();

        typedef struct RESOLUTION_t
        {
            std::string image;
            std::string description;
            std::vector<QSize> size;

            void clear() { image.clear(); description.clear(); size.clear(); }
        }RESOLUTION_t;

        std::vector<QSize> getResolutions(const std::string& name);
        std::vector<QSize> getResolutions(const QString& name) { return getResolutions(name.toStdString()); }
        std::string getImageName(const std::string& name);
        QString getImageName(const QString& name) { return QString::fromStdString(getImageName(name.toStdString())); }
        std::string getDescription(const std::string&name);
        QString getDescription(const QString& name) { return QString::fromStdString(getDescription(name.toStdString())); }

    private:
        std::map<std::string, RESOLUTION_t> mResolutions;
};

#endif

