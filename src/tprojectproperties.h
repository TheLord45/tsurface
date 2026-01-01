/*
 * Copyright (C) 2025, 2026 by Andreas Theofilu <andreas@theosys.at>
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
#ifndef TPROJECTPROPERTIES_H
#define TPROJECTPROPERTIES_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
    class TProjectProperties;
}

class QListWidgetItem;

class TProjectProperties : public QDialog
{
        Q_OBJECT

    public:
        explicit TProjectProperties(QWidget *parent = nullptr);
        ~TProjectProperties();

        // Initializing
        void setPanelImage(const QPixmap& pm);
        void setPanelType(const QString& name);
        void setPanelSize(const QSize& size);

        // Getter/Setter
        QString getJobName() { return mJobName; }
        void setJobName(const QString& name);

        QString getDesignerID() { return mDesignerID; }
        void setDesignerID(const QString& id);

        QString getDealerID() { return mDealerID; }
        void setDealerID(const QString& id);

        QString getFileRevision() { return mFileRevision; }
        void setFileRevision(const QString& rev);

        QString getJobComments() { return mJobComments; }
        void setJobComments(const QString& com);

        bool getSystemFileName() { return mSysFileName; }
        void setSystemFileName(bool sf);

        QString getStartup() { return mStartup; }
        void setStartup(const QString& start);

        QString getWakeup() { return mWakeup; }
        void setWakeup(const QString& wu);

        QString getSleep() { return mSleep; }
        void setSleep(const QString& sleep);

        QString getStandby() { return mStandBy; }
        void setStandby(const QString& sby);

        QString getShutdown() { return mShutdown; }
        void setShutown(const QString& sd);

        QString getPowerUpPage() { return mPowerUpPage; }
        void setPowerUpPage(const QString& pup);

        QList<QString>& getPopwerUpPopups() { return mPowerUpPopups; }
        void setPowerUpPopups(const QList<QString>& list);

        QString getInactivityPage() { return mInactivityPage; }
        void setInactivityPage(const QString& pg);

        void setFileName(const QString& fn);
        void setTimeCreation(const QDateTime& dt);
        void setTimeLastChange(const QDateTime& dt);

    private slots:
        void on_lineEditJobName_textChanged(const QString &arg1);
        void on_lineEditDesignerID_textChanged(const QString &arg1);
        void on_lineEditDealerID_textChanged(const QString &arg1);
        void on_lineEditFileRevision_textChanged(const QString &arg1);
        void on_plainTextEditJobComments_textChanged();
        void on_checkBoxSysFileName_checkStateChanged(const Qt::CheckState &arg1);
        void on_lineEditStartup_textChanged(const QString &arg1);
        void on_lineEditWakeup_textChanged(const QString &arg1);
        void on_lineEditSleep_textChanged(const QString &arg1);
        void on_lineEditStandBy_textChanged(const QString &arg1);
        void on_lineEditShutdown_textChanged(const QString &arg1);
        void on_comboBoxPowerUpPage_currentIndexChanged(int index);
        void on_listWidgetPowerUpPopup_itemActivated(QListWidgetItem *item);
        void on_toolButtonAdd_clicked();
        void on_toolButtonMoveUp_clicked();
        void on_toolButtonMoveDown_clicked();
        void on_toolButtonDelete_clicked();
        void on_comboBoxInactivityPage_currentIndexChanged(int index);

    private:
        Ui::TProjectProperties *ui;
        QString mJobName;
        QString mDesignerID;
        QString mDealerID;
        QString mFileRevision;
        QString mJobComments;
        QString mFileName;
        bool mSysFileName{false};
        QString mStartup;
        QString mWakeup;
        QString mSleep;
        QString mStandBy;
        QString mShutdown;
        QString mPowerUpPage;
        QList<QString> mPowerUpPopups;
        QString mInactivityPage;
        QDateTime mCreation;
        QDateTime mLastChange;
};

#endif // TPROJECTPROPERTIES_H
