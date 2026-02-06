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
#ifndef TELEMENTGRADIENTCOLORS_H
#define TELEMENTGRADIENTCOLORS_H

#include <QWidget>

class QTableWidget;
class QHBoxLayout;

class TElementGradientColors : public QWidget
{
    Q_OBJECT

    public:
        TElementGradientColors(const QList<QColor>& colors, const QString& name, QWidget *parent=nullptr);

    signals:
        void gradientColorChanged(QList<QColor>& colors, const QString& name);

    protected:
        void onPushButtonClicked();

    private:
        void createLine();

        QList<QColor> mGradients;
        QString mName;
        QWidget *mParent{nullptr};
        QTableWidget *mTable{nullptr};
        QHBoxLayout *mLayout{nullptr};
};

#endif // TELEMENTGRADIENTCOLORS_H
