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

#include "tobjecthandler.h"
#include "tdrawobject.h"
#include "terror.h"

using namespace ObjHandler;

TObjectHandler::TObjectHandler()
{
    DECL_TRACER("TObjectHandler::TObjectHandler()");
}

TObjectHandler::TObjectHandler(ObjHandler::BUTTONTYPE bt, int num, const QString& name)
{
    DECL_TRACER("TObjectHandler::TObjectHandler(ObjHandler::BUTTONTYPE bt, int num, const QString& name)");

    mObject.type = bt;
    mObject.bi = num;
    mObject.na = name;
}

/**
 * @brief TObjectHandler::getSr
 * The method takes the instance number and searches in the array for the
 * instance. If the number is found, the instance is returned.
 *
 * @param number    The number of the instance. This number starts by 1!
 *
 * @return If the instance was found the structure containing the instance
 * data is returned. Otherwise an empty structure is returned.
 */
ObjHandler::SR_T TObjectHandler::getSr(int number)
{
    DECL_TRACER("TObjectHandler::getSr(int number)");

    for (SR_T sr : mObject.sr)
    {
        if (sr.number == number)
            return sr;
    }

    return SR_T();
}

/**
 * @brief TObjectHandler::getSrFromIndex
 * This method returns an instance from the given index. If the index is
 * out of range a common object is generated. For details look at
 * @keyword getSrFromIndex().
 *
 * @param index     A valid index number.
 *
 * @return The structure containing all data of the wanted instance. If
 * the index number was out of range an empty structure is returned.
 */
ObjHandler::SR_T TObjectHandler::getSrFromIndex(int index)
{
    DECL_TRACER("TObjectHandler::getSrFromIndex(int index)");

    if (index < 0 || index >= mObject.sr.size())
        return getSrCommon();

    return mObject.sr[index];
}

/**
 * @brief TObjectHandler::getSrCommon
 * The method takes the first element in the @bold sr array and compares
 * every value with the other entries in the array. Every value contained
 * in the other instances too is copied into a virtual object.
 * Values which are different are left blank or on default value.
 *
 * @return A virtual object containing only the common values of all entries
 * in the array @bold sr.
 */
ObjHandler::SR_T TObjectHandler::getSrCommon()
{
    DECL_TRACER("TObjectHandler::getSrCommon()");

    TOBJECT_t obj = mObject;
    SR_T common = obj.sr[0];

    for (int i = 1; i < mObject.sr.size(); ++i)
    {
        if (common._do != mObject.sr[i]._do)
            common._do.clear();

        if (common.bs != mObject.sr[i].bs)
            common.bs.clear();

        if (common.mi != mObject.sr[i].mi)
            common.mi.clear();

        if (common.cb != mObject.sr[i].cb)
            common.cb = Qt::transparent;

        if (common.ft != mObject.sr[i].ft)
            common.ft.clear();

        if (common.cf != mObject.sr[i].cf)
            common.cf = Qt::transparent;

        if (common.ct != mObject.sr[i].ct)
            common.ct = Qt::transparent;

        if (common.ec != mObject.sr[i].ec)
            common.ec = Qt::transparent;

        if (!compareBitmaps(common.bitmaps, mObject.sr[i].bitmaps))
            common.bitmaps.clear();

        if (common.gradientColors != mObject.sr[i].gradientColors)
            common.gradientColors.clear();

        if (common.gr != mObject.sr[i].gr)
            common.gr = 0;

        if (common.gx != mObject.sr[i].gx)
            common.gx = 0;

        if (common.gy != mObject.sr[i].gy)
            common.gy = 0;

        if (common.sd != mObject.sr[i].sd)
            common.sd.clear();

        if (common.dynamic != mObject.sr[i].dynamic)
            common.dynamic = false;

        if (common.sb != mObject.sr[i].sb)
            common.sb = 0;

        if (common.te != mObject.sr[i].te)
            common.te.clear();

        if (common.jt != mObject.sr[i].jt)
            common.jt = ObjHandler::ORI_CENTER_MIDDLE;

        if (common.tx != mObject.sr[i].tx)
            common.tx = 0;

        if (common.ty != mObject.sr[i].ty)
            common.ty = 0;

        if (common.ff != mObject.sr[i].ff)
            common.ff.clear();

        if (common.fs != mObject.sr[i].fs)
            common.fs = 0;

        if (common.ww != mObject.sr[i].ww)
            common.ww = 0;

        if (common.et != mObject.sr[i].et)
            common.et = 0;

        if (common.oo != mObject.sr[i].oo)
            common.oo = -1;

        if (common.md != mObject.sr[i].md)
            common.md = 0;

        if (common.mr != mObject.sr[i].mr)
            common.mr = 0;

        if (common.ms != mObject.sr[i].ms)
            common.ms = 0;

        if (common.vf != mObject.sr[i].vf)
            common.vf.clear();

        if (common.dv != mObject.sr[i].dv)
            common.dv.clear();
    }

    return common;
}

