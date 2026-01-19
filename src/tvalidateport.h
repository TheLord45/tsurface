#ifndef TVALIDATEPORT_H
#define TVALIDATEPORT_H

#include <QValidator>

class TValidatePort : public QValidator
{
    Q_OBJECT

    public:
        TValidatePort(QObject *parent=nullptr);
        ~TValidatePort();

        void fixup(QString &input) const override;
        QValidator::State validate(QString &input, int &pos) const override;

    private:
        int mSystemPort{0};
        QString mSysString;
};

#endif // TVALIDATEPORT_H
