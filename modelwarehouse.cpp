#include "modelwarehouse.h"
#include <QMessageBox>
#include <QDate>
#include <QLocale>

ModelWarehouse::ModelWarehouse(QObject *parent) :
    QSqlQueryModel(parent)
{
    notZero=true;
}

void ModelWarehouse::refresh()
{
    QString nz= notZero? "where c.st<>0 or s.st<>0 " : "";
    QString dat=date.toString("yyyy-MM-dd");
    setQuery("select c.id_wparti, m.n_s ||'-'|| date_part('year',m.dat), pr.nam, d.sdim, pk.short, c.st as cex, s.st as sklad "
             "from wire_calc_cex_new('"+dat+"') as c "
             "inner join wire_calc_stock('"+dat+"') as s on c.id_wparti=s.id_wparti "
             "inner join wire_parti as p on c.id_wparti=p.id "
             "inner join wire_parti_m as m on p.id_m=m.id "
             "inner join provol as pr on m.id_provol=pr.id "
             "inner join diam as d on m.id_diam=d.id "
             "inner join wire_pack_kind as pk on p.id_pack=pk.id "+
             nz+"order by p.iyear, p.n_s");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Партия"));
        setHeaderData(2, Qt::Horizontal,tr("Марка"));
        setHeaderData(3, Qt::Horizontal,tr("Ф"));
        setHeaderData(4, Qt::Horizontal,tr("Намотка"));
        setHeaderData(5, Qt::Horizontal,tr("В цехе, кг"));
        setHeaderData(6, Qt::Horizontal,tr("На скл., кг"));
    }
}

void ModelWarehouse::setNotZero(bool val)
{
    notZero=val;
}

QVariant ModelWarehouse::data(const QModelIndex &item, int role) const
{
    if (role==Qt::BackgroundRole){
        double val1=QSqlQueryModel::data(index(item.row(),5),Qt::DisplayRole).toDouble();
        double val2=QSqlQueryModel::data(index(item.row(),6),Qt::DisplayRole).toDouble();
        if (val1<0 || val2<0) return QVariant(QColor(255,170,170));
        if (val1>0 || val2>0) return QVariant(QColor(170,255,170));
    }
    if (item.column()==5 || item.column()==6){
        double val=QSqlQueryModel::data(item,Qt::DisplayRole).toDouble();
        if (role==Qt::TextAlignmentRole)
            return int(Qt::AlignRight | Qt::AlignVCenter);
        if (role==Qt::DisplayRole) return QLocale().toString(val,'f',2);
    }
    return QSqlQueryModel::data(item,role);
}

void ModelWarehouse::setDate(QDate d)
{
    date=d;
}


ModelCex::ModelCex(QObject *parent):
    QSqlQueryModel(parent)
{
    refresh(-1,QDate::currentDate());
}

QVariant ModelCex::data(const QModelIndex &item, int role) const
{
    if (item.column()==2){
        if (role==Qt::TextAlignmentRole) return int(Qt::AlignRight | Qt::AlignVCenter);
        if (role==Qt::DisplayRole) return QLocale().toString(QSqlQueryModel::data(item,role).toDouble(),'f',2);
    }
    if (item.column()==0 && role==Qt::DisplayRole)
        return QSqlQueryModel::data(item,role).toDate().toString("dd.MM.yy");
    return QSqlQueryModel::data(item,role);
}

