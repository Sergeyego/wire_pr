#include "formreport.h"
#include "ui_formreport.h"

FormReport::FormReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormReport)
{
    ui->setupUi(this);
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    //loadSettings();

    modelReport = new ModelReport(this);

    ui->tableView->setModel(modelReport);

    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(updReport()));
    //connect(ui->radioButtonMar,SIGNAL(clicked(bool)),this,SLOT(updReport()));
    //connect(ui->radioButtonPart,SIGNAL(clicked(bool)),this,SLOT(updReport()));
    connect(ui->cmdSave,SIGNAL(clicked(bool)),this,SLOT(save()));
}

FormReport::~FormReport()
{
    //saveSettings();
    delete ui;
}

void FormReport::loadSettings()
{
    QSettings settings("szsm", "wire_pr");
    this->restoreGeometry(settings.value("report_geometry").toByteArray());
}

void FormReport::saveSettings()
{
    QSettings settings("szsm", "wire_pr");
    settings.setValue("report_geometry", this->saveGeometry());
}

void FormReport::updReport()
{
    modelReport->refresh(ui->dateEditBeg->date().addDays(-1),ui->dateEditEnd->date(),ui->radioButtonPart->isChecked());
    ui->tableView->resizeToContents();
}

void FormReport::save()
{
    ui->tableView->save(tr("Отчет СЗСМ с ")+ui->dateEditBeg->date().toString("dd.MM.yy")+tr(" по ")+ui->dateEditEnd->date().toString("dd.MM.yy"));
}

