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
class TElementWidgetFont;
class TElementTextEffect;
class TElementBorderName;
class TElementGradientColors;
class TElementSound;

class TPropertiesStates : public QObject
{
    Q_OBJECT

    public:
        TPropertiesStates(QTreeWidget *widget);
        ~TPropertiesStates();

        void setPage(const Page::PAGE_t& page);
        void setActualButton(int index, STATE_TYPE stype);
        void setObjectType(ObjHandler::BUTTONTYPE btype, int index);
        void setState(STATE_TYPE stype);
        bool isChanged() { return mChanged; }

    protected:
        // Interface methods
        virtual void saveChangedData(Page::PAGE_t *page, PROPERTIES_t prop=TBL_UNKNOWN) = 0;
        virtual void markChanged() = 0;
        virtual void requestRedraw(Page::PAGE_t *page) = 0;
        virtual Page::PAGE_t getCurrentPage() = 0;

        // Other methods
        void setParent(QWidget *widget) { mParent = widget; }
        void createPage(bool force=false);
        QTableWidget *createTableWidget(QWidget *parent=nullptr);
        QString getLeftColText(int line);
        TElementBorderName *makeBorderName(const QString& name);
        QComboBox *makeFillType(const QString& name);
        QWidget *makeColorSelector(const QString& name);
        QWidget *makeVideoFill(const QString& name);
        TElementBitmapSelector *makeBitmapSelector(const QString& name);
        TElementWidgetFont *makeFontSelector(const QString& name);
        QSpinBox *makeValueSelector(const QString& name, int max);
        QWidget *makeTextValue(const QString& name);
        TElementWidgetCombo *makeTextJustification(const QString& name);
        TElementTextEffect *makeTextEffect(const QString& name);
        TElementWidgetCombo *makeWordWrap(const QString& name);
        TElementGradientColors *makeGradientColors(const QString& name);
        TElementSound *makeSoundSelector(const QString& name);
        void clear();

        // Callbacks
        void onBitmapsChanged(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name);
        void onOrientationChanged(const QString& text, const QVariant& data, const QString& name);
        void onFontChanged(const QFont& font, const QString& name);
        void onTextEffectChanged(int eff, const QString& effect, const QString& name);
        void onBorderNameChanged(const QString& border, const QString& name);
        void onWordWrapChanged(const QString& text, const QVariant& data, const QString& name);
        void onGradientColorChanged(const QList<QColor>& colors, const QString& name);
        void onSoundChanged(const QString& file, const QString& name);
        void onVideoFillChanged(const QString& text, const QVariant& data, const QString& name);
        void onColorChanged(const QColor& color, const QString& name);
        void onTextValueChanged(const QString& text, const QString& name);
        void onFillTypeChanged(const QString& text, const QVariant& data, const QString& name);

    private:
        QFont chooseFont(const QFont& font);
        void setValue(const QString& name, const QVariant& value);
        void setColor(QLabel *label, QColor& color);
        void setTable(QTableWidget *table, int instance=-1);
        void createPage(QTableWidget *table, int instance=-1);
        void setSType();
        bool isAnyPage();
        bool isValidObjectIndex();
        void rebuildTree();
        QString getStateName(ObjHandler::BUTTONTYPE type, int instance);
        QTableWidget *getTableWidget(int state=0);
        void setTableWidget(QTableWidget *table, int row, int col, const QVariant& data, ELEMENT_TYPE_t etype);
        void setTableWidget(QTableWidget *table, int row, int col, const QList<ObjHandler::BITMAPS_t>& bm, ELEMENT_TYPE_t etype);
        void setTableWidget(QTableWidget *table, int row, int col, const QFont& font, ELEMENT_TYPE_t etype);

        QTreeWidget *mTreeWidget{nullptr};
        QWidget *mParent{nullptr};
        Page::PAGE_t mPage;
        ObjHandler::TOBJECT_t mActObject;
        STATE_TYPE mSType{STATE_UNKNOWN};
        QList<QTableWidget *> mStates;
        int mActState {0};              // The index ob the actual selected state
        int mActObjectID{0};            // The index of the actual selected object
        int mActInstance{0};            // The index number for the SR structure inside an object
        bool mChanged{false};
        bool mInitialized{false};
};

#endif // TPROPERTIESSTATES_H
