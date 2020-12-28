#include "models.h"

Models* Models::models_instance=0;

Models::Models(QObject *parent) :
    QObject(parent)
{
    relProvol = new DbRelation(new DbRelationalModel(QString("select id, nam from provol where is_w=1 order by nam"), this),0,1,this);
    relDiam = new DbRelation(new DbRelationalModel(QString("select id, sdim from diam order by sdim"), this),0,1,this);
    relPack = new DbRelation(new DbRelationalModel(QString("select id, nam, short, id_pack_def from wire_pack_kind order by nam"), this),0,2,this);
    relPackType = new DbRelation(new DbRelationalModel(QString("select id, pack_ed ||', '|| pack_group as nam from wire_pack order by nam"), this),0,1,this);
    relSource = new DbRelation(new DbRelationalModel(QString("select id, nam from wire_source order by nam"), this),0,1,this);
    relSrcPart = new DbRelation(new DbRelationalModel(QString("select h.id as id_buht, "
                                                 "case when h.id<>0 then "
                                                 "(h.n_plav ||' '|| cast(p.dat_pr as varchar(96)) ||"+tr("' п.'")+ "|| h.parti ||' '|| r.nam ||"+ tr("' ф.'") +"|| d.sdim) "
                                                 "else '-' end, h.n_plav, date_part('year',p.dat_pr) "
                                                 "from prov_buht h "
                                                 "inner join prov_prih p on p.id=h.id_prih "
                                                 "inner join provol r on r.id=p.id_pr "
                                                 "inner join diam d on d.id=p.id_dim "
                                                 "where (h.n_plav<>'') and (h.parti<>'') "
                                                 "order by p.dat_pr desc, h.parti"), this),0,1,this);
    relRab = new DbRelation(new DbRelationalModel(QString("select id, first_name||' '||last_name||' '||middle_name||' id='||id from wire_empl order by first_name"), this),0,1,this);
    relOp = new DbRelation(new DbRelationalModel(QString("select id, naim from wire_rab_liter where id_zon<4 order by naim"), this),0,1,this);
    relLine = new DbRelation(new DbRelationalModel(QString("select id, snam from wire_line order by snam"), this),0,1,this);
    relWirePart = new DbRelation(new DbRelationalModel(QString("select p.id, m.n_s ||'-'||date_part('year',m.dat) ||' '||pr.nam ||' '|| d.sdim || ' '|| k.short  as part, date_part('year',m.dat) "
                                                  "from wire_parti as p "
                                                  "inner join wire_parti_m as m on p.id_m=m.id "
                                                  "inner join provol as pr on pr.id=m.id_provol "
                                                  "inner join diam as d on d.id=m.id_diam "
                                                  "inner join wire_pack_kind as k on k.id=p.id_pack "
                                                  "order by part desc"), this),0,1,this);
    relNewGost = new DbRelation(new DbRelationalModel(QString("select id, nam from gost_new order by nam"), this),0,1,this);
    relZvdSert = new DbRelation(new DbRelationalModel(QString("select id, nom_doc from zvd_sert order by nom_doc"), this),0,1,this);
    relChemTbl = new DbRelation(new DbRelationalModel(QString("select id, sig from chem_tbl order by sig"), this),0,1,this);
    relMechTbl = new DbRelation(new DbRelationalModel(QString("select id, nam from mech_tbl order by nam"), this),0,1,this);
    relRoute = new DbRelation(new DbRelationalModel(QString("select id, nam from wire_route order by nam"), this),0,1,this);
    relMechCat = new DbRelation(new DbRelationalModel(QString("select id, nam from mech_category order by nam"), this),0,1,this);
    relType = new DbRelation(new DbRelationalModel(QString("select id, nam from wire_type order by nam"), this),0,1,this);
    relCause = new DbRelation(new DbRelationalModel(QString("select id, short from wire_in_cex_type where koef=-1 order by nam"), this),0,1,this);
    relAddType = new DbRelation(new DbRelationalModel(QString("select id, short from wire_in_cex_type where koef=1 order by nam"), this),0,1,this);
    relPodt = new DbRelation(new DbRelationalModel(QString("select pd.id, pd.n_s || '-'|| date_part('year',pd.dat) ||' '||r.nam ||' '||d.sdim || ' ('||  h.n_plav || ')' as str, h.id, date_part('year',pd.dat) as year "
                                              "from wire_podt as pd "
                                              "inner join prov_buht as h on pd.id_buht=h.id "
                                              "inner join prov_prih as p on p.id=h.id_prih "
                                              "inner join provol as r on r.id=p.id_pr "
                                              "inner join diam as d on pd.id_diam=d.id "
                                              "order by year desc, str desc"), this),0,1,this);
    relOven = new DbRelation(new DbRelationalModel(QString("select id, nam from wire_oven order by nam"),this),0,1,this);
    relVol = new DbRelation(new DbRelationalModel(QString("select r.id, r.first_name||' '||substr(r.last_name,1,1)||'. '||substr(r.middle_name,1,1)||'. '||'('||n.num||')' as rab, n.num "
                                                          "from wire_namoch as n "
                                                          "inner join wire_empl as r on n.id_rab = r.id "
                                                          "where r.id <> 0 and n.id_pr = 2 "
                                                          "order by rab"),this),0,1,this);
    relNam = new DbRelation(new DbRelationalModel(QString("select r.id, r.first_name||' '||substr(r.last_name,1,1)||'. '||substr(r.middle_name,1,1)||'. '||'('||n.num||')' as rab, n.num "
                                                          "from wire_namoch as n "
                                                          "inner join wire_empl as r on n.id_rab = r.id "
                                                          "where r.id <> 0 and n.id_pr = 3 "
                                                          "order by rab"),this),0,1,this);
    relOtk = new DbRelation(new DbRelationalModel(QString("select id, nam from otk order by nam"),this),0,1,this);
    relNaklType = new DbRelation(new DbRelationalModel(QString("select id, short from wire_in_cex_type order by sort_order"), this),0,1,this);    
    relVedPix = new DbRelation(new DbRelationalModel(QString("select id, simb from zvd_ved"),this),0,1,this);
    relWireType = new DbRelation(new DbRelationalModel(QString("select id, nam from provol_type order by id"), this),0,1,this);
    relEan = new DbRelation(new DbRelationalModel(QString("select ean from eans order by ean"), this),0,0,this);
    relPodtType = new DbRelation(new DbRelationalModel(QString("select id, nam from wire_podt_type order by id"), this),0,1,this);

    rels.push_back(relProvol);
    rels.push_back(relDiam);
    rels.push_back(relPack);
    rels.push_back(relPackType);
    rels.push_back(relSource);
    rels.push_back(relSrcPart);
    rels.push_back(relRab);
    rels.push_back(relOp);
    rels.push_back(relLine);
    rels.push_back(relWirePart);
    rels.push_back(relNewGost);
    rels.push_back(relZvdSert);
    rels.push_back(relChemTbl);
    rels.push_back(relMechTbl);
    rels.push_back(relRoute);
    rels.push_back(relMechCat);
    rels.push_back(relType);
    rels.push_back(relCause);
    rels.push_back(relAddType);
    rels.push_back(relPodt);
    rels.push_back(relOven);
    rels.push_back(relVol);
    rels.push_back(relNam);
    rels.push_back(relOtk);
    rels.push_back(relNaklType);
    rels.push_back(relVedPix);
    rels.push_back(relWireType);
    rels.push_back(relEan);
    rels.push_back(relPodtType);

    int year=QDate::currentDate().year();

    relPodt->proxyModel()->setFilterKeyColumn(3);
    relPodt->proxyModel()->setFilterRegExp(QString::number(year));

    relWirePart->proxyModel()->setFilterKeyColumn(2);
    relWirePart->proxyModel()->setFilterRegExp(QRegExp(QString::number(year-1)+"|"+QString::number(year)));

    relSrcPart->proxyModel()->setFilterKeyColumn(3);
    relSrcPart->proxyModel()->setFilterRegExp(QRegExp(QString::number(year-1)+"|"+QString::number(year)));

    relEan->proxyModel()->setFilterKeyColumn(0);
}

Models *Models::instance()
{
    if (models_instance == 0)
        models_instance = new Models;
    return models_instance;
}

void Models::refresh()
{
    for (int i=0; i < rels.size(); ++i) {
        rels.at(i)->refreshModel();
    }
    emit sigRefresh();
}
