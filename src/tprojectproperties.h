#ifndef TPROJECTPROPERTIES_H
#define TPROJECTPROPERTIES_H

#include <QDialog>

namespace Ui {
    class TProjectProperties;
}

class TProjectProperties : public QDialog
{
        Q_OBJECT

    public:
        explicit TProjectProperties(QWidget *parent = nullptr);
        ~TProjectProperties();

    private:
        Ui::TProjectProperties *ui;
};

#endif // TPROJECTPROPERTIES_H
