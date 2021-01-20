#include "formpodt.h"
#include "ui_formpodt.h"

FormPodt::FormPodt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPodt)
{
    ui->setupUi(this);
    loadSettings();
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    //modelPodtMech = new ModelPodtMech(this);
    //ui->tableViewMech->setModel(modelPodtMech);
    //ui->tableViewMech->setColumnHidden(0,true);
    //ui->tableViewMech->setColumnHidden(1,true);
    //ui->tableViewMech->setColumnWidth(2,150);
    //ui->tableViewMech->setColumnWidth(3,90);

    modelPodtIn = new ModelPodtCex(this);
    ui->tableViewPodtIn->setModel(modelPodtIn);
    ui->tableViewPodtIn->setColumnHidden(0,true);
    ui->tableViewPodtIn->setColumnHidden(1,true);
    ui->tableViewPodtIn->setColumnHidden(2,true);
    ui->tableViewPodtIn->setColumnWidth(3,90);
    ui->tableViewPodtIn->setColumnWidth(4,80);

    modelPodtOut = new ModelPodtCex(this);
    ui->tableViewPodtOut->setModel(modelPodtOut);
    ui->tableViewPodtOut->setColumnHidden(0,true);
    ui->tableViewPodtOut->setColumnHidden(1,true);
    ui->tableViewPodtOut->setColumnHidden(2,true);
    ui->tableViewPodtOut->setColumnWidth(3,90);
    ui->tableViewPodtOut->setColumnWidth(4,80);

    modelCont = new ModelPodtCont(this);
    ui->tableViewCont->setModel(modelCont);
    ui->tableViewCont->setColumnHidden(0,true);
    ui->tableViewCont->setColumnHidden(1,true);
    ui->tableViewCont->setColumnWidth(2,90);
    ui->tableViewCont->setColumnWidth(3,80);
    ui->tableViewCont->setColumnWidth(4,150);
    ui->tableViewCont->setColumnWidth(5,320);

    modelPodtVol = new ModelPodtVol(this);
    ui->tableViewVol->setModel(modelPodtVol);
    ui->tableViewVol->setColumnHidden(0,true);
    ui->tableViewVol->setColumnWidth(1,90);
    ui->tableViewVol->setColumnWidth(2,80);
    ui->tableViewVol->setColumnWidth(3,140);
    ui->tableViewVol->setColumnWidth(4,150);

    modelPodt = new ModelPodt(this);
    ui->tableView->verticalHeader()->setDefaultSectionSize(ui->tableView->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableView->setModel(modelPodt);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(1,50);
    ui->tableView->setColumnWidth(2,70);
    ui->tableView->setColumnWidth(3,360);
    ui->tableView->setColumnWidth(4,40);
    for (int i=5; i<ui->tableView->model()->columnCount(); i++)
        ui->tableView->setColumnHidden(i,true);

    modelPodtPart = new ModelPodtPart(this);
    ui->tableViewPart->setModel(modelPodtPart);

    modelPodtAnn = new ModelPodtAnn(this);
    ui->tableViewAnn->setModel(modelPodtAnn);

    push = new DbMapper(ui->tableView,this);
    ui->horizontalLayoutBtn->insertWidget(0,push);
    push->addMapping(ui->lineEditNum,1);
    push->addMapping(ui->dateEdit,2);
    push->addMapping(ui->comboBoxSrs,3);
    push->addMapping(ui->comboBoxDiam,4);
    push->addMapping(ui->comboBoxLine,5);
    push->addMapping(ui->lineEditComm,6);
    push->addMapping(ui->comboBoxType,7);
    push->addLock(ui->dateEditBeg);
    push->addLock(ui->dateEditEnd);
    push->addLock(ui->cmdUpd);
    push->addEmptyLock(ui->tableViewCont);
    push->addEmptyLock(ui->tableViewPodtIn);
    push->addEmptyLock(ui->tableViewPodtOut);
    //push->addEmptyLock(ui->tableViewMech);
    push->addUnLock(ui->toolButtonSrc);
    push->addLock(ui->pushButtonFltPodt);

    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(refresh()));
    connect(push,SIGNAL(currentIndexChanged(int)),this,SLOT(updPart(int)));
    connect(modelCont,SIGNAL(sigSum(QString)),this,SLOT(setContItogo(QString)));
    connect(modelPodtVol,SIGNAL(sigSum(QString)),this,SLOT(setVolItogo(QString)));
    connect(ui->toolButtonSrc,SIGNAL(clicked(bool)),this,SLOT(fltSrc()));
    connect(ui->pushButtonFltPodt,SIGNAL(clicked(bool)),this,SLOT(fltPodt()));
    connect(modelPodtIn,SIGNAL(sigSum(QString)),this,SLOT(setInItogo(QString)));
    connect(modelPodtOut,SIGNAL(sigSum(QString)),this,SLOT(setOutItogo(QString)));

    refresh();
}

FormPodt::~FormPodt()
{
    saveSettings();
    delete ui;
}

void FormPodt::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    //this->restoreGeometry(settings.value("podt_geometry").toByteArray());
    this->ui->splitter->restoreState(settings.value("podt_splitter_width").toByteArray());
}

void FormPodt::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    //settings.setValue("podt_geometry", this->saveGeometry());
    settings.setValue("podt_splitter_width",ui->splitter->saveState());
}

void FormPodt::refresh()
{
    modelPodt->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date());
}

void FormPodt::updPart(int index)
{
    int id_podt=ui->tableView->model()->data(ui->tableView->model()->index(index,0),Qt::EditRole).toInt();
    QDate date=ui->tableView->model()->data(ui->tableView->model()->index(index,2),Qt::EditRole).toDate();
    modelCont->setDefaultValue(2,date);
    modelCont->refresh(id_podt);
    modelPodtVol->refresh(id_podt);
    modelPodtPart->refresh(id_podt);
    //modelPodtMech->refresh(id_podt);
    ui->tableViewPart->resizeToContents();
    modelPodtAnn->refresh(id_podt);
    ui->tableViewAnn->resizeToContents();
    modelPodtIn->refresh(id_podt,1);
    modelPodtOut->refresh(id_podt,2);
}

void FormPodt::setContItogo(QString s)
{
    ui->groupBoxCont->setTitle(s);
}

void FormPodt::setVolItogo(QString s)
{
    ui->groupBoxVol->setTitle(s);
}

void FormPodt::fltSrc()
{
    DialogFlt d(tr("исходные партии"),Models::instance()->relSrcPart);
    d.exec();
}

void FormPodt::fltPodt()
{
    DialogFlt d(tr("подтяжки"),Models::instance()->relPodt);
    d.exec();
}

void FormPodt::setInItogo(QString s)
{
    ui->groupBoxPodtIn->setTitle(s);
}

void FormPodt::setOutItogo(QString s)
{
    ui->groupBoxPodtOut->setTitle(s);
}
