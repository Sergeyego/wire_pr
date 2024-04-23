#include "formedtline.h"
#include "ui_formedtline.h"

FormEdtLine::FormEdtLine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEdtLine)
{
    ui->setupUi(this);
    modelLine = new DbTableModel("wire_line",this);
    modelLine->addColumn("id",tr("id"));
    modelLine->addColumn("nam",tr("Наименование"));
    modelLine->addColumn("snam",tr("Крат. наименование"));
    modelLine->setSort("wire_line.nam");
    modelLine->select();
    ui->tableView->setModel(modelLine);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(1,250);
    ui->tableView->setColumnWidth(2,150);

    connect(modelLine,SIGNAL(sigUpd()),Rels::instance()->relLine,SLOT(refreshModel()));
}

FormEdtLine::~FormEdtLine()
{
    delete ui;
}
