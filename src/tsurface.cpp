#include "tsurface.h"
#include "ui_tsurface.h"

tsurface::tsurface(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::tsurface)
{
    m_ui->setupUi(this);
}

tsurface::~tsurface() = default;

#include "moc_tsurface.cpp"
