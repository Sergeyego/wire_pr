#include "dialogpack.h"
#include "ui_dialogpack.h"

DialogPack::DialogPack(QString dafault, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPack)
{
    ui->setupUi(this);    
    ui->comboBox->setModel(Models::instance()->relPack->model());
    ui->comboBox->setModelColumn(2);

    ui->comboBoxType->setModel(Models::instance()->relPackType->model());
    ui->comboBoxType->setModelColumn(1);

    int pos=ui->comboBox->findText(dafault);
    if (pos>=0) {
        ui->comboBox->setCurrentIndex(pos);
        selectPackType(pos);
    }

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
        id=ui->comboBox->model()->data(ui->comboBox->model()->index(ui->comboBox->currentIndex(),0)).toInt();
    } else {
        id=-1;
    }
    return id;
}

int DialogPack::getIdType()
{
    int id;
    if (ui->comboBoxType->model()->rowCount()){
        id=ui->comboBoxType->model()->data(ui->comboBoxType->model()->index(ui->comboBoxType->currentIndex(),0)).toInt();
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
    int id_type=ui->comboBox->model()->data(ui->comboBox->model()->index(n,3),Qt::EditRole).toInt();
    for (int i=0; i<ui->comboBoxType->model()->rowCount();i++){
        int t= ui->comboBoxType->model()->data(ui->comboBoxType->model()->index(i,0),Qt::EditRole).toInt();
        if (t==id_type){
            ui->comboBoxType->setCurrentIndex(i);
            break;
        }
    }
}
