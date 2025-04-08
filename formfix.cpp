#include "formfix.h"
#include "ui_formfix.h"

FormFix::FormFix(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFix)
{
    ui->setupUi(this);

    modelDat = new QSqlQueryModel(this);
    ui->comboBoxDate->setModel(modelDat);

    modelFix = new DbTableModel("wire_ost",this);
    modelFix->addColumn("dat",tr("Дата"));
    modelFix->addColumn("id_part",tr("Партия"),Rels::instance()->relWirePart);
    modelFix->addColumn("kvo",tr("Кол-во, кг"));
    modelFix->setSuffix("inner join wire_parti_m on wire_parti_m.id = wire_parti.id_m "
                        "inner join provol on provol.id = wire_parti_m.id_provol "
                        "inner join diam on diam.id = wire_parti_m.id_diam "
                        "inner join wire_pack_kind on wire_pack_kind.id = wire_parti.id_pack");
    modelFix->setSort("provol.nam, diam.sdim, wire_pack_kind.short, wire_parti_m.n_s, date_part('year',wire_parti_m.dat)");

    ui->dateEditFix->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()));
    modelFix->setDefaultValue(0,ui->dateEditFix->date());

    ui->tableViewOst->setModel(modelFix);
    ui->tableViewOst->setColumnHidden(0,true);
    ui->tableViewOst->setColumnWidth(1,400);
    ui->tableViewOst->setColumnWidth(2,80);

    connect(ui->comboBoxDate,SIGNAL(currentIndexChanged(int)),this,SLOT(updFixData(int)));
    connect(ui->pushButtonFix,SIGNAL(clicked(bool)),this,SLOT(fixNewOst()));
    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(updFix()));

    updFix();
}

FormFix::~FormFix()
{
    delete ui;
}

void FormFix::updFix()
{
    modelDat->setQuery("select distinct dat from wire_ost order by dat desc");
    if (modelDat->lastError().isValid()){
        QMessageBox::critical(this,"Error",modelDat->lastError().text(),QMessageBox::Cancel);
    }
    if (modelDat->rowCount()){
        ui->comboBoxDate->setCurrentIndex(0);
    } else {
        modelFix->select();
    }
}

void FormFix::updFixData(int index)
{
    if (index>=0){
        QDate date=ui->comboBoxDate->model()->data(ui->comboBoxDate->model()->index(index,0),Qt::EditRole).toDate();
        modelFix->setDefaultValue(0,date);
        modelFix->setFilter(QString("wire_ost.dat = '%1'").arg(date.toString("yyyy-MM-dd")));
        modelFix->select();
    }
}

void FormFix::fixNewOst()
{
    QDate d=ui->dateEditFix->date();
    if (d.day()!=d.daysInMonth()){
        QMessageBox::information(this,tr("Внимание"),tr("Дата должна быть последним числом месяца!"),QMessageBox::Cancel);
    } else {
        int n=QMessageBox::question(this,tr("Подтвердите действие"),tr("Зафиксировать остатки на ")+d.toString("dd.MM.yy")+tr("?"),QMessageBox::Yes,QMessageBox::No);
        if (n==QMessageBox::Yes){
            delOst(d);
            QSqlQuery query;
            query.prepare("insert into wire_ost (dat, id_part, kvo) (select :d1, p.id_wparti, p.st from wire_calc_cex_new(:d2) as p where p.st<>0)");
            query.bindValue(":d1",d);
            query.bindValue(":d2",d);
            if (!query.exec()){
                QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
            } else {
                updFix();
            }
        }
    }
}

void FormFix::delOst(QDate date)
{
    QSqlQuery query;
    query.prepare("delete from wire_ost where dat = :d ");
    query.bindValue(":d",date);
    if (!query.exec()){
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        updFix();
    }
}
