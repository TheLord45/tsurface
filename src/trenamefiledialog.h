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
#ifndef TRENAMEFILEDIALOG_H
#define TRENAMEFILEDIALOG_H

#include <QDialog>

namespace Ui {
class TRenameFileDialog;
}

class TRenameFileDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TRenameFileDialog(QWidget *parent = nullptr);
        TRenameFileDialog(const QString& file, QWidget *parent = nullptr);
        ~TRenameFileDialog();

        void setFileName(const QString& file);
        QString& getFileName() { return mFileName; }

    protected:
        void accept() override;

    private slots:
        void on_lineEditFile_textChanged(const QString &arg1);

    private:
        Ui::TRenameFileDialog *ui;
        QString mFileName;
};

#endif // TRENAMEFILEDIALOG_H
