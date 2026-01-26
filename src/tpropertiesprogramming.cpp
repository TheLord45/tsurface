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
#include <QTableWidget>
#include <QStandardItemModel>
#include <QComboBox>
#include <QSpinBox>
#include <QIntValidator>

#include "tpropertiesprogramming.h"
#include "tconfmain.h"
#include "tvalidateport.h"
#include "terror.h"

TPropertiesProgramming::TPropertiesProgramming()
{
    DECL_TRACER("TPropertiesProgramming::TPropertiesProgramming()");

    mValidatePort = new TValidatePort(this);
    mIntValidator = new QIntValidator(this);
}

TPropertiesProgramming::TPropertiesProgramming(QTableWidget *table)
    : mTable(table)
{
    DECL_TRACER("TPropertiesProgramming::TPropertiesProgramming(QTableWidget *table)");

    mValidatePort = new TValidatePort(this);
    mIntValidator = new QIntValidator(this);
}

TPropertiesProgramming::~TPropertiesProgramming()
{
    DECL_TRACER("TPropertiesProgramming::~TPropertiesProgramming()");

    delete mValidatePort;
    delete mIntValidator;
}

void TPropertiesProgramming::setProgrammingPage(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::setProgrammingPage(const QString& name)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_PROGRAM);

    mChanged = false;
    mPage = Page::PAGE_t();
    mPage = TPageHandler::Current().getPage(name);
    setProgrammingPage(mPage.pageID, true);
}

