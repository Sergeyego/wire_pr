#include "formpodt.h"
#include "ui_formpodt.h"

FormPodt::FormPodt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPodt)
{
    ui->setupUi(this);
    loadSettings();
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    modelPodtIn = new ModelPodtCex(this);
    ui->tableViewPodtIn->setModel(modelPodtIn);
    ui->tableViewPodtIn->setColumnHidden(0,true);
    ui->tableViewPodtIn->setColumnHidden(1,true);
    ui->tableViewPodtIn->setColumnHidden(2,true);
    ui->tableViewPodtIn->setColumnWidth(3,90);
    ui->tableViewPodtIn->setColumnWidth(4,80);

    modelPodtOut = new ModelPodtCex(this);
    ui->tableViewPodtOut->setModel(modelPodtOut);
    ui->tableViewPodtOut->setColumnHidden(0,true);
    ui->tableViewPodtOut->setColumnHidden(1,true);
    ui->tableViewPodtOut->setColumnHidden(2,true);
    ui->tableViewPodtOut->setColumnWidth(3,90);
    ui->tableViewPodtOut->setColumnWidth(4,80);

    modelPodtDef = new ModelPodtCex(this);
    ui->tableViewPodtDef->setModel(modelPodtDef);
    ui->tableViewPodtDef->setColumnHidden(0,true);
    ui->tableViewPodtDef->setColumnHidden(1,true);
    ui->tableViewPodtDef->setColumnHidden(2,true);
    ui->tableViewPodtDef->setColumnWidth(3,90);
    ui->tableViewPodtDef->setColumnWidth(4,80);

    modelCont = new ModelPodtCont(this);
    ui->tableViewCont->setModel(modelCont);
    ui->tableViewCont->setColumnHidden(0,true);
    ui->tableViewCont->setColumnHidden(1,true);
    ui->tableViewCont->setColumnWidth(2,90);
    ui->tableViewCont->setColumnWidth(3,80);
    ui->tableViewCont->setColumnWidth(4,140);
    ui->tableViewCont->setColumnWidth(5,400);

    modelPodtVol = new ModelPodtVol(this);
    ui->tableViewVol->setModel(modelPodtVol);
    ui->tableViewVol->setColumnHidden(0,true);
    ui->tableViewVol->setColumnWidth(1,90);
    ui->tableViewVol->setColumnWidth(2,80);
    ui->tableViewVol->setColumnWidth(3,80);
    ui->tableViewVol->setColumnWidth(4,140);
    ui->tableViewVol->setColumnWidth(5,140);

    modelPodt = new ModelPodt(this);
    ui->tableView->verticalHeader()->setDefaultSectionSize(ui->tableView->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableView->setModel(modelPodt);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(1,50);
    ui->tableView->setColumnWidth(2,70);
    ui->tableView->setColumnWidth(3,360);
    ui->tableView->setColumnWidth(4,40);
    for (int i=5; i<ui->tableView->model()->columnCount(); i++){
        if (i!=7) {
            ui->tableView->setColumnHidden(i,true);
        }
    }
    ui->tableView->setColumnWidth(7,130);

    modelPodtPart = new ModelPodtPart(this);
    ui->tableViewPart->setModel(modelPodtPart);

    if (!Rels::instance()->relMark->isInital()){
        Rels::instance()->relMark->refreshModel();
    }
    colVal ev;
    ev.val=-1;

    ui->comboBoxFMark->setModel(Rels::instance()->relMark->model());
    ui->comboBoxFDiam->setModel(Rels::instance()->relDiam->model());
    ui->comboBoxFType->setModel(Rels::instance()->relPodtType->model());
    ui->comboBoxFMark->setCurrentData(ev);
    ui->comboBoxFDiam->setCurrentData(ev);
    ui->comboBoxFType->setCurrentData(ev);

    push = new DbMapper(ui->tableView,this);
    ui->horizontalLayoutBtn->insertWidget(0,push);
    push->addMapping(ui->lineEditNum,1);
    push->addMapping(ui->dateEdit,2);
    push->addMapping(ui->comboBoxSrs,3);
    push->addMapping(ui->comboBoxDiam,4);
    push->addMapping(ui->comboBoxLine,5);
    push->addMapping(ui->lineEditComm,6);
    push->addMapping(ui->comboBoxType,7);
    push->addMapping(ui->comboBoxVol,8);
    push->addLock(ui->dateEditBeg);
    push->addLock(ui->dateEditEnd);
    push->addLock(ui->cmdUpd);
    push->addEmptyLock(ui->tableViewCont);
    push->addEmptyLock(ui->tableViewPodtIn);
    push->addEmptyLock(ui->tableViewPodtOut);
    push->addEmptyLock(ui->tableViewVol);
    push->addEmptyLock(ui->tableViewPodtDef);

    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(refresh()));
    connect(push,SIGNAL(currentIndexChanged(int)),this,SLOT(updPart(int)));
    connect(modelCont,SIGNAL(sigSum(QString)),this,SLOT(setContItogo(QString)));
    connect(modelPodtVol,SIGNAL(sigSum(QString)),this,SLOT(setVolItogo(QString)));
    connect(modelPodtIn,SIGNAL(sigSum(QString)),this,SLOT(setInItogo(QString)));
    connect(modelPodtOut,SIGNAL(sigSum(QString)),this,SLOT(setOutItogo(QString)));
    connect(modelPodtDef,SIGNAL(sigSum(QString)),this,SLOT(setDefItogo(QString)));
    connect(ui->comboBoxFMark,SIGNAL(currentIndexChanged(int)),this,SLOT(refresh()));
    connect(ui->comboBoxFDiam,SIGNAL(currentIndexChanged(int)),this,SLOT(refresh()));
    connect(ui->comboBoxFType,SIGNAL(currentIndexChanged(int)),this,SLOT(refresh()));
    connect(ui->comboBoxFMark,SIGNAL(currentTextChanged(QString)),this,SLOT(fltChanged(QString)));
    connect(ui->comboBoxFDiam,SIGNAL(currentTextChanged(QString)),this,SLOT(fltChanged(QString)));
    connect(ui->comboBoxFType,SIGNAL(currentTextChanged(QString)),this,SLOT(fltChanged(QString)));
    refresh();
}

