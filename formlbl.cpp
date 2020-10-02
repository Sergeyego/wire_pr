#include "formlbl.h"
#include "ui_formlbl.h"

FormLbl::FormLbl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormLbl)
{
    ui->setupUi(this);
    ui->dateEditPart->setDate(QDate::currentDate());
    setComboBoxModel(ui->comboBoxMar,Models::instance()->relProvol);
    setComboBoxModel(ui->comboBoxDiam,Models::instance()->relDiam);
    setComboBoxModel(ui->comboBoxPack,Models::instance()->relPack);
    setComboBoxModel(ui->comboBoxNam,Models::instance()->relNam);
    ui->lineEditMas->setValidator(new QDoubleValidator(0,999999999,3,this));

    modelOtk = new DbTableModel("otk",this);
    modelOtk->addColumn("id",tr("id"),true,TYPE_INT);
    modelOtk->addColumn("num",tr("№"),false,TYPE_INT, new QIntValidator(0,999999,this));
    modelOtk->addColumn("nam",tr("ОТК"),false,TYPE_STRING);
    modelOtk->setSort("otk.nam");
    modelOtk->select();
    ui->tableViewOtk->setModel(modelOtk);
    ui->tableViewOtk->setColumnHidden(0,true);
    ui->tableViewOtk->setColumnWidth(1,40);
    ui->tableViewOtk->setColumnWidth(2,150);

    modelNam = new DbTableModel("wire_namoch",this);
    modelNam->addColumn("id",tr("id"),true,TYPE_INT);
    modelNam->addColumn("num",tr("№"),false,TYPE_INT, new QIntValidator(0,999999,this));
    modelNam->addColumn("id_rab",tr("ФИО"),false,TYPE_STRING,NULL,Models::instance()->relRab);
    modelNam->addColumn("id_pr",tr("id_pr"),false,TYPE_INT);
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
    modelVol->addColumn("id",tr("id"),true,TYPE_INT);
    modelVol->addColumn("num",tr("№"),false,TYPE_INT, new QIntValidator(0,999999,this));
    modelVol->addColumn("id_rab",tr("ФИО"),false,TYPE_STRING,NULL,Models::instance()->relRab);
    modelVol->addColumn("id_pr",tr("id_pr"),false,TYPE_INT);
    modelVol->setFilter("wire_namoch.id_pr = 2");
    modelVol->setSort("wire_namoch.num");
    modelVol->setDefaultValue(3,2);
    modelVol->select();
    ui->tableViewVol->setModel(modelVol);
    ui->tableViewVol->setColumnHidden(0,true);
    ui->tableViewVol->setColumnWidth(1,40);
    ui->tableViewVol->setColumnWidth(2,290);
    ui->tableViewVol->setColumnHidden(3,true);

    connect(modelVol,SIGNAL(sigUpd()),Models::instance()->relVol->model(),SLOT(refresh()));
    connect(modelNam,SIGNAL(sigUpd()),Models::instance()->relNam->model(),SLOT(refresh()));
    connect(modelOtk,SIGNAL(sigUpd()),Models::instance()->relOtk->model(),SLOT(refresh()));
    connect(ui->pushButtonLbl1,SIGNAL(clicked(bool)),this,SLOT(goLbl1()));
}

FormLbl::~FormLbl()
{
    delete ui;
}

void FormLbl::setComboBoxModel(QComboBox *c, DbRelation *r)
{
    c->setModel(r->model());
    c->setModelColumn(r->columnDisplay());
    c->completer()->setCompletionMode(QCompleter::PopupCompletion);
    c->completer()->setCaseSensitivity(Qt::CaseInsensitive);
    c->lineEdit()->clear();
}

QString FormLbl::getNum(QComboBox *c)
{
    int n=0;
    if (c->findText(c->currentText())!=-1 && c->model()->columnCount()>2){
        n=c->model()->data(c->model()->index(c->currentIndex(),2),Qt::EditRole).toInt();
    }
    return QString("%1").arg(n,2,'d',0,QChar('0'));
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

