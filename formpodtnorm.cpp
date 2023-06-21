#include "formpodtnorm.h"
#include "ui_formpodtnorm.h"

FormPodtNorm::FormPodtNorm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPodtNorm)
{
    ui->setupUi(this);

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    ui->comboBoxType->setModel(Models::instance()->relPodtType->model());
    ui->comboBoxType->setModelColumn(1);

    modelNorm = new DbTableModel("wire_podt_norm",this);
    modelNorm->addColumn("id_podt_type","id_podt_type");
    modelNorm->addColumn("id_provol","id_provol");
    modelNorm->addColumn("id_buht_diam","id_buht_diam");
    modelNorm->addColumn("id_src_diam","id_src_diam");
    modelNorm->addColumn("id_diam","id_diam");
    modelNorm->addColumn("id_type_src","id_type_src");
    modelNorm->addColumn("id_type_vol","id_type_vol");
    modelNorm->addColumn("id_matr",tr("Материал"),Models::instance()->relMatr);
    modelNorm->addColumn("kvo",tr("Норма"));
    modelNorm->addColumn("id_vid",tr("Вид затрат"),Models::instance()->relRasxVid);
    modelNorm->addColumn("dat_beg",tr("Дата нач."));
    modelNorm->addColumn("dat_end",tr("Дата кон."));

    modelNorm->setSort("id_vid, id_matr, dat_beg");
    modelNorm->setDecimals(7,3);
    modelNorm->setDefaultValue(10,QDate());

    ui->tableViewNorm->setModel(modelNorm);
    for (int i=0; i<=6; i++){
        ui->tableViewNorm->setColumnHidden(i,true);
    }
    ui->tableViewNorm->setColumnWidth(7,330);
    ui->tableViewNorm->setColumnWidth(8,80);
    ui->tableViewNorm->setColumnWidth(9,200);
    ui->tableViewNorm->setColumnWidth(10,80);
    ui->tableViewNorm->setColumnWidth(11,80);

    modelPodtProd = new ModelPodtProd(this);
    ui->tableViewPodt->setModel(modelPodtProd);

    connect(ui->tableViewPodt->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(updNorm(QModelIndex)));
    upd();

    for (int i=0; i<=6; i++){
        ui->tableViewPodt->setColumnHidden(i,true);
    }

    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->pushButtonCopy,SIGNAL(clicked(bool)),this,SLOT(copy()));
    connect(ui->pushButtonPaste,SIGNAL(clicked(bool)),this,SLOT(paste()));
    connect(ui->pushButtonRep,SIGNAL(clicked(bool)),this,SLOT(report()));
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this,SLOT(save()));
    connect(modelNorm,SIGNAL(sigUpd()),modelPodtProd,SLOT(updState()));
    connect(ui->comboBoxType,SIGNAL(currentIndexChanged(int)),this,SLOT(upd()));
}

FormPodtNorm::~FormPodtNorm()
{
    delete ui;
}

QVariant FormPodtNorm::currentData(int column)
{
    int row=ui->tableViewPodt->currentIndex().row();
    return ui->tableViewPodt->model()->data(ui->tableViewPodt->model()->index(row,column),Qt::EditRole);
}

int FormPodtNorm::getIdType()
{
    return ui->comboBoxType->model()->data(ui->comboBoxType->model()->index(ui->comboBoxType->currentIndex(),0),Qt::EditRole).toInt();
}

bool FormPodtNorm::ready()
{
    bool ok=modelPodtProd->ready();
    if (!ok){
        QMessageBox::information(this,QString::fromUtf8("Предупреждение"),QString::fromUtf8("Не для всех марок указаны нормы!"),QMessageBox::Ok);
    }
    return ok;
}

void FormPodtNorm::upd()
{
    modelPodtProd->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),getIdType());
    if (ui->tableViewPodt->model()->rowCount()){
        ui->tableViewPodt->selectRow(0);
    }
    calcSum();
    ui->tableViewPodt->resizeToContents();
}

