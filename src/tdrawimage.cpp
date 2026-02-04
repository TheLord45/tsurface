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
#include <QPainter>

#include "tdrawimage.h"
#include "tconfmain.h"
#include "terror.h"

using namespace ObjHandler;

TDrawImage::TDrawImage(const QString& file, QWidget *widget)
    : mFileName(file),
      mWidget(widget)
{
    DECL_TRACER("TDrawImage::TDrawImage(const QString& file, QWidget *widget)");
}

void TDrawImage::setPosition(ORIENTATION ori, int x, int y)
{
    DECL_TRACER("TDrawImage::setPosition(ORIENTATION ori, int x, int y)");

    mOrientation = ori;
    mX = 0;
    mY = 0;

    if (ori == ORI_ABSOLUT)
    {
        mX = x;
        mY = y;
    }
}

void TDrawImage::setOpacity(int oo)
{
    DECL_TRACER("TDrawImage::setOpacity(int oo)");

    if (oo < -1 || oo > 255)
        return;

    mOpacity = oo;
}

void TDrawImage::draw()
{
    DECL_TRACER("TDrawImage::draw()");

    if (mFileName.isEmpty())
    {
        MSG_ERROR("Got no file name!");
        return;
    }

    if (!mWidget)
    {
        MSG_ERROR("Got no widget to draw to.");
        return;
    }

    // Load the image
    QString pathTemp = TConfMain::Current().getPathTemporary();

    if (!mPixmap.load(pathTemp + "/images/" + mFileName))
    {
        MSG_ERROR("Couldn't load image " << mFileName.toStdString());
        return;
    }

    // Scale image to fit into widget
    if (mPixmap.width() > mWidget->width() || mPixmap.height() > mWidget->height())
        mPixmap = mPixmap.scaled(mWidget->size(), Qt::KeepAspectRatio);

    // Find the wanted position of the pixmap
    int x, y;

    switch(mOrientation)
    {
        case ORI_ABSOLUT:
            x = mX;
            y = mY;
        break;

        case ORI_TOP_LEFT:
            x = 0;
            y = 0;
        break;

        case ORI_TOP_MIDDLE:
            x = mWidget->width() - mPixmap.width() / 2;
            y = 0;
        break;

        case ORI_TOP_RIGHT:
            x = mWidget->width() - mPixmap.width();
            y = 0;
        break;

        case ORI_CENTER_LEFT:
            x = 0;
            y = mWidget->height() - mPixmap.height() / 2;
        break;

        case ORI_CENTER_MIDDLE:
            x = mWidget->width() - mPixmap.width() / 2;
            y = mWidget->height() - mPixmap.height() / 2;
        break;

        case ORI_CENTER_RIGHT:
            x = mWidget->width() - mPixmap.width();
            y = mWidget->height() - mPixmap.height() / 2;
        break;

        case ORI_BOTTOM_LEFT:
            x = 0;
            y = mWidget->height() - mPixmap.height();
        break;

        case ORI_BOTTOM_MIDDLE:
            x = mWidget->width() - mPixmap.width() / 2;
            y = mWidget->height() - mPixmap.height();
        break;

        case ORI_BOTTOM_RIGHT:
            x = mWidget->width() - mPixmap.width();
            y = mWidget->height() - mPixmap.height();
        break;

        default:
            x = mWidget->width() - mPixmap.width() / 2;
            y = mWidget->height() - mPixmap.height() / 2;
    }

    QPixmap image(mPixmap.size());      // Image with given size and format.
    image.fill(Qt::transparent);        // fills with transparent
    QPainter p(&image);

    if (mOpacity >= 0)
        p.setOpacity(1.0 / 255.0 * static_cast<qreal>(mOpacity));         // set opacity from 0.0 to 1.0, where 0.0 is fully transparent and 1.0 is fully opaque.

    p.drawPixmap(x, y, mPixmap);        // given pixmap into the paint device.
    p.end();
    QPalette palette(mWidget->palette());
    palette.setBrush(QPalette::Window, QBrush(image));
    mWidget->setPalette(palette);
}
