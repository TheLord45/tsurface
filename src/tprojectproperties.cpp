#include "tprojectproperties.h"
#include "ui_tprojectproperties.h"

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
