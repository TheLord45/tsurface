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

#include <filesystem>
//#include <cjson/cJSON.h>
//#include <archive.h>

#include "tsurfacereader.h"
#include "terror.h"

namespace fs = std::filesystem;
using std::string;

TSurfaceReader::TSurfaceReader(const string& file)
    : mFile(file)
{
    DECL_TRACER("TSurfaceReader::TSurfaceReader(const string& file)");
}

TSurfaceReader::~TSurfaceReader()
{
    DECL_TRACER("TSurfaceReader::~TSurfaceReader()");
}

bool TSurfaceReader::dismantleFile()
{
    DECL_TRACER("TSurfaceReader::dismantleFile()");

    if (mFile.empty() || !fs::is_regular_file(mFile))
    {
        MSG_ERROR("File " << mFile << " doesn't exist or is not readable!");
        return false;
    }
}
