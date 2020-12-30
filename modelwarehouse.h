#ifndef MODELWAREHOUSE_H
#define MODELWAREHOUSE_H

#include <QSqlQueryModel>
#include <QSqlError>
#include <QDate>
#include "db/dbtablemodel.h"
#include "models.h"

class ModelWarehouse : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelWarehouse(QObject *parent = 0);
public slots:
    void refresh();
    void setNotZero(bool val);
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
    void setDate(QDate d);
private:
    bool notZero;
    QDate date;
    
signals:
    
public slots:
    
};

class ModelCex : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelCex(QObject *parent=0);
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
public slots:
    void refresh(int id_part, QDate date);
};

class ModelStock : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelStock(QObject *parent=0);
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
public slots:
    void refresh(int id_part, QDate date);
};

class ModelShipment : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelShipment(QObject *parent=0);
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
public slots:
    void refresh(int id_part, QDate date);
signals:
    void sigSum(QString sum);
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

class ModelNakl : public DbTableModel
{
    Q_OBJECT
public:
    ModelNakl(QObject *parent=0);
    void refresh(QDate beg, QDate end);
    bool insertRow(int row, const QModelIndex &parent);
};

class ModelPerepack : public DbTableModel
{
    Q_OBJECT
public:
    ModelPerepack(QObject *parent=0);
    void refresh(int id_nakl);
    bool submit();
public slots:
    void refreshPart();
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

class ModelPodt : public DbTableModel
{
    Q_OBJECT
public:
    ModelPodt(QObject *parent=0);
    void refresh(QDate beg, QDate end);
    bool insertRow(int row, const QModelIndex &parent);
};

class ModelPodtPart : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelPodtPart(QObject *parent=0);
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
public slots:
    void refresh(int id_podt);
};

class ModelPodtCont : public DbTableModel
{
    Q_OBJECT
public:
    ModelPodtCont(QObject *parent=0);
    void refresh(int id_part);
signals:
    void sigSum(QString sum);
private slots:
    void calcSum();
};

class ModelPodtNext : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelPodtNext(QObject *parent=0);
    void refresh(int id_podt);
};

class ModelPodtAnn : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelPodtAnn(QObject *parent=0);
    void refresh(int id_podt);
};

class ModelNaklGen : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelNaklGen(QObject *parent=0);
    void refresh(QDate beg, QDate end, int id_type);
};

class ModelNaklGenCont : public QSqlQueryModel
{
    Q_OBJECT
public:
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
    ModelNaklGenCont(QObject *parent=0);
    void refresh(QDate dat, int id_type);
signals:
    void sigSum(QString sum);
};

class ModelNaklPodt : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelNaklPodt(QObject *parent=0);
    void refresh(QDate beg, QDate end, int id_type, int id_podt_type);
};

class ModelNaklPodtCont : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelNaklPodtCont(QObject *parent=0);
    void refresh(QDate dat, int id_type);
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
signals:
    void sigSum(QString sum);
};

class ModelPodtProd : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelPodtProd(QObject *parent);
    void refresh(QDate beg, QDate end);
    QVariant data(const QModelIndex &item, int role) const;
    bool ready();
private:
    QStringList exList;
public slots:
    void updState();
};

#endif // MODELWAREHOUSE_H
