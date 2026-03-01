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
#include "tcanvaswidget.h"
#include "tfonts.h"
#include "terror.h"

#define LABEL_NAME  "LabelText"

using namespace ObjHandler;

ShadowLabel::ShadowLabel(QWidget *parent)
            : QLabel(parent)
{
    DECL_TRACER("ShadowLabel::ShadowLabel(QWidget *parent)");

    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QLabel { background: transparent; }");
}

void ShadowLabel::setShadowType(int number)
{
    DECL_TRACER("ShadowLabel::setShadowType(int number)");

    mTextEffect = number;
    mStyle = TGraphics::Current().getEffectDetails(number);
    updateEffect();
}

void ShadowLabel::setTextColor(const QColor &color)
{
    DECL_TRACER("ShadowLabel::setTextColor(const QColor &color)");

    mTextColor = color;
    update();
}

void ShadowLabel::setTextEffectolor(const QColor& color)
{
    DECL_TRACER("ShadowLabel::setTextEffectolor(const QColor& color)");

    mTextEffectColor = color;
    update();
}

// Outline: 1 - 4
// Glow: 5 - 8
// Soft Drop Shadow: 9 - 16
// Medium Drop Shadow: 17 - 24
// Hard Drop Shadow: 25 - 32
// Soft Drop Shadow with outline: 33 - 40
// Medium Drop Shadow with Outline: 41 - 48
// Hard Drop Shadow with Outline: 49 - 56
void ShadowLabel::paintEvent(QPaintEvent *event)
{
    DECL_TRACER("ShadowLabel::paintEvent(QPaintEvent *event)");

    if ((mTextEffect >= 1 && mTextEffect <= 4) ||   // Outline
        (mTextEffect >= 33 && mTextEffect <= 56))
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setFont(font());
        QPen pen(mTextEffectColor, mTextEffect);
        p.setPen(pen);

        for (int dx = ((mStyle.width / 2) * -1); dx <= (mStyle.width / 2); ++dx)
        {
            for (int dy = ((mStyle.height / 2) * -1); dy <= (mStyle.height / 2); ++dy)
            {
                if (dx == 0 && dy == 0)
                    continue;

                p.drawText(rect().translated(dx, dy), alignment(), text());
            }
        }

        p.setPen(mTextColor);
        p.drawText(rect(), alignment(), text());
    }
    else
    {
        if (graphicsEffect())
        {
            // Shadow effect active, just draw text with textColor
            QPainter p(this);
            p.setRenderHint(QPainter::Antialiasing);
            p.setPen(mTextColor);
            p.setFont(font());
            p.drawText(rect(), alignment(), text());
        }
        else
            QLabel::paintEvent(event);
    }
}

void ShadowLabel::updateEffect()
{
    DECL_TRACER("ShadowLabel::updateEffect()");

    setGraphicsEffect(nullptr);

    if (mStyle.number != mTextEffect)
        return;
    // Outline: 1 - 4
    // Glow: 5 - 8
    if (mTextEffect >= 5 && mTextEffect <= 8)   // Glow
    {
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setColor(mTextEffectColor);
        qreal sigma = 0.0;

        switch(mTextEffect)
        {
            case 5: sigma = 4.0; break;     // Glow-S
            case 6: sigma = 8.0; break;     // Glow-M
            case 7: sigma = 12.0; break;    // Glow-L
            case 8: sigma = 16.0; break;    // Glow-X
        }

        shadow->setBlurRadius(sigma);
        shadow->setOffset(0, 0);
        update();
        setGraphicsEffect(shadow);
        return;
    }

    if (mTextEffect == 0 || (mTextEffect >= 1 && mTextEffect <= 8))
    {
        update();
        return;
    }

    // Draw a shadow
    // Calculate offset
    if (mTextEffect >= 9 && mTextEffect <= 32)
    {
        QString stName = mStyle.name;
        qsizetype pos = stName.lastIndexOf(" ");

        if (pos > 0)
            mOffset = stName.remove(0, pos+1).toInt();
    }
    else if (mTextEffect > 32)
    {
        if (mTextEffect > 48)
            mOffset = mTextEffect - 48;
        else if (mTextEffect > 40)
            mOffset = mTextEffect - 40;
        else
            mOffset = mTextEffect - 32;
    }

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setColor(mTextEffectColor);
    // Soft Drop Shadow: 9 - 16
    // Medium Drop Shadow: 17 - 24
    // Hard Drop Shadow: 25 - 32
    // Soft Drop Shadow with outline: 33 - 40
    // Medium Drop Shadow with Outline: 41 - 48
    // Hard Drop Shadow with Outline: 49 - 56
    if ((mTextEffect >= 25 && mTextEffect <= 32) ||
        (mTextEffect >= 49 && mTextEffect <= 56))
        shadow->setBlurRadius(0);
    else if ((mTextEffect >= 17 && mTextEffect <= 24) ||
             (mTextEffect >= 41 && mTextEffect <= 48))
        shadow->setBlurRadius(8);
    else if ((mTextEffect >= 9 && mTextEffect <= 16) ||
             (mTextEffect >= 33 && mTextEffect <= 40))
        shadow->setBlurRadius(15);
    else
        shadow->setBlurRadius(0);

    shadow->setOffset(mOffset, mOffset);
    update();
    setGraphicsEffect(shadow);
}

