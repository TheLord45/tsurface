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
#include "tprojectproperties.h"
#include "ui_tprojectproperties.h"

#include "tconfmain.h"
#include "terror.h"

TProjectProperties::TProjectProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TProjectProperties)
{
    ui->setupUi(this);

    QList<QString> allPages = TConfMain::Current().getAllPages();
    QList<QString>::Iterator iter;
    ui->comboBoxPowerUpPage->addItem("<NONE>");
    ui->comboBoxInactivityPage->addItem("<NONE>");

    for (iter = allPages.begin(); iter != allPages.end(); ++iter)
    {
        ui->comboBoxPowerUpPage->addItem(*iter);
        ui->comboBoxInactivityPage->addItem(*iter);
    }
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

void TProjectProperties::setJobName(const QString& name)
{
    DECL_TRACER("TProjectProperties::setJobName(const QString& name)");

    mJobName = name;
    ui->lineEditJobName->setText(name);
}

void TProjectProperties::setDesignerID(const QString& id)
{
    DECL_TRACER("TProjectProperties::setDesignerID(const QString& id) ");

    mDesignerID = id;
    ui->lineEditDesignerID->setText(id);
}

void TProjectProperties::setDealerID(const QString& id)
{
    DECL_TRACER("TProjectProperties::setDealerID(const QString& id) ");

    mDealerID = id;
    ui->lineEditDealerID->setText(id);
}

void TProjectProperties::setFileRevision(const QString& rev)
{
    DECL_TRACER("TProjectProperties::setFileRevision(const QString& rev)");

    mFileRevision = rev;
    ui->lineEditFileRevision->setText(rev);
}

void TProjectProperties::setJobComments(const QString& com)
{
    DECL_TRACER("TProjectProperties::setJobComments(const QString& com)");

    mJobComments = com;
    ui->plainTextEditJobComments->setPlainText(com);
}

void TProjectProperties::setSystemFileName(bool sf)
{
    DECL_TRACER("TProjectProperties::setSystemFileName(bool sf)");

    if (sf)
        ui->checkBoxSysFileName->setCheckState(Qt::Checked);
    else
        ui->checkBoxSysFileName->setCheckState(Qt::Unchecked);
}

void TProjectProperties::setStartup(const QString& start)
{
    DECL_TRACER("TProjectProperties::setStartup(const QString& start) ");

    mStartup = start;
    ui->lineEditStartup->setText(start);
}

void TProjectProperties::setWakeup(const QString& wu)
{
    DECL_TRACER("TProjectProperties::setWakeup(const QString& wu)");

    mWakeup = wu;
    ui->lineEditWakeup->setText(wu);
}

void TProjectProperties::setSleep(const QString& sleep)
{
    DECL_TRACER("TProjectProperties::setSleep(const QString& sleep)");

    mSleep = sleep;
    ui->lineEditSleep->setText(sleep);
}

void TProjectProperties::setStandby(const QString& sby)
{
    DECL_TRACER("TProjectProperties::setStandby(const QString& sby)");

    mStandBy = sby;
    ui->lineEditStandBy->setText(sby);
}

void TProjectProperties::setShutown(const QString& sd)
{
    DECL_TRACER("TProjectProperties::setShutown(const QString& sd)");

    mShutdown = sd;
    ui->lineEditShutdown->setText(sd);
}

void TProjectProperties::setPowerUpPage(const QString& pup)
{
    DECL_TRACER("TProjectProperties::setPowerUpPage(const QString& pup)");

    mPowerUpPage = pup;

    if (ui->comboBoxPowerUpPage->count() > 0)
    {
        for (int i = 0; i < ui->comboBoxPowerUpPage->count(); ++i)
        {
            if (ui->comboBoxPowerUpPage->itemText(i) == pup)
            {
                ui->comboBoxPowerUpPage->setCurrentIndex(i);
                break;
            }
        }
    }
    else if (!pup.isEmpty())
        ui->comboBoxPowerUpPage->addItem(pup);
}

void TProjectProperties::setPowerUpPopups(const QList<QString>& list)
{
    DECL_TRACER("TProjectProperties::setPowerUpPopups(const QList<QString>& list)");

    ui->listWidgetPowerUpPopup->clear();

    if (list.isEmpty())
        return;

    mPowerUpPopups = list;
    QList<QString>::ConstIterator iter;

    for (iter = list.constBegin(); iter != list.constEnd(); ++iter)
        ui->listWidgetPowerUpPopup->addItem(*iter);
}

void TProjectProperties::setInactivityPage(const QString& pg)
{
    DECL_TRACER("TProjectProperties::setInactivityPage(const QString& pg)");

    mInactivityPage = pg;

    if (ui->comboBoxInactivityPage->count() > 0)
    {
        for (int i = 0; i < ui->comboBoxInactivityPage->count(); ++i)
        {
            if (ui->comboBoxInactivityPage->itemText(i) == pg)
            {
                ui->comboBoxInactivityPage->setCurrentIndex(i);
                break;
            }
        }
    }
    else if (!pg.isEmpty())
        ui->comboBoxInactivityPage->addItem(pg);
}

void TProjectProperties::setFileName(const QString& fn)
{
    DECL_TRACER("TProjectProperties::setFileName(const QString& fn)");

    mFileName = fn;
    ui->labelShowFileName->setText(fn);
}

void TProjectProperties::setTimeCreation(const QDateTime& dt)
{
    DECL_TRACER("TProjectProperties::setTimeCreation(const QDateTime& dt)");

    ui->labelDateCreated->setText(dt.toString());
}

void TProjectProperties::setTimeLastChange(const QDateTime& dt)
{
    DECL_TRACER("TProjectProperties::setTimeLastChange(const QDateTime& dt)");

    ui->labelDateModified->setText(dt.toString());
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

