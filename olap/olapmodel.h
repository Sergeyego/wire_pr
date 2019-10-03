#ifndef OLAPMODEL_H
#define OLAPMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "cubic.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

class OlapModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit OlapModel(QStringList axes, int dec, QObject *parent = 0);
    ~OlapModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void setQuery(QString qu);
    
signals:
    void sigRefresh();
    
public slots:
    void refresh();
    void setTypeSum(bool b);
    void setTypeAvg(bool b);
    void setTypeMax(bool b);
    void setTypeMin(bool b);
    void setX(QStringList x);
    void setY(QStringList y);

private:
    hyper_cube *hCube;
    int decimal;
    i_dims yI,xJ;
    QMap<QString,int> m_axe;
    int n;
    Ttl *pTtl;
    Sum *tSum;
    Avg *tAvg;
    Min *tMin;
    Max *tMax;

    
};

#endif // OLAPMODEL_H
