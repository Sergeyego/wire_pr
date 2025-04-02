#include "formnorm.h"
#include "ui_formnorm.h"

FormNorm::FormNorm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormNorm)
{
    ui->setupUi(this);
    loadSettings();

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    if (!Rels::instance()->relAddType->isInital()){
        Rels::instance()->relAddType->refreshModel();
    }
    ui->comboBoxType->setModel(Rels::instance()->relAddType->model());
    ui->comboBoxType->setEditable(false);
    colVal t;
    t.val=3;
    ui->comboBoxType->setCurrentData(t);

    modelNorm = new DbTableModel("wire_norm",this);
    modelNorm->addColumn("id_add_type","id_add_type");
    modelNorm->addColumn("id_line","id_line");
    modelNorm->addColumn("id_provol","id_provol");
    modelNorm->addColumn("id_diam","id_diam");
    modelNorm->addColumn("id_spool","id_spool");
    modelNorm->addColumn("id_pack","id_pack");
    modelNorm->addColumn("id_provol_src","id_provol_src");
    modelNorm->addColumn("id_podt_type","id_podt_type");
    modelNorm->addColumn("id_podt_type_src","id_podt_type_src");
    modelNorm->addColumn("id_diam_src","id_diam_src");
    modelNorm->addColumn("id_diam_podt","id_diam_podt");
    modelNorm->addColumn("id_matr",tr("Материал"),Rels::instance()->relMatr);
    modelNorm->addColumn("kvo",tr("Норма"));
    modelNorm->addColumn("id_vid",tr("Вид затрат"),Rels::instance()->relRasxVid);
    modelNorm->addColumn("dat_beg",tr("Дата нач."));
    modelNorm->addColumn("dat_end",tr("Дата кон."));

    modelNorm->setSort("id_vid, id_matr, dat_beg");
    modelNorm->setDecimals(12,3);
    modelNorm->setDefaultValue(15,QDate());

    ui->tableViewNorm->setModel(modelNorm);
    for (int i=0; i<=10; i++){
        ui->tableViewNorm->setColumnHidden(i,true);
    }
    ui->tableViewNorm->setColumnWidth(11,330);
    ui->tableViewNorm->setColumnWidth(12,80);
    ui->tableViewNorm->setColumnWidth(13,200);
    ui->tableViewNorm->setColumnWidth(14,80);
    ui->tableViewNorm->setColumnWidth(15,80);

    modelProd = new ModelProd(this);
    ui->tableViewProd->setModel(modelProd);

    connect(ui->tableViewProd->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(updNorm(QModelIndex)));
    upd();

    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->pushButtonCopy,SIGNAL(clicked(bool)),this,SLOT(copy()));
    connect(ui->pushButtonPaste,SIGNAL(clicked(bool)),this,SLOT(paste()));
    connect(ui->pushButtonRep,SIGNAL(clicked(bool)),this,SLOT(report()));
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this,SLOT(save()));
    connect(modelNorm,SIGNAL(sigUpd()),modelProd,SLOT(updState()));
    connect(ui->comboBoxType,SIGNAL(currentIndexChanged(int)),this,SLOT(upd()));
}

FormNorm::~FormNorm()
{
    saveSettings();
    delete ui;
}

QVariant FormNorm::currentData(int column)
{
    int row=ui->tableViewProd->currentIndex().row();
    return ui->tableViewProd->model()->data(ui->tableViewProd->model()->index(row,column),Qt::EditRole);
}

int FormNorm::getIdType()
{
    return ui->comboBoxType->model()->data(ui->comboBoxType->model()->index(ui->comboBoxType->currentIndex(),0),Qt::EditRole).toInt();
}

bool FormNorm::ready()
{
    bool ok=modelProd->ready();
    if (!ok){
        QMessageBox::information(this,QString::fromUtf8("Предупреждение"),QString::fromUtf8("Не для всех марок указаны нормы!"),QMessageBox::Ok);
    }
    return ok;
}

