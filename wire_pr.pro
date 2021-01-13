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

include(xlsx/qtxlsx.pri)

SOURCES += main.cpp\
    mainwindow.cpp \
    checkform.cpp \
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
    lblengine.cpp \
    modelpresence.cpp \
    models_edt.cpp \
    models.cpp \
    modelship.cpp \
    modelwarehouse.cpp \
    sertbuild.cpp \
    formann.cpp \
    formedtpack.cpp \
    formedtline.cpp \
    formlbl.cpp \
    dialogflt.cpp \
    formnakl.cpp \
    db/dbdelegate.cpp \
    db/dblogin.cpp \
    db/dbmapper.cpp \
    db/dbtablemodel.cpp \
    db/dbviewer.cpp \
    olap/axiswidget.cpp \
    olap/cubewidget.cpp \
    olap/cubic.cpp \
    olap/olapmodel.cpp \
    olap/tableview.cpp \
    modelstu.cpp \
    qr/qrencode.cpp \
    formfixpodt.cpp \
    formpresencepodt.cpp \
    formnaklpodt.cpp \
    formreportpodt.cpp \
    formmatr.cpp \
    formpodtnorn.cpp

HEADERS  += mainwindow.h \
    checkform.h \
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
    lblengine.h \
    modelpresence.h \
    models_edt.h \
    models.h \
    modelship.h \
    modelwarehouse.h \
    sertbuild.h \
    formann.h \
    formedtpack.h \
    formedtline.h \
    formlbl.h \
    dialogflt.h \
    formnakl.h \
    db/dbdelegate.h \
    db/dblogin.h \
    db/dbmapper.h \
    db/dbtablemodel.h \
    db/dbviewer.h \
    olap/axiswidget.h \
    olap/cubewidget.h \
    olap/cubic.h \
    olap/olapmodel.h \
    olap/tableview.h \
    modelstu.h \
    qr/qrencode.h \
    formfixpodt.h \
    formpresencepodt.h \
    formnaklpodt.h \
    formreportpodt.h \
    formmatr.h \
    formpodtnorn.h

FORMS    += mainwindow.ui \
    checkform.ui \
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
    formann.ui \
    formedtpack.ui \
    formedtline.ui \
    formlbl.ui \
    dialogflt.ui \
    formnakl.ui \
    db/dblogin.ui \
    olap/axiswidget.ui \
    olap/cubewidget.ui \
    formfixpodt.ui \
    formpresencepodt.ui \
    formnaklpodt.ui \
    formreportpodt.ui \
    formmatr.ui \
    formpodtnorn.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    runlbl.sh \
    images/otk.png \
    images/logo2.png \
    templates/glabels_szsm.xml