FormPodt::~FormPodt()
{
    saveSettings();
    delete ui;
}

void FormPodt::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    this->ui->splitter->restoreState(settings.value("podt_splitter_width").toByteArray());
}

void FormPodt::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    settings.setValue("podt_splitter_width",ui->splitter->saveState());
}

void FormPodt::refresh()
{
    if (sender()==ui->cmdUpd){
        modelPodt->refreshRelsModel();
        modelCont->refreshRelsModel();
        Rels::instance()->relMark->refreshModel();
    }

    int id_mark=ui->comboBoxFMark->getCurrentData().val.toInt();
    int id_diam=ui->comboBoxFDiam->getCurrentData().val.toInt();
    int id_type=ui->comboBoxFType->getCurrentData().val.toInt();
    modelPodt->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),id_mark,id_diam,id_type);
}

void FormPodt::fltChanged(QString s)
{
    if (s.isEmpty()){
        DbComboBox *combo = qobject_cast<DbComboBox *>(sender());
        if (combo) {
            colVal ev;
            ev.val=-1;
            combo->setCurrentData(ev);
        }
    }
}

void FormPodt::updPart(int index)
{
    int id_podt=ui->tableView->model()->data(ui->tableView->model()->index(index,0),Qt::EditRole).toInt();
    QDate date=ui->tableView->model()->data(ui->tableView->model()->index(index,2),Qt::EditRole).toDate();
    modelCont->setDefaultValue(2,date);
    modelCont->refresh(id_podt);
    modelPodtVol->refresh(id_podt);
    modelPodtPart->refresh(id_podt);
    ui->tableViewPart->resizeToContents();
    modelPodtIn->refresh(id_podt,1);
    modelPodtOut->refresh(id_podt,2);
    modelPodtDef->refresh(id_podt,3);
}

void FormPodt::setContItogo(QString s)
{
    ui->groupBoxCont->setTitle(s);
}

void FormPodt::setVolItogo(QString s)
{
    ui->groupBoxVol->setTitle(s);
}

void FormPodt::setInItogo(QString s)
{
    ui->groupBoxPodtIn->setTitle(s);
}

void FormPodt::setOutItogo(QString s)
{
    ui->groupBoxPodtOut->setTitle(s);
}

void FormPodt::setDefItogo(QString s)
{
    ui->groupBoxDef->setTitle(s);
}

