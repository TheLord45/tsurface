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

/**
 * @brief The ShadowLabel class
 * The class is based on QLabel to draw some effects for text.
 * It can draw glowing and shadows. The colors of text and for glowing or
 * shadow is setable. The class supports different sizes for the effects. For
 * glowing as well as outline are 4 steps available. For the shadows are 8 steps
 * available. The class supports 3 types of shadow: hard, medium and soft.
 * "Hard" is a sharp shadow of the text. "Medium" is with a little bit more
 * blure and "soft" is with even more blur.
 * The class is using the class @class QGraphicsDropShadowEffect to draw the
 * shadows. Where the outline effect is also a shadow but the text is zentered
 * in it. The outline effect and the shadow effect can be combined.
 *
 * The methods @b setTextColor() and @b setTextEffectColor() are setting the
 * colors only to internal variables. To activate them the method @b exec()
 * must be called.
 * The method @b setShadowType() should be called as the last one because it
 * starts drawing immediately. If the colors for the shadow and/or the text
 * effect color was not set the default colors are used.
 */
class ShadowLabel : public QLabel
{
    Q_OBJECT

    public:
        ShadowLabel(QWidget *parent = nullptr);
        ~ShadowLabel();

        void setShadowType(int number);
        void setTextColor(const QColor &color);
        void setTextEffectColor(const QColor& color);
        void exec();

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

/**
 * @brief The TDrawText class
 * This class is the frontend class to draw a text. Optional the text can be
 * drawn with different effects like glowing, outline or shadows. Beside the
 * possibility to set a text effect it is possible to set the colors for the
 * text and the effect.
 */
class TDrawText
{
    public:
        TDrawText(ObjHandler::TBASEOBJ_t *object);
        TDrawText(ObjHandler::TBASEOBJ_t *object, const QString& text, const QFont& font, ObjHandler::ORIENTATION ori=ObjHandler::ORI_CENTER_MIDDLE, int x=0, int y=0);
        TDrawText(const ObjHandler::TOBJECT_t& object);
        ~TDrawText();

        /**
         * @brief setText
         * Set text to draw.
         *
         * @param text  Any string.
         */
        void setText(const QString& text) { mText = text; }
        /**
         * @brief setFont
         * The font to use to draw a text.
         *
         * @param font  Any valid font.
         */
        void setFont(const QFont& font) { mFont = font; }
        /**
         * @brief setFontSize
         * The font size to use. If this is not set the result is not defined.
         * It depends whether there was already a size set or not.
         *
         * @param pts   The font size in points.
         */
        void setFontSize(int pts) { mFont.setPointSize(pts); }
        /**
         * @brief setOrientation
         * The orientation of the text. This defines the position of text.
         *
         * @param ori   Orientation of text.
         */
        void setOrientation(ObjHandler::ORIENTATION ori) { mOrientation = ori; }
        void setAbolutePosition(int x, int y);
        /**
         * @brief setTextColor
         * The color of the text. If this is not set the default color black is
         * used.
         *
         * @param color Any valid color. Default: black
         */
        void setTextColor(const QColor& color) { mColor = color; };
        /**
         * @brief setTextEffect
         * Set the text effect and the color to use for the effect.
         * The effect must be a number between 1 and 56.
         *
         * @param effect    The text effect. A number between  and 56.
         * @param color     Any valid color.
         */
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
        QColor mTextEffectColor{qRgb(255, 0, 0)};

        ShadowLabel *mLabel{nullptr};
};

#endif // TDRAWTEXT_H
