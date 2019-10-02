#include "formpresence.h"
#include "ui_formpresence.h"

FormPresence::FormPresence(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPresence)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    modelPresence = new ModelPresence(this);
    ui->tableView->setModel(modelPresence);

    connect(ui->cmdRefresh,SIGNAL(clicked(bool)),this,SLOT(updPresence()));
    connect(ui->radioButtonMar,SIGNAL(clicked(bool)),this,SLOT(updPresence()));
    connect(ui->radioButtonPart,SIGNAL(clicked(bool)),this,SLOT(updPresence()));
    connect(ui->cmdSave,SIGNAL(clicked(bool)),this,SLOT(save()));
}

FormPresence::~FormPresence()
{
    delete ui;
}

void FormPresence::updPresence()
{
    modelPresence->refresh(ui->dateEdit->date(),ui->radioButtonPart->isChecked());
    ui->tableView->resizeToContents();
}

void FormPresence::save()
{
    ui->tableView->save(tr("Наличие проволоки на ")+ui->dateEdit->date().toString("dd.MM.yy"));
}
