#include "formpart.h"
#include "ui_formpart.h"

FormPart::FormPart(bool edtSert, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPart)
{
    ui->setupUi(this);
    if (!edtSert){
        ui->groupBoxChem->hide();
        ui->groupBoxMech->hide();
    }

    sert=new SertBuild(this);
    editor=new Editor(sert,NULL);

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());
    ui->tableViewPart->setEditTriggers(QAbstractItemView::NoEditTriggers);

    modelMechReal = new ModelMechReal(this);
    ui->tableViewMechReal->setModel(modelMechReal);
    ui->tableViewMechReal->setColumnHidden(0,true);
    ui->tableViewMechReal->setColumnHidden(1,true);
    ui->tableViewMechReal->setColumnWidth(2,150);
    ui->tableViewMechReal->setColumnWidth(3,90);

    modelPart = new ModelPart(this);
    modelPart->setDateBeg(ui->dateEditBeg->date());
    modelPart->setDateEnd(ui->dateEditEnd->date());

    ui->comboBoxOnly->setModel(modelPart->relation(5)->model());
    ui->comboBoxOnly->setModelColumn(modelPart->relation(5)->columnDisplay());

    ui->tableViewPart->setModel(modelPart);
    ui->tableViewPart->setColumnHidden(0,true);
    ui->tableViewPart->setColumnHidden(4,true);
    ui->tableViewPart->setColumnHidden(7,true);
    ui->tableViewPart->setColumnHidden(8,true);
    ui->tableViewPart->setColumnHidden(9,true);
    ui->tableViewPart->setColumnHidden(10,true);
    ui->tableViewPart->setColumnHidden(11,true);
    ui->tableViewPart->setColumnWidth(1,50);
    ui->tableViewPart->setColumnWidth(2,70);
    ui->tableViewPart->setColumnWidth(3,90);
    ui->tableViewPart->setColumnWidth(5,150);
    ui->tableViewPart->setColumnWidth(6,50);

    srsChemModel = new SrcChemModel(this);
    chemModel = new ModelWirePartiChem(this);
    mechModel = new ModelWirePartiMech(this);
    modelShip = new ModelShipment(this);
    push = new DbMapper(ui->tableViewPart,this);

    ui->tableViewProvChem->setModel(srsChemModel);
    ui->tableViewProvChem->verticalHeader()->hide();
    ui->tableViewProvChem->setColumnWidth(2,100);
    ui->tableViewProvChem->setColumnWidth(3,100);
    ui->tableViewProvChem->setMenuEnabled(false);


    ui->tableViewWirePartChem->setModel(chemModel);
    ui->tableViewWirePartChem->setColumnHidden(0,true);
    ui->tableViewWirePartChem->setColumnHidden(1,true);
    ui->tableViewWirePartChem->setColumnWidth(2,100);
    ui->tableViewWirePartChem->setColumnWidth(3,100);


    ui->tableViewWirePartMech->setModel(mechModel);
    ui->tableViewWirePartMech->setColumnHidden(0,true);
    ui->tableViewWirePartMech->setColumnHidden(1,true);
    ui->tableViewWirePartMech->setColumnWidth(2,260);
    ui->tableViewWirePartMech->setColumnWidth(3,82);
    ui->tableViewWirePartMech->setColumnWidth(4,82);
    ui->tableViewWirePartMech->setColumnWidth(5,250);

    ui->horizontalLayoutData->insertWidget(0,push);
    push->addMapping(ui->lineEditNum,1);
    push->addMapping(ui->dateEdit,2);
    push->addMapping(ui->comboBoxIst,3);
    push->addMapping(ui->comboBoxSrcPart,4);
    push->addMapping(ui->comboBoxMar,5);
    push->addMapping(ui->comboBoxDiam,6);
    push->addMapping(ui->comboBoxType,7);
    push->addMapping(ui->lineEditComment,8);
    push->addMapping(ui->comboBoxVol,9);
    push->addMapping(ui->lineEditKvo,10);
    push->addMapping(ui->comboBoxPodt,11);
    push->addLock(ui->dateEditBeg);
    push->addLock(ui->dateEditEnd);
    push->addLock(ui->cmdUpdate);
    push->addLock(ui->checkBoxOnly);
    push->addEmptyLock(ui->groupBoxChem);
    push->addEmptyLock(ui->groupBoxMech);
    push->addEmptyLock(ui->cmdSert);
    push->addEmptyLock(ui->cmdLblPack);
    push->addEmptyLock(ui->cmdLblSpool);
    push->addEmptyLock(ui->groupBoxShip);
    push->addEmptyLock(ui->groupBoxAdd);
    push->addEmptyLock(ui->groupBoxDef);
    push->addEmptyLock(ui->groupBoxNam);
    push->addEmptyLock(ui->comboBoxPack);
    push->addEmptyLock(ui->toolButtonAddPack);
    push->addEmptyLock(ui->toolButtonDelPack);
    push->addEmptyLock(ui->toolButtonEdtPack);
    push->addUnLock(ui->toolButtonPodt);
    push->addUnLock(ui->toolButtonSrc);

    loadSettings();

    modelNamCex = new ModelNamCex(this);
    ui->tableViewNam->setModel(modelNamCex);
    ui->tableViewNam->setColumnHidden(0,true);
    ui->tableViewNam->setColumnHidden(1,true);
    ui->tableViewNam->setColumnWidth(2,85);
    ui->tableViewNam->setColumnWidth(3,70);
    ui->tableViewNam->setColumnWidth(4,140);
    ui->tableViewNam->setColumnWidth(5,110);

    partPackModel = new PartPackModel(this);
    ui->comboBoxPack->setModel(partPackModel);
    ui->comboBoxPack->setModelColumn(1);

    blockShip(true);

    modelInCex = new ModelInCex(this);
    ui->tableViewInCex->setModel(modelInCex);
    ui->tableViewInCex->setColumnHidden(0,true);
    ui->tableViewInCex->setColumnHidden(1,true);
    ui->tableViewInCex->setColumnWidth(2,85);
    ui->tableViewInCex->setColumnWidth(3,80);
     ui->tableViewInCex->setColumnWidth(4,120);

    modelOutCex = new ModelOutCex(this);
    ui->tableViewOutCex->setModel(modelOutCex);
    ui->tableViewOutCex->setColumnHidden(0,true);
    ui->tableViewOutCex->setColumnHidden(1,true);
    ui->tableViewOutCex->setColumnWidth(2,85);
    ui->tableViewOutCex->setColumnWidth(3,80);
    ui->tableViewOutCex->setColumnWidth(4,120);

    connect(ui->cmdUpdate,SIGNAL(clicked()),modelPart,SLOT(select()));
    connect(ui->dateEditBeg,SIGNAL(dateChanged(QDate)),modelPart,SLOT(setDateBeg(QDate)));
    connect(ui->dateEditEnd,SIGNAL(dateChanged(QDate)),modelPart,SLOT(setDateEnd(QDate)));
    connect(push,SIGNAL(currentIndexChanged(int)),this,SLOT(updData(int)));
    connect(chemModel,SIGNAL(sigUpd()),modelPart,SLOT(refreshState()));
    connect(mechModel,SIGNAL(sigUpd()),modelPart,SLOT(refreshState()));
    connect(ui->cmdCopyChem,SIGNAL(clicked()),this,SLOT(copyChem()));
    connect(ui->cmdSert,SIGNAL(clicked()),this,SLOT(showSert()));
    connect(ui->checkBoxOnly,SIGNAL(clicked(bool)),ui->comboBoxOnly,SLOT(setEnabled(bool)));
    connect(ui->checkBoxOnly,SIGNAL(clicked(bool)),this,SLOT(setFilter()));
    connect(ui->comboBoxOnly,SIGNAL(currentIndexChanged(int)),this,SLOT(setFilter()));
    connect(ui->toolButtonAddPack,SIGNAL(clicked(bool)),this,SLOT(addPack()));
    connect(ui->toolButtonDelPack,SIGNAL(clicked(bool)),this,SLOT(delPack()));
    connect(ui->toolButtonEdtPack,SIGNAL(clicked(bool)),this,SLOT(edtPack()));
    connect(ui->comboBoxPack,SIGNAL(currentIndexChanged(int)),this,SLOT(updShip()));
    connect(modelInCex,SIGNAL(sigSum(QString)),this,SLOT(setAddItogo(QString)));
    connect(modelOutCex,SIGNAL(sigSum(QString)),this,SLOT(setOutItogo(QString)));
    connect(modelNamCex,SIGNAL(sigSum(QString)),this,SLOT(setNamItogo(QString)));
    connect(modelShip,SIGNAL(sigSum(QString)),this,SLOT(setShipItogo(QString)));
    connect(ui->cmdLblSpool,SIGNAL(clicked(bool)),this,SLOT(lblEd()));
    connect(ui->cmdLblPack,SIGNAL(clicked(bool)),this,SLOT(lblGroup()));
    connect(ui->tableViewShip,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(showSertShip(QModelIndex)));
    connect(ui->toolButtonPodt,SIGNAL(clicked(bool)),this,SLOT(fltPodt()));
    connect(ui->toolButtonSrc,SIGNAL(clicked(bool)),this,SLOT(fltSrc()));

    modelPart->select();

    ui->tableViewShip->setModel(modelShip);
    ui->tableViewShip->verticalHeader()->hide();
    ui->tableViewShip->setColumnWidth(0,80);
    ui->tableViewShip->setColumnWidth(1,50);
    ui->tableViewShip->setColumnWidth(2,190);
    ui->tableViewShip->setColumnWidth(3,80);
    ui->tableViewShip->setMenuEnabled(false);
    ui->tableViewShip->setColumnHidden(4,true);
}

