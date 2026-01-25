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
class QWidget;
class QComboBox;

class TPropertiesStates : public QObject
{
    Q_OBJECT

    public:
        TPropertiesStates(QTreeWidget *widget);
        ~TPropertiesStates();

        void setStatesPage(const QString& name);
        void setStatesPage(int id, bool loaded);
        void update();

    protected:
        // Interface methods
        virtual void saveChangedData(Page::PAGE_t *page, PROPERTIES_t prop=TBL_UNKNOWN) = 0;

        // Callbacks
        void onComboBoxFillTypeIndex(int index);
        void onPushButtonColorSelector(bool checked);
        void onComboBoxVideoFill(int index);
        void onPushButtonBitmapSelector(bool checked);

        // Other methods
        void setParent(QWidget *widget) { mParent = widget; }
        void createPage();
        QWidget *createTableWidget(STATE_TYPE stype);
        QString getLeftColText(STATE_TYPE stype, int state, int line);
        QWidget *makeFillType();
        QWidget *makeColorSelector(const QColor& col, const QString& name);
        QWidget *makeVideoFill();
        QWidget *makeBitmapSelector(const QString& bitmap, const QString& name);
        void setGeometry(int bi, const QRect& geom);

    private:
        QTreeWidget *mTreeWidget{nullptr};
        QWidget *mParent{nullptr};
        Page::PAGE_t mPage;
        bool mChanged{false};
        bool mInitialized{false};
        // Elements
        QComboBox *mComboBoxFillType{nullptr};
        QWidget *mWidgetColorSelector{nullptr};
        QComboBox *mComboBoxVideoFill{nullptr};
        QWidget *mWidgetBitmapSelector{nullptr};
};

#endif // TPROPERTIESSTATES_H
