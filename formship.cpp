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
    modelShip->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date());
    ui->tableViewShip->setColumnHidden(0,true);
    ui->tableViewShip->resizeToContents();
}

void FormShip::updateDataShip(QModelIndex shipIndex)
{
    int id_ship=modelShip->data(modelShip->index(shipIndex.row(),0)).toInt();
    modelDataShip->refresh(id_ship);
    ui->tableViewDataShip->setColumnHidden(0,true);
    ui->tableViewDataShip->resizeToContents();
    ui->tableViewDataShip->selectRow(0);
}

void FormShip::updateSertificat(QModelIndex shipDataIndex)
{
    int id_dataShip=modelDataShip->data(modelDataShip->index(shipDataIndex.row(),0)).toInt();
    sertificat->build(id_dataShip,true);
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
    for (int i=0; i<modelDataShip->rowCount(); i++){
        updateSertificat(modelDataShip->index(i,0));
        QDir dir(QDir::homePath()+"/wire_sertificat");
        if (!dir.exists()) dir.mkdir(dir.path());
        dir.setPath(dir.path()+"/"+sertificat->getYearSert());
        if (!dir.exists()) dir.mkdir(dir.path());
        dir.setPath(dir.path()+"/"+sertificat->getNomSert());
        if (!dir.exists()) dir.mkdir(dir.path());
        QFile file(dir.path()+"/ws_"+sertificat->getNomPart()+"_"+sertificat->getYearPart()+"_"+sertificat->getNomSert()+".pdf");
        editor->exportPdf(file.fileName());
    }
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
    QString f="/ws_"+sertificat->getYearSert()+"_"+sertificat->getNomSert()+".pdf";
    QString fname = QFileDialog::getSaveFileName(this,tr("Сохранить PDF"),QDir::homePath()+"/"+f, "*.pdf");
    if (!fname.isEmpty()){
        QPdfWriter writer(fname);
        writer.setPageOrientation(QPageLayout::Portrait);
        writer.setPageSize(QPdfWriter::A4);
        writer.setPageMargins(QMarginsF(30, 30, 30, 30));
        printAll(&writer);
    }
}
