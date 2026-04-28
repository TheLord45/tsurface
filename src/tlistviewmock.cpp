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
#include <QPixmap>
#include <QPainter>

#include "tlistviewmock.h"
#include "tobjecthandler.h"
#include "terror.h"

// Item layout --> 4 = vertical, image top; 2 = horizontal, image right; 1 = horizontal, image left
#define LAYOUT_IMAGE_LEFT       1
#define LAYOUT_IMAGE_RIGHT      2
#define LAYOUT_IMAGE_TOP        4

/**
 * @brief TListViewMock::TListViewMock - Constructor
 */
TListViewMock::TListViewMock()
{
    DECL_TRACER("TListViewMock::TListViewMock()");
}

/**
 * @brief TListViewMock::TListViewMock - Constructor
 *
 * Constructs the class.
 *
 * @param width     The total width of the object in pixels.
 * @param height    The total height of the object in pixles.
 * @param bm        A pointer to a pixmap. If this is a @b nullptr, the class
 * will allocate an internal pixmap and fills it transparent. When the class is
 * destroyed, the pixmap will also be destroyed!
 */
TListViewMock::TListViewMock(int width, int height, QPixmap *bm)
    : mPixmap(bm),
      mWidth(width),
      mHeight(height)
{
    DECL_TRACER("TListViewMock::TListViewMock(QPixmap *bm)");
}

/**
 * @brief TListViewMock::~TListViewMock - Destructor
 *
 * In case the class allocated it's own pixmap, it will be destroyed here.
 */
TListViewMock::~TListViewMock()
{
    DECL_TRACER("TListViewMock::~TListViewMock()");

    if (mInternPixmap && mPixmap)
        delete mPixmap;
}

/**
 * @brief TListViewMock::drawListview - Listview mockup
 *
 * This method draws a mockup representing a listview. The representation tries
 * to come as close as possible to the real listview visible on TPanel. The
 * mockup does not support any interaction.
 *
 * Before this can be used, any or better all of the otions should have been
 * set. Otherwise it will use defaults.
 */
