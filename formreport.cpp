#include "formreport.h"
#include "ui_formreport.h"

FormReport::FormReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormReport)
{
    ui->setupUi(this);

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    executor = new ProgressExecutor(this);

    model = new ModelReport(this);

    ui->tableView->setModel(model);

    connect(executor,SIGNAL(finished()), this, SLOT(updFinished()));
    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this,SLOT(save()));
}

FormReport::~FormReport()
{
    delete ui;
}

void FormReport::upd()
{
    QString query;
    if (ui->radioButtonPart->isChecked()){
        query=QString("select pr.nam, d.sdim, k.short, r.n_s ||'-'|| r.year, ev.nam, p.pack_ed, "
                        "r.beg, r.post, r.postp, r.postb, r.poste, r.ver, r.brak, r.isp, r.obr, r.rekl, r.elrtr, r.other, r.perep, r.otgr, r.ost "
                        "from wire_calc_cex_report('%1','%2') as r "
                        "inner join provol as pr on r.id_provol=pr.id "
                        "inner join diam as d on r.id_diam=d.id "
                        "inner join wire_pack_kind as k on r.id_pack=k.id "
                        "inner join wire_parti as wp on wp.id = r.id_wparti "
                        "inner join elrtr_vars as ev on ev.id = wp.id_var "
                        "inner join wire_pack as p on p.id = wp.id_pack_type "
                        "order by pr.nam, d.sdim, k.short, r.n_s, r.year").arg(ui->dateEditBeg->date().addDays(-1).toString("yyyy-MM-dd")).arg(ui->dateEditEnd->date().toString("yyyy-MM-dd"));
    } else {
        query=QString("select pr.nam, d.sdim, k.short, NULL, ev.nam, p.pack_ed, "
                        "sum(r.beg), sum(r.post), sum(r.postp), sum(r.postb), sum(r.poste), sum(r.ver), sum(r.brak), sum(r.isp), sum(r.obr), "
                        "sum(r.rekl), sum(r.elrtr), sum(r.other), sum(r.perep), sum(r.otgr), sum(r.ost) "
                        "from wire_calc_cex_report('%1','%2') as r "
                        "inner join provol as pr on r.id_provol=pr.id "
                        "inner join diam as d on r.id_diam=d.id "
                        "inner join wire_pack_kind as k on r.id_pack=k.id "
                        "inner join wire_parti as wp on wp.id = r.id_wparti "
                        "inner join elrtr_vars as ev on ev.id = wp.id_var "
                        "inner join wire_pack as p on p.id = wp.id_pack_type "
                        "group by pr.nam, d.sdim, k.short, ev.nam, p.pack_ed "
                        "order by pr.nam, d.sdim, k.short, ev.nam, p.pack_ed").arg(ui->dateEditBeg->date().addDays(-1).toString("yyyy-MM-dd")).arg(ui->dateEditEnd->date().toString("yyyy-MM-dd"));
    }

    executor->setQuery(query);
    executor->start();
}

void FormReport::updFinished()
{
    QVector<QVector<QVariant>> data=executor->getData();
    QStringList titles=executor->getTitles();
    QVector<QVariant> sums;
    int colCount= titles.size();
    sums.resize(colCount);
    if (colCount>20){
        sums[0]=tr("ИТОГО");
    }
    for (QVector<QVariant> dt : data){
        for (int i=colCount-15; i<colCount; i++){
            sums[i]=sums[i].toDouble()+dt[i].toDouble();
        }
    }
    if (data.size()>1){
        data.push_back(sums);
    }

    model->setModelData(data);
    ui->tableView->resizeToContents();
}

void FormReport::save()
{
    QString tit=tr("Отчет цеха проволоки с ")+ui->dateEditBeg->date().toString("dd.MM.yyyy")+tr(" по ")+ui->dateEditEnd->date().toString("dd.MM.yyyy");
    ui->tableView->save(tit,1,true,Qt::LandscapeOrientation);
}

ModelReport::ModelReport(QWidget *parent) : TableModel(parent)
{
    QStringList titles;
    titles<<tr("Марка")<<tr("Диам.")<<tr("Носитель")<<tr("Партия")<<tr("Вариант")<<tr("Упаковка")<<tr("Налич.на нач.")<<tr("Пост. c пр-ва")<<tr("Пост. полуф.")<<tr("Пост. брак")<<tr("Пост. электр.");
    titles<<tr("Воз. со скл.")<<tr("Брак")<<tr("Испыт.")<<tr("Образцы")<<tr("Реклама")<<tr("Для эл. цеха")<<tr("Другое")<<tr("Переп.(+/-)")<<tr("Пер. на скл.")<<tr("Ост. на конец");
    setHeader(titles);
}

QVariant ModelReport::data(const QModelIndex &index, int role) const
{
    if((role == Qt::BackgroundRole)) {
        double beg = this->data(this->index(index.row(),6),Qt::EditRole).toDouble();
        double end = this->data(this->index(index.row(),20),Qt::EditRole).toDouble();
        if (beg<0 || end<0) {
            return QVariant(QColor(255,170,170));
        } else {
            return TableModel::data(index,role);
        }
    } else return TableModel::data(index,role);
}
