#include "formnakl.h"
#include "ui_formnakl.h"

FormNakl::FormNakl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormNakl)
{
    ui->setupUi(this);
    loadSettings();

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());
    if (!Rels::instance()->relNaklType->isInital()){
        Rels::instance()->relNaklType->refreshModel();
    }
    ui->comboBoxType->setModel(Rels::instance()->relNaklType->model());
    ui->comboBoxType->setEditable(false);

    modelNakl = new ModelNaklGen(this);
    modelNaklCont = new ModelNaklGenCont(this);

    ui->tableViewNakl->setModel(modelNakl);

    ui->tableViewItem->setModel(modelNaklCont);

    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(refreshNakl()));
    connect(ui->comboBoxType,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshNakl()));
    connect(ui->tableViewNakl->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(refreshCont(QModelIndex)));
    connect(ui->cmdNakl,SIGNAL(clicked(bool)),this,SLOT(printNakl()));
    connect(modelNaklCont,SIGNAL(sigSum(QString)),ui->labelItogo,SLOT(setText(QString)));

    refreshNakl();
}

FormNakl::~FormNakl()
{
    saveSettings();
    delete ui;
}

void FormNakl::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    this->ui->splitter->restoreState(settings.value("nakl_splitter_width").toByteArray());
}

void FormNakl::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    settings.setValue("nakl_splitter_width",ui->splitter->saveState());
}

void FormNakl::refreshNakl()
{
    if (sender()==ui->cmdUpd){
        Rels::instance()->relNaklType->refreshModel();
    }
    int r=ui->comboBoxType->currentIndex();
    int id_type=ui->comboBoxType->model()->data(ui->comboBoxType->model()->index(r,0),Qt::EditRole).toInt();
    modelNakl->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),id_type);
    if (ui->tableViewNakl->model()->rowCount()){
        ui->tableViewNakl->setColumnHidden(2,true);
        ui->tableViewNakl->setColumnWidth(0,100);
        ui->tableViewNakl->setColumnWidth(1,100);
        ui->tableViewNakl->selectRow(modelNakl->rowCount()-1);
    } else {
        modelNaklCont->refresh(QDate(),-1);
    }
}

void FormNakl::refreshCont(QModelIndex index)
{
    QDate dat=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(index.row(),1),Qt::EditRole).toDate();
    int id_type=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(index.row(),2),Qt::EditRole).toInt();
    modelNaklCont->refresh(dat,id_type);
    if (ui->tableViewItem->model()->rowCount()){
        ui->tableViewItem->setColumnWidth(0,200);
        ui->tableViewItem->setColumnWidth(1,60);
        ui->tableViewItem->setColumnWidth(2,80);
        ui->tableViewItem->setColumnWidth(3,100);
        ui->tableViewItem->setColumnWidth(4,100);
    }
}

void FormNakl::printNakl()
{
    int row=ui->tableViewNakl->currentIndex().row();
    QDate dat=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,1),Qt::EditRole).toDate();
    int id_type=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,2),Qt::EditRole).toInt();
    QString vid=tr("Проволока");
    QString type=tr("Цех");
    QString filename=ui->comboBoxType->currentText().toUpper()+"_"+ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,0),Qt::EditRole).toString();
    int year=dat.year();
    Rels::instance()->invoiceManager->getInvoice("invoices/wire/workshop/"+QString::number(id_type)+"/"+dat.toString("yyyy-MM-dd"),vid,type,filename,year);
}

ModelNaklGen::ModelNaklGen(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelNaklGen::refresh(QDate beg, QDate end, int id_type)
{
    QSqlQuery query;
    query.prepare("select distinct date_part('doy',dat), dat, id_type "
                  "from wire_in_cex_data "
                  "where dat between :d1 and :d2 and id_type = :t "
                  "order by dat");
    query.bindValue(":d1",beg);
    query.bindValue(":d2",end);
    query.bindValue(":t",id_type);
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

QVariant ModelNaklGenCont::data(const QModelIndex &item, int role) const
{
    if (role==Qt::DisplayRole && item.column()==4){
        return QLocale().toString(QSqlQueryModel::data(item,role).toDouble(),'f',2);
    }
    if (role==Qt::TextAlignmentRole && item.column()==4){
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    return QSqlQueryModel::data(item,role);
}

ModelNaklGenCont::ModelNaklGenCont(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelNaklGenCont::refresh(QDate dat, int id_type)
{
    double sum=0.0;
    QSqlQuery query;
    query.prepare("select prg.nam, dg.sdim, k.short, m.n_s||'-'||date_part('year',m.dat), i.m_netto as rac "
                  "from wire_in_cex_data as i "
                  "inner join wire_parti as p on p.id=i.id_wparti "
                  "inner join wire_parti_m as m on p.id_m=m.id "
                  "inner join provol as prg on m.id_provol=prg.id "
                  "inner join diam as dg on m.id_diam=dg.id "
                  "inner join wire_pack_kind as k on p.id_pack=k.id "
                  "where (i.id_type = :t) and i.dat= :d "
                  "order by i.id");
    query.bindValue(":t",id_type);
    query.bindValue(":d",dat);
    query.exec();
    this->setQuery(query);
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(0, Qt::Horizontal,tr("Марка"));
        setHeaderData(1, Qt::Horizontal,tr("Диаметр"));
        setHeaderData(2, Qt::Horizontal,tr("Носитель"));
        setHeaderData(3, Qt::Horizontal,tr("Партия"));
        setHeaderData(4, Qt::Horizontal,tr("Масса"));
        for (int i=0; i<rowCount(); i++){
            sum+=this->data(this->index(i,4),Qt::EditRole).toDouble();
        }
    }
    emit sigSum(tr("Итого по накладной ")+QLocale().toString(sum,'f',2)+tr(" кг"));
}
