#include "formreportpodt.h"
#include "ui_formreportpodt.h"

FormReportPodt::FormReportPodt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormReportPodt)
{
    ui->setupUi(this);
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    ui->comboBoxType->setModel(Models::instance()->relPodtType->model());
    ui->comboBoxType->setModelColumn(1);

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
    int id_type=ui->comboBoxType->model()->data(ui->comboBoxType->model()->index(ui->comboBoxType->currentIndex(),0),Qt::EditRole).toInt();
    int num=1;
    if (ui->radioButtonMark->isChecked()){
        num=2;
    } else if (ui->radioButtonKat->isChecked()){
        num=3;
    }

    modelReport->refresh(ui->dateEditBeg->date().addDays(-1),ui->dateEditEnd->date(),num,id_type);
    ui->tableView->resizeToContents();
}

void FormReportPodt::save()
{
    ui->tableView->save(tr("Подтяжки с ")+ui->dateEditBeg->date().toString("dd.MM.yy")+tr(" по ")+ui->dateEditEnd->date().toString("dd.MM.yy"));
}
