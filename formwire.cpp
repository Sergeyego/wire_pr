#include "formwire.h"
#include "ui_formwire.h"

FormWire::FormWire(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormWire)
{
    ui->setupUi(this);
    loadSettings();
    modelGost = new DbTableModel("wire_gost",this);
    modelGost->addColumn("id","id",true,true,TYPE_INT);
    modelGost->addColumn("id_provol","id_provol",false,false,TYPE_INT);
    modelGost->addColumn("id_gost",tr("ГОСТ/ТУ"),false,false,TYPE_STRING,NULL,Models::instance()->relNewGost);
    modelDiamSert = new DbTableModel("zvd_wire_diam_sert",this);
    modelDiamSert->addColumn("id","id",true,true,TYPE_INT);
    modelDiamSert->addColumn("id_provol","id_provol",false,false,TYPE_INT);
    modelDiamSert->addColumn("id_diam",tr("Диаметр"),false,false,TYPE_STRING,NULL,Models::instance()->relDiam);
    modelDiamSert->addColumn("id_sert",tr("№ Документа"),false,false,TYPE_STRING,NULL,Models::instance()->relZvdSert);
    modelDiamSert->setSuffix("inner join diam as d on zvd_wire_diam_sert.id_diam=d.id");
    modelDiamSert->setSort("d.sdim");

    ui->listViewWire->setModel(Models::instance()->relProvol->model());
    ui->listViewWire->setModelColumn(1);

    ui->tableViewTu->setModel(modelGost);
    ui->tableViewTu->setColumnHidden(0,true);
    ui->tableViewTu->setColumnHidden(1,true);
    ui->tableViewTu->setColumnWidth(2,300);

    ui->tableViewAtt->setModel(modelDiamSert);
    ui->tableViewAtt->setColumnHidden(0,true);
    ui->tableViewAtt->setColumnHidden(1,true);
    ui->tableViewAtt->setColumnWidth(2,70);
    ui->tableViewAtt->setColumnWidth(3,250);

    connect(ui->listViewWire->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(setCurrentIndex(QModelIndex)));

    ui->listViewWire->setCurrentIndex(ui->listViewWire->model()->index(0,1));
}

FormWire::~FormWire()
{
    saveSettings();
    delete ui;
}

void FormWire::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    //this->restoreGeometry(settings.value("wire_geometry").toByteArray());
    this->ui->splitter1->restoreState(settings.value("tu_splitter_width").toByteArray());
}

void FormWire::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    //settings.setValue("wire_geometry", this->saveGeometry());
    settings.setValue("tu_splitter_width",ui->splitter1->saveState());
}

void FormWire::setCurrentIndex(QModelIndex index)
{
    int id_provol=ui->listViewWire->model()->data(ui->listViewWire->model()->index(index.row(),0)).toInt();
    modelGost->setFilter("wire_gost.id_provol="+QString::number(id_provol));
    modelGost->setDefaultValue(1,id_provol);
    modelGost->select();

    modelDiamSert->setFilter("zvd_wire_diam_sert.id_provol="+QString::number(id_provol));
    modelDiamSert->setDefaultValue(1,id_provol);
    modelDiamSert->select();
}