/**
 * @brief TObjectHandler::setSrToAllInstances
 * This method sets all values of the structure to all instances.
 *
 * @param sr    The structure of an instance.
 */
void TObjectHandler::setSrToAllInstances(const SR_T& sr)
{
    DECL_TRACER("TObjectHandler::setSrToAllInstances(const SR_T& sr)");

    QList<SR_T>::Iterator iter;

    for (iter = mObject.sr.begin(); iter != mObject.sr.end(); ++iter)
    {
        iter->_do = sr._do;             // Order on how to show a multistate bargraph (010203...)
        iter->bs = sr.bs;               // Frame type (circle, ...)
        iter->mi = sr.mi;               // Chameleon image
        iter->cb = sr.cb;               // Border color
        iter->ft = sr.ft;               // G5: Fill type for gradient colors.
        iter->cf = sr.cf;               // Fill color
        iter->ct = sr.ct;               // Text Color
        iter->ec = sr.ec;               // Text effect color
        iter->bitmaps = sr.bitmaps;     // G5 table of bitmaps. Limited to 5 max.
        iter->gradientColors = sr.gradientColors;  // G5 optional gradient colors
        iter->gr = sr.gr;               // G5 Gradient radius
        iter->gx = sr.gx;               // G5 Gradient center X in percent
        iter->gy = sr.gy;               // G5 Gradient center Y in percent
        iter->sd = sr.sd;               // Sound file to play
        iter->dynamic = sr.dynamic;     // TRUE = moving image
        iter->sb = sr.sb;               // Index to external graphics download
        iter->te = sr.te;               // Text
        iter->jt = sr.jt;               // Text orientation
        iter->tx = sr.tx;               // Text X position
        iter->ty = sr.ty;               // Text Y position
        iter->ff = sr.ff;               // G5 font file name
        iter->fs = sr.fs;               // G5 font size
        iter->ww = sr.ww;               // line break when 1
        iter->et = sr.et;               // Text effect (^TEF)
        iter->oo = sr.oo;               // Over all opacity
        iter->md = sr.md;               // Marquee type: 1 = scroll left, 2 = scroll right, 3 = ping pong, 4 = scroll up, 5 = scroll down
        iter->mr = sr.mr;               // Marquee enabled: 1 = enabled, 0 = disabled
        iter->ms = sr.ms;               // Marquee speed: Range: 1 to 10 (look for command ^MSP to see of how to set this)
        iter->vf = sr.vf;               // G5: Video fill; Marks the button reserved for video (100 = video stream, 101 = MXA-MPL)
        iter->dv = sr.dv;               // G5: Streaming source; Only valid if "vf" is 100!
    }
}

