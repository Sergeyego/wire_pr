#include "formwarehouse.h"
#include "ui_formwarehouse.h"

FormWarehouse::FormWarehouse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormWarehouse)
{
    ui->setupUi(this);
    loadSettings();
    ui->dateEdit->setDate(QDate::currentDate());

    modelCex=new ModelCex(this);
    ui->tableViewInCex->verticalHeader()->setDefaultSectionSize(ui->tableViewInCex->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewInCex->setModel(modelCex);

    modelStock = new ModelStock(this);
    ui->tableViewStock->verticalHeader()->setDefaultSectionSize(ui->tableViewStock->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewStock->setModel(modelStock);
    ui->tableViewStock->setColumnHidden(4,true);

    modelShip = new ModelShipment(this);
    ui->tableViewShip->verticalHeader()->setDefaultSectionSize(ui->tableViewShip->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewShip->setModel(modelShip);
    ui->tableViewShip->setColumnHidden(4,true);

    modelWar=new ModelWarehouse(this);
    modelWar->setDate(ui->dateEdit->date());
    modelWar->refresh();
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(modelWar);
    ui->tableViewPart->verticalHeader()->setDefaultSectionSize(ui->tableViewPart->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewPart->setModel(proxyModel);
    ui->tableViewPart->setColumnHidden(0,true);
    ui->tableViewPart->setColumnWidth(1,80);
    ui->tableViewPart->setColumnWidth(2,130);
    ui->tableViewPart->setColumnWidth(3,50);
    ui->tableViewPart->setColumnWidth(4,80);
    ui->tableViewPart->setColumnWidth(5,80);
    ui->tableViewPart->setColumnWidth(6,80);

    connect(ui->cmdUpd,SIGNAL(clicked()),modelWar,SLOT(refresh()));
    connect(ui->checkBoxNZ,SIGNAL(toggled(bool)),modelWar,SLOT(setNotZero(bool)));
    connect(ui->tableViewPart->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(updData(QModelIndex)));
    connect(ui->dateEdit,SIGNAL(dateChanged(QDate)),modelWar,SLOT(setDate(QDate)));
}

FormWarehouse::~FormWarehouse()
{
    saveSettings();
    delete ui;
}

void FormWarehouse::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    //this->restoreGeometry(settings.value("ware_geometry").toByteArray());
    this->ui->splitter->restoreState(settings.value("ware_splitter_width").toByteArray());
}

void FormWarehouse::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    //settings.setValue("ware_geometry", this->saveGeometry());
    settings.setValue("ware_splitter_width",ui->splitter->saveState());
}

void FormWarehouse::updData(QModelIndex index)
{
    QDate d=ui->dateEdit->date();
    int id_part=proxyModel->data(proxyModel->index(index.row(),0)).toInt();
    modelCex->refresh(id_part,d);
    modelStock->refresh(id_part,d);
    modelShip->refresh(id_part,d);
    ui->tableViewInCex->resizeColumnsToContents();
    ui->tableViewStock->resizeColumnsToContents();
    ui->tableViewShip->resizeColumnsToContents();
}