void ModelCex::refresh(int id_part, QDate date)
{
    QString str_part=QString::number(id_part);
    QString str_date="'"+date.toString("yyyy-MM-dd")+"'";
    setQuery("(select d.dat, t.nam, d.m_netto from wire_in_cex_data as d "
             "inner join wire_in_cex_type as t on t.id=d.id_type "
             "where d.id_wparti= "+str_part+" and d.dat<= "+str_date+" ) "
             "union "
             "(select n.dat, "+tr("'Перепаковка в партию '") + " || m.n_s ||'-'||date_part('year',m.dat) as nam, p.m_netto from wire_perepack as p "
             "inner join wire_perepack_nakl as n on p.id_nakl=n.id "
             "inner join wire_parti as wp on p.id_wpartires=wp.id "
             "inner join wire_parti_m as m on wp.id_m=m.id "
             "where p.id_wpartisrc= "+str_part+" and n.dat<= "+str_date+" ) "
             "union "
             "(select n.dat, "+tr("'Перепаковка из парти '") + " || m.n_s ||'-'||date_part('year',m.dat) as nam, p.m_netto from wire_perepack as p "
             "inner join wire_perepack_nakl as n on p.id_nakl=n.id "
             "inner join wire_parti as wp on p.id_wpartisrc=wp.id "
             "inner join wire_parti_m as m on wp.id_m=m.id "
             "where p.id_wpartires= "+str_part+" and n.dat<= "+str_date+" ) "
             "order by dat, nam");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(0, Qt::Horizontal,tr("Дата"));
        setHeaderData(1, Qt::Horizontal,tr("Операция"));
        setHeaderData(2, Qt::Horizontal,tr("Масса, кг"));
    }
}


ModelStock::ModelStock(QObject *parent):
    QSqlQueryModel(parent)
{
    refresh(-1,QDate::currentDate());
}

QVariant ModelStock::data(const QModelIndex &item, int role) const
{
    if (role==Qt::BackgroundRole && QSqlQueryModel::data(index(item.row(),4),Qt::DisplayRole).toBool())
        return QVariant(QColor(255,200,100));
    if (item.column()==3){
        if (role==Qt::TextAlignmentRole) return int(Qt::AlignRight | Qt::AlignVCenter);
        if (role==Qt::DisplayRole) return QLocale().toString(QSqlQueryModel::data(item,role).toDouble(),'f',2);
    }
    if (item.column()==0 && role==Qt::DisplayRole)
        return QSqlQueryModel::data(item,role).toDate().toString("dd.MM.yy");
    return QSqlQueryModel::data(item,role);
}

void ModelStock::refresh(int id_part, QDate date)
{
    setQuery("select i.dat, i.num, t.nam, d.m_netto, i.nu from wire_warehouse as d "
             "inner join wire_whs_waybill as i on i.id=d.id_waybill "
             "inner join wire_way_bill_type as t on t.id=i.id_type "
             "where d.id_wparti="+QString::number(id_part)+" and i.dat<='"+date.toString("yyyy-MM-dd")+"' order by i.dat");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(0, Qt::Horizontal,tr("Дата"));
        setHeaderData(1, Qt::Horizontal,tr("Накладная"));
        setHeaderData(2, Qt::Horizontal,tr("Операция"));
        setHeaderData(3, Qt::Horizontal,tr("Масса, кг"));
    }
}


ModelShipment::ModelShipment(QObject *parent):
    QSqlQueryModel(parent)
{
    refresh(-1,QDate::currentDate());
}

QVariant ModelShipment::data(const QModelIndex &item, int role) const
{
    if (item.column()==3){
        if (role==Qt::TextAlignmentRole) return int(Qt::AlignRight | Qt::AlignVCenter);
        if (role==Qt::DisplayRole) return QLocale().toString(QSqlQueryModel::data(item,role).toDouble(),'f',2);
    }
    if (item.column()==0 && role==Qt::DisplayRole)
        return QSqlQueryModel::data(item,role).toDate().toString("dd.MM.yy");
    return QSqlQueryModel::data(item,role);
}