ModelPodt::ModelPodt(QObject *parent) : DbTableModel("wire_podt",parent)
{
    addColumn("id","id");
    addColumn("n_s",tr("№"));
    addColumn("dat",tr("Дата"));
    addColumn("id_buht",tr("Исх. партия"),Rels::instance()->relSrcPart);
    addColumn("id_diam",tr("Ф"),Rels::instance()->relDiam);
    addColumn("id_line",tr("Стан"),Rels::instance()->relLine);
    addColumn("comment",tr("Комментарий"));
    addColumn("id_type",tr("Тип"),Rels::instance()->relPodtType);
    addColumn("id_vol_type",tr("Волочение"),Rels::instance()->relType);
    setSuffix("left join prov_prih on prov_prih.id = prov_buht.id_prih");
    setSort("dat,n_s");
    setDefaultValue(7,1);
    connect(this,SIGNAL(sigUpd()),Rels::instance()->relPodt,SLOT(refreshModel()));
}

void ModelPodt::refresh(QDate beg, QDate end, int id_mark, int id_diam, int id_type)
{
    QString flt="wire_podt.dat between '"+beg.toString("yyyy.MM.dd")+"' and '"+end.toString("yyyy.MM.dd")+"'";
    if (id_mark>0){
        flt+=" and prov_prih.id_pr = "+QString::number(id_mark);
    }
    if (id_diam>0){
        flt+=" and wire_podt.id_diam = "+QString::number(id_diam);
    }
    if (id_type>0){
        flt+=" and wire_podt.id_type = "+QString::number(id_type);
    }
    setFilter(flt);
    select();
}

bool ModelPodt::insertRow(int row, const QModelIndex &parent)
{
    select();
    int old_num=0;
    if (rowCount()>0) old_num=this->data(this->index(rowCount()-1,1),Qt::EditRole).toInt();
    QString num = QString::number(old_num+1);
    num=num.rightJustified(4,'0',true);
    setDefaultValue(1,num);
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
    addColumn("id_rab",tr("Работник"),Rels::instance()->relVol);
    addColumn("id_podt_src",tr("Исходный полуфабрикат"),Rels::instance()->relPodt);
    setSort("wire_podt_cont.dat");
    setDecimals(3,2);
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

ModelPodtVol::ModelPodtVol(QObject *parent) : DbTableModel("wire_podt_out",parent)
{
    addColumn("id_podt",tr("id_podt"));
    addColumn("dat",tr("Дата"));
    addColumn("kvo",tr("Масса, кг"));
    addColumn("kvo_defect",tr("Брак, кг"));
    addColumn("id_line",tr("Линия"),Rels::instance()->relLine);
    addColumn("id_vol",tr("Волочильщик"),Rels::instance()->relVol);
    setDecimals(2,2);
    setDecimals(3,2);
    setSort("wire_podt_out.dat");
    connect(this,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(this,SIGNAL(sigRefresh()),this,SLOT(calcSum()));
}

void ModelPodtVol::refresh(int id_part)
{
    setDefaultValue(0,id_part);
    setFilter("wire_podt_out.id_podt = "+QString::number(id_part));
    select();
}

void ModelPodtVol::calcSum()
{
    double sum=0;
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,2),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (tr("Волочение итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : tr("Волочение");

    double def=0;
    for (int i=0; i<rowCount(); i++){
        def+=data(index(i,3),Qt::EditRole).toDouble();
    }
    if (def>0){
        s+=(tr(" (Брак: ")+QLocale().toString(def,'f',2)+tr(" кг)"));
    }
    emit sigSum(s);
}

ModelPodtCex::ModelPodtCex(QObject *parent) : DbTableModel("wire_podt_cex",parent)
{
    addColumn("id","id");
    addColumn("id_podt","id_podt");
    addColumn("id_op","id_op");
    addColumn("dat",tr("Дата"));
    addColumn("kvo",tr("Кол-во, кг"));
    setDecimals(4,2);
    setSort("wire_podt_cex.dat");
    connect(this,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(this,SIGNAL(sigRefresh()),this,SLOT(calcSum()));
}

void ModelPodtCex::refresh(int id_podt, int id_op)
{
    setDefaultValue(1,id_podt);
    setDefaultValue(2,id_op);
    setFilter(QString("wire_podt_cex.id_podt = %1 and wire_podt_cex.id_op = %2").arg(id_podt).arg(id_op));
    select();
}

void ModelPodtCex::calcSum()
{
    double sum=0;
    QString title=Rels::instance()->relPodtOp->getDisplayValue(this->defaultValue(2));
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,4),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (title + tr(" итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : title;
    emit sigSum(s);
}
