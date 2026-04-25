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
#ifndef TELEMENTLISTVIEWCOMPONENTS_H
#define TELEMENTLISTVIEWCOMPONENTS_H

#include <QWidget>

#define LISTVIEW_IMAGE_CELL     1
#define LISTVIEW_TEXT_PRIMARY   2
#define LISTVIEW_TEXT_SECONDARY 4

class QLineEdit;
class QToolButton;

class TElementListviewComponents : public QWidget
{
    Q_OBJECT

    public:
        TElementListviewComponents(int setting, const QString& name, QWidget *widget=nullptr);

        void setProperty(int setting);
        int getProperty() { return mSetting; }

    signals:
        void componentChanged(int components, const QString& name);

    protected:
        void onButtonClicked();
        QString makeText();

    private:
        int mSetting{0};
        QString mName;
        QLineEdit *mLine{nullptr};
        QToolButton *mButton{nullptr};
};

#endif // TELEMENTLISTVIEWCOMPONENTS_H
