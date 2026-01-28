#ifndef TBITMAPSELECTDIALOG_H
#define TBITMAPSELECTDIALOG_H

#include <QDialog>
#include "tconfmain.h"

namespace Ui {
    class TBitmapSelectDialog;
}

class QStandardItemModel;

class TBitmapSelectDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TBitmapSelectDialog(QWidget *parent = nullptr);
        ~TBitmapSelectDialog();

        void setTemporaryPath(const QString& path) { mPathTemporary = path; }

    private slots:
        void on_pushButtonImport_clicked();
        void on_listViewImages_clicked(const QModelIndex &index);
        void on_listViewDynamic_clicked(const QModelIndex &index);
        void on_tabWidget_tabBarClicked(int index);

    private:
        QPixmap sizeImage(const QSize& size, const QString& file);
        void addDynamicResource(const ConfigMain::RESOURCE_t& res, QStandardItemModel *model);

        Ui::TBitmapSelectDialog *ui;

        QList<QString> mImages;
        QString mPathTemporary;
        QList<ConfigMain::RESOURCE_t> mDynamicResources;
        int mCurrentTab{0};
};

#endif // TBITMAPSELECTDIALOG_H
