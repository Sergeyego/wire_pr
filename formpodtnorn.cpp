#include "formpodtnorn.h"
#include "ui_formpodtnorn.h"

FormPodtNorn::FormPodtNorn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPodtNorn)
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
    modelNorm->addColumn("id_matr",tr("Материал"),Models::instance()->relMatr);
    modelNorm->addColumn("kvo",tr("Норма"));
    modelNorm->addColumn("id_vid",tr("Вид затрат"),Models::instance()->relRasxVid);
    modelNorm->addColumn("dat_beg",tr("Дата нач."));
    modelNorm->addColumn("dat_end",tr("Дата кон."));

    modelNorm->setSort("id_vid, id_matr, dat_beg");
    modelNorm->setDecimals(6,3);
    modelNorm->setDefaultValue(9,QDate());

    ui->tableViewNorm->setModel(modelNorm);
    for (int i=0; i<=4; i++){
        ui->tableViewNorm->setColumnHidden(i,true);
    }
    ui->tableViewNorm->setColumnWidth(5,330);
    ui->tableViewNorm->setColumnWidth(6,80);
    ui->tableViewNorm->setColumnWidth(7,200);
    ui->tableViewNorm->setColumnWidth(8,80);
    ui->tableViewNorm->setColumnWidth(9,80);
    ui->tableViewNorm->setColumnWidth(10,80);

    modelPodtProd = new ModelPodtProd(this);
    ui->tableViewPodt->setModel(modelPodtProd);

    connect(ui->tableViewPodt->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(updNorm(QModelIndex)));
    upd();

    for (int i=0; i<=4; i++){
        ui->tableViewPodt->setColumnHidden(i,true);
    }
    ui->tableViewPodt->setColumnWidth(5,220);
    ui->tableViewPodt->setColumnWidth(6,160);
    ui->tableViewPodt->setColumnWidth(7,60);
    ui->tableViewPodt->setColumnWidth(8,60);
    ui->tableViewPodt->setColumnWidth(9,60);
    ui->tableViewPodt->setColumnWidth(10,90);

    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->pushButtonCopy,SIGNAL(clicked(bool)),this,SLOT(copy()));
    connect(ui->pushButtonPaste,SIGNAL(clicked(bool)),this,SLOT(paste()));
    connect(ui->pushButtonRep,SIGNAL(clicked(bool)),this,SLOT(report()));
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this,SLOT(save()));
    connect(modelNorm,SIGNAL(sigUpd()),modelPodtProd,SLOT(updState()));
}

FormPodtNorn::~FormPodtNorn()
{
    delete ui;
}

QVariant FormPodtNorn::currentData(int column)
{
    int row=ui->tableViewPodt->currentIndex().row();
    return ui->tableViewPodt->model()->data(ui->tableViewPodt->model()->index(row,column),Qt::EditRole);
}

int FormPodtNorn::getIdType()
{
    return ui->comboBoxType->model()->data(ui->comboBoxType->model()->index(ui->comboBoxType->currentIndex(),0),Qt::EditRole).toInt();
}

bool FormPodtNorn::ready()
{
    bool ok=modelPodtProd->ready();
    if (!ok){
        QMessageBox::information(this,QString::fromUtf8("Предупреждение"),QString::fromUtf8("Не для всех марок указаны нормы!"),QMessageBox::Ok);
    }
    return ok;
}

void FormPodtNorn::upd()
{
    modelPodtProd->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),getIdType());
    if (ui->tableViewPodt->model()->rowCount()){
        ui->tableViewPodt->selectRow(0);
    }
    calcSum();
}

void FormPodtNorn::updNorm(QModelIndex /*ind*/)
{
    int id_type=currentData(0).toInt();
    int id_provol=currentData(1).toInt();
    int id_buht_diam=currentData(2).toInt();
    int id_src_diam=currentData(3).toInt();
    int id_diam=currentData(4).toInt();
    modelNorm->setDefaultValue(0,id_type);
    modelNorm->setDefaultValue(1,id_provol);
    modelNorm->setDefaultValue(2,id_buht_diam);
    modelNorm->setDefaultValue(3,id_src_diam);
    modelNorm->setDefaultValue(4,id_diam);
    modelNorm->setFilter(QString("id_podt_type = %1 and id_provol = %2 and id_src_diam = %3 and id_diam = %4 and id_buht_diam = %5 ").arg(id_type).arg(id_provol).arg(id_src_diam).arg(id_diam).arg(id_buht_diam));
    modelNorm->select();
}

void FormPodtNorn::copy()
{
    buf.clear();
    for (int i=0; i<modelNorm->rowCount();i++){
        mnorm m;
        m.id_matr=modelNorm->data(modelNorm->index(i,5),Qt::EditRole).toInt();
        m.kvo=modelNorm->data(modelNorm->index(i,6),Qt::EditRole).toDouble();
        m.id_vid=modelNorm->data(modelNorm->index(i,7),Qt::EditRole).toInt();
        m.d_beg=modelNorm->data(modelNorm->index(i,8),Qt::EditRole).toDate();
        m.d_end=modelNorm->data(modelNorm->index(i,9),Qt::EditRole).toDate();
        buf.push_back(m);
    }
    ui->pushButtonPaste->setEnabled(true);
}

void FormPodtNorn::paste()
{
    int id_type=currentData(0).toInt();
    int id_provol=currentData(1).toInt();
    int id_buht_diam=currentData(2).toInt();
    int id_src_diam=currentData(3).toInt();
    int id_diam=currentData(4).toInt();
    QString err;
    foreach (mnorm m, buf) {
        QSqlQuery query;
        query.prepare("insert into wire_podt_norm (id_podt_type, id_provol, id_buht_diam, id_src_diam, id_diam, id_matr, kvo, id_vid, dat_beg, dat_end) values "
                      "(:id_podt_type, :id_provol, :id_buht_diam, :id_src_diam, :id_diam, :id_matr, :kvo, :id_vid, :dat_beg, :dat_end)");
        query.bindValue(":id_podt_type",id_type);
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

void FormPodtNorn::report()
{
    upd();
    int id_olap=-1;
    int id_type=getIdType();
    if (id_type==1){
        id_olap=35;
    } else if (id_type==2){
        id_olap=37;
    }
    if (id_olap>0 && ready()){
        CubeWidget *w = new CubeWidget(id_olap);
        w->setRange(ui->dateEditBeg->date(),ui->dateEditEnd->date(),true);
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
    }
}

void FormPodtNorn::save()
{
    QString title=ui->comboBoxType->currentText()+" с "+ui->dateEditBeg->date().toString("dd.MM.yyyy")+" по "+ui->dateEditEnd->date().toString("dd.MM.yyyy");
    ui->tableViewPodt->save(title,1,true);
}

void FormPodtNorn::calcSum()
{
    double sum=0;
    for (int i=0; i<modelPodtProd->rowCount(); i++){
        sum+=modelPodtProd->data(modelPodtProd->index(i,10),Qt::EditRole).toDouble();
    }
    ui->labelSum->setText(ui->comboBoxType->currentText()+tr(" итого: ")+QLocale().toString(sum,'f',2)+tr(" кг"));
}
