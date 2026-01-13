#include <QMessageBox>

#include "trenamefiledialog.h"
#include "ui_trenamefiledialog.h"
#include "terror.h"

TRenameFileDialog::TRenameFileDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TRenameFileDialog)
{
    ui->setupUi(this);
}

TRenameFileDialog::TRenameFileDialog(const QString& file, QWidget *parent)
    : QDialog(parent),
      mFileName(file),
      ui(new Ui::TRenameFileDialog)
{
    DECL_TRACER("TRenameFileDialog::TRenameFileDialog(const QString& file, QWidget *parent)");

    ui->setupUi(this);
    ui->lineEditFile->setText(file);
}

TRenameFileDialog::~TRenameFileDialog()
{
    delete ui;
}

void TRenameFileDialog::on_lineEditFile_textChanged(const QString &arg1)
{
    DECL_TRACER("TRenameFileDialog::on_lineEditFile_textChanged(const QString &arg1)");

    mFileName = arg1;
    QString invalChars("!\"$%&\\()?*`´;<>|'");
    QString::Iterator iter;

    for (iter = invalChars.begin(); iter != invalChars.end(); ++iter)
    {
        if (mFileName.contains(*iter))
        {
            mFileName.remove(*iter);
            ui->lineEditFile->setText(mFileName);
            break;
        }
    }
}

void TRenameFileDialog::accept()
{
    DECL_TRACER("TRenameFileDialog::accept()");

    if (!mFileName.isEmpty())
    {
        if (!mFileName.endsWith(".png") && !mFileName.endsWith(".jpg"))
        {
            QMessageBox::warning(this, tr("Invalid file name"), tr("The file name must either end with the extension <b>.png</b> or <b>.jpg</b>!"));
            return;
        }
    }

    done(QDialog::Accepted);
}

void TRenameFileDialog::setFileName(const QString& file)
{
    DECL_TRACER("TRenameFileDialog::setFileName(const QString& file)");

    ui->lineEditFile->setText(file);
    mFileName = file;
}
