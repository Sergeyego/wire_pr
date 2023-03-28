#include "modelship.h"

ModelShip::ModelShip(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void ModelShip::refresh(QDate begDate, QDate endDate)
{
    setQuery("select s.id, s.nom_s, s.dat_vid, p.short from sertifikat s "
                 "inner join poluch p on s.id_pol=p.id "
                 "inner join (select distinct id_ship from wire_shipment_consist) w on w.id_ship=s.id "
                 "where s.dat_vid between '"+begDate.toString("yyyy.MM.dd")+"' and '"+endDate.toString("yyyy.MM.dd")+
                 "' order by s.dat_vid, s.nom_s");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Номер"));
        setHeaderData(2, Qt::Horizontal,tr("Дата"));
        setHeaderData(3, Qt::Horizontal,tr("Получатель"));
    }
}


ModelDataShip::ModelDataShip(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void ModelDataShip::refresh(int id_ship)
{
    setQuery("select w.id, pr.nam || CASE WHEN p.id_var <> 1 THEN (' /'::text || ev.nam::text) || '/'::text ELSE ''::text END AS mark, "
             "d.sdim, k.short, m.n_s, w.m_netto "
             "from wire_shipment_consist w "
             "inner join wire_parti p on p.id=w.id_wparti "
             "inner join wire_parti_m m on p.id_m=m.id "
             "inner join provol pr on pr.id=m.id_provol "
             "inner join diam d on d.id=m.id_diam "
             "inner join wire_pack_kind k on p.id_pack=k.id "
             "inner join elrtr_vars ev on ev.id=p.id_var "
             "where w.id_ship= "+QString::number(id_ship)+
             " order by pr.nam, d.sdim, k.short, m.n_s");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Марка"));
        setHeaderData(2, Qt::Horizontal,tr("Ф"));
        setHeaderData(3, Qt::Horizontal,tr("Катушка"));
        setHeaderData(4, Qt::Horizontal,tr("Партия"));
        setHeaderData(5, Qt::Horizontal,tr("Кг"));
        refreshState(id_ship);
    }
}

QVariant ModelDataShip::data(const QModelIndex &item, int role) const
{
    if(role==Qt::TextAlignmentRole){ // Выравнивание
        if(item.column() == 2 || item.column()==5)
            return int(Qt::AlignRight | Qt::AlignVCenter);
        else return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    if((role == Qt::BackgroundColorRole)) {
        int area = colorState.value(QSqlQueryModel::data(this->index(item.row(),0),Qt::DisplayRole).toInt());
        if(area == 0) return QVariant(QColor(255,170,170)); else
            if(area == 1) return QVariant(QColor(Qt::yellow)); else
                if(area == 2) return QVariant(QColor(Qt::gray)); else
                    if(area == 3) return QVariant(QColor(170,255,170));
    } else return QSqlQueryModel::data(item,role);
}

void ModelDataShip::refreshState(int id_ship)
{
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("select wire_shipment_consist.id, "
                  "(select case when exists(select id from wire_parti_chem where id_part=(select p.id_m from wire_parti as p where p.id = wire_shipment_consist.id_wparti)) "
                           "then 1 else 0 end "
                           "+ "
                           "case when exists(select id from wire_parti_mech where id_part=(select p.id_m from wire_parti as p where p.id = wire_shipment_consist.id_wparti)) "
                           "then 2 else 0 end "
                           "as r) from wire_shipment_consist where wire_shipment_consist.id_ship=:id");
    query.bindValue(":id",id_ship);
    if (query.exec()){
        colorState.clear();
        while (query.next()){
            colorState[query.value(0).toInt()]=query.value(1).toInt();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}
