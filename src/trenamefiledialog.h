#ifndef TRENAMEFILEDIALOG_H
#define TRENAMEFILEDIALOG_H

#include <QDialog>

namespace Ui {
class TRenameFileDialog;
}

class TRenameFileDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TRenameFileDialog(QWidget *parent = nullptr);
        TRenameFileDialog(const QString& file, QWidget *parent = nullptr);
        ~TRenameFileDialog();

        void setFileName(const QString& file);
        QString& getFileName() { return mFileName; }

    protected:
        void accept() override;

    private slots:
        void on_lineEditFile_textChanged(const QString &arg1);

    private:
        Ui::TRenameFileDialog *ui;
        QString mFileName;
};

#endif // TRENAMEFILEDIALOG_H
