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
    void removeMoney(int index);

    QList<KedighCash> cashOwned();

    QString name;
    QString getSummary();
    QString getEmail();

    bool operator < (const KedighKid& k2) const
    {
        KedighKid other = k2;
        return name.toLower() < other.name.toLower();
    }

    bool operator > (const KedighKid& k2) const
    {
        KedighKid other = k2;
        return name.toLower() > other.name.toLower();
    }//end bool

    bool operator == (const KedighKid& k2) const
    {
        KedighKid other = k2;
        return name.toLower() == other.name.toLower();
    }//end bool

    bool operator != (const KedighKid& k2) const
    {
        KedighKid other = k2;
        return name.toLower() != other.name.toLower();
    }

    bool operator >= (const KedighKid& k2) const
    {
        KedighKid other = k2;
        return name.toLower() >= other.name.toLower();
    }

    bool operator <= (const KedighKid& k2) const
    {
        KedighKid other = k2;
        return name.toLower() <= other.name.toLower();
    }//end bool

private:
    void updateAccount();

    QString lastname;
    QString period;
    QString email;

    QList<KedighCash> money;
};

#endif // KEDIGHKID_H
