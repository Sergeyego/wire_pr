#include "formedtwire.h"
#include "ui_formedtwire.h"

FormEdtWire::FormEdtWire(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEdtWire)
{
    ui->setupUi(this);
    loadSettings();
    ui->tableViewWire->verticalHeader()->setDefaultSectionSize(ui->tableViewWire->verticalHeader()->fontMetrics().height()*1.5);

    modelChemTu = new DbTableModel("wire_chem_tu",this);
    modelChemTu->addColumn("id_provol","id_provol",true,TYPE_INT);
    modelChemTu->addColumn("id_chem",QString::fromUtf8("Элемент"),true,TYPE_STRING,NULL,Models::instance()->relChemTbl);
    modelChemTu->addColumn("min",QString::fromUtf8("Минимум, %"),false,TYPE_DOUBLE,new QDoubleValidator(0,100,3,this));
    modelChemTu->addColumn("max",QString::fromUtf8("Максимум, %"),false,TYPE_DOUBLE,new QDoubleValidator(0,100,3,this));
    modelChemTu->setSort("wire_chem_tu.id_chem");

    modelDim = new DbTableModel("wire_diams",this);
    modelDim->addColumn("id_wire","id_wire",true,TYPE_INT);
    modelDim->addColumn("id_diam",QString::fromUtf8("Диаметр"),true,TYPE_STRING,NULL,Models::instance()->relDiam);
    modelDim->setSuffix("inner join diam on wire_diams.id_diam=diam.id");
    modelDim->setSort("diam.sdim");
    ui->tableViewDiam->setModel(modelDim);
    ui->tableViewDiam->setColumnHidden(0,true);
    ui->tableViewDiam->setColumnWidth(1,70);

    ui->tableViewChem->setModel(modelChemTu);
    ui->tableViewChem->setColumnHidden(0,true);
    ui->tableViewChem->setColumnWidth(1,100);
    ui->tableViewChem->setColumnWidth(2,100);
    ui->tableViewChem->setColumnWidth(3,100);

    modelEan = new ModelEan(this);

    ui->tableViewPack->setModel(modelEan);
    ui->tableViewPack->setColumnHidden(0,true);
    ui->tableViewPack->setColumnWidth(1,70);
    ui->tableViewPack->setColumnWidth(2,90);
    ui->tableViewPack->setColumnWidth(3,250);
    ui->tableViewPack->setColumnWidth(4,125);
    ui->tableViewPack->setColumnWidth(5,125);

    DbDelegate *delegate=qobject_cast<DbDelegate *>(ui->tableViewPack->itemDelegate());
    if (delegate){
        connect(delegate,SIGNAL(createEdt(QModelIndex)),modelEan,SLOT(updRels(QModelIndex)));
    }

    modelKr = new DbTableModel("wire_kr",this);
    modelKr->addColumn("id_prov",tr("id_prov"),true,TYPE_INT);
    modelKr->addColumn("dat",tr("Дата"),true,TYPE_DATE);
    modelKr->addColumn("val",tr("Значение"),false,TYPE_INT,new QIntValidator(0,1000000,this));
    modelKr->setSort("wire_kr.dat");
    ui->tableViewKr->setModel(modelKr);
    ui->tableViewKr->setColumnHidden(0,true);
    ui->tableViewKr->setColumnWidth(1,100);
    ui->tableViewKr->setColumnWidth(2,100);

    modelGost = new DbTableModel("wire_gost",this);
    modelGost->addColumn("id","id",true,TYPE_INT);
    modelGost->addColumn("id_provol","id_provol",false,TYPE_INT);
    modelGost->addColumn("id_gost",tr("ГОСТ/ТУ"),false,TYPE_STRING,NULL,Models::instance()->relNewGost);
    ui->tableViewTu->setModel(modelGost);
    ui->tableViewTu->setColumnHidden(0,true);
    ui->tableViewTu->setColumnHidden(1,true);
    ui->tableViewTu->setColumnWidth(2,200);

    modelWire = new DbTableModel("provol",this);
    modelWire->addColumn("id","id",true,TYPE_INT);
    modelWire->addColumn("nam",tr("Название"),false,TYPE_STRING);
    modelWire->addColumn("is_w",tr("3 Цех"),false,TYPE_INTBOOL);
    modelWire->addColumn("nerzh",tr("Нержавейка"),false,TYPE_INTBOOL);
    modelWire->addColumn("is_cored",tr("Порошковая"),false,TYPE_BOOL);
    modelWire->addColumn("description",tr("Описание"),false,TYPE_STRING);
    modelWire->addColumn("katalog",tr("Каталог"),false,TYPE_BOOL);
    modelWire->addColumn("id_base",tr("Базовая марка"),false,TYPE_STRING,NULL,Models::instance()->relProvol);
    modelWire->addColumn("id_type",tr("Тип"),false,TYPE_STRING,NULL,Models::instance()->relWireType);
    modelWire->setSort("provol.nam");

    ui->tableViewWire->setModel(modelWire);
    ui->tableViewWire->setColumnHidden(0,true);
    ui->tableViewWire->setColumnWidth(1,200);
    ui->tableViewWire->setColumnWidth(7,150);

    for (int i=2; i<ui->tableViewWire->model()->columnCount(); i++){
        if (i!=ui->tableViewWire->model()->columnCount()-2){
            ui->tableViewWire->setColumnHidden(i,true);
        }
    }

    mapper = new DbMapper(ui->tableViewWire,this);
    ui->horizontalLayoutBtn->insertWidget(0,mapper);

    mapper->addMapping(ui->lineEditMark,1);
    mapper->addMapping(ui->checkBoxWire,2);
    mapper->addMapping(ui->checkBoxNerj,3);
    mapper->addMapping(ui->checkBoxCored,4);
    mapper->addMapping(ui->plainTextEdit,5);
    mapper->addMapping(ui->checkBoxKat,6);
    mapper->addMapping(ui->comboBoxBase,7);
    mapper->addMapping(ui->comboBoxType,8);

    mapper->addEmptyLock(ui->groupBoxChem);
    mapper->addEmptyLock(ui->groupBoxKoef);
    mapper->addEmptyLock(ui->groupBoxEan);
    mapper->addEmptyLock(ui->groupBoxDiam);
    mapper->addEmptyLock(ui->tableViewTu);

    connect(modelWire,SIGNAL(sigUpd()),Models::instance()->relProvol->model(),SLOT(refresh()));
    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(updEan(int)));

    modelWire->select();
}

