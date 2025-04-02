#ifndef FORMPART_H
#define FORMPART_H

#include <QWidget>
#include <QInputDialog>
#include "rels.h"
#include "db/dbmapper.h"
#include "dialogpack.h"
#include "lblengine.h"
#include "dialogotk.h"
#include "modelstu.h"

namespace Ui {
class FormPart;
}

class ModelPart : public DbTableModel
{
    Q_OBJECT
public:
    ModelPart(QObject *parent=0);
    bool insertRow(int row, const QModelIndex &parent);
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void copyChem(int id_wparti);
    void setIdProv(int id);

private:
    QDate dateBeg;
    QDate dateEnd;
    QMap <int,int> colorState;
    int id_prov;

public slots:
    void setDateBeg(QDate date);
    void setDateEnd(QDate date);
    bool select();
    void refreshState();
};

class PartPackModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    PartPackModel(QObject *parent=0);
    void refresh(int id_part);
    bool addPack(int id_pack, int id_type);
    bool delPack(int id);
    bool edtPack(int id, int id_pack_new, int id_type_new);
private:
    int id_m;
};

class ModelGostSrc : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelGostSrc(QObject *parent=0);
    void refresh(int id_part);
};

class ModelGostPart : public DbTableModel
{
    Q_OBJECT
public:
    ModelGostPart(QObject *parent=0);
    void refresh(int id_part);
private:
    int id_p;
public slots:
    void copyTu();
};

class ModelMechReal : public DbTableModel
{
    Q_OBJECT
public:
    ModelMechReal(QObject *parent=0);
    void refresh(int id_part);
};

class ModelWirePartiMech : public DbTableModel
{
    Q_OBJECT
public:
    ModelWirePartiMech(QObject *parent=0);
    void refresh(int id_part);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
private:
    int id_wparti;
};

class ModelShipment : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelShipment(QObject *parent=0);
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
public slots:
    void refresh(int id_part);
signals:
    void sigSum(QString sum);
};

class ModelUnionCex : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelUnionCex(QObject *parent=0);
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
public slots:
    void refresh(int id_part);
signals:
    void sigSum(QString sum);
};

class ModelNamCex : public DbTableModel
{
    Q_OBJECT
public:
    ModelNamCex(QObject *parent=0);
    void refresh(int id_part);
signals:
    void sigSum(QString sum);
private slots:
    void calcSum();
};

class ModelInCex : public DbTableModel
{
    Q_OBJECT
public:
    ModelInCex(QObject *parent=0);
    void refresh(int id_part);
signals:
    void sigSum(QString sum);
private slots:
    void calcSum();
};

class ModelOutCex : public DbTableModel
{
    Q_OBJECT
public:
    ModelOutCex(QObject *parent=0);
    void refresh(int id_part);
signals:
    void sigSum(QString sum);
private slots:
    void calcSum();
};

class ModelPackCex : public DbTableModel
{
    Q_OBJECT
public:
    ModelPackCex(QObject *parent=0);
    void refresh(int id_part);
signals:
    void sigSum(QString sum);
private slots:
    void calcSum();
};

class ModelStockCex : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelStockCex(QObject *parent=0);
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
public slots:
    void refresh(int id_part);
    void calcSum();
signals:
    void sigSum(QString sum);
};

class ModelPerepackCex : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelPerepackCex(QObject *parent=0);
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
public slots:
    void refresh(int id_part);
    void calcSum();
signals:
    void sigSum(QString sum);
};

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
    PartPackModel *partPackModel;
    ModelGostSrc *modelGostSrc;
    ModelGostPart *modelGostPart;
    ModelChemSrc *srsChemModel;
    ModelChemSert *chemModel;
    ModelMechReal *modelMechReal;
    ModelWirePartiMech *mechModel;
    ModelShipment *modelShip;
    ModelUnionCex *modelUnionCex;
    ModelNamCex *modelNamCex;
    ModelInCex *modelInCex;
    ModelOutCex *modelOutCex;
    ModelPackCex *modelPackCex;
    ModelStockCex *modelStockCex;
    ModelPerepackCex *modelPerepackCex;


    void loadSettings();
    void saveSettings();
    void blockShip(bool val);

private slots:
    void updData(int row);
    void copyChem();
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
    void savePrim();
    void enPrimSave();
};

#endif // FORMPART_H
