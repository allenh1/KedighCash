#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QCryptographicHash>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_loggedIn(false)
{
    ui->setupUi(this);
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->email, SIGNAL(textEdited(QString)), this, SLOT(updateEmail()));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addCash()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(exit()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->killKid, SIGNAL(clicked()), this, SLOT(killKid()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveData()));
    connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(findCash()));
    connect(ui->fileDisplay, SIGNAL(itemSelectionChanged()), this, SLOT(displayInfo2()));
    connect(ui->actionExport, SIGNAL(triggered()), this, SLOT(exportTab()));
    connect(ui->studentSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(displayInfo()));
    connect(ui->deleteCurrency, SIGNAL(clicked()), this, SLOT(removeCash()));
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(login()));


    ui->actionAbout->setIcon(QIcon("questionface.xpm"));
    setWindowIcon(QIcon("btemp.xpm"));

    checkForSaveFile();
    checkForPasswordFile();

    addCashWindow = new CashWindow();
    addCashWindow->setWindowIcon(QIcon("btemp.xpm"));
    addCashWindow->setWindowTitle("Add Cash");

    connect(addCashWindow, SIGNAL(newKid()), this , SLOT(addCashFromWindow()));

    loginWindow = new UserWindow(0, true);
    loginWindow->setWindowIcon(QIcon("btemp.xpm"));

    connect(loginWindow, SIGNAL(unlockCash(QString,QString)), this, SLOT(tryUnlock(QString,QString)));
    //connect(loginWindow, SIGNAL(createAccount(QString,QString)), this, SLOT(makeAccount(QString,QString)));
}

void MainWindow::addCash()
{
    addCashWindow->show();
}

void MainWindow::login()
{
    loginWindow->show();
}//end login

void MainWindow::tryUnlock(QString _user, QString _password)
{
    QString encryptedPass;
    QString encryptedUser;

    encryptedUser = QString(QCryptographicHash::hash((_user.toStdString().c_str()), QCryptographicHash::Md5).toHex());
    encryptedPass = QString(QCryptographicHash::hash((_password.toStdString().c_str()),QCryptographicHash::Md5).toHex());

    if (encryptedUser == m_username && encryptedPass == m_password)
    {
        m_loggedIn = true;
    }//end if

    else
    {
        m_loggedIn = false;
        QMessageBox messageBox;
        messageBox.setWindowIcon(QIcon("questionface.xpm"));
        messageBox.setWindowTitle(tr("Login Failed"));
        messageBox.setText(tr("ERROR: Invalid Login! You cannot modify cash without a valid login!"));
        messageBox.setStandardButtons(QMessageBox::Ok);

        if (messageBox.exec() == QMessageBox::Ok)
            messageBox.close();
    }//invalid login window
}//end void

void MainWindow::makeAccount(QString _user, QString _password)
{
    m_loggedIn = true;
    m_username = _user;
    m_password = _password;
    QString encryptedPass;
    QString encryptedUser;

    encryptedUser = QString(QCryptographicHash::hash((_user.toStdString().c_str()), QCryptographicHash::Md5).toHex());
    encryptedPass = QString(QCryptographicHash::hash((_password.toStdString().c_str()),QCryptographicHash::Md5).toHex());

    QString output;
    output = "User: " + encryptedUser + "\n";
    output += "Password: " + encryptedPass;

    QFile file("password.txt");
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, tr("Error"), tr("Could not save file"));
        return;
    }//end if

    else
    {
        QTextStream stream(&file);
        stream << output;
        stream.flush();
        file.close();
    }//end else

}//create account

