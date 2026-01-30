/*
 * Copyright (C) 2026 by Andreas Theofilu <andreas@theosys.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QBrush>
#include <QComboBox>
#include <QColorDialog>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QHeaderView>
#include <QFontDialog>
#include <QSpinBox>

#include "tpropertiesstates.h"
#include "ttextboxdialog.h"
#include "telementbitmapselector.h"
#include "telementwidgetcombo.h"
#include "terror.h"

using namespace Page;

TPropertiesStates::TPropertiesStates(QTreeWidget *widget)
    : mTreeWidget(widget)
{
    DECL_TRACER("TPropertiesStates::TPropertiesStates(QTreeWidget *widget)");
}

TPropertiesStates::~TPropertiesStates()
{
    DECL_TRACER("TPropertiesStates::~TPropertiesStates()");
}

void TPropertiesStates::setGeometry(int bi, const QRect& geom)
{
    DECL_TRACER("TPropertiesStates::setGeometry(int bi, const QRect& geom)");

    for (TObjectHandler *obj : mPage.objects)
    {
        if (obj->getButtonIndex() == bi)
        {
            obj->setSize(geom);
            break;
        }
    }
}

void TPropertiesStates::setStatesPage(const QString& name)
{
    DECL_TRACER("TPropertiesStates::setStatesPage(const QString& name)");

    if (!mTreeWidget)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTreeWidget\"!");
        return;
    }

    Page::PAGE_t page = TPageHandler::Current().getPage(name);

    if (page.pageID == mPage.pageID)
        return;

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_STATES);

    mChanged = false;
    mPage = page;
    setStatesPage(mPage.pageID, true);
}

void TPropertiesStates::setStatesPage(int id, bool loaded)
{
    DECL_TRACER("TPropertiesStates::setStatesPage(int id, bool loaded)");

    if (!mTreeWidget)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTreeWidget\"!")
        return;
    }

    Page::PAGE_t page;

    if (!loaded)
    {
        page = TPageHandler::Current().getPage(id);

        if (page.pageID == mPage.pageID)
            return;
    }

    if (!loaded && mPage.pageID > 0 && mChanged)
    {
        saveChangedData(&mPage, TBL_STATES);
        mPage = Page::PAGE_t();
    }

    mChanged = false;

    if (!loaded)
        mPage = page;

    if (mPage.pageID <= 0)
        return;

    createPage();
}

void TPropertiesStates::setActualButton(int bi)
{
    DECL_TRACER("TPropertiesStates::setActualButton(int bi)");

    if (bi <= 0 || bi >= mPage.objects.size() || mActualObjectID == bi)
        return;

    mActualObjectID = bi;
    // TODO: Add code to show the states for an object
}

void TPropertiesStates::update()
{

}

void TPropertiesStates::clear()
{
    DECL_TRACER("TPropertiesStates::clear()");

    if (mBlocked)
        return;

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_STATES);

    mChanged = true;
    mPage = Page::PAGE_t();
    mTreeWidget->clear();
    mTreeWidget->setColumnCount(0);
}

void TPropertiesStates::createPage()
{
    DECL_TRACER("TPropertiesStates::createPage()");

    if (mBlocked)
        return;

    if (!mTreeWidget)
    {
        MSG_WARNING("Tree widget is not initialized!")
        return;
    }

    mInitialized = false;
    QBrush brush;
    brush.setColor(Qt::GlobalColor::lightGray);
    mTreeWidget->clear();
    mTreeWidget->setColumnCount(1);
    mTreeWidget->setHeaderHidden(true);

    if (mPage.popupType == PT_PAGE)
    {
        QTreeWidgetItem *top = new QTreeWidgetItem(mTreeWidget);
        top->setText(0, tr("Off"));

        QTreeWidgetItem *item = new QTreeWidgetItem(top);
        item->setFirstColumnSpanned(true);
        item->setFlags(Qt::ItemIsEnabled);      // Not selectable

        QTableWidget *table = createTableWidget(STATE_PAGE);
        mTreeWidget->setItemWidget(item, 0, table);
        mTreeWidget->expandAll();
    }
    else if (mPage.popupType == PT_POPUP)
    {
        QTreeWidgetItem *top = new QTreeWidgetItem(mTreeWidget);
        top->setText(0, tr("Off"));

        QTreeWidgetItem *item = new QTreeWidgetItem(top);
        item->setFirstColumnSpanned(true);
        item->setFlags(Qt::ItemIsEnabled);      // Not selectable

        QTableWidget *table = createTableWidget(STATE_POPUP, mTreeWidget);
        mTreeWidget->setItemWidget(item, 0, table);
        mTreeWidget->expandAll();
    }

    mInitialized = true;
}

QTableWidget *TPropertiesStates::createTableWidget(STATE_TYPE stype, QWidget *parent)
{
    DECL_TRACER("TPropertiesStates::createTableWidget(STATE_TYPE stype, QWidget *parent)");

    if (mBlocked)
        return nullptr;

    QBrush brush;
    brush.setColor(Qt::GlobalColor::lightGray);
    QTableWidget *table = new QTableWidget(parent);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setVisible(false);
    table->setShowGrid(true);
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table->setColumnCount(2);
    int rows = 0;

    if (stype == STATE_PAGE)
        rows = 12;
    else if (stype == STATE_POPUP)
        rows = 15;
    else if (stype == STATE_BUTTON)
        rows = 17;
    else if (stype == STATE_BARGRAPH)
        rows = 16;

    // The row count must be defined before rows are added to the table! Otherwise
    // an empty table with only the grid will be visible.
    table->setRowCount(rows);

    for (int row = 0; row < rows; ++row)
    {
        QTableWidgetItem *col0 = new QTableWidgetItem;
        col0->setBackground(brush);
        col0->setText(getLeftColText(stype, 0, row));
        col0->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);
        table->setItem(row, 0, col0);

        switch(row)
        {
            case 0:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeFillType(mPage.srPage.ft, "PgFillType"));
            break;

            case 1:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeColorSelector(mPage.srPage.cf, "PgFillColor"));
            break;

            case 2:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeColorSelector(mPage.srPage.ct, "PgTextColor"));
            break;

            case 3:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeColorSelector(mPage.srPage.ec, "PgTextEffectColor"));
            break;

            case 4:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeVideoFill(mPage.srPage.vf, "PgVideoFill"));
            break;

            case 5:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeBitmapSelector(bitmapArrayToList(mPage.srPage.bitmaps), "PgBitmapSelector"));
            break;

            case 6:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeFontSelector(mPage.srPage.ff, "PgFontSelector"));
            break;

            case 7:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeValueSelector(mPage.srPage.fs, "PgFontSize"));
            break;

            case 8:
                if (stype == STATE_PAGE)
                {
                    QFont font;

                    if (mPage.srPage.ff.isEmpty())
                    {
                        font = mParent->font();
                        mPage.srPage.ff = font.family();
                    }

                    if (mPage.srPage.fs <= 0)
                        font.setPointSize(10);
                    else
                        font.setPointSize(mPage.srPage.fs);

                    table->setCellWidget(row, 1, makeTextValue(mPage.srPage.te, font, "PgText"));
                }
            break;

            case 9:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeTextJustification(mPage.srPage.jt, "PgTextOrientation"));
            break;

            case 10:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeTextEffect(mPage.srPage.et, "PgTextEffect"));
            break;

            case 11:
                if (stype == STATE_PAGE)
                    table->setCellWidget(row, 1, makeWordWrap(mPage.srPage.ww, "PgWordWrap"));
            break;
        }
    }

    table->resizeRowsToContents();
    return table;
}

QString TPropertiesStates::getLeftColText(STATE_TYPE stype, int state, int line)
{
    DECL_TRACER("TPropertiesStates::getLeftColText(Page::STATE_TYPE stype, int state, int line)");

    if (stype == STATE_PAGE)
    {
        switch(line)
        {
            case 0: return tr("Fill Type");
            case 1: return tr("Fill Color");
            case 2: return tr("Text Color");
            case 3: return tr("Text Effect Color");
            case 4: return tr("Video Fill");
            case 5: return tr("Bitmaps");
            case 6: return tr("Font");
            case 7: return tr("Font Size");
            case 8: return tr("Text");
            case 9: return tr("Text Justification");
            case 10: return tr("Text Effect");
            case 11: return tr("Word Wrap");
        }
    }
    else if (stype == STATE_POPUP)
    {
        switch(line)
        {
            case 0: return tr("Border Name");
            case 1: return tr("Border Color");
            case 2: return tr("Fill Type");
            case 3: return tr("Fill Color");
            case 4: return tr("Text Color");
            case 5: return tr("Text Effect Color");
            case 6: return tr("Overall Opacity");
            case 7: return tr("Video Fill");
            case 8: return tr("Bitmaps");
            case 9: return tr("Font");
            case 10: return tr("Font Size");
            case 11: return tr("Text");
            case 12: return tr("Text Justification");
            case 13: return tr("Text Effect");
            case 14: return tr("Word Wrap");
        }
    }
    else if (stype == STATE_BUTTON)   // All states
    {
        switch(line)
        {
            case 0: return tr("Border Name");
            case 1: return tr("Chameleon Image");   // Only if there is no border
            case 2: return tr("Border Color");
            case 3: return tr("Fill Type");
            case 4: return tr("Fill Color");
            case 5: return tr("Text Color");
            case 6: return tr("Text Effect Color");
            case 7: return tr("Overall Opacity");
            case 8: return tr("Video Fill");
            case 9: return tr("Bitmaps");
            case 10: return tr("Font");
            case 11: return tr("Font Size");
            case 12: return tr("Text");
            case 13: return tr("Text Justification");
            case 14: return tr("Text Effect");
            case 15: return tr("Word Wrap");
            case 16: return tr("Sound");
        }
    }
    else if (stype == STATE_BARGRAPH)   // All states
    {
        switch(line)
        {
            case 0: return tr("Border Name");
            case 1: return tr("Chameleon Image");   // Only if there is no border
            case 2: return tr("Border Color");
            case 3: return tr("Fill Type");
            case 4: return tr("Fill Color");
            case 5: return tr("Text Color");
            case 6: return tr("Text Effect Color");
            case 7: return tr("Overall Opacity");
            case 8: return tr("Video Fill");
            case 9: return tr("Bitmaps");
            case 10: return tr("Font");
            case 11: return tr("Font Size");
            case 12: return tr("Text");
            case 13: return tr("Text Justification");
            case 14: return tr("Text Effect");
            case 15: return tr("Word Wrap");
        }
    }

    return "";
}

QComboBox *TPropertiesStates::makeFillType(const QString& ftype, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeFillType(int ftype, const QString& name)");

    QList<QString> list = {
        "solid", "radial", "sweep", "left to right",
        "top-left to bottom-right", "top to bottom",
        "top-right to bottom-left", "right to left",
        "bottom-right to top-left", "bottom to top"
    };

    QComboBox *cbox = new QComboBox;
    cbox->setObjectName(name);
    int idx = 0;

    for (QString txt : list)
    {
        cbox->addItem(txt);

        if (txt == ftype)
            cbox->setCurrentIndex(idx);

        idx++;
    }

    connect(cbox, &QComboBox::currentIndexChanged, [this, cbox](int index) {
        mBlocked = true;
        QString objName = cbox->objectName();
        QString item = cbox->itemText(index);
        setValue(objName, item);
        mBlocked = false;
    });

    return cbox;
}

QWidget *TPropertiesStates::makeColorSelector(const QColor& col, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeColorSelector(const QColor& col, const QString& name)");

    QWidget *widget = new QWidget;
    widget->setAccessibleIdentifier(col.name(QColor::HexArgb));
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    QLabel *label = new QLabel;
    label->setObjectName(name);
    label->setStyleSheet(QString("background-color: %1").arg(col.name()));
    layout->addWidget(label, 1);

    QPushButton *button = new QPushButton;
    button->setObjectName("PushButtonColorSelector");
    button->setText("...");
    layout->addWidget(button);

    connect(button, &QPushButton::clicked, [this, label, col](bool) {
        mBlocked = true;
        QColor color = col;
        setColor(label, color);
        setValue(label->objectName(), color);
        mBlocked = false;
    });

    return widget;
}

QWidget *TPropertiesStates::makeVideoFill(const QString& vf, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeVideoFill(const QString& vf, const QString& name)");

    QList<QString> items = { "none", "streaming video", "MXA-MPL" };

    QComboBox *cbox = new QComboBox;
    cbox->setObjectName(name);
    int idx = 0;

    for (QString txt : items)
    {
        cbox->addItem(txt);

        if (txt == vf)
            cbox->setCurrentIndex(idx);
    }

    connect(cbox, &QComboBox::currentIndexChanged, [this, cbox](int index) {
        mBlocked = true;
        QString vf = cbox->itemText(index);
        setValue(cbox->objectName(), vf);
        mBlocked = false;
    });

    return cbox;
}

TElementBitmapSelector *TPropertiesStates::makeBitmapSelector(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeBitmapSelector(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name)");

    TElementBitmapSelector *bs = new TElementBitmapSelector(name, bitmaps, mParent);
    connect(bs, &TElementBitmapSelector::bitmapsChanged, this, &TPropertiesStates::onBitmapsChanged);
    return bs;
}

QWidget *TPropertiesStates::makeFontSelector(const QString& fname, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeFontSelector(const QString& fname, const QString& name)");

    QWidget *widget = new QWidget;

    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    QLineEdit *line = new QLineEdit;
    line->setObjectName(name);
    QFont font;

    if (fname.isEmpty())
    {
        font = line->font();
        line->setText(font.family());
    }
    else
    {
        font = QFont(fname);
        line->setText(fname);
    }

    layout->addWidget(line, 1);

    QPushButton *button = new QPushButton;
    button->setText("...");
    button->setMaximumWidth(30);
    layout->addWidget(button);

    connect(button, &QPushButton::clicked, [this, font, line](bool) {
        mBlocked = true;
        QFont f = chooseFont(font);
        line->setText(f.family());
        mBlocked = false;
    });

    return widget;
}

QSpinBox *TPropertiesStates::makeValueSelector(int value, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeValueSelector(int value, const QString& name)");

    QSpinBox *spin = new QSpinBox;
    spin->setObjectName(name);
    spin->setValue(value);

    connect(spin, &QSpinBox::valueChanged, [this, spin](int value) { setValue(spin->objectName(), value); });
    return spin;
}

QWidget *TPropertiesStates::makeTextValue(const QString& txt,  const QFont& font, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeTextValue(const QString& txt,  const QFont& font, const QString& name)");

    QWidget *widget = new QWidget;

    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    QLineEdit *line = new QLineEdit;
    line->setObjectName(name);
    line->setText(txt);

    QPushButton *button = new QPushButton;
    button->setText("...");
    button->setMaximumWidth(30);

    layout->addWidget(line);
    layout->addWidget(button);

    connect(line, &QLineEdit::textChanged, [this, line](const QString& text) {
        setValue(line->objectName(), text);
    });

    connect(button, &QPushButton::clicked, [this, line, font]() {
        mBlocked = true;
        TTextBoxDialog dialog(mParent);
        dialog.setTextFont(line->text(), font);

        if (dialog.exec() == QDialog::Rejected)
            return;

        line->setText(dialog.getText());
        setValue(line->objectName(), dialog.getText());
        mBlocked = false;
    });

    return widget;
}

TElementWidgetCombo *TPropertiesStates::makeTextJustification(ObjHandler::ORIENTATION ori, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeTextJustification(ObjHandler::ORIENTATION ori, const QString& name)");

    QList<QString> items = { "absolute", "top-left", "top-middle", "top-right",
                            "center-left", "center-middle", "center-right",
                            "bottom-left", "bottom-middle", "bottom-right" };
    QList<QVariant> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    TElementWidgetCombo *combo = new TElementWidgetCombo(name, mParent);
    combo->addItems(items);
    combo->addData(data);
    combo->setCurrentIndex(5);
    connect(combo, &TElementWidgetCombo::selectionChanged, this, &TPropertiesStates::onOrientationChanged);
    return combo;
}

QTreeWidget *TPropertiesStates::makeTextEffect(int ef, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeTextEffect(int ef, const QString& name)");

    return new QTreeWidget;
}

QComboBox *TPropertiesStates::makeWordWrap(bool ww, const QString& name)
{
    DECL_TRACER("TPropertiesStates::makeWordWrap(bool ww, const QString& name)");

    QComboBox *cbox = new QComboBox;
    cbox->setObjectName(name);
    cbox->addItem(tr("no"), false);
    cbox->addItem(tr("yes"), true);

    if (ww)
        cbox->setCurrentIndex(1);

    return cbox;
}

QFont TPropertiesStates::chooseFont(const QFont& font)
{
    DECL_TRACER("TPropertiesStates::chooseFont(const QFont& font)");

    bool ok = false;
    QFont f = QFontDialog::getFont(&ok, font, mParent, tr("Select font"));

    if (ok)
        return f;

    return font;
}

void TPropertiesStates::setValue(const QString& name, const QVariant& value)
{
    DECL_TRACER("TPropertiesStates::setValue(const QString& name, const QVariant& value)");

    if (name == "PgFillType")
        mPage.srPage.ft = value.toString();

    mChanged = true;
    markChanged();
    // TODO: Call to draw immediately
}

void TPropertiesStates::setColor(QLabel *label, QColor& color)
{
    DECL_TRACER("TPropertiesStates::setColor(QLabel *label, QColor& color)");

    QColorDialog colDialog(mParent);
    colDialog.setCurrentColor(color);

    if (colDialog.exec() == QDialog::Rejected)
        return;

    color = colDialog.selectedColor();
    label->setStyleSheet(QString("background-color: %1").arg(color.name(QColor::HexArgb)));
}

QList<ObjHandler::BITMAPS_t> TPropertiesStates::bitmapArrayToList(const ObjHandler::BITMAPS_t bitmaps[])
{
    DECL_TRACER("TPropertiesStates::bitmapArrayToList(const ObjHandler::BITMAPS_t bitmaps[])");

    QList<ObjHandler::BITMAPS_t> list;

    for (int i = 0; i < 5; ++i)
    {
        if (bitmaps[i].fileName.isEmpty())
            continue;

        list.append(bitmaps[i]);
    }

    return list;
}

// Callbacks

void TPropertiesStates::onBitmapsChanged(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onBitmapsChanged(const QList<ObjHandler::BITMAPS_t>& bitmaps, const QString& name)");

    MSG_DEBUG("Bitmap for name: " << name.toStdString());

    if (name == "PgBitmapSelector")
    {
        int i = 0;

        for (ObjHandler::BITMAPS_t bm : bitmaps)
        {
            if (i >= 5)
                break;

            mPage.srPage.bitmaps[i] = bm;
            i++;
        }
    }
}

void TPropertiesStates::onOrientationChanged(const QString& text, const QVariant& data, const QString& name)
{
    DECL_TRACER("TPropertiesStates::onOrientationChanged(const QString& text, const QVariant& data, const QString& name)");

    setValue(name, data);
}
