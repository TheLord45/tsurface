/*
 * Copyright (C) 2025 by Andreas Theofilu <andreas@theosys.at>
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
#ifndef TNEWPROJECTDIALOG_H
#define TNEWPROJECTDIALOG_H

#include <QDialog>

namespace Ui {
class TNewProjectDialog;
}

class TPanelType;

class TNewProjectDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TNewProjectDialog(QWidget *parent = nullptr);
        ~TNewProjectDialog();

        QString& getProjectName() { return mProjectName; }
        QString& getPanelName() { return mPanelName; }
        QSize& getResolution() { return mResolution; }

    private slots:
        void on_lineEditJobName_textChanged(const QString &arg1);
        void on_comboBoxPanelTypes_currentIndexChanged(int index);
        void on_comboBoxResolutions_currentIndexChanged(int index);
        void on_checkBoxGenFiles_stateChanged(int arg1);
        void on_pushButtonPrevious_clicked();
        void on_pushButtonNext_clicked();
        void on_pushButtonFinish_clicked();
        void on_pushButtonCancel_clicked();

    private:
        Ui::TNewProjectDialog *ui;
        TPanelType *mPanelType{nullptr};
        std::vector<QSize> mResolutions;
        QString mProjectName;
        QString mPanelName;
        QSize mResolution;

};

#endif // TNEWPROJECTDIALOG_H
