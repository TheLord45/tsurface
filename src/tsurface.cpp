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
#include <QFileDialog>

#include <filesystem>

#include "tsurface.h"
#include "ui_tsurface.h"
#include "moc_tsurface.cpp"

#include "terror.h"

namespace fs = std::filesystem;

TSurface::TSurface(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::tsurface)
{
    DECL_TRACER("TSurface::TSurface(QWidget *parent)");

    m_ui->setupUi(this);

    char *home = getenv("HOME");

    if (home)
        mLastOpenPath = home;
    else
        mLastOpenPath = QString::fromStdString(fs::current_path());
}

TSurface::~TSurface() = default;

void TSurface::on_actionOpen_triggered()
{
    DECL_TRACER("TSurface::on_actionOpen_triggered()");

    QString file = QFileDialog::getOpenFileName(this, tr("Open Logfile"), mLastOpenPath, tr("Surface (*.tsf);;AMX (*.TP4 *.TP5);;All (*)"));
    qsizetype pos = file.lastIndexOf("/");

    if (pos > 0)
        mLastOpenPath = file.left(pos);

}
