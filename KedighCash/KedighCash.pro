#-------------------------------------------------
#
# Project created by QtCreator 2012-08-20T09:01:43
#
#-------------------------------------------------

QT       += core gui network

TARGET = KedighCash
TEMPLATE = app

LIBS += -L $$(QMF_LIBDIR) -lqtopiamail

SOURCES += main.cpp\
        mainwindow.cpp \
    KedighCash.cc \
    kedighkid.cc \
    addCash.cc \
    smtp.cpp

HEADERS  += mainwindow.h \
    KedighCash.h \
    kedighkid.h \
    addCash.h \
    smtp.h

FORMS    += mainwindow.ui
