#include "sertbuild.h"

SertBuild::SertBuild(QObject *parent) :
    QTextDocument(parent)
{
    data=new DataSert(this);
    prn=false;
    l_rus=true;
    l_en=false;
    sample=false;
    connect(data,SIGNAL(sigRefresh()),this,SIGNAL(sigRefresh()));
}

QString SertBuild::getNomPart()
{
    return data->head()->nomPart;
}

QString SertBuild::getYearSert()
{
    return QString::number(data->head()->dateVidSert.year());
}

QString SertBuild::getYearPart()
{
    return QString::number(data->head()->yearPart);
}

QString SertBuild::getNomSert()
{
    return data->head()->nomSert;
}

bool SertBuild::getPrn()
{
    return prn;
}

DataSert *SertBuild::sData()
{
    return data;
}

void SertBuild::build(int id, bool is_ship)
{
    data->refresh(id, is_ship, sample);
    current_id=id;
    current_is_ship=is_ship;
    rebuild();
}

void SertBuild::rebuild()
{
    this->clear();

    QTextBlockFormat formatRirht;
    formatRirht.setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    QTextBlockFormat formatLeft;
    formatLeft.setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    QTextBlockFormat formatCenter;
    formatCenter.setAlignment(Qt::AlignCenter|Qt::AlignVCenter);

    QTextCharFormat textAdrFormat;
    QFont serifFont("Droid Sans",10,QFont::Normal);
    serifFont.setItalic(true);
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
    QFont tableFont("Droid Sans",10,QFont::Normal);
    textTableFormat.setFont(tableFont);

    QTextCursor cursor(this);

    cursor.movePosition(QTextCursor::Start);
    cursor.setBlockFormat(formatRirht);
    cursor.setCharFormat(textNormalFormat);
    insertText(cursor,tr("Форма 3.1 по EN 10204"),tr("Form 3.1 to EN 10204"));
    cursor.insertBlock(formatCenter);
    addResource(QTextDocument::ImageResource, QUrl("logo"), data->general()->logo);
    QTextImageFormat f;
    f.setName("logo");
    f.setHeight(70);
    cursor.insertImage(f);
    cursor.insertBlock(formatRirht);
    cursor.setCharFormat(textAdrFormat);
    insertText(cursor,data->general()->adres.rus,data->general()->adres.eng,true,false);
    cursor.insertBlock(formatRirht);
    cursor.insertText(data->general()->contact,textAdrFormat);
    cursor.insertBlock(formatLeft);

    QVector<int> id_ved;
    svData enSert;
    QHash<QString,QString> odobr;
    foreach (sertData s, *data->sert()){
        if (s.en){
            enSert.push_back(s);
            if (!id_ved.contains(s.id_ved)){
                id_ved.push_back(s.id_ved);

            }
            if (s.id_doc_t==3){
                QString str("Одобрено ");
                QString str_en("Approved by ");
                str+=s.ved_short.rus;
                str_en+=s.ved_short.eng;
                QString cat=s.grade_nam;
                QString sh=str+QString(".");
                if (!cat.isEmpty()){
                    str+=QString(" по категории ")+cat;
                    str_en+=QString(" in category ")+cat;
                }
                str+=QString(".");
                str_en+=QString(".");
                if (odobr.contains(sh) && (str!=sh)){
                    odobr.remove(sh);
                }
                QStringList lst=odobr.keys();
                if (lst.indexOf(QRegExp("^"+str+".*"))==-1) {
                    odobr.insert(str,str_en);
                }
            }
        }
    }

    foreach (int id, id_ved){
        QImage img;
        img.loadFromData(Models::instance()->relVedPix->data(QString::number(id)).toByteArray());
        if (!img.isNull()) {
            addResource(QTextDocument::ImageResource, QUrl("vedimage"+QString::number(id)), img);
            QTextImageFormat f;
            f.setName("vedimage"+QString::number(id));
            f.setHeight(45);
            cursor.insertImage(f);
        }
    }

    cursor.insertBlock(formatCenter);
    cursor.setCharFormat(textTitleFormat);
    insertText(cursor,tr("СЕРТИФИКАТ КАЧЕСТВА"),tr("QUALITY CERTIFICATE"));
    cursor.insertText(tr(" №")+data->head()->nomPart+"-"+QString::number(data->head()->yearPart),textTitleFormat);
    if (current_is_ship){
        cursor.insertText("/"+data->head()->nomSert,textTitleFormat);
    }
    cursor.insertBlock(formatLeft);
    cursor.setCharFormat(textBoldFormat);
    insertText(cursor,tr("Нормативная документация"),tr("Normative documents"));
    cursor.insertText(tr(": "),textBoldFormat);
    QString tulist=data->tu();
    cursor.insertText(tulist,textNormalFormat);
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

    v.push_back(QTextLength(QTextLength::PercentageLength,60));
    v.push_back(QTextLength(QTextLength::VariableLength,5));
    v.push_back(QTextLength(QTextLength::VariableLength,35));
    tableMechFormat.setColumnWidthConstraints(v);

    QTextTable *mainTable = cursor.insertTable(1, 7, tableFormat);

    cursor=mainTable->cellAt(0,0).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textBoldFormat);
    if (data->head()->is_cored){
        insertText(cursor,tr("Наименование продукции"),tr("Product name"),true);
    } else {
        insertText(cursor,tr("Марка проволоки"),tr("Wire mark"),true);
    }

    cursor=mainTable->cellAt(0,1).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textBoldFormat);
    if (data->head()->is_cored){
        insertText(cursor,tr("Марка"),tr("Mark"),true);
    } else {
        insertText(cursor,tr("Условное обозначение проволоки"),tr("Wire symbol"),true);
    }

    cursor=mainTable->cellAt(0,2).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textBoldFormat);
    if (data->head()->is_cored){
        insertText(cursor,tr("Диаметр, мм"),tr("Diameter, mm"),true);
    } else {
        insertText(cursor,tr("Тип носителя проволоки"),tr("Type of wire winding"),true);
    }

    cursor=mainTable->cellAt(0,3).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textBoldFormat);
    if (data->head()->is_cored){
        insertText(cursor,tr("Тип носителя проволоки"),tr("Type of wire winding"),true);
    } else {
        insertText(cursor,tr("Номер плавки"),tr("Heat number"),true);
    }

    cursor=mainTable->cellAt(0,4).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textBoldFormat);
    insertText(cursor,tr("Номер партии"),tr("Batch number"),true);

    cursor=mainTable->cellAt(0,5).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textBoldFormat);
    insertText(cursor,tr("Дата производства"),tr("Date of manufacture"),true);

    cursor=mainTable->cellAt(0,6).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textBoldFormat);
    insertText(cursor,tr("Масса проволоки нетто, кг"),tr("Net weight, kg"),true);

    mainTable->insertRows(mainTable->rows(),1);
    cursor=mainTable->cellAt(1,0).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textNormalFormat);
    if (data->head()->is_cored){
        insertText(cursor,tr("Порошковая проволока"),tr("Flux cored wire"),true);
    } else {
        cursor.insertText(data->head()->srcProv,textNormalFormat);
    }
    cursor=mainTable->cellAt(1,1).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textNormalFormat);
    if (data->head()->is_cored){
        cursor.insertText(data->head()->prov,textNormalFormat);
    } else {
        insertDouble(cursor,data->head()->diam,1);
        cursor.insertText(" "+data->head()->prov,textNormalFormat);
    }
    cursor=mainTable->cellAt(1,2).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textNormalFormat);
    if (data->head()->is_cored){
        insertDouble(cursor,data->head()->diam,1);
    } else {
        insertText(cursor,data->head()->spool.rus,data->head()->spool.eng,true);
    }
    cursor=mainTable->cellAt(1,3).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textNormalFormat);
    if (data->head()->is_cored){
        insertText(cursor,data->head()->spool.rus,data->head()->spool.eng,true);
    } else {
        cursor.insertText(data->head()->nPlav,textNormalFormat);
    }
    cursor=mainTable->cellAt(1,4).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(data->head()->nomPart,textNormalFormat);
    cursor=mainTable->cellAt(1,5).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textNormalFormat);
    insertDate(cursor,data->head()->datePart);
    cursor=mainTable->cellAt(1,6).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textNormalFormat);
    insertDouble(cursor,data->head()->netto,1);
    cursor.movePosition(QTextCursor::End);
    cursor.setBlockFormat(formatCenter);
    cursor.insertBlock();
    cursor.insertBlock();

    int row=data->chem()->size();
    if (row) {
        QTextTable *chemTable = cursor.insertTable(3, row, tableFormat);
        cursor=chemTable->cellAt(0,0).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textBoldFormat);
        if (data->head()->is_cored){
            insertText(cursor,tr("Химический состав наплавленного металла, % *"),tr("The chemical composition of the weld metal, % *"),true);
        } else {
            insertText(cursor,tr("Химический состав проволоки, %"),tr("The chemical composition of the wire, %"),true);
        }
        chemTable->mergeCells(0,0,1,row);
        int i=0;
        foreach (chemData c, *data->chem()) {
            if (!c.value.isNull()){
                cursor=chemTable->cellAt(2,i).firstCursorPosition();
                cursor.setBlockFormat(formatCenter);
                cursor.setCharFormat(textTableFormat);
                insertDouble(cursor,c.value,3);
                cursor=chemTable->cellAt(1,i).firstCursorPosition();
                cursor.setBlockFormat(formatCenter);
                cursor.insertText(c.name,textTableFormat);
                i++;
            }
        }
    }

    cursor.movePosition(QTextCursor::End);
    cursor.setBlockFormat(formatCenter);
    cursor.insertBlock();
    cursor.insertBlock();

    QVector<int> meh;
    foreach (mechData m, *data->mech()){
        if (!meh.contains(m.id_cat)){
            meh.push_back(m.id_cat);
        }
    }

    QTextTable *mechTable0 = cursor.insertTable(data->mech()->size()+meh.size()+1,3,tableMechFormat);
    int pos=0;
    foreach (int id_cat, meh){
        mechTable0->cellAt(pos,0).firstCursorPosition().setBlockFormat(formatCenter);
        QString nameCat=data->mechCategory(id_cat).rus;
        QString nameCat_en=data->mechCategory(id_cat).eng;
        cursor=mechTable0->cellAt(pos,0).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,nameCat,nameCat_en,true);
        mechTable0->mergeCells(pos,0,1,3);
        pos++;
        foreach (mechData m, *data->mech()){
            if (m.id_cat==id_cat){

                cursor= mechTable0->cellAt(pos,0).firstCursorPosition();
                cursor.setBlockFormat(formatLeft);
                cursor.setCharFormat(textTableFormat);
                insertText(cursor,m.nam_html.rus,m.nam_html.eng,true,true,true);

                cursor=mechTable0->cellAt(pos,1).firstCursorPosition();
                cursor.setBlockFormat(formatCenter);
                cursor.setCharFormat(textTableFormat);
                insertText(cursor,m.sig_htlm.rus,m.sig_htlm.eng,true,true,true);

                cursor=mechTable0->cellAt(pos,2).firstCursorPosition();
                cursor.setBlockFormat(formatRirht);
                cursor.setCharFormat(textTableFormat);
                insertText(cursor,m.prefix.rus,m.prefix.eng,true,true);
                if (!m.prefix.rus.isEmpty()){
                    cursor.insertText(" ",textTableFormat);
                }
                insertDouble(cursor,m.value,2);

                if (!m.value_max.isNull()){
                    cursor.insertText(" - ",textNormalFormat);
                    insertDouble(cursor,m.value_max,2);
                }

                pos++;
            }
        }
    }
    cursor=mechTable0->cellAt(pos,0).firstCursorPosition();
    cursor.setBlockFormat(formatLeft);
    cursor.setCharFormat(textNormalFormat);
    QString stat=tr("Состояние поверхности проволоки: поверхность проволоки чистая, "
                    "гладкая, без трещин, расслоений, плен, закатов, раковин, забоин "
                    "окалины, ржавчины, масла, технологической смазки и других загрязнений.\n"
                    "Гарантийный срок хранения сварочной проволоки в упаковке производителя - 12 месяцев с момента изготовления.");
    QString stat_en=tr("The condition of the surface of the wire: the surface of the wire is clean, smooth, without cracks, delaminations, "
                       "slivers, laps, shells, nicks, scale, rust, oil, grease and other contaminants.\n"
                       "The guaranteed shelf life of the welding wire in the manufacturer's packaging is 12 months from the date of manufacture.");
    insertText(cursor,stat,stat_en,true);
    mechTable0->mergeCells(pos,0,1,3);

    cursor.movePosition(QTextCursor::End);
    if (data->head()->is_cored){
        cursor.setBlockFormat(formatLeft);
        cursor.setCharFormat(textNormalFormat);
        insertText(cursor,tr("* для смеси газов CO<sub>2</sub> 20%"),tr("* for gas mixture CO<sub>2</sub> 20%"),false,true,true);
    }
    cursor.insertBlock();

    if (enSert.size()){
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textTitleFormat);
        insertText(cursor,tr("Аттестация и сертификация"),tr("Certification"),false,true);
        cursor.insertBlock();
        QTextTable *sertTable = cursor.insertTable(enSert.size()+1,4,tableFormat);
        cursor=sertTable->cellAt(0,0).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,tr("Вид документа"),tr("Type of document"),true);
        cursor=sertTable->cellAt(0,1).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,tr("Сертифицирующий орган"),tr("Certification authority"),true);
        cursor=sertTable->cellAt(0,2).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,tr("Номер документа"),tr("Document Number"),true);
        cursor=sertTable->cellAt(0,3).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,tr("Дата выдачи"),tr("Date of issue"),true);
        int i=0;
        foreach (sertData s, enSert){

            cursor=sertTable->cellAt(i+1,0).firstCursorPosition();
            cursor.setBlockFormat(formatCenter);
            cursor.setCharFormat(textTableFormat);
            insertText(cursor,s.doc_nam.rus,s.doc_nam.eng,true);

            cursor=sertTable->cellAt(i+1,1).firstCursorPosition();
            cursor.setBlockFormat(formatCenter);
            cursor.setCharFormat(textTableFormat);
            insertText(cursor,s.ved_nam.rus,s.ved_nam.eng,true);

            sertTable->cellAt(i+1,2).firstCursorPosition().setBlockFormat(formatCenter);
            sertTable->cellAt(i+1,2).firstCursorPosition().insertText(s.nom_doc,textTableFormat);

            cursor=sertTable->cellAt(i+1,3).firstCursorPosition();
            cursor.setBlockFormat(formatCenter);
            cursor.setCharFormat(textTableFormat);
            insertDate(cursor,s.date_doc);
            i++;
        }
        cursor.movePosition(QTextCursor::End);
        cursor.insertBlock();
    }

    cursor.setBlockFormat(formatLeft);
    cursor.setCharFormat(textNormalFormat);

    QHash<QString, QString>::const_iterator it = odobr.constBegin();
    while (it != odobr.constEnd()) {
        insertText(cursor,it.key(),it.value());
        cursor.insertBlock();
        ++it;
    }

    cursor.setBlockFormat(formatLeft);
    if (current_is_ship){
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,tr("Грузополучатель"),tr("Consignee"));
        cursor.insertText(":\n",textBoldFormat);
        cursor.setCharFormat(textNormalFormat);
        if (!sample){
            insertText(cursor,data->head()->poluch.rus,data->head()->poluch.eng,true);
        }
        cursor.insertBlock();
    }
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textNormalFormat);
    insertText(cursor,tr("При переписке по вопросам качества просьба ссылаться на номер партии"),tr("When correspondence on quality issues, please refer to the batch number"),true);

    cursor.insertBlock();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textBoldFormat);
    QDate date;
    date=(current_is_ship)? data->head()->dateVidSert : QDate::currentDate();
    insertText(cursor,tr("Дата выдачи сертификата"),tr("Date of issue of the certificate"),false);
    cursor.insertText(tr(": "),textBoldFormat);
    cursor.setCharFormat(textNormalFormat);
    if (!sample){
        insertDate(cursor,date,false);
    } else {
        insertDate(cursor,QDate(1111,11,11),false);
    }
    cursor.insertBlock();

    cursor.setBlockFormat(formatLeft);
    addResource(QTextDocument::ImageResource, QUrl("qrcode"), *data->qrCode());
    QTextImageFormat qrformat;
    qrformat.setName("qrcode");
    qrformat.setHeight(150);
    cursor.insertImage(qrformat);

    cursor.insertText("   ",textNormalFormat);
    QString nach=data->general()->otk_title.rus;
    QString nach_en=data->general()->otk_title.eng;
    QString line=tr("______________");
    if (prn && !sample) {
        QImage im(data->general()->sign);
        QPainter p(&im);
        QFont f(textNormalFormat.font());
        f.setPixelSize(44);
        p.setFont(f);
        QFontMetrics fm(f);
        QString str, suf;
        if (l_en && !l_rus){
            suf=nach_en;
            str=data->general()->otk.eng;
        } else if (l_rus && !l_en){
            suf=nach;
            str=data->general()->otk.rus;
        } else if (l_rus && l_en){
            suf=nach+" / "+nach_en;
            str=data->general()->otk.rus+" / "+data->general()->otk.eng;
        }
        str=suf+line+str;
        int pos=(im.width()/2-fm.horizontalAdvance(suf));
        if (pos<0){
            pos=0;
        }
        p.drawText(pos,150,str);
        addResource(QTextDocument::ImageResource, QUrl("sign"), im);
        QTextImageFormat signformat;
        signformat.setName("sign");
        signformat.setHeight(150);
        cursor.insertImage(signformat);
    } else {
        cursor.setCharFormat(textNormalFormat);
        if (l_en && !l_rus){
            cursor.insertText("                               ",textNormalFormat);
        } else if (l_rus && !l_en){
            cursor.insertText("                                                    ",textNormalFormat);
        }
        insertText(cursor,nach,nach_en);
        cursor.insertText(line,textNormalFormat);
        if (!sample){
            insertText(cursor,data->general()->otk.rus,data->general()->otk.eng);
        } else {
            insertText(cursor,tr("[МЕСТО ДЛЯ ПЕЧАТИ, ПОДПИСЬ]"),tr("[LOCUS SIGILLI, SIGNATURE]"));
        }
    }
}

