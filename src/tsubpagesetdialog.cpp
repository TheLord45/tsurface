#include <QStandardItemModel>
#include <QInputDialog>
#include <QMessageBox>

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
    ui->tableViewSubPagesRight->verticalHeader()->hide();
    ui->tableViewSubPageSets->horizontalHeader()->hide();
    ui->tableViewSubPageSets->verticalHeader()->hide();
    ui->tableViewSubPagesLeft->horizontalHeader()->hide();
    ui->tableViewSubPagesLeft->verticalHeader()->hide();
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

    for (iter = mSubPageSet.begin(); iter != mSubPageSet.end(); ++iter)
    {
        QStandardItem *cell1 = new QStandardItem;
        cell1->setEditable(false);
        cell1->setText(iter->name);
        modelSet->setItem(row, 0, cell1);
        row++;
    }

    ui->tableViewSubPageSets->setModel(modelSet);
    ui->tableViewSubPagesLeft->setModel(modelItems);
    ui->tableViewSubPagesRight->setModel(modelPages);

    QSignalBlocker sigBlock(this);

    if (!mSubPageSet.isEmpty())
    {
        ui->lineEditSetName->setText(mSubPageSet[0].name);
        setSlotSize(mSubPageSet[0].pgWidth, mSubPageSet[0].pgHeight);
        showItems(mSubPageSet[0].items);
    }
    // Select first entry in Sub-Page Sets, if there is any.
    if (modelSet->rowCount() > 0)
    {
        ui->tableViewSubPageSets->selectRow(0);
        ui->pushButtonDelete->setEnabled(true);
        mSelectedSet = 0;
    }

    ui->tableViewSubPageSets->resizeColumnsToContents();
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

    if (!ok || name.isEmpty())
        return;

    // Check name for duplicates
    for (ConfigMain::SUBPAGESET_t set : mSubPageSet)
    {
        if (set.name == name)
        {
            QMessageBox::critical(this, tr("Duplicate set"), tr("The page set <b>%1</b> exists already!").arg(name));
            return;
        }
    }

    ConfigMain::SUBPAGESET_t set;
    set.id = TConfMain::Current().getNextSubPageSetID();
    set.name = name;
    set.pgHeight = set.pgWidth = 0;
    mSubPageSet.append(set);
    setSlotSize(0, 0);
    appendNameToSet(name);
    showPages(mSubPageNames);
    mChanged = true;
}

void TSubPageSetDialog::on_pushButtonDelete_clicked()
{

}

void TSubPageSetDialog::on_lineEditSetName_textChanged(const QString &arg1)
{
    DECL_TRACER("TSubPageSetDialog::on_lineEditSetName_textChanged(const QString &arg1)");

    // This will rename the actual selected set only.
    if (mSelectedSet < 0 || mBlock)
        return;

    // Check if the new name does not exist already
    int index = 0;

    for (ConfigMain::SUBPAGESET_t set : mSubPageSet)
    {
        if (index != mSelectedSet && set.name == arg1)
        {
            QMessageBox::critical(this, tr("Error"), tr("The name <b>%1</b> is duplicate! Ignoring it.").arg(arg1));
            return;
        }

        index++;
    }

    mSubPageSet[mSelectedSet].name = arg1;
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewSubPageSets->model());

    if (model)
    {
        model->item(mSelectedSet, 0)->setText(arg1);
        ui->tableViewSubPageSets->resizeColumnsToContents();
    }

    mChanged = true;
}

void TSubPageSetDialog::on_pushButtonAdd_clicked()
{
    DECL_TRACER("TSubPageSetDialog::on_pushButtonAdd_clicked()");

    if (mSelectedSet < 0 || mSelectedPage < 0)
        return;

    // TODO: Add code to move page
    Page::PAGE_t page = TPageHandler::Current().getSubPage(mSubPageNames[mSelectedPage]);

    if (page.pageID <= 0)
    {
        MSG_ERROR("Page \"" << mSubPageNames[mSelectedPage].toStdString() << "\" not found!");
        return;
    }

    appendPageToLeft(mSubPageNames[mSelectedPage], page.width, page.height);
    QStringList leftPages = mSubPageNames;
    removeFromList(&leftPages, mSubPageNames[mSelectedPage]);
    ConfigMain::SUBPAGEITEMS_t item;
    item.pageID = page.pageID;
    item.pageName = mSubPageNames[mSelectedPage];
    item.index = mSubPageSet[mSelectedSet].items.size();
    mSubPageSet[mSelectedSet].items.append(item);
    showPages(leftPages);
    mChanged = true;
}

