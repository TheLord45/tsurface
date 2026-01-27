#include "tbitmapselectdialog.h"
#include "ui_tbitmapselectdialog.h"

TBitmapSelectDialog::TBitmapSelectDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TBitmapSelectDialog)
{
    ui->setupUi(this);
}

TBitmapSelectDialog::~TBitmapSelectDialog()
{
    delete ui;
}

void TBitmapSelectDialog::on_pushButtonImport_clicked()
{

}


void TBitmapSelectDialog::on_listViewImages_clicked(const QModelIndex &index)
{

}


void TBitmapSelectDialog::on_listViewDynamic_clicked(const QModelIndex &index)
{

}


void TBitmapSelectDialog::on_tabWidget_tabBarClicked(int index)
{

}

