#include "formperepack.h"
#include "ui_formperepack.h"

FormPerePack::FormPerePack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPerePack)
{
    ui->setupUi(this);

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    ui->comboBoxSelPart->addItem(tr("начиная с текущего года"));
    ui->comboBoxSelPart->addItem(tr("начиная с прошлого года"));
    ui->comboBoxSelPart->addItem(tr("за всё время"));

    modelNakl = new ModelNakl(this);
    ui->tableViewNakl->setModel(modelNakl);
    ui->tableViewNakl->verticalHeader()->setDefaultSectionSize(ui->tableViewNakl->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewNakl->setColumnHidden(0,true);

    push = new DbMapper(ui->tableViewNakl,this);
    ui->horizontalLayoutBtn->insertWidget(0,push);
    push->addMapping(ui->lineEditNum,1);
    push->addMapping(ui->dateEdit,2);
    push->addLock(ui->dateEditBeg);
    push->addLock(ui->dateEditEnd);
    push->addLock(ui->cmdUpdate);
    push->addLock(ui->cmdUpdPart);
    push->addLock(ui->comboBoxSelPart);
    push->addEmptyLock(ui->tableViewPerePack);

    modelPerepack = new ModelPerepack(this);
    ui->tableViewPerePack->setModel(modelPerepack);
    ui->tableViewPerePack->setColumnHidden(0,true);
    ui->tableViewPerePack->setColumnHidden(1,true);
    ui->tableViewPerePack->setColumnWidth(2,300);
    ui->tableViewPerePack->setColumnWidth(3,300);
    ui->tableViewPerePack->setColumnWidth(4,80);

    connect(ui->cmdUpdate,SIGNAL(clicked(bool)),this,SLOT(refresh()));
    connect(push,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshData(int)));
    connect(ui->cmdUpdPart,SIGNAL(clicked(bool)),modelPerepack,SLOT(refreshPart()));
    connect(ui->comboBoxSelPart,SIGNAL(currentIndexChanged(int)),this,SLOT(setPartFilter(int)));
    connect(ui->cmdNakl,SIGNAL(clicked(bool)),this,SLOT(printNakl()));

    ui->comboBoxSelPart->setCurrentIndex(1);

    refresh();
}

FormPerePack::~FormPerePack()
{
    delete ui;
}

void FormPerePack::refresh()
{
    modelNakl->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date());
}

void FormPerePack::refreshData(int row)
{
    int id_nakl=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,0),Qt::EditRole).toInt();
    modelPerepack->refresh(id_nakl);
}

void FormPerePack::setPartFilter(int index)
{
    int year=QDate::currentDate().year();
    QString pattern=QString();
    if (index==0){
        pattern=QString::number(year);
    } else if (index==1){
         pattern=QString::number(year-1)+"|"+QString::number(year);
    }
    Models::instance()->relWirePart->proxyModel()->setFilterRegExp(pattern);
}

void FormPerePack::printNakl()
{
    int row=ui->tableViewNakl->currentIndex().row();
    int id_nakl=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,0),Qt::EditRole).toInt();
    QString vid=tr("Проволока");
    QString type=tr("Переупаковка");
    QString filename=type.toUpper()+"_"+ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,1),Qt::EditRole).toString();
    int year=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,2),Qt::EditRole).toDate().year();
    Models::instance()->invoiceManager->getInvoice("invoices/wire/perepack/"+QString::number(id_nakl),vid,type,filename,year);
}

