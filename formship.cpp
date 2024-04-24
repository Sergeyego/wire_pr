#include "formship.h"
#include "ui_formship.h"

FormShip::FormShip(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormShip)
{
    ui->setupUi(this);
    sertificat = new SertBuild(this);
    editor = new Editor(sertificat);
    ui->splitterVert->addWidget(editor);
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());
    modelShip = new ModelShip(this);
    modelDataShip = new ModelDataShip(this);
    ui->tableViewShip->setModel(modelShip);
    ui->tableViewDataShip->setModel(modelDataShip);
    loadSettings();
    connect(ui->cmdUpd,SIGNAL(clicked()),this,SLOT(updateShip()));
    connect(ui->tableViewShip->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),SLOT(updateDataShip(QModelIndex)));
    connect(ui->tableViewDataShip->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(updateSertificat(QModelIndex)));
    connect(ui->cmdPrintAll,SIGNAL(clicked()),this,SLOT(printAll()));
    connect(ui->cmdPdfAll,SIGNAL(clicked()),this,SLOT(pdfAll()));
    connect(ui->cmdMultiPagePdf,SIGNAL(clicked(bool)),this,SLOT(multipagePdf()));
}

FormShip::~FormShip()
{
    saveSettings();
    delete ui;
}

void FormShip::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    this->ui->splitterVert->restoreState(settings.value("ship_splitter_width").toByteArray());
}

void FormShip::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    settings.setValue("ship_splitter_width",ui->splitterVert->saveState());
}

void FormShip::updateShip()
{
    if (sender()==ui->cmdUpd){
        sertificat->clearCache();
        Rels::instance()->relSertType->refreshModel();
    }
    modelShip->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date());
    ui->tableViewShip->setColumnHidden(0,true);
    ui->tableViewShip->resizeToContents();
    if (ui->tableViewShip->model()->rowCount()){
        ui->tableViewShip->selectRow(ui->tableViewShip->model()->rowCount()-1);
    }
}

void FormShip::updateDataShip(QModelIndex shipIndex)
{
    int id_ship=modelShip->data(modelShip->index(shipIndex.row(),0)).toInt();
    modelDataShip->refresh(id_ship);
    ui->tableViewDataShip->setColumnHidden(0,true);
    ui->tableViewDataShip->setColumnHidden(6,true);
    ui->tableViewDataShip->resizeToContents();
    ui->tableViewDataShip->selectRow(0);

}

void FormShip::updateSertificat(QModelIndex index)
{
    int id_part=modelDataShip->data(modelDataShip->index(index.row(),6),Qt::EditRole).toInt();
    int id_ship=modelDataShip->data(modelDataShip->index(index.row(),0),Qt::EditRole).toInt();
    QString nomSert=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(ui->tableViewShip->currentIndex().row(),1),Qt::EditRole).toString();
    QString name = modelDataShip->data(modelDataShip->index(index.row(),4),Qt::EditRole).toString();
    name+="_"+nomSert;
    name=name.replace(QRegExp("[^\\w]"), "_");
    sertificat->build(id_part,id_ship,name);
}

void FormShip::printAll()
{
    QPrintDialog printDialog(editor->printer, this);
    if (printDialog.exec()) {
        printAll(editor->printer);
    }
}

void FormShip::pdfAll()
{
    QProgressDialog* pprd = new QProgressDialog(tr("Идет формирование документов..."),"", 0, modelDataShip->rowCount(), this);
    pprd->setCancelButton(0);
    pprd->setMinimumDuration(0);
    pprd->setWindowTitle(tr("Пожалуйста, подождите"));
    int yearSert=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(ui->tableViewShip->currentIndex().row(),2),Qt::EditRole).toDate().year();
    QString nomSert=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(ui->tableViewShip->currentIndex().row(),1),Qt::EditRole).toString();
    for (int i=0; i<modelDataShip->rowCount(); i++){
        QCoreApplication::processEvents();
        pprd->setValue(i);
        updateSertificat(modelDataShip->index(i,0));
        QDir dir(QDir::homePath()+"/wire_sertificat");
        if (!dir.exists()) dir.mkdir(dir.path());
        dir.setPath(dir.path()+"/"+QString::number(yearSert));
        if (!dir.exists()) dir.mkdir(dir.path());
        dir.setPath(dir.path()+"/"+nomSert);
        if (!dir.exists()) dir.mkdir(dir.path());
        QFile file(dir.path()+"/"+sertificat->getName()+".pdf");
        editor->exportPdf(file.fileName());
    }
    delete pprd;
}