void SertBuild::insertText(QTextCursor &c, const QString &rus, const QString &eng, bool newpar, bool sep, bool html)
{
    QTextBlockFormat blockFormat=c.blockFormat();
    QTextCharFormat charFormat=c.charFormat();
    if (l_rus || eng.isEmpty() || rus==eng){
        if (html){
            insertHtml(c,rus);
        } else {
            c.insertText(rus,charFormat);
        }
    }
    if (l_en && !eng.isEmpty() && rus!=eng){
        if (l_rus){
                c.insertText(sep ? " / " : " ",charFormat);
            if (newpar){
                c.insertBlock(blockFormat);
            }
        }
        if (html){
            insertHtml(c,eng);
        } else {
            c.insertText(eng,charFormat);
        }
    }
}

void SertBuild::insertHtml(QTextCursor &c, const QString &html)
{
    QTextDocument hd;
    QTextCursor chd(&hd);
    chd.setBlockFormat(c.blockFormat());
    chd.setCharFormat(c.charFormat());
    chd.insertHtml(html);
    chd.select(QTextCursor::Document);
    chd.mergeCharFormat(c.charFormat());
    chd.mergeBlockFormat(c.blockFormat());
    c.insertFragment(chd.selection());
}

void SertBuild::insertDouble(QTextCursor &c, const QVariant &val, int dec)
{
    if (!val.isNull()){
        QLocale lrus(QLocale::Russian);
        bool ok=false;
        double v=val.toDouble(&ok);
        if (ok) {
            if (l_rus && !l_en){
                c.insertText(lrus.toString(v,'f',dec));
            } else {
                c.insertText(QString::number(v,'f',dec));
            }
        }
    }
}

