#include "tnewprojectdialog.h"
#include "ui_tnewprojectdialog.h"

TNewProjectDialog::TNewProjectDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TNewProjectDialog)
{
    ui->setupUi(this);
}

TNewProjectDialog::~TNewProjectDialog()
{
    delete ui;
}
