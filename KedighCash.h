#ifndef KEDIGHCASH_H
#define KEDIGHCASH_H
#include <QString>
#include <QFile>

class KedighCash
{
public:
    KedighCash(QString ser,  QString _date, int denom = 1);
    KedighCash(QString ser, QString _date, QString _owner, QString cashAddr, int denom);

    void toString();

    int getValue();
    int m_val;

    QString getCashDate();

    QString getSerial();
    QString m_string;
    QString getAddress();

    bool hasOwner();

    void setOwner(QString owner);
    void reset();

private:
    QString m_owner;
    QString m_remoteAddr;
    QString m_date;
    QString m_serial;
    int m_denomination;
};

#endif // KEDIGHCASH_H
