#include "formpart.h"
#include "ui_formpart.h"

FormPart::FormPart(bool edtSert, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPart)
{
    ui->setupUi(this);
    loadSettings();

    if (!edtSert){
        ui->groupBoxChem->hide();
        ui->groupBoxMech->hide();
    }

    ui->toolButtonAddPack->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->toolButtonDelPack->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    ui->toolButtonEdtPack->setIcon(style()->standardIcon(QStyle::SP_FileDialogListView));

    sert=new SertBuild(this);
    editor=new Editor(sert);

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());
    ui->tableViewPart->setEditTriggers(QAbstractItemView::NoEditTriggers);

    modelPart = new ModelPart(this);
    modelPart->setDateBeg(ui->dateEditBeg->date());
    modelPart->setDateEnd(ui->dateEditEnd->date());

    ui->comboBoxOnly->setModel(Rels::instance()->relProvol->model());

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
    ui->tableViewPart->setColumnWidth(3,100);
    ui->tableViewPart->setColumnWidth(5,150);
    ui->tableViewPart->setColumnWidth(6,40);

    modelGostSrc = new ModelGostSrc(this);
    ui->tableViewSrcTu->setModel(modelGostSrc);
    ui->tableViewSrcTu->setColumnHidden(0,true);
    ui->tableViewSrcTu->setColumnWidth(1,200);

    modelGostPart = new ModelGostPart(this);
    ui->tableViewPartiTu->setModel(modelGostPart);
    ui->tableViewPartiTu->setColumnHidden(0,true);
    ui->tableViewPartiTu->setColumnWidth(1,200);

    srsChemModel = new ModelChemSrc(this);
    ui->tableViewProvChem->setModel(srsChemModel);
    ui->tableViewProvChem->verticalHeader()->hide();
    ui->tableViewProvChem->setColumnHidden(0,true);
    ui->tableViewProvChem->setColumnWidth(1,100);
    ui->tableViewProvChem->setColumnWidth(2,90);
    ui->tableViewProvChem->setMenuEnabled(false);

    chemModel = new ModelChemSert(this);
    ui->tableViewWirePartChem->setModel(chemModel);
    ui->tableViewWirePartChem->setColumnHidden(0,true);
    ui->tableViewWirePartChem->setColumnHidden(1,true);
    ui->tableViewWirePartChem->setColumnWidth(2,100);
    ui->tableViewWirePartChem->setColumnWidth(3,90);

    modelMechReal = new ModelMechReal(this);
    ui->tableViewMechReal->setModel(modelMechReal);
    ui->tableViewMechReal->setColumnHidden(0,true);
    ui->tableViewMechReal->setColumnHidden(1,true);
    ui->tableViewMechReal->setColumnWidth(2,250);
    ui->tableViewMechReal->setColumnWidth(3,82);


    mechModel = new ModelWirePartiMech(this);
    ui->tableViewWirePartMech->setModel(mechModel);
    ui->tableViewWirePartMech->setColumnHidden(0,true);
    ui->tableViewWirePartMech->setColumnHidden(1,true);
    ui->tableViewWirePartMech->setColumnWidth(2,250);
    ui->tableViewWirePartMech->setColumnWidth(3,82);
    ui->tableViewWirePartMech->setColumnWidth(4,82);
    ui->tableViewWirePartMech->setColumnWidth(5,250);

    modelShip = new ModelShipment(this);
    ui->tableViewShip->setModel(modelShip);
    ui->tableViewShip->verticalHeader()->hide();
    ui->tableViewShip->resizeColumnsToContents();
    ui->tableViewShip->setMenuEnabled(false);
    ui->tableViewShip->setColumnHidden(5,true);

    push = new DbMapper(ui->tableViewPart,this);
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
    push->addEmptyLock(ui->tableViewNam);
    push->addEmptyLock(ui->tableViewOutCex);
    push->addEmptyLock(ui->tableViewInCex);
    push->addEmptyLock(ui->comboBoxPack);
    push->addEmptyLock(ui->toolButtonAddPack);
    push->addEmptyLock(ui->toolButtonDelPack);
    push->addEmptyLock(ui->toolButtonEdtPack);
    push->addEmptyLock(ui->groupBoxTu);
    push->addEmptyLock(ui->tableViewPack);
    push->addEmptyLock(ui->groupBoxPrim);

    partPackModel = new PartPackModel(this);
    ui->comboBoxPack->setModel(partPackModel);
    ui->comboBoxPack->setModelColumn(1);

    modelUnionCex = new ModelUnionCex(this);
    ui->tableViewUnion->setModel(modelUnionCex);

    modelNamCex = new ModelNamCex(this);
    ui->tableViewNam->setModel(modelNamCex);
    ui->tableViewNam->setColumnHidden(0,true);
    ui->tableViewNam->setColumnHidden(1,true);
    ui->tableViewNam->setColumnWidth(2,85);
    ui->tableViewNam->setColumnWidth(3,70);
    ui->tableViewNam->setColumnWidth(4,240);
    ui->tableViewNam->setColumnWidth(5,110);
    ui->tableViewNam->setColumnWidth(6,250);

    modelInCex = new ModelInCex(this);
    ui->tableViewInCex->setModel(modelInCex);
    ui->tableViewInCex->setColumnHidden(0,true);
    ui->tableViewInCex->setColumnHidden(1,true);
    ui->tableViewInCex->setColumnWidth(2,85);
    ui->tableViewInCex->setColumnWidth(3,70);
     ui->tableViewInCex->setColumnWidth(4,120);

    modelOutCex = new ModelOutCex(this);
    ui->tableViewOutCex->setModel(modelOutCex);
    ui->tableViewOutCex->setColumnHidden(0,true);
    ui->tableViewOutCex->setColumnHidden(1,true);
    ui->tableViewOutCex->setColumnWidth(2,85);
    ui->tableViewOutCex->setColumnWidth(3,70);
    ui->tableViewOutCex->setColumnWidth(4,120);

    modelPackCex = new ModelPackCex(this);
    ui->tableViewPack->setModel(modelPackCex);
    ui->tableViewPack->setColumnHidden(0,true);
    ui->tableViewPack->setColumnHidden(1,true);
    ui->tableViewPack->setColumnWidth(2,85);
    ui->tableViewPack->setColumnWidth(3,70);

    modelStockCex = new ModelStockCex(this);
    ui->tableViewStock->setModel(modelStockCex);

    modelPerepackCex = new ModelPerepackCex(this);
    ui->tableViewPerepack->setModel(modelPerepackCex);

    connect(ui->cmdUpdate,SIGNAL(clicked()),this,SLOT(setFilter()));
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
    connect(modelPackCex,SIGNAL(sigSum(QString)),ui->labelPackProd,SLOT(setText(QString)));
    connect(modelStockCex,SIGNAL(sigSum(QString)),ui->labelStock,SLOT(setText(QString)));
    connect(modelPerepackCex,SIGNAL(sigSum(QString)),ui->labelPerepack,SLOT(setText(QString)));
    connect(modelUnionCex,SIGNAL(sigSum(QString)),ui->labelUnion,SLOT(setText(QString)));
    connect(ui->cmdLblSpool,SIGNAL(clicked(bool)),this,SLOT(lblEd()));
    connect(ui->cmdLblPack,SIGNAL(clicked(bool)),this,SLOT(lblGroup()));
    connect(ui->tableViewShip,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(showSertShip(QModelIndex)));
    connect(ui->toolButtonCopyTu,SIGNAL(clicked(bool)),modelGostPart,SLOT(copyTu()));
    connect(ui->pushButtonSavePrim,SIGNAL(clicked(bool)),this,SLOT(savePrim()));
    connect(ui->plainTextEditPrim,SIGNAL(textChanged()),this,SLOT(enPrimSave()));
    connect(ui->toolButtonVar,SIGNAL(clicked(bool)),this,SLOT(edtVar()));

    modelPart->select();

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
    ui->splitter->restoreState(settings.value("part_splitter_width").toByteArray());
    ui->tabWidget->setCurrentIndex(settings.value("part_current_tab",0).toInt());
}

