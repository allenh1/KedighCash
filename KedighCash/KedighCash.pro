#-------------------------------------------------
#
# Project created by QtCreator 2012-08-20T09:01:43
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = KedighCash
TEMPLATE = app

#LIBS += -L $$(QMF_LIBDIR) -lqtopiamail

SOURCES += main.cpp\
        mainwindow.cpp \
    KedighCash.cc \
    kedighkid.cc \
    addCash.cc \
    smtp.cpp \
    MailThread.cpp \
    userWindow.cpp

HEADERS  += mainwindow.h \
    KedighCash.h \
    kedighkid.h \
    addCash.h \
    smtp.h \
    MailThread.h \
    userWindow.h

FORMS    += mainwindow.ui
