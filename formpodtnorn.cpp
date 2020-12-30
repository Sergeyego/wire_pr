#include "formpodtnorn.h"
#include "ui_formpodtnorn.h"

FormPodtNorn::FormPodtNorn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPodtNorn)
{
    ui->setupUi(this);

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    modelNorm = new DbTableModel("wire_podt_norm",this);
    modelNorm->addColumn("id_podt_type","id_podt_type");
    modelNorm->addColumn("id_provol","id_provol");
    modelNorm->addColumn("id_src_diam","id_src_diam");
    modelNorm->addColumn("id_diam","id_diam");
    modelNorm->addColumn("id_matr",tr("Материал"),Models::instance()->relMatr);
    modelNorm->addColumn("kvo",tr("Норма"));
    modelNorm->addColumn("id_vid",tr("Вид затрат"),Models::instance()->relRasxVid);

    modelNorm->setSort("id_vid, id_matr");
    modelNorm->setDecimals(5,2);

    ui->tableViewNorm->setModel(modelNorm);
    for (int i=0; i<=3; i++){
        ui->tableViewNorm->setColumnHidden(i,true);
    }
    ui->tableViewNorm->setColumnWidth(4,330);
    ui->tableViewNorm->setColumnWidth(5,80);
    ui->tableViewNorm->setColumnWidth(6,200);

    modelPodtProd = new ModelPodtProd(this);
    ui->tableViewPodt->setModel(modelPodtProd);

    connect(ui->tableViewPodt->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(updNorm(QModelIndex)));
    upd();

    for (int i=0; i<=3; i++){
        ui->tableViewPodt->setColumnHidden(i,true);
    }
    ui->tableViewPodt->setColumnWidth(4,220);
    ui->tableViewPodt->setColumnWidth(5,160);
    ui->tableViewPodt->setColumnWidth(6,60);
    ui->tableViewPodt->setColumnWidth(7,60);
    ui->tableViewPodt->setColumnWidth(8,90);

    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->pushButtonCopy,SIGNAL(clicked(bool)),this,SLOT(copy()));
    connect(ui->pushButtonPaste,SIGNAL(clicked(bool)),this,SLOT(paste()));

}

FormPodtNorn::~FormPodtNorn()
{
    delete ui;
}

void FormPodtNorn::upd()
{
    modelPodtProd->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date());
    if (ui->tableViewPodt->model()->rowCount()){
        ui->tableViewPodt->selectRow(0);
    }
}

void FormPodtNorn::updNorm(QModelIndex ind)
{
    int id_type=ui->tableViewPodt->model()->data(ui->tableViewPodt->model()->index(ind.row(),0),Qt::EditRole).toInt();
    int id_provol=ui->tableViewPodt->model()->data(ui->tableViewPodt->model()->index(ind.row(),1),Qt::EditRole).toInt();
    int id_src_diam=ui->tableViewPodt->model()->data(ui->tableViewPodt->model()->index(ind.row(),2),Qt::EditRole).toInt();
    int id_diam=ui->tableViewPodt->model()->data(ui->tableViewPodt->model()->index(ind.row(),3),Qt::EditRole).toInt();
    modelNorm->setDefaultValue(0,id_type);
    modelNorm->setDefaultValue(1,id_provol);
    modelNorm->setDefaultValue(2,id_src_diam);
    modelNorm->setDefaultValue(3,id_diam);
    modelNorm->setFilter(QString("id_podt_type = %1 and id_provol = %2 and id_src_diam = %3 and id_diam = %4").arg(id_type).arg(id_provol).arg(id_src_diam).arg(id_diam));
    modelNorm->select();
}

void FormPodtNorn::copy()
{

}

void FormPodtNorn::paste()
{

}
