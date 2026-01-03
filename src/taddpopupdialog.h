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
#ifndef TADDPOPUPDIALOG_H
#define TADDPOPUPDIALOG_H

#include <QDialog>

namespace Ui {
    class TAddPopupDialog;
}

class TAddPopupDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit TAddPopupDialog(QWidget *parent = nullptr);
        ~TAddPopupDialog();

        QString& getPopupName() { return mPopupName; }
        QString& getGroupName() { return mGroupName; }
        void setGroupNames(const QStringList& groups);
        int getPositionLeft() { return mLeft; }
        int getPositionTop() { return mTop; }
        int getSizeWidth() { return mWidth; }
        int getSizeHeight() { return mHeight; }
        QColor getColorPageBackground() { return mColorPageBackground; }
        void setColorPageBackground(const QColor& col);
        QColor getColorBorder() { return mColorBorder; }
        void setColorBorder(const QColor& col);
        QColor getColorText() { return mColorText; }
        void setColorText(const QColor& col);
        QFont& getFont() { return mFont; }
        void setFont(const QFont& font);
        int getFontSize() { return mSize; }
        void setFontSize(int size);
        void setPageSize(const QSize& size);

        bool isStandard() { return mStandardSub == false; }
        bool isSubPage() { return mStandardSub == true; }

    protected:
        QString styleSheetColor();
        void initFontSizes();

    private slots:
        void on_lineEditPopupName_textChanged(const QString &arg1);
        void on_comboBoxGroupName_currentIndexChanged(int index);
        void on_comboBoxGroupName_editTextChanged(const QString &arg1);
        void on_spinBoxPositionLeft_valueChanged(int arg1);
        void on_spinBoxPositionTop_valueChanged(int arg1);
        void on_spinBoxSizeWidth_valueChanged(int arg1);
        void on_spinBoxSizeHeight_valueChanged(int arg1);
        void on_toolButtonColorPageBackground_clicked();
        void on_toolButtonColorBorder_clicked();
        void on_toolButtonColorText_clicked();
        void on_radioButtonTypeStandard_toggled(bool checked);
        void on_radioButtonTypeSubPage_toggled(bool checked);
        void on_comboBoxBorder_currentIndexChanged(int index);
        void on_fontComboBoxFontName_currentFontChanged(const QFont &f);
        void on_comboBoxFontSize_currentIndexChanged(int index);

    private:
        Ui::TAddPopupDialog *ui;

        bool mInitialized{false};
        QString mPopupName;
        QString mGroupName;
        int mLeft{0};
        int mTop{0};
        int mWidth{0};
        int mHeight{0};
        QSize mPageSize;
        QColor mColorPageBackground{qRgb(255,255,255)};
        QColor mColorBorder{qRgb(255, 0, 0)};   // Color red by default
        QColor mColorText{qRgb(0, 0, 0)};
        bool mStandardSub{false};           // FALSE = Standard; TRUE = Sub-Page
        QFont mFont;
        int mSize{10};
};

#endif // TADDPOPUPDIALOG_H
