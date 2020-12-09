#ifndef MODELS_EDT_H
#define MODELS_EDT_H
#include "db/dbtablemodel.h"
#include "models.h"

class ModelPart : public DbTableModel
{
    Q_OBJECT
public:
    ModelPart(QObject *parent=0);
    bool submitRow();
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

class ModelAnn : public DbTableModel
{
    Q_OBJECT
public:
    ModelAnn(QObject *parent=0);
    void refresh(QDate beg, QDate end);
    bool insertRow(int row, const QModelIndex &parent);
};

class ModelPodtMech : public DbTableModel
{
    Q_OBJECT
public:
    ModelPodtMech(QObject *parent=0);
    void refresh(int id_podt);
};

class ModelMechReal : public DbTableModel
{
    Q_OBJECT
public:
    ModelMechReal(QObject *parent=0);
    void refresh(int id_part);
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

class ModelEan : public DbTableModel
{
    Q_OBJECT
public:
    ModelEan(QObject *parent=0);
    void refresh(int id_prov);
public slots:
    void updRels(QModelIndex index);
};

class ModelPodtCex : public DbTableModel
{
    Q_OBJECT
public:
    ModelPodtCex(QObject *parent=0);
    void refresh(int id_podt, int id_op);
private slots:
    void calcSum();
signals:
    void sigSum(QString sum);
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

#endif // MODELS_EDT_H