void FormNorm::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    this->ui->splitter->restoreState(settings.value("norm_splitter_width").toByteArray());
}

void FormNorm::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    settings.setValue("norm_splitter_width",ui->splitter->saveState());
}

void FormNorm::upd()
{
    if (sender()==ui->pushButtonUpd){
        Rels::instance()->relAddType->refreshModel();
        modelNorm->refreshRelsModel();
    }
    modelProd->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),getIdType());
    if (ui->tableViewProd->model()->rowCount()){
        ui->tableViewProd->selectRow(0);
    }
    for (int i=0; i<=11; i++){
        ui->tableViewProd->setColumnHidden(i,true);
    }
    ui->tableViewProd->resizeToContents();
    calcSum();
}

void FormNorm::updNorm(QModelIndex /*ind*/)
{
    int id_type=currentData(0).toInt();
    int id_line=currentData(1).toInt();
    int id_provol=currentData(2).toInt();
    int id_diam=currentData(3).toInt();
    int id_spool=currentData(4).toInt();
    int id_pack=currentData(5).toInt();
    int id_provol_src=currentData(6).toInt();
    int id_podt_type=currentData(7).toInt();
    int id_podt_type_src=currentData(8).toInt();
    int id_diam_src=currentData(9).toInt();
    int id_diam_podt=currentData(10).toInt();

    modelNorm->setDefaultValue(0,id_type);
    modelNorm->setDefaultValue(1,id_line);
    modelNorm->setDefaultValue(2,id_provol);
    modelNorm->setDefaultValue(3,id_diam);
    modelNorm->setDefaultValue(4,id_spool);
    modelNorm->setDefaultValue(5,id_pack);
    modelNorm->setDefaultValue(6,id_provol_src);
    modelNorm->setDefaultValue(7,id_podt_type);
    modelNorm->setDefaultValue(8,id_podt_type_src);
    modelNorm->setDefaultValue(9,id_diam_src);
    modelNorm->setDefaultValue(10,id_diam_podt);

    modelNorm->setFilter(QString("id_add_type = %1 and id_line = %2 and id_provol = %3 and id_diam = %4 and id_spool = %5 and id_pack = %6 and id_provol_src = %7"
                                 " and id_podt_type = %8 and id_podt_type_src = %9 and id_diam_src = %10 and id_diam_podt = %11")
                         .arg(id_type).arg(id_line).arg(id_provol).arg(id_diam).arg(id_spool).arg(id_pack).arg(id_provol_src).arg(id_podt_type).arg(id_podt_type_src).arg(id_diam_src).arg(id_diam_podt));
    modelNorm->select();
}

void FormNorm::copy()
{
    buf.clear();
    for (int i=0; i<modelNorm->rowCount();i++){
        pnorm m;
        m.id_matr=modelNorm->data(modelNorm->index(i,11),Qt::EditRole).toInt();
        m.kvo=modelNorm->data(modelNorm->index(i,12),Qt::EditRole).toDouble();
        m.id_vid=modelNorm->data(modelNorm->index(i,13),Qt::EditRole).toInt();
        m.d_beg=modelNorm->data(modelNorm->index(i,14),Qt::EditRole).toDate();
        m.d_end=modelNorm->data(modelNorm->index(i,15),Qt::EditRole).toDate();
        buf.push_back(m);
    }
    ui->pushButtonPaste->setEnabled(true);
}

