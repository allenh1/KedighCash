#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include "KedighCash.h"
#include "kedighkid.h"

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
    Q_SLOT void exportTab();
    Q_SLOT void displayInfo();
    Q_SLOT void displayInfo2();
    Q_SLOT void findCash();
    QString getList();
    
private:
    Ui::MainWindow *ui;
    QList<KedighCash> cashList;
    QList<KedighKid> kids;
    void parseFile(QString fileInput);
    void sortKids();
};

#endif // MAINWINDOW_H
