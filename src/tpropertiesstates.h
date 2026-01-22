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
#ifndef TPROPERTIESSTATES_H
#define TPROPERTIESSTATES_H

#include <QObject>

#include "tpagehandler.h"
#include "tmisc.h"

class QTreeWidget;

class TPropertiesStates : public QObject
{
    Q_OBJECT

    public:
        TPropertiesStates(QTreeWidget *widget);
        ~TPropertiesStates();

        void setStatesPage(const QString& name);
        void setStatesPage(int id, bool loaded);

    protected:
        // Interface methods
        virtual void saveChangedData(Page::PAGE_t *page, PROPERTIES_t prop=TBL_UNKNOWN) = 0;

        // Other methods
        void createPage();

    private:
        QTreeWidget *mTreeWidget{nullptr};
        Page::PAGE_t mPage;
        bool mChanged{false};
        bool mInitialized{false};
};

#endif // TPROPERTIESSTATES_H