FormPart::~FormPart()
{
    saveSettings();
    delete ui;
    delete editor;
}

void FormPart::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    //this->restoreGeometry(settings.value("part_geometry").toByteArray());
    this->ui->splitter->restoreState(settings.value("part_splitter_width").toByteArray());

}

void FormPart::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    //settings.setValue("part_geometry", this->saveGeometry());
    settings.setValue("part_splitter_width",ui->splitter->saveState());
}

void FormPart::blockShip(bool val)
{
    ui->groupBoxAdd->setDisabled(val);
    ui->groupBoxDef->setDisabled(val);
    ui->groupBoxShip->setDisabled(val);
    ui->groupBoxNam->setDisabled(val);
    ui->cmdSert->setDisabled(val);
    ui->cmdLblPack->setDisabled(val);
    ui->cmdLblSpool->setDisabled(val);
    ui->toolButtonDelPack->setDisabled(val);
    ui->toolButtonEdtPack->setDisabled(val);
}


void FormPart::updData(int row)
{
    int id_p=modelPart->data(modelPart->index(row,0),Qt::EditRole).toInt();
    srsChemModel->refresh(id_p);
    chemModel->refresh(id_p);
    mechModel->refresh(id_p);
    partPackModel->refresh(id_p);
    modelMechReal->refresh(id_p);
    bool ok= partPackModel->rowCount()>0;
    if (ok){
        ui->comboBoxPack->setCurrentIndex(0);
    }
    QString pack;
    pack=tr("Носители: ");
    for (int i=0; i< partPackModel->rowCount(); i++){
        pack+=partPackModel->data(partPackModel->index(i,1)).toString()+partPackModel->data(partPackModel->index(i,2)).toString()+", ";
    }
    pack.truncate(pack.size()-2);
    ui->labelPack->setText(pack);
    blockShip(!ok);
    updShip();
}