void SertBuild::insertDate(QTextCursor &c, const QDate &date, bool newpar)
{
    QLocale lrus(QLocale::Russian);
    QLocale leng(QLocale::English);
    if (l_rus && !l_en){
        c.insertText(lrus.toString(date,"dd.MM.yyyy"));
    }
    if (l_en){
        if (l_rus){
            c.insertText(lrus.toString(date,"dd MMM yyyy")+" / ");
            if(newpar){
                c.insertText("\n");
            }
        }
        c.insertText(leng.toString(date,"MMM dd, yyyy"));
    }
}

void SertBuild::setPrn(bool p)
{
    prn=p;
    this->rebuild();
}

void SertBuild::setLRus(bool b)
{
    if (b){
        l_rus=true;
        l_en=false;
        this->rebuild();
    }
}

void SertBuild::setLEn(bool b)
{
    if (b){
        l_rus=false;
        l_en=true;
        this->rebuild();
    }
}

void SertBuild::setLMix(bool b)
{
    if (b){
        l_rus=true;
        l_en=true;
        this->rebuild();
    }
}

void SertBuild::setSample(bool b)
{
    sample=b;
    this->build(current_id,current_is_ship);
}

void SertBuild::setDocEn(int id_doc, bool en)
{
    data->setDocEn(id_doc,en);
    this->rebuild();
}

