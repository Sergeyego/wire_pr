#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString key, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tabWidget = new TabWidget(this);
    this->setCentralWidget(tabWidget);

    formPart = new FormPart(key==QString("srt"));
    formWire = new FormWire();
    formRoute = new FormRoute();
    formShip = new FormShip();
    formWarehouse = new FormWarehouse();
    formEdtWire = new FormEdtWire();
    checkForm = new CheckForm();
    formPerePack = new FormPerePack();
    formPresence = new FormPresence();
    formReport = new FormReport();
    formPodt = new FormPodt();
    formAnn = new FormAnn();
    formPack = new FormEdtPack();
    formLine = new FormEdtLine();
    formLbl = new FormLbl();
    formNakl = new FormNakl();

    QStringList axesStock;
    axesStock.push_back(tr("Источник"));
    axesStock.push_back(tr("Тип поступления"));
    axesStock.push_back(tr("Марка"));
    axesStock.push_back(tr("Диаметр"));
    axesStock.push_back(tr("Катушка"));
    axesStock.push_back(tr("Партия"));
    axesStock.push_back(tr("Год"));
    axesStock.push_back(tr("Месяц"));
    axesStock.push_back(tr("День"));
    QString queryStock="select s.nam, t.nam, pr.nam, d.sdim, k.short, m.n_s||'-'||date_part('year',m.dat), "
                       "substr(cast(w.dat as char(32)),1,4) as yr, "
                       "substr(cast(w.dat as char(32)),1,7) as mn, "
                       "substr(cast(w.dat as char(32)),1,10) as dy, w.m_netto "
                       "from wire_in_cex_data w "
                       "inner join wire_in_cex_type t on w.id_type=t.id and t.koef=1 "
                       "inner join wire_parti p on w.id_wparti=p.id "
                       "inner join wire_parti_m m on p.id_m=m.id "
                       "inner join provol pr on pr.id=m.id_provol "
                       "inner join diam d on d.id=m.id_diam "
                       "inner join wire_pack_kind k on p.id_pack=k.id "
                       "inner join wire_source s on m.id_source=s.id "
                       "where w.dat between :d1 and :d2";
    addCube = new CubeWidget(tr("Поступление готовой проволоки"),axesStock,queryStock,2);

    QStringList axesNam;
    axesNam.push_back(tr("Марка"));
    axesNam.push_back(tr("Диаметр"));
    axesNam.push_back(tr("Катушка"));
    axesNam.push_back(tr("Партия"));
    axesNam.push_back(tr("Линия"));
    axesNam.push_back(tr("Работник"));
    axesNam.push_back(tr("Год"));
    axesNam.push_back(tr("Месяц"));
    axesNam.push_back(tr("День"));
    QString queryNam="select pr.nam, d.sdim, k.short, m.n_s||'-'||date_part('year',m.dat), l.snam, "
                     "e.first_name||' '||substr(e.last_name,1,1)||'. '||substr(e.middle_name,1,1)||'.', "
                     "substr(cast(n.dat as char(32)),1,4) as yr, "
                     "substr(cast(n.dat as char(32)),1,7) as mn, "
                     "substr(cast(n.dat as char(32)),1,10) as dy, n.m_netto "
                     "from wire_in_cex_nam n "
                     "inner join wire_parti p on n.id_wparti=p.id "
                     "inner join wire_parti_m m on p.id_m=m.id "
                     "inner join provol pr on pr.id=m.id_provol "
                     "inner join diam d on d.id=m.id_diam "
                     "inner join wire_pack_kind k on p.id_pack=k.id "
                     "inner join wire_line l on n.id_line=l.id "
                     "inner join wire_empl e on n.id_empl=e.id "
                     "where n.dat between :d1 and :d2";
    namCube = new CubeWidget(tr("Намотка проволоки"),axesNam,queryNam,2);

    QStringList axesSrc;
    axesSrc.push_back(tr("Марка"));
    axesSrc.push_back(tr("Диаметр"));
    axesSrc.push_back(tr("Плавка"));
    axesSrc.push_back(tr("Партия"));
    axesSrc.push_back(tr("Поставщик"));
    axesSrc.push_back(tr("Стан"));
    axesSrc.push_back(tr("Гот.партия"));
    axesSrc.push_back(tr("Гот.марка"));
    axesSrc.push_back(tr("Гот.диам."));
    axesSrc.push_back(tr("Гот.намот."));
    axesSrc.push_back(tr("Год"));
    axesSrc.push_back(tr("Месяц"));
    axesSrc.push_back(tr("День"));
    QString querySrc="select pr.nam, d.sdim, b.n_plav, b.parti, s.short, l.snam, "
                     "m.n_s||'-'||date_part('year',m.dat), prg.nam, dg.sdim, k.short, "
                     "substr(cast(i.dat as char(32)),1,4) as yr, "
                     "substr(cast(i.dat as char(32)),1,7) as mn, "
                     "substr(cast(i.dat as char(32)),1,10) as dy, "
                     "i.m_netto*(select val from wire_kr where id_prov=pp.id_pr and dat=(select max(dat) from wire_kr where id_prov=pp.id_pr and dat<=i.dat))/1000.0 as rac "
                     "from wire_in_cex_data as i "
                     "inner join wire_parti as p on p.id=i.id_wparti "
                     "inner join wire_parti_m as m on p.id_m=m.id "
                     "inner join wire_line as l on m.id_type=l.id "
                     "inner join prov_buht as b on m.id_buht=b.id "
                     "inner join prov_prih as pp on b.id_prih=pp.id "
                     "inner join provol as pr on pp.id_pr=pr.id "
                     "inner join diam as d on pp.id_dim=d.id "
                     "inner join suppl as s on pp.id_spl=s.id "
                     "inner join provol as prg on m.id_provol=prg.id "
                     "inner join diam as dg on m.id_diam=dg.id "
                     "inner join wire_pack_kind as k on p.id_pack=k.id "
                     "where (i.id_type=3 or i.id_type=13 or i.id_type=20 or i.id_type=23) and i.dat between :d1 and :d2";
    srcCube = new CubeWidget(tr("Расход сырья на производство проволоки, кг"),axesSrc,querySrc,3);

    QStringList axesPerepack;
    axesPerepack.push_back(tr("Накладная"));
    axesPerepack.push_back(tr("Исх. партия"));
    axesPerepack.push_back(tr("Исх. марка"));
    axesPerepack.push_back(tr("Исх. диам."));
    axesPerepack.push_back(tr("Исх. носитель"));
    axesPerepack.push_back(tr("Исх. упаковка"));
    axesPerepack.push_back(tr("Партия"));
    axesPerepack.push_back(tr("Марка"));
    axesPerepack.push_back(tr("Диам."));
    axesPerepack.push_back(tr("Носитель"));
    axesPerepack.push_back(tr("Упаковка"));
    axesPerepack.push_back(tr("Год"));
    axesPerepack.push_back(tr("Месяц"));
    axesPerepack.push_back(tr("День"));
    QString queryPerepack="select n.num ||'-'||date_part('year',n.dat), "
            "psm.n_s||'-'||date_part('year',psm.dat), ws.nam, ds.sdim, ss.short, sts.pack_ed, "
            "prm.n_s||'-'||date_part('year',prm.dat), wr.nam, dr.sdim, sr.short, str.pack_ed, "
            "substr(cast(n.dat as char(32)),1,4) as yr, "
            "substr(cast(n.dat as char(32)),1,7) as mn, "
            "substr(cast(n.dat as char(32)),1,10) as dy, p.m_netto "
            "from wire_perepack_nakl as n "
            "inner join wire_perepack as p on n.id=p.id_nakl "
            "inner join wire_parti as ps on p.id_wpartisrc=ps.id "
            "inner join wire_parti_m as psm on ps.id_m=psm.id "
            "inner join provol as ws on psm.id_provol=ws.id "
            "inner join diam as ds on psm.id_diam=ds.id "
            "inner join wire_pack_kind as ss on ps.id_pack=ss.id "
            "inner join wire_pack as sts on ps.id_pack_type=sts.id "
            "inner join wire_parti as pr on p.id_wpartires=pr.id "
            "inner join wire_parti_m as prm on pr.id_m=prm.id "
            "inner join provol as wr on prm.id_provol=wr.id "
            "inner join diam as dr on prm.id_diam=dr.id "
            "inner join wire_pack_kind as sr on pr.id_pack=sr.id "
            "inner join wire_pack as str on pr.id_pack_type=str.id "
            "where n.dat between :d1 and :d2";
    perepackCube = new CubeWidget(tr("Переупаковка, кг"),axesPerepack,queryPerepack,2);

    QStringList axesPodt;
    axesPodt.push_back(tr("Марка"));
    axesPodt.push_back(tr("Диаметр исх."));
    axesPodt.push_back(tr("Диаметр кон."));
    axesPodt.push_back(tr("Плавка"));
    axesPodt.push_back(tr("Партия подт."));
    axesPodt.push_back(tr("Стан"));
    axesPodt.push_back(tr("Волочильщик"));
    axesPodt.push_back(tr("Исходная протяжка"));
    axesPodt.push_back(tr("Год"));
    axesPodt.push_back(tr("Месяц"));
    axesPodt.push_back(tr("День"));
    QString queryPodt="select pr.nam, d.sdim, dp.sdim, b.n_plav, p.n_s||'-'||date_part('year',p.dat), l.snam, "
            "e.first_name||' '||substr(e.last_name,1,1)||'. '||substr(e.middle_name,1,1)||'.', q.str, "
            "substr(cast(pc.dat as char(32)),1,4) as yr, "
            "substr(cast(pc.dat as char(32)),1,7) as mn, "
            "substr(cast(pc.dat as char(32)),1,10) as dy, pc.kvo "
            "from wire_podt_cont as pc "
            "inner join wire_podt as p on pc.id_podt=p.id "
            "inner join prov_buht as b on p.id_buht=b.id "
            "inner join prov_prih as pp on b.id_prih=pp.id "
            "inner join provol as pr on pp.id_pr=pr.id "
            "inner join diam as d on pp.id_dim=d.id "
            "inner join diam as dp on p.id_diam=dp.id "
            "inner join wire_line as l on p.id_line=l.id "
            "inner join wire_empl e on pc.id_rab=e.id "
            "left join ( "
            "select pd.id as id, pd.n_s || '-'|| date_part('year',pd.dat) ||' '||r.nam ||' '||d.sdim || ' ('||  h.n_plav || ')' as str "
            "from wire_podt as pd "
            "inner join prov_buht as h on pd.id_buht=h.id "
            "inner join prov_prih as p on p.id=h.id_prih "
            "inner join provol as r on r.id=p.id_pr "
            "inner join diam as d on pd.id_diam=d.id "
            ") as q on q.id=pc.id_podt_src "
            "where pc.dat between :d1 and :d2";
    podtCube = new CubeWidget(tr("Анализ подтяжек"),axesPodt,queryPodt,2);

    loadSettings();

    tabWidget->addTabAction(formWire,ui->actionProv);
    tabWidget->addTabAction(formPart,ui->actionPart);
    tabWidget->addTabAction(formRoute,ui->actionRoute);
    tabWidget->addTabAction(formShip,ui->actionShip);
    tabWidget->addTabAction(formWarehouse,ui->actionWarehouse);
    tabWidget->addTabAction(formEdtWire,ui->actionEdtWire);
    tabWidget->addTabAction(checkForm,ui->actionCod);
    tabWidget->addTabAction(formPerePack,ui->actionPerepack);
    tabWidget->addTabAction(formPresence,ui->actionPrecense);
    tabWidget->addTabAction(formReport,ui->actionPeport);
    tabWidget->addTabAction(formPodt,ui->actionPodt);
    tabWidget->addTabAction(addCube,ui->actionAdd);
    tabWidget->addTabAction(namCube,ui->actionAnSpool);
    tabWidget->addTabAction(srcCube,ui->actionSrc);
    tabWidget->addTabAction(formAnn,ui->actionAnn);
    tabWidget->addTabAction(formPack,ui->actionSpool);
    tabWidget->addTabAction(formLine,ui->actionLine);
    tabWidget->addTabAction(formLbl,ui->actionLbl);
    tabWidget->addTabAction(perepackCube,ui->actionPerepackCube);
    tabWidget->addTabAction(formNakl,ui->actionNakl);
    tabWidget->addTabAction(podtCube,ui->actionCubePodt);
    tabWidget->loadSettings();

    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->actionRefresh,SIGNAL(triggered(bool)),Models::instance(),SLOT(refresh()));
}

MainWindow::~MainWindow()
{
    tabWidget->saveSettings();
    saveSettings();
    /delete formWarehouse;
    delete formShip;
    delete formRoute;
    delete formWire;
    delete formPart;
    delete formEdtWire;
    delete checkForm;
    delete formPerePack;
    delete formPresence;
    delete formReport;
    delete addCube;
    delete formPodt;
    delete namCube;
    delete srcCube;
    delete formAnn;
    delete formPack;
    delete formLine;
    delete formLbl;
    delete perepackCube;
    delete formNakl;
    delete Models::instance();
    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    this->restoreGeometry(settings.value("main_geometry").toByteArray());
    this->restoreState(settings.value("main_state").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    settings.setValue("main_state", this->saveState());
    settings.setValue("main_geometry", this->saveGeometry());
}
