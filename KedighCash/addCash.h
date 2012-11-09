#ifndef ADDCASH_H
#define ADDCASH_H

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
#include "KedighCash.h"
#include "kedighkid.h"

class CashWindow : public QWidget
{
    Q_OBJECT

public:
    CashWindow(QWidget * parent = 0);
    //~CashWindow();
    void setWindowFlags(Qt::WindowFlags flags);

    Q_SLOT void close();
    Q_SLOT void sendToMain();
    Q_SIGNAL void newKid();
    KedighKid getLastKid();

    QString name;
    QString period;
    QString email;
    QString serial;
    QString denom;
private:
    QLabel *p_label;
    QLabel *p_label2;
    QLabel *p_label3;
    QLabel *p_label4;

    QLineEdit *serialEdit;
    QLineEdit *nameEdit;
    QLineEdit *denomEdit;

    QVBoxLayout *mainLayout;
    QHBoxLayout *layout;
    QHBoxLayout *layout2;
    QHBoxLayout *layout3;
    QHBoxLayout *layout4;

    QPushButton *closeButton;


};

#endif
