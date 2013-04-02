#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QFileDialog>
#include <QMessageBox>
#include <QtAlgorithms>
#include <QString>
#include <QTextStream>

#include "KedighCash.h"
#include "kedighkid.h"
#include "addCash.h"
#include "smtp.h"
#include "userWindow.h"
#include "MailThread.h"

const QString fromAddress = "<kedighcash@yahoo.com>";
const QString toAddress = "<larryk@wcs.edu>";
const QString fromPass = "LarryKBio";

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Q_SLOT void open();
    Q_SLOT void updateEmail();
    Q_SLOT void exportTab();
    Q_SLOT void displayInfo();
    Q_SLOT void displayInfo2();
    Q_SLOT void findCash();
    Q_SLOT void addCash();
    Q_SLOT void addCashFromWindow();
    Q_SLOT void saveData();
    Q_SLOT void exit();
    Q_SLOT void about();
    Q_SLOT void removeCash();
    Q_SLOT void killKid();
    Q_SLOT void login();

    Q_SLOT void tryUnlock(QString,QString);
    Q_SLOT void makeAccount(QString,QString);

    void checkForDoubles();

    CashWindow * addCashWindow;
    UserWindow * loginWindow;

    QString dataOutput();
    QString getList();
    QList<KedighKid> kids;
    
private:
    Ui::MainWindow *ui;
    QList<KedighCash> cashList;

    bool caseInsensitiveLessThan(const KedighCash &s1,
                                 const KedighCash &s2);

    QString m_password;
    QString m_username;

    void parseFile(QString fileInput);
    void sortKids();
    void checkForSaveFile();
    void checkForPasswordFile();
    void save();
    void countCash();

    bool m_loggedIn;
};

#endif // MAINWINDOW_H
