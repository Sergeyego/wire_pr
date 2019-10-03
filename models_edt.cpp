#include "models_edt.h"

ModelPart::ModelPart(QObject *parent):DbTableModel("wire_parti_m",parent)
{
    id_prov=-1;
    addColumn("id","id",true,TYPE_INT);
    addColumn("n_s",tr("№"),false,TYPE_STRING);
    addColumn("dat",tr("Дата"),false,TYPE_DATE);
    addColumn("id_source",tr("Источник"), false,TYPE_STRING,NULL,Models::instance()->relSource);
    addColumn("id_buht",tr("Исх. партия"),false,TYPE_STRING,NULL,Models::instance()->relSrcPart);
    addColumn("id_provol",tr("Марка"),false,TYPE_STRING,NULL,Models::instance()->relProvol);
    addColumn("id_diam",tr("Ф"),false,TYPE_STRING,NULL,Models::instance()->relDiam);
    addColumn("id_type",tr("Стан"),false,TYPE_STRING,NULL,Models::instance()->relLine);
    addColumn("comment",tr("Комментарий"),false,TYPE_STRING);
    addColumn("id_empl",tr("Волочильщик"),false,TYPE_STRING,NULL,Models::instance()->relVol);
    addColumn("kvo",tr("Масса"),false,TYPE_DOUBLE, new QDoubleValidator(0,999999999,1,this));
    addColumn("id_podt",tr("Подтяжка"),false,TYPE_STRING,NULL, Models::instance()->relPodt);
    setSort("dat,n_s");
    setDefaultValue(10,0);
    connect(this,SIGNAL(sigUpd()),Models::instance()->relWirePart->model(),SLOT(refresh()));
}

bool ModelPart::submitRow()
{
    /*if (isAdd()&& isEdt()){
        int year=this->data(this->index(rowCount()-1,2),Qt::EditRole).toDate().year();
        setData(this->index(rowCount()-1,8),year,Qt::EditRole);
        setData(this->index(rowCount()-1,8),year,Qt::DisplayRole);
    }*/
    return DbTableModel::submitRow();
}

bool ModelPart::insertRow(int row, const QModelIndex &parent)
{
    int old_num=0;
    if (rowCount()>0) old_num=this->data(this->index(rowCount()-1,1),Qt::EditRole).toInt();
    setDefaultValue(1,QString("%1").arg((old_num+1),4,'d',0,QChar('0')));
    return DbTableModel::insertRow(row,parent);
}

QVariant ModelPart::data(const QModelIndex &index, int role) const
{
    if((role == Qt::BackgroundColorRole)) {
        int area = colorState.value(data(this->index(index.row(),0),Qt::EditRole).toInt());
        if(area == 0) return QVariant(QColor(255,170,170)); else
            if(area == 1) return QVariant(QColor(Qt::yellow)); else
                if(area == 2) return QVariant(QColor(Qt::gray)); else
                    if(area == 3) return QVariant(QColor(170,255,170));
    } else return DbTableModel::data(index,role);
}

bool ModelPart::setData(const QModelIndex &index, const QVariant &value, int role)
{
    /*if (index.column()==2){
        bool ok=false;
        if (DbTableModel::setData(index,value,role)){
            ok=setData(this->index(index.row(),8),value.toDate().year(),role);
        }
        return ok;
    } else */return DbTableModel::setData(index,value,role);
}

