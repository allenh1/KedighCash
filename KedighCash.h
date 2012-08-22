#ifndef KEDIGHCASH_H
#define KEDIGHCASH_H
#include <QString>
#include <QFile>

class KedighCash
{
public:
    KedighCash(QString last, QString _email, QString date,
               QString per, QString ser, QString remote, int denom);

    QString toString();

private:
    int denomination;

    QString lastname;
    QString email;
    QString dateSent;
    QString period;
    QString serial;
    QString remoteAddress;
};

#endif // KEDIGHCASH_H