void FormShip::printAll(QPagedPaintDevice *printer)
{
    QProgressDialog* pprd = new QProgressDialog(tr("Идет формирование документа..."),"", 0, modelDataShip->rowCount(), this);
    pprd->setCancelButton(0);
    pprd->setMinimumDuration(0);
    pprd->setWindowTitle(tr("Пожалуйста, подождите"));

    QPainter painter(printer);
    for (int i=0; i<modelDataShip->rowCount(); i++){
        QCoreApplication::processEvents();
        pprd->setValue(i);
        updateSertificat(modelDataShip->index(i,0));
        editor->drawDoc(&painter);
        if (i!=modelDataShip->rowCount()-1){
            printer->newPage();
        }
    }
    delete pprd;
}

void FormShip::multipagePdf()
{
    QString f = ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(ui->tableViewShip->currentIndex().row(),1),Qt::EditRole).toString()+".pdf";
    QString fname = QFileDialog::getSaveFileName(this,tr("Сохранить PDF"),QDir::homePath()+"/"+f, "*.pdf");
    if (!fname.isEmpty()){
        QPdfWriter writer(fname);
        writer.setPageOrientation(QPageLayout::Portrait);
        writer.setPageSize(QPageSize(QPageSize::A4));
        writer.setPageMargins(QMarginsF(30, 30, 30, 30));
        printAll(&writer);
    }
}

ModelShip::ModelShip(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void ModelShip::refresh(QDate begDate, QDate endDate)
{
    setQuery("select s.id, s.nom_s, s.dat_vid, p.short from sertifikat s "
                 "inner join poluch p on s.id_pol=p.id "
                 "inner join (select distinct id_ship from wire_shipment_consist) w on w.id_ship=s.id "
                 "where s.dat_vid between '"+begDate.toString("yyyy.MM.dd")+"' and '"+endDate.toString("yyyy.MM.dd")+
                 "' order by s.dat_vid, s.nom_s");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Номер"));
        setHeaderData(2, Qt::Horizontal,tr("Дата"));
        setHeaderData(3, Qt::Horizontal,tr("Получатель"));
    }
}

QVariant ModelShip::data(const QModelIndex &index, int role) const
{
    if((role == Qt::DisplayRole) && index.column()==2){
        return QSqlQueryModel::data(index,role).toDate().toString("dd.MM.yy");
    }
    return QSqlQueryModel::data(index, role);
}

ModelDataShip::ModelDataShip(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void ModelDataShip::refresh(int id_ship)
{
    setQuery("select w.id, pr.nam || CASE WHEN p.id_var <> 1 THEN (' /'::text || ev.nam::text) || '/'::text ELSE ''::text END AS mark, "
             "d.sdim, k.short, m.n_s||'-'||date_part('year',m.dat), w.m_netto, p.id "
             "from wire_shipment_consist w "
             "inner join wire_parti p on p.id=w.id_wparti "
             "inner join wire_parti_m m on p.id_m=m.id "
             "inner join provol pr on pr.id=m.id_provol "
             "inner join diam d on d.id=m.id_diam "
             "inner join wire_pack_kind k on p.id_pack=k.id "
             "inner join elrtr_vars ev on ev.id=p.id_var "
             "where w.id_ship= "+QString::number(id_ship)+
             " order by pr.nam, d.sdim, k.short, m.n_s");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Марка"));
        setHeaderData(2, Qt::Horizontal,tr("Ф"));
        setHeaderData(3, Qt::Horizontal,tr("Катушка"));
        setHeaderData(4, Qt::Horizontal,tr("Партия"));
        setHeaderData(5, Qt::Horizontal,tr("Кг"));
        refreshState(id_ship);
    }
}

QVariant ModelDataShip::data(const QModelIndex &item, int role) const
{
    if(role==Qt::TextAlignmentRole){ // Выравнивание
        if(item.column() == 2 || item.column()==5)
            return int(Qt::AlignRight | Qt::AlignVCenter);
        else return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    if((role == Qt::BackgroundRole)) {
        int area = colorState.value(QSqlQueryModel::data(this->index(item.row(),0),Qt::DisplayRole).toInt());
        if(area == 0) return QVariant(QColor(255,170,170)); else
            if(area == 1) return QVariant(QColor(Qt::yellow)); else
                if(area == 2) return QVariant(QColor(Qt::gray)); else
                    if(area == 3) return QVariant(QColor(170,255,170));
    } else return QSqlQueryModel::data(item,role);
}

void ModelDataShip::refreshState(int id_ship)
{
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("select wire_shipment_consist.id, "
                  "(select case when exists(select id from wire_parti_chem where id_part=(select p.id_m from wire_parti as p where p.id = wire_shipment_consist.id_wparti)) "
                           "then 1 else 0 end "
                           "+ "
                           "case when exists(select id from wire_parti_mech where id_part=(select p.id_m from wire_parti as p where p.id = wire_shipment_consist.id_wparti)) "
                           "then 2 else 0 end "
                           "as r) from wire_shipment_consist where wire_shipment_consist.id_ship=:id");
    query.bindValue(":id",id_ship);
    if (query.exec()){
        colorState.clear();
        while (query.next()){
            colorState[query.value(0).toInt()]=query.value(1).toInt();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}
