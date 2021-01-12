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

QVariant FormPodtNorn::currentData(int column)
{
    int row=ui->tableViewPodt->currentIndex().row();
    return ui->tableViewPodt->model()->data(ui->tableViewPodt->model()->index(row,column),Qt::EditRole);
}

void FormPodtNorn::upd()
{
    modelPodtProd->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date());
    if (ui->tableViewPodt->model()->rowCount()){
        ui->tableViewPodt->selectRow(0);
    }
}

void FormPodtNorn::updNorm(QModelIndex /*ind*/)
{
    int id_type=currentData(0).toInt();
    int id_provol=currentData(1).toInt();
    int id_src_diam=currentData(2).toInt();
    int id_diam=currentData(3).toInt();
    modelNorm->setDefaultValue(0,id_type);
    modelNorm->setDefaultValue(1,id_provol);
    modelNorm->setDefaultValue(2,id_src_diam);
    modelNorm->setDefaultValue(3,id_diam);
    modelNorm->setFilter(QString("id_podt_type = %1 and id_provol = %2 and id_src_diam = %3 and id_diam = %4").arg(id_type).arg(id_provol).arg(id_src_diam).arg(id_diam));
    modelNorm->select();
}

void FormPodtNorn::copy()
{
    buf.clear();
    for (int i=0; i<modelNorm->rowCount();i++){
        mnorm m;
        m.id_matr=modelNorm->data(modelNorm->index(i,4),Qt::EditRole).toInt();
        m.kvo=modelNorm->data(modelNorm->index(i,5),Qt::EditRole).toDouble();
        m.id_vid=modelNorm->data(modelNorm->index(i,6),Qt::EditRole).toInt();
        buf.push_back(m);
    }
    ui->pushButtonPaste->setEnabled(true);
}

void FormPodtNorn::paste()
{
    int id_type=currentData(0).toInt();
    int id_provol=currentData(1).toInt();
    int id_src_diam=currentData(2).toInt();
    int id_diam=currentData(3).toInt();
    QString err;
    foreach (mnorm m, buf) {
        QSqlQuery query;
        query.prepare("insert into wire_podt_norm (id_podt_type, id_provol, id_src_diam, id_diam, id_matr, kvo, id_vid) values "
                      "(:id_podt_type, :id_provol, :id_src_diam, :id_diam, :id_matr, :kvo, :id_vid)");
        query.bindValue(":id_podt_type",id_type);
        query.bindValue(":id_provol",id_provol);
        query.bindValue(":id_src_diam",id_src_diam);
        query.bindValue(":id_diam",id_diam);
        query.bindValue(":id_matr",m.id_matr);
        query.bindValue(":kvo",m.kvo);
        query.bindValue(":id_vid",m.id_vid);
        if (!query.exec()){
            err=query.lastError().text();
        }
    }
    if (!err.isEmpty()){
        QMessageBox::critical(this,tr("Ошибка"),err,QMessageBox::Ok);
    }
    modelNorm->select();
}
