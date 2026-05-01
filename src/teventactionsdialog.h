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

        void setFuncs(const QList<ObjHandler::PUSH_FUNC_T>& funcs) { mFuncs = funcs; }
        QList<ObjHandler::PUSH_FUNC_T> getFuncs() { return mFuncs; }
        void setEventType(ObjHandler::BUTTON_EVENT_t event) { mEventType = event; }
        ObjHandler::BUTTON_EVENT_t getEventType() { return mEventType; }

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

    private:
        Ui::TEventActionsDialog *ui;
        QList<ObjHandler::PUSH_FUNC_T> mFuncs;
        ObjHandler::BUTTON_EVENT_t mEventType;
};

#endif // TEVENTACTIONSDIALOG_H
