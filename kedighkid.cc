#include "kedighkid.h"

KedighKid::KedighKid(QString _name, QString _period, QString _email)
{
    lastname = _name;
    name = _name;
    period = _period;
    email = _email;
}

void KedighKid::addMoney(KedighCash toAdd)
{
    money.push_back(toAdd);
}

QString KedighKid::getEmail()
{
    return email;
}

void KedighKid::updateAccount()
{
    for (int x = 0; x < money.size(); x++)
    {
        bool broke = false;
        KedighCash current1 = money.at(x);

        for (int y = 0; y < money.size(); y++)
        {
            if (y == x)
                y++;
            KedighCash current2 = money.at(y);

            if (current2.getSerial() == current1.getSerial())
            {
                money.removeAt(y);
                broke = true;
                break;
            }//end if
        }//end for x

        if (broke)
            x--;
    }//end for x
}

QList<KedighCash> KedighKid::cashOwned()
{ updateAccount(); return money;}

int KedighKid::getBalance()
{
    int balance = 0;

    for (int x = 0; x < money.size(); x++)
    {
        KedighCash current = money.at(x);
        int current1 = 0;
        current1 = current.getValue();
        balance += current1;
    }//end for x.

    return balance;
}//end void.
