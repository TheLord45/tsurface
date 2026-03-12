#ifndef TDRAWBORDER_H
#define TDRAWBORDER_H

#include "tobjecthandler.h"

class QPixmap;

class TDrawBorder
{
    public:
        TDrawBorder(QPixmap *bm);

        bool draw(const ObjHandler::TOBJECT_t& object, int instance);
        int getBorderWidth() { return mBorderWidth; }

    protected:
        bool getBorderFragment(const QString& path, const QString& pathAlpha, QPixmap* image, QColor color);
        bool retrieveImage(const QString& path, QPixmap *image);
        bool stretchImageWidth(QPixmap *bm, int width);
        bool stretchImageHeight(QPixmap *bm, int height);
        bool stretchImageWH(QPixmap *bm, int width, int height);
        void erasePart(QPixmap *bm, const QPixmap& mask, const QRect& clip);

    private:
        bool setPixel(QImage *img, QColor col, int x, int y);

        QPixmap *mPixmap{nullptr};
        int mBorderWidth{0};
};

#endif // TDRAWBORDER_H
