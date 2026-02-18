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
#include <filesystem>
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>

#include <QStringList>
#include <QDirIterator>

#include "tsurfacewriter.h"
#include "tconfig.h"
#include "terror.h"
#include "tmisc.h"

namespace fs = std::filesystem;

TSurfaceWriter::TSurfaceWriter(const QString& tmpPath, const QString& target)
{
    DECL_TRACER("TSurfaceWriter::TSurfaceWriter(const QString& tmpPath, const QString& target)");

    if (!fs::is_directory(tmpPath.toStdString()))
    {
        MSG_ERROR("Invalid source directory " << tmpPath.toStdString());
        mHaveError = true;
        return;
    }

    if (target.isEmpty())
    {
        MSG_ERROR("No target file given!");
        mHaveError = true;
        return;
    }

    fs::path oldPath(fs::current_path());
    fs::current_path(tmpPath.toStdString());
    // Here we iterate through the directory structure
    QStringList list;
    QDirIterator it(".", QDirIterator::Subdirectories);

    while(it.hasNext())
    {
        QString entry = it.next();

        if (entry.endsWith(".") || entry.endsWith(".."))
            continue;

        list.append(entry);
    }

    if (TConfig::Current().getCreateBackup() && fs::exists(target.toStdString()))
    {
        QString t;
        QString file = basename(target);
        QString path = TConfig::Current().getFilesBackups();
        bool haveName = false;
        int num = 0;

        if (path.isEmpty() || !fs::is_directory(path.toStdString()))
            path = pathname(target);

        do
        {
            if (num == 0)
            {
                if (TConfig::Current().getUsePostfix())
                    t = path + "/copy-" + file;
                else
                    t = path + "/~" + file;
            }
            else
            {
                if (TConfig::Current().getUsePostfix())
                    t = QString("%1/copy-%2_%3").arg(path).arg(num).arg(file);
                else
                    t = QString("%1/~%2_%3").arg(path).arg(num).arg(file);
            }

            if (!fs::exists(t.toStdString()))
                haveName = true;
            else
                num++;
        }
        while (!haveName);

        fs::rename(target.toStdString(), t.toStdString());
    }

    archiveFile(list, target);
    fs::current_path(oldPath);
}

void TSurfaceWriter::archiveFile(const QStringList& files, const QString& target)
{
    DECL_TRACER("TSurfaceWriter::archiveFile(const QStringList& files, const QString& target)");

    struct archive *a;
    struct archive_entry *entry;
    struct stat st;
    char buff[8192];
    int len;
    int fd;

    a = archive_write_new();
    archive_write_add_filter_gzip(a);
    archive_write_set_format_pax_restricted(a);
    archive_write_open_filename(a, target.toUtf8());

    QStringList::ConstIterator iter;

    for (iter = files.constBegin(); iter != files.constEnd(); ++iter)
    {
        stat(iter->toUtf8(), &st);
        entry = archive_entry_new();
        archive_entry_set_pathname(entry, iter->toUtf8());
        archive_entry_copy_stat(entry, &st);

        if (fs::is_directory(iter->toStdString()))
        {
            archive_entry_set_filetype(entry, AE_IFDIR);
            archive_entry_set_perm(entry, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
        }
        else
        {
            archive_entry_set_filetype(entry, AE_IFREG);
            archive_entry_set_perm(entry, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        }

        archive_write_header(a, entry);

        if ((fd = open(iter->toUtf8(), O_RDONLY)) < 0)
        {
            MSG_ERROR("Error reading file \"" << iter->toStdString() << "\": " << strerror(errno));
            mHaveError = true;
            archive_entry_free(entry);
            continue;
        }

        len = read(fd, buff, sizeof(buff));

        while (len > 0)
        {
            archive_write_data(a, buff, len);
            len = read(fd, buff, sizeof(buff));
        }

        close(fd);
        archive_entry_free(entry);
    }

    archive_write_close(a);
    archive_write_free(a);
}
