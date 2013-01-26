#ifndef MAILTHREAD_H
#define MAILTHREAD_H
#include <QList>
#include <QThread>
#include <QEventLoop>
#include <QString>

#include "smtp.h"

class MailThread : public QThread {
    Q_OBJECT
public:
    MailThread(const QString &fromUser, const QString &toAddress,
               const QString &fromPass, const QString &subject,
               const QString & messageBody);

    virtual ~MailThread();

    void run();

private:
    QString m_sendTo;
    QString m_from;
    QString m_Message;
    QString m_Server;
    QString m_password;
    QString m_Subject;
    QString m_User;
};

#endif // MAILTHREAD_H