//
// -----------------------------------------------------------------
//
TDrawText::TDrawText(TBASEOBJ_t *object)
    : mObject(object)
{
    DECL_TRACER("TDrawText::TDrawText(TBASEOBJ_t *object)");
}

TDrawText::TDrawText(TBASEOBJ_t *object, const QString& text, const QFont& font, ORIENTATION ori, int x, int y)
    : mObject(object),
      mText(text),
      mFont(font),
      mOrientation(ori),
      mX(x),
      mY(y)
{
    DECL_TRACER("TDrawText::TDrawText(TBASEOBJ_t *object, const QString& text, const QFont& font, ORIENTATION ori, int x, int y)");
}

TDrawText::TDrawText(const ObjHandler::TOBJECT_t& object)
    : mBtObject(object)
{
    DECL_TRACER("TDrawText::TDrawText(const ObjHandler::TOBJECT_t& object)");
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

    if (!mObject || !mObject->widget || mText.isEmpty())
        return false;

    MSG_DEBUG("Drawing text: " << mText.toStdString());
    bool haveLabel = false;
    QObjectList objects = mObject->widget->children();

    for (QObject *obj : objects)
    {
        if (obj->objectName() == LABEL_NAME)
        {
            mLabel = static_cast<ShadowLabel *>(obj);
            haveLabel = true;
            break;
        }
    }

    if (!haveLabel)
    {
        MSG_DEBUG("Adding a new label to widget ...");
        mLabel = new ShadowLabel(mObject->widget);
        mLabel->setObjectName(LABEL_NAME);
        mLabel->hide();
        mLabel->setScaledContents(true);
        mLabel->setGeometry(mObject->widget->rect());
        mLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        mLabel->setAttribute(Qt::WA_TranslucentBackground);
        mLabel->show();
    }

    mLabel->setFont(mFont);
    mLabel->setText(mText);
    mLabel->setTextColor(mColor);
    mLabel->setTextEffectolor(mTextEffectColor);
    mLabel->setShadowType(mTextEffect);
    mLabel->setStyleSheet(QString("background: transparent; color: %1").arg(mColor.name(QColor::HexArgb)));
    MSG_DEBUG("Using text color: " << mColor.name(QColor::HexArgb).toStdString());

    switch(mOrientation)
    {
        case ORI_ABSOLUT:
        {
            QRect rect = mObject->widget->geometry();
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

    mLabel->show();
    return true;
}

bool TDrawText::drawObject(QPixmap *bm, int instance)
{
    DECL_TRACER("TDrawText::drawObject(QPixmap *bm, int instance)");

    if (!bm || bm->isNull() || instance < 0 || instance >= mBtObject.sr.size())
        return false;

    if (mBtObject.bi <= 0 || mBtObject.sr[instance].te.isEmpty())
        return false;

    QPainter painter(bm);

    QFont font = TFonts::getFont(mBtObject.sr[instance].ff);
    font.setPixelSize(mBtObject.sr[instance].fs);
    painter.setFont(font);
    painter.setPen(mBtObject.sr[instance].ct);
    QRect rect(0, 0, mBtObject.wt, mBtObject.ht);
    QFlags<Qt::AlignmentFlag> aFlag = Qt::AlignCenter;

    switch(mBtObject.sr[instance].jt)
    {
        case ORI_ABSOLUT:       aFlag = Qt::AlignTop | Qt::AlignLeft; break;
        case ORI_TOP_LEFT:      aFlag = Qt::AlignTop |Qt::AlignHCenter; break;
        case ORI_TOP_MIDDLE:    aFlag = Qt::AlignTop | Qt::AlignHCenter; break;
        case ORI_TOP_RIGHT:     aFlag = Qt::AlignTop | Qt::AlignRight; break;
        case ORI_CENTER_LEFT:   aFlag = Qt::AlignVCenter | Qt::AlignLeft; break;
        case ORI_CENTER_MIDDLE: aFlag = Qt::AlignCenter; break;
        case ORI_CENTER_RIGHT:  aFlag = Qt::AlignVCenter | Qt::AlignRight; break;
        case ORI_BOTTOM_LEFT:   aFlag = Qt::AlignBottom | Qt::AlignLeft; break;
        case ORI_BOTTOM_MIDDLE: aFlag = Qt::AlignBottom | Qt::AlignHCenter; break;
        case ORI_BOTTOM_RIGHT:  aFlag = Qt::AlignBottom | Qt::AlignRight; break;

        default:
            mLabel->setAlignment(Qt::AlignCenter);
    }

    if (mBtObject.sr[instance].et > 0)
    {
        ShadowLabel label;
        label.setGeometry(rect);
        label.setFont(font);
        label.setAlignment(aFlag);
        label.setText(mBtObject.sr[instance].te);
        label.setTextColor(mBtObject.sr[instance].ct);
        label.setShadowType(mBtObject.sr[instance].et);
        label.setTextEffectolor(mBtObject.sr[instance].ec);
        QPixmap px = label.grab();
        painter.drawPixmap(0, 0, px);
    }
    else
        painter.drawText(rect, aFlag, mBtObject.sr[instance].te);

    // TODO: Continue
    return false;
}