void FormPodtNorm::updNorm(QModelIndex /*ind*/)
{
    int id_type=currentData(0).toInt();
    int id_provol=currentData(1).toInt();
    int id_buht_diam=currentData(2).toInt();
    int id_src_diam=currentData(3).toInt();
    int id_diam=currentData(4).toInt();
    int id_type_src=currentData(5).toInt();
    int id_type_vol=currentData(6).toInt();
    modelNorm->setDefaultValue(0,id_type);
    modelNorm->setDefaultValue(1,id_provol);
    modelNorm->setDefaultValue(2,id_buht_diam);
    modelNorm->setDefaultValue(3,id_src_diam);
    modelNorm->setDefaultValue(4,id_diam);
    modelNorm->setDefaultValue(5,id_type_src);
    modelNorm->setDefaultValue(6,id_type_vol);
    modelNorm->setFilter(QString("id_podt_type = %1 and id_provol = %2 and id_src_diam = %3 and id_diam = %4 and id_buht_diam = %5 and id_type_src = %6 and id_type_vol = %7").arg(id_type).arg(id_provol).arg(id_src_diam).arg(id_diam).arg(id_buht_diam).arg(id_type_src).arg(id_type_vol));
    modelNorm->select();
}

void FormPodtNorm::copy()
{
    buf.clear();
    for (int i=0; i<modelNorm->rowCount();i++){
        mnorm m;
        m.id_matr=modelNorm->data(modelNorm->index(i,7),Qt::EditRole).toInt();
        m.kvo=modelNorm->data(modelNorm->index(i,8),Qt::EditRole).toDouble();
        m.id_vid=modelNorm->data(modelNorm->index(i,9),Qt::EditRole).toInt();
        m.d_beg=modelNorm->data(modelNorm->index(i,10),Qt::EditRole).toDate();
        m.d_end=modelNorm->data(modelNorm->index(i,11),Qt::EditRole).toDate();
        buf.push_back(m);
    }
    ui->pushButtonPaste->setEnabled(true);
}

void FormPodtNorm::paste()
{
    int id_type=currentData(0).toInt();
    int id_provol=currentData(1).toInt();
    int id_buht_diam=currentData(2).toInt();
    int id_src_diam=currentData(3).toInt();
    int id_diam=currentData(4).toInt();
    int id_type_src=currentData(5).toInt();
    int id_type_vol=currentData(6).toInt();
    QString err;
    foreach (mnorm m, buf) {
        QSqlQuery query;
        query.prepare("insert into wire_podt_norm (id_podt_type, id_type_src, id_type_vol, id_provol, id_buht_diam, id_src_diam, id_diam, id_matr, kvo, id_vid, dat_beg, dat_end) values "
                      "(:id_podt_type, :id_type_src, :id_type_vol, :id_provol, :id_buht_diam, :id_src_diam, :id_diam, :id_matr, :kvo, :id_vid, :dat_beg, :dat_end)");
        query.bindValue(":id_podt_type",id_type);
        query.bindValue(":id_type_src",id_type_src);
        query.bindValue(":id_type_vol",id_type_vol);
        query.bindValue(":id_provol",id_provol);
        query.bindValue(":id_buht_diam",id_buht_diam);
        query.bindValue(":id_src_diam",id_src_diam);
        query.bindValue(":id_diam",id_diam);
        query.bindValue(":id_matr",m.id_matr);
        query.bindValue(":kvo",m.kvo);
        query.bindValue(":id_vid",m.id_vid);
        query.bindValue(":dat_beg",m.d_beg);
        query.bindValue(":dat_end",m.d_end);
        if (!query.exec()){
            err=query.lastError().text();
        }
    }
    if (!err.isEmpty()){
        QMessageBox::critical(this,tr("Ошибка"),err,QMessageBox::Ok);
    }
    modelNorm->select();
    modelPodtProd->updState();
}

void FormPodtNorm::report()
{
    upd();
    int id_olap=-1;
    int id_type=getIdType();
    if (id_type==1){
        id_olap=35;
    } else if (id_type==2){
        id_olap=37;
    } else if (id_type==3){
        id_olap=46;
    }
    if (id_olap>0 && ready()){
        CubeWidget *w = new CubeWidget(id_olap);
        w->setRange(ui->dateEditBeg->date(),ui->dateEditEnd->date(),true);
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
    }
}

void FormPodtNorm::save()
{
    QString title=ui->comboBoxType->currentText()+" с "+ui->dateEditBeg->date().toString("dd.MM.yyyy")+" по "+ui->dateEditEnd->date().toString("dd.MM.yyyy");
    ui->tableViewPodt->save(title,1,true);
}

void FormPodtNorm::calcSum()
{
    double sum=0;
    for (int i=0; i<modelPodtProd->rowCount(); i++){
        sum+=modelPodtProd->data(modelPodtProd->index(i,14),Qt::EditRole).toDouble();
    }
    ui->labelSum->setText(ui->comboBoxType->currentText()+tr(" итого: ")+QLocale().toString(sum,'f',2)+tr(" кг"));
}

