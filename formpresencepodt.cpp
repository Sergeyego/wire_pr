#include "formpresencepodt.h"
#include "ui_formpresencepodt.h"

FormPresencePodt::FormPresencePodt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPresencePodt)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());

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
    modelPresence->refresh(ui->dateEdit->date(),ui->radioButtonPart->isChecked());
    ui->tableView->resizeToContents();
}

void FormPresencePodt::save()
{
    ui->tableView->save(QString::fromUtf8("Наличие подтяжек на ")+ui->dateEdit->date().toString("dd.MM.yy"));
}