void TListViewMock::drawListview()
{
    DECL_TRACER("TListViewMock::drawListview()");

    if (!mWidth || !mHeight)
    {
        MSG_ERROR("Invalid width or height!");
        return;
    }

    if (!mPixmap)
    {
        mPixmap = new QPixmap(mWidth, mHeight);
        mPixmap->fill(Qt::transparent);
        mInternPixmap = true;
    }

    if (mPixmap->isNull())
    {
        MSG_ERROR("Invalid pixmap!");
        return;
    }

    QPixmap pxSearch(":images/system-search.svg");      // Load a magnifying glass
    QPainter painter(mPixmap);                          // Initialize a painter for ouir pixmap
    painter.setClipRect(QRect(1, 1, mWidth - 2, mHeight - 2));  // Define a clipping region to avoid overwriting the frame
    QPen frame(mBorderColor);                           // Define a pen to draw lines
    frame.setStyle(Qt::SolidLine);                      // Solid lines for pen
    frame.setWidth(2);                                  // Set line width to 2 pixel
    int yPos = 0;                                       // The Y position we start of
    // If there is a filter defined, draw it first on top of list
    if (mLvs)                                           // Filter?
    {
        painter.setPen(frame);                          // Assign the pen for lines
        painter.drawRect(2, 2, mWidth - 4, mLsh - 4);   // Draw a rectangle
        painter.drawPixmap(3, 3, mLsh - 6, mLsh - 6, pxSearch); // Draw the preveously loaded pixmap. Scaling is done automatically.
        mFont.setPixelSize(mLsh - 6);                   // Set the font size in pixels.
        painter.setFont(mFont);                         // Assign the font to the painter
        painter.drawText(QRect(6 + mLsh, 3, mWidth - mLsh - 6, mLsh - 6), "Search", QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
        yPos = mLsh;                                    // Adjust the Y position
    }

    while (yPos < mHeight)                              // Draw the lines of the listview
        yPos = drawItem(yPos, &painter);                // Draw one line of the listview

    if (mLva)                                           // Alphabet scrollbar?
    {
        painter.setPen(frame);                          // Assign the pen to draw lines
        yPos = 1;                                       // Reset the Y position

        if (mLvs)                                       // Filter?
            yPos += mLsh;                               // Adjust the Y position below the filter

        painter.drawLine(mWidth - 15, yPos, mWidth - 15, mHeight);

        QPen text(mTextColor);                          // Define a pen to draw text with the color for it
        painter.setPen(text);                           // Assign the pen

        mFont.setPointSize(mFontSize);                  // Set the font size in points
        QFontMetrics fm(mFont);                         // Define a QFontMetric to meassure characters
        int aHeight = fm.boundingRect('A').height();    // Get the height of the letter 'A'
        QTextOption tOption(Qt::AlignCenter);           // Define a text option with center alignment
        tOption.setWrapMode(QTextOption::NoWrap);       // Disable wrapping
        painter.setFont(mFont);                         // Assign the font

        for (char ch = 'A'; ch <= 'Z'; ++ch)            // Loop through the alphabet
        {
            QString letter(ch);                         // Make a string out of the character
            QRect clip(mWidth - 13, yPos, 13, aHeight);
            painter.drawText(clip, letter, tOption);
            yPos += aHeight + 2;                        // Increase the Y for the next line

            if (yPos > mHeight)                         // If the next line would be out of the listview, stop here.
                break;
        }
    }

    painter.end();                                      // End painting
}

/**
 * @brief TListViewMock::drawItem - Draws a single line (item) of the listview.
 *
 * The method draws an item of the listview. An item can consist of an image and
 * up to 2 lines. The height of the item is defined by the parameter @b mLvh.
 * The size for the image and the lines(s) are defined by the parameters @b mLhp
 * and @b mLvp, where this parameters define a percentage value in the range of
 * 5 to 95. The @b mLhp defines the size for the image. If there's no image,
 * this parameter is ignored.
 *
 * The parameter @b mLvp defines the size for the first line. The rest of the
 * space, if there is one, is reserved for the second line.
 *
 * The parameter @b mLvg defines the number of columns. What was described above
 * is valid vor 1 cell of the listview. Each item is devided into @a mLvg
 * columns, where each cell may contain an image and up to 2 lines of text.
 *
 * @param start     This is the Y pixel to start with the item to draw.
 * @param painter   A pointer to a painter device.
 *
 * @return Returns the Y pixel to start for the next item. This is the value of
 * @b start plus the height of the item.
 */
int TListViewMock::drawItem(int start, QPainter *painter)
{
    DECL_TRACER("TListViewMock::drawItem(int start, QPainter *painter)");

    QFont font = mFont;
    font.setPointSize(mFontSize);

    QPen frame(mBorderColor);
    frame.setStyle(Qt::DotLine);
    frame.setWidth(1);

    QPen text(mTextColor);

    painter->setPen(frame);
    painter->setFont(font);
    MSG_DEBUG("mLvc: " << mLvc << ", mLvg: " << mLvg);
    int totalWidth = mWidth;        // The total width of the listview
    int cellWidth = mWidth;         // The width of 1 cell

    if (mLva)   // Alphabet scrollbar?
    {
        totalWidth -= 15;           // We reserve 15 pixels for the alphabet scrollbar
        cellWidth = totalWidth;
    }

    if (mLvg > 1)                   // More than 1 column?
        cellWidth /= mLvg;          // Calculate the width of 1 cell

    // Draw the lower separator line
    if ((start + mLvh) < mHeight)
        painter->drawLine(1, start + mLvh, totalWidth, start + mLvh);

    int startX = 1;

    for (int cell = 0; cell < mLvg; ++cell)
    {
        if (mLvc & LIST_TEXT_PRIMARY)
        {
            int l = startX + 1;         // Left start position
            int cellLeft = cellWidth;   // The number of pixels left for text
            int height1 = static_cast<double>(mLvh) / 100.0 * static_cast<double>(mLvp);
            int startY = start + 2;

            if (mLvc & LIST_IMAGE_CELL)         // Should we draw an image?
            {
                if (mLvl & LAYOUT_IMAGE_LEFT)   // Image left?
                {
                    l = (static_cast<double>(cellWidth) / 100.0 * static_cast<double>(mLhp)) + static_cast<double>(startX);
                    cellLeft -= static_cast<double>(cellWidth) / 100.0 * static_cast<double>(mLhp);
                }
                else if (mLvl & LAYOUT_IMAGE_RIGHT) // Image right?
                    cellLeft -= static_cast<double>(cellWidth) / 100.0 * static_cast<double>(mLhp);
                else if (mLvl & LAYOUT_IMAGE_TOP)   // Image on top?
                {
                    int image = static_cast<double>(mLvh) / 100.0 * static_cast<double>(mLhp);

                    if (mLvc & LIST_TEXT_SECONDARY)
                        height1 = static_cast<double>(mLvh - image) / 100.0 * static_cast<double>(mLvp);
                    else
                        height1 = mLvh - image;

                    cellLeft = 0;
                    startY += image;
                }
            }
            else
                cellLeft = 0;       // We use the whole width of the cell

            QTextOption tOption(Qt::AlignLeft | Qt::AlignVCenter);
            tOption.setWrapMode(QTextOption::NoWrap);
            painter->setPen(text);

            if (mLvl & LAYOUT_IMAGE_TOP)
                tOption.setAlignment(Qt::AlignCenter);

            painter->drawText(QRect(l, startY, cellWidth - cellLeft, height1), "Primary Text", tOption);

            if (mLvc & LIST_TEXT_SECONDARY)
            {
                int height2 = mLvh - height1;

                if (mLvl & LAYOUT_IMAGE_TOP)
                {
                    int image = static_cast<double>(mLvh) / 100.0 * static_cast<double>(mLhp);
                    height2 = mLvh - image - height1;
                }

                painter->drawText(QRect(l, startY + height1, cellWidth - cellLeft, height2), "Secondary Text", tOption);
            }
        }

        if (mLvc & LIST_IMAGE_CELL)
        {
            int w = static_cast<double>(cellWidth) / 100.0 * static_cast<double>(mLhp);
            int h = static_cast<double>(mLvh) / 100.0 * static_cast<double>(mLhp);

            QPixmap px(":images/user-mowgli.svg");
            QRect iArea;

            if (mLvl & LAYOUT_IMAGE_LEFT)
            {
                MSG_DEBUG("Left image");
                px = px.scaled(QSize(w, qMin(h * 2, mLvh - 2)), Qt::KeepAspectRatio);
                iArea = QRect((w - px.width()) / 2 + startX, start + 1 + ((mLvh - px.height()) / 2), px.width(), px.height());
            }
            else if (mLvl & LAYOUT_IMAGE_RIGHT)
            {
                MSG_DEBUG("Right image");
                int cellLeft = cellWidth - (static_cast<double>(cellWidth) / 100.0 * static_cast<double>(mLhp));
                px = px.scaled(QSize(w, qMin(h * 2, mLvh - 2)), Qt::KeepAspectRatio);
                iArea = QRect((w - px.width()) / 2 + startX + cellLeft, start + 1 + ((mLvh - px.height()) / 2), px.width(), px.height());
            }
            else if (mLvl & LAYOUT_IMAGE_TOP)
            {
                MSG_DEBUG("Top image");
                int ht = static_cast<double>(mLvh) / 100.0 * static_cast<double>(mLhp);
                px = px.scaled(QSize(cellWidth, ht), Qt::KeepAspectRatio);
                iArea = QRect(startX + ((cellWidth - px.width()) / 2), start + 1, px.width(), px.height());
            }

            painter->drawPixmap(iArea, px);
        }

        startX += cellWidth;

        if (cell < (mLvg - 1))
        {
            painter->setPen(frame);
            painter->drawLine(startX, start, startX, start + mLvh);
        }
    }

    return start + mLvh;
}

// Getter/Setter

/**
 * @brief TListViewMock::pixmap
 *
 * Return the pointer to the internal pixmap. If this pixmap was allocated by
 * the class, don't destroy it! It will be destroyed at the moment the class is
 * destroyed or if a new pixmap is assigned with @b setPixmap().
 *
 * @return A pointer to the internal pixmap.
 */
QPixmap *TListViewMock::pixmap() const
{
    DECL_TRACER("TListViewMock::pixmap() const");

    return mPixmap;
}

/**
 * @brief TListViewMock::setPixmap
 *
 * Assigns a pixmap to the class. If the class previously allocated a pixmap,
 * it will be destroyed and replaced by the new one.
 *
 * @param newPixmap     A pointer to a valid pixmap in the size of the object
 * it belongs to.
 */
void TListViewMock::setPixmap(QPixmap *newPixmap)
{
    DECL_TRACER("TListViewMock::setPixmap(QPixmap *newPixmap)");

    if (mInternPixmap && mPixmap)
    {
        delete mPixmap;
        mInternPixmap = false;
    }

    mPixmap = newPixmap;
}

void TListViewMock::setFillColor1(const QColor& col)
{
    DECL_TRACER("TListViewMock::setFillColor1(const QColor& col)");

    if (col == mFillColor2)
        return;

    mFillColor1 = col;
}

void TListViewMock::setFillColor2(const QColor& col)
{
    DECL_TRACER("TListViewMock::setFillColor2(const QColor& col)");

    if (col == mFillColor1)
        return;

    mFillColor2 = col;
}

/**
 * @brief TListViewMock::setFontSize
 *
 * Sets the size of the font in points.
 *
 * @param pt    The font size in points.
 */
void TListViewMock::setFontSize(int pt)
{
    DECL_TRACER("TListViewMock::setFontSize(int pt)");

    if (pt < 1)
        return;

    mFontSize = pt;
}

/**
 * @brief TListViewMock::getLvc
 *
 * @return Returns the listview components value.
 */
int TListViewMock::getLvc() const
{
    DECL_TRACER("TListViewMock::getLvc() const");

    return mLvc;
}

/**
 * @brief TListViewMock::setLvc
 *
 * Sets the listview components. This is a bit encoded value.
 *
 * @param newLvc    The listview components. The bits have the following meaning:
 *
 * Bit 0: Draw an image
 *
 * Bit 1: Draw the primary text
 *
 * Bit 2: Draw the secondary text. But only if bit 1 is set.
 */
void TListViewMock::setLvc(int newLvc)
{
    DECL_TRACER("TListViewMock::setLvc(int newLvc)");

    mLvc = newLvc;
}

int TListViewMock::getLvh() const
{
    DECL_TRACER("TListViewMock::getLvh() const");

    return mLvh;
}

void TListViewMock::setLvh(int newLvh)
{
    DECL_TRACER("TListViewMock::setLvh(int newLvh)");
    mLvh = newLvh;
}

int TListViewMock::getLvl() const
{
    DECL_TRACER("TListViewMock::getLvl() const");
    return mLvl;
}

void TListViewMock::setLvl(int newLvl)
{
    DECL_TRACER("TListViewMock::setLvl(int newLvl)");

    mLvl = newLvl;
}

int TListViewMock::getLvg() const
{
    DECL_TRACER("TListViewMock::getLvg() const");

    return mLvg;
}

void TListViewMock::setLvg(int newLvg)
{
    DECL_TRACER("TListViewMock::setLvg(int newLvg)");

    if (mLvg < 1)
        return;

    mLvg = newLvg;
}

int TListViewMock::getLhp() const
{
    DECL_TRACER("TListViewMock::getLhp() const");

    return mLhp;
}

void TListViewMock::setLhp(int newLhp)
{
    DECL_TRACER("TListViewMock::setLhp(int newLhp)");

    mLhp = newLhp;
}

int TListViewMock::getLvp() const
{
    DECL_TRACER("TListViewMock::getLvp() const");

    return mLvp;
}

void TListViewMock::setLvp(int newLvp)
{
    DECL_TRACER("TListViewMock::setLvp(int newLvp)");

    mLvp = newLvp;
}

int TListViewMock::getLvs() const
{
    DECL_TRACER("TListViewMock::getLvs() const");

    return mLvs;
}

void TListViewMock::setLvs(int newLvs)
{
    DECL_TRACER("TListViewMock::setLvs(int newLvs)");

    mLvs = newLvs;
}

int TListViewMock::getLsh() const
{
    DECL_TRACER("TListViewMock::getLsh() const");

    return mLsh;
}

void TListViewMock::setLsh(int newLsh)
{
    DECL_TRACER("TListViewMock::setLsh(int newLsh)");

    mLsh = newLsh;
}

int TListViewMock::getLva() const
{
    DECL_TRACER("TListViewMock::getLva() const");

    return mLva;
}

void TListViewMock::setLva(int newLva)
{
    mLva = newLva;
}
