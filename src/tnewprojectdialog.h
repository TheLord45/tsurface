/*
 * Copyright (C) 2025 by Andreas Theofilu <andreas@theosys.at>
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
#ifndef TNEWPROJECTDIALOG_H
#define TNEWPROJECTDIALOG_H

#include <QDialog>

#include "tpaneltypes.h"

namespace Ui {
class TNewProjectDialog;
}

class TPanelType;

class TNewProjectDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TNewProjectDialog(QWidget *parent = nullptr);
        ~TNewProjectDialog();

        void init();

        QString& getProjectName() { return mProjectName; }
        void setProjectName(const QString& name) { mProjectName = name; }
        QString& getPanelName() { return mPanelName; }
        void setPanelName(const QString& name) { mPanelName = name; }
        TPanelType::PANELTYPE_t getPanelType() { return mPanType; }
        void setPanType(const TPanelType::PANELTYPE_t& pt) { mPanType = pt; }
        QSize& getResolution() { return mResolution; }
        QString& getPageName() { return mPageName; }
        void setPageName(const QString& text) { mPageName = text; }
        QString& getDesigner() { return mDesigner; }
        void setDesigner(const QString& text) { mDesigner = text; }
        QString& getDealer() { return mDealer; }
        void setDealer(const QString& text) { mDealer = text; }
        QString& getRevision() { return mRevision; }
        void setRevision(const QString& rev) { mRevision = rev; }
        QString& getComment() { return mComment; }
        void setComment(const QString& text) { mComment = text; }
        QColor& getColorBackground() { return mColorBackground; }
        void setColorBackground(const QColor& color) { mColorBackground = color; }
        QColor& getColorText() { return mColorText; }
        void setColorText(const QColor& color) { mColorText = color; }
        QFont& getFontName() { return mFont; }
        void setFont(const QFont& font) { mFont = font; }
        int getFontSize() { return mSize; }
        void setFontSize(int pt) { mSize = pt; }

    private slots:
        void on_lineEditJobName_textChanged(const QString &arg1);
        void on_comboBoxPanelTypes_currentIndexChanged(int index);
        void on_comboBoxResolutions_currentIndexChanged(int index);
        void on_checkBoxGenFiles_stateChanged(int arg1);
        void on_toolButtonColorBackground_clicked();
        void on_toolButtonColorText_clicked();
        void on_fontComboBox_currentFontChanged(const QFont &f);
        void on_comboBoxSize_currentIndexChanged(int index);
        void on_lineEditDesigner_textChanged(const QString &arg1);
        void on_lineEditDealer_textChanged(const QString &arg1);
        void on_lineEditRevision_textChanged(const QString &arg1);
        void on_plainTextEditComments_textChanged();
        void on_lineEditPageName_textChanged(const QString &arg1);
        void on_pushButtonFinish_clicked();
        void on_pushButtonCancel_clicked();

    private:
        Ui::TNewProjectDialog *ui;
        QWidget *mParent{nullptr};
        TPanelType *mPanelType{nullptr};
        std::vector<QSize> mResolutions;
        bool mInitialized{false};
        QString mProjectName;
        QString mPanelName;
        TPanelType::PANELTYPE_t mPanType{TPanelType::PAN_VARIA_SL50};
        QString mPageName;
        QColor mColorBackground{qRgb(0xff, 0xff, 0xff)};
        QColor mColorText{qRgb(0, 0, 0)};
        QFont mFont;
        int mSize{10};
        QSize mResolution;
        QString mDesigner;
        QString mDealer;
        QString mRevision;
        QString mComment;
};

#endif // TNEWPROJECTDIALOG_H
