#include "formreport.h"
#include "ui_formreport.h"

FormReport::FormReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormReport)
{
    ui->setupUi(this);
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    //loadSettings();

    modelReport = new ModelReport(this);

    ui->tableView->setModel(modelReport);

    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(updReport()));
    //connect(ui->radioButtonMar,SIGNAL(clicked(bool)),this,SLOT(updReport()));
    //connect(ui->radioButtonPart,SIGNAL(clicked(bool)),this,SLOT(updReport()));
    connect(ui->cmdSave,SIGNAL(clicked(bool)),this,SLOT(save()));
}

FormReport::~FormReport()
{
    //saveSettings();
    delete ui;
}

void FormReport::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    this->restoreGeometry(settings.value("report_geometry").toByteArray());
}

void FormReport::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    settings.setValue("report_geometry", this->saveGeometry());
}

void FormReport::updReport()
{
    modelReport->refresh(ui->dateEditBeg->date().addDays(-1),ui->dateEditEnd->date(),ui->radioButtonPart->isChecked());
    ui->tableView->resizeToContents();
}

void FormReport::save()
{
    ui->tableView->save(tr("Отчет СЗСМ с ")+ui->dateEditBeg->date().toString("dd.MM.yy")+tr(" по ")+ui->dateEditEnd->date().toString("dd.MM.yy"));
}