void FormPart::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    settings.setValue("part_current_tab",ui->tabWidget->currentIndex());
    settings.setValue("part_splitter_width",ui->splitter->saveState());
}

void FormPart::blockShip(bool val)
{
    ui->tableViewNam->setDisabled(val);
    ui->tableViewOutCex->setDisabled(val);
    ui->groupBoxShip->setDisabled(val);
    ui->tableViewInCex->setDisabled(val);
    ui->tableViewPack->setDisabled(val);
    ui->cmdSert->setDisabled(val);
    ui->cmdLblPack->setDisabled(val);
    ui->cmdLblSpool->setDisabled(val);
    ui->toolButtonDelPack->setDisabled(val);
    ui->toolButtonEdtPack->setDisabled(val);
    ui->groupBoxPrim->setDisabled(val);
}


void FormPart::updData(int row)
{
    int id_p=modelPart->data(modelPart->index(row,0),Qt::EditRole).toInt();
    int id_buht=modelPart->data(modelPart->index(row,4),Qt::EditRole).toInt();
    partPackModel->refresh(id_p);
    modelGostSrc->refresh(id_p);
    modelGostPart->refresh(id_p);
    srsChemModel->refresh(id_buht);
    chemModel->refresh(id_p);
    mechModel->refresh(id_p);
    modelMechReal->refresh(id_p);
    bool ok = partPackModel->rowCount()>0;
    if (ok){
        ui->comboBoxPack->setCurrentIndex(0);
    }
    QString pack;
    pack=tr("Носители: ");
    for (int i=0; i< partPackModel->rowCount(); i++){
        pack+=partPackModel->data(partPackModel->index(i,1)).toString()+", ";
    }
    pack.truncate(pack.size()-2);
    ui->labelPack->setText(pack);
    updShip();
    blockShip(!ok);
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
    QString name=modelPart->data(modelPart->index(ui->tableViewPart->currentIndex().row(),1),Qt::EditRole).toString();
    name+="_"+QString::number(modelPart->data(modelPart->index(ui->tableViewPart->currentIndex().row(),2),Qt::EditRole).toDate().year());
    name=name.replace(QRegExp("[^\\w]"), "_");
    sert->build(id_p,-1,name);
    editor->show();
}

