#include "KedighCash.h"

KedighCash::KedighCash(QString ser, QString _date, int denom)
{
    m_serial = ser;
    m_denomination = denom;
}//end constructor.

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

QString KedighCash::getSerial()
{ return m_serial; }
