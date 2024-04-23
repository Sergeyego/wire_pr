#include "dialogpack.h"
#include "ui_dialogpack.h"

DialogPack::DialogPack(int id_p, int id_t, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPack)
{
    ui->setupUi(this);

    if (!Rels::instance()->relPack->isInital()){
        Rels::instance()->relPack->refreshModel();
    }
    ui->comboBox->setModel(Rels::instance()->relPack->model());
    ui->comboBox->setEditable(false);
    colVal p;
    p.val=id_p;
    ui->comboBox->setCurrentData(p);

    if (!Rels::instance()->relPackType->isInital()){
        Rels::instance()->relPackType->refreshModel();
    }
    ui->comboBoxType->setModel(Rels::instance()->relPackType->model());
    ui->comboBoxType->setEditable(false);
    colVal t;
    t.val=id_t;
    ui->comboBoxType->setCurrentData(t);

    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectPackType(int)));
}

DialogPack::~DialogPack()
{
    delete ui;
}

int DialogPack::getId()
{
    int id;
    if (ui->comboBox->model()->rowCount()){
        id=ui->comboBox->getCurrentData().val.toInt();
    } else {
        id=-1;
    }
    return id;
}

int DialogPack::getIdType()
{
    int id;
    if (ui->comboBoxType->model()->rowCount()){
        id=ui->comboBoxType->getCurrentData().val.toInt();
    } else {
        id=-1;
    }
    return id;
}

QString DialogPack::getText()
{
    return ui->comboBox->currentText();
}

void DialogPack::selectPackType(int n)
{
    int id_type=Rels::instance()->relPack->model()->data(Rels::instance()->relPack->model()->index(n,2),Qt::EditRole).toInt();
    colVal c;
    c.val=id_type;
    ui->comboBoxType->setCurrentData(c);
}
