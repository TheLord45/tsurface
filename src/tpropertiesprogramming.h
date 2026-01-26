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

class TPropertiesProgramming : public QObject
{
    Q_OBJECT

    public:
        TPropertiesProgramming();
        TPropertiesProgramming(QTableWidget *table);
        ~TPropertiesProgramming();

        void setProgrammingPage(const QString& name);
        void setProgrammingPage(int id, bool loaded=false);
        void setProgrammingPopup(const QString& name);
        void setProgrammingPopup(int id, bool loaded=false);
        bool isChanged() { return mChanged; }
        Page::PAGE_t& getActualPage() { return mPage; }
        void setParent(QWidget *parent) { mParent = parent; };

    protected:
        void createPage();
        void clear();
        // Callbacks
        void onComboAddrPort(int index);
        void onComboAddrPortText(const QString& text);
        void onComboAddrCode(int index);
        void onComboAddrCodeText(const QString& text);
        void onComboChanPort(int index);
        void onComboChanPortText(const QString& text);
        void onComboChanCode(int index);
        void onComboChanCodeText(const QString& text);
        // Interface methods
        virtual void saveChangedData(Page::PAGE_t *page, PROPERTIES_t prop=TBL_UNKNOWN) = 0;
        virtual void markChanged() = 0;

    private:
        QTableWidget *mTable{nullptr};
        QWidget *mParent{nullptr};
        bool mChanged{false};
        Page::PAGE_t mPage;
        bool mInitialized{false};
        TValidatePort *mValidatePort{nullptr};
        QIntValidator *mIntValidator{nullptr};
        // Widgets
        QComboBox *mFeedback{nullptr};      // Buttons. Defines the type of feedback
        QComboBox *mAddrPort{nullptr};      // All: Address port
        QComboBox *mAddrCode{nullptr};      // All: Address code
        QComboBox *mChanPort{nullptr};      // All: Channel port
        QComboBox *mChanCode{nullptr};      // All: Channel code
        QComboBox *mLevelCtrl{nullptr};     // Buttons: Level control
        QComboBox *mLevelPort{nullptr};     // Bargraph: Level port
        QComboBox *mLevelCode{nullptr};     // Bargraph: Level code
        QComboBox *mLevelFunc{nullptr};     // Bargraph: Level function
        QSpinBox *mRangeLow{nullptr};       // Bargraph: Range low
        QSpinBox *mRangeHigh{nullptr};      // Bargraph: Range high
        QComboBox *mRangeInverted{nullptr}; // Bargraph: Range inverted
};

#endif // TPROPERTIESPROGRAMMING_H
