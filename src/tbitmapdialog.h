#ifndef TBITMAPDIALOG_H
#define TBITMAPDIALOG_H

#include <QDialog>
#include "tobjecthandler.h"

namespace Ui {
    class TBitmapDialog;
}

class QTableWidget;
class QLineEdit;
class TWidgetCombo;
class QSpinBox;

class TBitmapDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TBitmapDialog(QWidget *parent = nullptr);
        ~TBitmapDialog();

        void setBitmaps(const QList<ObjHandler::BITMAPS_t>& bm);

    protected:
        QTableWidget *createTable(const ObjHandler::BITMAPS_t& bm);
        QWidget *makeBitmapSelector(const QString& bitmap, const QString& name);
        TWidgetCombo *makeTextJustification(ObjHandler::ORIENTATION ori, const QString& name);
        QSpinBox *makeValueSelector(int value, const QString& name);
        void setBitmap(QLineEdit *line);
        void setValue(const QString& name, const QVariant& value);

    private slots:
        void on_pushButtonAdd_clicked();
        void on_pushButtonDelete_clicked();
        void on_pushButtonMoveUp_clicked();
        void on_pushButtonMoveDown_clicked();
        void on_pushButton_clicked();

    private:
        Ui::TBitmapDialog *ui;
};

#endif // TBITMAPDIALOG_H
