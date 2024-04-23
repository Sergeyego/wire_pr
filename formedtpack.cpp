#include "formedtpack.h"
#include "ui_formedtpack.h"

FormEdtPack::FormEdtPack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEdtPack)
{
    ui->setupUi(this);
    modelPack = new DbTableModel("wire_pack_kind",this);
    modelPack->addColumn("id",tr("id"));
    modelPack->addColumn("nam",tr("Наименование"));
    modelPack->addColumn("short",tr("Крат. наименование"));
    modelPack->addColumn("short_en",tr("Наименование анг."));
    modelPack->addColumn("id_pack_def",tr("Упаковка по умолчанию"),Rels::instance()->relPackType);
    modelPack->setSort("wire_pack_kind.nam");
    modelPack->select();
    ui->tableView->setModel(modelPack);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(1,160);
    ui->tableView->setColumnWidth(2,150);
    ui->tableView->setColumnWidth(3,150);
    ui->tableView->setColumnWidth(4,250);

    modelPackType = new DbTableModel("wire_pack",this);
    modelPackType->addColumn("id",tr("id"));
    modelPackType->addColumn("pack_ed",tr("Единичная упаковка"));
    modelPackType->addColumn("pack_group",tr("Групповая упаковка"));
    modelPackType->addColumn("mas_ed",tr("Масса ед., кг"));
    modelPackType->addColumn("mas_group",tr("Масса гр., кг"));
    modelPackType->setSort("wire_pack.pack_ed");
    modelPackType->select();
    ui->tableViewType->setModel(modelPackType);
    ui->tableViewType->setColumnHidden(0,true);
    ui->tableViewType->setColumnWidth(1,150);
    ui->tableViewType->setColumnWidth(2,150);
    ui->tableViewType->setColumnWidth(3,100);
    ui->tableViewType->setColumnWidth(4,100);

    connect(modelPack,SIGNAL(sigUpd()),Rels::instance()->relPack,SLOT(refreshModel()));
    connect(modelPackType,SIGNAL(sigUpd()),Rels::instance()->relPackType,SLOT(refreshModel()));
}

FormEdtPack::~FormEdtPack()
{
    delete ui;
}
