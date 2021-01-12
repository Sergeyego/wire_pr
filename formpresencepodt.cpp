#include "formpresencepodt.h"
#include "ui_formpresencepodt.h"

FormPresencePodt::FormPresencePodt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPresencePodt)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());

    ui->comboBoxType->setModel(Models::instance()->relPodtType->model());
    ui->comboBoxType->setModelColumn(1);

    modelPresence = new ModelPresencePodt(this);
    ui->tableView->setModel(modelPresence);

    connect(ui->pushButtonUpd, SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this,SLOT(save()));
}

FormPresencePodt::~FormPresencePodt()
{
    delete ui;
}

void FormPresencePodt::upd()
{
    int id_type=ui->comboBoxType->model()->data(ui->comboBoxType->model()->index(ui->comboBoxType->currentIndex(),0),Qt::EditRole).toInt();
    modelPresence->refresh(ui->dateEdit->date(),ui->radioButtonPart->isChecked(),id_type);
    ui->tableView->resizeToContents();
}

void FormPresencePodt::save()
{
    ui->tableView->save(QString::fromUtf8("Наличие подтяжек на ")+ui->dateEdit->date().toString("dd.MM.yy"));
}
