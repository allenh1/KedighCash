#include "userWindow.h"
#include "kedighkid.h"

UserWindow::UserWindow(QWidget *parent, bool existingFile)
    : QWidget(parent)
{
    bool m_createOnly = !existingFile;

    p_label = new QLabel(tr("User:"));
    p_label2 = new QLabel(tr("Password:"));

    loginButton = new QPushButton(tr("Login"));

    passwordEdit = new QLineEdit();
    nameEdit = new QLineEdit();

    mainLayout = new QVBoxLayout();
    layout = new QHBoxLayout();
    layout2 = new QHBoxLayout();

    connect(loginButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(passwordEdit, SIGNAL(textEdited(QString)), this, SLOT(sendToMain()));

    layout->addWidget(p_label);
    layout->addWidget(nameEdit);

    layout2->addWidget(p_label2);
    layout2->addWidget(passwordEdit);


    mainLayout->addLayout(layout);
    mainLayout->addLayout(layout2);
    mainLayout->addWidget(loginButton);

    setLayout(mainLayout);

    setWindowTitle(tr("Login Window"));

    QString _name = "";
    QString _period = "";
    QString _email = "";
}

void UserWindow::setWindowFlags(Qt::WindowFlags flags)
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


void UserWindow::close()
{
    m_userName = nameEdit->text();
    m_password = passwordEdit->text();

    if (!m_createOnly)
        Q_EMIT unlockCash(m_userName,m_password);
    else
        Q_EMIT createAccount(m_userName,m_password);

    nameEdit->setText("");
    passwordEdit->setText("");

    this->hide();
}

