#include "tcombobutton.h"

#include <QApplication>
#include <QListWidget>
#include <QMouseEvent>
#include <QScreen>
#include <QStyle>

#include "terror.h"

TComboButton::TComboButton(QWidget* parent)
    : QPushButton(parent)
{
    DECL_TRACER("TComboButton::TComboButton(QWidget* parent)");

    mText = "...";
    init();
}

TComboButton::TComboButton(const QString& text, QWidget* parent)
    : QPushButton(parent),
      mText(text)
{
    DECL_TRACER("TComboButton::TComboButton(const QString& text, QWidget* parent)");

    init();
    setText(text);
}

TComboButton::TComboButton(const QString& text, const QStringList& items, QWidget* parent)
    : QPushButton(parent),
      mText(text)
{
    DECL_TRACER("TComboButton::TComboButton(const QString& text, const QStringList& items, QWidget* parent)");

    init();
    setText(text);
    setItems(items);
}

TComboButton::~TComboButton()
{
    DECL_TRACER("TComboButton::~TComboButton()");

    if (qApp)
        qApp->removeEventFilter(this);

    delete mList;
}

void TComboButton::init()
{
    DECL_TRACER("TComboButton::init()");

    setText(mText);
    // Look like a combobox: text with a down-arrow on the right
    setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
    setLayoutDirection(Qt::RightToLeft);
    setIconSize(QSize(18, 18));

    // Non-modal popup list (top-level, frameless, stays on top)
    mList = new QListWidget; // no parent -> top-level
    mList->setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowStaysOnTopHint);
    mList->setSelectionMode(QAbstractItemView::SingleSelection);
    mList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Hide on focus out/deactivate (extra safety)
    mList->installEventFilter(this);

    connect(mList, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        if (!(item->flags() & Qt::ItemIsEnabled))
            return; // ignore disabled

        const int idx = mList->row(item);
        mCurrentIndex = idx;
        setText(item->text());
        emit currentIndexChanged(idx);
        emit currentTextChanged(item->text());
        hidePopup();
    });

    // Toggle popup on press: if visible -> close (no selection), else open.
    connect(this, &QPushButton::pressed, this, [this] {
        if (mList->isVisible())
            hidePopup();
        else
            showPopup();
    });
}

void TComboButton::setItems(const QStringList& items)
{
    DECL_TRACER("TComboButton::setItems(const QStringList& items)");

    mItems = items;
    mList->clear();
    mList->addItems(items);

    // Select the first enabled item by default
    int firstEnabled = -1;

    for (int i = 0; i < mList->count(); ++i)
    {
        if (mList->item(i)->flags() & Qt::ItemIsEnabled)
        {
            firstEnabled = i;
            break;
        }
    }

    if (firstEnabled >= 0)
    {
        setCurrentIndex(firstEnabled);
    }
    else
    {
        mCurrentIndex = -1;
        setText(mText);
    }
}

void TComboButton::enableItem(int index, bool state)
{
    DECL_TRACER("TComboButton::enableItem(int index, bool state)");

    disableItem(index, !state);
}

void TComboButton::disableItem(int index, bool state)
{
    DECL_TRACER("TComboButton::disableItem(int index, bool state)");

    if (index < 0 || index >= mItems.size())
        return;

    int idx = mDisabled.indexOf(index);

    if (!state && idx >= 0)
    {
        mDisabled.remove(idx, 1);
        QListWidgetItem* it = mList->item(index);

        if (!it)
            return;

        it->setFlags(it->flags() & ~(Qt::ItemIsEnabled | Qt::ItemIsSelectable));
        return;
    }

    mDisabled.append(index);
    mDisabled.remove(idx, 1);
    QListWidgetItem* it = mList->item(index);

    if (!it)
        return;

    it->setFlags(it->flags() & (Qt::ItemIsEnabled | Qt::ItemIsSelectable));
}

void TComboButton::setData(const QList<QVariant>& data)
{
    DECL_TRACER("TComboButton::setData(const QList<QVariant>& data)");

    int loop = qMin(mItems.size(), data.size());

    for (int i = 0; i < loop; ++loop)
    {
        QListWidgetItem* it = mList->item(i);
        it->setData(Qt::UserRole, data[i]);
    }
}

