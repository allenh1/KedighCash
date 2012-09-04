#include "KedighCash.h"

KedighCash::KedighCash(QString ser, QString _date, int denom)
{
    m_serial = ser;
    m_date = _date;
    m_denomination = denom;
    toString();
}//end constructor.

KedighCash::KedighCash(QString ser, QString _date, QString _owner, QString cashAddr, int denom)
{
    m_serial = ser;
    m_date = _date;
    m_owner = _owner;
    m_remoteAddr = cashAddr;
    m_denomination = denom;
    toString();
}//end constructor

void KedighCash::setOwner(QString owner)
{ m_owner = owner; }

void KedighCash::reset()
{ m_owner = ""; }

bool KedighCash::hasOwner()
{ return m_owner != ""; }

int KedighCash::getValue()
{ return m_denomination; }

QString KedighCash::getCashDate()
{ return m_date; }

QString KedighCash::getAddress()
{ return m_remoteAddr; }

void KedighCash::toString()
{
    QString toReturn = "";
    toReturn += m_serial;
    toReturn += ": ";
    toReturn += m_date;
    QString convert;
    convert.setNum(m_denomination);
    toReturn += " :$ "+convert;
    toReturn += ": "+m_remoteAddr;

    m_string = toReturn;
}//get the string

QString KedighCash::getSerial()
{ return m_serial; }
