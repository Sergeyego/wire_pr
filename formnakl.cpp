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

    refreshNakl();
    ui->tableViewNakl->setModel(modelNakl);
    ui->tableViewNakl->verticalHeader()->setDefaultSectionSize(ui->tableViewNakl->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewNakl->setColumnHidden(2,true);
    ui->tableViewNakl->setColumnWidth(0,100);
    ui->tableViewNakl->setColumnWidth(1,200);

    modelNaklCont->refresh(QDate(),-1);
    ui->tableViewItem->setModel(modelNaklCont);
    ui->tableViewItem->verticalHeader()->setDefaultSectionSize(ui->tableViewItem->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewItem->setColumnWidth(0,200);
    ui->tableViewItem->setColumnWidth(1,60);
    ui->tableViewItem->setColumnWidth(2,80);
    ui->tableViewItem->setColumnWidth(3,100);
    ui->tableViewItem->setColumnWidth(4,100);

    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(refreshNakl()));
    connect(ui->comboBoxType,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshNakl()));
    connect(ui->tableViewNakl->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(refreshCont(QModelIndex)));
    connect(ui->cmdNakl,SIGNAL(clicked(bool)),this,SLOT(printNakl()));
    connect(modelNaklCont,SIGNAL(sigSum(QString)),ui->labelItogo,SLOT(setText(QString)));

    if (modelNakl->rowCount()){
        ui->tableViewNakl->selectRow(0);
    }
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
    if (modelNakl->rowCount()){
        ui->tableViewNakl->selectRow(0);
    } else {
        modelNaklCont->refresh(QDate(),-1);
    }
}

void FormNakl::refreshCont(QModelIndex index)
{
    QDate dat=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(index.row(),1),Qt::EditRole).toDate();
    int id_type=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(index.row(),2),Qt::EditRole).toInt();
    modelNaklCont->refresh(dat,id_type);
}

void FormNakl::printNakl()
{
    int row=ui->tableViewNakl->currentIndex().row();
    QDate dat=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,1),Qt::EditRole).toDate();
    int id_type=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,2),Qt::EditRole).toInt();
    QTcpSocket tcpSocket;
    tcpSocket.connectToHost("127.0.0.1", 5555);
    if (tcpSocket.waitForConnected()) {
        tcpSocket.write((QString("%1:%2:%3:%4").arg(1).arg(0).arg(dat.toString("dd.MM.yyyy")).arg(id_type)).toLocal8Bit().data());
        tcpSocket.waitForBytesWritten();
        tcpSocket.disconnectFromHost();
    } else {
        QMessageBox::critical(this,tr("Ошибка"),tcpSocket.errorString(),QMessageBox::Ok);
    }
}
