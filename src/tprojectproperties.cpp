#include "tprojectproperties.h"
#include "ui_tprojectproperties.h"

#include "terror.h"

TProjectProperties::TProjectProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TProjectProperties)
{
    ui->setupUi(this);
}

TProjectProperties::~TProjectProperties()
{
    delete ui;
}

void TProjectProperties::setPanelImage(const QPixmap& pm)
{
    DECL_TRACER("TProjectProperties::setPanelImage(const QPixmap& pm)");

    ui->labelShowPanelIcon->setPixmap(pm);
}

void TProjectProperties::setPanelType(const QString& type)
{
    DECL_TRACER("TProjectProperties::setPanelType(const QString& type)");

    ui->labelShowPanelType->setText(type);
}

void TProjectProperties::setPanelSize(const QSize& size)
{
    DECL_TRACER("TProjectProperties::setPanelSize(const QSize& size)");

    ui->labelShowScreenSize->setText(QString("%1x%2").arg(size.width()).arg(size.height()));
}

void TProjectProperties::on_lineEditJobName_textChanged(const QString &arg1)
{
    DECL_TRACER("TProjectProperties::on_lineEditJobName_textChanged(const QString &arg1)");

    mJobName = arg1;
}

void TProjectProperties::on_lineEditDesignerID_textChanged(const QString &arg1)
{
    DECL_TRACER("TProjectProperties::on_lineEditDesignerID_textChanged(const QString &arg1)");

    mDesignerID = arg1;
}

void TProjectProperties::on_lineEditDealerID_textChanged(const QString &arg1)
{
    DECL_TRACER("TProjectProperties::on_lineEditDealerID_textChanged(const QString &arg1)");

    mDealerID = arg1;
}

void TProjectProperties::on_lineEditFileRevision_textChanged(const QString &arg1)
{
    DECL_TRACER("TProjectProperties::on_lineEditFileRevision_textChanged(const QString &arg1)");

    mFileRevision = arg1;
}

void TProjectProperties::on_plainTextEditJobComments_textChanged()
{
    DECL_TRACER("TProjectProperties::on_plainTextEditJobComments_textChanged()");

    mJobComments = ui->plainTextEditJobComments->toPlainText();
}

void TProjectProperties::on_checkBoxSysFileName_checkStateChanged(const Qt::CheckState &arg1)
{
    DECL_TRACER("TProjectProperties::on_checkBoxSysFileName_checkStateChanged(const Qt::CheckState &arg1)");

    if (arg1 == Qt::Checked)
        mSysFileName = true;
    else
        mSysFileName = false;
}

void TProjectProperties::on_lineEditStartup_textChanged(const QString &arg1)
{
    DECL_TRACER("TProjectProperties::on_lineEditStartup_textChanged(const QString &arg1)");

    mStartup = arg1;
}
void TProjectProperties::on_lineEditWakeup_textChanged(const QString &arg1)
{
    DECL_TRACER("TProjectProperties::on_lineEditWakeup_textChanged(const QString &arg1)");

    mWakeup = arg1;
}

void TProjectProperties::on_lineEditSleep_textChanged(const QString &arg1)
{
    DECL_TRACER("TProjectProperties::on_lineEditSleep_textChanged(const QString &arg1)");

    mSleep = arg1;
}

void TProjectProperties::on_lineEditStandBy_textChanged(const QString &arg1)
{
    DECL_TRACER("TProjectProperties::on_lineEditStandBy_textChanged(const QString &arg1)");

    mStandBy = arg1;
}

void TProjectProperties::on_lineEditShutdown_textChanged(const QString &arg1)
{
    DECL_TRACER("TProjectProperties::on_lineEditShutdown_textChanged(const QString &arg1)");

    mShutdown = arg1;
}

void TProjectProperties::on_comboBoxPowerUpPage_currentIndexChanged(int index)
{
    DECL_TRACER("TProjectProperties::on_comboBoxPowerUpPage_currentIndexChanged(int index)");

    mPowerUpPage = ui->comboBoxPowerUpPage->itemText(index);
}

void TProjectProperties::on_listWidgetPowerUpPopup_itemActivated(QListWidgetItem *item)
{
    DECL_TRACER("TProjectProperties::on_listWidgetPowerUpPopup_itemActivated(QListWidgetItem *item)");

    // TODO: Add code
}

void TProjectProperties::on_toolButtonAdd_clicked()
{
    DECL_TRACER("TProjectProperties::on_toolButtonAdd_clicked()");


}

void TProjectProperties::on_toolButtonMoveUp_clicked()
{
    DECL_TRACER("TProjectProperties::on_toolButtonMoveUp_clicked()");

}

void TProjectProperties::on_toolButtonMoveDown_clicked()
{
    DECL_TRACER("TProjectProperties::on_toolButtonMoveDown_clicked()");

}

void TProjectProperties::on_toolButtonDelete_clicked()
{
    DECL_TRACER("TProjectProperties::on_toolButtonDelete_clicked()");

}

void TProjectProperties::on_comboBoxInactivityPage_currentIndexChanged(int index)
{
    DECL_TRACER("TProjectProperties::on_comboBoxInactivityPage_currentIndexChanged(int index)");

    mInactivityPage = ui->comboBoxInactivityPage->itemText(index);
}

