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
        QWidget* contentWidget() const { return m_content; }

        void setMinimumSize(const QSize& s) { m_minSize = s; QWidget::setMinimumSize(s); }
        void setMinimumSize(int w, int h) { setMinimumSize(QSize(w, h)); }

        // Selection
        void setSelected(bool on);
        bool isSelected() const { return m_selected; }

        // Grid snapping configuration (kept for compatibility; snapping uses Canvas settings)
        void setGridSize(const QSize& s) { m_grid = QSize(qMax(1, s.width()), qMax(1, s.height())); }
        QSize gridSize() const { return m_grid; }
        void setSnapToGrid(bool on) { m_snapToGrid = on; }
        bool snapToGrid() const { return m_snapToGrid; }

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

        QWidget* m_content = nullptr;
        QSize m_minSize = QSize(60, 40);
        int m_frameMargin = 2;      // space for dotted frame
        int m_gripSize = 10;        // size of grip widgets
        std::array<Grip*, HandleCount> m_grips{};

        bool m_moving = false;
        QPoint m_moveStartGlobal;
        QRect m_moveStartGeom;

        // Grid (unused by group snapping directly)
        QSize m_grid = QSize(20, 20);
        bool m_snapToGrid = true;

        bool m_selected = false;
};

#endif