void SertBuild::setDefaultDoc()
{
    data->setDefaultDoc();
    this->build(current_id,current_is_ship);
}

DataSert::DataSert(QObject *parent) : QObject(parent)
{

}

void DataSert::refresh(int id, bool is_ship, bool sample)
{
    QSqlQuery query;
    QString sQuery;
    sQuery= is_ship ? QString("select w.id_wparti, w.m_netto, s.nom_s, s.dat_vid, m.n_s, date_part('year',m.dat), m.dat, "
                  "b.n_plav, prov.nam, pprov.nam, d.diam, k.short, pol.naim, pprov.is_cored, pol.naim_en, k.short_en, coalesce(bprov.nam, pprov.nam) "
                  "from wire_shipment_consist as w "
                  "inner join sertifikat as s on w.id_ship=s.id "
                  "inner join wire_parti as p on w.id_wparti=p.id "
                  "inner join wire_parti_m as m on p.id_m=m.id "
                  "inner join prov_buht as b on m.id_buht=b.id "
                  "inner join prov_prih as pr on b.id_prih=pr.id "
                  "inner join provol as prov on pr.id_pr=prov.id "
                  "inner join provol as pprov on m.id_provol=pprov.id "
                  "left join provol as bprov on pprov.id_base=bprov.id "
                  "inner join diam as d on m.id_diam=d.id "
                  "inner join poluch as pol on s.id_pol=pol.id "
                  "inner join wire_pack_kind as k on p.id_pack=k.id "
                  "where w.id= :id ") :
                  QString("select p.id, m.kvo, NULL, NULL, m.n_s, date_part('year',m.dat), m.dat, "
                          "b.n_plav, prov.nam, pprov.nam, d.diam, k.short, NULL, pprov.is_cored, NULL, k.short_en, coalesce(bprov.nam, pprov.nam) "
                          "from wire_parti as p "
                          "inner join wire_parti_m as m on p.id_m=m.id "
                          "inner join prov_buht as b on m.id_buht=b.id "
                          "inner join prov_prih as pr on b.id_prih=pr.id "
                          "inner join provol as prov on pr.id_pr=prov.id "
                          "inner join provol as pprov on m.id_provol=pprov.id "
                          "left join provol as bprov on pprov.id_base=bprov.id "
                          "inner join diam as d on m.id_diam=d.id "
                          "inner join wire_pack_kind as k on p.id_pack=k.id "
                          "where p.id= :id ");
    query.prepare(sQuery);
    query.bindValue(":id",id);
    if (query.exec()){
        while(query.next()){
            hData.id_wparti=query.value(0).toInt();
            hData.netto=sample ? 1111 : query.value(1).toDouble();
            hData.nomSert=query.value(2).toString();
            hData.dateVidSert=query.value(3).toDate();
            hData.nomPart=sample ? "1111" : query.value(4).toString();
            hData.yearPart=query.value(5).toInt();
            hData.datePart=sample ? QDate(1111,11,11) :query.value(6).toDate();
            hData.nPlav=sample ? "1111" : query.value(7).toString();
            hData.prov=query.value(9).toString();
            hData.diam=query.value(10).toDouble();
            hData.spool.rus=query.value(11).toString();
            hData.poluch.rus=query.value(12).toString();
            hData.is_cored=query.value(13).toBool();
            hData.poluch.eng=query.value(14).toString();
            hData.spool.eng=query.value(15).toString();
            hData.srcProv=query.value(16).toString();
        }
        refreshGeneralData(hData.dateVidSert);
        refreshTu();
        refreshChem();
        refreshMech();
        refreshSert();
        refreshQR(id,is_ship,sample);
        emit sigRefresh();
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}

const generalData *DataSert::general()
{
    return &gData;
}

QString DataSert::tu()
{
    QString str;
    QStringList::const_iterator constIterator;
    for (constIterator = tuList.constBegin(); constIterator != tuList.constEnd();++constIterator){
        if (!str.isEmpty()){
            str+=", ";
        }
        str+=(*constIterator);
    }
    return str;
}

const headData *DataSert::head()
{
    return &hData;
}

const cvData *DataSert::chem()
{
    return &cData;
}

sLang DataSert::mechCategory(int id)
{
    return mechCat.value(id);
}

const mvData *DataSert::mech()
{
    return &mData;
}

const svData *DataSert::sert()
{
    return &sData;
}

const QImage *DataSert::qrCode()
{
    return &qr_code;
}

void DataSert::setDocEn(int id_doc, bool en)
{
    for (int i=0; i<sData.size(); i++){
        if (sData[i].id_doc==id_doc){
            sData[i].en=en;
        }
    }
    mapSert[id_doc]=en;
}

void DataSert::setDefaultDoc()
{
    mapSert.clear();
}

void DataSert::refreshTu()
{
    QSqlQuery tuQuery;
    tuQuery.prepare("select g.nam from wire_parti_gost as w "
                    "inner join gost_new as g on w.id_gost=g.id "
                    "where w.id_parti = (select p.id_m from wire_parti as p where p.id = :id) order by g.nam");
    tuQuery.bindValue(":id",hData.id_wparti);
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
    QSqlQuery query;
    query.prepare("select c.sig, w.value from wire_parti_chem as w "
                  "inner join chem_tbl as c on w.id_chem=c.id "
                  "where w.id_part = (select p.id_m from wire_parti as p where p.id= :id_part ) order by c.sig");
    query.bindValue(":id_part",hData.id_wparti);
    cData.clear();
    if (query.exec()){
        while (query.next()) {
            chemData c;
            c.name=query.value(0).toString();
            c.value=query.value(1);
            cData.push_back(c);
        }
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Ok);
    }
}

