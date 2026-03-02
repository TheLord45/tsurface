#include <QPainter>

#include "tconfmain.h"
#include "tgraphics.h"
#include "tdrawborder.h"
#include "terror.h"

using namespace Graphics;

TDrawBorder::TDrawBorder(QPixmap *bm)
    : mPixmap(bm)
{
    DECL_TRACER("TDrawBorder::TDrawBorder(QPixmap *bm, Graphics::FAMILY_t bFamily)");
}

bool TDrawBorder::draw(const ObjHandler::TOBJECT_t& object, int instance)
{
    DECL_TRACER("TDrawBorder::draw(const ObjHandler::TOBJECT_t& object, int instance)");

    if (!mPixmap || mPixmap->isNull())
    {
        MSG_ERROR("No valid pixmap defined!");
        return false;
    }

    if (object.bi <= 0 || instance < 0 || instance >= object.sr.size())
    {
        MSG_ERROR("Invalid object!");
        return false;
    }

    Graphics::FAMILY_t family = TGraphics::Current().getBorder(object.sr[instance].bs);

    if (family.name.isEmpty() || family.member.empty())
    {
        MSG_ERROR("Unknown border name " << object.sr[instance].bs.toStdString());
        return false;
    }

    BORDER_t bd;
    int numBorders = 0;
    LINE_TYPE_t lnType = LT_OFF;
    QString bname = object.sr[instance].bs;

    if (object.sr.size() == 2)
    {
        QString n = object.sr[instance].bs.toLower();

        if ((n.contains("inset") || n.contains("active on")) && lnType == LT_OFF)
            lnType = LT_ON;

        if (TGraphics::Current().getBorder(bname, lnType, &bd))
            numBorders++;
    }
    else if (lnType == LT_OFF && TGraphics::Current().getBorder(bname, LT_ON, &bd))
        numBorders++;
    else if (TGraphics::Current().getBorder(bname, lnType, &bd))
        numBorders++;

    if (numBorders > 0)
    {
        QColor color = object.sr[instance].cb;      // border color
        // Load images
        QPixmap imgB, imgBR, imgR, imgTR, imgT, imgTL, imgL, imgBL;

        if (!getBorderFragment(bd.b, bd.b_alpha, &imgB, color) || imgB.isNull())
            return false;

        if (!getBorderFragment(bd.br, bd.br_alpha, &imgBR, color) || imgBR.isNull())
            return false;

        if (!getBorderFragment(bd.r, bd.r_alpha, &imgR, color) || imgR.isNull())
            return false;

        if (!getBorderFragment(bd.tr, bd.tr_alpha, &imgTR, color) || imgTR.isNull())
            return false;

        if (!getBorderFragment(bd.t, bd.t_alpha, &imgT, color) || imgT.isNull())
            return false;

        if (!getBorderFragment(bd.tl, bd.tl_alpha, &imgTL, color) || imgTL.isNull())
            return false;

        if (!getBorderFragment(bd.l, bd.l_alpha, &imgL, color) || imgL.isNull())
            return false;

        mBorderWidth = imgL.width();

        if (!getBorderFragment(bd.bl, bd.bl_alpha, &imgBL, color) || imgBL.isNull())
            return false;

        MSG_DEBUG("Got images \"" << bd.bl.toStdString() << "\" and \"" << bd.bl_alpha.toStdString() << "\" with size " << imgBL.width() << " x " << imgBL.height());
        MSG_DEBUG("Button image size: " << (imgTL.width() + imgT.width() + imgTR.width()) << " x " << (imgTL.height() + imgL.height() + imgBL.height()));
        MSG_DEBUG("Total size: " << object.wt << " x " << object.ht);
        stretchImageWidth(&imgB, object.wt - imgBL.width() - imgBR.width());
        stretchImageWidth(&imgT, object.wt - imgTL.width() - imgTR.width());
        stretchImageHeight(&imgL, object.ht - imgTL.height() - imgBL.height());
        stretchImageHeight(&imgR, object.ht - imgTR.height() - imgBR.height());
        MSG_DEBUG("Stretched button image size: " << (imgTL.width() + imgT.width() + imgTR.width()) << " x " << (imgTL.height() + imgL.height() + imgBL.height()));
        // Draw the frame
        QPixmap frame(mPixmap->width(), mPixmap->height());
        frame.fill(Qt::transparent);

        QPainter target(mPixmap);
        QPainter canvas(&frame);

        canvas.drawPixmap(imgBL.width(), object.ht - imgB.height(), imgB);
        canvas.drawPixmap(imgTL.width(), 0, imgT);
        canvas.drawPixmap(object.wt - imgBR.width(), object.ht - imgBR.height(), imgBR);
        canvas.drawPixmap(object.wt - imgTR.width(), 0, imgTR);
        canvas.drawPixmap(0, 0, imgTL);
        canvas.drawPixmap(0, object.ht - imgBL.height(), imgBL);
        canvas.drawPixmap(0, imgTL.height(), imgL);
        canvas.drawPixmap(object.wt - imgR.width(), imgTR.height(), imgR);

        // Define inner clip region
        QRect clip(imgL.width(), imgT.width(), object.wt - imgR.width(), object.ht - imgB.width());
        // Erase everything outside of clip region
        erasePart(mPixmap, frame, clip);
        // Make final pixmap
        target.drawPixmap(0, 0, frame);
        target.end();
    }

    return true;
}

/**
 * @brief getBorderFragment - get part of border
 * The method reads a border image fragment from the disk and converts it to
 * the border color. If there is a base image and an alpha mask image, the
 * pixels of the alpha mask are converted to the border color and then the base
 * image is layed over the mask image.
 * In case there is no base image, an image with the same size as the mask image
 * is created and filled transparaent.
 *
 * @param path      The path and file name of the base image.
 * @param pathAlpha The path and file name of the alpha mask image.
 * @param image     A pointer to an empty bitmap.
 * @param color     The border color
 *
 * @return In case the images exists and were loaded successfully, TRUE is
 * returned.
 */
