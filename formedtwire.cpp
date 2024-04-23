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
    modelChemTu->addColumn("id_provol","id_provol");
    modelChemTu->addColumn("id_chem",QString::fromUtf8("Элемент"),Rels::instance()->relChem);
    modelChemTu->addColumn("min",QString::fromUtf8("Минимум, %"));
    modelChemTu->addColumn("max",QString::fromUtf8("Максимум, %"));
    modelChemTu->setSort("wire_chem_tu.id_chem");
    modelChemTu->setDecimals(2,3);
    modelChemTu->setDecimals(3,3);

    modelDim = new DbTableModel("wire_diams",this);
    modelDim->addColumn("id_wire","id_wire");
    modelDim->addColumn("id_diam",QString::fromUtf8("Диаметр"),Rels::instance()->relDiam);
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
    modelKr->addColumn("id_prov",tr("id_prov"));
    modelKr->addColumn("dat",tr("Дата"));
    modelKr->addColumn("val",tr("Значение"));
    modelKr->setSort("wire_kr.dat");
    ui->tableViewKr->setModel(modelKr);
    ui->tableViewKr->setColumnHidden(0,true);
    ui->tableViewKr->setColumnWidth(1,100);
    ui->tableViewKr->setColumnWidth(2,100);

    modelGost = new DbTableModel("wire_gost",this);
    modelGost->addColumn("id","id");
    modelGost->addColumn("id_provol","id_provol");
    modelGost->addColumn("id_gost",tr("ГОСТ/ТУ"),Rels::instance()->relNewGost);
    ui->tableViewTu->setModel(modelGost);
    ui->tableViewTu->setColumnHidden(0,true);
    ui->tableViewTu->setColumnHidden(1,true);
    ui->tableViewTu->setColumnWidth(2,200);

    modelWire = new DbTableModel("provol",this);
    modelWire->addColumn("id","id");
    modelWire->addColumn("nam",tr("Название"));
    modelWire->addColumn("is_w",tr("3 Цех"));
    modelWire->addColumn("nerzh",tr("Нержавейка"));
    modelWire->addColumn("is_cored",tr("Порошковая"));
    modelWire->addColumn("description",tr("Описание"));
    modelWire->addColumn("katalog",tr("Каталог"));
    modelWire->addColumn("id_base",tr("Базовая марка"),Rels::instance()->relProvol);
    modelWire->addColumn("id_type",tr("Тип"),Rels::instance()->relWireType);
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

    connect(modelWire,SIGNAL(sigUpd()),Rels::instance()->relProvol,SLOT(refreshModel()));
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

ModelEan::ModelEan(QObject *parent) : DbTableModel("wire_ean",parent)
{
    addColumn("id_prov","id_prov");
    addColumn("id_diam",tr("Диаметр"),Rels::instance()->relDiam);
    addColumn("id_spool",tr("Носитель"),Rels::instance()->relPack);
    addColumn("id_pack",tr("Упаковка (ед., групп.)"),Rels::instance()->relPackType);
    addColumn("ean_ed",tr("Штрих код (ед.)"),Rels::instance()->relEanEd);
    addColumn("ean_group",tr("Штрих код (гр.)"),Rels::instance()->relEanGr);
    setSort("diam.sdim");
}

void ModelEan::refresh(int id_prov)
{
    setDefaultValue(0,id_prov);
    setFilter("wire_ean.id_prov = "+QString::number(id_prov));
    select();
}

void ModelEan::updRels(QModelIndex index)
{
    if (index.column()==4){
        Rels::instance()->relEanEd->refreshModel();
    } else if (index.column()==5){
        Rels::instance()->relEanGr->refreshModel();
    }
}