FormEdtWire::~FormEdtWire()
{
    saveSettings();
    delete ui;
}

void FormEdtWire::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    if (!settings.value("edtwire_splitter_width").isNull()){
        this->ui->splitter->restoreState(settings.value("edtwire_splitter_width").toByteArray());
    }
}

void FormEdtWire::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    settings.setValue("edtwire_splitter_width",ui->splitter->saveState());
}

void FormEdtWire::updEan(int row)
{
    int id_prov=ui->tableViewWire->model()->data(ui->tableViewWire->model()->index(row,0),Qt::EditRole).toInt();
    modelEan->refresh(id_prov);

    modelKr->setDefaultValue(0,id_prov);
    modelKr->setFilter("wire_kr.id_prov = "+QString::number(id_prov));
    modelKr->select();

    modelDim->setDefaultValue(0,id_prov);
    modelDim->setFilter("wire_diams.id_wire = "+QString::number(id_prov));
    modelDim->select();

    modelGost->setFilter("wire_gost.id_provol="+QString::number(id_prov));
    modelGost->setDefaultValue(1,id_prov);
    modelGost->select();

    if (!ui->tableViewWire->model()->data(ui->tableViewWire->model()->index(row,7),Qt::EditRole).isNull()){
        id_prov=ui->tableViewWire->model()->data(ui->tableViewWire->model()->index(row,7),Qt::EditRole).toInt();
    }

    modelChemTu->setDefaultValue(0,id_prov);
    modelChemTu->setFilter("wire_chem_tu.id_provol = "+QString::number(id_prov));
    modelChemTu->select();

}
