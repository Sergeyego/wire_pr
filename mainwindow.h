#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "tabwidget.h"
#include "formpart.h"
#include "models.h"
#include "formwire.h"
#include "formroute.h"
#include "formship.h"
#include "formwarehouse.h"
#include "formedtwire.h"
#include "checkform.h"
#include "formperepack.h"
#include "formpresence.h"
#include "formreport.h"
#include "cubewidget.h"
#include "formpodt.h"
#include "formann.h"
#include "formedtpack.h"
#include "formedtline.h"
#include "formlbl.h"
#include "formnakl.h"

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
    FormWire *formWire;
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

    void loadSettings();
    void saveSettings();
};

#endif // MAINWINDOW_H