void FormPart::setFilter()
{
    if (sender()==ui->cmdUpdate){
        modelPart->refreshRelsModel();
        modelGostPart->refreshRelsModel();
        chemModel->refreshRelsModel();
        mechModel->refreshRelsModel();
        Rels::instance()->relWirePart->refreshModel();
        Rels::instance()->relAddType->refreshModel();
        Rels::instance()->relCause->refreshModel();
    }

    bool b=ui->checkBoxOnly->isChecked();
    if (b && sender()==ui->checkBoxOnly){
        colVal d;
        d.val=push->modelData(push->currentIndex(),5).toInt();
        ui->comboBoxOnly->setCurrentData(d);
    }
    int id= b? ui->comboBoxOnly->model()->data(ui->comboBoxOnly->model()->index(ui->comboBoxOnly->currentIndex(),0),Qt::EditRole).toInt() : -1;
    modelPart->setIdProv(id);
    modelPart->select();
}

void FormPart::updShip()
{
    int id_p = partPackModel->rowCount()>0 ? partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),0),Qt::EditRole).toInt() : -1;
    modelShip->refresh(id_p);
    modelUnionCex->refresh(id_p);
    modelNamCex->refresh(id_p);
    modelInCex->refresh(id_p);
    modelOutCex->refresh(id_p);
    modelPackCex->refresh(id_p);
    modelStockCex->refresh(id_p);
    modelPerepackCex->refresh(id_p);
    ui->tableViewShip->resizeColumnsToContents();
    ui->tableViewStock->resizeColumnsToContents();
    ui->tableViewPerepack->resizeColumnsToContents();
    ui->tableViewUnion->resizeColumnsToContents();

    int id_ist=modelPart->data(modelPart->index(push->currentIndex(),3),Qt::EditRole).toInt();
    bool isUnion=id_ist==8 || id_ist==9;

    ui->tableViewNam->setVisible(!isUnion);
    ui->labelProd->setVisible(!isUnion);
    ui->tableViewUnion->setVisible(isUnion);
    ui->labelUnion->setVisible(isUnion);

    ui->plainTextEditPrim->clear();
    ui->lineEditVar->clear();
    ui->toolButtonVar->setEnabled(id_p>0);
    if (id_p>0){
        QSqlQuery query;
        query.prepare("select wp.prim_prod, ev.nam from wire_parti wp inner join elrtr_vars ev on ev.id = wp.id_var  where wp.id = :id");
        query.bindValue(":id",id_p);
        if (query.exec()){
            query.next();
            ui->plainTextEditPrim->setPlainText(query.value(0).toString());
            ui->lineEditVar->setText(query.value(1).toString());
            ui->pushButtonSavePrim->setEnabled(false);
        } else {
            QMessageBox::critical(this,tr("Ошибка"),query.lastError().text(),QMessageBox::Cancel);
        }
    }
}

void FormPart::addPack()
{
    DialogPack p(1,3);
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
            if (ok) {
                updData(push->currentIndex());
            }
        }
    }
}

void FormPart::edtPack()
{
    if (partPackModel->rowCount()){
        int id_part = partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),0),Qt::EditRole).toInt();
        int id_pack = partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),3),Qt::EditRole).toInt();
        int id_pack_type = partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),4),Qt::EditRole).toInt();
        DialogPack p(id_pack,id_pack_type);
        if (p.exec()==QDialog::Accepted){
            bool ok=partPackModel->edtPack(id_part,p.getId(),p.getIdType());
            if (ok) {
                updData(push->currentIndex());
            }
            int pos=ui->comboBoxPack->findText(p.getText());
            if (pos>=0){
                ui->comboBoxPack->setCurrentIndex(pos);
            }
        }
    }
}

void FormPart::edtVar()
{
    if (partPackModel->rowCount()){
        int id_p = partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),0),Qt::EditRole).toInt();
        QMap <QString, int> map;
        QSqlQuery query;
        query.prepare("select id, nam from elrtr_vars order by nam");
        if (query.exec()){
            while (query.next()){
                map.insert(query.value(1).toString(),query.value(0).toInt());
            }
        } else {
            QMessageBox::critical(this,tr("Ошибка"),query.lastError().text(),QMessageBox::Cancel);
        }
        bool ok;
        QStringList items = map.keys();
        QString item = QInputDialog::getItem(this,tr("Редактировать вариант"),tr("Выберите вариант"),items, items.indexOf(ui->lineEditVar->text()),false,&ok);
        if (ok){
            QSqlQuery queryVar;
            queryVar.prepare("update wire_parti set id_var = :id_var where id = :id ");
            queryVar.bindValue(":id_var",map.value(item,1));
            queryVar.bindValue(":id",id_p);
            if (queryVar.exec()){
                updShip();
            } else {
                QMessageBox::critical(this,tr("Ошибка"),queryVar.lastError().text(),QMessageBox::Cancel);
            }
        }
    }
}

