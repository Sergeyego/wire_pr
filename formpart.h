#ifndef FORMPART_H
#define FORMPART_H

#include <QWidget>
#include "models.h"
#include "db/dbmapper.h"
#include "models_edt.h"
#include "editor.h"
#include "sertbuild.h"
#include "modelwarehouse.h"
#include "dialogpack.h"
#include "lblengine.h"
#include "dialogotk.h"
#include "dialogflt.h"
#include "modelstu.h"

namespace Ui {
class FormPart;
}

class FormPart : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormPart(bool edtSert = true, QWidget *parent = 0);
    ~FormPart();
    
private:
    Ui::FormPart *ui;
    DbMapper *push;
    ModelPart *modelPart;
    ModelChemSert *chemModel;
    ModelWirePartiMech *mechModel;
    PartPackModel *partPackModel;
    ModelChemSrc *srsChemModel;
    ModelShipment *modelShip;
    ModelInCex *modelInCex;
    ModelOutCex *modelOutCex;
    ModelNamCex *modelNamCex;
    ModelPackCex *modelPackCex;
    ModelUnionCex *modelUnionCex;
    void loadSettings();
    void saveSettings();
    SertBuild *sert;
    Editor *editor;
    void blockShip(bool val);
    ModelMechReal *modelMechReal;
    ModelGostSrc *modelGostSrc;
    ModelGostPart *modelGostPart;
    ModelStockCex *modelStockCex;
    ModelPerepackCex *modelPerepackCex;

private slots:
    void updData(int row);
    void copyChem();
    void showSert();
    void setFilter();
    void updShip();
    void addPack();
    void delPack();
    void edtPack();
    void edtVar();
    void setAddItogo(QString s);
    void setOutItogo(QString s);
    void setNamItogo(QString s);
    void setShipItogo(QString s);
    void lblEd();
    void lblGroup();
    void showSertShip(QModelIndex index);
    void fltPodt();
    void fltSrc();
    void savePrim();
    void enPrimSave();
};

#endif // FORMPART_H
