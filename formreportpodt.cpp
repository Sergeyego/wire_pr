#include "formreportpodt.h"
#include "ui_formreportpodt.h"

FormReportPodt::FormReportPodt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormReportPodt)
{
    ui->setupUi(this);
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    modelReport = new ModelReportPodt(this);
    ui->tableView->setModel(modelReport);

    connect(ui->pushButtonUpd, SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this,SLOT(save()));
}

FormReportPodt::~FormReportPodt()
{
    delete ui;
}

void FormReportPodt::upd()
{
    modelReport->refresh(ui->dateEditBeg->date().addDays(-1),ui->dateEditEnd->date(),ui->radioButtonPart->isChecked());
    ui->tableView->resizeToContents();
}

void FormReportPodt::save()
{
    ui->tableView->save(tr("Подтяжки с ")+ui->dateEditBeg->date().toString("dd.MM.yy")+tr(" по ")+ui->dateEditEnd->date().toString("dd.MM.yy"));
}