void FormPart::setAddItogo(QString s)
{
    ui->labelNam->setText(s);
}

void FormPart::setOutItogo(QString s)
{
    ui->labelDef->setText(s);
}

void FormPart::setNamItogo(QString s)
{
    ui->labelProd->setText(s);
}

void FormPart::setShipItogo(QString s)
{
    ui->groupBoxShip->setTitle(s);
}

void FormPart::lblEd()
{
    if (partPackModel->rowCount()){
        int id_p = partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),0),Qt::EditRole).toInt();
        DialogOtk d(id_p);
        if (d.exec()==QDialog::Accepted){
            LblEngine e;
            //qDebug()<<d.getNam()<<" "<<d.getVol()<<" "<<d.getCod();
            e.createLblEd(id_p,d.getCod(),d.barCode());
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
        int id_p = partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),0),Qt::EditRole).toInt();
        QString name=modelPart->data(modelPart->index(ui->tableViewPart->currentIndex().row(),1),Qt::EditRole).toString();
        name+="_"+QString::number(modelPart->data(modelPart->index(ui->tableViewPart->currentIndex().row(),2),Qt::EditRole).toDate().year());
        int id_ship=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(index.row(),5),Qt::EditRole).toInt();
        name+="_"+ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(index.row(),1),Qt::EditRole).toString();
        name=name.replace(QRegExp("[^\\w]"), "_");
        sert->build(id_p,id_ship,name);
        editor->show();
    }
}

void FormPart::savePrim()
{
    int id_p = partPackModel->rowCount()>0 ? partPackModel->data(partPackModel->index(ui->comboBoxPack->currentIndex(),0),Qt::EditRole).toInt() : -1;
    if (id_p>0){
        QSqlQuery query;
        query.prepare("update wire_parti set prim_prod = :prim where id = :id");
        query.bindValue(":prim",ui->plainTextEditPrim->toPlainText());
        query.bindValue(":id",id_p);
        if (!query.exec()){
            QMessageBox::critical(this,tr("Ошибка"),query.lastError().text(),QMessageBox::Cancel);
        } else {
            ui->pushButtonSavePrim->setEnabled(false);
        }
    }
}

void FormPart::enPrimSave()
{
    ui->pushButtonSavePrim->setEnabled(true);
}

ModelPart::ModelPart(QObject *parent):DbTableModel("wire_parti_m",parent)
{
    id_prov=-1;
    addColumn("id","id");
    addColumn("n_s",tr("№"));
    addColumn("dat",tr("Дата"));
    addColumn("id_source",tr("Источник"),Rels::instance()->relSource);
    addColumn("id_buht",tr("Исх. партия"),Rels::instance()->relSrcPart);
    addColumn("id_provol",tr("Марка"),Rels::instance()->relProvol);
    addColumn("id_diam",tr("Ф"),Rels::instance()->relDiam);
    addColumn("id_type",tr("Стан"),Rels::instance()->relLine);
    addColumn("comment",tr("Комментарий"));
    addColumn("id_empl",tr("Волочильщик"),Rels::instance()->relVol);
    addColumn("kvo",tr("Масса"));
    addColumn("id_podt",tr("Подтяжка"),Rels::instance()->relPodt);
    setSort("dat,n_s");
    setDefaultValue(10,0);
    connect(this,SIGNAL(sigUpd()),Rels::instance()->relWirePart,SLOT(refreshModel()));
}


bool ModelPart::insertRow(int row, const QModelIndex &parent)
{
    select();
    int old_num=0;
    if (rowCount()>0){
        old_num=this->data(this->index(rowCount()-1,1),Qt::EditRole).toInt();
    }
    setDefaultValue(1,QString("%1").arg((old_num+1),4,'d',0,QChar('0')));
    setDefaultValue(2,QDate::currentDate());
    return DbTableModel::insertRow(row,parent);
}

QVariant ModelPart::data(const QModelIndex &index, int role) const
{
    if((role == Qt::BackgroundRole)) {
        int area = colorState.value(data(this->index(index.row(),0),Qt::EditRole).toInt());
        if(area == 0) return QVariant(QColor(255,170,170)); else
            if(area == 1) return QVariant(QColor(Qt::yellow)); else
                if(area == 2) return QVariant(QColor(Qt::gray)); else
                    if(area == 3) return QVariant(QColor(170,255,170));
    } else return DbTableModel::data(index,role);
}

