#ifndef TCOMBOBUTTON_H
#define TCOMBOBUTTON_H

#include <QPushButton>

class QListWidget;

class TComboButton : public QPushButton
{
    Q_OBJECT

    public:
        explicit TComboButton(QWidget* parent = nullptr);
        TComboButton(const QString& text, QWidget* parent = nullptr);
        TComboButton(const QString& text, const QStringList& items, QWidget* parent = nullptr);
        ~TComboButton() override;

        void setItems(const QStringList& items);
        void setData(const QList<QVariant>& data);
        void enableItem(int index, bool state);
        void disableItem(int index, bool state);
        int currentIndex() const { return mCurrentIndex; }
        QString currentText() const { return text(); }
        void setCurrentIndex(int index);
        QString getText(int index);
        bool isEnabled(int index);
        bool isDisabled(int index);

    signals:
        void currentIndexChanged(int index);
        void currentTextChanged(const QString& text);

    protected:
        void resizeEvent(QResizeEvent* e) override;
        bool eventFilter(QObject* obj, QEvent* ev) override;

    private:
        void init();
        void showPopup();
        void hidePopup();
        void updatePopupGeometry() const;

        QListWidget *mList = nullptr;   // non-modal, top-level list
        int mCurrentIndex = -1;
        QStringList mItems;
        QList<QVariant> mData;
        QList<int> mDisabled;
        QString mText;
};

#endif // COMBO_BUTTON_H
