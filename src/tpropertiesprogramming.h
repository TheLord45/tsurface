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
#ifndef TPROPERTIESPROGRAMMING_H
#define TPROPERTIESPROGRAMMING_H

#include <QObject>
#include "tpagehandler.h"
#include "tmisc.h"

class QTableWidget;
class QComboBox;
class QSpinBox;
class QIntValidator;
class TValidatePort;
class TElementWidgetCombo;
class TElementSpinBox;

class TPropertiesProgramming : public QObject
{
    Q_OBJECT

    public:
        TPropertiesProgramming();
        TPropertiesProgramming(QTableWidget *table);
        ~TPropertiesProgramming();

        void setPage(const Page::PAGE_t& page);
        void setProgrammingPage(const QString& name);
        void setProgrammingPage(int id, bool loaded=false);
        void setProgrammingPopup(const QString& name);
        void setProgrammingPopup(int id, bool loaded=false);
        void setObject(ObjHandler::TOBJECT_t& object, int id);
        void setObjectID(int id);
        bool isChanged() { return mChanged; }
        Page::PAGE_t& getActualPage() { return mPage; }
        void setParent(QWidget *parent) { mParent = parent; };

    protected:
        void setTable(STATE_TYPE stype=STATE_UNKNOWN);
        void clear();
        // Callbacks
        void onComboObjectFeedback(const QString& text, const QVariant& data, const QString& name);
        void onSpinAddressPort(int value, const QString& name);
        void onSpinAddressCode(int value, const QString& name);
        void onSpinChannelPort(int value, const QString& name);
        void onSpinChannelCode(int value, const QString& name);
        void onComboLevelControlType(const QString& text, const QVariant& data, const QString& name);
        void onSpinLevelPort(int value, const QString& name);
        void onSpinLevelCode(int value, const QString& name);
        void onSpinLevelControlValue(int value, const QString& name);
        void onSpinControlRepeatTime(int value, const QString& name);
        void onComboLevelFunction(const QString& text, const QVariant& data, const QString& name);
        void onSpinRangeLowHigh(int value, const QString& name);
        void onSpinRangeDragIncrement(int value, const QString& name);
        void onComboRangeInverted(const QString& text, const QVariant& data, const QString& name);
        void onSpinRangeTimeUpDown(int value, const QString& name);
        // Interface methods
        virtual void saveChangedData(Page::PAGE_t *page, PROPERTIES_t prop=TBL_UNKNOWN) = 0;
        virtual void markChanged() = 0;
        virtual Page::PAGE_t getCurrentPage() = 0;


    private:
        TElementWidgetCombo *makeObjectFeedback(const QString& name);
        TElementSpinBox *makeAddressPort(const QString& name);
        TElementSpinBox *makeAddressCode(const QString& name);
        TElementSpinBox *makeChannelPort(const QString& name);
        TElementSpinBox *makeChannelCode(const QString& name);
        TElementWidgetCombo *makeLevelControlType(const QString& name);
        TElementSpinBox *makeLevelPort(const QString& name);
        TElementSpinBox *makeLevelCode(const QString& name);
        TElementSpinBox *makeLevelControlValue(const QString& name);
        TElementSpinBox *makeControlRepeatTime(const QString& name);
        TElementWidgetCombo *makeLevelFunction(const QString& name);
        TElementSpinBox *makeRangeLowHigh(const QString& name);
        TElementSpinBox *makeRangeDragIncrement(const QString& name);
        TElementWidgetCombo *makeRangeInverted(const QString& name);
        TElementSpinBox *makeRangeTimeUpDown(const QString& name);

        void createPage();
        bool isAnyPage();
        void setSType();
        void setTableWidget(int row, int col, const QVariant& data, ELEMENT_TYPE_t etype);

        QTableWidget *mTable{nullptr};
        QWidget *mParent{nullptr};
        bool mChanged{false};
        Page::PAGE_t mPage;
        bool mInitialized{false};
        TValidatePort *mValidatePort{nullptr};
        QIntValidator *mIntValidator{nullptr};
        ObjHandler::TOBJECT_t mActObject;
        int mActObjectID{-1};
        STATE_TYPE mStype{STATE_UNKNOWN};
        // Widgets
        QComboBox *mAddrPort{nullptr};      // All: Address port
        QComboBox *mAddrCode{nullptr};      // All: Address code
        QComboBox *mChanPort{nullptr};      // All: Channel port
        QComboBox *mChanCode{nullptr};      // All: Channel code
};

#endif // TPROPERTIESPROGRAMMING_H
