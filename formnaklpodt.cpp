#include "formnaklpodt.h"
#include "ui_formnaklpodt.h"

FormNaklPodt::FormNaklPodt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormNaklPodt)
{
    ui->setupUi(this);
}

FormNaklPodt::~FormNaklPodt()
{
    delete ui;
}
