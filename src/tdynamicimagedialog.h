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
#ifndef TDYNAMICIMAGEDIALOG_H
#define TDYNAMICIMAGEDIALOG_H

#include <QDialog>

namespace Ui {
class TDynamicImageDialog;
}

class TDynamicImageDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TDynamicImageDialog(QWidget *parent = nullptr);
        ~TDynamicImageDialog();

        QString getName() { return mName; }
        void setName(const QString& name);
        QString getProtocol() { return mProtocol; }
        void setProtocol(const QString& proto);
        QString getHost() { return mHost; }
        void setHost(const QString& host);
        QString getPath() { return mPath; }
        void setPath(const QString& path);
        QString getFile() { return mFile; }
        void setFile(const QString& file);
        QString getUser() { return mUser; }
        void setUser(const QString& user);
        QString getPassword() { return mPassword; }
        void setPassword(const QString& pass);
        int getRefreshRate() { return mRefreshRate; }
        void setRefreshRate(int rr);
        bool getRefreshStart() { return mRefreshStart; }
        void setRefreshStart(bool start);
        void setEdit(bool state);

    protected:
        void accept() override;

    private slots:
        void on_lineEditName_textChanged(const QString &arg1);
        void on_comboBox_currentIndexChanged(int index);
        void on_lineEditHost_textChanged(const QString &arg1);
        void on_lineEditPath_textChanged(const QString &arg1);
        void on_lineEditFile_textChanged(const QString &arg1);
        void on_lineEditUser_textChanged(const QString &arg1);
        void on_lineEditPassword_textChanged(const QString &arg1);
        void on_spinBoxRefreshRate_valueChanged(int arg1);
        void on_checkBoxRefreshStart_clicked(bool checked);

    private:
        Ui::TDynamicImageDialog *ui;
        QString mName;
        QString mProtocol;
        QString mHost;
        QString mPath;
        QString mFile;
        QString mUser;
        QString mPassword;
        int mRefreshRate{0};
        bool mRefreshStart{false};
};

#endif // TDYNAMICIMAGEDIALOG_H
