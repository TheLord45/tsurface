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

class TElementWidgetCombo;
class TElementLineEdit;
class TElementWidgetText;
class TElementSpinBox;
class TElementBorderName;

class TPropertiesGeneral : public QObject
{
    Q_OBJECT

    public:
        TPropertiesGeneral();
        TPropertiesGeneral(QTableWidget *view);
        ~TPropertiesGeneral();

        void setGeneralPage(Page::PAGE_t& page, STATE_TYPE stype, int objid=-1);
        void setGeneralObjectID(int index);
        bool isChanged() { return mChanged; }
        Page::PAGE_t& getActualPage() { return mPage; }
        void setGeometryPopup(const QRect& geom);
        void setGeometryButton(int bi, const QRect& geom);
        void update();

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
        virtual void requestRedraw(Page::PAGE_t *page) = 0;
        virtual ObjHandler::TOBJECT_t getActualObject(const Page::PAGE_t& page) = 0;
        // Callbacks
        void onCellChanged(int row, int column);
        void onCellActivated(int row, int column);
        void onComboPopupTypeChanged(const QString& text, const QVariant& data, const QString& name);
        void onComboButtonTypeChanged(const QString& text, const QVariant& data, const QString& name);
        void onComboLockButtonName(const QString& text, const QVariant& data, const QString& name);
        void onObjectNameChanged(const QString& text, const QString& name);
        void onDescriptionChanged(const QString& text, const QString& name);
        void onSpinGeometryChanged(int value, const QString& name);
        void onComboPopupGroupChanged(const QString& text, const QVariant& data, const QString& name);
        void onPopupTimeout(int value, const QString& name);
        void onPopupModalChanged(const QString& text, const QVariant& data, const QString& name);
        void onPopupShowEffectChanged(const QString& text, const QVariant& data, const QString& name);
        void onPopupHideEffectChanged(const QString& text, const QVariant& data, const QString& name);
        void onPopupCollapseDirChanged(const QString& text, const QVariant& data, const QString& name);
        void onSpinTimeoutValue(int value, const QString& name);
        void onSpinPopupEffectTimeChnaged(int value, const QString& name);
        void onComboPopupEffectPos(const QString& text, const QVariant& data, const QString& name);
        void onSpinCollapseOffset(int value, const QString& name);
        void onComboCollapseShowOpen(const QString& text, const QVariant& data, const QString& name);
        void onComboObjectDragDropType(const QString& text, const QVariant& data, const QString& name);
        void onComboObjectDropGroup(const QString& text, const QVariant& data, const QString& name);
        void onComboObjectTouchStyle(const QString& text, const QVariant& data, const QString& name);
        void onObjectBorderStyle(const QString& border, const QString& name);

        // Internal methods
        void setParent(QWidget *parent) { mParent = parent; };
        void setWidget(QTableWidget *view);
        void clear();
        QString getLabelText(int line);
        void createTable(STATE_TYPE stype);
        void setTable(STATE_TYPE stype);

        TElementWidgetCombo *makePopupType(const QString& name);
        TElementWidgetCombo *makeButtonType(const QString& name);
        TElementLineEdit *makeObjectName(const QString& text, const QString& name);
        TElementWidgetCombo *makeButtonLockName(const QString& name);
        TElementWidgetText *makeDescription(const QString& name);
        TElementSpinBox *makeSpinGeometry(const QString& name);
        TElementWidgetCombo *makePopupResetOnPos(const QString& name);
        TElementLineEdit *makeObjectZOrder(const QString& name);
        TElementWidgetCombo *makePopupGroups(const QString& name);
        TElementSpinBox *makePopupTimeout(const QString& name);
        TElementWidgetCombo *makePopupModal(const QString& name);
        TElementWidgetCombo *makePopupShowEffect(const QString& name);
        TElementWidgetCombo *makePopupHideEffect(const QString& name);
        TElementWidgetCombo *makePopupCollapseDir(const QString& name);
        TElementSpinBox *makePopupEffectTime(int t, const QString& name);
        TElementWidgetCombo *makePopupEffectPos(Page::SHOWEFFECT effect, const QString& name);
        TElementSpinBox *makePopupCollapseOffset(const QString& name);
        TElementWidgetCombo *makePopupCollapseShowOpen(const QString& name);
        TElementWidgetCombo *makeObjectDragDropType(const QString& name);
        TElementWidgetCombo *makeObjectDropGroup(const QString& name);
        TElementWidgetCombo *makeObjectTouchStyle(const QString& name);
        TElementBorderName *makeObjectBorderStyle(const QString& name);

    private:
        void initYesNo(QStringList& list, QList<QVariant>& data);

        QTableWidget *mTable{nullptr};
        QWidget *mParent{nullptr};
        bool mConnected{false};
        ACTIVE_t mCellActive;
        Page::PAGE_t mPage;
        ObjHandler::TOBJECT_t mActObject;
        QLineEdit *mLineDescription{nullptr};
        bool mChanged{false};
        bool mInitialized{false};
        void loadPage(int pageID);
        int mActObjectID{-1};

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
