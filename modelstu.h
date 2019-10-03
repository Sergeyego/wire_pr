#ifndef MODELSTU_H
#define MODELSTU_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "models.h"

struct range {
    QVariant min;
    QVariant max;
};

class ModelChem : public DbTableModel
{
public:
    ModelChem(QString tname, QObject *parent=0);
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    ~ModelChem();
    void refresh(int id_part);
private:
    QMap <int,range> map;
    QColor getColor(const QModelIndex &index) const;
    QString getToolTip(const QModelIndex &index) const;

protected:
    QString tablename;
    int colVal;
    int colIdChem;
    int colIdPart;
    QString flt;
    QString tuQuery;
};

class ModelChemSrc : public ModelChem
{
public:
    ModelChemSrc(QObject *parent=0);
};

class ModelChemSert : public ModelChem
{
public:
    ModelChemSert(QObject *parent=0);
};


#endif // MODELSTU_H
