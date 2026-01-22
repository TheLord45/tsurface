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
#include <QMouseEvent>
#include <QEvent>
#include <algorithm>

#include "tresizablewidget.h"
#include "tcanvaswidget.h"
#include "terror.h"

// Small grip widget that handles drag-resize
class TResizableWidget::Grip : public QWidget
{
    public:
        Grip(TResizableWidget* owner, Handle h)
            : QWidget(owner), m_owner(owner), m_handle(h)
        {
            setCursor(TResizableWidget::cursorFor(h));
            setAttribute(Qt::WA_NoMousePropagation, true);
            setAttribute(Qt::WA_TransparentForMouseEvents, false);
            setMouseTracking(true);
            setToolTip("Drag to resize");
        }

    protected:
        void mousePressEvent(QMouseEvent* e) override
        {
            if (e->button() != Qt::LeftButton)
                return;

            m_dragging = true;
            m_startGlobal = e->globalPosition().toPoint();
            m_startGeom = m_owner->geometry();
            grabMouse();
            e->accept();
        }

        void mouseMoveEvent(QMouseEvent* e) override
        {
            if (!m_dragging)
                return;

            const QPoint gp = e->globalPosition().toPoint();
            const QPoint delta = gp - m_startGlobal;

            QRect g = m_startGeom;
            const int minW = m_owner->minimumWidth();
            const int minH = m_owner->minimumHeight();

            auto clampLeft = [&](int newLeft)
            {
                const int maxLeft = g.right() - (minW - 1);
                return std::min(newLeft, maxLeft);
            };
            auto clampTop = [&](int newTop)
            {
                const int maxTop = g.bottom() - (minH - 1);
                return std::min(newTop, maxTop);
            };

            switch (m_handle)
            {
                case TopLeft:
                    g.setLeft(clampLeft(m_startGeom.left() + delta.x()));
                    g.setTop(clampTop(m_startGeom.top() + delta.y()));
                    break;
                case Top:
                    g.setTop(clampTop(m_startGeom.top() + delta.y()));
                    break;
                case TopRight:
                    g.setRight(std::max(m_startGeom.right() + delta.x(), m_startGeom.left() + minW - 1));
                    g.setTop(clampTop(m_startGeom.top() + delta.y()));
                    break;
                case Right:
                    g.setRight(std::max(m_startGeom.right() + delta.x(), m_startGeom.left() + minW - 1));
                    break;
                case BottomRight:
                    g.setRight(std::max(m_startGeom.right() + delta.x(), m_startGeom.left() + minW - 1));
                    g.setBottom(std::max(m_startGeom.bottom() + delta.y(), m_startGeom.top() + minH - 1));
                    break;
                case Bottom:
                    g.setBottom(std::max(m_startGeom.bottom() + delta.y(), m_startGeom.top() + minH - 1));
                    break;
                case BottomLeft:
                    g.setLeft(clampLeft(m_startGeom.left() + delta.x()));
                    g.setBottom(std::max(m_startGeom.bottom() + delta.y(), m_startGeom.top() + minH - 1));
                    break;
                case Left:
                    g.setLeft(clampLeft(m_startGeom.left() + delta.x()));
                    break;
                default: break;
            }

            m_owner->setGeometry(g);
            e->accept();
        }

        void mouseReleaseEvent(QMouseEvent* e) override
        {
            if (m_dragging && e->button() == Qt::LeftButton)
            {
                m_dragging = false;
                releaseMouse();
                e->accept();
            }
        }

        void paintEvent(QPaintEvent*) override
        {
            QPainter p(this);
            p.setRenderHint(QPainter::Antialiasing, false);
            p.setPen(Qt::black);
            p.setBrush(Qt::white);
            p.drawRect(rect().adjusted(0, 0, -1, -1));
        }

    private:
        TResizableWidget* m_owner;
        Handle m_handle;
        bool m_dragging = false;
        QPoint m_startGlobal;
        QRect m_startGeom;
};

// TResizableWidget implementation
TResizableWidget::TResizableWidget(QWidget* content, QWidget* parent)
    : QWidget(parent)
{
    DECL_TRACER("TResizableWidget::TResizableWidget(QWidget* content, QWidget* parent)");

    setAttribute(Qt::WA_Hover, true);
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background: transparent;");
    setMouseTracking(true);

    for (int i = 0; i < HandleCount; ++i)
    {
        m_grips[i] = new Grip(this, static_cast<Handle>(i));
        m_grips[i]->setFixedSize(mGripSize, mGripSize);
        m_grips[i]->raise();
        m_grips[i]->hide(); // hidden until selected
    }

    if (content)
        setContentWidget(content);
}

void TResizableWidget::setContentWidget(QWidget* w)
{
    DECL_TRACER("TResizableWidget::setContentWidget(QWidget* w)");

    if (mContent == w)
        return;

    if (mContent)
    {
        mContent->removeEventFilter(this);
        mContent->deleteLater();
    }

    mContent = w;

    if (mContent)
    {
        mContent->setParent(this);
        mContent->installEventFilter(this);
        mContent->show();
        mContent->raise();
        layoutGrips();
        setSelected(mSelected);
    }
}

void TResizableWidget::setSelected(bool on)
{
    DECL_TRACER("TResizableWidget::setSelected(bool on)");

    if (mSelected == on)
        return;

    mSelected = on;
    updateGripsVisibility();

    if (mContent)
        mContent->setCursor(mSelected ? Qt::SizeAllCursor : Qt::ArrowCursor);

    update();
}

void TResizableWidget::paintEvent(QPaintEvent*)
{
    if (!mSelected)
        return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);
    QPen pen(Qt::black);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(1);
    p.setPen(pen);
    const QRect frameRect = rect().adjusted(0, 0, -1, -1);
    p.drawRect(frameRect);
}

