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
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsView>

#include "tdrawtext.h"
#include "tgraphics.h"
#include "terror.h"

#define LABEL_NAME  "LabelText"

using namespace ObjHandler;

TDrawText::TDrawText(QWidget *widget)
    : mWidget(widget)
{
    DECL_TRACER("TDrawText::TDrawText(QWidget *widget)");
}

TDrawText::TDrawText(QWidget *widget, const QString& text, const QFont& font, ORIENTATION ori, int x, int y)
    : mWidget(widget),
      mText(text),
      mFont(font),
      mOrientation(ori),
      mX(x),
      mY(y)
{
    DECL_TRACER("TDrawText::TDrawText(QWidget *widget, const QString& text, const QFont& font, ORIENTATION ori, int x, int y)");
}

void TDrawText::setAbolutePosition(int x, int y)
{
    DECL_TRACER("TDrawText::setAbolutePosition(int x, int y)");

    if (x < 0 || y < 0)
        return;

    mX = x;
    mY = y;
}

bool TDrawText::draw()
{
    DECL_TRACER("TDrawText::draw()");

    if (!mWidget || mText.isEmpty())
        return false;

    MSG_DEBUG("Drawing text: " << mText.toStdString());
    bool haveLabel = false;
    QObjectList objects = mWidget->children();

    for (QObject *obj : objects)
    {
        if (obj->objectName() == LABEL_NAME)
        {
            mLabel = static_cast<QLabel *>(obj);
            haveLabel = true;
            break;
        }
    }

    if (!haveLabel)
    {
        MSG_DEBUG("Adding a new label to widget ...");
        mLabel = new QLabel(mWidget);
        mLabel->setObjectName(LABEL_NAME);
//        mLabel->setMargin(0);
        mLabel->move(0, 0);
        mLabel->setFixedSize(mWidget->size());
        QRect rect = mLabel->rect();
        MSG_DEBUG("Size of label: " << rect.x() << ", " << rect.y() << ", " << rect.width() << ", " << rect.height());

//        QPalette palette(mWidget->palette());
//        palette.setColor(QPalette::Window, Qt::transparent);
//        mLabel->setPalette(palette);
        mLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    mLabel->setFont(mFont);
    mLabel->setText(mText);
    mLabel->setStyleSheet(QString("color: %1").arg(mColor.name(QColor::HexArgb)));
    MSG_DEBUG("Using text color: " << mColor.name(QColor::HexArgb).toStdString());

    switch(mOrientation)
    {
        case ORI_ABSOLUT:
        {
            QRect rect = mWidget->geometry();
            mLabel->setGeometry(QRect(mX, mY, rect.width() - mX, rect.height() - mY));
            mLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        }
        break;

        case ORI_TOP_LEFT:      mLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft); break;
        case ORI_TOP_MIDDLE:    mLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter); break;
        case ORI_TOP_RIGHT:     mLabel->setAlignment(Qt::AlignTop | Qt::AlignRight); break;
        case ORI_CENTER_LEFT:   mLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft); break;
        case ORI_CENTER_MIDDLE: mLabel->setAlignment(Qt::AlignCenter); break;
        case ORI_CENTER_RIGHT:  mLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight); break;
        case ORI_BOTTOM_LEFT:   mLabel->setAlignment(Qt::AlignBottom | Qt::AlignLeft); break;
        case ORI_BOTTOM_MIDDLE: mLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter); break;
        case ORI_BOTTOM_RIGHT:  mLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight); break;

        default:
            mLabel->setAlignment(Qt::AlignCenter);
    }

    if (mTextEffect > 0)
        drawTextEffect(mTextEffect);

    mLabel->show();
    return true;
}

void TDrawText::drawTextEffect(int number)
{
    DECL_TRACER("TDrawText::drawTextEffect()");

    Graphics::EFFECT_STYLE_t style = TGraphics::Current().getEffectDetails(number);

    if (style.number != number)
        return;

    MSG_DEBUG("Using effect: " << number);
    // Drop shadow
    if (number >= 9 && number <= 32)
    {
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
        shadow->setColor(mTextEffectColor);
        shadow->setOffset(style.startx, style.starty);
        shadow->setBlurRadius(style.width);
        shadow->setEnabled(true);
        mLabel->setGraphicsEffect(shadow);
        MSG_DEBUG("Offset: " << style.startx << ", " << style.starty);
        MSG_DEBUG("Radius: " << style.width);
    }
}
