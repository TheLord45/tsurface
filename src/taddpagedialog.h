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
#ifndef TADDPAGEDIALOG_H
#define TADDPAGEDIALOG_H

#include <QDialog>

namespace Ui {
    class TAddPageDialog;
}

class TAddPageDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit TAddPageDialog(QWidget *parent = nullptr);
        ~TAddPageDialog();

        // Getter / Setter
        QString& getPageName() { return mPageName; }

        void setFont(const QFont& font);
        QFont& getFont() { return mFont; }
        void setFontSize(int size);
        int getFontSize() { return mSize; }
        void setColorBackground(const QColor& col);
        QColor& getColorBackground() { return mColorBackground; }
        void setColorText(const QColor& col);
        QColor& getColorText() { return mColorText; }

    private slots:
        void on_lineEditPageName_textChanged(const QString &arg1);
        void on_toolButtonColorBackground_clicked();
        void on_toolButtonColorText_clicked();
        void on_fontComboBoxFontName_currentFontChanged(const QFont& f);
        void on_comboBoxFontSize_currentIndexChanged(int index);

    private:
        Ui::TAddPageDialog *ui;

        QString mPageName;
        QFont mFont;
        int mSize{0};
        QColor mColorBackground;
        QColor mColorText;
};

#endif // TADDPAGEDIALOG_H
