#include "formpresencepodt.h"
#include "ui_formpresencepodt.h"

FormPresencePodt::FormPresencePodt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPresencePodt)
{
    ui->setupUi(this);
}

FormPresencePodt::~FormPresencePodt()
{
    delete ui;
}
