#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include "KedighCash.h"

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
    
private:
    Ui::MainWindow *ui;
    QList<KedighCash> cashList;
    void parseFile(QString fileInput);
};

#endif // MAINWINDOW_H