bool TObjectHandler::drawObject(TResizableWidget *widget, int instance)
{
    DECL_TRACER("TObjectHandler::drawObject(TResizableWidget *widget, int instance)");

    TDrawObject drawObject(this, widget);
    drawObject.draw(instance);
    return drawObject.haveError();
    // TODO: Add code to save images so they must not be drawn new every time.
}

// Getter / Setter
void TObjectHandler::setDrawOrder(const QString& _do, int instance)
{
    DECL_TRACER("TObjectHandler::setDrawOrder(const QString& _do, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i]._do = _do;
    }
    else
        mObject.sr[instance]._do = _do;
}

QString TObjectHandler::getDrawOrder(int instance)
{
    DECL_TRACER("TObjectHandler::getDrawOrder(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QString _do = mObject.sr[0]._do;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (_do != mObject.sr[i]._do)
                return QString();
        }

        return _do;
    }
    else
        return mObject.sr[instance]._do;
}

void TObjectHandler::setBorder(const QString& bs, int instance)
{
    DECL_TRACER("TObjectHandler::setBorder(const QString& bs, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].bs = bs;
    }
    else
        mObject.sr[instance].bs = bs;
}

QString TObjectHandler::getBorder(int instance)
{
    DECL_TRACER("TObjectHandler::getBorder(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QString bs = mObject.sr[0].bs;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (bs != mObject.sr[i].bs)
                return QString();
        }

        return bs;
    }
    else
        return mObject.sr[instance].bs;
}

void TObjectHandler::setChameleonImage(const QString& mi, int instance)
{
    DECL_TRACER("TObjectHandler::setChameleonImage(const QString& mi, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].mi = mi;
    }
    else
        mObject.sr[instance].mi = mi;
}

QString TObjectHandler::getChameleonImage(int instance)
{
    DECL_TRACER("TObjectHandler::getChameleonImage(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QString mi = mObject.sr[0].mi;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (mi != mObject.sr[i].mi)
                return QString();
        }

        return mi;
    }
    else
        return mObject.sr[instance].mi;
}

void TObjectHandler::setBorderColor(const QColor& cb, int instance)
{
    DECL_TRACER("TObjectHandler::setBorderColor(const QColor& cb, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].cb = cb;
    }
    else
        mObject.sr[instance].cb = cb;
}

QColor TObjectHandler::getBorderColor(int instance)
{
    DECL_TRACER("TObjectHandler::getBorderColor(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QColor cb = mObject.sr[0].cb;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (cb != mObject.sr[i].cb)
                return QColor();
        }

        return cb;
    }
    else
        return mObject.sr[instance].cb;
}

void TObjectHandler::setGradientFillType(const QString& ft, int instance)
{
    DECL_TRACER("TObjectHandler::setGradientFillType(const QString& ft, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].ft = ft;
    }
    else
        mObject.sr[instance].ft = ft;
}

QString TObjectHandler::getGradientFillType(int instance)
{
    DECL_TRACER("TObjectHandler::getGradientFillType(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QString ft = mObject.sr[0].ft;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (ft != mObject.sr[i].ft)
                return QString();
        }

        return ft;
    }
    else
        return mObject.sr[instance].ft;
}

void TObjectHandler::setFillColor(const QColor& cf, int instance)
{
    DECL_TRACER("TObjectHandler::setFillColor(const QColor& cf, int instance)");

    MSG_DEBUG("Setting color " << cf.name(QColor::HexArgb).toStdString() << " on instance " << instance);

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].cf = cf;
    }
    else
        mObject.sr[instance].cf = cf;
}

QColor TObjectHandler::getFillColor(int instance)
{
    DECL_TRACER("TObjectHandler::getFillColor(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QColor cf = mObject.sr[0].cf;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (cf != mObject.sr[i].cf)
                return QColor();
        }

        return cf;
    }
    else
        return mObject.sr[instance].cf;
}