ModelReport::ModelReport(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void ModelReport::refresh(QDate begDate, QDate endDate, bool bypart)
{
    this->clear();
    by_Part=bypart;
    if (by_Part){
        setQuery("select r.n_s, r.year, pr.nam, d.sdim, k.short, r.beg, r.post, r.postp, r.postb, r.poste, r.ver, r.brak, r.isp, r.obr, r.rekl, r.elrtr, r.other, r.perep, r.otgr, r.ost "
                 "from wire_calc_cex_report('"+ begDate.toString("yyyy.MM.dd")+"','"+endDate.toString("yyyy.MM.dd")+"') as r "
                 "inner join provol as pr on r.id_provol=pr.id "
                 "inner join diam as d on r.id_diam=d.id "
                 "inner join wire_pack_kind as k on r.id_pack=k.id "
                 "order by pr.nam, d.sdim, k.short, r.n_s, r.year");
        if (lastError().isValid()){
            QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
        } else {
            setHeaderData(0, Qt::Horizontal,tr("Партия"));
            setHeaderData(1, Qt::Horizontal,tr("Год"));
            setHeaderData(2, Qt::Horizontal,tr("Марка проволоки"));
            setHeaderData(3, Qt::Horizontal,tr("Ф"));
            setHeaderData(4, Qt::Horizontal,tr("Катушка"));
            setHeaderData(5, Qt::Horizontal,tr("Нал. на нач."));
            setHeaderData(6, Qt::Horizontal,tr("Пост. c пр-ва"));
            setHeaderData(7, Qt::Horizontal,tr("Пост. полуф."));
            setHeaderData(8, Qt::Horizontal,tr("Пост. брак"));
            setHeaderData(9, Qt::Horizontal,tr("Пост. электр."));
            setHeaderData(10, Qt::Horizontal,tr("Воз. со скл."));
            setHeaderData(11, Qt::Horizontal,tr("Брак"));
            setHeaderData(12, Qt::Horizontal,tr("Испыт."));
            setHeaderData(13, Qt::Horizontal,tr("Образцы"));
            setHeaderData(14, Qt::Horizontal,tr("Реклама"));
            setHeaderData(15, Qt::Horizontal,tr("Для эл. цеха"));
            setHeaderData(16, Qt::Horizontal,tr("Другое"));
            setHeaderData(17, Qt::Horizontal,tr("Переп.(+/-)"));
            setHeaderData(18, Qt::Horizontal,tr("Пер. на скл."));
            setHeaderData(19, Qt::Horizontal,tr("Ост. на конец"));
        }
    } else {
        setQuery("select pr.nam, d.sdim, k.short, sum(r.beg), sum(r.post), sum(r.postp), sum(r.postb), sum(r.poste), sum(r.ver), sum(r.brak), sum(r.isp), sum(r.obr), sum(r.rekl), sum(r.elrtr), sum(r.other), sum(r.perep), sum(r.otgr), sum(r.ost) "
                 "from wire_calc_cex_report('"+ begDate.toString("yyyy.MM.dd")+"','"+endDate.toString("yyyy.MM.dd")+"') as r "
                 "inner join provol as pr on r.id_provol=pr.id "
                 "inner join diam as d on r.id_diam=d.id "
                 "inner join wire_pack_kind as k on r.id_pack=k.id "
                 "group by pr.nam, d.sdim, k,short "
                 "order by pr.nam, d.sdim, k.short");
        if (lastError().isValid()){
            QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
        } else {
            setHeaderData(0, Qt::Horizontal,tr("Марка проволоки"));
            setHeaderData(1, Qt::Horizontal,tr("Ф"));
            setHeaderData(2, Qt::Horizontal,tr("Катушка"));
            setHeaderData(3, Qt::Horizontal,tr("Нал. на нач."));
            setHeaderData(4, Qt::Horizontal,tr("Пост. c пр-ва"));
            setHeaderData(5, Qt::Horizontal,tr("Пост. полуф."));
            setHeaderData(6, Qt::Horizontal,tr("Пост. брак"));
            setHeaderData(7, Qt::Horizontal,tr("Пост. электр."));
            setHeaderData(8, Qt::Horizontal,tr("Воз. со скл."));
            setHeaderData(9, Qt::Horizontal,tr("Брак"));
            setHeaderData(10, Qt::Horizontal,tr("Испыт."));
            setHeaderData(11, Qt::Horizontal,tr("Образцы"));
            setHeaderData(12, Qt::Horizontal,tr("Реклама"));
            setHeaderData(13, Qt::Horizontal,tr("Для эл. цеха"));
            setHeaderData(14, Qt::Horizontal,tr("Другое"));
            setHeaderData(15, Qt::Horizontal,tr("Переп.(+/-)"));
            setHeaderData(16, Qt::Horizontal,tr("Пер. на скл."));
            setHeaderData(17, Qt::Horizontal,tr("Ост. на конец"));
        }
    }
}

QVariant ModelReport::data(const QModelIndex &index, int role) const
{

    if (index.row()>=rowCount()|| index.row()<0 || index.column()>=columnCount() || index.column()<0) return QVariant();

    QVariant value;

    if ((index.row()==QSqlQueryModel::rowCount()) && (role==Qt::DisplayRole || role==Qt::EditRole)){
        if (index.column()==0) {
            value= tr("Итого");
        } else if ((index.column()>= 5 && by_Part)||(index.column() >= 3 && !by_Part)) {
            double sum=0.0;
            for (int i=0; i<QSqlQueryModel::rowCount(); i++){
                QModelIndex cs=this->index(i,index.column());
                sum+=(QSqlQueryModel::data(cs).toDouble());
            }
            value=sum;
        } else value=QVariant();

    } else {
        value = (index.row()==QSqlQueryModel::rowCount()) ? QVariant() : QSqlQueryModel::data(index, role);
    }
    switch (role) {
        case Qt::DisplayRole:
        {
            if((index.column() >=5 && by_Part)||(index.column() >= 3 && !by_Part))
                if (value!=0.0){
                    return QLocale().toString(value.toDouble(),'f',2);
                } else {
                    return QVariant();
                }
            else
                return value;
        }

        case Qt::EditRole:
        {
            return value;
        }

        case Qt::TextAlignmentRole:
        {
             if((index.column()>=5 && by_Part)||(index.column() >= 3 && !by_Part))
                 return int(Qt::AlignRight | Qt::AlignVCenter);
             else return int(Qt::AlignLeft | Qt::AlignVCenter);
        }

        case Qt::BackgroundRole:
        {
            double val1= by_Part ? data(this->index(index.row(),5),Qt::EditRole).toDouble() :  data(this->index(index.row(),3),Qt::EditRole).toDouble();
            double val2= by_Part ? data(this->index(index.row(),19),Qt::EditRole).toDouble() :  data(this->index(index.row(),17),Qt::EditRole).toDouble();
            return (val1>=0 && val2>=0) ? value : QVariant(QColor(255,170,170));
        }

    }
    return value;
}

int ModelReport::rowCount(const QModelIndex &parent) const
{
    return (QSqlQueryModel::rowCount(parent)==0)? 0 : QSqlQueryModel::rowCount(parent)+1;
}
