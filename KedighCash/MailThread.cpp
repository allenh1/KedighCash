#include "MailThread.h"

MailThread::MailThread(const QString &fromUser, const QString &toAddress,
                       const QString &fromPass, const QString &subject,
                       const QString & messageBody)
{
    m_from = fromUser;
    m_sendTo = toAddress;
    m_password = fromPass;
    m_Subject = subject;
    m_Message = messageBody;
}//set up the mail thread with the message data

void MailThread::run()
{
    /** Send the Message **/
    QStringList receivers;
    receivers.push_back(m_sendTo);

    Smtp *newMail = new Smtp("smtp.mail.yahoo.com", "kedighcash@yahoo.com",
                             m_password, m_from, receivers, m_Subject, m_Message);

    QEventLoop loop;
    QObject::connect(newMail, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}//execute the thread

MailThread::~MailThread()
{
    wait();
}
