#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QFileDialog>
#include <QMessageBox>
#include <QtAlgorithms>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextStream>
#include <QLineEdit>
#include <QMessageBox>

#include "KedighCash.h"
#include "kedighkid.h"

class UserWindow : public QWidget
{
    Q_OBJECT

public:
    UserWindow(QWidget * parent = 0, bool existingFile = true);
    //~UserWindow();
    void setWindowFlags(Qt::WindowFlags flags);

    Q_SLOT void close();
    Q_SIGNAL void unlockCash(QString,QString);
    Q_SIGNAL void createAccount(QString,QString);

    const QString & getUser();
    const QString & getPassword();

private:
    QLabel *p_label;
    QLabel *p_label2;

    QString m_userName;
    QString m_password;

    QLineEdit *nameEdit;
    QLineEdit *passwordEdit;

    QVBoxLayout *mainLayout;
    QHBoxLayout *layout;
    QHBoxLayout *layout2;

    QPushButton *loginButton;

    bool m_createOnly;
};

#endif // USERWINDOW_H
