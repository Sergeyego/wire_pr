#include "dialogotk.h"
#include "ui_dialogotk.h"

DialogOtk::DialogOtk(QString vol, QString nam, QDate d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOtk)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(d);

    /*ui->comboBoxOtk->setModel(Models::instance()->relOtk->model());
    ui->comboBoxOtk->setModelColumn(Models::instance()->relOtk->columnDisplay());
    ui->comboBoxOtk->completer()->setCompletionMode(QCompleter::PopupCompletion);
    ui->comboBoxOtk->completer()->setCaseSensitivity(Qt::CaseInsensitive);*/
    ui->comboBoxNam->setModel(Models::instance()->relNam->model());
    ui->comboBoxNam->setModelColumn(Models::instance()->relNam->columnDisplay());
    ui->comboBoxNam->setCurrentIndex(ui->comboBoxNam->findText(nam));
    ui->comboBoxNam->completer()->setCompletionMode(QCompleter::PopupCompletion);
    ui->comboBoxNam->completer()->setCaseSensitivity(Qt::CaseInsensitive);
    ui->comboBoxVol->setModel(Models::instance()->relVol->model());
    ui->comboBoxVol->setModelColumn(Models::instance()->relVol->columnDisplay());
    ui->comboBoxVol->setCurrentIndex(ui->comboBoxVol->findText(vol));
    ui->comboBoxVol->completer()->setCompletionMode(QCompleter::PopupCompletion);
    ui->comboBoxVol->completer()->setCaseSensitivity(Qt::CaseInsensitive);
}

DialogOtk::~DialogOtk()
{
    delete ui;
}

QString DialogOtk::getNam()
{
    return getNum(ui->comboBoxNam);
}

QString DialogOtk::getOtk()
{
    return /*ui->comboBoxOtk->currentText()*/QString();
}

QString DialogOtk::getVol()
{
    return getNum(ui->comboBoxVol);
}

QString DialogOtk::getCod()
{
    return ui->dateEdit->date().toString("yyMMdd")+getNam()+getVol();
}

bool DialogOtk::barCode()
{
    return ui->checkBox->isChecked();
}

bool DialogOtk::getOpt()
{
    return ui->checkBoxOpt->isChecked();
}

QString DialogOtk::getNum(QComboBox *c)
{
    int n=0;
    if (c->findText(c->currentText())!=-1 && c->model()->columnCount()>2){
        n=c->model()->data(c->model()->index(c->currentIndex(),2),Qt::EditRole).toInt();
    }
    return QString("%1").arg((n),2,'d',0,QChar('0'));
}