void MainWindow::addCashFromWindow()
{
    if (!m_loggedIn)
        return;

    QString serial = addCashWindow->serial;
    QString date = "November 12, 2012";
    int denom = addCashWindow->denom.toInt();
    int index = -1;

    for (int x = 0; x < kids.size(); x++)
    {
        KedighKid current = kids.at(x);

        if (current.name == addCashWindow->name)
        {
            index = x;
            break;
        }
    }

    if (index == -1)
    {
        KedighKid toPush(addCashWindow->name, addCashWindow->period, addCashWindow->email);
        KedighCash test(serial, date, denom);
        toPush.addMoney(test);
        if (toPush.name != "")
            kids.push_back(toPush);
    }//end if

    else
    {
        KedighCash test(serial, date, denom);
        KedighKid current = kids.at(index);
        current.addMoney(test);
        kids.replace(index, current);
    }

    ui->studentSelect->clear();
    ui->fileDisplay->clear();

    qSort(kids.begin(), kids.end());
    for (int i = 0; i < kids.size(); i++)
    {
        ui->studentSelect->addItem(kids.at(i).name);
    }

    for (int y = 0; y < kids.size(); y++)
    {
        ui->fileDisplay->addItem(kids.at(y).name);
    }

    displayInfo();
    countCash();

    /** Smtp *newMail  = new Smtp("from@address.com","to@address.com"," Your Subject","My body text");
        delete newMail; **/

    QStringList receivers;
    receivers.push_back(toAddress);
    //Smtp(server, user, pass, from, to, subject, body);

	QString messageBody; 
	messageBody += "KedighCash Deposit: \n";
	messageBody += "Lastname: " + addCashWindow->name + "\n"; 
	messageBody += "Serial: " + addCashWindow->serial + "\n"; 
    messageBody += "Denomination: $" + addCashWindow->denom;
	
    //Smtp *newMail = new Smtp("smtp.mail.yahoo.com", "kedighcash@yahoo.com", fromPass, fromAddress, receivers, "Test", messageBody);
 
    MailThread sendMessage(fromAddress, toAddress, fromPass, "New Cash Deposit", messageBody);
    addCashWindow->hide();
    sendMessage.run();
}

void MainWindow::killKid()
{
    if (!m_loggedIn)
        return;

    int index = ui->studentSelect->currentIndex();

    if (index == -1)
        index = 0;

    kids.removeAt(index);
    ui->studentSelect->clear();
    ui->fileDisplay->clear();

    qSort(kids.begin(), kids.end());
    for (int i = 0; i < kids.size(); i++)
    {
        ui->studentSelect->addItem(kids.at(i).name);
    }

    for (int y = 0; y < kids.size(); y++)
    {
        ui->fileDisplay->addItem(kids.at(y).name);
    }

    ui->studentSelect->setCurrentIndex(index);

    countCash();
}//remove kid.

void MainWindow::updateEmail()
{
    int index = ui->studentSelect->currentIndex();

    KedighKid current = kids.at(index);
    current.setMail(ui->email->text());

    kids.replace(index, current);

    countCash();
}

void MainWindow::exit()
{
    QMessageBox messageBox;
    messageBox.setWindowIcon(QIcon("questionface.xpm"));
    messageBox.setWindowTitle(tr("Exit"));
    messageBox.setText(tr("Do you really want to quit?"));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);

    if (messageBox.exec() == QMessageBox::Yes)
        qApp->quit();
}//end void

void MainWindow::about()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("About"));
    messageBox.setText(tr("\t KedighCash \n \t Version: 1.0 \n            Author: Hunter Allen \n Email: hutner.allen@Vanderbilt.edu"));
    messageBox.setWindowIcon(QIcon("questionface.xpm"));
    messageBox.exec();
}

void MainWindow::checkForDoubles()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("WARNING: Doubles Detected!"));

    for (int x = 0; x < cashList.size(); x++)
    {
        for (int y = 0; y < cashList.size(); y++)
        {
            if (cashList.at(x) == cashList.at(y))
            {
                KedighCash one = cashList.at(x);
                KedighCash two = cashList.at(y);
                QString ownerOne = one.getOwner();
                QString ownerTwo = two.getOwner();


                messageBox.setText(tr("Doubles: "));
                messageBox.exec();
            }//end if
        }//end for y
    }//end for
}//end void

void MainWindow::displayInfo()
{
    int index;
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
    checkForDoubles();
}//update the display per selection.

