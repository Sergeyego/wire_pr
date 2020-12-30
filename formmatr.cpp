#include "formmatr.h"
#include "ui_formmatr.h"

FormMatr::FormMatr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMatr)
{
    ui->setupUi(this);

    modelMatr = new DbTableModel("wire_matr",this);
    modelMatr->addColumn("id","id");
    modelMatr->addColumn("nam",tr("Название"));
    modelMatr->addColumn("id_ed",tr("Ед.изм"),Models::instance()->relEd);

    modelMatr->setSort("nam");
    modelMatr->setDefaultValue(2,1);
    modelMatr->select();

    ui->tableView->setModel(modelMatr);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(1,350);
    ui->tableView->setColumnWidth(2,70);

    connect(modelMatr,SIGNAL(sigUpd()),Models::instance()->relMatr,SLOT(refreshModel()));
}

FormMatr::~FormMatr()
{
    delete ui;
}
