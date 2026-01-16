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
#ifndef TDYNAMICDATADIALOG_H
#define TDYNAMICDATADIALOG_H

#include <QDialog>

namespace Ui {
class TDynamicDataDialog;
}

class TDynamicDataDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TDynamicDataDialog(QWidget *parent = nullptr);
        ~TDynamicDataDialog();

        typedef enum
        {
            FORMAT_XPORT,
            FORMAT_CSVHEAD,
            FORMAT_CSV
        }FORMAT_t;

        QString getName() { return mName; }
        QString getProtocol() { return mProtocol; }
        QString getHost() { return mHost; }
        QString getPath() { return mPath; }
        QString getFile() { return mFile; }
        QString getUser() { return mUser; }
        QString getPassword();
        int getRefreshRate() { return mRefreshRate; }
        bool getForceData() { return mForceData; }
        FORMAT_t getFormat() { return mFormat; }

        void setNew(bool state);

        void setName(const QString& name);
        void setProtocol(const QString& proto);
        void setHost(const QString& host);
        void setPath(const QString& path);
        void setFile(const QString& file);
        void setUser(const QString& user);
        void setPassword(const QString& pass);
        void setRefreshRate(int rate);
        void setForceData(bool force);
        void setFormat(FORMAT_t format);

    private slots:
        void on_lineEditName_textChanged(const QString &arg1);
        void on_comboBoxProtocol_currentIndexChanged(int index);
        void on_lineEditHost_textChanged(const QString &arg1);
        void on_lineEditPath_textChanged(const QString &arg1);
        void on_lineEditFile_textChanged(const QString &arg1);
        void on_lineEditUser_textChanged(const QString &arg1);
        void on_lineEditPassword_textChanged(const QString &arg1);
        void on_spinBoxRefreshRate_valueChanged(int arg1);
        void on_checkBoxForce_checkStateChanged(const Qt::CheckState &arg1);
        void on_radioButtonXPort_clicked(bool checked);
        void on_radioButtonCsvHead_clicked(bool checked);
        void on_radioButtonCsv_clicked(bool checked);

    private:
        Ui::TDynamicDataDialog *ui;
        QString mName;
        QString mProtocol;
        QString mHost;
        QString mPath;
        QString mFile;
        QString mUser;
        QString mPassword;
        int mRefreshRate{0};
        bool mForceData{false};
        FORMAT_t mFormat{FORMAT_XPORT};
};

#endif // TDYNAMICDATADIALOG_H
