#include "formperepack.h"
#include "ui_formperepack.h"

FormPerePack::FormPerePack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPerePack)
{
    ui->setupUi(this);

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

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
    push->addEmptyLock(ui->tableViewPerePack);

    modelPerepack = new ModelPerepack(this);
    ui->tableViewPerePack->setModel(modelPerepack);
    ui->tableViewPerePack->setColumnHidden(0,true);
    ui->tableViewPerePack->setColumnHidden(1,true);
    ui->tableViewPerePack->setColumnWidth(2,320);
    ui->tableViewPerePack->setColumnWidth(3,320);
    ui->tableViewPerePack->setColumnWidth(4,80);

    connect(ui->cmdUpdate,SIGNAL(clicked(bool)),this,SLOT(refresh()));
    connect(push,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshData(int)));
    connect(ui->cmdNakl,SIGNAL(clicked(bool)),this,SLOT(printNakl()));

    refresh();
}

FormPerePack::~FormPerePack()
{
    delete ui;
}

void FormPerePack::refresh()
{
    if (sender()==ui->cmdUpdate){
        modelPerepack->refreshRelsModel();
    }
    modelNakl->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date());
}

void FormPerePack::refreshData(int row)
{
    int id_nakl=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,0),Qt::EditRole).toInt();
    modelPerepack->refresh(id_nakl);
}


void FormPerePack::printNakl()
{
    int row=ui->tableViewNakl->currentIndex().row();
    int id_nakl=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,0),Qt::EditRole).toInt();
    QString vid=tr("Проволока");
    QString type=tr("Переупаковка");
    QString filename=type.toUpper()+"_"+ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,1),Qt::EditRole).toString();
    int year=ui->tableViewNakl->model()->data(ui->tableViewNakl->model()->index(row,2),Qt::EditRole).toDate().year();
    Rels::instance()->invoiceManager->getInvoice("invoices/wire/perepack/"+QString::number(id_nakl),vid,type,filename,year);
}

ModelNakl::ModelNakl(QObject *parent) : DbTableModel("wire_perepack_nakl",parent)
{
    addColumn("id",tr("id"));
    addColumn("num",tr("Номер"));
    addColumn("dat",tr("Дата"));
    setSort("wire_perepack_nakl.dat, wire_perepack_nakl.num");
}

void ModelNakl::refresh(QDate beg, QDate end)
{
    setDefaultValue(1,QVariant());
    setFilter("wire_perepack_nakl.dat between '"+beg.toString("yyyy.MM.dd")+"' and '"+end.toString("yyyy.MM.dd")+"'");
    select();
}

bool ModelNakl::insertRow(int row, const QModelIndex &parent)
{
    int old_num=0;
    if (rowCount()>0) old_num=this->data(this->index(rowCount()-1,1),Qt::EditRole).toInt();
    QString num = QString::number(old_num+1);
    num=num.rightJustified(4,'0',true);
    setDefaultValue(1,num);
    setDefaultValue(2,QDate::currentDate());
    return DbTableModel::insertRow(row,parent);
}


ModelPerepack::ModelPerepack(QObject *parent) : DbTableModel("wire_perepack",parent)
{
    addColumn("id",tr("id"));
    addColumn("id_nakl",tr("id_nakl"));
    addColumn("id_wpartisrc",tr("Из партии"),Rels::instance()->relWirePart);
    addColumn("id_wpartires",tr("В партию"),Rels::instance()->relWirePartNew);
    addColumn("m_netto",tr("Масса, кг"));
    setDecimals(4,2);
}

void ModelPerepack::refresh(int id_nakl)
{
    setFilter("wire_perepack.id_nakl = "+QString::number(id_nakl));
    setDefaultValue(1,id_nakl);
    select();
}

bool ModelPerepack::submit()
{
    bool ok=false;
    if (this->isEdt()){
        int id_nakl=this->data(this->index(currentEdtRow(),1),Qt::EditRole).toInt();
        int id_part=this->data(this->index(currentEdtRow(),2),Qt::EditRole).toInt();
        double kvo=this->data(this->index(currentEdtRow(),4),Qt::EditRole).toDouble();
        QSqlQuery query;
        query.prepare("select st, (select dat from wire_perepack_nakl where id= :id_n ) "
                      "from wire_calc_cex_new((select dat from wire_perepack_nakl where id= :id_nakl)) "
                      "where id_wparti = :id_part ");
        query.bindValue(":id_n",id_nakl);
        query.bindValue(":id_nakl",id_nakl);
        query.bindValue(":id_part",id_part);
        if (query.exec()) {
            double m=0;
            QDate date;
            while (query.next()){
                m=query.value(0).toDouble();
                date=query.value(1).toDate();
            }
            if (m>=kvo){
                ok=DbTableModel::submit();
            } else {
                QMessageBox::critical(NULL,tr("Ошибка"),tr("В цехе на ")+date.toString("dd.MM.yy")+tr(" числится только ")+
                                      QLocale().toString(m,'f',2)+tr(" кг. Вы не можете перепаковать эту партию. Проверьте поступление."),QMessageBox::Cancel);
            }
        } else {
            QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
        }
    } else {
        ok=DbTableModel::submit();
    }
    return ok;
}
