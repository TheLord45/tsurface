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
#ifndef TELEMENTSOUND_H
#define TELEMENTSOUND_H

#include <QWidget>

class QLabel;
class QToolButton;

class TElementSound : public QWidget
{
    Q_OBJECT

    public:
        TElementSound(QWidget *parent=nullptr);
        TElementSound(const QString& sound, const QString& name, QWidget *parent=nullptr);

        QString getSound() const;
        void setSound(const QString& sound);
        void setInstance(int inst) { mInstance = inst; }
        int getInstance() { return mInstance; }

    signals:
        void soundFileChanged(const QString& sound, const QString& name);
        void soundFileChangedInst(const QString& sound, const QString& name, int instance);

    protected:
        void onLineEditTextChanged(const QString& txt);
        void onPushButtonClicked();

    private:
        void init();

        QLabel *mLine{nullptr};
        QToolButton *mButton{nullptr};

        QString mName;
        QString mSound;
        int mInstance{-1};
};

#endif // TELEMENTSOUND_H
