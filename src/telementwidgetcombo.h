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
#ifndef TELEMENTWIDGETCOMBO_H
#define TELEMENTWIDGETCOMBO_H

#include <QComboBox>

class TElementWidgetCombo : public QComboBox
{
    Q_OBJECT

    public:
        TElementWidgetCombo(QWidget *parent=nullptr);
        TElementWidgetCombo(const QString& name, QWidget *parent=nullptr);
        ~TElementWidgetCombo();

        void addData(const QList<QVariant>& data);
        void setName(const QString& name);
        void setDefaultText(const QString& text);
        void setDefaultText(const QString& text, const QVariant& data);
        QString& actText() { return mActText; }
        int actIndex() { return mActIndex; }
        QVariant& actData() { return mActData; }

    signals:
        void selectionChanged(const QString& text, const QVariant& data, const QString& name);

    protected:
        void onCurrentTextChanged(const QString &text);
        void onCurrentIndexChanged(int index);

    private:
        void init();

        QString mActText;
        QVariant mActData;
        QString mName;
        int mActIndex{0};
        bool mHaveData{false};
};

#endif // TELEMENTWIDGETCOMBO_H
