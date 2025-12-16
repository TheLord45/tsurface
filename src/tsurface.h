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
#ifndef TSURFACE_H
#define TSURFACE_H

#include <QMainWindow>

#include <memory>

namespace Ui {
class tsurface;
}

class TSurface : public QMainWindow
{
    Q_OBJECT

    public:
        explicit TSurface(QWidget *parent = nullptr);
        ~TSurface() override;

    protected:
        void resizeEvent(QResizeEvent *event) override;

    private slots:
        void on_actionOpen_triggered();

    private:
        const std::unique_ptr<Ui::tsurface> m_ui;
        QString mLastOpenPath;
};

#endif // TSURFACE_H
