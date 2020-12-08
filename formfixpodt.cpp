#include "formfixpodt.h"
#include "ui_formfixpodt.h"

FormFixPodt::FormFixPodt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFixPodt)
{
    ui->setupUi(this);

    modelDat = new QSqlQueryModel(this);
    ui->comboBoxDat->setModel(modelDat);

    modelFix = new DbTableModel("wire_podt_ost",this);
    modelFix->addColumn("dat",tr("Дата"),true,TYPE_DATE);
    modelFix->addColumn("id_podt",tr("Подтяжка"),true,TYPE_STRING,NULL,Models::instance()->relPodt);
    modelFix->addColumn("kvo",tr("Кол-во, кг"),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000,2,this));
    modelFix->setSuffix("inner join wire_podt as wp on wp.id = wire_podt_ost.id_podt "
                        "inner join prov_buht as h on wp.id_buht=h.id "
                        "inner join prov_prih as p on p.id=h.id_prih "
                        "inner join provol as r on r.id=p.id_pr "
                        "inner join diam as d on wp.id_diam=d.id");
    modelFix->setSort("r.nam, d.diam, wp.n_s, wp.dat");
    ui->dateEdit->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()));
    modelFix->setDefaultValue(0,ui->dateEdit->date());

    ui->tableView->setModel(modelFix);
    ui->tableView->setColumnWidth(0,80);
    ui->tableView->setColumnWidth(1,300);
    ui->tableView->setColumnWidth(2,80);

    connect(ui->comboBoxDat,SIGNAL(currentIndexChanged(int)),this,SLOT(updFixData(int)));
    connect(ui->pushButtonFlt,SIGNAL(clicked(bool)),this,SLOT(setFltPodt()));
    connect(ui->pushButtonFix,SIGNAL(clicked(bool)),this,SLOT(fixNewOst()));

    updFix();
}

FormFixPodt::~FormFixPodt()
{
    delete ui;
}

void FormFixPodt::updFix()
{
    modelDat->setQuery("select distinct dat from wire_podt_ost order by dat desc");
    if (modelDat->lastError().isValid()){
        QMessageBox::critical(this,"Error",modelDat->lastError().text(),QMessageBox::Cancel);
    }
    if (modelDat->rowCount()){
        ui->comboBoxDat->setCurrentIndex(0);
    } else {
        modelFix->select();
    }
}

void FormFixPodt::updFixData(int index)
{
    if (index>=0){
        QDate date=ui->comboBoxDat->model()->data(ui->comboBoxDat->model()->index(index,0),Qt::EditRole).toDate();
        modelFix->setDefaultValue(0,date);
        modelFix->setFilter(QString("wire_podt_ost.dat = '%1'").arg(date.toString("yyyy-MM-dd")));
        modelFix->select();
    }
}

void FormFixPodt::setFltPodt()
{
    DialogFlt d(tr("подтяжки"),Models::instance()->relPodt);
    d.exec();
}

void FormFixPodt::fixNewOst()
{
    updFix();
}