void TSubPageSetDialog::on_pushButtonRemove_clicked()
{
    DECL_TRACER("TSubPageSetDialog::on_pushButtonRemove_clicked()");

    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewSubPagesLeft->model());

    if (mSelectedItem < 0 || mSelectedSet < 0 || !model || mSelectedItem >= model->rowCount())
    {
        ui->pushButtonRemove->setDisabled(true);
        return;
    }

    QString pageName = model->item(mSelectedItem, 0)->text();
    model->removeRows(mSelectedItem, 1);
    appendPageToRight(pageName);

    QList<ConfigMain::SUBPAGEITEMS_t>::Iterator iter;

    for (iter = mSubPageSet[mSelectedSet].items.begin(); iter != mSubPageSet[mSelectedSet].items.end(); ++iter)
    {
        if (iter->pageName == pageName)
        {
            mSubPageSet[mSelectedSet].items.erase(iter);
            break;
        }
    }

    mChanged = true;
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
    MSG_DEBUG("Selected row: " << mSelectedSet);

    if (mSelectedSet < 0 || mSelectedSet >= mSubPageSet.size())
    {
        MSG_ERROR("Selection of " << mSelectedSet << " is out of range!");
        mSelectedSet = -1;
        ui->pushButtonDelete->setDisabled(true);
        return;
    }

    QStandardItemModel *modelLeft = static_cast<QStandardItemModel *>(ui->tableViewSubPagesLeft->model());

    if (modelLeft && modelLeft->rowCount() > 0)
        modelLeft->removeRows(0, modelLeft->rowCount());

    QItemSelectionModel *selModel = ui->tableViewSubPageSets->selectionModel();

    if (selModel && selModel->hasSelection())
    {
        ui->pushButtonDelete->setEnabled(true);
        ui->lineEditSetName->setText(mSubPageSet[mSelectedSet].name);
        setSlotSize(mSubPageSet[mSelectedSet].pgWidth, mSubPageSet[mSelectedSet].pgHeight);
        showItems(mSubPageSet[mSelectedSet].items);
    }
    else
        ui->pushButtonDelete->setDisabled(true);
}

void TSubPageSetDialog::on_tableViewSubPagesLeft_clicked(const QModelIndex &index)
{
    DECL_TRACER("TSubPageSetDialog::on_tableViewSubPagesLeft_clicked(const QModelIndex &index)");

    mSelectedItem = index.row();
    QItemSelectionModel *selModel = ui->tableViewSubPagesLeft->selectionModel();

    if (selModel && selModel->hasSelection())
        ui->pushButtonRemove->setEnabled(true);
    else
        ui->pushButtonRemove->setDisabled(true);
}

void TSubPageSetDialog::on_tableViewSubPagesRight_clicked(const QModelIndex &index)
{
    DECL_TRACER("TSubPageSetDialog::on_tableViewSubPagesRight_clicked(const QModelIndex &index)");

    mSelectedPage = index.row();
    QItemSelectionModel *selModel = ui->tableViewSubPagesRight->selectionModel();

    if (selModel && selModel->hasSelection())
        ui->pushButtonAdd->setEnabled(true);
    else
        ui->pushButtonAdd->setDisabled(true);
}

void TSubPageSetDialog::accept()
{
    DECL_TRACER("TSubPageSetDialog::accept()");

    TConfMain::Current().setSubPageSet(mSubPageSet);
    done(QDialog::Accepted);
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
    int row = model->rowCount();
    model->setItem(row, 0, cell1);
    mBlock = true;
    ui->lineEditSetName->setText(name);
    ui->tableViewSubPageSets->selectRow(row);
    ui->tableViewSubPageSets->resizeColumnsToContents();
    // A new set name rquires the table of selected pages to be empty.
    model = static_cast<QStandardItemModel *>(ui->tableViewSubPagesLeft->model());
    row = model->rowCount();
    model->removeRows(0, row);
    mBlock = false;
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
        Page::PAGE_t pg = TPageHandler::Current().getSubPage(*itPages);
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
        model->setItem(row, 1, cell2);
        model->setItem(row, 2, cell3);
        row++;
    }

    ui->tableViewSubPagesRight->resizeColumnsToContents();
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
    QStringList leftPages = mSubPageNames;

    for (itItems = items.cbegin(); itItems != items.cend(); ++itItems)
    {
        QStandardItem *cell1 = new QStandardItem;
        cell1->setEditable(false);
        cell1->setText(itItems->pageName);
        model->setItem(line, 0, cell1);
        removeFromList(&leftPages, itItems->pageName);
        line++;
    }

    showPages(leftPages);
    ui->tableViewSubPagesLeft->resizeColumnsToContents();
}

void TSubPageSetDialog::setSlotSize(int width, int height)
{
    DECL_TRACER("TSubPageSetDialog::setSlotSize(int width, int height)");

    MSG_DEBUG("Slot size: " << width << " x " << height);
    ui->labelWidth->setNum(width);
    ui->labelHeight->setNum(height);
}

void TSubPageSetDialog::appendPageToLeft(const QString& name, int width, int height)
{
    DECL_TRACER("TSubPageSetDialog::appendPageToLeft(const QString& name, int width, int height)");

    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewSubPagesLeft->model());

    if (!model)
        return;

    QStandardItem *cell1 = new QStandardItem;
    cell1->setEditable(false);
    cell1->setText(name);
    model->setItem(model->rowCount(), 0, cell1);
    ui->tableViewSubPagesLeft->resizeColumnsToContents();
    setSlotSize(width, height);
}

void TSubPageSetDialog::appendPageToRight(const QString& name)
{
    DECL_TRACER("TSubPageSetDialog::appendPageToRight(const QString& name)");


    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewSubPagesRight->model());

    if (!model)
        return;

    Page::PAGE_t pg = TPageHandler::Current().getSubPage(name);
    QStandardItem *cell1 = new QStandardItem;
    QStandardItem *cell2 = new QStandardItem;
    QStandardItem *cell3 = new QStandardItem;

    cell1->setEditable(false);
    cell2->setEditable(false);
    cell3->setEditable(false);

    cell1->setText(name);

    cell2->setText(QString("%1").arg(pg.width));
    cell2->setTextAlignment(Qt::AlignRight);

    cell3->setText(QString("%1").arg(pg.height));
    cell3->setTextAlignment(Qt::AlignRight);

    int row = model->rowCount();
    model->setItem(row, 0, cell1);
    model->setItem(row, 1, cell2);
    model->setItem(row, 2, cell3);

    ui->tableViewSubPagesRight->resizeColumnsToContents();
}