void TResizableWidget::resizeEvent(QResizeEvent*)
{
    if (mContent)
    {
        const int m = mFrameMargin + 2;
        mContent->setGeometry(m, m, std::max(0, width() - 2 * m), std::max(0, height() - 2 * m));
    }

    layoutGrips();
}

void TResizableWidget::layoutGrips()
{
    DECL_TRACER("TResizableWidget::layoutGrips()");

    const int s = mGripSize;
    const int w = width();
    const int h = height();

    auto place = [&](Handle hnd, int x, int y)
    {
        m_grips[hnd]->move(x, y);
        m_grips[hnd]->raise();
    };

    place(TopLeft,       0,          0);
    place(TopRight,      w - s,      0);
    place(BottomRight,   w - s,      h - s);
    place(BottomLeft,    0,          h - s);

    place(Top,           (w - s) / 2, 0);
    place(Right,         w - s,       (h - s) / 2);
    place(Bottom,        (w - s) / 2, h - s);
    place(Left,          0,           (h - s) / 2);
}

void TResizableWidget::updateGripsVisibility()
{
    DECL_TRACER("TResizableWidget::updateGripsVisibility()");

    for (auto* g : m_grips)
    {
        if (!g)
            continue;

        mSelected ? g->show() : g->hide();
    }
}

Qt::CursorShape TResizableWidget::cursorFor(Handle h)
{
    DECL_TRACER("TResizableWidget::cursorFor(Handle h)");

    switch (h)
    {
        case TopLeft:
        case BottomRight: return Qt::SizeFDiagCursor;
        case TopRight:
        case BottomLeft:  return Qt::SizeBDiagCursor;
        case Left:
        case Right:       return Qt::SizeHorCursor;
        case Top:
        case Bottom:      return Qt::SizeVerCursor;
        default:          return Qt::ArrowCursor;
    }
}

// ---- Drag-to-move delegation to CanvasWidget (group move) ----
void TResizableWidget::beginMove(const QPoint& globalPos)
{
    mMoving = true;
    mMoveStartGlobal = globalPos;
    mMoveStartGeom = geometry();

    if (auto* canvas = qobject_cast<TCanvasWidget*>(parentWidget()))
        canvas->beginGroupMove(this, globalPos);

    grabMouse();
}

void TResizableWidget::updateMove(const QPoint& globalPos)
{
    if (!mMoving)
        return;

    if (auto* canvas = qobject_cast<TCanvasWidget*>(parentWidget()))
        canvas->updateGroupMove(globalPos);
}

void TResizableWidget::endMove()
{
    if (!mMoving)
        return;

    if (auto* canvas = qobject_cast<TCanvasWidget*>(parentWidget()))
        canvas->endGroupMove();

    mMoving = false;
    releaseMouse();
}

void TResizableWidget::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        // If press is on a grip, let resizing handle it (grips exist only when selected)
        QWidget* child = childAt(e->pos());
        const bool onGrip = child && dynamic_cast<Grip*>(child) != nullptr;

        auto* canvas = qobject_cast<TCanvasWidget*>(parentWidget());
        const bool ctrl = e->modifiers() & Qt::ControlModifier;

        if (!onGrip)
        {
            if (canvas)
            {
                if (ctrl)
                    canvas->toggleSelection(this);
                else
                    canvas->selectOnly(this);

                // Start group move only if this widget is selected after toggle
                if (canvas->isSelected(this))
                    beginMove(e->globalPosition().toPoint());
            }
            else
                beginMove(e->globalPosition().toPoint());

            e->accept();
            return;
        }
    }

    QWidget::mousePressEvent(e);
}

void TResizableWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (mMoving)
    {
        updateMove(e->globalPosition().toPoint());
        e->accept();
        return;
    }

    QWidget::mouseMoveEvent(e);
}

void TResizableWidget::mouseReleaseEvent(QMouseEvent* e)
{
    if (mMoving && e->button() == Qt::LeftButton)
    {
        endMove();
        e->accept();
        return;
    }

    QWidget::mouseReleaseEvent(e);
}

bool TResizableWidget::eventFilter(QObject* obj, QEvent* ev)
{
    if (obj == mContent)
    {
        switch (ev->type())
        {
            case QEvent::MouseButtonPress:
            {
                QMouseEvent *e = static_cast<QMouseEvent*>(ev);

                if (e->button() == Qt::LeftButton)
                {
                    TCanvasWidget *canvas = qobject_cast<TCanvasWidget*>(parentWidget());
                    const bool ctrl = e->modifiers() & Qt::ControlModifier;

                    if (canvas)
                    {
                        if (ctrl)
                            canvas->toggleSelection(this);
                        else
                            canvas->selectOnly(this);

                        if (canvas->isSelected(this))
                            beginMove(e->globalPosition().toPoint());
                    }
                    else
                        beginMove(e->globalPosition().toPoint());

                    ev->accept();
                    return true;
                }
                break;
            }

            case QEvent::MouseMove:
            {
                QMouseEvent *e = static_cast<QMouseEvent*>(ev);

                if (mMoving)
                {
                    updateMove(e->globalPosition().toPoint());
                    ev->accept();
                    return true;
                }
                break;
            }

            case QEvent::MouseButtonRelease:
            {
                QMouseEvent *e = static_cast<QMouseEvent*>(ev);

                if (mMoving && e->button() == Qt::LeftButton)
                {
                    endMove();
                    ev->accept();
                    return true;
                }

                break;
            }

            default:
                break;
        }
    }

    return QWidget::eventFilter(obj, ev);
}
