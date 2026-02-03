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
#ifndef TDRAWTEXT_H
#define TDRAWTEXT_H

#include <QObject>

#include "tobjecthandler.h"

class QLabel;

class TDrawText
{
    public:
        TDrawText(QWidget *widget);
        TDrawText(QWidget *widget, const QString& text, const QFont& font, ObjHandler::ORIENTATION ori=ObjHandler::ORI_CENTER_MIDDLE, int x=0, int y=0);

        void setText(const QString& text) { mText = text; }
        void setFont(const QFont& font) { mFont = font; }
        void setFontSize(int pts) { mFont.setPointSize(pts); }
        void setOrientation(ObjHandler::ORIENTATION ori) { mOrientation = ori; }
        void setAbolutePosition(int x, int y);
        void setTextColor(const QColor& color) { mColor = color; };
        void setTextEffect(int effect, const QColor& color) { mTextEffect = effect; mTextEffectColor = color; };
        bool draw();

    protected:
        void drawTextEffect(int number);

    private:
        QWidget *mWidget{nullptr};
        QFont mFont;
        QString mText;
        ObjHandler::ORIENTATION mOrientation{ObjHandler::ORI_CENTER_MIDDLE};
        int mX{0};
        int mY{0};
        QColor mColor{qRgb(0, 0, 0)};
        int mTextEffect{0};
        QColor mTextEffectColor{qRgb(0, 0, 0)};

        QLabel *mLabel{nullptr};
};

#endif // TDRAWTEXT_H