void FormPart::copyChem()
{
    if (!srsChemModel->rowCount()) return;
    int r=chemModel->isAdd()? chemModel->rowCount()-1 : chemModel->rowCount();
    int id_p=modelPart->data(modelPart->index(ui->tableViewPart->currentIndex().row(),0),Qt::EditRole).toInt();
    if (r){
        QMessageBox::information(this,tr("Предупреждение"),tr("Сначала удалите все уже существующие элементы!"),QMessageBox::Ok);
        return;
    }
    int n=QMessageBox::question(this,tr("Подтвердите действия"),tr("Скопировать значения?"),QMessageBox::Yes,QMessageBox::No);
    if (n==QMessageBox::Yes){
        modelPart->copyChem(id_p);
        updData(push->currentIndex());
    }
}

void FormPart::showSert()
{
    int id_p=partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),0),Qt::EditRole).toInt();
    sert->build(id_p, false);
    editor->show();
}

void FormPart::setFilter()
{
    bool b=ui->checkBoxOnly->isChecked();
    int id= b? ui->comboBoxOnly->model()->data(ui->comboBoxOnly->model()->index(ui->comboBoxOnly->currentIndex(),0),Qt::EditRole).toInt() : -1;
    modelPart->setIdProv(id);
    modelPart->select();
}

