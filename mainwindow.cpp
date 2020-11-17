#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString key, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tabWidget = new TabWidget(this);
    this->setCentralWidget(tabWidget);

    formPart = new FormPart(key==QString("srt"));
    formRoute = new FormRoute();
    formShip = new FormShip();
    formWarehouse = new FormWarehouse();
    formEdtWire = new FormEdtWire();
    checkForm = new CheckForm();
    formPerePack = new FormPerePack();
    formPresence = new FormPresence();
    formReport = new FormReport();
    formPodt = new FormPodt();
    formAnn = new FormAnn();
    formPack = new FormEdtPack();
    formLine = new FormEdtLine();
    formLbl = new FormLbl();
    formNakl = new FormNakl();

    addCube = new CubeWidget(25);
    namCube = new CubeWidget(24);
    srcCube = new CubeWidget(26);
    perepackCube = new CubeWidget(27);
    podtCube = new CubeWidget(28);

    loadSettings();

    tabWidget->addTabAction(formPart,ui->actionPart);
    tabWidget->addTabAction(formRoute,ui->actionRoute);
    tabWidget->addTabAction(formShip,ui->actionShip);
    tabWidget->addTabAction(formWarehouse,ui->actionWarehouse);
    tabWidget->addTabAction(formEdtWire,ui->actionEdtWire);
    tabWidget->addTabAction(checkForm,ui->actionCod);
    tabWidget->addTabAction(formPerePack,ui->actionPerepack);
    tabWidget->addTabAction(formPresence,ui->actionPrecense);
    tabWidget->addTabAction(formReport,ui->actionPeport);
    tabWidget->addTabAction(formPodt,ui->actionPodt);
    tabWidget->addTabAction(addCube,ui->actionAdd);
    tabWidget->addTabAction(namCube,ui->actionAnSpool);
    tabWidget->addTabAction(srcCube,ui->actionSrc);
    tabWidget->addTabAction(formAnn,ui->actionAnn);
    tabWidget->addTabAction(formPack,ui->actionSpool);
    tabWidget->addTabAction(formLine,ui->actionLine);
    tabWidget->addTabAction(formLbl,ui->actionLbl);
    tabWidget->addTabAction(perepackCube,ui->actionPerepackCube);
    tabWidget->addTabAction(formNakl,ui->actionNakl);
    tabWidget->addTabAction(podtCube,ui->actionCubePodt);
    tabWidget->loadSettings();

    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->actionRefresh,SIGNAL(triggered(bool)),Models::instance(),SLOT(refresh()));
}

MainWindow::~MainWindow()
{
    tabWidget->saveSettings();
    saveSettings();
    delete formWarehouse;
    delete formShip;
    delete formRoute;
    delete formPart;
    delete formEdtWire;
    delete checkForm;
    delete formPerePack;
    delete formPresence;
    delete formReport;
    delete addCube;
    delete formPodt;
    delete namCube;
    delete srcCube;
    delete formAnn;
    delete formPack;
    delete formLine;
    delete formLbl;
    delete perepackCube;
    delete formNakl;
    delete Models::instance();
    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    this->restoreGeometry(settings.value("main_geometry").toByteArray());
    this->restoreState(settings.value("main_state").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    settings.setValue("main_state", this->saveState());
    settings.setValue("main_geometry", this->saveGeometry());
}