void ModelPart::copyChem(int id_wparti)
{
    QSqlQuery query;
    query.prepare("insert into wire_parti_chem (id_part, id_chem, value ) "
                  "(select :id_part1, pr.id_el, pr.kvo from prov_chem as pr "
                  "where pr.id_buht=(select p.id_buht from wire_parti_m as p where p.id= :id_part2 ))");
    query.bindValue(":id_part1",id_wparti);
    query.bindValue(":id_part2",id_wparti);
    if (query.exec()){
        refreshState();
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}

void ModelPart::setIdProv(int id)
{
    id_prov=id;
}

void ModelPart::setDateBeg(QDate date)
{
    dateBeg=date;
}

void ModelPart::setDateEnd(QDate date)
{
    dateEnd=date;
}

bool ModelPart::select()
{
    QString flt= id_prov==-1 ? "" : " and wire_parti_m.id_provol = "+QString::number(id_prov);
    setFilter("wire_parti_m.dat between '"+dateBeg.toString("yyyy.MM.dd")+"' and '"
                             +dateEnd.toString("yyyy.MM.dd")+"'"+flt);
    refreshState();
    return DbTableModel::select();
}

void ModelPart::refreshState()
{
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("select wire_parti_m.id, "
                      "(select case when exists(select id from wire_parti_chem where id_part=wire_parti_m.id) "
                               "then 1 else 0 end "
                               "+ "
                               "case when exists(select id from wire_parti_mech where id_part=wire_parti_m.id) "
                               "then 2 else 0 end "
                               "as r) from wire_parti_m where wire_parti_m.dat between :d1 and :d2");
    query.bindValue(":d1",dateBeg);
    query.bindValue(":d2",dateEnd);
    if (query.exec()){
        colorState.clear();
        while (query.next()){
            colorState[query.value(0).toInt()]=query.value(1).toInt();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}


ModelWirePartiChem::ModelWirePartiChem(QObject *parent):
    DbTableModel("wire_parti_chem",parent)
{
    addColumn("id","id",true,TYPE_INT);
    addColumn("id_part","id_part",false,TYPE_INT);
    addColumn("id_chem",tr("Элемент"),false,TYPE_STRING,NULL,Models::instance()->relChemTbl);
    addColumn("value",tr("Содерж., %"),false,TYPE_DOUBLE,new QDoubleValidator(0,100,3,this));
    setSuffix("inner join chem_tbl on wire_parti_chem.id_chem=chem_tbl.id");
    setSort("chem_tbl.sig");
}

void ModelWirePartiChem::refresh(int id_part)
{
    setFilter("wire_parti_chem.id_part="+QString::number(id_part));
    id_wparti=id_part;
    setDefaultValue(1,id_wparti);
    select();
}


ModelWirePartiMech::ModelWirePartiMech(QObject *parent):DbTableModel("wire_parti_mech",parent)
{
    addColumn("id","id",true,TYPE_INT);
    addColumn("id_part","id_part",false,TYPE_INT);
    addColumn("id_mech",tr("Параметр"),false,TYPE_STRING,NULL,Models::instance()->relMechTbl);
    addColumn("value",tr("Значение"),false,TYPE_DOUBLE,new QDoubleValidator(-9999999,9999999,3,this));
    addColumn("value_max",tr("Значение-2"),false,TYPE_DOUBLE,new QDoubleValidator(-9999999,9999999,3,this));
    addColumn("id_cat",tr("Категория"),false,TYPE_STRING,NULL,Models::instance()->relMechCat);
    setSuffix("inner join mech_tbl on wire_parti_mech.id_mech=mech_tbl.id");
    setSort("mech_tbl.nam");
}

void ModelWirePartiMech::refresh(int id_part)
{
    setFilter("wire_parti_mech.id_part="+QString::number(id_part));
    id_wparti=id_part;
    setDefaultValue(1,id_wparti);
    select();
}

bool ModelWirePartiMech::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column()==2 && role==Qt::EditRole){
        int id_cat=0;
        int id_mech=value.toInt();
        if (id_mech==1 || id_mech==2 || id_mech==3 || id_mech==21)
            id_cat=0;
        else if (id_mech==23 || id_mech==24)
            id_cat=1;
        else id_cat=2;
        DbTableModel::setData(this->index(index.row(),5),id_cat,role);
    }
    return DbTableModel::setData(index,value,role);
}


SrcChemModel::SrcChemModel(QObject *parent):
    QSqlQueryModel(parent)
{
}

void SrcChemModel::refresh(int id_part)
{
    setQuery("select sig, kvo "
             "from prov_chem p "
             "inner join chem_tbl t on t.id=p.id_el "
             "where id_buht=(select id_buht from wire_parti_m as p where p.id= "+QString::number(id_part)+
             " ) order by sig");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(0,Qt::Horizontal,tr("Элемент"));
        setHeaderData(1,Qt::Horizontal,tr("Содерж., %"));
    }
}

QVariant SrcChemModel::data(const QModelIndex &item, int role) const
{
    if (item.column()==1){
        if (role==Qt::TextAlignmentRole)
            return int(Qt::AlignRight | Qt::AlignVCenter);
        if (role==Qt::DisplayRole){
            QVariant value=QSqlQueryModel::data(item,role);
            return QLocale().toString(value.toDouble(),'f',3);
        }
    }
    return QSqlQueryModel::data(item,role);
}


PartPackModel::PartPackModel(QObject *parent):
    QSqlQueryModel(parent)
{
    refresh(-1);
}

void PartPackModel::refresh(int id_part)
{
    id_m=id_part;
    setQuery("select p.id, k.short, COALESCE(' ('||t.mas_ed::text||' "+tr("кг")+")','') "
             "from wire_parti as p "
             "inner join wire_pack_kind as k on k.id=p.id_pack "
             "left outer join wire_pack as t on t.id=p.id_pack_type "
             "where p.id_m = "+QString::number(id_part)+" "
             "order by k.short");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    }
}

