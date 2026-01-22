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
#ifndef __TCANVASWIDGET_H__
#define __TCANVASWIDGET_H__

#include <QWidget>
#include <QSet>
#include <QVector>

class TResizableWidget;

class TCanvasWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit TCanvasWidget(QWidget* parent = nullptr);

        // Grid controls
        void setGridSize(const QSize& s);
        void setGridSize(int w, int h) { setGridSize(QSize(w, h)); }
        QSize gridSize() const { return mGrid; }

        void setSnapEnabled(bool on);
        bool snapEnabled() const { return mSnapEnabled; }

        void setShowGrid(bool on);
        bool showGrid() const { return mShowGrid; }

        // Selection API
        bool isSelected(TResizableWidget* w) const { return mSelection.contains(w); }
        void selectOnly(TResizableWidget* w);
        void toggleSelection(TResizableWidget* w);
        void clearSelection();
        void selectAll();
        bool allSelected() const;
        int selectedCount() const { return mSelection.size(); }
        int totalWidgetCount() const;

        // Group move (called by TResizableWidget)
        void beginGroupMove(TResizableWidget* lead, const QPoint& globalPos);
        void updateGroupMove(const QPoint& globalPos);
        void endGroupMove();

        // Deletion and cleanup
        void removeSelected();
        void removeObject(TResizableWidget *w);

    signals:
        void gridChanged(const QSize& size);
        void snapChanged(bool enabled);
        void failedClickAt(const QPoint& pt);

    protected:
        void paintEvent(QPaintEvent*) override;
        void mousePressEvent(QMouseEvent*) override;

    private:
        QSize mGrid = QSize(20, 20);
        bool mSnapEnabled = true;
        bool mShowGrid = true;

        QSet<TResizableWidget*> mSelection;

        struct MoveItem
        {
            TResizableWidget* w = nullptr;
            QRect startGeom;
        };

        bool mGroupMoving{false};
        QPoint mMoveStartGlobal;
        QVector<MoveItem> mMoveItems;
        TResizableWidget* mMoveLead{nullptr};

        QList<TResizableWidget*> mResizableChildren() const;
        static inline int mSnapCoord(int v, int step);
};

#endif
