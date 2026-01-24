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
#ifndef __TRESIZEABLEWIDGET_H__
#define __TRESIZEABLEWIDGET_H__

#include <QWidget>
#include <array>

class TResizableWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit TResizableWidget(QWidget* content = nullptr, QWidget* parent = nullptr);
        void setContentWidget(QWidget* w);
        QWidget* contentWidget() const { return mContent; }

        void setMinimumSize(const QSize& s) { mMinSize = s; QWidget::setMinimumSize(s); }
        void setMinimumSize(int w, int h) { setMinimumSize(QSize(w, h)); }

        // Selection
        void setSelected(bool on);
        bool isSelected() const { return mSelected; }

        // Grid snapping configuration (kept for compatibility; snapping uses Canvas settings)
        void setGridSize(const QSize& s) { mGrid = QSize(qMax(1, s.width()), qMax(1, s.height())); }
        QSize gridSize() const { return mGrid; }
        void setSnapToGrid(bool on) { mSnapToGrid = on; }
        bool snapToGrid() const { return mSnapToGrid; }

        // Identity settings
        void setId(int id) { mID = id; }
        int getId() { return mID; }
        void setPageId(int id) { mPageID = id; }
        int getPageId() { return mPageID; }

    signals:
        void selectChanged(TResizableWidget *w, bool selected);
        void objectMoved(TResizableWidget *w, QPoint pt);
        void objectSizeChanged(TResizableWidget *w, QSize size);

    protected:
        void paintEvent(QPaintEvent*) override;
        void resizeEvent(QResizeEvent*) override;

        // Drag-to-move support (delegates group move to CanvasWidget)
        void mousePressEvent(QMouseEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;
        void mouseReleaseEvent(QMouseEvent*) override;
        bool eventFilter(QObject*, QEvent*) override;

    private:
        enum Handle {
            TopLeft = 0, Top, TopRight,
            Right, BottomRight, Bottom,
            BottomLeft, Left,
            HandleCount
        };
        class Grip;

        void layoutGrips();
        void updateGripsVisibility();
        static Qt::CursorShape cursorFor(Handle h);

        // Move helpers
        void beginMove(const QPoint& globalPos);
        void updateMove(const QPoint& globalPos);
        void endMove();

        QWidget* mContent = nullptr;
        QSize mMinSize = QSize(60, 40);
        int mFrameMargin = 2;      // space for dotted frame
        int mGripSize = 10;        // size of grip widgets
        std::array<Grip*, HandleCount> mGrips{};

        bool mMoving = false;
        QPoint mMoveStartGlobal;
        QRect mMoveStartGeom;

        // Grid (unused by group snapping directly)
        QSize mGrid = QSize(20, 20);
        bool mSnapToGrid = true;

        bool mSelected = false;
        // Identifier settings
        int mID{0};             // Button ID
        int mPageID{0};         // The ID of the page this object belongs to.
};

#endif
