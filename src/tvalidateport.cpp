#include "tvalidateport.h"
#include "tconfmain.h"
#include "terror.h"

TValidatePort::TValidatePort(QObject *parent)
    : QValidator(parent)
{
    DECL_TRACER("TValidatePort::TValidatePort(QObject *parent)");

    mSystemPort = TConfMain::Current().getSetupPort();
    mSysString = QString("%1 - setup port").arg(mSystemPort);
}

TValidatePort::~TValidatePort()
{
    DECL_TRACER("TValidatePort::~TValidatePort()");
}

void TValidatePort::fixup(QString &input) const
{
    DECL_TRACER("TValidatePort::fixup(QString &input)");

    QString start = QString("%1 - ").arg(mSystemPort);

    if (input.startsWith(start))
        input = QString("%1 - setup port");
}

QValidator::State TValidatePort::validate(QString &input, int &pos) const
{
    DECL_TRACER("TValidatePort::validate(QString &input, int &pos)");

    if (input == mSysString)
        return QValidator::Acceptable;

    if (input.isEmpty())
        return QValidator::Intermediate;

    if (input.length() > 3)
        return QValidator::Invalid;

    for (int i = 0; i < pos; ++i)
    {
        if (input[i] < '0' || input[i] > '9')
            return QValidator::Invalid;
    }

    int port = input.toInt();

    if (port > 255)
        return QValidator::Invalid;

    return QValidator::Acceptable;
}
