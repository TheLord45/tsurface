#include <QStandardItemModel>
#include <QInputDialog>

#include "tsubpagesetdialog.h"
#include "ui_tsubpagesetdialog.h"
#include "tpagehandler.h"
#include "terror.h"

TSubPageSetDialog::TSubPageSetDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TSubPageSetDialog)
{
    DECL_TRACER("TSubPageSetDialog::TSubPageSetDialog(QWidget *parent)");

    ui->setupUi(this);
    ui->tableViewSubPagesRight->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->pushButtonDelete->setDisabled(true);
    ui->pushButtonAdd->setDisabled(true);
    ui->pushButtonRemove->setDisabled(true);
    ui->pushButtonMoveUp->setDisabled(true);
    ui->pushButtonMoveDown->setDisabled(true);

    mSubPageSet = TConfMain::Current().getSubPageSetList();
    mSubPageNames = TPageHandler::Current().getSubPageNames();
    QStandardItemModel *modelSet = new QStandardItemModel(this);
    QStandardItemModel *modelItems = new QStandardItemModel(this);
    QStandardItemModel *modelPages = new QStandardItemModel(this);
    modelSet->setColumnCount(1);
    modelItems->setColumnCount(1);
    modelPages->setColumnCount(3);

    QStringList headers;
    headers << tr("Sub-Page") << tr("Width") << tr("Height");
    modelPages->setHorizontalHeaderLabels(headers);

    QList<ConfigMain::SUBPAGESET_t>::Iterator iter;
    int row = 0;
    QStringList leftPages = mSubPageNames;

    for (iter = mSubPageSet.begin(); iter != mSubPageSet.end(); ++iter)
    {
        QStandardItem *cell1 = new QStandardItem;
        cell1->setEditable(false);
        cell1->setText(iter->name);
        modelSet->setItem(row, 0, cell1);
        // Add the items of this set
        QList<ConfigMain::SUBPAGEITEMS_t>::Iterator itItems;
        int line = 0;

        for (itItems = iter->items.begin(); itItems != iter->items.end(); ++itItems)
        {
            QStandardItem *cell1 = new QStandardItem;
            cell1->setEditable(false);
            cell1->setText(itItems->pageName);
            modelItems->setItem(line, 0, cell1);

            if (row == 0)   // The first one will be selected on startup
                removeFromList(&leftPages, itItems->pageName);

            line++;
        }

        row++;
    }

    ui->tableViewSubPageSets->setModel(modelSet);
    ui->tableViewSubPagesLeft->setModel(modelItems);
    ui->tableViewSubPagesRight->setModel(modelPages);

    showPages(leftPages);
    // Select first entry in Sub-Page Sets, if there is any.
    if (modelSet->rowCount() > 0)
    {
        ui->tableViewSubPageSets->selectRow(0);
        ui->pushButtonDelete->setEnabled(true);
    }

    ui->tableViewSubPageSets->resizeColumnsToContents();
    ui->tableViewSubPagesLeft->resizeColumnsToContents();
    ui->tableViewSubPagesRight->resizeColumnsToContents();
}

TSubPageSetDialog::~TSubPageSetDialog()
{
    DECL_TRACER("TSubPageSetDialog::~TSubPageSetDialog()");

    delete ui;
}

void TSubPageSetDialog::on_pushButtonNew_clicked()
{
    DECL_TRACER("TSubPageSetDialog::on_pushButtonNew_clicked()");

    bool ok = false;
    QString name = QInputDialog::getText(this, tr("Create SubPage Set"), tr("Set Name:"), QLineEdit::Normal, "", &ok);

    if (!ok)
        return;

    appendNameToSet(name);
    showPages(mSubPageNames);
}

void TSubPageSetDialog::on_pushButtonDelete_clicked()
{

}

void TSubPageSetDialog::on_lineEditSetName_textChanged(const QString &arg1)
{

}

void TSubPageSetDialog::on_pushButtonAdd_clicked()
{
    DECL_TRACER("TSubPageSetDialog::on_pushButtonAdd_clicked()");

    if (mSelectedSet < 0)
        return;

    // TODO: Add code to move page
}

void TSubPageSetDialog::on_pushButtonRemove_clicked()
{

}