void FormPart::updShip()
{
    int id_p = partPackModel->rowCount()>0 ? partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),0),Qt::EditRole).toInt() : -1;
    modelShip->refresh(id_p,QDate::currentDate().addYears(100));
    modelNamCex->refresh(id_p);
    modelInCex->refresh(id_p);
    modelOutCex->refresh(id_p);
}

void FormPart::addPack()
{
    DialogPack p(tr("К-300"));
    if (p.exec()==QDialog::Accepted){
        int id_pack=p.getId();
        int id_type=p.getIdType();
        bool ok=partPackModel->addPack(id_pack,id_type);
        if (ok) updData(push->currentIndex());
        int pos=ui->comboBoxPack->findText(p.getText());
        if (pos>=0)
            ui->comboBoxPack->setCurrentIndex(pos);
    }
}

void FormPart::delPack()
{
    if (partPackModel->rowCount()){
        int n=QMessageBox::question(this,tr("Подтвердите удаление"),tr("Подтверждаете удаление ")+ui->comboBoxPack->currentText()+tr("?"),QMessageBox::Yes,QMessageBox::No);
        if (n==QMessageBox::Yes){
            int id_p = partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),0),Qt::EditRole).toInt();
            bool ok=partPackModel->delPack(id_p);
            if (ok) updData(push->currentIndex());
        }
    }
}

void FormPart::edtPack()
{
    if (partPackModel->rowCount()){
        int id_p = partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),0),Qt::EditRole).toInt();
        DialogPack p(ui->comboBoxPack->currentText());
        if (p.exec()==QDialog::Accepted){
            bool ok=partPackModel->edtPack(id_p,p.getId(),p.getIdType());
            if(ok) updData(push->currentIndex());
            int pos=ui->comboBoxPack->findText(p.getText());
            if (pos>=0)
                ui->comboBoxPack->setCurrentIndex(pos);
        }
    }
}

void FormPart::setAddItogo(QString s)
{
    ui->groupBoxAdd->setTitle(s);
}

void FormPart::setOutItogo(QString s)
{
    ui->groupBoxDef->setTitle(s);
}

void FormPart::setNamItogo(QString s)
{
    ui->groupBoxNam->setTitle(s);
}

void FormPart::setShipItogo(QString s)
{
    ui->groupBoxShip->setTitle(s);
}

void FormPart::lblEd()
{
    if (partPackModel->rowCount()){
        int id_p = partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),0),Qt::EditRole).toInt();
        DialogOtk d(ui->comboBoxVol->currentText(),QString(),ui->dateEdit->date());
        if (d.exec()==QDialog::Accepted){
            LblEngine e;
            //qDebug()<<d.getNam()<<" "<<d.getVol()<<" "<<d.getCod();
            e.createLblEd(id_p,d.getCod(),d.getOtk(),d.barCode(),d.getOpt());
        }
    }
}

void FormPart::lblGroup()
{
    if (partPackModel->rowCount()){
        int id_p = partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),0),Qt::EditRole).toInt();
        LblEngine e;
        e.createLblGroup(id_p);
    }
}

void FormPart::showSertShip(QModelIndex index)
{
    if (index.isValid()){
        int id_ship=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(index.row(),4),Qt::EditRole).toInt();
        sert->build(id_ship, true);
        editor->show();
    }
}

void FormPart::fltPodt()
{
    DialogFlt d(tr("подтяжки"), Models::instance()->relPodt);
    d.exec();
}

void FormPart::fltSrc()
{
    DialogFlt d(tr("исходные партии"), Models::instance()->relSrcPart);
    d.exec();
}
