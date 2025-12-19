#ifndef TNEWPROJECTDIALOG_H
#define TNEWPROJECTDIALOG_H

#include <QDialog>

namespace Ui {
class TNewProjectDialog;
}

class TNewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TNewProjectDialog(QWidget *parent = nullptr);
    ~TNewProjectDialog();

private:
    Ui::TNewProjectDialog *ui;
};

#endif // TNEWPROJECTDIALOG_H
