#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
     connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(findCash()));
     connect(ui->fileDisplay, SIGNAL(currentRowChanged()), this, SLOT(displayInfo2()));
     connect(ui->actionExport, SIGNAL(triggered()), this, SLOT(exportTab()));
     connect(ui->studentSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(displayInfo()));
     connect(ui->deleteCurrency, SIGNAL(clicked()), this, SLOT(removeCash()));
}

void MainWindow::displayInfo()
{
    int index, indexb;
    index = ui->studentSelect->currentIndex();

    if (index == -1)
        index = 0;
    KedighKid current = kids.at(index);
    QString balance, balance1;
    QString dollaSign = "$";
    balance1.setNum(current.getBalance());
    balance = dollaSign + " " + balance1;
    ui->name->setText(current.name);
    ui->balance->setText(balance);
    ui->email->setText(current.getEmail());

    QString daMoney;

    QList<KedighCash> monay;
    for (int t = 0; t < current.cashOwned().size(); t++)
    {
        KedighCash toPush = current.cashOwned().at(t);
        monay.push_back(toPush);
    }

    ui->cashListView->clear();
    for (int q = 0; q < monay.size(); q++)
    {
        QString toAppend = "";
        toAppend += monay.at(q).m_string;
        ui->cashListView->addItem(toAppend);
    }//end for x.
}//update the display per selection.

void MainWindow::displayInfo2()
{
    int index;

    index = ui->fileDisplay->currentIndex().row();

    if (index == -1)
        index = 0;
    KedighKid current = kids.at(index);
    QString balance, balance1;
    QString dollaSign = "$";
    balance1.setNum(current.getBalance());
    balance = dollaSign + " " + balance1;
    ui->name->setText(current.name);
    ui->balance->setText(balance);
    ui->email->setText(current.getEmail());

    QString daMoney;

    QList<KedighCash> monay;
    for (int t = 0; t < current.cashOwned().size(); t++)
    {
        KedighCash toPush = current.cashOwned().at(t);
        monay.push_back(toPush);
    }

    ui->cashListView->clear();
    for (int q = 0; q < monay.size(); q++)
    {
        QString toAppend = "";
        toAppend += monay.at(q).m_string;
        daMoney += toAppend;
        ui->cashListView->addItem(toAppend);
    }//end for x.
}//update the display per selection.

MainWindow::~MainWindow()
{
    delete ui;
}

inline int toInt(QString s)
{
    return s.toInt();
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
        QString email, date;

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



            if (asList.at(z).contains("From"))
            {
                int i1, i2;
                QString current;
                current = asList.at(z);
                if (current.contains("["))
                {
                    i1 = current.indexOf("[");
                    current.remove(0, i1 + 1);
                    i2 = current.indexOf("]");
                    current.remove(i2, current.size() - 1);
                }//end if

                else
                {
                    i1 = current.indexOf("<");
                    current.remove(0, i1 + 1);
                    i2 = current.indexOf(">");
                    current.remove(i2, current.size() - 1);
                }

                email = current;
                z++;

                current = asList.at(z);

                i1 = current.indexOf(",");
                current.remove(0, i1 + 1);
                i2 = current.indexOf(":");
                current.remove(i2 - 2, current.size() - 1);
                date = current;
                z++;
            }

            else if (asList.at(z).contains("lastname: "))
            {
                QString lastname, period, version, denom,
                        serial, remote;
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

                if (asList.at(z).contains("period: "))
                {
                    toSize = "period: ";
                    current = asList.at(z);
                    current.remove(0, toSize.size() - 1);
                    period = current;
                    z++;
                }//make sure it exists!
                else
                    period = "invalid";

                if (asList.at(z).contains("version"))
                {
                    current = asList.at(z);
                    toSize = "version: ";
                    current.remove(0, toSize.size() - 1);
                    version = current;
                    z++;
                }

                if (asList.at(z).contains("denomination"))
                {
                    toSize = "denomination: ";
                    current = asList.at(z);
                    current.remove(0, toSize.size() - 1);
                    denom = current;
                    z++;
                }

                if (asList.at(z).contains("serial"))
                {
                    toSize = "serial: ";
                    current = asList.at(z);
                    current.remove(0, toSize.size() - 1);
                    serial = current;
                    z += 3; //go to remote line
                }

                if (asList.at(z).contains("REMOTE_ADDR"))
                {
                    toSize = "REMOTE_ADDR: ";
                    current = asList.at(z);
                    current.remove(0, toSize.size() - 1);
                    remote = current;
                }

                //get email

                bool exists = false;
                int index = -1;

                for (int k = 0; k < kids.size(); k++)
                {
                    if (QString::compare(kids.at(k).name, lastname, Qt::CaseInsensitive) == 0)
                    { exists = true; index = k; break; }
                }

                KedighKid toPush(lastname, period, email);

                if (!exists)
                {
                    kids.push_back(toPush);

                    KedighCash dolla(serial, date, lastname, remote, toInt(denom));
                    kids.last().addMoney(dolla);
                }//end if.

                else
                {
                    KedighCash dolla(serial, date, lastname, remote, toInt(denom));
                    kids.last().addMoney(dolla);
                }//end else
            }
        }

        for (int i = 0; i < kids.size(); i++)
        {
            ui->studentSelect->addItem(kids.at(i).name);
        }

        for (int y = 0; y < kids.size(); y++)
        {
            ui->fileDisplay->addItem(kids.at(y).name);
        }

        file.close();
    }//end if
}


void MainWindow::removeCash()
{
    int index = 0;
    int cash_index;
    cash_index = ui->cashListView->currentIndex().row();
    index = (int) ui->studentSelect->currentIndex();

    KedighKid current = kids.at(index);

    current.removeMoney(cash_index);
    kids.removeAt(index);
    kids.insert(index, current);
    displayInfo();
}

void MainWindow::findCash()
{
    QString searchInput;
    bool broke = false;
    searchInput = ui->serialInput->text();

    /** Now we bring up the second window
      to see where exactly the serial is found.
    **/

    for (int i = 0; i < kids.size(); i++)
    {
        KedighKid current = kids.at(i);
        for (int x = 0; x < current.cashOwned().size(); x++)
        {
            KedighCash toCheck = current.cashOwned().at(x);
            if (QString::compare(toCheck.getSerial(),  searchInput, Qt::CaseInsensitive) == 0)
            {
                //found the student!
                ui->studentSelect->setCurrentIndex(i);
                broke = true;
                break;
            }//end if

            if (broke)
                break;
        }//end for x.

        if (broke)
            break;
    }//end for i.
}//ya.

QString MainWindow::getList()
{
    QString toReturn;

    toReturn = "lastname \t period \t balance \n";

    for (int x = 0; x < kids.size(); x++)
    {
        KedighKid thisKid = kids.at(x);
        toReturn += thisKid.getSummary();
        toReturn += "\n";
    }//end for x.

    return toReturn;
}//get the file.

void MainWindow::exportTab()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt);;C++ Files (*.cpp *.h *.cc)"));

    if (fileName != "")
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not save file"));
            return;
        }//end if

        else
        {
            QTextStream stream(&file);
            stream << getList();
            stream.flush();
            file.close();
        }//end else
    }//end if
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt);; C++ Files (*.cpp *.h *.cc)"));
    parseFile(fileName);
}//open file dialog
