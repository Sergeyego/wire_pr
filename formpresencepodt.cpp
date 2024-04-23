#include "formpresencepodt.h"
#include "ui_formpresencepodt.h"

FormPresencePodt::FormPresencePodt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPresencePodt)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());

    if (!Rels::instance()->relPodtType->isInital()){
        Rels::instance()->relPodtType->refreshModel();
    }
    ui->comboBoxType->setModel(Rels::instance()->relPodtType->model());
    ui->comboBoxType->setEditable(false);

    modelPresence = new ModelPresencePodt(this);
    ui->tableView->setModel(modelPresence);

    connect(ui->pushButtonUpd, SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this,SLOT(save()));
}

FormPresencePodt::~FormPresencePodt()
{
    delete ui;
}

void FormPresencePodt::upd()
{
    int id_type=ui->comboBoxType->getCurrentData().val.toInt();
    modelPresence->refresh(ui->dateEdit->date(),ui->radioButtonPart->isChecked(),id_type);
    ui->tableView->resizeToContents();
}

void FormPresencePodt::save()
{
    ui->tableView->save(QString::fromUtf8("Наличие подтяжек на ")+ui->dateEdit->date().toString("dd.MM.yy"));
}

ModelPresencePodt::ModelPresencePodt(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelPresencePodt::refresh(QDate date, bool bypart, int id_type)
{
    QSqlQueryModel::clear();
    sum=0;
    by_Part=bypart;
    if (by_Part){
        setQuery("select wp.n_s ||'-'||date_part('year',wp.dat) as part, coalesce(p2.nam,p.nam) as wnam, d.sdim, n.ost "
                 "from wire_calc_podt('"+date.toString("yyyy-MM-dd")+"') as n "
                 "inner join wire_podt wp on wp.id=n.id "
                 "inner join prov_buht pb on pb.id=wp.id_buht "
                 "inner join prov_prih pp on pp.id =pb.id_prih "
                 "inner join provol p on p.id = pp.id_pr "
                 "inner join diam d on d.id = wp.id_diam "
                 "left join provol p2 on p2.id = p.id_base "
                 "where wp.id_type = "+QString::number(id_type)+" "
                 "order by wnam, d.sdim, wp.n_s, wp.dat");
        if (lastError().isValid()){
            QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
        } else {
            setHeaderData(0, Qt::Horizontal,tr("Партия полуф."));
            setHeaderData(1, Qt::Horizontal,tr("Марка проволоки"));
            setHeaderData(2, Qt::Horizontal,tr("Ф"));
            setHeaderData(3, Qt::Horizontal,tr("Наличие, кг"));
        }
        colsum=3;
    } else {
        setQuery("select coalesce(p2.nam,p.nam) as wnam, d.sdim, sum(n.ost) "
                 "from wire_calc_podt('"+date.toString("yyyy-MM-dd")+"') as n "
                 "inner join wire_podt wp on wp.id=n.id "
                 "inner join prov_buht pb on pb.id=wp.id_buht "
                 "inner join prov_prih pp on pp.id =pb.id_prih "
                 "inner join provol p on p.id = pp.id_pr "
                 "inner join diam d on d.id = wp.id_diam "
                 "left join provol p2 on p2.id = p.id_base "
                 "where wp.id_type = "+QString::number(id_type)+" "
                 "group by wnam, d.sdim "
                 "order by wnam, d.sdim");
        if (lastError().isValid()){
            QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
        } else {
            setHeaderData(0, Qt::Horizontal,tr("Марка проволоки"));
            setHeaderData(1, Qt::Horizontal,tr("Ф"));
            setHeaderData(2, Qt::Horizontal,tr("Наличие, кг"));
        }
        colsum=2;
    }
    for (int i=0; i<QSqlQueryModel::rowCount(); i++){
        QModelIndex cs=this->index(i,colsum);
        sum+=(QSqlQueryModel::data(cs,Qt::EditRole).toDouble());
    }
}

QVariant ModelPresencePodt::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    QVariant value;

    if ((index.row()==QSqlQueryModel::rowCount()) && (role==Qt::DisplayRole || role==Qt::EditRole)){
        if (index.column()==0) {
            value= tr("Итого");
        } else if (index.column() == colsum) {
            value=sum;
        } else value=QVariant();

    } else {
        value = (index.row()==QSqlQueryModel::rowCount()) ? QVariant() : QSqlQueryModel::data(index, role);
    }
    switch (role) {
    case Qt::DisplayRole:
    {
        if(index.column() == colsum)
            return QLocale().toString(value.toDouble(),'f',2);
        else
            return value;
    }

    case Qt::EditRole:
    {
        return value;
    }

    case Qt::TextAlignmentRole:
    {
        if(index.column() == colsum)
            return int(Qt::AlignRight | Qt::AlignVCenter);
        else return int(Qt::AlignLeft | Qt::AlignVCenter);
    }

    case Qt::BackgroundRole:
    {
        double val=data(this->index(index.row(),colsum),Qt::EditRole).toDouble();
        return val>=0 ? value : QVariant(QColor(255,170,170));
    }
    }
    return value;
}

int ModelPresencePodt::rowCount(const QModelIndex &parent) const
{
     return (QSqlQueryModel::rowCount(parent)==0)? 0 : QSqlQueryModel::rowCount(parent)+1;
}
