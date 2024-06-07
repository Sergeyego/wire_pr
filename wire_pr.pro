#-------------------------------------------------
#
# Project created by QtCreator 2017-01-11T10:41:42
#
#-------------------------------------------------

QT       += core gui sql xml printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wire_pr
TEMPLATE = app

RC_FILE = ico.rc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(xlsx/qtxlsx.pri)

SOURCES += main.cpp\
    checkform.cpp \
    db/dbcombobox.cpp \
    db/dbdateedit.cpp \
    db/dbdelegate.cpp \
    db/dblogin.cpp \
    db/dbmapper.cpp \
    db/dbrelationeditdialog.cpp \
    db/dbtablemodel.cpp \
    db/dbviewer.cpp \
    db/dbxlsx.cpp \
    db/executor.cpp \
    db/modelro.cpp \
    db/tablemodel.cpp \
    dialogotk.cpp \
    dialogpack.cpp \
    editor.cpp \
    formedtline.cpp \
    formedtpack.cpp \
    formedtwire.cpp \
    formfixpodt.cpp \
    formlbl.cpp \
    formmatr.cpp \
    formmovwire.cpp \
    formnakl.cpp \
    formnaklpodt.cpp \
    formnorm.cpp \
    formpart.cpp \
    formperepack.cpp \
    formpodt.cpp \
    formpodtnorm.cpp \
    formpresence.cpp \
    formpresencepodt.cpp \
    formreport.cpp \
    formreportpodt.cpp \
    formship.cpp \
    invoicemanager.cpp \
    lblengine.cpp \
    mainwindow.cpp \
    modelstu.cpp \
    olap/axiswidget.cpp \
    olap/cubewidget.cpp \
    olap/cubic.cpp \
    olap/dialogolapflt.cpp \
    olap/formfilter.cpp \
    olap/olapmodel.cpp \
    olap/tableview.cpp \
    progressexecutor.cpp \
    progressreportdialog.cpp \
    rels.cpp \
    sertbuild.cpp

HEADERS  += mainwindow.h \
    checkform.h \
    db/dbcombobox.h \
    db/dbdateedit.h \
    db/dbdelegate.h \
    db/dblogin.h \
    db/dbmapper.h \
    db/dbrelationeditdialog.h \
    db/dbtablemodel.h \
    db/dbviewer.h \
    db/dbxlsx.h \
    db/executor.h \
    db/modelro.h \
    db/tablemodel.h \
    dialogotk.h \
    dialogpack.h \
    editor.h \
    formedtline.h \
    formedtpack.h \
    formedtwire.h \
    formfixpodt.h \
    formlbl.h \
    formmatr.h \
    formmovwire.h \
    formnakl.h \
    formnaklpodt.h \
    formnorm.h \
    formpart.h \
    formperepack.h \
    formpodt.h \
    formpodtnorm.h \
    formpresence.h \
    formpresencepodt.h \
    formreport.h \
    formreportpodt.h \
    formship.h \
    invoicemanager.h \
    lblengine.h \
    modelstu.h \
    olap/axiswidget.h \
    olap/cubewidget.h \
    olap/cubic.h \
    olap/dialogolapflt.h \
    olap/formfilter.h \
    olap/olapmodel.h \
    olap/tableview.h \
    progressexecutor.h \
    progressreportdialog.h \
    rels.h \
    sertbuild.h

FORMS    += mainwindow.ui \
    checkform.ui \
    db/dblogin.ui \
    db/dbrelationeditdialog.ui \
    dialogotk.ui \
    dialogpack.ui \
    editor.ui \
    formedtline.ui \
    formedtpack.ui \
    formedtwire.ui \
    formfixpodt.ui \
    formlbl.ui \
    formmatr.ui \
    formmovwire.ui \
    formnakl.ui \
    formnaklpodt.ui \
    formnorm.ui \
    formpart.ui \
    formperepack.ui \
    formpodt.ui \
    formpodtnorm.ui \
    formpresence.ui \
    formpresencepodt.ui \
    formreport.ui \
    formreportpodt.ui \
    formship.ui \
    olap/axiswidget.ui \
    olap/cubewidget.ui \
    olap/dialogolapflt.ui \
    olap/formfilter.ui \
    progressreportdialog.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    ico.rc \
    images/otk.png \
    images/logo2.png \
    images/wire.ico \
    templates/glabels_szsm.xml \
    ico.rc