bool TDrawBorder::getBorderFragment(const QString& path, const QString& pathAlpha, QPixmap* image, QColor color)
{
    DECL_TRACER("TButton::getBorderFragment(const QString& path, const QString& pathAlpha, QPixmap* image, QColor color)");

    if (!image)
    {
        MSG_ERROR("Invalid pointer to image!");
        return false;
    }

    QImage img = image->toImage();
    QPixmap bm;
    bool haveBaseImage = false;
    QColor swCol = color;

    // If the path ends with "alpha.png" then it is a mask image. This is not what
    // we want first unless this is the only image available.
    if (!path.endsWith("alpha.png") || pathAlpha.isEmpty())
    {
        if (!path.isEmpty() && retrieveImage(path, image))
        {
            haveBaseImage = true;
            // Underly the pixels with the border color
            MSG_DEBUG("Path: " << path.toStdString() << ", pathAlpha: " << pathAlpha.toStdString());

            if (pathAlpha.isEmpty() || path == pathAlpha)
            {
                QImage b(image->width(), image->height(), QImage::Format_ARGB32);
                b.fill(Qt::transparent);

                for (int x = 0; x < image->width(); ++x)
                {
                    for (int y = 0; y < image->height(); ++y)
                    {
                        int alpha = img.pixelColor(x, y).alpha();
                        QColor pix = b.pixelColor(x, y);

                        if (alpha > 0)
                        {
                            pix = swCol.rgb();
                            pix.setAlpha(alpha);
                        }

                        img.setPixelColor(x, y, pix);
                    }
                }

                QPainter painter(image);
                painter.drawImage(QPoint(0, 0), b);
                painter.end();
            }
        }
    }

    // If there is no valid path return.
    if (pathAlpha.isEmpty())
        return haveBaseImage;

    // On error retrieving the image, return.
    if (!retrieveImage(pathAlpha, &bm))
        return haveBaseImage;

    // If there was no base image loaded, allocate the space for an image
    // filled transparent. Make it the same size as the mask image.
    if (!haveBaseImage)
    {
        *image = QPixmap(bm.width(), bm.height());
        image->fill(Qt::transparent);
    }

    // Only if the base image and the mask image have the same size, which
    // should be the case, then the visible pixels of the mask image are
    // colored by the border color.
    if (image->size() == bm.size())
    {
        img = bm.toImage();

        for (int y = 0; y < image->height(); ++y)
        {
            for (int x = 0; x < image->width(); ++x)
            {
                QColor col = img.pixelColor(x, y);
                uint alpha = col.alpha();
                QColor pix = img.pixel(x, y);

                if (alpha == 0)
                    pix = Qt::transparent;
                else
                {
                    swCol.setAlpha(alpha);
                    pix = swCol;
                }

                img.setPixelColor(x, y, pix);
            }
        }
    }

    // Here we draw the border fragment over the base image.
    QPainter painter(image);
    painter.drawImage(0, 0, img);
    painter.end();
    return true;
}

bool TDrawBorder::retrieveImage(const QString& path, QPixmap *image)
{
    DECL_TRACER("TDrawBorder::retrieveImage(const QString& path, QPixmap *image)");

    if (path.isEmpty() || image)
        return false;

    QString tempPath = TConfMain::Current().getPathTemporary();
    QString file = tempPath + "/__system/graphics/" + path;

    return image->load(file);
}

bool TDrawBorder::stretchImageWidth(QPixmap *bm, int width)
{
    DECL_TRACER("TDrawBorder::stretchImageWidth(QPixmap *bm, int width)");

    if (!bm || bm->isNull() || width <= 0)
        return false;

    QPixmap stretched = bm->scaledToWidth(width);
    *bm = stretched;
    return true;
}

bool TDrawBorder::stretchImageHeight(QPixmap *bm, int height)
{
    DECL_TRACER("TDrawBorder::stretchImageHeight(QPixmap *bm, int height)");

    if (!bm || bm->isNull() || height <= 0)
        return false;

    QPixmap stretched = bm->scaledToHeight(height);
    *bm = stretched;
    return true;
}

bool TDrawBorder::stretchImageWH(QPixmap *bm, int width, int height)
{
    DECL_TRACER("TDrawBorder::stretchImageWH(QPixmap *bm, int width, int height)");

    if (!bm || bm->isNull() || width <= 0)
        return false;

    QPixmap stretched = bm->scaled(width, height);
    *bm = stretched;
    return true;
}

void TDrawBorder::erasePart(QPixmap *bm, QPixmap& mask, QRect& clip)
{
    DECL_TRACER("TDrawBorder::erasePart(QPixmap *bm, QPixmap& mask, QRect& clip)");

    if (!bm || bm->isNull())
        return;

    if (bm->size() != mask.size())
    {
        MSG_WARNING("Sizes of pixmap and mask don't match!");
        return;
    }

    QImage img = bm->toImage();
    QImage msk = mask.toImage();

    int x, y;

    for (x = 0; x < img.width(); ++x)
    {
        for (y = 0; y < img.height(); ++y)
        {
            if (x > clip.x() && x < clip.width() &&
                    y > clip.y() && y < clip.height())
                continue;

            QColor colImg = img.pixelColor(x, y);
            QColor colMsk = msk.pixelColor(x, y);
            int alpha = colMsk.alpha();

            if (alpha > 0)
            {
                colImg.setAlpha(0);
                img.setPixelColor(x, y, colImg);
            }
        }
    }

    *bm = QPixmap::fromImage(img);
}
