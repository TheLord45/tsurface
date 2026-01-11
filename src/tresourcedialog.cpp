#include <QIcon>

#include "tresourcedialog.h"
#include "ui_tresourcedialog.h"
#include "terror.h"

TResourceDialog::TResourceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TResourceDialog)
{
    ui->setupUi(this);

    ui->comboBoxStyle->setItemIcon(0, QIcon(":/images/view-list-tree.svg"));
    ui->comboBoxStyle->setItemIcon(1, QIcon(":/images/view-list-details.svg"));
    ui->comboBoxStyle->setItemIcon(2, QIcon(":/images/view-list-icons.svg"));
    ui->comboBoxStyle->setCurrentIndex(mShowType);
}

TResourceDialog::~TResourceDialog()
{
    delete ui;
}

void TResourceDialog::on_pushButtonCut_clicked()
{

}


void TResourceDialog::on_pushButtonCopy_clicked()
{

}


void TResourceDialog::on_pushButtonPaste_clicked()
{

}


void TResourceDialog::on_pushButtonDelete_clicked()
{

}


void TResourceDialog::on_pushButtonRename_clicked()
{

}


void TResourceDialog::on_pushButtonUndo_clicked()
{

}


void TResourceDialog::on_pushButtonRedo_clicked()
{

}


void TResourceDialog::on_pushButtonImport_clicked()
{

}


void TResourceDialog::on_pushButtonExport_clicked()
{

}


void TResourceDialog::on_pushButtonDataMap_clicked()
{

}


void TResourceDialog::on_comboBoxStyle_currentIndexChanged(int index)
{
    DECL_TRACER("TResourceDialog::on_comboBoxStyle_currentIndexChanged(int index)");

    mShowType = static_cast<SHOW_t>(index);
}


void TResourceDialog::on_listViewImages_activated(const QModelIndex &index)
{

}


void TResourceDialog::on_tableViewDynamicImages_activated(const QModelIndex &index)
{

}


void TResourceDialog::on_tableViewDynamicImages_doubleClicked(const QModelIndex &index)
{

}


void TResourceDialog::on_tableViewSounds_activated(const QModelIndex &index)
{

}


void TResourceDialog::on_tableViewDynamicDataSources_activated(const QModelIndex &index)
{

}

