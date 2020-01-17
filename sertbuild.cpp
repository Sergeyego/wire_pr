#include "sertbuild.h"

SertBuild::SertBuild(QObject *parent) :
    QTextDocument(parent)
{
    data=new DataSert(this);
    prn=false;
}

QString SertBuild::getNomPart()
{
    return data->nomPart;
}

QString SertBuild::getYearSert()
{
    return QString::number(data->dateVidSert.year());
}

QString SertBuild::getYearPart()
{
    return QString::number(data->yearPart);
}

QString SertBuild::getNomSert()
{
    return data->nomSert;
}

bool SertBuild::getPrn()
{
    return prn;
}

void SertBuild::build(int id, bool is_ship)
{
    data->refresh(id, is_ship);
    current_id=id;
    current_is_ship=is_ship;

    this->clear();

    QTextBlockFormat formatRirht;
    formatRirht.setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    QTextBlockFormat formatLeft;
    formatLeft.setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    QTextBlockFormat formatCenter;
    formatCenter.setAlignment(Qt::AlignCenter|Qt::AlignVCenter);

    QTextCharFormat textAdrFormat;
    QFont serifFont("Cursive",10,QFont::Normal);
    textAdrFormat.setFont(serifFont);

    QTextCharFormat textTitleFormat;
    QFont titleFont("Droid Serif",13,QFont::Normal);
    textTitleFormat.setFont(titleFont);

    QTextCharFormat textNormalFormat;
    QFont normalFont("Droid Sans",10,QFont::Normal);
    textNormalFormat.setFont(normalFont);

    QTextCharFormat textNormalSubFormat(textNormalFormat);
    textNormalSubFormat.setVerticalAlignment(QTextCharFormat::AlignSubScript);

    QTextCharFormat textBoldFormat;
    QFont boldFont("Droid Sans",10,QFont::Bold);
    textBoldFormat.setFont(boldFont);

    QTextCharFormat textTableFormat;
    QFont tableFont("Droid Sans",8,QFont::Normal);
    textTableFormat.setFont(tableFont);

    QTextCursor cursor(this);

    cursor.movePosition(QTextCursor::Start);
    cursor.setBlockFormat(formatRirht);
    cursor.insertText(tr("Форма 3.1 по EN 10204"),textNormalFormat);
    cursor.insertBlock(formatCenter);
    addResource(QTextDocument::ImageResource, QUrl("logo"), data->logo);
    QTextImageFormat f;
    f.setName("logo");
    f.setHeight(70);
    cursor.insertImage(f);
    cursor.insertBlock(formatRirht);
    cursor.insertText(data->adres,textAdrFormat);
    cursor.insertBlock(formatRirht);
    cursor.insertText(data->contact,textAdrFormat);
    cursor.insertBlock(formatLeft);

    QVector<int> id_ved;
    for (int i=0; i<data->sertModel->rowCount(); i++){
        int ved=data->sertModel->data(data->sertModel->index(i,0)).toInt();
        if (!id_ved.contains(ved)){
            id_ved.push_back(ved);
        }
    }

    for (int i=0; i<id_ved.size(); i++){
        QImage img;
        img.loadFromData(Models::instance()->relVedPix->data(QString::number(id_ved.at(i))).toByteArray());
        if (!img.isNull()) {
            addResource(QTextDocument::ImageResource, QUrl("vedimage"+QString::number(id_ved.at(i))), img);
            QTextImageFormat f;
            f.setName("vedimage"+QString::number(id_ved.at(i)));
            f.setHeight(45);
            cursor.insertImage(f);
        }
    }

    cursor.insertBlock(formatCenter);
    cursor.insertText(tr("СЕРТИФИКАТ КАЧЕСТВА №")+data->nomPart+"-"+QString::number(data->yearPart),textTitleFormat);
    if (is_ship){
        cursor.insertText("/"+data->nomSert,textTitleFormat);
    }
    cursor.insertBlock(formatLeft);
    cursor.insertText(tr("Нормативная документация: "),textBoldFormat);
    for (int i=0; i<data->tuList.size(); i++){
        cursor.insertText(data->tuList.at(i),textNormalFormat);
        if (i!=data->tuList.size()-1)
            cursor.insertText(tr(", "),textNormalFormat);
    }
    cursor.insertBlock();

    QTextTableFormat tableFormat;
    tableFormat.setAlignment(Qt::AlignHCenter);
    QBrush brush = tableFormat.borderBrush();
    brush.setColor(QColor(Qt::black));
    tableFormat.setBorderBrush(brush);
    tableFormat.setCellPadding(3);
    tableFormat.setCellSpacing(1);

    QTextTableFormat tableMechFormat(tableFormat);
    QVector<QTextLength> v;

    v.push_back(QTextLength(QTextLength::PercentageLength,80));
    v.push_back(QTextLength(QTextLength::VariableLength,10));
    v.push_back(QTextLength(QTextLength::VariableLength,10));
    tableMechFormat.setColumnWidthConstraints(v);

    QTextTable *mainTable = cursor.insertTable(1, 7, tableFormat);

    cursor=mainTable->cellAt(0,0).firstCursorPosition();  
    cursor.setBlockFormat(formatCenter);
    QString nam=data->is_cored? tr("Наименование продукции") : tr("Марка проволоки");
    cursor.insertText(nam,textBoldFormat);

    cursor=mainTable->cellAt(0,1).firstCursorPosition();  
    cursor.setBlockFormat(formatCenter);
    nam=data->is_cored? tr("Марка") : tr("Условное обозначение проволоки");
    cursor.insertText(nam,textBoldFormat);

    cursor=mainTable->cellAt(0,2).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    nam=data->is_cored? tr("Диаметр, мм") : tr("Тип носителя проволоки");
    cursor.insertText(nam,textBoldFormat);

    cursor=mainTable->cellAt(0,3).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    nam=data->is_cored? tr("Тип носителя проволоки") : tr("Номер плавки");
    cursor.insertText(nam,textBoldFormat);

    cursor=mainTable->cellAt(0,4).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(tr("Номер партии"),textBoldFormat);

    cursor=mainTable->cellAt(0,5).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(tr("Дата производства"),textBoldFormat);

    cursor=mainTable->cellAt(0,6).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(tr("Масса проволоки нетто, кг"),textBoldFormat);

    mainTable->insertRows(mainTable->rows(),1);
    cursor=mainTable->cellAt(1,0).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    QString dat=data->is_cored? tr("Порошковая проволока") : data->srcProv;
    cursor.insertText(dat,textNormalFormat);
    cursor=mainTable->cellAt(1,1).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    dat=data->is_cored? data->prov : data->diam+" "+data->prov;
    cursor.insertText(dat,textNormalFormat);
    cursor=mainTable->cellAt(1,2).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    dat=data->is_cored? data->diam : data->spul;
    cursor.insertText(dat,textNormalFormat);
    cursor=mainTable->cellAt(1,3).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    dat=data->is_cored? data->spul : data->nPlav;
    cursor.insertText(dat,textNormalFormat);
    cursor=mainTable->cellAt(1,4).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(data->nomPart,textNormalFormat);
    cursor=mainTable->cellAt(1,5).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(data->datePart.toString("dd.MM.yy"),textNormalFormat);
    cursor=mainTable->cellAt(1,6).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(QString::number(data->netto),textNormalFormat);
    cursor.movePosition(QTextCursor::End);
    cursor.setBlockFormat(formatCenter);
    cursor.insertBlock();
    cursor.insertBlock();
    double value=0;
    QString head;
    int row=data->chemModel->rowCount();
    if (row) {
        QTextTable *chemTable = cursor.insertTable(3, row, tableFormat);
        cursor=chemTable->cellAt(0,0).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        QString txt= data->is_cored? tr("Химический состав наплавленного металла, % *") : tr("Химический состав проволоки, %");
        cursor.insertText(txt,textTitleFormat);
        chemTable->mergeCells(0,0,1,row);
        for (int i=0; i<row; i++)
        {
            head=data->chemModel->data(data->chemModel->index(i,0)).toString();
            value=data->chemModel->data(data->chemModel->index(i,1)).toDouble();
            if (value!=0){
                cursor=chemTable->cellAt(2,i).firstCursorPosition();
                cursor.setBlockFormat(formatCenter);
                cursor.insertText(" "+QLocale().toString(value,'f',3)+" ",textTableFormat);
                cursor=chemTable->cellAt(1,i).firstCursorPosition();
                cursor.setBlockFormat(formatCenter);
                cursor.insertText(head,textTableFormat);
            }
        }
    }

    cursor.movePosition(QTextCursor::End);
    cursor.setBlockFormat(formatCenter);
    cursor.insertBlock();
    cursor.insertBlock();

    QVector<int> meh;
    for (int j=0; j<data->mechModel->rowCount(); j++){
        bool ex=false;
        int cnt=0;
        int mCat=data->mechModel->data(data->mechModel->index(j,0)).toInt();
        while (!ex && cnt<meh.size()){
            ex=(mCat==meh.at(cnt));
            cnt++;
        }
        if (!ex) meh.push_back(mCat);
    }
    QTextTable *mechTable0 = cursor.insertTable(data->mechModel->rowCount()+meh.size()+1,3,tableMechFormat);
    QString sig, prefix;
    int pos=0;
    for (int n=0; n<meh.size(); n++){
            mechTable0->cellAt(pos,0).firstCursorPosition().setBlockFormat(formatCenter);
            QString nameCat=data->mechCategory->data(data->mechCategory->index(meh.at(n),1)).toString();
            if (data->is_cored) nameCat+=" *";
            mechTable0->cellAt(pos,0).firstCursorPosition().insertText(nameCat,textTitleFormat);
            mechTable0->mergeCells(pos,0,1,3);
            pos++;
            for (int i=0; i<data->mechModel->rowCount(); i++){
                if (data->mechModel->data(data->mechModel->index(i,0)).toInt()==meh.at(n)){
                    head=data->mechModel->data(data->mechModel->index(i,1)).toString();
                    sig=data->mechModel->data(data->mechModel->index(i,2)).toString();
                    prefix=data->mechModel->data(data->mechModel->index(i,3)).toString();
                    value=data->mechModel->data(data->mechModel->index(i,4)).toDouble();
                    QVariant value2=data->mechModel->data(data->mechModel->index(i,5));
                    QString valStr=value2.isNull() ?
                                (prefix+" "+QLocale().toString(value,'f',2)) : (QLocale().toString(value,'f',2)+" - "+QLocale().toString(value2.toDouble(),'f',2));
                    mechTable0->cellAt(pos,0).firstCursorPosition().setBlockFormat(formatLeft);
                    mechTable0->cellAt(pos,0).firstCursorPosition().setCharFormat(textNormalFormat);

                    QTextDocument hd;
                    hd.setHtml(head);
                    QTextCursor chd(&hd);
                    chd.select(QTextCursor::Document);
                    chd.mergeCharFormat(textNormalFormat);
                    mechTable0->cellAt(pos,0).firstCursorPosition().insertFragment(chd.selection());

                    mechTable0->cellAt(pos,1).firstCursorPosition().setBlockFormat(formatCenter);
                    mechTable0->cellAt(pos,1).firstCursorPosition().setCharFormat(textNormalFormat);

                    QTextDocument sd;
                    sd.setHtml(sig);
                    QTextCursor csd(&sd);
                    csd.select(QTextCursor::Document);
                    csd.mergeCharFormat(textNormalFormat);
                    mechTable0->cellAt(pos,1).firstCursorPosition().insertFragment(csd.selection());

                    mechTable0->cellAt(pos,2).firstCursorPosition().setBlockFormat(formatRirht);
                    mechTable0->cellAt(pos,2).firstCursorPosition().insertText(valStr,textNormalFormat);
                    pos++;
                }
            }
    }
    mechTable0->cellAt(pos,0).firstCursorPosition().setBlockFormat(formatLeft);
    mechTable0->cellAt(pos,0).firstCursorPosition().insertText(tr("Состояние поверхности проволоки: поверхность проволоки чистая, "
                                                                  "гладкая, без трещин, расслоений, плен, закатов, раковин, забоин "
                                                                  "окалины, ржавчины, масла, технологической смазки и других загрязнений"),textNormalFormat);
    mechTable0->mergeCells(pos,0,1,3);

    cursor.movePosition(QTextCursor::End);
    if (data->is_cored){
        cursor.setBlockFormat(formatLeft);
        cursor.insertText(tr("* для смеси газов CO"),textNormalFormat);
        cursor.insertText("2",textNormalSubFormat);
        cursor.insertText(" 20%",textNormalFormat);
    }
    cursor.insertBlock();

    if (data->sertModel->rowCount()){
        cursor.setBlockFormat(formatCenter);
        cursor.insertText(tr("Аттестация и сертификация"),textTitleFormat);
        cursor.insertBlock();
        QTextTable *sertTable = cursor.insertTable(data->sertModel->rowCount()+1,4,tableFormat);
        sertTable->cellAt(0,0).firstCursorPosition().setBlockFormat(formatCenter);
        sertTable->cellAt(0,0).firstCursorPosition().insertText(tr("Вид документа"),textBoldFormat);
        sertTable->cellAt(0,1).firstCursorPosition().setBlockFormat(formatCenter);
        sertTable->cellAt(0,1).firstCursorPosition().insertText(tr("Сертифицирующий орган"),textBoldFormat);
        sertTable->cellAt(0,2).firstCursorPosition().setBlockFormat(formatCenter);
        sertTable->cellAt(0,2).firstCursorPosition().insertText(tr("Номер документа"),textBoldFormat);
        sertTable->cellAt(0,3).firstCursorPosition().setBlockFormat(formatCenter);
        sertTable->cellAt(0,3).firstCursorPosition().insertText(tr("Дата"),textBoldFormat);
        QString vid, organ, nom, dat;
        for(int i=0; i<data->sertModel->rowCount(); i++){
            vid=data->sertModel->data(data->sertModel->index(i,1)).toString();
            organ=data->sertModel->data(data->sertModel->index(i,2)).toString();
            nom=data->sertModel->data(data->sertModel->index(i,3)).toString();
            dat=data->sertModel->data(data->sertModel->index(i,4)).toDate().toString("dd.MM.yy");
            sertTable->cellAt(i+1,0).firstCursorPosition().setBlockFormat(formatCenter);
            sertTable->cellAt(i+1,0).firstCursorPosition().insertText(vid,textTableFormat);
            sertTable->cellAt(i+1,1).firstCursorPosition().setBlockFormat(formatCenter);
            sertTable->cellAt(i+1,1).firstCursorPosition().insertText(organ,textTableFormat);
            sertTable->cellAt(i+1,2).firstCursorPosition().setBlockFormat(formatCenter);
            sertTable->cellAt(i+1,2).firstCursorPosition().insertText(nom,textTableFormat);
            sertTable->cellAt(i+1,3).firstCursorPosition().setBlockFormat(formatCenter);
            sertTable->cellAt(i+1,3).firstCursorPosition().insertText(dat,textTableFormat);
        }
        cursor.movePosition(QTextCursor::End);
        cursor.insertBlock();
    }

    cursor.setBlockFormat(formatLeft);
    if (is_ship){
        cursor.insertText(tr("Грузополучатель: "),textBoldFormat);
        cursor.insertText(data->poluch,textNormalFormat);
        cursor.insertBlock();
    }
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(tr("При переписке по вопросам качества просьба ссылаться на номер партии"),textNormalFormat);
    cursor.insertBlock();
    cursor.setBlockFormat(formatCenter);
    QDate date;
    date=(is_ship)? data->dateVidSert : QDate::currentDate();
    cursor.insertText(tr("Дата ")+date.toString("dd.MM.yy"),textNormalFormat);
    cursor.insertBlock();

    cursor.setBlockFormat(formatLeft);
    addResource(QTextDocument::ImageResource, QUrl("qrcode"), data->qrCode);
    QTextImageFormat qrformat;
    qrformat.setName("qrcode");
    qrformat.setHeight(150);
    cursor.insertImage(qrformat);
    cursor.insertText("                                    ",textNormalFormat);
    if (prn) {
        addResource(QTextDocument::ImageResource, QUrl("sign"), data->sign);
        QTextImageFormat signformat;
        signformat.setName("sign");
        signformat.setHeight(150);
        cursor.insertImage(signformat);
    } else {
        cursor.insertText(tr("Начальник ОТК______________")+data->otk,textNormalFormat);
    }

}

