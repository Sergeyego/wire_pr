#include "formroute.h"
#include "ui_formroute.h"

FormRoute::FormRoute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormRoute)
{
    ui->setupUi(this);

    modelRoute = new DbTableModel("wire_route",this);
    modelRoute->addColumn("id","id",true,true,TYPE_INT);
    modelRoute->addColumn("nam",tr("Название"),false,false,TYPE_STRING);
    modelRoute->addColumn("data",tr("Маршрут"),false,false,TYPE_STRING);
    modelRoute->setSort("nam");

    draw = new Drawing(ui->lineEditRoute,this);
    draw->setEnabled(false);
    ui->tableViewRoute->setModel(modelRoute);
    ui->tableViewRoute->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewRoute->setColumnHidden(0,true);
    //ui->tableViewRoute->setColumnWidth(1,150);
    //ui->tableViewRoute->setColumnWidth(2,140);
    ui->tableViewRoute->setFocus();
    ui->verticalLayout->insertWidget(1,draw);

    push = new PushForm(ui->tableViewRoute,this);
    push->addMapping(ui->lineEditName,1);
    push->addMapping(ui->lineEditRoute,2);
    push->addLock(ui->tableViewRoute);
    push->addUnLock(draw);
    ui->verticalLayout_2->addWidget(push);

    loadSettings();

    connect(push,SIGNAL(currentIndexChanged(int)),draw,SLOT(loadRoute()));
    connect(modelRoute,SIGNAL(sigUpd()),Models::instance()->relRoute->model(),SLOT(refresh()));

    modelRoute->select();
}

FormRoute::~FormRoute()
{
    saveSettings();
    delete ui;
}

void FormRoute::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    //this->restoreGeometry(settings.value("route_geometry").toByteArray());
    this->ui->splitter->restoreState(settings.value("route_splitter_width").toByteArray());
    for (int i=0; i<ui->tableViewRoute->model()->columnCount(); i++){
        int val=settings.value("cw"+QString::number(i)).toInt();
        ui->tableViewRoute->setColumnWidth(i,val? val : 150);
    }
}

void FormRoute::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    //settings.setValue("route_geometry", this->saveGeometry());
    settings.setValue("route_splitter_width",ui->splitter->saveState());
    for (int i=0; i<ui->tableViewRoute->model()->columnCount(); i++){
        settings.setValue("cw"+QString::number(i),ui->tableViewRoute->columnWidth(i));
    }
}
