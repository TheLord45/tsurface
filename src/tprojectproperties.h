#ifndef TPROJECTPROPERTIES_H
#define TPROJECTPROPERTIES_H

#include <QDialog>

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
        QString getJbName() { return mJobName; }
        void setJobName(const QString& name) { mJobName = name; }

        QString getDesignerID() { return mDesignerID; }
        void setDesignerID(const QString& id) { mDesignerID = id; }

        QString getDealerID() { return mDealerID; }
        void setDealerID(const QString& id) { mDealerID = id; }

        QString getFileRevision() { return mFileRevision; }
        void setFileRevision(const QString& rev) { mFileRevision = rev; }

        QString getJobComments() { return mJobComments; }
        void setJbComments(const QString& com) { mJobComments = com; }

        bool getSystemFileName() { return mSysFileName; }
        void setSystemFileName(bool sf) { mSysFileName = sf; }

        QString getStartup() { return mStartup; }
        void setStartup(const QString& start) { mStartup = start; }

        QString getWakeup() { return mWakeup; }
        void setWakeup(const QString& wu) { mWakeup = wu; }

        QString getSleep() { return mSleep; }
        void setSleep(const QString& sleep) { mSleep = sleep; }

        QString getStandby() { return mStandBy; }
        void setStandby(const QString& sby) { mStandBy = sby; }

        QString getShutdown() { return mShutdown; }
        void setShutown(const QString& sd) { mShutdown = sd; }

        QString getPowerUpPage() { return mPowerUpPage; }
        void setPowerUpPage(const QString& pup) { mPowerUpPage = pup; }

        QList<QString>& getPopwerUpPopups() { return mPowerUpPopups; }
        void setPowerUpPopups(const QList<QString>& list) { mPowerUpPopups = list; }

        QString getInactivityPage() { return mInactivityPage; }
        void setInactivityPage(const QString& pg) { mInactivityPage = pg; }

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
        bool mSysFileName{false};
        QString mStartup;
        QString mWakeup;
        QString mSleep;
        QString mStandBy;
        QString mShutdown;
        QString mPowerUpPage;
        QList<QString> mPowerUpPopups;
        QString mInactivityPage;
};

#endif // TPROJECTPROPERTIES_H
