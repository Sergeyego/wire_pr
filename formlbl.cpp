#include "formlbl.h"
#include "ui_formlbl.h"

FormLbl::FormLbl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormLbl)
{
    ui->setupUi(this);
    ui->dateEditPart->setDate(QDate::currentDate());

    setComboBoxModel(ui->comboBoxMar,Rels::instance()->relProvol);
    setComboBoxModel(ui->comboBoxDiam,Rels::instance()->relDiam);
    setComboBoxModel(ui->comboBoxPack,Rels::instance()->relPack);

    ui->comboBoxNam->setModel(Rels::instance()->modelNam);
    ui->comboBoxNam->setModelColumn(1);
    ui->comboBoxNam->completer()->setCompletionMode(QCompleter::PopupCompletion);
    ui->comboBoxNam->completer()->setCaseSensitivity(Qt::CaseInsensitive);

    ui->lineEditMas->setValidator(new QDoubleValidator(0,999999999,3,this));

    ui->lineEditOrigPart->setValidator(new QIntValidator(0,9999,this));
    ui->lineEditYear->setValidator(new QIntValidator(0,99,this));

    modelOtk = new DbTableModel("otk",this);
    modelOtk->addColumn("id",tr("id"));
    modelOtk->addColumn("num",tr("№"));
    modelOtk->addColumn("nam",tr("ОТК"));
    modelOtk->setSort("otk.nam");
    modelOtk->select();
    ui->tableViewOtk->setModel(modelOtk);
    ui->tableViewOtk->setColumnHidden(0,true);
    ui->tableViewOtk->setColumnWidth(1,40);
    ui->tableViewOtk->setColumnWidth(2,150);

    modelNam = new DbTableModel("wire_namoch",this);
    modelNam->addColumn("id",tr("id"));
    modelNam->addColumn("num",tr("№"));
    modelNam->addColumn("id_rab",tr("ФИО"),Rels::instance()->relRab);
    modelNam->addColumn("id_pr",tr("id_pr"));
    modelNam->setFilter("wire_namoch.id_pr = 3");
    modelNam->setSort("wire_namoch.num");
    modelNam->setDefaultValue(3,3);
    modelNam->select();
    ui->tableViewNam->setModel(modelNam);
    ui->tableViewNam->setColumnHidden(0,true);
    ui->tableViewNam->setColumnWidth(1,40);
    ui->tableViewNam->setColumnWidth(2,290);
    ui->tableViewNam->setColumnHidden(3,true);

    modelVol = new DbTableModel("wire_namoch",this);
    modelVol->addColumn("id",tr("id"));
    modelVol->addColumn("num",tr("№"));
    modelVol->addColumn("id_rab",tr("ФИО"),Rels::instance()->relRab);
    modelVol->addColumn("id_pr",tr("id_pr"));
    modelVol->setFilter("wire_namoch.id_pr = 2");
    modelVol->setSort("wire_namoch.num");
    modelVol->setDefaultValue(3,2);
    modelVol->select();
    ui->tableViewVol->setModel(modelVol);
    ui->tableViewVol->setColumnHidden(0,true);
    ui->tableViewVol->setColumnWidth(1,40);
    ui->tableViewVol->setColumnWidth(2,290);
    ui->tableViewVol->setColumnHidden(3,true);

    connect(ui->pushButtonLbl1,SIGNAL(clicked(bool)),this,SLOT(goLbl1()));
    connect(modelNam,SIGNAL(sigUpd()),Rels::instance(),SLOT(refreshNam()));
    connect(modelVol,SIGNAL(sigUpd()),Rels::instance()->relVol,SLOT(refreshModel()));
}

FormLbl::~FormLbl()
{
    delete ui;
}


QString FormLbl::getNum(QComboBox *c)
{
    int n=0;
    if (c->findText(c->currentText())!=-1 && c->model()->columnCount()>2){
        n=c->model()->data(c->model()->index(c->currentIndex(),2),Qt::EditRole).toInt();
    }
    QString num = QString::number(n);
    num=num.rightJustified(2,'0',true);
    return num;
}

void FormLbl::setComboBoxModel(DbComboBox *c, DbSqlRelation *r)
{
    if (!r->isInital()){
        r->refreshModel();
    }
    c->setModel(r->model());
    c->setCurrentText("");
}

void FormLbl::goLbl1()
{
    QString year=ui->lineEditYear->text();
    year=year.rightJustified(2,QChar('0'));
    QString opart=ui->lineEditOrigPart->text();
    opart=opart.rightJustified(4,QChar('0'));
    QString num=year+opart+getNum(ui->comboBoxNam);
    QString str;
    str+=tr("Марка - ")+ui->comboBoxMar->currentText()+"\n";
    str+=tr("Диаметр, мм - ")+ui->comboBoxDiam->currentText()+"\n";
    str+=tr("Плавка - ")+ui->lineEditPlav->text()+"\n";
    str+=tr("Партия - ")+ui->lineEditPart->text()+"\n";
    str+=tr("Тип носителя - ")+ui->comboBoxPack->currentText()+"\n";
    str+=tr("Код продукции - ")+num+"\n";
    str+=tr("Масса нетто, кг - ")+ui->lineEditMas->text()+"\n";
    str+=tr("Дата изг. - ")+ui->dateEditPart->date().toString("dd.MM.yy");
    LblEngine e;
    e.createLblEd(str,QString());
}

