#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QtSql>
#include "db/dbtablemodel.h"

class Models : public QObject
{
    Q_OBJECT
public:
    static Models *instance();
    DbRelation *relProvol;
    DbRelation *relDiam;
    DbRelation *relPack;
    DbRelation *relPackType;
    DbRelation *relSource;
    DbRelation *relSrcPart;
    DbRelation *relRab;
    DbRelation *relOp;
    DbRelation *relLine;
    DbRelation *relWirePart;
    DbRelation *relNewGost;
    DbRelation *relZvdSert;
    DbRelation *relChemTbl;
    DbRelation *relMechTbl;
    DbRelation *relRoute;
    DbRelation *relMechCat;
    DbRelation *relType;
    DbRelation *relCause;
    DbRelation *relAddType;
    DbRelation *relPodt;
    DbRelation *relOven;
    DbRelation *relVol;
    DbRelation *relNam;
    DbRelation *relOtk;
    DbRelation *relNaklType;
    DbRelation *relVedPix;
    DbRelation *relWireType;
    DbRelation *relEan;
    DbRelation *relPodtType;
    DbRelation *relEd;
    DbRelation *relMatr;
    DbRelation *relRasxVid;

public slots:
    void refresh();

signals:
    void sigRefresh();

protected:
    Models(QObject *parent = 0);

private:
    static Models* models_instance;
    QVector<DbRelation*> rels;
};

#endif // MODELS_H
