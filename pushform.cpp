#include "pushform.h"
#include "ui_pushform.h"

PushForm::PushForm(QAbstractItemView *v, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PushForm)
{
    ui->setupUi(this);
    viewer=v;
    addLock(viewer);
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(v->model());
    mapper->setItemDelegate(new CbRelationDelegate(this));
    isEdt=false;

    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(mapper->model());
    if (sqlModel){
        connect(sqlModel,SIGNAL(sigRefresh()),this,SLOT(first()));
        connect(sqlModel,SIGNAL(sigRefresh()),this,SLOT(checkEmpty()));
    }

    connect(ui->cmdNew,SIGNAL(clicked()),this,SLOT(slotNew()));
    connect(ui->cmdWrite,SIGNAL(clicked()),this,SLOT(slotWrite()));
    connect(ui->cmdEdt,SIGNAL(clicked()),this,SLOT(slotEdt()));
    connect(ui->cmdEsc,SIGNAL(clicked()),this,SLOT(slotEsc()));
    connect(ui->cmdDel,SIGNAL(clicked()),this,SLOT(slotDel()));
    connect(mapper->itemDelegate(),SIGNAL(commitData(QWidget*)),this,SLOT(slotEdt()));
    connect(viewer->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),mapper,SLOT(setCurrentModelIndex(QModelIndex)));
    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SIGNAL(currentIndexChanged(int)));

}

PushForm::~PushForm()
{
    delete ui;
}

void PushForm::addLock(QWidget *widget)
{
    lock1.push_back(widget);
}

void PushForm::addUnLock(QWidget *widget)
{
    lock2.push_back(widget);
}

void PushForm::addEmptyLock(QWidget *widget)
{
    lockEmpty.push_back(widget);
}

void PushForm::lock(bool val)
{
    isEdt=val;
    ui->cmdDel->setEnabled(!val);
    ui->cmdEdt->setEnabled(!val);
    ui->cmdEsc->setEnabled(val);
    ui->cmdNew->setEnabled(!val);
    ui->cmdWrite->setEnabled(val);
    for (int i=0; i<lock1.size(); i++) lock1[i]->setEnabled(!val);
    for (int i=0; i<lock2.size(); i++) lock2[i]->setEnabled(val);
    for (int i=0; i<lockEmpty.size(); i++) lockEmpty[i]->setEnabled(!val);
}

void PushForm::checkEmpty()
{
    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(mapper->model());
    if (sqlModel) {
        bool val= sqlModel->rowCount()<=1 && sqlModel->isAdd();
        for (int i=0; i<lockEmpty.size(); i++) lockEmpty[i]->setEnabled(!val);
        ui->cmdEdt->setEnabled(!val);
        ui->cmdDel->setEnabled(!val);
    }
}

bool PushForm::isLock()
{
    return (ui->cmdWrite->isEnabled());
}

void PushForm::addMapping(QWidget *widget, int section)
{
    QPalette pal=widget->palette();
    pal.setColor(QPalette::Disabled, QPalette::Text, pal.color(QPalette::Active,QPalette::Text));
    pal.setColor(QPalette::Disabled, QPalette::WindowText, pal.color(QPalette::Active,QPalette::WindowText));
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, pal.color(QPalette::Active,QPalette::ButtonText));
    widget->setEnabled(false);
    widget->setPalette(pal);
    addUnLock(widget);
    mapper->addMapping(widget,section);
}

int PushForm::currentIndex()
{
    return mapper->currentIndex();
}

void PushForm::setCurrentViewRow(int row)
{
    int n=0;
    QTableView *table = qobject_cast<QTableView *> (viewer);
    if (table){
        bool hidden=table->isColumnHidden(n);
        while (hidden && n<table->model()->columnCount()){
            n++;
            hidden=table->isColumnHidden(n);
        }
    }
    viewer->setCurrentIndex(viewer->model()->index(row,n));
    viewer->scrollTo(viewer->currentIndex());
}


void PushForm::slotNew()
{
    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(mapper->model());
    if (sqlModel) {
        sqlModel->insertRow(sqlModel->rowCount());
        mapper->toLast();
        if (mapper->mappedWidgetAt(1)) mapper->mappedWidgetAt(1)->setFocus();
        setCurrentViewRow(sqlModel->rowCount()-1);
        if (sqlModel->isAdd()) lock(true);
    }
}

void PushForm::slotEdt()
{
    if (!isEdt && mapper->mappedWidgetAt(1)) mapper->mappedWidgetAt(1)->setFocus();
    lock(true);
}

void PushForm::slotDel()
{
    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(mapper->model());
    if (sqlModel) {
        sqlModel->removeRow(mapper->currentIndex());
    }
    checkEmpty();
}


void PushForm::slotWrite()
{
    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(mapper->model());
    if (sqlModel) {
        this->setFocus();
        bool ok=mapper->submit();
        if (sqlModel->isAdd() || sqlModel->isEdt()) ok=sqlModel->submitRow();
        if (ok) {
            lock(false);
        }
        mapper->setCurrentIndex(mapper->currentIndex());
    }
}

void PushForm::slotEsc()
{
    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(mapper->model());
    this->setFocus();
    if (sqlModel){
        if (sqlModel->isAdd()) {
            sqlModel->escAdd();
            setCurrentViewRow(sqlModel->rowCount()-1);
        } else if (sqlModel->isEdt()){
            sqlModel->escAdd();
        }
    }
    lock(false);
    mapper->setCurrentIndex(mapper->currentIndex());
    checkEmpty();
}

void PushForm::first()
{
    mapper->toFirst();
    setCurrentViewRow(0);
}
