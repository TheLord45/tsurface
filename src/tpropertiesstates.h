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
class QTableWidget;
class QWidget;
class QComboBox;
class QLabel;
class QLineEdit;
class QSpinBox;

class TElementBitmapSelector;
class TElementWidgetCombo;

class TPropertiesStates : public QObject
{
    Q_OBJECT

    public:
        TPropertiesStates(QTreeWidget *widget);
        ~TPropertiesStates();

        void setStatesPage(const QString& name);
        void setStatesPage(int id, bool loaded);
        void setActualButton(int bi);
        void update();

    protected:
        // Interface methods
        virtual void saveChangedData(Page::PAGE_t *page, PROPERTIES_t prop=TBL_UNKNOWN) = 0;
        virtual void markChanged() = 0;

        // Other methods
        void setParent(QWidget *widget) { mParent = widget; }
        void createPage();
        QTableWidget *createTableWidget(STATE_TYPE stype, QWidget *parent=nullptr);
        QString getLeftColText(STATE_TYPE stype, int state, int line);
        QComboBox *makeFillType(const QString& ftype, const QString& name);
        QWidget *makeColorSelector(const QColor& col, const QString& name);
        QWidget *makeVideoFill(const QString& vf, const QString& name);
        TElementBitmapSelector *makeBitmapSelector(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name);
        QWidget *makeFontSelector(const QString& fname, const QString& name);
        QSpinBox *makeValueSelector(int value, const QString& name);
        QWidget *makeTextValue(const QString& txt, const QFont& font, const QString& name);
        TElementWidgetCombo *makeTextJustification(ObjHandler::ORIENTATION ori, const QString& name);
        QTreeWidget *makeTextEffect(int ef, const QString& name);
        QComboBox *makeWordWrap(bool ww, const QString& name);
        void setGeometry(int bi, const QRect& geom);
        void clear();

        // Callbacks
        void onBitmapsChanged(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name);
        void onOrientationChanged(const QString& text, const QVariant& data, const QString& name);

    private:
        QFont chooseFont(const QFont& font);
        void setValue(const QString& name, const QVariant& value);
        void setColor(QLabel *label, QColor& color);

        QTreeWidget *mTreeWidget{nullptr};
        QWidget *mParent{nullptr};
        Page::PAGE_t mPage;
        int mActualObjectID{0};
        bool mChanged{false};
        bool mInitialized{false};
        bool mBlocked{false};       // TRUE = A dialog box is open. Recreating is blocked!
};

#endif // TPROPERTIESSTATES_H
