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
    ui->comboBoxType->setModel(Models::instance()->relNaklType->model());
    ui->comboBoxType->setModelColumn(Models::instance()->relNaklType->columnDisplay());

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
    Models::instance()->invoiceManager->getInvoice("invoices/wire/workshop/"+QString::number(id_type)+"/"+dat.toString("yyyy-MM-dd"),vid,type,filename,year);
}
