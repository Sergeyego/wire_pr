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

    modelType = new QSqlQueryModel(this);
    modelType->setQuery("select id, nam from wire_podt_op order by id");
    if (modelType->lastError().isValid()){
        QMessageBox::critical(this,"Error",modelType->lastError().text(),QMessageBox::Cancel);
    } else {
        ui->comboBoxType->setModel(modelType);
        ui->comboBoxType->setModelColumn(1);
    }

    ui->comboBoxVid->setModel(Models::instance()->relPodtType->model());
    ui->comboBoxVid->setModelColumn(1);

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
    int id_type=ui->comboBoxType->model()->data(ui->comboBoxType->model()->index(ui->comboBoxType->currentIndex(),0),Qt::EditRole).toInt();
    int id_vid=ui->comboBoxVid->model()->data(ui->comboBoxVid->model()->index(ui->comboBoxVid->currentIndex(),0),Qt::EditRole).toInt();
    modelNaklPodt->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),id_type,id_vid);
    if (modelNaklPodt->rowCount()){
        ui->tableViewNakl->setColumnHidden(2,true);
        ui->tableViewNakl->setColumnHidden(3,true);
        ui->tableViewNakl->setColumnWidth(0,100);
        ui->tableViewNakl->setColumnWidth(1,100);
        ui->tableViewNakl->selectRow(0);
    } else {
        modelNaklPodtCont->refresh(QDate(),-1,-1);
    }
}

void FormNaklPodt::refreshCont(QModelIndex index)
{
    QDate dat=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(index.row(),1),Qt::EditRole).toDate();
    int id_type=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(index.row(),2),Qt::EditRole).toInt();
    int id_podt_type=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(index.row(),3),Qt::EditRole).toInt();
    //qDebug()<<id_type;
    modelNaklPodtCont->refresh(dat,id_type,id_podt_type);
    ui->tableViewCont->setColumnWidth(1,120);
    ui->tableViewCont->setColumnWidth(2,100);
    ui->tableViewCont->setColumnWidth(3,100);
    ui->tableViewCont->setColumnWidth(4,100);
}

void FormNaklPodt::printNakl()
{
    int row=ui->tableViewNakl->currentIndex().row();
    QDate dat=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,1),Qt::EditRole).toDate();
    int id_type=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,2),Qt::EditRole).toInt();
    int id_vid=ui->comboBoxVid->model()->data(ui->comboBoxVid->model()->index(ui->comboBoxVid->currentIndex(),0),Qt::EditRole).toInt();
    QTcpSocket tcpSocket;
    tcpSocket.connectToHost("127.0.0.1", 5555);
    if (tcpSocket.waitForConnected()) {
        tcpSocket.write((QString("%1:%2:%3:%4-%5").arg(1).arg(3).arg(dat.toString("dd.MM.yyyy")).arg(id_type).arg(id_vid)).toLocal8Bit().data());
        tcpSocket.waitForBytesWritten();
        tcpSocket.disconnectFromHost();
    } else {
        QMessageBox::critical(this,tr("Ошибка"),tcpSocket.errorString(),QMessageBox::Ok);
    }
}
