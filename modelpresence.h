#ifndef MODELPRESENCE_H
#define MODELPRESENCE_H

#include <QSqlQueryModel>
#include <QtSql>

class ModelPresence : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelPresence(QObject *parent = 0);
    void refresh(QDate date, bool bypart);
    QVariant data(const QModelIndex &index,int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    
private:
    bool by_Part;
    
public slots:

signals:
    void sigUpd();
    
};

class ModelReport : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelReport(QObject *parent = 0);
    void refresh(QDate begDate, QDate endDate, bool bypart);
    QVariant data(const QModelIndex &index,int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:
    bool by_Part;
};

class ModelPresencePodt : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelPresencePodt(QObject *parent = 0);
    void refresh(QDate date, bool bypart);
    QVariant data(const QModelIndex &index,int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:
    bool by_Part;
    double sum;
    int colsum;
};

#endif // MODELPRESENCE_H
