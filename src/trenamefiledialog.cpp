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
#include <QMessageBox>

#include "trenamefiledialog.h"
#include "ui_trenamefiledialog.h"
#include "terror.h"

TRenameFileDialog::TRenameFileDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TRenameFileDialog)
{
    ui->setupUi(this);
}

TRenameFileDialog::TRenameFileDialog(const QString& file, QWidget *parent)
    : QDialog(parent),
      mFileName(file),
      ui(new Ui::TRenameFileDialog)
{
    DECL_TRACER("TRenameFileDialog::TRenameFileDialog(const QString& file, QWidget *parent)");

    ui->setupUi(this);
    ui->lineEditFile->setText(file);
}

TRenameFileDialog::~TRenameFileDialog()
{
    delete ui;
}

void TRenameFileDialog::on_lineEditFile_textChanged(const QString &arg1)
{
    DECL_TRACER("TRenameFileDialog::on_lineEditFile_textChanged(const QString &arg1)");

    mFileName = arg1;
    QString invalChars("!\"$%&\\()?*`´;<>|'");
    QString::Iterator iter;

    for (iter = invalChars.begin(); iter != invalChars.end(); ++iter)
    {
        if (mFileName.contains(*iter))
        {
            mFileName.remove(*iter);
            ui->lineEditFile->setText(mFileName);
            break;
        }
    }
}

void TRenameFileDialog::accept()
{
    DECL_TRACER("TRenameFileDialog::accept()");

    if (!mFileName.isEmpty())
    {
        if (!mFileName.endsWith(".png") && !mFileName.endsWith(".jpg"))
        {
            QMessageBox::warning(this, tr("Invalid file name"), tr("The file name must either end with the extension <b>.png</b> or <b>.jpg</b>!"));
            return;
        }
    }

    done(QDialog::Accepted);
}

void TRenameFileDialog::setFileName(const QString& file)
{
    DECL_TRACER("TRenameFileDialog::setFileName(const QString& file)");

    ui->lineEditFile->setText(file);
    mFileName = file;
}
