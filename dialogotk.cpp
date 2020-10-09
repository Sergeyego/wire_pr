#include "dialogotk.h"
#include "ui_dialogotk.h"

DialogOtk::DialogOtk(int id_p, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOtk)
{
    ui->setupUi(this);
    ui->comboBoxNam->setModel(Models::instance()->relNam->model());
    ui->comboBoxNam->setModelColumn(Models::instance()->relNam->columnDisplay());
    ui->comboBoxNam->completer()->setCompletionMode(QCompleter::PopupCompletion);
    ui->comboBoxNam->completer()->setCaseSensitivity(Qt::CaseInsensitive);

    ui->comboBoxPart->setModel(Models::instance()->relWirePart->proxyModel());
    ui->comboBoxPart->setModelColumn(1);
    ui->comboBoxPart->completer()->setCompletionMode(QCompleter::PopupCompletion);
    ui->comboBoxPart->completer()->setCaseSensitivity(Qt::CaseInsensitive);

    ui->comboBoxPart->setCurrentIndex(ui->comboBoxPart->findText(Models::instance()->relWirePart->data(QString::number(id_p)).toString()));

    connect(ui->comboBoxPart->lineEdit(),SIGNAL(editingFinished()),this,SLOT(edtPartFinished()));
}

DialogOtk::~DialogOtk()
{
    delete ui;
}


QString DialogOtk::getCod()
{
    QModelIndex ind=ui->comboBoxPart->model()->index(ui->comboBoxPart->currentIndex(),0);
    int id_p=ui->comboBoxPart->model()->data(ind,Qt::EditRole).toInt();
    QString opart, year;
    QSqlQuery query;
    query.prepare("select m.n_s, m.dat "
                  "from wire_parti as p "
                  "inner join wire_parti_m as m on p.id_m=m.id "
                  "where p.id = :id_part ");
    query.bindValue(":id_part",id_p);
    if (query.exec()){
        while (query.next()){
            opart=query.value(0).toString();
            year=query.value(1).toDate().toString("yy");
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
    year=year.rightJustified(2,QChar('0'));
    opart=opart.rightJustified(4,QChar('0'));
    return year+opart+getNum(ui->comboBoxNam);
}

bool DialogOtk::barCode()
{
    return ui->checkBox->isChecked();
}

QString DialogOtk::getNum(QComboBox *c)
{
    int n=0;
    if (c->findText(c->currentText())!=-1 && c->model()->columnCount()>2){
        n=c->model()->data(c->model()->index(c->currentIndex(),2),Qt::EditRole).toInt();
    }
    return QString("%1").arg(n,2,'d',0,QChar('0'));
}

void DialogOtk::edtPartFinished()
{
    ui->comboBoxPart->setCurrentIndex(ui->comboBoxPart->findText(ui->comboBoxPart->currentText()));
}
