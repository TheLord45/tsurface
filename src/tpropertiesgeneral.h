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
#ifndef TPROPERTIESGENERAL_H
#define TPROPERTIESGENERAL_H

#include <QTableWidget>

#include "tpagehandler.h"
#include "tmisc.h"

class QWidget;
class QComboBox;
class QSpinBox;

class TPropertiesGeneral : public QObject
{
    Q_OBJECT

    public:
        TPropertiesGeneral();
        TPropertiesGeneral(QTableWidget *view);
        ~TPropertiesGeneral();

        void setGeneralPage(const QString& name);
        void setGeneralPage(int id, bool loaded=false);
        void setGeneralPopup(const QString& name);
        void setGeneralPopup(int id, bool loaded=false);
        bool isChanged() { return mChanged; }
        Page::PAGE_t& getActualPage() { return mPage; }

    protected:
        typedef struct ACTIVE_t
        {
            int row{0};
            int col{0};
        }ACTIVE_t;

        // Interface methods
        virtual void pageNameChanged(int id, const QString& name) = 0;
        virtual void saveChangedData(Page::PAGE_t *page, PROPERTIES_t prop=TBL_UNKNOWN) = 0;
        virtual void markChanged() = 0;
        // Callbacks
        void onCellChanged(int row, int column);
        void onCellActivated(int row, int column);
        void onLineEditTextChanged(const QString& text);
        void onToolButtonClicked();
        void onComboPopupTypeChanged(int index);
        void onSpinLeftValue(int value);
        void onSpinTopValue(int value);
        void onSpinWidthValue(int value);
        void onSpinHeightValue(int value);
        void onComboResetPosChanged(int index);
        void onComboGroupTextChanged(const QString& text);
        void onSpinTimeoutValue(int value);
        void onComboModalChanged(int index);
        void onComboShowChanged(int index);
        void onComboHideChanged(int index);
        void onComboColapseChange(int index);
        // Internal methods
        void setParent(QWidget *parent) { mParent = parent; };
        void setWidget(QTableWidget *view);
        QWidget *makeLabelTool(const QString& text, int id);

    private:
        QTableWidget *mTable{nullptr};
        QWidget *mParent{nullptr};
        bool mConnected{false};
        ACTIVE_t mCellActive;
        Page::PAGE_t mPage;
        QLineEdit *mLineDescription{nullptr};
        bool mChanged{false};
        bool mInitialized{false};

        QComboBox *mComboPopupType{nullptr};
        QSpinBox *mSpinLeft{nullptr};
        QSpinBox *mSpinTop{nullptr};
        QSpinBox *mSpinWidth{nullptr};
        QSpinBox *mSpinHeight{nullptr};
        QComboBox *mComboResetPos{nullptr};
        QComboBox *mComboGroupText{nullptr};
        QSpinBox *mSpinTimeout{nullptr};
        QComboBox *mComboModal{nullptr};
        QComboBox *mComboShow{nullptr};
        QComboBox *mComboHide{nullptr};
        QComboBox *mComboColapse{nullptr};
};

#endif // TPROPERTIESGENERAL_H
