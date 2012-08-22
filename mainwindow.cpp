#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parseFile(QString fileInput)
{
    if (fileInput != "")
    {
        QFile file(fileInput);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }//end if
        QTextStream in(&file);
        QList<QString> asList;
        QList<QString> dataList;

        while (!in.atEnd())
        {
            asList.push_back(in.readLine());
        }//iterate through the file. All of it. Store.

        ui->fileDisplay->clear();

        for (int x = 0; x < asList.size(); x++)
        {
            if (asList.at(x) == "" && x > 0)
            { asList.removeAt(x); x--; }
            //remove the blank lines
        }//end for x

        for (int z = 0; z < asList.size(); z++)
        {
            /** Order of the file here,
              lastname:
              period:
              version:
              denomination:
              serial:
              Submit: Submit
              HTTP_USER_AGENT:
              WOW64;
              REMOTE_ADDR:
              From: lastname [email]
              Sent: date **/

            if (asList.at(z).contains("lastname: "))
            {
                QString lastname, period, version, denom,
                        serial, remote, email, date;
                //we have a section to extract from.
                QString toSize;
                QString current;
                current = asList.at(z);
                int theSize = 0;
                toSize = "lastname: ";
                theSize = toSize.size();
                current.remove(0, theSize - 1);
                lastname = current;
                z++; //we can go to the next line now

                toSize = "period: ";
                current = asList.at(z);
                current.remove(0, toSize.size() - 1);
                period = current;
                z++;

                current = asList.at(z);
                toSize = "version: ";
                current.remove(0, toSize.size() - 1);
                version = current;
                z++;

                toSize = "denomination: ";
                current = asList.at(z);
                current.remove(0, toSize.size() - 1);
                denom = current;
                z++;

                toSize = "serial: ";
                current = asList.at(z);
                current.remove(0, toSize.size() - 1);
                serial = current;
                z += 3; //go to remote line

                toSize = "REMOTE_ADDR: ";
                current = asList.at(z);
                current.remove(0, toSize.size() - 1);
                remote = current;

                dataList.push_back(lastname +" "+period+" "+version+
                                   " "+denom+" "+" "+serial+" "+remote);
            }
        }

        for (int y = 0; y < dataList.size(); y++)
        {
            ui->fileDisplay->addItem(dataList.at(y));
        }

        file.close();
    }//end if
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt);; C++ Files (*.cpp *.h *.cc)"));
    parseFile(fileName);
}//open file dialog
