#include "formreportpodt.h"
#include "ui_formreportpodt.h"

FormReportPodt::FormReportPodt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormReportPodt)
{
    ui->setupUi(this);
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    if (!Rels::instance()->relPodtType->isInital()){
        Rels::instance()->relPodtType->refreshModel();
    }
    ui->comboBoxType->setModel(Rels::instance()->relPodtType->model());
    ui->comboBoxType->setEditable(false);

    modelReport = new ModelReportPodt(this);
    ui->tableView->setModel(modelReport);

    connect(ui->pushButtonUpd, SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this,SLOT(save()));
}

FormReportPodt::~FormReportPodt()
{
    delete ui;
}

void FormReportPodt::upd()
{
    int id_type=ui->comboBoxType->getCurrentData().val.toInt();
    int num=1;
    if (ui->radioButtonMark->isChecked()){
        num=2;
    } else if (ui->radioButtonKat->isChecked()){
        num=3;
    }

    modelReport->refresh(ui->dateEditBeg->date().addDays(-1),ui->dateEditEnd->date(),num,id_type);
    ui->tableView->resizeToContents();
}

void FormReportPodt::save()
{
    ui->tableView->save(tr("Подтяжки с ")+ui->dateEditBeg->date().toString("dd.MM.yy")+tr(" по ")+ui->dateEditEnd->date().toString("dd.MM.yy"));
}

ModelReportPodt::ModelReportPodt(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelReportPodt::refresh(QDate begDate, QDate endDate, int num, int id_type)
{
    this->clear();
    by_Part=(num==1 || num==3);
    if (num==1){
        setQuery("select wp.n_s ||'-'||date_part('year',wp.dat) as part, coalesce(p2.nam,p.nam) as wnam, d.sdim, n.beg, n.pr, n.rs, n.cor, n.ost "
                 "from wire_calc_podt_report('"+ begDate.toString("yyyy.MM.dd")+"','"+endDate.toString("yyyy.MM.dd")+"') as n "
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
            setHeaderData(3, Qt::Horizontal,tr("Нал. на нач."));
            setHeaderData(4, Qt::Horizontal,tr("Приход"));
            setHeaderData(5, Qt::Horizontal,tr("Расход"));
            setHeaderData(6, Qt::Horizontal,tr("Брак"));
            setHeaderData(7, Qt::Horizontal,tr("Ост. на конец"));
        }
    } else if (num==2) {
        setQuery("select coalesce(p2.nam,p.nam) as wnam, d.sdim, sum(n.beg), sum(n.pr), sum(n.rs), sum(n.cor), sum(n.ost) "
                 "from wire_calc_podt_report('"+ begDate.toString("yyyy.MM.dd")+"','"+endDate.toString("yyyy.MM.dd")+"') as n "
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
            setHeaderData(2, Qt::Horizontal,tr("Нал. на нач."));
            setHeaderData(3, Qt::Horizontal,tr("Приход"));
            setHeaderData(4, Qt::Horizontal,tr("Расход"));
            setHeaderData(5, Qt::Horizontal,tr("Брак"));
            setHeaderData(6, Qt::Horizontal,tr("Ост. на конец"));
        }
    } else if (num==3){
        setQuery("select p.nam||' ф'||d2.sdim, coalesce(p2.nam,p.nam) as wnam, d.sdim, sum(n.beg), sum(n.pr), sum(n.rs), sum(n.cor), sum(n.ost) "
                 "from wire_calc_podt_report('"+ begDate.toString("yyyy.MM.dd")+"','"+endDate.toString("yyyy.MM.dd")+"') as n "
                 "inner join wire_podt wp on wp.id=n.id "
                 "inner join prov_buht pb on pb.id=wp.id_buht "
                 "inner join prov_prih pp on pp.id =pb.id_prih "
                 "inner join provol p on p.id = pp.id_pr "
                 "inner join diam d on d.id = wp.id_diam "
                 "inner join diam d2 on d2.id=pp.id_dim "
                 "left join provol p2 on p2.id = p.id_base "
                 "where wp.id_type =  "+QString::number(id_type)+" "
                 "group by wnam, d.sdim, p.nam, d2.sdim "
                 "order by p.nam, d2.sdim, wnam, d.sdim");
        if (lastError().isValid()){
            QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
        } else {
            setHeaderData(0, Qt::Horizontal,tr("Катанка"));
            setHeaderData(1, Qt::Horizontal,tr("Марка проволоки"));
            setHeaderData(2, Qt::Horizontal,tr("Ф"));
            setHeaderData(3, Qt::Horizontal,tr("Нал. на нач."));
            setHeaderData(4, Qt::Horizontal,tr("Приход"));
            setHeaderData(5, Qt::Horizontal,tr("Расход"));
            setHeaderData(6, Qt::Horizontal,tr("Брак"));
            setHeaderData(7, Qt::Horizontal,tr("Ост. на конец"));
        }
    }
}

QVariant ModelReportPodt::data(const QModelIndex &index, int role) const
{
    if (index.row()>=rowCount()|| index.row()<0 || index.column()>=columnCount() || index.column()<0) return QVariant();

    QVariant value;

    if ((index.row()==QSqlQueryModel::rowCount()) && (role==Qt::DisplayRole || role==Qt::EditRole)){
        if (index.column()==0) {
            value= tr("Итого");
        } else if ((index.column()>= 3 && by_Part)||(index.column() >= 2 && !by_Part)) {
            double sum=0.0;
            for (int i=0; i<QSqlQueryModel::rowCount(); i++){
                QModelIndex cs=this->index(i,index.column());
                sum+=(QSqlQueryModel::data(cs).toDouble());
            }
            value=sum;
        } else value=QVariant();

    } else {
        value = (index.row()==QSqlQueryModel::rowCount()) ? QVariant() : QSqlQueryModel::data(index, role);
    }
    switch (role) {
        case Qt::DisplayRole:
        {
            if((index.column() >=3 && by_Part)||(index.column() >= 2 && !by_Part))
                if (value!=0.0){
                    return QLocale().toString(value.toDouble(),'f',2);
                } else {
                    return QVariant();
                }
            else
                return value;
        }

        case Qt::EditRole:
        {
            return value;
        }

        case Qt::TextAlignmentRole:
        {
             if((index.column()>=3 && by_Part)||(index.column() >= 2 && !by_Part))
                 return int(Qt::AlignRight | Qt::AlignVCenter);
             else return int(Qt::AlignLeft | Qt::AlignVCenter);
        }

        case Qt::BackgroundRole:
        {
            double val1= by_Part ? data(this->index(index.row(),3),Qt::EditRole).toDouble() :  data(this->index(index.row(),2),Qt::EditRole).toDouble();
            double val2= by_Part ? data(this->index(index.row(),7),Qt::EditRole).toDouble() :  data(this->index(index.row(),6),Qt::EditRole).toDouble();
            return (val1>=0 && val2>=0) ? value : QVariant(QColor(255,170,170));
        }

    }
    return value;
}

int ModelReportPodt::rowCount(const QModelIndex &parent) const
{
    return (QSqlQueryModel::rowCount(parent)==0)? 0 : QSqlQueryModel::rowCount(parent)+1;
}
