#include "formnorm.h"
#include "ui_formnorm.h"

FormNorm::FormNorm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormNorm)
{
    ui->setupUi(this);

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    ui->comboBoxType->setModel(Models::instance()->relAddType->model());
    ui->comboBoxType->setModelColumn(1);

    modelNorm = new DbTableModel("wire_norm",this);
    modelNorm->addColumn("id_add_type","id_add_type");
    modelNorm->addColumn("id_line","id_line");
    modelNorm->addColumn("id_provol","id_provol");
    modelNorm->addColumn("id_diam","id_diam");
    modelNorm->addColumn("id_spool","id_spool");
    modelNorm->addColumn("id_pack","id_pack");
    modelNorm->addColumn("id_matr",tr("Материал"),Models::instance()->relMatr);
    modelNorm->addColumn("kvo",tr("Норма"));
    modelNorm->addColumn("id_vid",tr("Вид затрат"),Models::instance()->relRasxVid);
    modelNorm->addColumn("dat_beg",tr("Дата нач."));
    modelNorm->addColumn("dat_end",tr("Дата кон."));

    modelNorm->setSort("id_vid, id_matr");
    modelNorm->setDecimals(7,3);
    modelNorm->setDefaultValue(10,QDate());

    ui->tableViewNorm->setModel(modelNorm);
    for (int i=0; i<=5; i++){
        ui->tableViewNorm->setColumnHidden(i,true);
    }
    ui->tableViewNorm->setColumnWidth(6,330);
    ui->tableViewNorm->setColumnWidth(7,80);
    ui->tableViewNorm->setColumnWidth(8,200);
    ui->tableViewNorm->setColumnWidth(9,80);
    ui->tableViewNorm->setColumnWidth(10,80);

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
}

FormNorm::~FormNorm()
{
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

void FormNorm::upd()
{
    modelProd->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),getIdType());
    if (ui->tableViewProd->model()->rowCount()){
        ui->tableViewProd->selectRow(0);
    }
    for (int i=0; i<=5; i++){
        ui->tableViewProd->setColumnHidden(i,true);
    }
    ui->tableViewProd->resizeToContents();
    calcSum();
}

void FormNorm::updNorm(QModelIndex ind)
{
    int id_type=currentData(0).toInt();
    int id_line=currentData(1).toInt();
    int id_provol=currentData(2).toInt();
    int id_diam=currentData(3).toInt();
    int id_spool=currentData(4).toInt();
    int id_pack=currentData(5).toInt();

    modelNorm->setDefaultValue(0,id_type);
    modelNorm->setDefaultValue(1,id_line);
    modelNorm->setDefaultValue(2,id_provol);
    modelNorm->setDefaultValue(3,id_diam);
    modelNorm->setDefaultValue(4,id_spool);
    modelNorm->setDefaultValue(5,id_pack);

    modelNorm->setFilter(QString("id_add_type = %1 and id_line = %2 and id_provol = %3 and id_diam = %4 and id_spool = %5 and id_pack = %6")
                         .arg(id_type).arg(id_line).arg(id_provol).arg(id_diam).arg(id_spool).arg(id_pack));
    modelNorm->select();
}

void FormNorm::copy()
{
    buf.clear();
    for (int i=0; i<modelNorm->rowCount();i++){
        pnorm m;
        m.id_matr=modelNorm->data(modelNorm->index(i,6),Qt::EditRole).toInt();
        m.kvo=modelNorm->data(modelNorm->index(i,7),Qt::EditRole).toDouble();
        m.id_vid=modelNorm->data(modelNorm->index(i,8),Qt::EditRole).toInt();
        m.d_beg=modelNorm->data(modelNorm->index(i,9),Qt::EditRole).toDate();
        m.d_end=modelNorm->data(modelNorm->index(i,10),Qt::EditRole).toDate();
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
    QString err;
    foreach (pnorm m, buf) {
        QSqlQuery query;
        query.prepare("insert into wire_norm (id_add_type, id_line, id_provol, id_diam, id_spool, id_pack, id_matr, kvo, id_vid, dat_beg, dat_end) values "
                      "(:id_add_type, :id_line, :id_provol, :id_diam, :id_spool, :id_pack, :id_matr, :kvo, :id_vid, :dat_beg, :dat_end)");
        query.bindValue(":id_add_type",id_type);
        query.bindValue(":id_line",id_line);
        query.bindValue(":id_provol",id_provol);
        query.bindValue(":id_diam",id_diam);
        query.bindValue(":id_spool",id_spool);
        query.bindValue(":id_pack",id_pack);
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
        id_olap=39;
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
        sum+=modelProd->data(modelProd->index(i,12),Qt::EditRole).toDouble();
    }
    ui->labelSum->setText(ui->comboBoxType->currentText()+tr(" итого: ")+QLocale().toString(sum,'f',2)+tr(" кг"));
}