void TObjectHandler::setTextColor(const QColor& ct, int instance)
{
    DECL_TRACER("TObjectHandler::setTextColor(const QColor& ct, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].ct = ct;
    }
    else
        mObject.sr[instance].ct = ct;
}

QColor TObjectHandler::getTextColor(int instance)
{
    DECL_TRACER("TObjectHandler::getTextColor(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QColor ct = mObject.sr[0].ct;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (ct != mObject.sr[i].ct)
                return QColor();
        }

        return ct;
    }
    else
        return mObject.sr[instance].ct;
}

void TObjectHandler::setTextEffectColor(const QColor& ec, int instance)
{
    DECL_TRACER("TObjectHandler::setTextEffectColor(const QColor& ec, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].ec = ec;
    }
    else
        mObject.sr[instance].ec = ec;
}

QColor TObjectHandler::getTextEffectColor(int instance)
{
    DECL_TRACER("TObjectHandler::getTextEffectColor(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QColor ec = mObject.sr[0].ec;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (ec != mObject.sr[i].ec)
                return QColor();
        }

        return ec;
    }
    else
        return mObject.sr[instance].ec;
}

void TObjectHandler::setBitmaps(const QList<BITMAPS_t>& bitmaps, int instance)
{
    DECL_TRACER("TObjectHandler::setBitmaps(const QList<BITMAPS_t>& bitmaps, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
        {
            mObject.sr[i].bitmaps.clear();
            mObject.sr[i].bitmaps = bitmaps;
        }
    }
    else
        mObject.sr[instance].bitmaps = bitmaps;
}

QList<BITMAPS_t> TObjectHandler::getBitmaps(int instance)
{
    DECL_TRACER("TObjectHandler::getBitmaps(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QList<BITMAPS_t> bm = mObject.sr[0].bitmaps;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (!compareBitmaps(bm, mObject.sr[i].bitmaps))
                return QList<BITMAPS_t>();
        }

        return bm;
    }
    else
        return mObject.sr[instance].bitmaps;
}

void TObjectHandler::setGradientColors(const QList<QColor>& colors, int instance)
{
    DECL_TRACER("TObjectHandler::setGradientColors(const QList<QColor>& colors, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
        {
            mObject.sr[i].gradientColors.clear();
            mObject.sr[i].gradientColors = colors;
        }
    }
    else
        mObject.sr[instance].gradientColors = colors;
}

QList<QColor> TObjectHandler::getGradientColors(int instance)
{
    DECL_TRACER("TObjectHandler::getGradientColors(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QList<QColor> gc = mObject.sr[0].gradientColors;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (gc != mObject.sr[i].gradientColors)
                return QList<QColor>();
        }

        return gc;
    }
    else
        return mObject.sr[instance].gradientColors;
}

void TObjectHandler::setGradientRadius(int gr, int instance)
{
    DECL_TRACER("TObjectHandler::setGradientRadius(int gr, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].gr = gr;
    }
    else
        mObject.sr[instance].gr = gr;
}

int TObjectHandler::getGradientRadius(int instance)
{
    DECL_TRACER("TObjectHandler::getGradientRadius(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        int gr = mObject.sr[0].gr;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (gr != mObject.sr[i].gr)
                return -1;
        }

        return gr;
    }
    else
        return mObject.sr[instance].gr;
}

void TObjectHandler::setGradientCenterX(int gx, int instance)
{
    DECL_TRACER("TObjectHandler::setGradientCenterX(int gx, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].gx = gx;
    }
    else
        mObject.sr[instance].gx = gx;
}

int TObjectHandler::getGradientCenterX(int instance)
{
    DECL_TRACER("TObjectHandler::getGradientCenterX(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        int gx = mObject.sr[0].gx;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (gx != mObject.sr[i].gx)
                return -1;
        }

        return gx;
    }
    else
        return mObject.sr[instance].gx;
}

void TObjectHandler::setGradientCenterY(int gy, int instance)
{
    DECL_TRACER("TObjectHandler::setGradientCenterY(int gy, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].gy = gy;
    }
    else
        mObject.sr[instance].gy = gy;
}

