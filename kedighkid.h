#ifndef KEDIGHKID_H
#define KEDIGHKID_H
#include "KedighCash.h"

class KedighKid
{
public:
    KedighKid(QString _name, QString _period, QString _email);
    int getBalance();
    int compareTo(KedighKid other);
    void addMoney(KedighCash toAdd);

    QList<KedighCash> cashOwned();

    QString name;
    QString getSummary();
    QString getEmail();

private:
    void updateAccount();

    QString lastname;
    QString period;
    QString email;

    QList<KedighCash> money;
};

#endif // KEDIGHKID_H