void ModelShipment::refresh(int id_part, QDate date)
{
    setQuery("select s.dat_vid, s.nom_s, p.short, d.m_netto, d.id from wire_shipment_consist as d "
             "inner join sertifikat as s on s.id=d.id_ship "
             "inner join poluch as p on s.id_pol=p.id "
             "where d.id_wparti="+QString::number(id_part)+" and s.dat_vid<='"+date.toString("yyyy-MM-dd")+"' order by s.dat_vid");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(0, Qt::Horizontal,tr("Дата"));
        setHeaderData(1, Qt::Horizontal,tr("№"));
        setHeaderData(2, Qt::Horizontal,tr("Получатель"));
        setHeaderData(3, Qt::Horizontal,tr("Масса, кг"));
    }
    double sum=0;
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,3),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (tr("Отгрузки итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : tr("Отгрузки");
    emit sigSum(s);
}


ModelInCex::ModelInCex(QObject *parent):
    DbTableModel("wire_in_cex_data",parent)
{
    addColumn("id",tr("id"));
    addColumn("id_wparti",tr("id_wparti"));
    addColumn("dat",tr("Дата"));
    addColumn("m_netto",tr("Масса, кг"));
    addColumn("id_type",tr("Операция"),Models::instance()->relAddType);
    setDefaultValue(4,3);
    setSuffix("inner join wire_in_cex_type on wire_in_cex_type.id=wire_in_cex_data.id_type and wire_in_cex_type.koef=1");
    setSort("wire_in_cex_data.dat");
    connect(this,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(this,SIGNAL(sigRefresh()),this,SLOT(calcSum()));
}

void ModelInCex::refresh(int id_part)
{
    setDefaultValue(1,id_part);
    setFilter("wire_in_cex_data.id_wparti = "+QString::number(id_part));
    select();
}

void ModelInCex::calcSum()
{
    double sum=0;
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,3),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (tr("Намотка итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : tr("Намотка");
    emit sigSum(s);
}


ModelOutCex::ModelOutCex(QObject *parent) :
    DbTableModel("wire_in_cex_data",parent)
{
    addColumn("id",tr("id"));
    addColumn("id_wparti",tr("id_wparti"));
    addColumn("dat",tr("Дата"));
    addColumn("m_netto",tr("Масса, кг"));
    addColumn("id_type",tr("Причина"),Models::instance()->relCause);
    setSuffix("inner join wire_in_cex_type on wire_in_cex_type.id = wire_in_cex_data.id_type");
    setSort("wire_in_cex_data.dat");
    connect(this,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(this,SIGNAL(sigRefresh()),this,SLOT(calcSum()));
}

void ModelOutCex::refresh(int id_part)
{
    setDefaultValue(1,id_part);
    setFilter("wire_in_cex_data.id_wparti = "+QString::number(id_part)+" and wire_in_cex_type.koef = -1");
    select();
}

void ModelOutCex::calcSum()
{
    double sum=0;
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,3),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (tr("Списание итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : tr("Списание");
    emit sigSum(s);
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
    setDefaultValue(1,QString("%1").arg((old_num+1),4,'d',0,QChar('0')));
    setDefaultValue(2,QDate::currentDate());
    return DbTableModel::insertRow(row,parent);
}


ModelPerepack::ModelPerepack(QObject *parent) : DbTableModel("wire_perepack",parent)
{
    addColumn("id",tr("id"));
    addColumn("id_nakl",tr("id_nakl"));
    addColumn("id_wpartisrc",tr("Из партии"),Models::instance()->relWirePart);
    addColumn("id_wpartires",tr("В партию"),Models::instance()->relWirePart);
    addColumn("m_netto",tr("Масса, кг"));
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

void ModelPerepack::refreshPart()
{
    Models::instance()->relWirePart->refreshModel();
}

ModelNamCex::ModelNamCex(QObject *parent) : DbTableModel("wire_in_cex_nam",parent)
{
    addColumn("id",tr("id"));
    addColumn("id_wparti",tr("id_wparti"));
    addColumn("dat",tr("Дата"));
    addColumn("m_netto",tr("Масса, кг"));
    addColumn("id_empl",tr("Работник"),Models::instance()->relRab);
    addColumn("id_line",tr("Линия"),Models::instance()->relLine);
    setSort("wire_in_cex_nam.dat");
    connect(this,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(this,SIGNAL(sigRefresh()),this,SLOT(calcSum()));
}

void ModelNamCex::refresh(int id_part)
{
    setDefaultValue(1,id_part);
    setFilter("wire_in_cex_nam.id_wparti = "+QString::number(id_part));
    select();
}

void ModelNamCex::calcSum()
{
    double sum=0;
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,3),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (tr("Производство итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : tr("Производство");
    emit sigSum(s);
}

ModelPodt::ModelPodt(QObject *parent) : DbTableModel("wire_podt",parent)
{
    addColumn("id","id");
    addColumn("n_s",tr("№"));
    addColumn("dat",tr("Дата"));
    addColumn("id_buht",tr("Исх. партия"),Models::instance()->relSrcPart);
    addColumn("id_diam",tr("Ф"),Models::instance()->relDiam);
    addColumn("id_line",tr("Стан"),Models::instance()->relLine);
    addColumn("comment",tr("Комментарий"));
    addColumn("id_type",tr("Тип"),Models::instance()->relPodtType);
    setSort("dat,n_s");
    setDefaultValue(7,1);
    connect(this,SIGNAL(sigUpd()),Models::instance()->relPodt->model(),SLOT(refresh()));
}

void ModelPodt::refresh(QDate beg, QDate end)
{
    setFilter("wire_podt.dat between '"+beg.toString("yyyy.MM.dd")+"' and '"+end.toString("yyyy.MM.dd")+"'");
    select();
}

bool ModelPodt::insertRow(int row, const QModelIndex &parent)
{
    int old_num=0;
    if (rowCount()>0) old_num=this->data(this->index(rowCount()-1,1),Qt::EditRole).toInt();
    setDefaultValue(1,QString("%1").arg((old_num+1),4,'d',0,QChar('0')));
    setDefaultValue(2,QDate::currentDate());
    return DbTableModel::insertRow(row,parent);
}

ModelPodtPart::ModelPodtPart(QObject *parent) : QSqlQueryModel(parent)
{
}

QVariant ModelPodtPart::data(const QModelIndex &index, int role) const
{
    if (index.row()>=rowCount()|| index.row()<0 || index.column()>=columnCount() || index.column()<0) return QVariant();

    QVariant value;

    if ((index.row()==QSqlQueryModel::rowCount()) && (role==Qt::DisplayRole || role==Qt::EditRole)){
        if (index.column()==0) {
            value= tr("Итого");
        } else if (index.column() == 1) {
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
            if(index.column() >=1)
                return value.isNull() ? QVariant() : QLocale().toString(value.toDouble(),'f',2);
            else
                return value;
        }

        case Qt::EditRole:
        {
            return value;
        }

        case Qt::TextAlignmentRole:
        {
             if(index.column() >=1)
                 return int(Qt::AlignRight | Qt::AlignVCenter);
             else return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
    return value;
}

int ModelPodtPart::rowCount(const QModelIndex &parent) const
{
    return (QSqlQueryModel::rowCount(parent)==0)? 0 : QSqlQueryModel::rowCount(parent)+1;
}

void ModelPodtPart::refresh(int id_podt)
{
    this->clear();
    //setQuery("select m.n_s, m.dat, m.kvo from wire_parti_m as m where m.id_podt = "+QString::number(id_podt));
    setQuery("select m.n_s||'-'||date_part('year',m.dat) ||' '|| pr.nam ||' '|| dm.sdim ||' '||k.short  as parti, sum(d.m_netto), r.kvo, z.kvo "
             "from wire_in_cex_nam as d "
             "inner join wire_parti as p on p.id=d.id_wparti "
             "inner join wire_parti_m as m on m.id=p.id_m "
             "inner join provol as pr on m.id_provol=pr.id "
             "inner join diam as dm on m.id_diam=dm.id "
             "inner join wire_pack_kind as k on p.id_pack=k.id "
             "left join (select id_part as id, value as kvo from wire_parti_mech where id_mech=1) as z on z.id=m.id "
             "left join (select id_part as id, avg(kvo) as kvo from wire_mech where id_mech=1 group by id_part) as r on r.id=m.id "
             "where m.id_podt = "+QString::number(id_podt)+
             "group by parti, r.kvo, z.kvo order by parti");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(0, Qt::Horizontal,tr("Партия"));
        //setHeaderData(1, Qt::Horizontal,tr("Дата"));
        setHeaderData(1, Qt::Horizontal,tr("Масса, кг"));
        setHeaderData(2, Qt::Horizontal,tr("σв, МПа"));
        setHeaderData(3, Qt::Horizontal,tr("σв(c),МПа"));
    }
}

ModelPodtCont::ModelPodtCont(QObject *parent) : DbTableModel("wire_podt_cont",parent)
{
    addColumn("id",tr("id"));
    addColumn("id_podt",tr("id_podt"));
    addColumn("dat",tr("Дата"));
    addColumn("kvo",tr("Масса, кг"));
    addColumn("id_rab",tr("Волочильщик"),Models::instance()->relVol);
    addColumn("id_podt_src",tr("Исходная протяжка"),Models::instance()->relPodt);
    setSort("wire_podt_cont.dat");
    connect(this,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(this,SIGNAL(sigRefresh()),this,SLOT(calcSum()));
}

void ModelPodtCont::refresh(int id_part)
{
    setDefaultValue(1,id_part);
    setFilter("wire_podt_cont.id_podt = "+QString::number(id_part));
    select();
}

void ModelPodtCont::calcSum()
{
    double sum=0;
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,3),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (tr("Поступление итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : tr("Поступление");
    emit sigSum(s);
}

ModelPodtNext::ModelPodtNext(QObject *parent) : QSqlQueryModel(parent)
{
    refresh(-1);
}

void ModelPodtNext::refresh(int id_podt)
{
    setQuery("select p.n_s ||'-'||date_part('year',p.dat), c.dat, d.sdim from wire_podt_cont as c "
             "inner join wire_podt as p on p.id=c.id_podt "
             "inner join diam as d on p.id_diam=d.id "
             "where c.id_podt_src = "+QString::number(id_podt)+" order by p.n_s, p.dat");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(0, Qt::Horizontal,tr("Подтяжка"));
        setHeaderData(1, Qt::Horizontal,tr("Дата"));
        setHeaderData(2, Qt::Horizontal,tr("Диаметр"));
    }
}

ModelPodtAnn::ModelPodtAnn(QObject *parent) : QSqlQueryModel(parent)
{
    refresh(-1);
}

void ModelPodtAnn::refresh(int id_podt)
{
    setQuery("select a.n_s, a.dat, o.nam from wire_ann_cont as c "
             "inner join wire_ann as a on a.id=c.id_ann "
             "inner join wire_oven as o on o.id=a.id_wire_oven "
             "where c.id_podt = "+QString::number(id_podt)+" group by a.n_s, a.dat, o.nam order by a.n_s, a.dat");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(0, Qt::Horizontal,tr("Отжиг"));
        setHeaderData(1, Qt::Horizontal,tr("Дата"));
        setHeaderData(2, Qt::Horizontal,tr("Стенд"));
    }
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

ModelPackCex::ModelPackCex(QObject *parent) : DbTableModel("wire_parti_pack",parent)
{
    addColumn("id",tr("id"));
    addColumn("id_part",tr("id_part"));
    addColumn("dat",tr("Дата"));
    addColumn("kvo",tr("Масса, кг"));
    setSort("wire_parti_pack.dat");
    connect(this,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(this,SIGNAL(sigRefresh()),this,SLOT(calcSum()));
}

void ModelPackCex::refresh(int id_part)
{
    setDefaultValue(1,id_part);
    setFilter("wire_parti_pack.id_part = "+QString::number(id_part));
    select();
}

void ModelPackCex::calcSum()
{
    double sum=0;
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,3),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (tr("Упаковка итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : tr("Упаковка");
    emit sigSum(s);
}

ModelNaklPodt::ModelNaklPodt(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelNaklPodt::refresh(QDate beg, QDate end, int id_type, int id_podt_type)
{
    QSqlQuery query;
    query.prepare("select distinct date_part('doy',wpc.dat), wpc.dat, wpc.id_op "
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

void ModelNaklPodtCont::refresh(QDate dat, int id_type)
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
                  "where (wpc.id_op = :t) and wpc.dat = :d ");
    query.bindValue(":t",id_type);
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

ModelPodtProd::ModelPodtProd(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelPodtProd::refresh(QDate beg, QDate end)
{
    QSqlQuery query;
    query.prepare("select p.id_type, pp.id_pr, coalesce(wps.id_diam,pp.id_dim), p.id_diam, wpt.nam, pr.nam, coalesce(ds.sdim,d.sdim) as fromdim, dp.sdim, sum(pc.kvo) "
                  "from wire_podt_cont as pc "
                  "inner join wire_podt as p on pc.id_podt=p.id "
                  "inner join prov_buht as b on p.id_buht=b.id "
                  "inner join prov_prih as pp on b.id_prih=pp.id "
                  "inner join provol as pr on pp.id_pr=pr.id "
                  "inner join diam as d on pp.id_dim=d.id "
                  "inner join diam as dp on p.id_diam=dp.id "
                  "inner join wire_podt_type wpt on wpt.id=p.id_type "
                  "left join wire_podt as wps on wps.id=pc.id_podt_src "
                  "left join diam as ds on ds.id=wps.id_diam "
                  "where pc.dat between :d1 and :d2 "
                  "group by p.id_type, pp.id_pr, pp.id_dim, p.id_diam, wpt.nam, pr.nam, d.sdim, dp.sdim, wps.id_diam, ds.sdim "
                  "order by wpt.nam, pr.nam, fromdim, dp.sdim");
    query.bindValue(":d1",beg);
    query.bindValue(":d2",end);
    if (query.exec()){
        setQuery(query);
        setHeaderData(4,Qt::Horizontal,QString::fromUtf8("Тип полуфабриката"));
        setHeaderData(5,Qt::Horizontal,QString::fromUtf8("Марка"));
        setHeaderData(6,Qt::Horizontal,QString::fromUtf8("Ф нач."));
        setHeaderData(7,Qt::Horizontal,QString::fromUtf8("Ф кон."));
        setHeaderData(8,Qt::Horizontal,QString::fromUtf8("Выпуск, кг"));
        updState();
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
    }
}

QVariant ModelPodtProd::data(const QModelIndex &item, int role) const
{
    /*if (role==Qt::BackgroundColorRole){
        QString s=this->data(this->index(item.row(),0),Qt::EditRole).toString();
        s+=":";
        s+=this->data(this->index(item.row(),1),Qt::EditRole).toString();
        s+=":";
        s+=this->data(this->index(item.row(),2),Qt::EditRole).toString();
        return exList.contains(s) ? QVariant(QColor(255,255,255)) : QVariant(QColor(255,170,170));
    }*/
    if (role==Qt::DisplayRole && item.column()==8){
        return QLocale().toString(QSqlQueryModel::data(item,Qt::EditRole).toDouble(),'f',1);
    }
    if (role==Qt::TextAlignmentRole && item.column()==8){
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    return QSqlQueryModel::data(item,role);
}

bool ModelPodtProd::ready()
{
    bool ok=true;
    /*for (int i=0; i<rowCount(); i++){
        QString s=this->data(this->index(i,0),Qt::EditRole).toString();
        s+=":";
        s+=this->data(this->index(i,1),Qt::EditRole).toString();
        s+=":";
        s+=this->data(this->index(i,2),Qt::EditRole).toString();
        ok=ok && exList.contains(s);
        if (!ok){
            break;
        }
    }*/
    return ok;
}

void ModelPodtProd::updState()
{
    /*QSqlQuery query;
    query.prepare("select distinct id_provol||':'||id_diam||':'|| id_spool as nam from econom_zp_norm order by nam");
    if (query.exec()){
        exList.clear();
        while(query.next()){
            exList.push_back(query.value(0).toString());
        }
        emit dataChanged(this->index(0,0),this->index(rowCount()-1,columnCount()-1));
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
    }*/
}
