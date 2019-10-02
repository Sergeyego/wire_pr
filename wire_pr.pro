#-------------------------------------------------
#
# Project created by QtCreator 2017-01-11T10:41:42
#
#-------------------------------------------------

QT       += core gui sql xml printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wire_pr
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    tabwidget.cpp \
    axiswidget.cpp \
    cbrelationdelegate.cpp \
    checkform.cpp \
    cubewidget.cpp \
    cubic.cpp \
    dbtablemodel.cpp \
    dialogotk.cpp \
    dialogpack.cpp \
    drawing.cpp \
    editor.cpp \
    formedtwire.cpp \
    formpart.cpp \
    formperepack.cpp \
    formpodt.cpp \
    formpresence.cpp \
    formreport.cpp \
    formroute.cpp \
    formship.cpp \
    formwarehouse.cpp \
    formwire.cpp \
    lblengine.cpp \
    modelpresence.cpp \
    models_edt.cpp \
    models.cpp \
    modelship.cpp \
    modelwarehouse.cpp \
    olapmodel.cpp \
    pgdialog.cpp \
    pushform.cpp \
    sertbuild.cpp \
    tableview.cpp \
    viewer.cpp \
    formann.cpp \
    formedtpack.cpp \
    formedtline.cpp \
    formlbl.cpp \
    dialogflt.cpp \
    formnakl.cpp

HEADERS  += mainwindow.h \
    tabwidget.h \
    axiswidget.h \
    cbrelationdelegate.h \
    checkform.h \
    cubewidget.h \
    cubic.h \
    dbtablemodel.h \
    dialogotk.h \
    dialogpack.h \
    drawing.h \
    editor.h \
    formedtwire.h \
    formpart.h \
    formperepack.h \
    formpodt.h \
    formpresence.h \
    formreport.h \
    formroute.h \
    formship.h \
    formwarehouse.h \
    formwire.h \
    lblengine.h \
    modelpresence.h \
    models_edt.h \
    models.h \
    modelship.h \
    modelwarehouse.h \
    olapmodel.h \
    pgdialog.h \
    pushform.h \
    sertbuild.h \
    tableview.h \
    viewer.h \
    formann.h \
    formedtpack.h \
    formedtline.h \
    formlbl.h \
    dialogflt.h \
    formnakl.h

FORMS    += mainwindow.ui \
    axiswidget.ui \
    checkform.ui \
    cubewidget.ui \
    dialogotk.ui \
    dialogpack.ui \
    editor.ui \
    formedtwire.ui \
    formpart.ui \
    formperepack.ui \
    formpodt.ui \
    formpresence.ui \
    formreport.ui \
    formroute.ui \
    formship.ui \
    formwarehouse.ui \
    formwire.ui \
    pgdialog.ui \
    pushform.ui \
    formann.ui \
    formedtpack.ui \
    formedtline.ui \
    formlbl.ui \
    dialogflt.ui \
    formnakl.ui

RESOURCES += \
    res.qrc
LIBS += -lqrencode \
        -lxls
INCLUDEPATH+=/usr/include/xlslib
