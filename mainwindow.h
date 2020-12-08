#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "tabwidget.h"
#include "formpart.h"
#include "models.h"
#include "formroute.h"
#include "formship.h"
#include "formwarehouse.h"
#include "formedtwire.h"
#include "checkform.h"
#include "formperepack.h"
#include "formpresence.h"
#include "formreport.h"
#include "olap/cubewidget.h"
#include "formpodt.h"
#include "formann.h"
#include "formedtpack.h"
#include "formedtline.h"
#include "formlbl.h"
#include "formnakl.h"
#include "formfixpodt.h"
#include "formpresencepodt.h"
#include "formnaklpodt.h"
#include "formreportpodt.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString key, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TabWidget *tabWidget;
    FormPart *formPart;
    FormRoute *formRoute;
    FormShip *formShip;
    FormWarehouse *formWarehouse;
    FormEdtWire *formEdtWire;
    CheckForm *checkForm;
    FormPerePack *formPerePack;
    FormPresence *formPresence;
    FormReport *formReport;
    CubeWidget *addCube;
    CubeWidget *namCube;
    CubeWidget *srcCube;
    CubeWidget *perepackCube;
    CubeWidget *podtCube;
    FormPodt *formPodt;
    FormAnn *formAnn;
    FormEdtPack *formPack;
    FormEdtLine *formLine;
    FormLbl *formLbl;
    FormNakl *formNakl;
    FormFixPodt *formFixPodt;
    FormPresencePodt *formPressencePodt;
    FormNaklPodt *formNaklPodt;
    FormReportPodt *formReportPodt;

    void loadSettings();
    void saveSettings();
};

#endif // MAINWINDOW_H