ModelPodtProd::ModelPodtProd(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelPodtProd::refresh(QDate beg, QDate end, int id_type)
{
    QSqlQuery query;
    query.prepare("select p.id_type, pp.id_pr, pp.id_dim, coalesce(wps.id_diam,pp.id_dim) as id_fromdim, p.id_diam, coalesce(wps.id_type,0) as id_type_src, p.id_vol_type, "
                  "wpt.snam, coalesce(wpt2.snam ,'-'), wt.nam, pr.nam, "
                  "d.sdim as diamsrc, coalesce(ds.sdim,d.sdim) as fromdim, dp.sdim, sum(pc.kvo) "
                  "from wire_podt_cont as pc "
                  "inner join wire_podt as p on pc.id_podt=p.id "
                  "inner join prov_buht as b on p.id_buht=b.id "
                  "inner join prov_prih as pp on b.id_prih=pp.id "
                  "inner join provol as pr on pp.id_pr=pr.id "
                  "inner join diam as d on pp.id_dim=d.id "
                  "inner join diam as dp on p.id_diam=dp.id "
                  "inner join wire_podt_type wpt on wpt.id=p.id_type "
                  "inner join wire_type wt on wt.id = p.id_vol_type "
                  "left join wire_podt as wps on wps.id=pc.id_podt_src "
                  "left join diam as ds on ds.id=wps.id_diam "
                  "left join wire_podt_type wpt2 on wpt2.id = wps.id_type "
                  "where p.id_type = :id_type and pc.dat between :d1 and :d2 "
                  "group by p.id_type, p.id_vol_type, pp.id_pr, pp.id_dim, p.id_diam, wpt.snam, wt.nam, pr.nam, d.sdim, dp.sdim, wps.id_diam, ds.sdim, wps.id_type, wpt2.snam "
                  "order by wpt.snam, pr.nam, fromdim, dp.sdim");
    query.bindValue(":id_type",id_type);
    query.bindValue(":d1",beg);
    query.bindValue(":d2",end);
    if (query.exec()){
        setQuery(query);
        setHeaderData(7,Qt::Horizontal,QString::fromUtf8("Тип полуфабриката"));
        setHeaderData(8,Qt::Horizontal,QString::fromUtf8("Исход. полуфабрикат"));
        setHeaderData(9,Qt::Horizontal,QString::fromUtf8("След. волоч."));
        setHeaderData(10,Qt::Horizontal,QString::fromUtf8("Марка"));
        setHeaderData(11,Qt::Horizontal,QString::fromUtf8("Ф кат."));
        setHeaderData(12,Qt::Horizontal,QString::fromUtf8("Ф нач."));
        setHeaderData(13,Qt::Horizontal,QString::fromUtf8("Ф кон."));
        setHeaderData(14,Qt::Horizontal,QString::fromUtf8("Выпуск, кг"));
        updState();
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
    }
}

QVariant ModelPodtProd::data(const QModelIndex &item, int role) const
{
    if (role==Qt::BackgroundColorRole){
        QString s;
        for (int i=0; i<=6; i++){
            if (!s.isEmpty()){
                s+=":";
            }
            s+=this->data(this->index(item.row(),i),Qt::EditRole).toString();
        }
        return exList.contains(s) ? QVariant(QColor(255,255,255)) : QVariant(QColor(255,170,170));
    }
    if (role==Qt::DisplayRole && item.column()==14){
        return QLocale().toString(QSqlQueryModel::data(item,Qt::EditRole).toDouble(),'f',1);
    }
    if (role==Qt::TextAlignmentRole && item.column()==14){
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    return QSqlQueryModel::data(item,role);
}

bool ModelPodtProd::ready()
{
    bool ok=true;
    for (int i=0; i<rowCount(); i++){
        QString s;
        for (int j=0; j<=6; j++){
            if (!s.isEmpty()){
                s+=":";
            }
            s+=this->data(this->index(i,j),Qt::EditRole).toString();
        }
        ok=ok && exList.contains(s);
        if (!ok){
            break;
        }
    }
    return ok;
}

void ModelPodtProd::updState()
{
    QSqlQuery query;
    query.prepare("select distinct id_podt_type||':'||id_provol||':'||id_buht_diam||':'|| id_src_diam||':'|| id_diam || ':' || id_type_src || ':' || id_type_vol as nam from wire_podt_norm order by nam");
    if (query.exec()){
        exList.clear();
        while(query.next()){
            exList.push_back(query.value(0).toString());
        }
        emit dataChanged(this->index(0,0),this->index(rowCount()-1,columnCount()-1));
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
    }
}
