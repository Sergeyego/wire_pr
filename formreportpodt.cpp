#include "formreportpodt.h"
#include "ui_formreportpodt.h"

FormReportPodt::FormReportPodt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormReportPodt)
{
    ui->setupUi(this);
}

FormReportPodt::~FormReportPodt()
{
    delete ui;
}
