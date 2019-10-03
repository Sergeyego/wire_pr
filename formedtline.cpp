#include "formedtline.h"
#include "ui_formedtline.h"

FormEdtLine::FormEdtLine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEdtLine)
{
    ui->setupUi(this);
    modelLine = new DbTableModel("wire_line",this);
    modelLine->addColumn("id",tr("id"),true,TYPE_INT);
    modelLine->addColumn("nam",tr("Наименование"),false,TYPE_STRING);
    modelLine->addColumn("snam",tr("Крат. наименование"),false,TYPE_STRING);
    modelLine->setSort("wire_line.nam");
    modelLine->select();
    ui->tableView->setModel(modelLine);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(1,250);
    ui->tableView->setColumnWidth(2,150);

    connect(modelLine,SIGNAL(sigUpd()),Models::instance()->relLine->model(),SLOT(refresh()));
}

FormEdtLine::~FormEdtLine()
{
    delete ui;
}
