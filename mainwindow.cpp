#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString k, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    key=k;

    actAction(ui->actionEdtWire,&MainWindow::edtWire);
    actAction(ui->actionSpool,&MainWindow::edtSpool);
    actAction(ui->actionLine,&MainWindow::edtLine);
    actAction(ui->actionMatr,&MainWindow::edtMatr);

    actAction(ui->actionPodt,&MainWindow::podtPart);
    actAction(ui->actionPrecensePodt,&MainWindow::podtPresence);
    actAction(ui->actionReportPodt,&MainWindow::podtReport);
    actAction(ui->actionFixPodt,&MainWindow::podtFix);
    actAction(ui->actionPodtNakl,&MainWindow::podtNakl);
    actAction(ui->actionPodtNorm,&MainWindow::podtNorm);

    actAction(ui->actionPart,&MainWindow::wirePart);
    actAction(ui->actionLbl,&MainWindow::wireLbl);

    actAction(ui->actionPerepack,&MainWindow::perepack);
    actAction(ui->actionPrecense,&MainWindow::presence);
    actAction(ui->actionReport,&MainWindow::report);
    actAction(ui->actionWarehouse,&MainWindow::warehouse);
    actAction(ui->actionNakl,&MainWindow::nakl);
    actAction(ui->actionNorm,&MainWindow::norm);
    actAction(ui->actionFix,&MainWindow::fix);

    actAction(ui->actionCubeSpool,&MainWindow::cubeNam);
    actAction(ui->actionCubePack,&MainWindow::cubePack);
    actAction(ui->actionCubeAdd,&MainWindow::cubePost);
    actAction(ui->actionCubeSrc,&MainWindow::cubeRas);
    actAction(ui->actionCubePerepack,&MainWindow::cubePerepack);
    actAction(ui->actionCubePodt,&MainWindow::cubePodt);
    actAction(ui->actionCubePodtOut,&MainWindow::cubePodtOut);

    loadSettings();

    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));

    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(close()));
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete Rels::instance();
    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings("szsm", "wire_pr"); 
    this->restoreState(settings.value("main_state").toByteArray());
    QString opentab=settings.value("main_opentab").toString();
    QString current=settings.value("main_currenttab").toString();

    if (opentab.isEmpty()){
        ui->actionPart->trigger();
    } else {
        QStringList l=opentab.split("|");
        foreach (QString a, l) {
            if (actions.contains(a)){
                actions.value(a)->trigger();
            }
        }
    }
    setActiveSubWindow(current);
    this->restoreGeometry(settings.value("main_geometry").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    settings.setValue("main_state", this->saveState());
    settings.setValue("main_geometry", this->saveGeometry());
    QString opentab, currenttab;
    for (int i=0; i<ui->tabWidget->count(); i++){
        if (!opentab.isEmpty()){
            opentab+="|";
        }
        opentab+=ui->tabWidget->tabText(i);
    }
    currenttab=ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    settings.setValue("main_opentab", opentab);
    settings.setValue("main_currenttab",currenttab);
}

bool MainWindow::exist(QObject *a)
{
    bool b=false;
    QAction *action = qobject_cast<QAction *>(a);
    if (action){
        b=setActiveSubWindow(action->text());
    }
    return b;
}

void MainWindow::actAction(QAction *a, void (MainWindow::*sl)())
{
    connect(a, &QAction::triggered, this, sl);
    actions.insert(a->text(),a);
}

void MainWindow::addSubWindow(QWidget *w, QObject *a)
{
    w->setAttribute(Qt::WA_DeleteOnClose);
    QAction *action = qobject_cast<QAction *>(a);
    if (action){
        w->setWindowTitle(action->text());
    }
    ui->tabWidget->addTab(w,w->windowTitle());
    ui->tabWidget->setCurrentWidget(w);
}

bool MainWindow::setActiveSubWindow(QString t)
{
    bool b=false;
    for (int i=0; i<ui->tabWidget->count(); i++){
        if (ui->tabWidget->tabText(i)==t){
            ui->tabWidget->setCurrentIndex(i);
            b=true;
            break;
        }
    }
    return b;
}

void MainWindow::closeTab(int index)
{
    ui->tabWidget->widget(index)->close();
}

void MainWindow::edtWire()
{
    if (!exist(sender())){
        addSubWindow(new FormEdtWire(),sender());
    }
}

void MainWindow::edtSpool()
{
    if (!exist(sender())){
        addSubWindow(new FormEdtPack(),sender());
    }
}

void MainWindow::edtLine()
{
    if (!exist(sender())){
        addSubWindow(new FormEdtLine(),sender());
    }
}

void MainWindow::edtMatr()
{
    if (!exist(sender())){
        addSubWindow(new FormMatr(),sender());
    }
}

void MainWindow::podtPart()
{
    if (!exist(sender())){
        addSubWindow(new FormPodt(),sender());
    }
}

void MainWindow::podtPresence()
{
    if (!exist(sender())){
        addSubWindow(new FormPresencePodt(),sender());
    }
}

void MainWindow::podtReport()
{
    if (!exist(sender())){
        addSubWindow(new FormReportPodt(),sender());
    }
}

void MainWindow::podtFix()
{
    if (!exist(sender())){
        addSubWindow(new FormFixPodt(),sender());
    }
}

void MainWindow::podtNakl()
{
    if (!exist(sender())){
        addSubWindow(new FormNaklPodt(),sender());
    }
}

void MainWindow::podtNorm()
{
    if (!exist(sender())){
        addSubWindow(new FormPodtNorm(),sender());
    }
}

void MainWindow::wirePart()
{
    if (!exist(sender())){
        addSubWindow(new FormPart(key==QString("srt")),sender());
    }
}

void MainWindow::wireLbl()
{
    if (!exist(sender())){
        addSubWindow(new FormLbl(),sender());
    }
}

void MainWindow::perepack()
{
    if (!exist(sender())){
        addSubWindow(new FormPerePack(),sender());
    }
}

void MainWindow::presence()
{
    if (!exist(sender())){
        addSubWindow(new FormPresence(),sender());
    }
}

void MainWindow::report()
{
    if (!exist(sender())){
        addSubWindow(new FormReport(),sender());
    }
}

void MainWindow::warehouse()
{
    if (!exist(sender())){
        addSubWindow(new FormMovWire(),sender());
    }
}

void MainWindow::nakl()
{
    if (!exist(sender())){
        addSubWindow(new FormNakl(),sender());
    }
}

void MainWindow::norm()
{
    if (!exist(sender())){
        addSubWindow(new FormNorm(),sender());
    }
}

void MainWindow::fix()
{
    if (!exist(sender())){
        addSubWindow(new FormFix(),sender());
    }
}

void MainWindow::cubeNam()
{
    if (!exist(sender())){
        addSubWindow(new CubeWidget(24),sender());
    }
}

void MainWindow::cubePack()
{
    if (!exist(sender())){
        addSubWindow(new CubeWidget(43),sender());
    }
}

void MainWindow::cubePost()
{
    if (!exist(sender())){
        addSubWindow(new CubeWidget(25),sender());
    }
}

void MainWindow::cubeRas()
{
    if (!exist(sender())){
        addSubWindow(new CubeWidget(26),sender());
    }
}

void MainWindow::cubePerepack()
{
    if (!exist(sender())){
        addSubWindow(new CubeWidget(27),sender());
    }
}

void MainWindow::cubePodt()
{
    if (!exist(sender())){
        addSubWindow(new CubeWidget(28),sender());
    }
}

void MainWindow::cubePodtOut()
{
    if (!exist(sender())){
        addSubWindow(new CubeWidget(45),sender());
    }
}
