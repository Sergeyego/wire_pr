#include "formedtwire.h"
#include "ui_formedtwire.h"

FormEdtWire::FormEdtWire(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEdtWire)
{
    ui->setupUi(this);    
    ui->tableViewWire->verticalHeader()->setDefaultSectionSize(ui->tableViewWire->verticalHeader()->fontMetrics().height()*1.5);
    modelEan = new DbTableModel("wire_ean",this);
    modelEan->addColumn("id_prov",tr("id_prov"),true,false,TYPE_STRING);
    modelEan->addColumn("id_diam",tr("Диаметр"),true,false,TYPE_STRING,NULL,Models::instance()->relDiam);
    modelEan->addColumn("id_spool",tr("Носитель"),true,false,TYPE_STRING,NULL,Models::instance()->relPack);
    modelEan->addColumn("id_pack",tr("Упаковка (ед., групп.)"),true,false,TYPE_STRING,NULL,Models::instance()->relPackType);
    modelEan->addColumn("ean_ed",tr("Штрих код (ед.)"),false,false,TYPE_STRING);
    modelEan->addColumn("ean_group",tr("Штрих код (гр.)"),false,false,TYPE_STRING);
    modelEan->setSuffix("inner join diam on wire_ean.id_diam=diam.id");
    modelEan->setSort("diam.sdim");
    ui->tableViewPack->setModel(modelEan);
    ui->tableViewPack->setColumnHidden(0,true);
    ui->tableViewPack->setColumnWidth(1,70);
    ui->tableViewPack->setColumnWidth(2,90);
    ui->tableViewPack->setColumnWidth(3,250);
    ui->tableViewPack->setColumnWidth(4,125);
    ui->tableViewPack->setColumnWidth(5,125);

    modelKr = new DbTableModel("wire_kr",this);
    modelKr->addColumn("id_prov",tr("id_prov"),true,false,TYPE_INT);
    modelKr->addColumn("dat",tr("Дата"),true,false,TYPE_DATE);
    modelKr->addColumn("val",tr("Значение"),false,false,TYPE_INT,new QIntValidator(0,1000000,this));
    modelKr->setSort("wire_kr.dat");
    ui->tableViewKr->setModel(modelKr);
    ui->tableViewKr->setColumnHidden(0,true);
    ui->tableViewKr->setColumnWidth(1,100);
    ui->tableViewKr->setColumnWidth(2,100);

    modelWire = new DbTableModel("provol",this);
    modelWire->addColumn("id","id",true,true,TYPE_INT);
    modelWire->addColumn("nam",tr("Название"),false,false,TYPE_STRING);
    modelWire->addColumn("is_w",tr("3 Цех"),false,false,TYPE_INTBOOL);
    modelWire->addColumn("nerzh",tr("Нержавейка"),false,false,TYPE_INTBOOL);
    modelWire->addColumn("is_cored",tr("Порошковая"),false,false,TYPE_BOOL);
    modelWire->addColumn("description",tr("Описание"),false,false,TYPE_STRING);
    modelWire->setSort("provol.nam");
    modelWire->setDefaultValue(5,1025);
    ui->tableViewWire->setModel(modelWire);
    ui->tableViewWire->setColumnHidden(0,true);
    ui->tableViewWire->setColumnWidth(1,200);

    for (int i=2; i<ui->tableViewWire->model()->columnCount(); i++){
        ui->tableViewWire->setColumnHidden(i,true);
    }

    mapper = new PushForm(ui->tableViewWire,this);
    this->layout()->addWidget(mapper);

    mapper->addMapping(ui->lineEditMark,1);
    mapper->addMapping(ui->checkBoxWire,2);
    mapper->addMapping(ui->checkBoxNerj,3);
    mapper->addMapping(ui->checkBoxCored,4);
    mapper->addMapping(ui->plainTextEdit,5);

    mapper->addEmptyLock(ui->groupBox);

    connect(modelWire,SIGNAL(sigUpd()),Models::instance()->relProvol->model(),SLOT(refresh()));
    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(updEan(int)));

    modelWire->select();
}

FormEdtWire::~FormEdtWire()
{
    delete ui;
}

void FormEdtWire::updEan(int row)
{
    int id_prov=ui->tableViewWire->model()->data(ui->tableViewWire->model()->index(row,0),Qt::EditRole).toInt();
    modelEan->setDefaultValue(0,id_prov);
    modelEan->setFilter("wire_ean.id_prov = "+QString::number(id_prov));
    modelEan->select();

    modelKr->setDefaultValue(0,id_prov);
    modelKr->setFilter("wire_kr.id_prov = "+QString::number(id_prov));
    modelKr->select();
}
