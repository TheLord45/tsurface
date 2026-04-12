#ifndef TSUBPAGESETDIALOG_H
#define TSUBPAGESETDIALOG_H

#include <QDialog>

#include "tconfmain.h"

namespace Ui {
    class TSubPageSetDialog;
}

class QStandardItemModel;

class TSubPageSetDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit TSubPageSetDialog(QWidget *parent = nullptr);
        ~TSubPageSetDialog();

        QList<ConfigMain::SUBPAGESET_t> getSubPageSet() { return mSubPageSet; }
        bool isChanged() { return mChanged; }

    private slots:
        void on_pushButtonNew_clicked();
        void on_pushButtonDelete_clicked();
        void on_lineEditSetName_textChanged(const QString &arg1);
        void on_pushButtonAdd_clicked();
        void on_pushButtonRemove_clicked();
        void on_pushButtonMoveUp_clicked();
        void on_pushButtonMoveDown_clicked();
        void on_tableViewSubPageSets_clicked(const QModelIndex &index);
        void on_tableViewSubPagesLeft_clicked(const QModelIndex &index);
        void on_tableViewSubPagesRight_clicked(const QModelIndex &index);

    protected:
        void accept() override;

        void removeFromList(QStringList *list, const QString& term);
        void removeFromList(QStringList *list, const QStringList& names);
        void appendNameToSet(const QString& name);
        void showSets(QStandardItemModel *model=nullptr);
        void showPages(const QStringList& pages);
        void showItems(const QList<ConfigMain::SUBPAGEITEMS_t>& items);
        void setSlotSize(int width, int height);
        void appendPageToLeft(const QString& name, int width, int height);
        void appendPageToRight(const QString& name);
        void setMoveButtons();
        QString getSelectedSubPageName();
        QStringList getElectedPages();

    private:
        Ui::TSubPageSetDialog *ui;

        QList<ConfigMain::SUBPAGESET_t> mSubPageSet;
        QStringList mSubPageNames;
        int mSelectedSet{-1};
        int mSelectedItem{-1};
        int mSelectedPage{-1};
        bool mBlock{false};
        bool mChanged{false};
};

#endif // TSUBPAGESETDIALOG_H