int TObjectHandler::getGradientCenterY(int instance)
{
    DECL_TRACER("TObjectHandler::getGradientCenterY(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        int gy = mObject.sr[0].gy;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (gy != mObject.sr[i].gy)
                return -1;
        }

        return gy;
    }
    else
        return mObject.sr[instance].gy;
}

void TObjectHandler::setSound(const QString& sd, int instance)
{
    DECL_TRACER("TObjectHandler::setSound(const QString& sd, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].sd = sd;
    }
    else
        mObject.sr[instance].sd = sd;
}

QString TObjectHandler::getSound(int instance)
{
    DECL_TRACER("TObjectHandler::getSound(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QString sd = mObject.sr[0].sd;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (sd != mObject.sr[i].sd)
                return QString();
        }

        return sd;
    }
    else
        return mObject.sr[instance].sd;
}

void TObjectHandler::setDynamic(bool dynamic, int instance)
{
    DECL_TRACER("TObjectHandler::setDynamic(bool dynamic, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].dynamic = dynamic;
    }
    else
        mObject.sr[instance].dynamic = dynamic;
}

bool TObjectHandler::getDynamic(int instance)
{
    DECL_TRACER("TObjectHandler::getDynamic(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        bool dynamic = mObject.sr[0].dynamic;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (dynamic != mObject.sr[i].dynamic)
                return false;
        }

        return dynamic;
    }
    else
        return mObject.sr[instance].dynamic;
}

void TObjectHandler::setExtGraphicIndex(int sb, int instance)
{
    DECL_TRACER("TObjectHandler::setExtGraphicIndex(int sb, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].sb = sb;
    }
    else
        mObject.sr[instance].sb = sb;
}

int TObjectHandler::getExtGraphicIndex(int instance)
{
    DECL_TRACER("TObjectHandler::getExtGraphicIndex(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        int sb = mObject.sr[0].sb;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (sb != mObject.sr[i].sb)
                return -1;
        }

        return sb;
    }
    else
        return mObject.sr[instance].sb;
}

void TObjectHandler::setText(const QString& te, int instance)
{
    DECL_TRACER("TObjectHandler::setText(const QString& te, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].te = te;
    }
    else
        mObject.sr[instance].te = te;
}

QString TObjectHandler::getText(int instance)
{
    DECL_TRACER("TObjectHandler::getText(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QString te = mObject.sr[0].te;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (te != mObject.sr[i].te)
                return QString();
        }

        return te;
    }
    else
        return mObject.sr[instance].te;
}

void TObjectHandler::setTextOrientation(ORIENTATION jt, int instance)
{
    DECL_TRACER("TObjectHandler::setTextOrientation(ORIENTATION jt, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].jt = jt;
    }
    else
        mObject.sr[instance].jt = jt;
}

ORIENTATION TObjectHandler::getTextOrientation(int instance)
{
    DECL_TRACER("TObjectHandler::getTextOrientation(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        ORIENTATION jt = mObject.sr[0].jt;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (jt != mObject.sr[i].jt)
                return ObjHandler::ORI_CENTER_MIDDLE;
        }

        return jt;
    }
    else
        return mObject.sr[instance].jt;
}

void TObjectHandler::setTextAbsoluteX(int tx, int instance)
{
    DECL_TRACER("TObjectHandler::setTextAbsoluteX(int tx, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].tx = tx;
    }
    else
        mObject.sr[instance].tx = tx;
}

int TObjectHandler::getTextAbsoluteX(int instance)
{
    DECL_TRACER("TObjectHandler::getTextAbsoluteX(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        int tx = mObject.sr[0].tx;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (tx != mObject.sr[i].tx)
                return -1;
        }

        return tx;
    }
    else
        return mObject.sr[instance].tx;
}