void SertBuild::setPrn(bool p)
{
    prn=p;
    this->build(current_id,current_is_ship);
}


DataSert::DataSert(QObject *parent) : QObject(parent)
{
    chemModel = new QSqlQueryModel(this);
    mechModel = new QSqlQueryModel(this);
    sertModel = new QSqlQueryModel(this);
    mechCategory = new QSqlQueryModel(this);
    refreshMechCategory();
    QSqlQuery query;
    query.prepare("select adr, telboss||', '||telfax||', '||teldop||' '||site||' '||email, otk from hoz where id=1");
    if (query.exec()){
        while(query.next()){
            adres=query.value(0).toString();
            contact=query.value(1).toString();
            otk=query.value(2).toString();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
    logo.load("images/logo2.png");
    sign.load("images/nach.png");
}

void DataSert::refresh(int id, bool is_ship)
{
    QSqlQuery query;
    QString sQuery;
    sQuery= is_ship ? QString("select w.id_wparti, w.m_netto, s.nom_s, s.dat_vid, m.n_s, date_part('year',m.dat), m.dat, "
                  "b.n_plav, prov.nam, pprov.nam, d.diam, k.nam, pol.naim, pprov.is_cored "
                  "from wire_shipment_consist as w "
                  "inner join sertifikat as s on w.id_ship=s.id "
                  "inner join wire_parti as p on w.id_wparti=p.id "
                  "inner join wire_parti_m as m on p.id_m=m.id "
                  "inner join prov_buht as b on m.id_buht=b.id "
                  "inner join prov_prih as pr on b.id_prih=pr.id "
                  "inner join provol as prov on pr.id_pr=prov.id "
                  "inner join provol as pprov on m.id_provol=pprov.id "
                  "inner join diam as d on m.id_diam=d.id "
                  "inner join poluch as pol on s.id_pol=pol.id "
                  "inner join wire_pack_kind as k on p.id_pack=k.id "
                  "where w.id= :id ") :
                  QString("select p.id, m.kvo, NULL, NULL, m.n_s, date_part('year',m.dat), m.dat, "
                          "b.n_plav, prov.nam, pprov.nam, d.diam, k.nam, NULL, pprov.is_cored "
                          "from wire_parti as p "
                          "inner join wire_parti_m as m on p.id_m=m.id "
                          "inner join prov_buht as b on m.id_buht=b.id "
                          "inner join prov_prih as pr on b.id_prih=pr.id "
                          "inner join provol as prov on pr.id_pr=prov.id "
                          "inner join provol as pprov on m.id_provol=pprov.id "
                          "inner join diam as d on m.id_diam=d.id "
                          "inner join wire_pack_kind as k on p.id_pack=k.id "
                          "where p.id= :id ");
    query.prepare(sQuery);
    query.bindValue(":id",id);
    if (query.exec()){
        while(query.next()){
            id_wparti=query.value(0).toInt();
            netto=query.value(1).toDouble();
            nomSert=query.value(2).toString();
            dateVidSert=query.value(3).toDate();
            nomPart=query.value(4).toString();
            yearPart=query.value(5).toInt();
            datePart=query.value(6).toDate();
            nPlav=query.value(7).toString();
            srcProv=query.value(8).toString();
            prov=query.value(9).toString();
            diam=QLocale().toString(query.value(10).toDouble(),'f',1);
            spul=query.value(11).toString();
            poluch=query.value(12).toString();
            is_cored=query.value(13).toBool();
        }
        srcProv=prov;
        if (srcProv.right(2)==tr("-О") || srcProv.right(2)==tr("-П")) srcProv.truncate(srcProv.size()-2);
        refreshTu();
        refreshChem();
        refreshMech();
        refreshSert();
        refreshQR(id,is_ship);

    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}

void DataSert::refreshTu()
{
    QSqlQuery tuQuery;
    tuQuery.prepare("select g.nam from wire_gost as w "
                    "inner join gost_new as g on w.id_gost=g.id "
                    "where w.id_provol=(select m.id_provol from wire_parti as p inner join wire_parti_m as m on p.id_m=m.id where p.id=:id) order by g.nam");
    tuQuery.bindValue(":id",id_wparti);
    if (tuQuery.exec()){
        tuList.clear();
        while(tuQuery.next()){
            tuList.push_back(tuQuery.value(0).toString());
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),tuQuery.lastError().text(),QMessageBox::Ok);
    }
}

void DataSert::refreshChem()
{
    chemModel->setQuery("select c.sig, w.value from wire_parti_chem as w "
                        "inner join chem_tbl as c on w.id_chem=c.id "
                        "where w.id_part= (select p.id_m from wire_parti as p where p.id= "+QString::number(id_wparti)+" ) order by c.sig");
    if (chemModel->lastError().isValid())
        QMessageBox::critical(NULL,"Error",chemModel->lastError().text(),QMessageBox::Ok);
}

void DataSert::refreshMech()
{
    mechModel->setQuery("select w.id_cat, "
                        "m.nam_html, m.sig_html, m.prefix, w.value, w.value_max "
                        "from wire_parti_mech as w "
                        "inner join mech_tbl as m on w.id_mech=m.id "
                        "inner join wire_parti_m as p on p.id=w.id_part "
                        "inner join provol as prov on p.id_provol=prov.id "
                        "where w.id_part= (select p.id_m from wire_parti as p where p.id= "+QString::number(id_wparti)+" ) order by w.id_cat");
    if (mechModel->lastError().isValid())
        QMessageBox::critical(NULL,"Error",mechModel->lastError().text(),QMessageBox::Ok);
}

void DataSert::refreshSert()
{
    QString id_part=QString::number(id_wparti);
    sertModel->setQuery("select i.id_ved, i.vid, i.ved, i.nom, i.dat from ( "
                        "select pd.id_provol as id_provol, pd.id_diam as id_diam, zd.id_ved as id_ved, "
                        "d.fnam as vid, v.fnam as ved, zd.nom_doc as nom, zd.dat_doc as dat, "
                        "zd.dat_beg as beg, zd.dat_end as end "
                        "from zvd_wire_diam_sert as pd "
                        "inner join zvd_sert as zd on pd.id_sert=zd.id "
                        "inner join zvd_doc as d on zd.id_doc=d.id "
                        "inner join zvd_ved as v on zd.id_ved=v.id "
                        ") as i "
                        "where i.beg<=(select dat from wire_parti_m where id= (select p.id_m from wire_parti as p where p.id= "+id_part+") ) "
                        "and i.end>=(select dat from wire_parti_m where id= (select p.id_m from wire_parti as p where p.id= "+id_part+") ) "
                        "and i.id_provol=(select id_provol from wire_parti_m where id= (select p.id_m from wire_parti as p where p.id= "+id_part+") ) "
                        "and i.id_diam=(select id_diam from wire_parti_m where id= (select p.id_m from wire_parti as p where p.id= "+id_part+") ) order by i.id_ved");
    if (sertModel->lastError().isValid())
        QMessageBox::critical(NULL,"Error",sertModel->lastError().text(),QMessageBox::Ok);
}

void DataSert::refreshQR(int id, bool is_ship)
{
    QString str;
    str+="СЕРТИФИКАТ КАЧЕСТВА №"+nomPart.toUtf8()+"-"+QString::number(yearPart).toUtf8();
    if (is_ship) str+="/"+nomSert.toUtf8();
    str+="\n";
    str+=is_cored? "Наименование продукции " : "Марка проволоки ";
    str+=is_cored? "Порошковая проволока" : srcProv.toUtf8();
    str+="\n";
    str+=is_cored? "Марка " : "Условное обозначение проволоки ";
    str+=is_cored? prov.toUtf8() : diam.toUtf8()+" "+prov.toUtf8();
    str+="\n";
    str+=is_cored? "Диаметр, мм " : "Тип носителя проволоки ";
    str+=is_cored? diam.toUtf8() : spul.toUtf8();
    str+="\n";
    str+=is_cored? "Тип носителя проволоки " : "Номер плавки ";
    str+=is_cored? spul.toUtf8() : nPlav.toUtf8();
    str+="\n";
    str+="Номер партии "+nomPart.toUtf8();
    str+="\n";
    str+="Дата производства "+datePart.toString("dd.MM.yy").toUtf8();
    str+="\n";
    str+="Масса проволоки нетто, кг "+QString::number(netto).toUtf8();
    str+="\n";
    if (is_ship) str+="Грузополучатель: "+poluch.toUtf8()+"\n";
    QDate date;
    date=(is_ship)? dateVidSert : QDate::currentDate();
    str+="Дата "+date.toString("dd.MM.yy").toUtf8()+"\n";
    u_int32_t id_part=id_wparti;
    u_int32_t id_ship= is_ship ? id : 0;
    u_int64_t cod=0;

    memcpy((u_int8_t*)&cod,(u_int8_t*)&id_ship,4);
    memcpy((u_int8_t*)&cod+4,(u_int8_t*)&id_part,4);

    str+="Код подлинности "+QString::number(cod).toUtf8();
    QRcode *qr = QRcode_encodeString(str.toStdString().c_str(), 1, QR_ECLEVEL_L, QR_MODE_8, 0);
        bool ok= qr!=0;
        const int scale=10;
        int s=1;
        if (ok) s=(qr->width>0)? qr->width : 1;
        QImage img(s*scale,s*scale,QImage::Format_RGB32);
        QPainter painter(&img);
        if(ok){
            QColor fg("black");
            QColor bg("white");
            painter.setBrush(bg);
            painter.setPen(Qt::NoPen);
            painter.drawRect(0,0,s*scale,s*scale);
            painter.setBrush(fg);
            for(int y=0;y<s;y++){
                const int yy=y*s;
                for(int x=0;x<s;x++){
                    const int xx=yy+x;
                    const unsigned char b=qr->data[xx];
                    if(b & 0x01){
                        const double rx1=x*scale, ry1=y*scale;
                        QRectF r(rx1, ry1, scale, scale);
                        painter.drawRects(&r,1);
                    }
                }
            }
            QRcode_free(qr);
        }
        else{
            QColor error("red");
            painter.setBrush(error);
            painter.drawRect(0,0,s*scale,s*scale);
        }
        qrCode=img;
}

void DataSert::refreshMechCategory()
{
    mechCategory->setQuery("select id, nam from mech_category order by id");
    if (mechCategory->lastError().isValid())
            QMessageBox::critical(NULL,"Error",mechCategory->lastError().text(),QMessageBox::Ok);
}