bool ModelPart::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return DbTableModel::setData(index,value,role);
}

void ModelPart::copyChem(int id_wparti)
{
    QSqlQuery query;
    query.prepare("insert into wire_parti_chem (id_part, id_chem, value ) "
                  "(select :id_part1, pr.id_el, pr.kvo from prov_chem as pr "
                  "where pr.id_buht=(select p.id_buht from wire_parti_m as p where p.id= :id_part2 ))");
    query.bindValue(":id_part1",id_wparti);
    query.bindValue(":id_part2",id_wparti);
    if (query.exec()){
        refreshState();
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}

void ModelPart::setIdProv(int id)
{
    id_prov=id;
}

void ModelPart::setDateBeg(QDate date)
{
    dateBeg=date;
}

void ModelPart::setDateEnd(QDate date)
{
    dateEnd=date;
}

bool ModelPart::select()
{
    QString flt= id_prov==-1 ? "" : " and wire_parti_m.id_provol = "+QString::number(id_prov);
    setFilter("wire_parti_m.dat between '"+dateBeg.toString("yyyy.MM.dd")+"' and '"
                             +dateEnd.toString("yyyy.MM.dd")+"'"+flt);
    refreshState();
    return DbTableModel::select();
}

void ModelPart::refreshState()
{
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("select wire_parti_m.id, "
                      "(select case when exists(select id from wire_parti_chem where id_part=wire_parti_m.id) "
                               "then 1 else 0 end "
                               "+ "
                               "case when exists(select id from wire_parti_mech where id_part=wire_parti_m.id) "
                               "then 2 else 0 end "
                               "as r) from wire_parti_m where wire_parti_m.dat between :d1 and :d2");
    query.bindValue(":d1",dateBeg);
    query.bindValue(":d2",dateEnd);
    if (query.exec()){
        colorState.clear();
        while (query.next()){
            colorState[query.value(0).toInt()]=query.value(1).toInt();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}

PartPackModel::PartPackModel(QObject *parent):
    QSqlQueryModel(parent)
{
    refresh(-1);
}

void PartPackModel::refresh(int id_part)
{
    id_m=id_part;
    setQuery("select p.id, k.short||' '|| COALESCE(' ('||t.mas_ed::text||' "+tr("кг")+")',''), COALESCE(' ('||t.mas_ed::text||' "+tr("кг")+")',''), p.id_pack, p.id_pack_type "
             "from wire_parti as p "
             "inner join wire_pack_kind as k on k.id=p.id_pack "
             "left outer join wire_pack as t on t.id=p.id_pack_type "
             "where p.id_m = "+QString::number(id_part)+" "
             "order by k.short");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    }
}

bool PartPackModel::addPack(int id_pack, int id_type)
{
    QSqlQuery query;
    query.prepare("insert into wire_parti (id_pack, id_pack_type, id_m) values ( :id_pack, :id_pack_type, :id_m )");
    query.bindValue(":id_pack",id_pack);
    query.bindValue(":id_pack_type",id_type);
    query.bindValue(":id_m",id_m);
    bool ok = query.exec();
    if (!ok){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        Rels::instance()->relWirePart->refreshModel();
    }
    return ok;
}

bool PartPackModel::delPack(int id)
{
    QSqlQuery query;
    query.prepare("delete from wire_parti where id= :val ");
    query.bindValue(":val",id);
    bool ok = query.exec();
    if (!ok){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        Rels::instance()->relWirePart->refreshModel();
    }
    return ok;
}

bool PartPackModel::edtPack(int id, int id_pack_new, int id_type_new)
{
    QSqlQuery query;
    query.prepare("update wire_parti set id_pack = :id_pack, id_pack_type = :id_pack_type where id= :id ");
    query.bindValue(":id",id);
    query.bindValue(":id_pack",id_pack_new);
    query.bindValue(":id_pack_type",id_type_new);
    bool ok = query.exec();
    if (!ok){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        Rels::instance()->relWirePart->refreshModel();
    }
    return ok;
}

ModelGostSrc::ModelGostSrc(QObject *parent) : QSqlQueryModel(parent)
{
    refresh(-1);
}

void ModelGostSrc::refresh(int id_part)
{
    setQuery(QString("select g.id, g.nam from wire_gost as w "
                     "inner join gost_new as g on w.id_gost=g.id "
                     "where w.id_provol=(select m.id_provol from wire_parti_m as m where m.id = %1 ) order by g.nam").arg(id_part));
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1,Qt::Horizontal,QString::fromUtf8("Наименование"));
    }
}

ModelGostPart::ModelGostPart(QObject *parent) : DbTableModel("wire_parti_gost",parent)
{
    addColumn("id_parti","id_parti");
    addColumn("id_gost",QString::fromUtf8("Наименование"),Rels::instance()->relNewGost);
    setSort("gost_new.nam");
}