void TObjectHandler::setTextAbsoluteY(int ty, int instance)
{
    DECL_TRACER("TObjectHandler::setTextAbsoluteY(int ty, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].ty = ty;
    }
    else
        mObject.sr[instance].ty = ty;
}

int TObjectHandler::getTextAbsoluteY(int instance)
{
    DECL_TRACER("TObjectHandler::getTextAbsoluteY(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        int ty = mObject.sr[0].ty;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (ty != mObject.sr[i].ty)
                return -1;
        }

        return ty;
    }
    else
        return mObject.sr[instance].ty;
}

void TObjectHandler::setFontFile(const QString& ff, int instance)
{
    DECL_TRACER("TObjectHandler::setFontFile(const QString& ff, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].ff = ff;
    }
    else
        mObject.sr[instance].ff = ff;
}

QString TObjectHandler::getFontFile(int instance)
{
    DECL_TRACER("TObjectHandler::getFontFile(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QString ff = mObject.sr[0].ff;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (ff != mObject.sr[i].ff)
                return QString();
        }

        return ff;
    }
    else
        return mObject.sr[instance].ff;
}

void TObjectHandler::setFontSize(int fs, int instance)
{
    DECL_TRACER("TObjectHandler::setFontSize(int fs, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].fs = fs;
    }
    else
        mObject.sr[instance].fs = fs;
}

int TObjectHandler::getFontSize(int instance)
{
    DECL_TRACER("TObjectHandler::getFontSize(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        int fs = mObject.sr[0].fs;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (fs != mObject.sr[i].fs)
                return -1;
        }

        return fs;
    }
    else
        return mObject.sr[instance].fs;
}

void TObjectHandler::setWordWrap(int ww, int instance)
{
    DECL_TRACER("TObjectHandler::setWordWrap(int ww, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].ww = ww;
    }
    else
        mObject.sr[instance].ww = ww;
}

int TObjectHandler::getWordWrap(int instance)
{
    DECL_TRACER("TObjectHandler::getWordWrap(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        int ww = mObject.sr[0].ww;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (ww != mObject.sr[i].ww)
                return -1;
        }

        return ww;
    }
    else
        return mObject.sr[instance].ww;
}

void TObjectHandler::setTextEffect(int et, int instance)
{
    DECL_TRACER("TObjectHandler::setTextEffect(int et, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].et = et;
    }
    else
        mObject.sr[instance].et = et;
}

int TObjectHandler::getTextEffect(int instance)
{
    DECL_TRACER("TObjectHandler::getTextEffect(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        int et = mObject.sr[0].et;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (et != mObject.sr[i].et)
                return -1;
        }

        return et;
    }
    else
        return mObject.sr[instance].et;
}

void TObjectHandler::setOverallOpacity(int oo, int instance)
{
    DECL_TRACER("TObjectHandler::setOverallOpacity(int oo, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].oo = oo;
    }
    else
        mObject.sr[instance].oo = oo;
}

int TObjectHandler::getOverallOpacity(int instance)
{
    DECL_TRACER("TObjectHandler::getOverallOpacity(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        int oo = mObject.sr[0].oo;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (oo != mObject.sr[i].oo)
                return -1;
        }

        return oo;
    }
    else
        return mObject.sr[instance].oo;
}

void TObjectHandler::setMarqueeType(int md, int instance)
{
    DECL_TRACER("TObjectHandler::setMarqueeType(int md, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].md = md;
    }
    else
        mObject.sr[instance].md = md;
}

int TObjectHandler::getMarqueeType(int instance)
{
    DECL_TRACER("TObjectHandler::getMarqueeType(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        int md = mObject.sr[0].md;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (md != mObject.sr[i].md)
                return -1;
        }

        return md;
    }
    else
        return mObject.sr[instance].md;
}

void TObjectHandler::setMarqueeEnabled(int mr, int instance)
{
    DECL_TRACER("TObjectHandler::setMarqueeEnabled(int mr, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].mr = mr;
    }
    else
        mObject.sr[instance].mr = mr;
}

