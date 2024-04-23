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
    ui->tableViewDataShip->setColumnHidden(4,true);
    ui->tableViewDataShip->setColumnHidden(5,true);
    ui->tableViewDataShip->resizeToContents();
    ui->tableViewDataShip->selectRow(0);

}

void FormShip::updateSertificat(QModelIndex index)
{
    int id_part=modelDataShip->data(modelDataShip->index(index.row(),5),Qt::EditRole).toInt();
    int id_ship=modelDataShip->data(modelDataShip->index(index.row(),0),Qt::EditRole).toInt();
    QString nomSert=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(ui->tableViewShip->currentIndex().row(),1),Qt::EditRole).toString();
    QString name = modelDataShip->data(modelDataShip->index(index.row(),1),Qt::EditRole).toString();
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
        QDir dir(QDir::homePath()+"/el_sertificat");
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
    QString f=sertificat->getName()+".pdf";
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
    setQuery("select s.id, s.nom_s, s.dat_vid, p.short from sertifikat as s "
             "inner join poluch as p on p.id=s.id_pol "
             "inner join (select distinct id_sert from otpusk) as o on o.id_sert=s.id "
             "where s.dat_vid between '"+begDate.toString("yyyy-MM-dd")+"' and '"
             +endDate.toString("yyyy-MM-dd")+
             "' order by s.nom_s, s.dat_vid");
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
    setQuery("select o.id, p.n_s||'-'||date_part('year',p.dat_part), e.marka||' "+tr("ф")+" '||cast(p.diam as varchar(3))||"
             "CASE WHEN p.id_var <> 1 THEN (' /'::text || ev.nam::text) || '/'::text ELSE ''::text END AS mark, "
             "o.massa,  "
              "(select case when exists(select id_chem from sert_chem where id_part=p.id) "
                 "then 1 else 0 end "
                 "+ "
                 "case when exists(select id_mech from sert_mech where id_part=p.id) "
                 "then 2 else 0 end "
                 "as r), p.id "
             "from otpusk o inner join parti p on o.id_part=p.id "
             "inner join elrtr e on e.id=p.id_el "
             "inner join istoch i on i.id=p.id_ist "
             "inner join elrtr_vars ev on ev.id=p.id_var "
             "where o.id_sert ="+QString::number(id_ship)+" order by p.n_s, p.dat_part");
    if (lastError().isValid())
    {
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Партия"));
        setHeaderData(2, Qt::Horizontal,tr("Марка"));
        setHeaderData(3, Qt::Horizontal,tr("Масса, кг"));
    }
}

QVariant ModelDataShip::data(const QModelIndex &index, int role) const
{
    if((role == Qt::BackgroundRole)&&(this->columnCount()>3)) {
        int area = record(index.row()).value(4).toInt();
        if(area == 0) return QVariant(QColor(255,170,170)); else
            if(area == 1) return QVariant(QColor(Qt::yellow)); else
                if(area == 2) return QVariant(QColor(Qt::gray)); else
                    if(area == 3) return QVariant(QColor(170,255,170));
    }
    if (role == Qt::TextAlignmentRole){
        if((index.column() == 3))
            return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    if (role == Qt::DisplayRole){
        if((index.column() == 3))
            return QLocale().toString(QSqlQueryModel::data(index,role).toDouble(),'f',1);
    }
    return QSqlQueryModel::data(index, role);
}