void DataSert::refreshMech()
{
    QSqlQuery query;
    query.prepare("select w.id_cat, "
                  "m.nam_html, m.sig_html, m.prefix, w.value, w.value_max, m.nam_html_en, m.sig_html_en, m.prefix_en "
                  "from wire_parti_mech as w "
                  "inner join mech_tbl as m on w.id_mech=m.id "
                  "inner join wire_parti_m as p on p.id=w.id_part "
                  "inner join provol as prov on p.id_provol=prov.id "
                  "where w.id_part= (select p.id_m from wire_parti as p where p.id = :id_part ) order by w.id_cat");
    query.bindValue(":id_part",hData.id_wparti);
    mData.clear();
    if (query.exec()){
        while (query.next()){
            mechData m;
            m.id_cat=query.value(0).toInt();
            m.nam_html.rus=query.value(1).toString();
            m.sig_htlm.rus=query.value(2).toString();
            m.prefix.rus=query.value(3).toString();
            m.value=query.value(4);
            m.value_max=query.value(5);
            m.nam_html.eng=query.value(6).toString();
            m.sig_htlm.eng=query.value(7).toString();
            m.prefix.eng=query.value(8).toString();
            mData.push_back(m);
        }
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Ok);
    }
}

void DataSert::refreshSert()
{
    QSqlQuery query;
    query.prepare("select i.id_ved, i.vid, i.ved, i.nom, i.dat, i.vid_en, i.ved_en, i.id_doc, i.en, i.id_doc_t, i.ved_short, i.ved_short_en, i.grade "
                  "from zvd_get_wire_sert_var( "
                  "(select dat from wire_parti_m where id= (select p.id_m from wire_parti as p where p.id= :id_part1 ) ), "
                  "(select id_provol from wire_parti_m where id= (select p.id_m from wire_parti as p where p.id= :id_part2 ) ), "
                  "(select id_diam from wire_parti_m where id= (select p.id_m from wire_parti as p where p.id= :id_part3 ) ), "
                  "(select p.id_var from wire_parti as p where p.id = :id_part4 ) "
                  ") as i order by i.id_ved");
    query.bindValue(":id_part1",hData.id_wparti);
    query.bindValue(":id_part2",hData.id_wparti);
    query.bindValue(":id_part3",hData.id_wparti);
    query.bindValue(":id_part4",hData.id_wparti);
    sData.clear();
    if (query.exec()){
        while (query.next()){
            sertData s;
            s.id_ved=query.value(0).toInt();
            s.doc_nam.rus=query.value(1).toString();
            s.ved_nam.rus=query.value(2).toString();
            s.nom_doc=query.value(3).toString();
            s.date_doc=query.value(4).toDate();
            s.doc_nam.eng=query.value(5).toString();
            s.ved_nam.eng=query.value(6).toString();
            s.id_doc=query.value(7).toInt();
            s.en=mapSert.value(s.id_doc,query.value(8).toBool());
            s.id_doc_t=query.value(9).toInt();
            s.ved_short.rus=query.value(10).toString();
            s.ved_short.eng=query.value(11).toString();
            s.grade_nam=query.value(12).toString();
            sData.push_back(s);
        }
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Ok);
    }
}

