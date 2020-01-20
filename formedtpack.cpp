#include "formedtpack.h"
#include "ui_formedtpack.h"

FormEdtPack::FormEdtPack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEdtPack)
{
    ui->setupUi(this);
    modelPack = new DbTableModel("wire_pack_kind",this);
    modelPack->addColumn("id",tr("id"),true,TYPE_INT);
    modelPack->addColumn("nam",tr("Наименование"),false,TYPE_STRING);
    modelPack->addColumn("short",tr("Крат. наименование"),false,TYPE_STRING);
    modelPack->addColumn("short_en",tr("Наименование анг."),false,TYPE_STRING);
    modelPack->addColumn("id_pack_def",tr("Упаковка по умолчанию"),false,TYPE_STRING,NULL,Models::instance()->relPackType);
    modelPack->setSort("wire_pack_kind.nam");
    modelPack->select();
    ui->tableView->setModel(modelPack);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(1,160);
    ui->tableView->setColumnWidth(2,150);
    ui->tableView->setColumnWidth(3,150);
    ui->tableView->setColumnWidth(4,250);

    modelPackType = new DbTableModel("wire_pack",this);
    modelPackType->addColumn("id",tr("id"),true,TYPE_INT);
    modelPackType->addColumn("pack_ed",tr("Единичная упаковка"),false,TYPE_STRING);
    modelPackType->addColumn("pack_group",tr("Групповая упаковка"),false,TYPE_STRING);
    modelPackType->addColumn("mas_ed",tr("Масса ед., кг"),false,TYPE_DOUBLE,new QDoubleValidator(0,999999999,3,this));
    modelPackType->addColumn("mas_group",tr("Масса гр., кг"),false,TYPE_DOUBLE,new QDoubleValidator(0,999999999,3,this));
    modelPackType->setSort("wire_pack.pack_ed");
    modelPackType->select();
    ui->tableViewType->setModel(modelPackType);
    ui->tableViewType->setColumnHidden(0,true);
    ui->tableViewType->setColumnWidth(1,150);
    ui->tableViewType->setColumnWidth(2,150);
    ui->tableViewType->setColumnWidth(3,100);
    ui->tableViewType->setColumnWidth(4,100);

    connect(modelPack,SIGNAL(sigUpd()),Models::instance()->relPack->model(),SLOT(refresh()));
    connect(modelPackType,SIGNAL(sigUpd()),Models::instance()->relPackType->model(),SLOT(refresh()));
}

FormEdtPack::~FormEdtPack()
{
    delete ui;
}