void FormNorm::paste()
{
    int id_type=currentData(0).toInt();
    int id_line=currentData(1).toInt();
    int id_provol=currentData(2).toInt();
    int id_diam=currentData(3).toInt();
    int id_spool=currentData(4).toInt();
    int id_pack=currentData(5).toInt();
    int id_provol_src=currentData(6).toInt();
    int id_podt_type=currentData(7).toInt();
    int id_podt_type_src=currentData(8).toInt();
    int id_diam_src=currentData(9).toInt();
    int id_diam_podt=currentData(10).toInt();
    QString err;
    foreach (pnorm m, buf) {
        QSqlQuery query;
        query.prepare("insert into wire_norm (id_add_type, id_line, id_provol, id_diam, id_spool, id_pack, id_provol_src, id_podt_type, id_podt_type_src, id_diam_src, id_diam_podt, id_matr, kvo, id_vid, dat_beg, dat_end) values "
                      "(:id_add_type, :id_line, :id_provol, :id_diam, :id_spool, :id_pack, :id_provol_src, :id_podt_type, :id_podt_type_src, :id_diam_src, :id_diam_podt, :id_matr, :kvo, :id_vid, :dat_beg, :dat_end)");
        query.bindValue(":id_add_type",id_type);
        query.bindValue(":id_line",id_line);
        query.bindValue(":id_provol",id_provol);
        query.bindValue(":id_diam",id_diam);
        query.bindValue(":id_spool",id_spool);
        query.bindValue(":id_pack",id_pack);        
        query.bindValue(":id_provol_src",id_provol_src);
        query.bindValue(":id_podt_type",id_podt_type);
        query.bindValue(":id_podt_type_src",id_podt_type_src);
        query.bindValue(":id_diam_src",id_diam_src);
        query.bindValue(":id_diam_podt",id_diam_podt);
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
    modelProd->updState();
}

void FormNorm::report()
{
    upd();
    int id_olap=-1;
    int id_type=getIdType();
    if (id_type==3){
        id_olap=84;
    } else if (id_type==13){
        id_olap=40;
    } else if (id_type==20){
        id_olap=41;
    } else if (id_type==23){
        id_olap=42;
    }
    if (id_olap>0 && ready()){
        CubeWidget *w = new CubeWidget(id_olap);
        w->setRange(ui->dateEditBeg->date(),ui->dateEditEnd->date(),true);
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
    }
}

void FormNorm::save()
{
    QString title=ui->comboBoxType->currentText()+" с "+ui->dateEditBeg->date().toString("dd.MM.yyyy")+" по "+ui->dateEditEnd->date().toString("dd.MM.yyyy");
    ui->tableViewProd->save(title,1,true);
}

void FormNorm::calcSum()
{
    double sum=0;
    for (int i=0; i<modelProd->rowCount(); i++){
        sum+=modelProd->data(modelProd->index(i,20),Qt::EditRole).toDouble();
    }
    ui->labelSum->setText(ui->comboBoxType->currentText()+tr(" итого: ")+QLocale().toString(sum,'f',2)+tr(" кг"));
}

ModelProd::ModelProd(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelProd::refresh(QDate beg, QDate end, int id_type)
{
    QSqlQuery query;
    query.prepare("select w.id_type, m.id_type, m.id_provol, m.id_diam, p.id_pack, p.id_pack_type, pp.id_pr, coalesce(wp2.id_type,0), coalesce(ps.id_type,0), pp.id_dim, coalesce(wp2.id_diam,0), "
                  "t.short, l.snam, pr.nam, d.sdim, k.short, wp.pack_ed, pr2.nam||' ф '||d2.sdim, coalesce(wpt.snam||' ф '||d3.sdim,'-'), coalesce(wpt2.snam,'-'), sum(w.m_netto) "
                  "from wire_in_cex_data w "
                  "inner join wire_in_cex_type t on w.id_type=t.id and t.koef=1 "
                  "inner join wire_parti p on w.id_wparti=p.id "
                  "inner join wire_parti_m m on p.id_m=m.id "
                  "inner join wire_line l on m.id_type=l.id "
                  "inner join provol pr on pr.id=m.id_provol "
                  "inner join diam d on d.id=m.id_diam "
                  "inner join wire_pack_kind k on p.id_pack=k.id "
                  "inner join wire_pack wp on wp.id=p.id_pack_type "
                  "inner join prov_buht pb on pb.id = m.id_buht "
                  "inner join prov_prih pp on pp.id = pb.id_prih "
                  "inner join provol pr2 on pr2.id = pp.id_pr "
                  "inner join diam d2 on d2.id = pp.id_dim "
                  "left join wire_podt wp2 on wp2.id = m.id_podt "
                  "left join diam d3 on d3.id = wp2.id_diam "
                  "left join wire_podt_type wpt on wpt.id = wp2.id_type "
                  "left join ( "
                  "select wpc.id_podt as id_podt, max(wp.id_type) as id_type "
                  "from wire_podt_cont wpc "
                  "inner join wire_podt wp on wp.id = wpc.id_podt_src "
                  "group by wpc.id_podt "
                  ") as ps on ps.id_podt = wp2.id "
                  "left join wire_podt_type wpt2 on wpt2.id = ps.id_type "
                  "where w.id_type = :id_type and w.dat between :d1 and :d2 "
                  "group by w.id_type, m.id_type, m.id_provol, m.id_diam, p.id_pack, p.id_pack_type, pp.id_pr, wp2.id_type, ps.id_type, pp.id_dim, wp2.id_diam, "
                  "t.short, l.snam, pr.nam, d.sdim, k.short, wp.pack_ed, pr2.nam, wpt.snam, wpt2.snam, d2.sdim, d3.sdim "
                  "order by t.short, l.snam, pr.nam, d.sdim, k.short, wp.pack_ed, pr2.nam, wpt.snam, wpt2.snam");
    query.bindValue(":id_type",id_type);
    query.bindValue(":d1",beg);
    query.bindValue(":d2",end);
    if (query.exec()){
        setQuery(query);
        setHeaderData(11,Qt::Horizontal,QString::fromUtf8("Тип продукции"));
        setHeaderData(12,Qt::Horizontal,QString::fromUtf8("Стан"));
        setHeaderData(13,Qt::Horizontal,QString::fromUtf8("Марка"));
        setHeaderData(14,Qt::Horizontal,QString::fromUtf8("Диам."));
        setHeaderData(15,Qt::Horizontal,QString::fromUtf8("Носитель"));
        setHeaderData(16,Qt::Horizontal,QString::fromUtf8("Упаковка"));
        setHeaderData(17,Qt::Horizontal,QString::fromUtf8("Исх. марка"));
        setHeaderData(18,Qt::Horizontal,QString::fromUtf8("Полуфабрикат"));
        setHeaderData(19,Qt::Horizontal,QString::fromUtf8("Исх. полуфаб."));
        setHeaderData(20,Qt::Horizontal,QString::fromUtf8("Выпуск, кг"));
        updState();
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
    }
}

QVariant ModelProd::data(const QModelIndex &item, int role) const
{
    if (role==Qt::BackgroundRole){
        QString s;
        for (int i=0; i<=10; i++){
            if (!s.isEmpty()){
                s+=":";
            }
            s+=this->data(this->index(item.row(),i),Qt::EditRole).toString();
        }
        return exList.contains(s) ? QVariant(QColor(255,255,255)) : QVariant(QColor(255,170,170));
    }
    if (role==Qt::DisplayRole && item.column()==20){
        return QLocale().toString(QSqlQueryModel::data(item,Qt::EditRole).toDouble(),'f',1);
    }
    if (role==Qt::TextAlignmentRole && item.column()==20){
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    return QSqlQueryModel::data(item,role);
}

bool ModelProd::ready()
{
    bool ok=true;
    for (int i=0; i<rowCount(); i++){
        QString s;
        for (int j=0; j<=10; j++){
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

void ModelProd::updState()
{
    QSqlQuery query;
    query.prepare("select distinct id_add_type||':'||id_line||':'||id_provol||':'||id_diam||':'||id_spool||':'||id_pack||':'||id_provol_src||':'||id_podt_type||':'||id_podt_type_src||':'||id_diam_src||':'||id_diam_podt as nam from wire_norm order by nam");
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
