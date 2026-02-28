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

#include <QWidget>
#include <QLabel>

#include "tobjecthandler.h"
#include "tgraphics.h"

class ShadowLabel : public QLabel
{
    Q_OBJECT

    public:
        ShadowLabel(QWidget *parent = nullptr);

        void setShadowType(int number);
        void setTextColor(const QColor &color);
        void setTextEffectolor(const QColor& color);

    protected:
        void paintEvent(QPaintEvent *event) override;

    private:
        int mTextEffect{0};
        QColor mTextColor{Qt::black};
        QColor mTextEffectColor{Qt::red};
        int mOffset{1};
        Graphics::EFFECT_STYLE_t mStyle;

        void updateEffect();
};

class TDrawText
{
    public:
        TDrawText(ObjHandler::TBASEOBJ_t *object);
        TDrawText(ObjHandler::TBASEOBJ_t *object, const QString& text, const QFont& font, ObjHandler::ORIENTATION ori=ObjHandler::ORI_CENTER_MIDDLE, int x=0, int y=0);
        TDrawText(const ObjHandler::TOBJECT_t& object);

        void setText(const QString& text) { mText = text; }
        void setFont(const QFont& font) { mFont = font; }
        void setFontSize(int pts) { mFont.setPointSize(pts); }
        void setOrientation(ObjHandler::ORIENTATION ori) { mOrientation = ori; }
        void setAbolutePosition(int x, int y);
        void setTextColor(const QColor& color) { mColor = color; };
        void setTextEffect(int effect, const QColor& color) { mTextEffect = effect; mTextEffectColor = color; };
        bool draw();
        bool drawObject(QPixmap *bm, int instance);

    private:
        ObjHandler::TBASEOBJ_t *mObject{nullptr};
        ObjHandler::TOBJECT_t mBtObject;
        QFont mFont;
        QString mText;
        ObjHandler::ORIENTATION mOrientation{ObjHandler::ORI_CENTER_MIDDLE};
        int mX{0};
        int mY{0};
        QColor mColor{qRgb(0, 0, 0)};
        int mTextEffect{0};
        QColor mTextEffectColor{qRgb(0, 0, 0)};

        ShadowLabel *mLabel{nullptr};
};

#endif // TDRAWTEXT_H