void TComboButton::setCurrentIndex(int index)
{
    DECL_TRACER("TComboButton::setCurrentIndex(int index)");

    if (index < 0 || index >= mList->count())
        return;

    auto* item = mList->item(index);

    if (!(item->flags() & Qt::ItemIsEnabled))
        return;

    mCurrentIndex = index;
    mList->setCurrentItem(item);
    setText(item->text());
    emit currentIndexChanged(index);
    emit currentTextChanged(item->text());
}

QString TComboButton::getText(int index)
{
    DECL_TRACER("TComboButton::getText(int index)");

    if (index < 0 || index >= mItems.size())
        return QString();

    return mItems[index];
}

bool TComboButton::isEnabled(int index)
{
    DECL_TRACER("TComboButton::isEnabled(int index)");

    if (mDisabled.indexOf(index) < 0)
        return true;

    return false;
}

bool TComboButton::isDisabled(int index)
{
    DECL_TRACER("TComboButton::isDisabled(int index)");

    return !isEnabled(index);
}

void TComboButton::resizeEvent(QResizeEvent* e)
{
    QPushButton::resizeEvent(e);

    if (mList->isVisible())
        updatePopupGeometry();
}

void TComboButton::updatePopupGeometry() const
{
    // Same width as the button, top of list aligned to bottom of button
    const int rows = qMin(10, mList->count()); // show up to 10 lines
    int rowHeight = mList->sizeHintForRow(0);

    if (rowHeight <= 0)
        rowHeight = fontMetrics().height() + 8;

    const int frame = mList->frameWidth() * 2;
    const int height = rowHeight * rows + frame;

    QSize sz(width(), height);
    QPoint globalTopLeft = mapToGlobal(QPoint(0, this->height()));

    // Keep on-screen
    if (QScreen* screen = QGuiApplication::screenAt(globalTopLeft))
    {
        QRect avail = screen->availableGeometry();

        if (globalTopLeft.y() + sz.height() > avail.bottom())
            globalTopLeft.setY(qMax(avail.top(), mapToGlobal(QPoint(0, 0)).y() - sz.height()));

        if (globalTopLeft.x() + sz.width() > avail.right())
            globalTopLeft.setX(avail.right() - sz.width());

        if (globalTopLeft.x() < avail.left())
            globalTopLeft.setX(avail.left());
    }

    mList->setFixedWidth(sz.width());
    mList->setFixedHeight(sz.height());
    mList->move(globalTopLeft);
}

void TComboButton::showPopup()
{
    DECL_TRACER("TComboButton::showPopup()");

    if (mList->count() == 0)
        return;

    updatePopupGeometry();
    qApp->installEventFilter(this);  // close on outside click
    mList->show();
    mList->raise();
    mList->activateWindow();
    mList->setFocus();
}

void TComboButton::hidePopup()
{
    DECL_TRACER("TComboButton::hidePopup()");

    if (!mList->isVisible())
        return;

    mList->hide();
    qApp->removeEventFilter(this);
}

bool TComboButton::eventFilter(QObject* obj, QEvent* ev)
{
    // Close the non-modal list if user clicks outside or the list loses activation
    if (!mList->isVisible())
        return false;

    switch (ev->type())
    {
        case QEvent::MouseButtonPress:
        {
            // If click is outside both the list and the button, close (let event propagate).
            QPoint globalPos;

            if (auto* me = dynamic_cast<QMouseEvent*>(ev))
                globalPos = me->globalPosition().toPoint();
            else
                globalPos = QCursor::pos();

            const QRect listRect = QRect(mList->pos(), mList->size());
            const QRect btnRect = QRect(this->mapToGlobal(QPoint(0,0)), this->size());

            if (!listRect.contains(globalPos) && !btnRect.contains(globalPos))
                hidePopup();

            break;
        }

        case QEvent::WindowDeactivate:
        case QEvent::FocusOut:
            if (obj == mList)
                hidePopup();
        break;

        case QEvent::KeyPress:
        {
            if (obj == mList)
            {
                auto* ke = static_cast<QKeyEvent*>(ev);

                if (ke->key() == Qt::Key_Escape)
                    hidePopup();
            }

            break;
        }

        default:
            break;
    }

    return false; // never eat the event; keep it non-modal
}
