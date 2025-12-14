#include "tsurface.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    tsurface w;
    w.show();

    return app.exec();
}