void DataSert::refreshQR(int id, bool is_ship, bool sample)
{
    QString str;
    str+=sample? "ОБРАЗЕЦ СЕРТИФИКАТА КАЧЕСТВА №" : "СЕРТИФИКАТ КАЧЕСТВА №";
    str+=hData.nomPart+"-"+QString::number(hData.yearPart);
    if (is_ship) str+="/"+hData.nomSert;
    str+="\n";
    str+=hData.is_cored? "Наименование продукции " : "Марка проволоки ";
    str+=hData.is_cored? "Порошковая проволока" : hData.srcProv;
    str+="\n";
    str+=hData.is_cored? "Марка " : "Условное обозначение проволоки ";
    str+=hData.is_cored? hData.prov : QString::number(hData.diam)+" "+hData.prov;
    str+="\n";
    str+=hData.is_cored? "Диаметр, мм " : "Тип носителя проволоки ";
    str+=hData.is_cored? QString::number(hData.diam) : hData.spool.rus;
    str+="\n";
    str+=hData.is_cored? "Тип носителя проволоки " : "Номер плавки ";
    str+=hData.is_cored? hData.spool.rus : hData.nPlav;
    str+="\n";
    str+="Номер партии "+hData.nomPart;
    str+="\n";
    str+="Дата производства "+hData.datePart.toString("dd.MM.yy");
    str+="\n";
    str+="Масса проволоки нетто, кг "+QString::number(hData.netto);
    str+="\n";
    if (is_ship && !sample) {
        str+="Грузополучатель: "+hData.poluch.rus+"\n";
    }
    QDate date=QDate::currentDate();
    if (is_ship && !sample){
        date=hData.dateVidSert;
    } else if (sample){
        date=QDate(1111,11,11);
    }
    str+="Дата "+date.toString("dd.MM.yy")+"\n";

    quint32 id_part=hData.id_wparti;
    quint32 id_ship= is_ship ? id : 0;
    quint64 cod=0;

    memcpy((quint8*)&cod,(quint8*)&id_ship,4);
    memcpy((quint8*)&cod+4,(quint8*)&id_part,4);

    str+="Код подлинности "+QString::number(cod);
    QrEncode qr;
    bool ok=qr.encodeData(0,0,true,-1,str.toUtf8().data());
    const int scale=10;
    int s=(qr.size()>0)? qr.size() : 1;;
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
            for(int x=0;x<s;x++){
                if(qr.data(y,x)){
                    const double rx1=x*scale, ry1=y*scale;
                    QRectF r(rx1, ry1, scale, scale);
                    painter.drawRects(&r,1);
                }
            }
        }
    } else {
        QColor error("red");
        painter.setBrush(error);
        painter.drawRect(0,0,scale-1,scale-1);
    }
    qr_code=img;
}