bool PartPackModel::addPack(int id_pack, int id_type)
{
    QSqlQuery query;
    query.prepare("insert into wire_parti (id_pack, id_pack_type, id_m) values ( :id_pack, :id_pack_type, :id_m )");
    query.bindValue(":id_pack",id_pack);
    query.bindValue(":id_pack_type",id_type);
    query.bindValue(":id_m",id_m);
    bool ok = query.exec();
    if (!ok){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        Models::instance()->relWirePart->model()->refresh();
    }
    return ok;
}

bool PartPackModel::delPack(int id)
{
    QSqlQuery query;
    query.prepare("delete from wire_parti where id= :val ");
    query.bindValue(":val",id);
    bool ok = query.exec();
    if (!ok){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        Models::instance()->relWirePart->model()->refresh();
    }
    return ok;
}

bool PartPackModel::edtPack(int id, int id_pack_new, int id_type_new)
{
    QSqlQuery query;
    query.prepare("update wire_parti set id_pack = :id_pack, id_pack_type = :id_pack_type where id= :id ");
    query.bindValue(":id",id);
    query.bindValue(":id_pack",id_pack_new);
    query.bindValue(":id_pack_type",id_type_new);
    bool ok = query.exec();
    if (!ok){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        Models::instance()->relWirePart->model()->refresh();
    }
    return ok;
}

ModelAnn::ModelAnn(QObject *parent) : DbTableModel("wire_ann",parent)
{
    addColumn("id","id",true,TYPE_INT);
    addColumn("n_s",tr("№"),false,TYPE_STRING);
    addColumn("dat",tr("Дата"),false,TYPE_DATE);
    addColumn("id_wire_oven",tr("Стенд"),false,TYPE_STRING,NULL,Models::instance()->relOven);
    addColumn("time_blow",tr("Время продувки"),false,TYPE_DOUBLE, new QDoubleValidator(0,999999999,1,this));
    addColumn("temp",tr("Температура"),false,TYPE_DOUBLE, new QDoubleValidator(0,999999999,1,this));
    addColumn("time_ex",tr("Время выдержки"),false,TYPE_DOUBLE, new QDoubleValidator(0,999999999,1,this));
    addColumn("note",tr("Комментарий"),false,TYPE_STRING);
    setSort("wire_ann.dat, wire_ann.n_s");
}

void ModelAnn::refresh(QDate beg, QDate end)
{
    setFilter("wire_ann.dat between '"+beg.toString("yyyy.MM.dd")+"' and '"+end.toString("yyyy.MM.dd")+"'");
    select();
}

bool ModelAnn::insertRow(int row, const QModelIndex &parent)
{
    int old_num=0;
    if (rowCount()>0) old_num=this->data(this->index(rowCount()-1,1),Qt::EditRole).toInt();
    setDefaultValue(1,QString("%1").arg((old_num+1),4,'d',0,QChar('0')));
    return DbTableModel::insertRow(row,parent);
}

ModelPodtMech::ModelPodtMech(QObject *parent) : DbTableModel("wire_podt_mech", parent)
{
    addColumn("id","id",true,TYPE_INT);
    addColumn("id_podt","id_podt",false,TYPE_INT);
    addColumn("id_mech",tr("Параметр"),false,TYPE_STRING,NULL,Models::instance()->relMechTbl);
    addColumn("kvo",tr("Значение"),false,TYPE_DOUBLE,new QDoubleValidator(-9999999,9999999,3,this));
    setDefaultValue(2,1);
    setSort("wire_podt_mech.id_mech, wire_podt_mech.kvo");
}

void ModelPodtMech::refresh(int id_podt)
{
    setDefaultValue(1,id_podt);
    setFilter("wire_podt_mech.id_podt = "+QString::number(id_podt));
    select();
}

ModelMechReal::ModelMechReal(QObject *parent) : DbTableModel("wire_mech",parent)
{
    addColumn("id","id",true,TYPE_INT);
    addColumn("id_part","id_part",false,TYPE_INT);
    addColumn("id_mech",tr("Параметр"),false,TYPE_STRING,NULL,Models::instance()->relMechTbl);
    addColumn("kvo",tr("Значение"),false,TYPE_DOUBLE,new QDoubleValidator(-9999999,9999999,3,this));
    setDefaultValue(2,1);
    setSort("wire_mech.id_mech, wire_mech.kvo");
}

void ModelMechReal::refresh(int id_part)
{
    setDefaultValue(1,id_part);
    setFilter("wire_mech.id_part = "+QString::number(id_part));
    select();
}
