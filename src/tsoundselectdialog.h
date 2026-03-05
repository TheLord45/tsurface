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
#ifndef TSOUNDSELECTDIALOG_H
#define TSOUNDSELECTDIALOG_H

#include <QDialog>

class QStandardItemModel;

namespace Ui {
    class TSoundSelectDialog;
}

class TSoundSelectDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit TSoundSelectDialog(QWidget *parent = nullptr);
        ~TSoundSelectDialog();

        QString getSound() const;

    protected:
        void addSoundFile(const QString& file, QStandardItemModel *model);

    private slots:
        void on_tableViewSounds_clicked(const QModelIndex &index);
        void on_pushButtonImport_clicked();

    private:
        Ui::TSoundSelectDialog *ui;
        QString mSound;
        QStringList mSoundFiles;
};

#endif // TSOUNDSELECTDIALOG_H