void DataSert::refreshGeneralData(QDate date)
{
    refreshMechCategory();
    QSqlQuery query;
    query.prepare("select adr, telboss||', '||telfax||', '||teldop||' '||site||' '||email, otk, adr_en, otk_en, otk_title, otk_title_en, sign "
                  "from general_data where dat = (select max(mgd.dat) from general_data mgd where mgd.dat <= :dat )");
    query.bindValue(":dat",date);
    if (query.exec()){
        while(query.next()){
            gData.adres.rus=query.value(0).toString();
            gData.contact=query.value(1).toString();
            gData.otk.rus=query.value(2).toString();
            gData.adres.eng=query.value(3).toString();
            gData.otk.eng=query.value(4).toString();
            gData.otk_title.rus=query.value(5).toString();
            gData.otk_title.eng=query.value(6).toString();
            gData.sign.loadFromData(query.value(7).toByteArray());
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
    gData.logo.load("images/logo2.png");
}

void DataSert::refreshMechCategory()
{
    QSqlQuery query;
    query.prepare("select id, nam, nam_en from mech_category order by id");
    if (query.exec()){
        mechCat.clear();
        while (query.next()){
            int id=query.value(0).toInt();
            sLang nam;
            nam.rus=query.value(1).toString();
            nam.eng=query.value(2).toString();
            mechCat.insert(id,nam);
        }
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Ok);
    }
}
