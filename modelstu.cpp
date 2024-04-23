#include "modelstu.h"

ModelChem::ModelChem(QString tname, QObject *parent) : DbTableModel(tname,parent), tablename(tname)
{
    colIdPart=0;
    colIdChem=1;
    colVal=2;
}

QVariant ModelChem::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role==Qt::BackgroundRole){
        return getColor(index);
    }
    if (role==Qt::ToolTipRole){
        return getToolTip(index);
    }
    return DbTableModel::data(index,role);
}

ModelChem::~ModelChem()
{

}

void ModelChem::refresh(int id_part)
{
    map.clear();
    QSqlQuery query;
    query.prepare(tuQuery);
    query.bindValue(":id",id_part);
    if (query.exec()){
        while (query.next()){
            range r;
            r.min=query.value(1);
            r.max=query.value(2);
            map.insert(query.value(0).toInt(),r);
        }
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
    QString filter=flt;
    filter.replace(":idp",QString::number(id_part));
    setFilter(filter);
    setDefaultValue(colIdPart,id_part);
    select();
}

QColor ModelChem::getColor(const QModelIndex &index) const
{
    if (!index.isValid()){
        return QColor();
    }
    int id_chem=data(this->index(index.row(),colIdChem),Qt::EditRole).toInt();
    double val=data(this->index(index.row(),colVal),Qt::EditRole).toDouble();
    range r=map.value(id_chem);
    QColor color=Qt::white;
    if (!r.min.isNull() || !r.max.isNull()){
        if (!r.max.isNull()){
            if ((val>r.min.toDouble()) && (val<r.max.toDouble())){
                color=QColor(170,255,170);
            } else if ((val<r.min.toDouble()) || (val>r.max.toDouble())){
                color=QColor(255,170,170);
            } else {
                color=Qt::yellow;
            }
        } else {
            if (val<r.min.toDouble()){
                color=QColor(255,170,170);
            } else if (val>r.min.toDouble()){
                color=QColor(170,255,170);
            } else {
                color=Qt::yellow;
            }
        }
    }
    return color;
}

QString ModelChem::getToolTip(const QModelIndex &index) const
{
    if (!index.isValid()){
        return QString();
    }
    int id_chem=data(this->index(index.row(),colIdChem),Qt::EditRole).toInt();
    QString el=data(this->index(index.row(),colIdChem),Qt::DisplayRole).toString();
    range r=map.value(id_chem);
    QString s;
    if (!r.min.isNull() || !r.max.isNull()){
        s+=el;
        if (r.max.isNull() && !r.min.isNull()){
            s+=QString(": min %1").arg(r.min.toDouble());
        } else if (!r.max.isNull() && r.min.isNull()){
            s+=QString(": max %1").arg(r.max.toDouble());
        } else {
            s+=QString(": min %1 max %2").arg(r.min.toDouble()).arg(r.max.toDouble());
        }
    }
    return s;
}

ModelChemSrc::ModelChemSrc(QObject *parent) : ModelChem("prov_chem",parent)
{
    addColumn("id_buht","id_buht");
    addColumn("id_el",tr("Элем."),Rels::instance()->relChem);
    addColumn("kvo",tr("Сод., %"));
    setDecimals(2,3);
    setSort("chem_tbl.sig");
    flt=tableName+".id_buht = :idp";
    colIdPart=0;
    colIdChem=1;
    colVal=2;
    tuQuery="select c.id_chem, c.min, c.max from wire_chem_tu as c where c.id_provol = "
            "(select COALESCE(pr.id_base, pp.id_pr) from prov_buht as b inner join prov_prih as pp on b.id_prih=pp.id "
            "inner join provol as pr on pp.id_pr=pr.id where b.id = :id )";
}

ModelChemSert::ModelChemSert(QObject *parent) : ModelChem("wire_parti_chem",parent)
{
    addColumn("id","id");
    addColumn("id_part","id_part");
    addColumn("id_chem",tr("Элем."),Rels::instance()->relChem);
    addColumn("value",tr("Сод., %"));
    setDecimals(3,3);
    setSort("chem_tbl.sig");
    flt=tableName+".id_part = :idp";
    colIdPart=1;
    colIdChem=2;
    colVal=3;
    tuQuery="select c.id_chem, c.min, c.max from wire_chem_tu as c where c.id_provol = "
            "(select COALESCE(pr.id_base, p.id_provol) from wire_parti_m as p inner join provol as pr on p.id_provol=pr.id where p.id = :id )";
}