void MainWindow::displayInfo2()
{
    int index;

    index = ui->fileDisplay->currentIndex().row();
    ui->studentSelect->setCurrentIndex(index);

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
                int index1, index2;
                QString current;
                current = asList.at(z);
                if (current.contains("["))
                {
                    index1 = current.indexOf("[");
                    current.remove(0, index1 + 1);
                    index2 = current.indexOf("]");
                    current.remove(index2, current.size() - 1);
                }//end if

                else
                {
                    index1 = current.indexOf("<");
                    current.remove(0, index1 + 1);
                    index2 = current.indexOf(">");
                    current.remove(index2, current.size() - 1);
                }

                email = current;
                z++;

                current = asList.at(z);

                index1 = current.indexOf(",");
                current.remove(0, index1 + 1);
                index2 = current.indexOf(":");
                current.remove(index2 - 2, current.size() - 1);
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
                current.remove(0, theSize);
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

            else if (asList.at(z).contains("denomination"))
            {
                QString lastname, period, version, denom,
                        serial, remote;
                //we have a section to extract from.
                QString toSize; 
                QString current;
                current = asList.at(z);
                int theSize = 0;
                toSize = "denomination: ";
                current = asList.at(z); 
                current.remove(0, toSize.size() - 1);
                denom = current;
                z++; 


                if (asList.at(z).contains("serial"))
                {
                    toSize = "serial: ";
                    current = asList.at(z);
                    current.remove(0, toSize.size() - 1);
                    serial = current;
                    z ++; //go to remote line
                }

                if (asList.at(z).contains("period"))
                {
                    toSize = "period: ";
                    current = asList.at(z);
                    current.remove(0, toSize.size() - 1);
                    period = current;
                    z++;
                }//make sure it exists!
                else
                    period = "invalid";


                if (asList.at(z).contains("lastname"))
                {
                    current = asList.at(z);
                    toSize = "lastname: ";
                    theSize = toSize.size();
                    current.remove(0, theSize);
                    lastname = current;
                    z += 3; //we can go to the next line now
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
            }//end else if.
        }

        qSort(kids.begin(), kids.end());
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

    countCash();
}


void MainWindow::countCash()
{
    int sum = 0;
    const QList<KedighKid> theKids = kids;

    for (int x = 0; x < theKids.size(); x++)
    {
        KedighKid daKid = theKids.at(x);
        QList<KedighCash> current = daKid.cashOwned();

        for (int y = 0; y < current.size(); y++)
        {
            KedighCash cash = current.at(y);
            sum += cash.getValue();
        }//end for y.
    }//loop through and count cash.

    QString total;
    total.setNum(sum);
    ui->totalMoney->setText("$ "+total);
    save();
}//end void.

void MainWindow::removeCash()
{
    if (!m_loggedIn)
        return;

    int index = 0;
    int cash_index;
    cash_index = ui->cashListView->currentIndex().row();
    index = (int) ui->studentSelect->currentIndex();

    KedighKid current = kids.at(index);

    current.removeMoney(cash_index);
    kids.removeAt(index);
    kids.insert(index, current);
    displayInfo();
    countCash();
    checkForDoubles();
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

                displayInfo();
                countCash();
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

bool caseInsensitiveLessThan(const KedighCash &s1, const KedighCash &s2)
{
    KedighCash k1 = s1;
    KedighCash k2 = s2;
    return k1 < k2;
}//case-insensitive alphabetical sort.

void MainWindow::sortKids()
{
    cashList.clear();

    for (int x = 0; x < kids.size(); x++)
    {
        KedighKid current = kids.at(x);
        QList<KedighCash> monay = current.cashOwned();

        for (int y = 0; y < monay.size(); y++)
        {
            QString toPush = "";
            KedighCash monaymonay = monay.at(y);
            cashList.push_back(monaymonay);
        }//end for y.
    }

    qSort(cashList.begin(), cashList.end());
}

QString MainWindow::dataOutput()
{
    sortKids();
    QString toReturn = "";
    //serial    denom   owner
    for (int x = 0; x < cashList.size(); x++)
    {
        QList<KedighCash> monay = cashList;

        for (int y = 0; y < monay.size(); y++)
        {
            KedighCash monaymonay = monay.at(y);
            QString fromNum;
            fromNum.setNum(monaymonay.getValue());
            toReturn += monaymonay.getSerial() +"\t"
                    + fromNum +"\t"
                    + monaymonay.getOwner() + "\n";
        }//end for y.
    }//end for x.

    return toReturn;
}

void MainWindow::saveData()
{
    //serial    denom   owner
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
            stream << dataOutput();
            stream.flush();
            file.close();
        }//end else
    }//end if
}

void MainWindow::exportTab()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt)"));

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

inline QString reverse(QString in)
{
    QString toReturn = "";
    for (int x = in.size() - 1; x >= 0; x--)
        toReturn += in[x];

    return toReturn;
}//flip a string.

