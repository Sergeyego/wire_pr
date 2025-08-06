#ifndef RELS_H
#define RELS_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "invoicemanager.h"
#include "modelro.h"

class Rels : public QObject
{
    Q_OBJECT
public:
    static Rels *instance();
    DbSqlRelation *relChem;
    DbSqlRelation *relDiam;
    DbSqlRelation *relNewGost;
    DbSqlRelation *relProvol;
    DbSqlRelation *relMark;
    DbSqlRelation *relWireType;
    DbSqlRelation *relPack;
    DbSqlRelation *relPackEd;
    DbSqlRelation *relPackType;
    DbSqlRelation *relEanEd;
    DbSqlRelation *relEanGr;
    DbSqlRelation *relLine;
    DbSqlRelation *relEd;
    DbSqlRelation *relMatr;
    DbSqlRelation *relSrcPart;
    DbSqlRelation *relPodtType;
    DbSqlRelation *relType;
    DbSqlRelation *relPodt;
    DbSqlRelation *relRab;
    DbSqlRelation *relVol;
    DbSqlRelation *relPodtOp;
    DbSqlRelation *relRasxVid;
    DbSqlRelation *relSource;
    DbSqlRelation *relWirePart;
    DbSqlRelation *relWirePartNew;
    DbSqlRelation *relMech;
    DbSqlRelation *relMechCat;
    DbSqlRelation *relAddType;
    DbSqlRelation *relCause;
    DbSqlRelation *relNaklType;
    DbSqlRelation *relSertType;

    ModelRo *modelNam;
    InvoiceManager *invoiceManager;

public slots:
    void refreshNam();

protected:
    explicit Rels(QObject *parent = nullptr);

private:
    static Rels *rels_instance;

};

#endif // RELS_H