void ModelGostPart::refresh(int id_part)
{
    id_p=id_part;
    setFilter("wire_parti_gost.id_parti = "+QString::number(id_part));
    setDefaultValue(0,id_part);
    select();
}

void ModelGostPart::copyTu()
{
    QSqlQuery query;
    query.prepare("insert into wire_parti_gost (id_parti, id_gost) "
                  "select :id_p, g.id_gost from wire_gost as g where g.id_provol = (select m.id_provol from wire_parti_m as m where m.id = :id_part )");
    query.bindValue(":id_p",id_p);
    query.bindValue(":id_part",id_p);
    if (query.exec()){
        select();
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
}

ModelMechReal::ModelMechReal(QObject *parent) : DbTableModel("wire_mech",parent)
{
    addColumn("id","id");
    addColumn("id_part","id_part");
    addColumn("id_mech",tr("Параметр"),Rels::instance()->relMech);
    addColumn("kvo",tr("Значение"));
    setDecimals(3,3);
    setDefaultValue(2,1);
    setSort("wire_mech.id_mech, wire_mech.kvo");
}

void ModelMechReal::refresh(int id_part)
{
    setDefaultValue(1,id_part);
    setFilter("wire_mech.id_part = "+QString::number(id_part));
    select();
}

ModelWirePartiMech::ModelWirePartiMech(QObject *parent):DbTableModel("wire_parti_mech",parent)
{
    addColumn("id","id");
    addColumn("id_part","id_part");
    addColumn("id_mech",tr("Параметр"),Rels::instance()->relMech);
    addColumn("value",tr("Значение"));
    addColumn("value_max",tr("Значение-2"));
    addColumn("id_cat",tr("Категория"),Rels::instance()->relMechCat);
    setSort("mech_tbl.nam");
    setDecimals(3,3);
    setDecimals(4,3);
}

void ModelWirePartiMech::refresh(int id_part)
{
    setFilter("wire_parti_mech.id_part="+QString::number(id_part));
    id_wparti=id_part;
    setDefaultValue(1,id_wparti);
    select();
}

bool ModelWirePartiMech::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column()==2 && role==Qt::EditRole){
        int id_cat=0;
        int id_mech=value.toInt();
        if (id_mech==1 || id_mech==2 || id_mech==3 || id_mech==21)
            id_cat=0;
        else if (id_mech==23 || id_mech==24)
            id_cat=1;
        else id_cat=2;
        DbTableModel::setData(this->index(index.row(),5),id_cat,role);
    }
    return DbTableModel::setData(index,value,role);
}

ModelShipment::ModelShipment(QObject *parent):
    QSqlQueryModel(parent)
{
    refresh(-1);
}

QVariant ModelShipment::data(const QModelIndex &item, int role) const
{
    if (item.column()==3){
        if (role==Qt::TextAlignmentRole) return int(Qt::AlignRight | Qt::AlignVCenter);
        if (role==Qt::DisplayRole) return QLocale().toString(QSqlQueryModel::data(item,role).toDouble(),'f',2);
    }
    if (item.column()==0 && role==Qt::DisplayRole)
        return QSqlQueryModel::data(item,role).toDate().toString("dd.MM.yy");
    return QSqlQueryModel::data(item,role);
}