void TPropertiesProgramming::setProgrammingPage(int id, bool loaded)
{
    DECL_TRACER("TPropertiesProgramming::setProgrammingPage(int id, bool loaded)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    if (!loaded && mPage.pageID > 0 && mChanged)
    {
        saveChangedData(&mPage, TBL_PROGRAM);
        mPage = Page::PAGE_t();
    }

    mChanged = false;

    if (!loaded)
        mPage = TPageHandler::Current().getPage(id);

    if (mPage.pageID <= 0)
        return;

    createPage();
}

void TPropertiesProgramming::setProgrammingPopup(const QString& name)
{
    DECL_TRACER("TPropertiesProgramming::setProgrammingPopup(const QString& name)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_PROGRAM);

    mChanged = false;
    mPage = Page::PAGE_t();
    mPage = TPageHandler::Current().getPage(name);
    setProgrammingPopup(mPage.pageID, true);
}

void TPropertiesProgramming::setProgrammingPopup(int id, bool loaded)
{
    DECL_TRACER("TPropertiesProgramming::setProgrammingPopup(int id, bool loaded)");

    if (!mTable)
    {
        MSG_ERROR("Class was not properly initialized. Missing pointer to \"QTableWidget\"!")
        return;
    }

    if (!loaded && mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_PROGRAM);

    mChanged = false;

    if (!loaded)
        mPage = TPageHandler::Current().getPage(id);

    if (mPage.pageID <= 0)
        return;

    createPage();
}

void TPropertiesProgramming::clear()
{
    DECL_TRACER("TPropertiesProgramming::clear()");

    if (mPage.pageID > 0 && mChanged)
        saveChangedData(&mPage, TBL_PROGRAM);

    mChanged = false;
    mPage = Page::PAGE_t();
    mTable->clear();
    mTable->setRowCount(0);
    mTable->setColumnCount(0);
}

void TPropertiesProgramming::createPage()
{
    DECL_TRACER("TPropertiesProgramming::createPage()");

    int setupPort = TConfMain::Current().getSetupPort();
    mInitialized = false;
    QBrush brush;
    brush.setColor(Qt::GlobalColor::lightGray);
    mTable->clear();
    mTable->setColumnCount(2);
    mTable->setRowCount(4);

    for (int i = 0; i < 4; ++i)
    {
        QTableWidgetItem *cell1 = new QTableWidgetItem;

        cell1->setBackground(brush);

        switch(i)
        {
            case 0:
                cell1->setText(tr("Address Port"));
                mAddrPort = new QComboBox;
                mAddrPort->setEditable(true);
                mAddrPort->addItem(tr("%1 - setup port").arg(setupPort), setupPort);
                mAddrPort->addItem(tr("1"), 1);
                mAddrPort->setCurrentIndex(1);
                mAddrPort->setDuplicatesEnabled(false);
                mAddrPort->setValidator(mValidatePort);
                mAddrPort->setObjectName(QString("ComboBox_%1").arg(i));
                connect(mAddrPort, &QComboBox::currentIndexChanged, this, &TPropertiesProgramming::onComboAddrPort);
                connect(mAddrPort, &QComboBox::currentTextChanged, this, &TPropertiesProgramming::onComboAddrPortText);
                mTable->setCellWidget(i, 1, mAddrPort);
            break;

            case 1:
                cell1->setText(tr("Address Code"));
                mAddrCode = new QComboBox;
                mAddrCode->setEditable(true);
                mAddrCode->addItem("none", 0);
                mAddrCode->addItem("1", 1);
                mAddrCode->setDuplicatesEnabled(false);
                mAddrCode->setValidator(mIntValidator);
                mAddrCode->setObjectName(QString("ComboBox_%1").arg(i));
                connect(mAddrCode, &QComboBox::currentIndexChanged, this, &TPropertiesProgramming::onComboAddrCode);
                connect(mAddrCode, &QComboBox::currentTextChanged, this, &TPropertiesProgramming::onComboAddrCodeText);
                mTable->setCellWidget(i, 1, mAddrCode);
            break;

            case 2:
                cell1->setText(tr("Channel Port"));
                mChanPort = new QComboBox;
                mChanPort->setEditable(true);
                mChanPort->addItem(QString("%1 - setup port").arg(setupPort), setupPort);
                mChanPort->addItem("1", 1);
                mChanPort->setCurrentIndex(1);
                mChanPort->setDuplicatesEnabled(false);
                mChanPort->setValidator(mValidatePort);
                mChanPort->setObjectName(QString("ComboBox_%1").arg(i));
                connect(mChanPort, &QComboBox::currentIndexChanged, this, &TPropertiesProgramming::onComboChanPort);
                connect(mChanPort, &QComboBox::currentTextChanged, this, &TPropertiesProgramming::onComboChanPortText);
                mTable->setCellWidget(i, 1, mChanPort);
            break;

            case 3:
                cell1->setText(tr("Channel Code"));
                mChanCode = new QComboBox;
                mChanCode->setEditable(true);
                mChanCode->addItem("none", 0);
                mChanCode->addItem("1", 1);
                mChanCode->setDuplicatesEnabled(false);
                mChanCode->setValidator(mIntValidator);
                mChanCode->setObjectName(QString("ComboBox_%1").arg(i));
                connect(mChanCode, &QComboBox::currentIndexChanged, this, &TPropertiesProgramming::onComboChanCode);
                connect(mChanCode, &QComboBox::currentTextChanged, this, &TPropertiesProgramming::onComboChanCodeText);
                mTable->setCellWidget(i, 1, mChanCode);
            break;
        }

        cell1->setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEnabled);
        mTable->setItem(i, 0, cell1);
    }

    mInitialized = true;
}

void TPropertiesProgramming::onComboAddrPort(int index)
{
    DECL_TRACER("TPropertiesProgramming::onComboAddrPort(int index)");

    mPage.ap = mAddrPort->itemText(index).toInt();
    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboAddrPortText(const QString& text)
{
    DECL_TRACER("TPropertiesProgramming::onComboAddrPortText(const QString& text)")

    if (text.endsWith("setup port"))
        mPage.ap = TConfMain::Current().getSetupPort();
    else
        mPage.ap = text.toInt();

    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboAddrCode(int index)
{
    DECL_TRACER("TPropertiesProgramming::onComboAddrCode(int index)");

    mPage.ad = mAddrCode->itemText(index).toInt();
    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboAddrCodeText(const QString& text)
{
    DECL_TRACER("TPropertiesProgramming::onComboAddrCodeText(const QString& text)");

    if (text == "none")
        mPage.ad = 0;
    else
        mPage.ad = text.toInt();

    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboChanPort(int index)
{
    DECL_TRACER("TPropertiesProgramming::onComboChanPort(int index)");

    mPage.cp = mChanPort->itemText(index).toInt();
    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboChanPortText(const QString& text)
{
    DECL_TRACER("TPropertiesProgramming::onComboChanPortText(const QString& text)");

    if (text.endsWith("setup port"))
        mPage.cp = TConfMain::Current().getSetupPort();
    else
        mPage.cp = text.toInt();

    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboChanCode(int index)
{
    DECL_TRACER("TPropertiesProgramming::onComboChanCode(int index)");

    mPage.ch = mChanCode->itemText(index).toInt();
    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}

void TPropertiesProgramming::onComboChanCodeText(const QString& text)
{
    DECL_TRACER("TPropertiesProgramming::onComboChanCodeText(const QString& text)");

    if (text == "none")
        mPage.ch = 0;
    else
        mPage.ch = text.toInt();

    mChanged = true;
    saveChangedData(&mPage, TBL_PROGRAM);
}