int TObjectHandler::getMarqueeEnabled(int instance)
{
    DECL_TRACER("TObjectHandler::getMarqueeEnabled(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        int mr = mObject.sr[0].mr;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (mr != mObject.sr[i].mr)
                return -1;
        }

        return mr;
    }
    else
        return mObject.sr[instance].mr;
}

void TObjectHandler::setMarqueeSpeed(int ms, int instance)
{
    DECL_TRACER("TObjectHandler::setMarqueeSpeed(int ms, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].ms = ms;
    }
    else
        mObject.sr[instance].ms = ms;
}

int TObjectHandler::getMarqueeSpeed(int instance)
{
    DECL_TRACER("TObjectHandler::getMarqueeSpeed(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        int ms = mObject.sr[0].ms;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (ms != mObject.sr[i].ms)
                return -1;
        }

        return ms;
    }
    else
        return mObject.sr[instance].ms;
}

void TObjectHandler::setVideoFill(const QString& vf, int instance)
{
    DECL_TRACER("TObjectHandler::setVideoFill(const QString& vf, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].vf = vf;
    }
    else
        mObject.sr[instance].vf = vf;
}

QString TObjectHandler::getVideoFill(int instance)
{
    DECL_TRACER("TObjectHandler::getVideoFill(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QString vf = mObject.sr[0].vf;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (vf != mObject.sr[i].vf)
                return QString();
        }

        return vf;
    }
    else
        return mObject.sr[instance].vf;
}

void TObjectHandler::setStreamingSource(const QString& dv, int instance)
{
    DECL_TRACER("TObjectHandler::setStreamingSource(const QString& dv, int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        for (int i = 0; i < mObject.sr.size(); ++i)
            mObject.sr[i].dv = dv;
    }
    else
        mObject.sr[instance].dv = dv;
}

QString TObjectHandler::getStreamingSource(int instance)
{
    DECL_TRACER("TObjectHandler::getStreamingSource(int instance)");

    if (instance < 0 || instance >= mObject.sr.size())
    {
        QString dv = mObject.sr[0].dv;

        for (int i = 1; i < mObject.sr.size(); ++i)
        {
            if (dv != mObject.sr[i].dv)
                return QString();
        }

        return dv;
    }
    else
        return mObject.sr[instance].dv;
}

int TObjectHandler::getButtonTypeIndex(BUTTONTYPE bt)
{
    DECL_TRACER("TObjectHandler::getButtonTypeIndex(BUTTONTYPE bt)");

    // This is the index for G5 type panels!
    switch(bt)
    {
        case GENERAL:               return 0;
        case MULTISTATE_GENERAL:    return 1;
        case BARGRAPH:              return 2;
        case MULTISTATE_BARGRAPH:   return 3;
        case TEXT_INPUT:            return 4;
        case SUBPAGE_VIEW:          return 5;
        case LISTVIEW:              return 6;

        default:
            return 0;
    }

    return 0;
}

bool TObjectHandler::compareBitmaps(const QList<BITMAPS_t>& bm1, const QList<BITMAPS_t>& bm2)
{
    DECL_TRACER("TObjectHandler::compareBitmaps(const QList<BITMAPS_t>& bm1, const QList<BITMAPS_t>& bm2)");

    if (bm1.size() != bm2.size())
        return false;

    for (int i = 0; i < bm1.size(); ++i)
    {
        if (bm1[i].fileName != bm2[i].fileName)
            return false;
        else if (bm1[i].dynamic != bm2[i].dynamic)
            return false;
        else if (bm1[i].justification != bm2[i].justification)
            return false;
        else if (bm1[i].offsetX != bm2[i].offsetX)
            return false;
        else if (bm1[i].offsetY != bm2[i].offsetY)
            return false;
    }

    return true;
}