void TSubPageSetDialog::on_pushButtonMoveUp_clicked()
{

}

void TSubPageSetDialog::on_pushButtonMoveDown_clicked()
{

}

void TSubPageSetDialog::on_tableViewSubPageSets_clicked(const QModelIndex &index)
{
    DECL_TRACER("TSubPageSetDialog::on_tableViewSubPageSets_clicked(const QModelIndex &index)");

    mSelectedSet = index.row();

}

void TSubPageSetDialog::on_tableViewSubPagesLeft_clicked(const QModelIndex &index)
{
    DECL_TRACER("TSubPageSetDialog::on_tableViewSubPagesLeft_clicked(const QModelIndex &index)");

    mSelectedItem = index.row();
}

void TSubPageSetDialog::on_tableViewSubPagesRight_clicked(const QModelIndex &index)
{
    DECL_TRACER("TSubPageSetDialog::on_tableViewSubPagesRight_clicked(const QModelIndex &index)");

    mSelectedPage = index.row();
}

void TSubPageSetDialog::removeFromList(QStringList *list, const QString& term)
{
    DECL_TRACER("TSubPageSetDialog::removeFromList(QStringList *list, const QString& term)");

    if (!list || list->empty())
        return;

    QStringList::Iterator iter;

    for (iter = list->begin(); iter != list->end(); ++iter)
    {
        if (iter->compare(term) == 0)
        {
            list->erase(iter);
            return;
        }
    }
}

void TSubPageSetDialog::appendNameToSet(const QString& name)
{
    DECL_TRACER("TSubPageSetDialog::appendNameToSet(const QString& name)");

    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewSubPageSets->model());

    if (!model)
        return;

    QStandardItem *cell1 = new QStandardItem;
    cell1->setEditable(false);
    cell1->setText(name);
    model->setItem(model->rowCount(), 0, cell1);
}

void TSubPageSetDialog::showPages(const QStringList& pages)
{
    DECL_TRACER("TSubPageSetDialog::showPages(const QStringList& pages)");

    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewSubPagesRight->model());

    if (!model)
        return;

    model->removeRows(0, model->rowCount());
    QStringList::ConstIterator itPages;
    int row = 0;

    for (itPages = pages.cbegin(); itPages != pages.cend(); ++itPages)
    {
        Page::PAGE_t pg = TPageHandler::Current().getPage(*itPages);
        QStandardItem *cell1 = new QStandardItem;
        QStandardItem *cell2 = new QStandardItem;
        QStandardItem *cell3 = new QStandardItem;

        cell1->setEditable(false);
        cell2->setEditable(false);
        cell3->setEditable(false);

        cell1->setText(*itPages);

        cell2->setText(QString("%1").arg(pg.width));
        cell2->setTextAlignment(Qt::AlignRight);

        cell3->setText(QString("%1").arg(pg.height));
        cell3->setTextAlignment(Qt::AlignRight);

        model->setItem(row, 0, cell1);
        row++;
    }
}

void TSubPageSetDialog::showItems(const QList<ConfigMain::SUBPAGEITEMS_t>& items)
{
    DECL_TRACER("TSubPageSetDialog::showItems(const QList<ConfigMain::SUBPAGEITEMS_t>& items)");

    ui->labelWidth->setNum(0);
    ui->labelHeight->setNum(0);
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewSubPagesLeft->model());

    if (!model)
        return;

    if (model->rowCount() > 0)
        model->removeRows(0, model->rowCount());

    QList<ConfigMain::SUBPAGEITEMS_t>::ConstIterator itItems;
    int line = 0;
    int width = 0;
    int height = 0;

    for (itItems = items.cbegin(); itItems != items.cend(); ++itItems)
    {
        QStandardItem *cell1 = new QStandardItem;
        cell1->setEditable(false);
        cell1->setText(itItems->pageName);
        model->setItem(line, 0, cell1);

        Page::PAGE_t *pg = TPageHandler::Current().getPage(itItems->pageID);

        if (pg)
        {
            if (pg->width > width)
                width = pg->width;

            if (pg->height > height)
                height = pg->height;
        }

        line++;
    }

    ui->labelWidth->setNum(width);
    ui->labelHeight->setNum(height);
}
