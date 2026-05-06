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
#ifndef TEVENTACTIONSDIALOG_H
#define TEVENTACTIONSDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>

#include "tobjecthandler.h"

namespace Ui {
    class TEventActionsDialog;
}

class QListWidgetItem;

class TEventActionsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit TEventActionsDialog(QWidget *parent = nullptr);
        ~TEventActionsDialog();

        typedef enum
        {
            AnimLeft,
            AnimRight,
            AnimTop,
            AnimBottom
        }AnimOrigin_t;

        typedef enum
        {
            AnimFlipSlide,
            AnimFlipSlideBounce,
            AnimFlipFade,
            AnimFlipDoorFade,
            AnimFlipCenterDoorFade,
            AnimFlipZoomIn,
            AnimFlipZoomOut,
            AnimFlipSpinIn,
            AnimFlipSpinOut
        }AnimFlip_t;

        void setFuncs(const QList<ObjHandler::PUSH_FUNC_T>& funcs);
        QList<ObjHandler::PUSH_FUNC_T> getFuncs() { return mFuncs; }
        void setEventType(ObjHandler::BUTTON_EVENT_t event) { mEventType = event; }
        ObjHandler::BUTTON_EVENT_t getEventType() { return mEventType; }
        void setDuration(int dur);
        int getDuration() { return mDuration; }
        void setAnimationFlip(AnimFlip_t flip);
        AnimFlip_t getAnimationFlip() { return mAnimFlip; }
        void setAnimationOrigin(AnimOrigin_t origin);
        AnimOrigin_t getAnimationOrigin() { return mAnimOrigin; }

    protected:
        void accept() override;
        void onPageFlipSelectionChanged(const QString& text, const QVariant& data, const QString& name);
        void onLaunchOptionChanged(int port, const QString& text, const QString& name);
        void onActionChanged(const ObjHandler::PUSH_FUNC_T& pf, const QString& name);

    private slots:
        void on_comboButtonAddPageFlip_currentIndexChanged(int index);
        void on_comboButtonAddLaunchOption_currentIndexChanged(int index);
        void on_comboButtonAddAction_currentIndexChanged(int index);

        void on_pushButtonDelete_clicked();
        void on_pushButtonClearAll_clicked();
        void on_pushButtonMoveUp_clicked();
        void on_pushButtonMoveDown_clicked();
        void on_listWidgetAnimations_itemClicked(QListWidgetItem *item);
        void on_radioButtonLeft_clicked();
        void on_radioButtonRight_clicked();
        void on_radioButtonTop_clicked();
        void on_radioButtonBottom_clicked();
        void on_spinBoxDuration_valueChanged(int arg1);
        void on_tableWidgetActions_cellActivated(int row, int column);
        void on_tableWidgetActions_itemSelectionChanged();

    private:
        typedef struct LINE_EVENT_t
        {
            int line{0};
            ObjHandler::PUSH_FUNC_T pf;
        }LINE_EVENT_t;

        struct RowData
        {
            QTableWidgetItem item0;        // full copy of column 0 item
            int row{-1};
            ObjHandler::PUSH_FUNC_T pf;
        };

        void addPageFlip(const ObjHandler::PUSH_FUNC_T& pf, const QString& name);
        void addLaunchOption(const ObjHandler::PUSH_FUNC_T& pf, const QString& name);
        void addAction(const ObjHandler::PUSH_FUNC_T& pf, const QString& name);
        void setNavButtons();
        int getSelectedRow();
        RowData captureRow(int r) const;
        void populateRow(int r, const RowData& d);
        void moveRow(int from, int to);
        void renumberLines();

        Ui::TEventActionsDialog *ui;
        QList<ObjHandler::PUSH_FUNC_T> mFuncs;
        ObjHandler::BUTTON_EVENT_t mEventType{ObjHandler::EVENT_NONE};
        QList<LINE_EVENT_t> mLines;
        QStringList mPopups;
        QStringList mPages;
        QStringList mGroups;
        QStringList mList1;
        QStringList mList2;
        QStringList mList3;
        QStringList mCommands1;
        QStringList mCommands2;
        QStringList mCommands3;
        bool mBlock{false};
        // Not yet enabled things
        int mDuration{0};
        AnimOrigin_t mAnimOrigin{AnimLeft};
        AnimFlip_t mAnimFlip{AnimFlipSlide};
};

#endif // TEVENTACTIONSDIALOG_H
