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
#ifndef TPROPERTIESEVENTS_H
#define TPROPERTIESEVENTS_H

#include <QObject>

#include "tpagehandler.h"

class QTableWidget;

class TPropertiesEvents : public QObject
{
    Q_OBJECT

    public:
        TPropertiesEvents();
        TPropertiesEvents(QTableWidget *table);
        ~TPropertiesEvents();

        void setTableWidget(QTableWidget *table);
        void setPage(const Page::PAGE_t& page);
        void reset();

    protected:
        // Callbacks
        void onCellActivated(int row, int column);

    private:
        void doConnect(QTableWidget *table);
        QString getLabelText(int line);
        void createTable();

        QTableWidget *mTable{nullptr};
        bool mConnected{false};
        bool mInitialized{false};
        Page::PAGE_t mPage;
};

#endif // TPROPERTIESEVENTS_H
