#include "addCash.h"
#include "kedighkid.h"

CashWindow::CashWindow(QWidget *parent)
    : QWidget(parent)
{
    p_label = new QLabel(tr("Name:"));
    p_label2 = new QLabel(tr("Serial:"));
    p_label3 = new QLabel(tr("Denomination:"));
    p_label4 = new QLabel(tr("             "));
    closeButton = new QPushButton(tr("Close"));

    serialEdit = new QLineEdit();
    nameEdit = new QLineEdit();
    denomEdit = new QLineEdit();

    mainLayout = new QVBoxLayout();
    layout = new QHBoxLayout();
    layout2 = new QHBoxLayout();
    layout3 = new QHBoxLayout();
    layout4 = new QHBoxLayout();

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(serialEdit, SIGNAL(textEdited(QString)), this, SLOT(sendToMain()));

    layout->addWidget(p_label);
    layout->addWidget(nameEdit);

    layout2->addWidget(p_label2);
    layout2->addWidget(serialEdit);

    layout3->addWidget(p_label3);
    layout3->addWidget(denomEdit);

    layout4->addWidget(p_label4);
    layout4->addWidget(closeButton);

    mainLayout->addLayout(layout);
    mainLayout->addLayout(layout2);
    mainLayout->addLayout(layout3);
    mainLayout->addLayout(layout4);

    setLayout(mainLayout);

    setWindowTitle(tr("Preview"));

    QString _name = "";
    QString _period = "";
    QString _email = "";
}

void CashWindow::setWindowFlags(Qt::WindowFlags flags)
{
    QWidget::setWindowFlags(flags);

    QString text;

    Qt::WindowFlags type = (flags & Qt::WindowType_Mask);
    if (type == Qt::Window) {
        text = "Qt::Window";
    } else if (type == Qt::Dialog) {
        text = "Qt::Dialog";
    } else if (type == Qt::Sheet) {
        text = "Qt::Sheet";
    } else if (type == Qt::Drawer) {
        text = "Qt::Drawer";
    } else if (type == Qt::Popup) {
        text = "Qt::Popup";
    } else if (type == Qt::Tool) {
        text = "Qt::Tool";
    } else if (type == Qt::ToolTip) {
        text = "Qt::ToolTip";
    } else if (type == Qt::SplashScreen) {
        text = "Qt::SplashScreen";
    }

    if (flags & Qt::MSWindowsFixedSizeDialogHint)
        text += "\n| Qt::MSWindowsFixedSizeDialogHint";
    if (flags & Qt::X11BypassWindowManagerHint)
        text += "\n| Qt::X11BypassWindowManagerHint";
    if (flags & Qt::FramelessWindowHint)
        text += "\n| Qt::FramelessWindowHint";
    if (flags & Qt::WindowTitleHint)
        text += "\n| Qt::WindowTitleHint";
    if (flags & Qt::WindowSystemMenuHint)
        text += "\n| Qt::WindowSystemMenuHint";
    if (flags & Qt::WindowMinimizeButtonHint)
        text += "\n| Qt::WindowMinimizeButtonHint";
    if (flags & Qt::WindowMaximizeButtonHint)
        text += "\n| Qt::WindowMaximizeButtonHint";
    if (flags & Qt::WindowCloseButtonHint)
        text += "\n| Qt::WindowCloseButtonHint";
    if (flags & Qt::WindowContextHelpButtonHint)
        text += "\n| Qt::WindowContextHelpButtonHint";
    if (flags & Qt::WindowShadeButtonHint)
        text += "\n| Qt::WindowShadeButtonHint";
    if (flags & Qt::WindowStaysOnTopHint)
        text += "\n| Qt::WindowStaysOnTopHint";
    if (flags & Qt::CustomizeWindowHint)
        text += "\n| Qt::CustomizeWindowHint";
}

KedighKid CashWindow::getLastKid()
{
    name = nameEdit->text();
    period = "4";
    email = "student@brentwoodHigh.com";

    KedighKid toReturn(name, period, email);
    return toReturn;
}

void CashWindow::sendToMain()
{

}

void CashWindow::close()
{
    name = nameEdit->text();
    period = "2";
    email = "student@brentwoodHigh.com";
    serial = serialEdit->text();
    denom = denomEdit->text();

    Q_EMIT newKid();

    nameEdit->setText("");
    serialEdit->setText("");
    denomEdit->setText("");

    this->hide();
}
