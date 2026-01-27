#ifndef TBITMAPSELECTDIALOG_H
#define TBITMAPSELECTDIALOG_H

#include <QDialog>

namespace Ui {
class TBitmapSelectDialog;
}

class TBitmapSelectDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TBitmapSelectDialog(QWidget *parent = nullptr);
        ~TBitmapSelectDialog();

    private slots:
        void on_pushButtonImport_clicked();
        void on_listViewImages_clicked(const QModelIndex &index);
        void on_listViewDynamic_clicked(const QModelIndex &index);
        void on_tabWidget_tabBarClicked(int index);

    private:
        Ui::TBitmapSelectDialog *ui;
};

#endif // TBITMAPSELECTDIALOG_H
