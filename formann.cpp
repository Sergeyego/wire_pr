#include "formann.h"
#include "ui_formann.h"

FormAnn::FormAnn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormAnn)
{
    ui->setupUi(this);
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    relBuht = new DbRelation(Models::instance()->relPodt->model(),0,2,this);

    modelMechSrc = new DbTableModel("prov_mech",this);
    modelMechSrc->addColumn("id_buht",tr("id_buht"));
    modelMechSrc->addColumn("id_par",tr("Параметр"),Models::instance()->relMechTbl);
    modelMechSrc->addColumn("kvo",tr("Значение"));
    modelMechSrc->setDecimals(2,3);
    modelMechSrc->setDefaultValue(1,1);
    ui->tableViewMechSrc->setModel(modelMechSrc);
    ui->tableViewMechSrc->setColumnHidden(0,true);
    ui->tableViewMechSrc->setColumnWidth(1,150);
    ui->tableViewMechSrc->setColumnWidth(2,90);

    modelPodtMech = new ModelPodtMech(this);
    ui->tableViewPodtMech->setModel(modelPodtMech);
    ui->tableViewPodtMech->setColumnHidden(0,true);
    ui->tableViewPodtMech->setColumnHidden(1,true);
    ui->tableViewPodtMech->setColumnWidth(2,150);
    ui->tableViewPodtMech->setColumnWidth(3,90);

    modelMechAnn = new DbTableModel("wire_ann_mech",this);
    modelMechAnn->addColumn("id",tr("id"));
    modelMechAnn->addColumn("id_ann_cont",tr("id_ann_cont"));
    modelMechAnn->addColumn("id_mech",tr("Параметр"),Models::instance()->relMechTbl);
    modelMechAnn->addColumn("kvo",tr("Значение"));
    modelMechAnn->setDecimals(3,3);
    modelMechAnn->setDefaultValue(2,1);
    modelMechAnn->setSort("wire_ann_mech.id_mech, wire_ann_mech.kvo");
    ui->tableViewMechAnn->setModel(modelMechAnn);
    ui->tableViewMechAnn->setColumnHidden(0,true);
    ui->tableViewMechAnn->setColumnHidden(1,true);
    ui->tableViewMechAnn->setColumnWidth(2,150);
    ui->tableViewMechAnn->setColumnWidth(3,90);

    modelCont = new DbTableModel("wire_ann_cont",this);
    modelCont->addColumn("id",tr("id"));
    modelCont->addColumn("id_ann",tr("id_ann"));
    modelCont->addColumn("id_podt",tr("Подтяжка"),Models::instance()->relPodt);
    modelCont->addColumn("num",tr("NN"));
    modelCont->addColumn("kvo",tr("Масса, кг"));
    modelCont->setSort("wire_ann_cont.num");
    ui->tableViewCont->setModel(modelCont);
    ui->tableViewCont->setColumnHidden(0,true);
    ui->tableViewCont->setColumnHidden(1,true);
    ui->tableViewCont->setColumnWidth(2,320);
    ui->tableViewCont->setColumnWidth(3,50);
    ui->tableViewCont->setColumnWidth(4,70);

    modelPodtNext = new ModelPodtNext(this);
    ui->tableViewNext->setModel(modelPodtNext);
    ui->tableViewNext->resizeToContents();

    modelPodtPart = new ModelPodtPart(this);
    ui->tableViewPart->setModel(modelPodtPart);

    ui->tableViewAnn->verticalHeader()->setDefaultSectionSize(ui->tableViewAnn->verticalHeader()->fontMetrics().height()*1.5);
    modelAnn = new ModelAnn(this);
    ui->tableViewAnn->setModel(modelAnn);
    ui->tableViewAnn->setColumnHidden(0,true);
    for (int i=4; i<ui->tableViewAnn->model()->columnCount(); i++)
        ui->tableViewAnn->setColumnHidden(i,true);
    ui->tableViewAnn->setColumnWidth(1,100);
    ui->tableViewAnn->setColumnWidth(2,100);
    ui->tableViewAnn->setColumnWidth(3,100);

    push = new DbMapper(ui->tableViewAnn,this);
    ui->horizontalLayoutBtn->insertWidget(0,push);

    push->addMapping(ui->lineEditNum,1);
    push->addMapping(ui->dateEdit,2);
    push->addMapping(ui->comboBoxStend,3);
    push->addMapping(ui->lineEditBlow,4);
    push->addMapping(ui->lineEditTemp,5);
    push->addMapping(ui->lineEditEx,6);
    push->addMapping(ui->plainTextEdit,7);
    push->addLock(ui->dateEditBeg);
    push->addLock(ui->dateEditEnd);
    push->addLock(ui->cmdUpd);
    push->addLock(ui->groupBoxLoad);
    push->addLock(ui->groupBoxMech);
    push->addLock(ui->pushButtonFltPodt);

    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(refresh()));
    connect(push,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshCont(int)));
    connect(ui->tableViewCont->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(refreshMech(QModelIndex)));
    connect(ui->pushButtonFltPodt,SIGNAL(clicked(bool)),this,SLOT(fltPodt()));

    refresh();
}

FormAnn::~FormAnn()
{
    delete ui;
}

void FormAnn::refresh()
{
    modelAnn->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date());
}

void FormAnn::refreshCont(int index)
{
    int id_ann=ui->tableViewAnn->model()->data(ui->tableViewAnn->model()->index(index,0),Qt::EditRole).toInt();
    modelCont->setDefaultValue(1,id_ann);
    modelCont->setFilter("wire_ann_cont.id_ann = "+QString::number(id_ann));
    modelCont->select();
    ui->tableViewCont->selectRow(0);
}

void FormAnn::refreshMech(QModelIndex index)
{
    int id_podt=-1;
    int id_buht=-1;
    int id_cont=-1;
    if (!(index.row()==modelCont->rowCount()-1 && modelCont->isAdd()) && index.row()>=0 && index.row()<modelCont->rowCount()) {
        ui->groupBoxMech->setEnabled(true);
        id_podt=ui->tableViewCont->model()->data(ui->tableViewCont->model()->index(index.row(),2),Qt::EditRole).toInt();
        id_buht=relBuht->data(QString::number(id_podt)).toInt();
        id_cont=ui->tableViewCont->model()->data(ui->tableViewCont->model()->index(index.row(),0),Qt::EditRole).toInt();
    } else {
        ui->groupBoxMech->setEnabled(false);
    }

    modelMechSrc->setDefaultValue(0,id_buht);
    modelMechSrc->setFilter("prov_mech.id_buht = "+QString::number(id_buht));
    modelMechSrc->select();

    modelPodtMech->refresh(id_podt);

    modelMechAnn->setDefaultValue(1,id_cont);
    modelMechAnn->setFilter("wire_ann_mech.id_ann_cont = "+QString::number(id_cont));
    modelMechAnn->select();

    modelPodtNext->refresh(id_podt);
    ui->tableViewNext->resizeToContents();

    modelPodtPart->refresh(id_podt);
    ui->tableViewPart->resizeToContents();

}

void FormAnn::fltPodt()
{
    DialogFlt d(tr("подтяжки"),Models::instance()->relPodt);
    d.exec();
}