void MainWindow::save()
{
    QString output = "";

    /**
      Save format:

      lastname
        email
        period
        balance
        remote
            currency 1
            currency 2
            currency 3
            ...
      emantsal
      **/

    for (int x = 0; x < kids.size(); x++)
    {
        KedighKid current = kids.at(x);
        output += current.name;
        output += "\n\tEmail: " + current.getEmail();
        output += "\n\tPeriod: " + current.getPeriod();
        QString asString;
        asString.setNum(current.getBalance());
        output += "\n\tBalance: $" + asString;

        for (int y = 0; y < current.cashOwned().size(); y++)
        {
            KedighCash currentCash = current.cashOwned().at(y);
            output += "\n\t\t" + currentCash.getSerial() + ": $";
            QString val;
            val.setNum(currentCash.getValue());
            output += val;
        }//end for y.

        output += "\n" + reverse(current.name);
        output += "\n\n";
    }

    QFile file("autosave.dat");
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, tr("Error"), tr("Could not save file"));
        return;
    }//end if

    else
    {
        QTextStream stream(&file);
        stream << output;
        stream.flush();
        file.close();
    }//end else
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt);; C++ Files (*.cpp *.h *.cc)"));
    parseFile(fileName);
}//open file dialog

void MainWindow::checkForSaveFile()
{
    QFile file2("autosave.dat");
    QString fileInput = "autosave.dat";

    if (!file2.exists())
        QString fileInput = QFileDialog::getSaveFileName(this, tr("Autosave File"), "", tr("Data File (*.dat)"));

    QList<QString> asList;

    if (fileInput != "")
    {
        /**
          Load the last save into memory.

          Allows us to extract the serials/class members.
          **/

        QFile file(fileInput);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }//end if
        QTextStream in(&file);


        while (!in.atEnd())
        {
            asList.push_back(in.readLine());
        }//iterate through the file. All of it. Store.
    }//end if.

    for (int x = 0; x < asList.size(); x++)
    {
        /**
          Now, we segment the file.
          **/
        QString current = asList.at(x);
        QString flipped = reverse(current);
        QList<QString> currentKid;

        int y = x;

        while (asList.at(y) != flipped)
        {
            y++;
        }

        for (int z = x; z < y; z++)
        {
            currentKid.push_back(asList.at(z));
            x++;
        }//end for z.

        if (true)
        {
            QString last = currentKid.at(0);
            QString email = currentKid.at(1);
            int index = email.indexOf(":") + 1;
            email.remove(0, index);
            QString period = currentKid.at(2);
            index = period.indexOf(":") + 1;
            period.remove(0, index);
            QString balance = currentKid.at(3);

            KedighKid toPush(last, period, email);


            for (int i = 4; i < currentKid.size(); i++)
            {
                QString line = currentKid.at(i);
                QString tabLength = "\t\t";
                line.remove(0, tabLength.length());
                int index = line.indexOf(":");
                QString serial = line;
                serial.remove(index, line.size());
                QString denom = line;
                denom.remove(0, serial.size());
                QString toSize = " $";
                denom.remove(0, 3);
                int _denomination = denom.toInt();

                KedighCash money(serial, "Today", _denomination);
                toPush.addMoney(money);
            }
            kids.push_back(toPush);
            x++;
        }
    }

    qSort(kids.begin(), kids.end());
    for (int i = 0; i < kids.size(); i++)
    {
        ui->studentSelect->addItem(kids.at(i).name);
    }

    for (int y = 0; y < kids.size(); y++)
    {
        ui->fileDisplay->addItem(kids.at(y).name);
    }

    countCash();
    displayInfo();
}//open last save.

void MainWindow::checkForPasswordFile()
{
    QString fileInput = "password.txt";

    QList<QString> asList;

    QFile file(fileInput);

    if (!file.exists())
    {
        UserWindow * newUser = new UserWindow(0, false);
        connect(newUser, SIGNAL(createAccount(QString,QString)), this, SLOT(makeAccount(QString,QString)));
        newUser->show();
    }//first run, or file cannot be found.

    else if (fileInput != "")
    {
        /**
          Load the password into memory.

          **/

        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }//end if
        QTextStream in(&file);


        while (!in.atEnd())
        {
            asList.push_back(in.readLine());
        }//iterate through the file. All of it. Store.

    if (asList.size() > 0)
    {
        QString current = asList.at(0);
        int index1 = current.indexOf("User:");
        current.remove(0, 6);
        m_username = current;
        current = asList.at(1);
        //"password: "
        current.remove(0, 10);
        m_password = current;
    }//end if

    countCash();
    displayInfo();
    }//open last save.
}
