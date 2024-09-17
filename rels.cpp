#include "rels.h"

Rels* Rels::rels_instance=nullptr;

Rels *Rels::instance()
{
    if (rels_instance==nullptr)
        rels_instance = new Rels();
    return rels_instance;
}

void Rels::refreshNam()
{
    QSqlQuery query;
    query.prepare("select r.id, r.first_name||' '||substr(r.last_name,1,1)||'. '||substr(r.middle_name,1,1)||'. '||'('||n.num||')' as rab, n.num "
                  "from wire_namoch as n "
                  "inner join wire_empl as r on n.id_rab = r.id "
                  "where r.id <> 0 and n.id_pr = 3 "
                  "order by rab");
    modelNam->execQuery(query);
}

Rels::Rels(QObject *parent) : QObject(parent)
{  
    relChem = new DbSqlRelation("chem_tbl","id","sig",this);
    relChem->setSort("chem_tbl.sig");

    relDiam = new DbSqlRelation("diam","id","sdim",this);
    relDiam->setSort("diam.sdim");

    relNewGost = new DbSqlRelation("gost_new","id","nam",this);
    relNewGost->setSort("gost_new.nam");

    relProvol = new DbSqlRelation("provol","id","nam",this);
    relProvol->setAlias("relprov");
    relProvol->setSort("relprov.nam");
    relProvol->setFilter("relprov.is_w=1");

    relMark = new DbSqlRelation("provol","id","nam",this);
    relMark->setSort("provol.nam");

    relWireType = new DbSqlRelation("provol_type","id","nam",this);
    relWireType->setSort("provol_type.nam");

    relPack = new DbSqlRelation("wire_pack_kind","id","short",this);
    relPack->setFilterColumn("id_pack_def");
    relPack->setSort("wire_pack_kind.short");

    relPackType = new DbSqlRelation("wire_pack","id","pack_ed",this);
    relPackType->setSort("wire_pack.pack_ed");

    relEanEd = new DbSqlRelation("ean_free","ean","ean",this);
    relEanEd->model()->setAsync(false);
    relEanEd->setAlias("eansEd");
    relEanEd->setFilter("eansEd.ean_free = true");

    relEanGr = new DbSqlRelation("ean_free","ean","ean",this);
    relEanGr->model()->setAsync(false);
    relEanGr->setAlias("eansGr");
    relEanGr->setFilter("eansGr.ean_free = true");

    relLine = new DbSqlRelation("wire_line","id","snam",this);
    relLine->setSort("wire_line.snam");

    relEd = new DbSqlRelation("units","id","nam",this);
    relEd->setSort("units.id");

    relMatr = new DbSqlRelation("wire_matr","id","nam",this);
    relMatr->setSort("wire_matr.nam");

    relSrcPart = new DbSqlRelation("prov_buht","id","str",this);
    relSrcPart->setSort("coalesce(date_part('year',prov_buht.dt_cre),2010) desc, prov_buht.parti, prov_buht.str");
    relSrcPart->model()->setLimit(4000);

    relPodtType = new DbSqlRelation("wire_podt_type","id","snam",this);
    relPodtType->setSort("wire_podt_type.id");

    relType = new DbSqlRelation("wire_type","id","nam",this);
    relType->setSort("wire_type.nam");

    relPodt = new DbSqlRelation("wire_podt","id","str",this);
    relPodt->setSort("wire_podt.dat desc, wire_podt.str desc");
    relPodt->model()->setLimit(4000);

    relRab = new DbSqlRelation("wire_empl","id","snam",this);
    relRab->setSort("wire_empl.snam");

    relVol = new DbSqlRelation("wire_vol","id","rab",this);
    relVol->setSort("wire_vol.rab");

    relPodtOp = new DbSqlRelation("wire_podt_op","id","nam",this);
    relPodtOp->setSort("wire_podt_op.id");

    relRasxVid = new DbSqlRelation("rasx_vid","id","nam",this);
    relRasxVid->setSort("rasx_vid.id");

    relSource = new DbSqlRelation("wire_source","id","nam",this);
    relSource->setSort("wire_source.nam");

    relWirePart = new DbSqlRelation("wire_parti","id","str",this);
    relWirePart->setSort("substring(wire_parti.str,  position('-2' in wire_parti.str)+1, 4) desc, wire_parti.str desc");
    relWirePart->model()->setLimit(4000);

    relWirePartNew = new DbSqlRelation("wire_parti","id","str",this);
    relWirePartNew->setAlias("wire_parti_new");
    relWirePartNew->setSort("substring(wire_parti_new.str,  position('-2' in wire_parti_new.str)+1, 4) desc, wire_parti_new.str desc");
    relWirePartNew->model()->setLimit(4000);

    relMech = new DbSqlRelation("mech_tbl","id","nam",this);
    relMech->setSort("mech_tbl.nam");

    relMechCat = new DbSqlRelation("mech_category","id","nam",this);
    relMechCat->setSort("mech_category.nam");

    relAddType = new DbSqlRelation("wire_in_cex_type","id","short",this);
    relAddType->setSort("wire_in_cex_type.id");
    relAddType->setFilter("wire_in_cex_type.koef=1");

    relCause = new DbSqlRelation("wire_in_cex_type","id","short",this);
    relCause->setSort("wire_in_cex_type.id");
    relCause->setFilter("wire_in_cex_type.koef=-1");

    relNaklType = new DbSqlRelation("wire_in_cex_type","id","short",this);
    relNaklType->setSort("wire_in_cex_type.sort_order");
    relNaklType->model()->setAsync(false);

    relSertType = new DbSqlRelation("sertifikat_type","id","nam",this);
    relSertType->setSort("sertifikat_type.id");

    modelNam = new ModelRo(this);
    invoiceManager = new InvoiceManager(this);

    refreshNam();
}