void ModelShipment::refresh(int id_part)
{
    setQuery("select s.dat_vid, s.nom_s, p.short, d.m_netto, rp.short, d.id "
             "from wire_shipment_consist as d "
             "inner join sertifikat as s on s.id=d.id_ship "
             "inner join poluch as p on s.id_pol=p.id "
             "inner join poluch as rp on d.id_pol=rp.id "
             "where d.id_wparti="+QString::number(id_part)+" order by s.dat_vid");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(0, Qt::Horizontal,tr("Дата"));
        setHeaderData(1, Qt::Horizontal,tr("№"));
        setHeaderData(2, Qt::Horizontal,tr("Получатель"));
        setHeaderData(3, Qt::Horizontal,tr("Масса, кг"));
        setHeaderData(4, Qt::Horizontal,tr("Реальный получатель"));
    }
    double sum=0;
    for (int i=0; i<rowCount(); i++){
        sum+=this->data(index(i,3),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (tr("Отгрузки итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : tr("Отгрузки");
    emit sigSum(s);
}

ModelUnionCex::ModelUnionCex(QObject *parent) : QSqlQueryModel(parent)
{

}

QVariant ModelUnionCex::data(const QModelIndex &item, int role) const
{
    QVariant origData=QSqlQueryModel::data(item,Qt::EditRole);
    QVariant::Type type=origData.type();
    if (role==Qt::DisplayRole){
        if (type==QMetaType::Double){
            return (origData.isNull()) ? QString("") : QLocale().toString(origData.toDouble(),'f',2);
        } else if (type==QMetaType::QDate){
            return (origData.isNull()) ? QString("") : origData.toDate().toString("dd.MM.yy");
        }
    } else if (role==Qt::TextAlignmentRole){
        return (type==QMetaType::Int || type==QMetaType::Double || type==QMetaType::Float || type==QMetaType::LongLong ) ?
                    int(Qt::AlignRight | Qt::AlignVCenter) : int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    return QSqlQueryModel::data(item,role);
}

void ModelUnionCex::refresh(int id_part)
{
    QSqlQuery query;
    query.prepare("select wpm.n_s||'-'||date_part('year',wpm.dat)||' '||p.nam||' ф '||d.sdim||' '||wpk.short , wicn.m_netto, wicn.dat , wl.snam, "
                  "we.first_name ||' '||substr(we.last_name,1,1)||'. '||substr(we.middle_name,1,1)||'.' "
                  "from wire_in_cex_nam wicn "
                  "inner join wire_parti wp on wp.id = wicn.id_wparti "
                  "inner join wire_parti_m wpm on wpm.id = wp.id_m "
                  "inner join provol p on p.id = wpm.id_provol "
                  "inner join diam d on d.id = wpm.id_diam "
                  "inner join wire_pack_kind wpk on wpk.id = wp.id_pack "
                  "inner join wire_line wl on wl.id = wicn.id_line "
                  "inner join wire_empl we on we.id = wicn.id_empl "
                  "where wicn.id_wpartipack = :id "
                  "order by wicn.dat, wicn.id");
    query.bindValue(":id",id_part);
    if (query.exec()){
        setQuery(query);
        setHeaderData(0,Qt::Horizontal,tr("Партия"));
        setHeaderData(1,Qt::Horizontal,tr("Масса, кг"));
        setHeaderData(2,Qt::Horizontal,tr("Дата"));
        setHeaderData(3,Qt::Horizontal,tr("Линия"));
        setHeaderData(4,Qt::Horizontal,tr("Работник"));
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
    }
    double sum=0;
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,1),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (tr("Объединение итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : tr("Объединение");
    emit sigSum(s);
}

ModelNamCex::ModelNamCex(QObject *parent) : DbTableModel("wire_in_cex_nam",parent)
{
    addColumn("id",tr("id"));
    addColumn("id_wparti",tr("id_wparti"));
    addColumn("dat",tr("Дата"));
    addColumn("m_netto",tr("Масса, кг"));
    addColumn("id_empl",tr("Работник"),Rels::instance()->relRab);
    addColumn("id_line",tr("Линия"),Rels::instance()->relLine);
    addColumn("id_wpartipack",tr("Объединенная партия"),Rels::instance()->relWirePart);
    setSort("wire_in_cex_nam.dat");
    setDecimals(3,2);
    connect(this,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(this,SIGNAL(sigRefresh()),this,SLOT(calcSum()));
}

void ModelNamCex::refresh(int id_part)
{
    setDefaultValue(1,id_part);
    setFilter("wire_in_cex_nam.id_wparti = "+QString::number(id_part));
    select();
}

void ModelNamCex::calcSum()
{
    double sum=0;
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,3),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (tr("Производство итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : tr("Производство");
    emit sigSum(s);
}

ModelInCex::ModelInCex(QObject *parent):
    DbTableModel("wire_in_cex_data",parent)
{
    addColumn("id",tr("id"));
    addColumn("id_wparti",tr("id_wparti"));
    addColumn("dat",tr("Дата"));
    addColumn("m_netto",tr("Масса, кг"));
    addColumn("id_type",tr("Операция"),Rels::instance()->relAddType);
    setDefaultValue(4,3);
    setDecimals(3,2);
    setSort("wire_in_cex_data.dat");
    connect(this,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(this,SIGNAL(sigRefresh()),this,SLOT(calcSum()));
}

void ModelInCex::refresh(int id_part)
{
    setDefaultValue(1,id_part);
    setFilter("wire_in_cex_data.id_wparti = "+QString::number(id_part)+" and wire_in_cex_type.koef = 1");
    select();
}

void ModelInCex::calcSum()
{
    double sum=0;
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,3),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (tr("Намотка итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : tr("Намотка");
    emit sigSum(s);
}


ModelOutCex::ModelOutCex(QObject *parent) :
    DbTableModel("wire_in_cex_data",parent)
{
    addColumn("id",tr("id"));
    addColumn("id_wparti",tr("id_wparti"));
    addColumn("dat",tr("Дата"));
    addColumn("m_netto",tr("Масса, кг"));
    addColumn("id_type",tr("Причина"),Rels::instance()->relCause);
    setDecimals(3,2);
    setSort("wire_in_cex_data.dat");
    connect(this,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(this,SIGNAL(sigRefresh()),this,SLOT(calcSum()));
}

void ModelOutCex::refresh(int id_part)
{
    setDefaultValue(1,id_part);
    setFilter("wire_in_cex_data.id_wparti = "+QString::number(id_part)+" and wire_in_cex_type.koef = -1");
    select();
}

void ModelOutCex::calcSum()
{
    double sum=0;
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,3),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (tr("Списание итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : tr("Списание");
    emit sigSum(s);
}

ModelPackCex::ModelPackCex(QObject *parent) : DbTableModel("wire_parti_pack",parent)
{
    addColumn("id",tr("id"));
    addColumn("id_part",tr("id_part"));
    addColumn("dat",tr("Дата"));
    addColumn("kvo",tr("Масса, кг"));
    setDecimals(3,2);
    setSort("wire_parti_pack.dat");
    connect(this,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(this,SIGNAL(sigRefresh()),this,SLOT(calcSum()));
}

void ModelPackCex::refresh(int id_part)
{
    setDefaultValue(1,id_part);
    setFilter("wire_parti_pack.id_part = "+QString::number(id_part));
    select();
}

void ModelPackCex::calcSum()
{
    double sum=0;
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,3),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum>0)? (tr("Упаковка итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : tr("Упаковка");
    emit sigSum(s);
}

ModelStockCex::ModelStockCex(QObject *parent):
    QSqlQueryModel(parent)
{

}

QVariant ModelStockCex::data(const QModelIndex &item, int role) const
{
    if (item.column()==3){
        if (role==Qt::TextAlignmentRole) return int(Qt::AlignRight | Qt::AlignVCenter);
        if (role==Qt::DisplayRole) return QLocale().toString(QSqlQueryModel::data(item,role).toDouble(),'f',2);
    }
    if (item.column()==0 && role==Qt::DisplayRole)
        return QSqlQueryModel::data(item,role).toDate().toString("dd.MM.yy");
    return QSqlQueryModel::data(item,role);
}

void ModelStockCex::refresh(int id_part)
{
    setQuery("select i.dat, i.num, t.nam, d.m_netto*t.koef from wire_warehouse as d "
             "inner join wire_whs_waybill as i on i.id=d.id_waybill "
             "inner join wire_way_bill_type as t on t.id=i.id_type "
             "where d.id_wparti="+QString::number(id_part)+" and (i.id_type=3 or i.id_type=5) order by i.dat");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(0, Qt::Horizontal,tr("Дата"));
        setHeaderData(1, Qt::Horizontal,tr("Накл."));
        setHeaderData(2, Qt::Horizontal,tr("Операция"));
        setHeaderData(3, Qt::Horizontal,tr("Масса, кг"));
    }
    calcSum();
}

void ModelStockCex::calcSum()
{
    double sum=0;
    QString title=tr("Склад");
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,3),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum!=0)? (title + tr(" итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : title;
    emit sigSum(s);
}

ModelPerepackCex::ModelPerepackCex(QObject *parent) :
    QSqlQueryModel(parent)
{

}

QVariant ModelPerepackCex::data(const QModelIndex &item, int role) const
{
    if (item.column()==3){
        if (role==Qt::TextAlignmentRole) return int(Qt::AlignRight | Qt::AlignVCenter);
        if (role==Qt::DisplayRole) return QLocale().toString(QSqlQueryModel::data(item,role).toDouble(),'f',2);
    }
    if (item.column()==0 && role==Qt::DisplayRole)
        return QSqlQueryModel::data(item,role).toDate().toString("dd.MM.yy");
    return QSqlQueryModel::data(item,role);
}

void ModelPerepackCex::refresh(int id_part)
{
    setQuery("(select n.dat, n.num, m.n_s ||'-'||date_part('year',m.dat) as nam, -1*p.m_netto as mas from wire_perepack as p "
             "inner join wire_perepack_nakl as n on p.id_nakl=n.id "
             "inner join wire_parti as wp on p.id_wpartires=wp.id "
             "inner join wire_parti_m as m on wp.id_m=m.id "
             "where p.id_wpartisrc= "+QString::number(id_part)+" ) "
             "union all "
             "(select n.dat, n.num, m.n_s ||'-'||date_part('year',m.dat) as nam, p.m_netto from wire_perepack as p "
             "inner join wire_perepack_nakl as n on p.id_nakl=n.id "
             "inner join wire_parti as wp on p.id_wpartisrc=wp.id "
             "inner join wire_parti_m as m on wp.id_m=m.id "
             "where p.id_wpartires= "+QString::number(id_part)+" ) "
             "order by dat, nam");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(0, Qt::Horizontal,tr("Дата"));
        setHeaderData(1, Qt::Horizontal,tr("Накл."));
        setHeaderData(2, Qt::Horizontal,tr("Партия"));
        setHeaderData(3, Qt::Horizontal,tr("Масса, кг"));
    }
    calcSum();
}

void ModelPerepackCex::calcSum()
{
    double sum=0;
    QString title=tr("Переупаковка");
    for (int i=0; i<rowCount(); i++){
        sum+=data(index(i,3),Qt::EditRole).toDouble();
    }
    QString s;
    s = (sum!=0)? (title + tr(" итого: ")+QLocale().toString(sum,'f',2)+tr(" кг")) : title;
    emit sigSum(s);
}
