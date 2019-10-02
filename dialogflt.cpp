#include "dialogflt.h"
#include "ui_dialogflt.h"

DialogFlt::DialogFlt(QString nam, DbRelation *rel, QWidget *parent) :
    QDialog(parent), relation(rel),
    ui(new Ui::DialogFlt)
{
    ui->setupUi(this);
    ui->label->setText(tr("Выбирать ")+nam);
    ui->comboBox->addItem(tr("начиная с текущего года"));
    ui->comboBox->addItem(tr("начиная с прошлого года"));
    ui->comboBox->addItem(tr("за всё время"));
    int currentIndex=0;
    QString pattern = relation->proxyModel()->filterRegExp().pattern();
    if (pattern.isEmpty()){
        currentIndex=2;
    } else if (pattern==QString::number(QDate::currentDate().year())){
        currentIndex=0;
    } else {
        currentIndex=1;
    }
    ui->comboBox->setCurrentIndex(currentIndex);
}

DialogFlt::~DialogFlt()
{
    delete ui;
}

void DialogFlt::accept()
{
    int year=QDate::currentDate().year();
    int index=ui->comboBox->currentIndex();
    QString pattern=QString();
    if (index==0){
        pattern=QString::number(year);
    } else if (index==1){
         pattern=QString::number(year-1)+"|"+QString::number(year);
    }
    relation->proxyModel()->setFilterRegExp(pattern);
    return QDialog::accept();
}
