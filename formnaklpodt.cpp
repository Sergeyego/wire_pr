#include "formnaklpodt.h"
#include "ui_formnaklpodt.h"

FormNaklPodt::FormNaklPodt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormNaklPodt)
{
    ui->setupUi(this);
    loadSettings();

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    if (!Rels::instance()->relPodtOp->isInital()){
        Rels::instance()->relPodtOp->refreshModel();
    }
    ui->comboBoxType->setModel(Rels::instance()->relPodtOp->model());
    ui->comboBoxType->setEditable(false);

    if (!Rels::instance()->relPodtType->isInital()){
        Rels::instance()->relPodtType->refreshModel();
    }
    ui->comboBoxVid->setModel(Rels::instance()->relPodtType->model());
    ui->comboBoxVid->setEditable(false);

    modelNaklPodt = new ModelNaklPodt(this);
    ui->tableViewNakl->setModel(modelNaklPodt);

    modelNaklPodtCont = new ModelNaklPodtCont(this);
    ui->tableViewCont->setModel(modelNaklPodtCont);

    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(refreshNakl()));
    connect(ui->comboBoxType,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshNakl()));
    connect(ui->comboBoxVid,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshNakl()));
    connect(ui->tableViewNakl->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(refreshCont(QModelIndex)));
    connect(ui->pushButtonPrint,SIGNAL(clicked(bool)),this,SLOT(printNakl()));
    connect(modelNaklPodtCont,SIGNAL(sigSum(QString)),ui->labelItogo,SLOT(setText(QString)));

    refreshNakl();
}

FormNaklPodt::~FormNaklPodt()
{
    saveSettings();
    delete ui;
}

void FormNaklPodt::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    this->ui->splitter->restoreState(settings.value("nakl_podt_splitter_width").toByteArray());
}

void FormNaklPodt::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    settings.setValue("nakl_podt_splitter_width",ui->splitter->saveState());
}

void FormNaklPodt::refreshNakl()
{
    int id_type=ui->comboBoxType->getCurrentData().val.toInt();
    int id_vid=ui->comboBoxVid->getCurrentData().val.toInt();
    modelNaklPodt->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),id_type,id_vid);
    if (modelNaklPodt->rowCount()){
        ui->tableViewNakl->setColumnHidden(2,true);
        ui->tableViewNakl->setColumnHidden(3,true);
        ui->tableViewNakl->setColumnWidth(0,100);
        ui->tableViewNakl->setColumnWidth(1,100);
        ui->tableViewNakl->selectRow(ui->tableViewNakl->model()->rowCount()-1);
    } else {
        modelNaklPodtCont->refresh(QDate(),-1,-1);
    }
}

void FormNaklPodt::refreshCont(QModelIndex index)
{
    QDate dat=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(index.row(),1),Qt::EditRole).toDate();
    int id_type=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(index.row(),2),Qt::EditRole).toInt();
    int id_podt_type=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(index.row(),3),Qt::EditRole).toInt();
    modelNaklPodtCont->refresh(dat,id_type,id_podt_type);
    if (ui->tableViewCont->model()->rowCount()){
        ui->tableViewCont->setColumnWidth(1,120);
        ui->tableViewCont->setColumnWidth(2,100);
        ui->tableViewCont->setColumnWidth(3,100);
        ui->tableViewCont->setColumnWidth(4,100);
    }
}

void FormNaklPodt::printNakl()
{
    int row=ui->tableViewNakl->currentIndex().row();
    QDate dat=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,1),Qt::EditRole).toDate();
    int id_type=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,2),Qt::EditRole).toInt();
    int id_vid=ui->comboBoxVid->model()->data(ui->comboBoxVid->model()->index(ui->comboBoxVid->currentIndex(),0),Qt::EditRole).toInt();
    QString vid=tr("Проволока");
    QString type=tr("Подтяжка");
    QString filename=ui->comboBoxVid->currentText().toUpper()+"_"+ui->comboBoxType->currentText().toUpper()+"_"+ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,0),Qt::EditRole).toString();
    int year=dat.year();
    Rels::instance()->invoiceManager->getInvoice("invoices/wire/semifinished/"+QString::number(id_vid)+"/"+QString::number(id_type)+"/"+dat.toString("yyyy-MM-dd"),vid,type,filename,year);
}

ModelNaklPodt::ModelNaklPodt(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelNaklPodt::refresh(QDate beg, QDate end, int id_type, int id_podt_type)
{
    QSqlQuery query;
    query.prepare("select distinct (select count(distinct wpc2.dat) from wire_podt_cex wpc2 "
                  "inner join wire_podt as wp2 on wp2.id=wpc2.id_podt "
                  "where date_part('year',wpc2.dat) = date_part('year',wpc.dat) and wpc2.dat<=wpc.dat and wpc2.id_op = wpc.id_op and wp2.id_type = wp.id_type "
                  "), wpc.dat, wpc.id_op, wp.id_type "
                  "from wire_podt_cex as wpc "
                  "inner join wire_podt as wp on wp.id=wpc.id_podt "
                  "where wpc.dat between :d1 and :d2 and wpc.id_op = :t and wp.id_type = :pt "
                  "order by wpc.dat");
    query.bindValue(":d1",beg);
    query.bindValue(":d2",end);
    query.bindValue(":t",id_type);
    query.bindValue(":pt",id_podt_type);
    query.exec();
    this->setQuery(query);
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(0, Qt::Horizontal,tr("Номер"));
        setHeaderData(1, Qt::Horizontal,tr("Дата"));
        setHeaderData(2, Qt::Horizontal,tr("Тип"));
    }
}

ModelNaklPodtCont::ModelNaklPodtCont(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelNaklPodtCont::refresh(QDate dat, int id_type, int id_podt_type)
{
    double sum=0.0;
    QSqlQuery query;
    query.prepare("select coalesce(P2.nam, p.nam), d.sdim, wp.n_s, wpc.kvo "
                  "from wire_podt_cex wpc "
                  "inner join wire_podt wp on wp.id =wpc.id_podt "
                  "inner join prov_buht pb on pb.id=wp.id_buht "
                  "inner join prov_prih pp on pp.id =pb.id_prih "
                  "inner join provol p on p.id = pp.id_pr "
                  "inner join diam d on d.id = wp.id_diam "
                  "left join provol p2 on p2.id = p.id_base "
                  "where wpc.id_op = :op and wp.id_type = :t and wpc.dat = :d ");
    query.bindValue(":op",id_type);
    query.bindValue(":t",id_podt_type);
    query.bindValue(":d",dat);
    query.exec();
    this->setQuery(query);
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(0, Qt::Horizontal,tr("Марка"));
        setHeaderData(1, Qt::Horizontal,tr("Диаметр"));
        setHeaderData(2, Qt::Horizontal,tr("Партия"));
        setHeaderData(3, Qt::Horizontal,tr("Масса"));
        for (int i=0; i<rowCount(); i++){
            sum+=this->data(this->index(i,3),Qt::EditRole).toDouble();
        }
    }
    emit sigSum(tr("Итого по накладной ")+QLocale().toString(sum,'f',2)+tr(" кг"));
}

QVariant ModelNaklPodtCont::data(const QModelIndex &item, int role) const
{
    if (role==Qt::DisplayRole && item.column()==3){
        return QLocale().toString(QSqlQueryModel::data(item,role).toDouble(),'f',2);
    }
    if (role==Qt::TextAlignmentRole && item.column()==3){
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    return QSqlQueryModel::data(item,role);
}
