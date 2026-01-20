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
#include <QPen>
#include <QMouseEvent>
#include <algorithm>

#include "tcanvaswidget.h"
#include "tresizablewidget.h"
#include "terror.h"

TCanvasWidget::TCanvasWidget(QWidget* parent)
    : QWidget(parent)
{
    DECL_TRACER("TCanvasWidget::TCanvasWidget(QWidget* parent)");

    setObjectName("canvas");
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("#canvas { background: #f6f6f6; }");
    setMouseTracking(true);
    setMinimumSize(400, 300);
}

void TCanvasWidget::setGridSize(const QSize& s)
{
    DECL_TRACER("TCanvasWidget::setGridSize(const QSize& s)");

    QSize newSize(qMax(1, s.width()), qMax(1, s.height()));

    if (newSize == mGrid)
        return;

    mGrid = newSize;
    update();
    emit gridChanged(mGrid);
}

void TCanvasWidget::setSnapEnabled(bool on)
{
    DECL_TRACER("TCanvasWidget::setSnapEnabled(bool on)");

    if (mSnapEnabled == on)
        return;

    mSnapEnabled = on;
    update();
    emit snapChanged(mSnapEnabled);
}

void TCanvasWidget::setShowGrid(bool on)
{
    DECL_TRACER("TCanvasWidget::setShowGrid(bool on)");

    if (mShowGrid == on)
        return;

    mShowGrid = on;
    update();
}

void TCanvasWidget::paintEvent(QPaintEvent*)
{
    if (!mShowGrid)
        return;

    QPainter p(this);
    p.fillRect(rect(), palette().brush(QPalette::Base));

    QPen pen(QColor(180, 180, 180));
    pen.setStyle(Qt::DotLine);
    pen.setWidth(1);
    p.setPen(pen);

    const int gx = qMax(1, mGrid.width());
    const int gy = qMax(1, mGrid.height());

    for (int x = 0; x <= width(); x += gx)
        p.drawLine(x, 0, x, height());

    for (int y = 0; y <= height(); y += gy)
        p.drawLine(0, y, width(), y);
}

void TCanvasWidget::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        QWidget* w = childAt(e->pos());
        // Clicked background (or non-resizable): clear selection unless Ctrl is held
        if ((!w || !qobject_cast<TResizableWidget*>(w->parentWidget() ? w->parentWidget() : w)) &&
            !(e->modifiers() & Qt::ControlModifier))
        {
            clearSelection();
        }
    }

    QWidget::mousePressEvent(e);
}

QList<TResizableWidget*> TCanvasWidget::mResizableChildren() const
{
    DECL_TRACER("TCanvasWidget::mResizableChildren() const");

    return findChildren<TResizableWidget*>(QString(), Qt::FindDirectChildrenOnly);
}

void TCanvasWidget::selectOnly(TResizableWidget* w)
{
    DECL_TRACER("TCanvasWidget::selectOnly(TResizableWidget* w)");

    if (!w)
        return;

    if (mSelection.size() == 1 && mSelection.contains(w))
        return;

    for (auto* s : std::as_const(mSelection))
        s->setSelected(false);

    mSelection.clear();
    mSelection.insert(w);
    w->setSelected(true);
}

void TCanvasWidget::toggleSelection(TResizableWidget* w)
{
    DECL_TRACER("TCanvasWidget::toggleSelection(TResizableWidget* w)");

    if (!w)
        return;

    if (mSelection.contains(w))
    {
        mSelection.remove(w);
        w->setSelected(false);
    }
    else
    {
        mSelection.insert(w);
        w->setSelected(true);
    }
}

void TCanvasWidget::clearSelection()
{
    DECL_TRACER("TCanvasWidget::toggleSelection(TResizableWidget* w)");

    for (auto* s : std::as_const(mSelection))
        s->setSelected(false);

    mSelection.clear();
}

void TCanvasWidget::selectAll()
{
    DECL_TRACER("TCanvasWidget::selectAll()");

    const auto all = mResizableChildren();

    for (auto* w : all)
    {
        if (!mSelection.contains(w))
        {
            mSelection.insert(w);
            w->setSelected(true);
        }
    }
}

bool TCanvasWidget::allSelected() const
{
    DECL_TRACER("TCanvasWidget::allSelected() const");

    const int total = totalWidgetCount();
    return total > 0 && mSelection.size() == total;
}

int TCanvasWidget::totalWidgetCount() const
{
    DECL_TRACER("TCanvasWidget::totalWidgetCount() const");

    return mResizableChildren().size();
}

static inline int clampInt(int v, int lo, int hi)
{
    return std::min(std::max(v, lo), hi);

}

inline int TCanvasWidget::mSnapCoord(int v, int step)
{
    if (step <= 1)
        return v;

    const int half = step / 2;
    return ((v + half) / step) * step;
}

void TCanvasWidget::beginGroupMove(TResizableWidget* lead, const QPoint& globalPos)
{
    DECL_TRACER("TCanvasWidget::beginGroupMove(TResizableWidget* lead, const QPoint& globalPos)");

    if (!lead)
        return;

    mGroupMoving = true;
    mMoveStartGlobal = globalPos;
    mMoveLead = lead;

    mMoveItems.clear();
    const bool leadIsSelected = mSelection.contains(lead);
    QVector<TResizableWidget*> group;

    if (leadIsSelected)
    {
        group = mSelection.values().toVector();
    }
    else
    {
        // If lead isn't selected, move only it (and selection becomes just it)
        clearSelection();
        selectOnly(lead);
        group = { lead };
    }

    mMoveItems.reserve(group.size());

    for (TResizableWidget *w : group)
    {
        MoveItem it;
        it.w = w;
        it.startGeom = w->geometry();
        mMoveItems.push_back(it);
    }
}

void TCanvasWidget::updateGroupMove(const QPoint& globalPos)
{
    DECL_TRACER("TCanvasWidget::updateGroupMove(const QPoint& globalPos)");

    if (!mGroupMoving || mMoveItems.isEmpty())
        return;

    const QPoint delta = globalPos - mMoveStartGlobal;

    // Compute snapped delta relative to lead widget
    QRect leadStart = mMoveItems.first().startGeom; // first is lead due to beginGroupMove order
    QRect leadNow = leadStart.translated(delta);

    int nx = clampInt(leadNow.x(), 0, std::max(0, width() - leadNow.width()));
    int ny = clampInt(leadNow.y(), 0, std::max(0, height() - leadNow.height()));

    if (mSnapEnabled)
    {
        nx = clampInt(mSnapCoord(nx, mGrid.width()), 0, std::max(0, width() - leadNow.width()));
        ny = clampInt(mSnapCoord(ny, mGrid.height()), 0, std::max(0, height() - leadNow.height()));
    }

    const QPoint snappedLeadTopLeft(nx, ny);
    const QPoint snapDelta = snappedLeadTopLeft - leadNow.topLeft();

    // Apply to all
    for (const auto& it : std::as_const(mMoveItems))
    {
        if (!it.w)
            continue;

        QRect g = it.startGeom.translated(delta + snapDelta);
        // Clamp each within canvas
        const int maxX = std::max(0, width() - g.width());
        const int maxY = std::max(0, height() - g.height());
        g.moveLeft(clampInt(g.x(), 0, maxX));
        g.moveTop(clampInt(g.y(), 0, maxY));
        it.w->setGeometry(g);
    }
}

void TCanvasWidget::endGroupMove()
{
    DECL_TRACER("TCanvasWidget::endGroupMove()");

    if (!mGroupMoving)
        return;

    mGroupMoving = false;
    mMoveItems.clear();
    mMoveLead = nullptr;
}
