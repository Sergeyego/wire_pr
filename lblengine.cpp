#include "lblengine.h"

LblEngine::LblEngine(QObject *parent) : QObject(parent)
{
    dir=QDir::currentPath();
    QDomDocument domDocument;
    QFile fileTemp(dir+"/templates/glabels_szsm.xml");
    domDocument.setContent(&fileTemp, true);
    QDomNodeList list;
    QDomElement main = domDocument.documentElement();
    if ((main.tagName() == "labels-templates")||(main.tagName() == "Glabels-templates")) {
        list = main.elementsByTagName("Template");
        for (int i=0; i<list.size(); i++){
            QString nam=list.at(i).toElement().attribute("brand");
            map[nam]=list.at(i).toElement();
        }
    }
    fileTemp.close();
}

void LblEngine::createLblEd(int id_part, QString nam, QString otk, bool barcode, bool opt)
{
    QString marka, diam, spool, plavka, part, date, massa, ean;
    QSqlQuery query;
    query.prepare("select pr.nam, d.diam, k.short, b.n_plav, m.n_s, m.dat, t.mas_ed, e.ean_ed, e.ean_group "
                  "from wire_parti as p "
                  "inner join wire_parti_m as m on p.id_m=m.id "
                  "inner join provol as pr on m.id_provol=pr.id "
                  "inner join diam as d on m.id_diam=d.id "
                  "inner join wire_pack_kind as k on p.id_pack=k.id "
                  "inner join prov_buht as b on m.id_buht=b.id "
                  "left join wire_pack as t on p.id_pack_type=t.id "
                  "left join wire_ean as e on (pr.id=e.id_prov and d.id=e.id_diam and k.id=e.id_spool and t.id=e.id_pack) "
                  "where p.id = :id_part ");
    query.bindValue(":id_part",id_part);
    if (query.exec()){
        while (query.next()){
            marka= query.value(0).toString();
            diam=QLocale().toString(query.value(1).toDouble());
            spool=query.value(2).toString();
            plavka=query.value(3).toString();
            part=query.value(4).toString();
            date=query.value(5).toDate().toString("dd.MM.yy");
            massa=query.value(6).toString();
            ean=(!query.value(8).isNull() && barcode) ? query.value(7).toString() : QString();
        }

        if (massa.toDouble()==0.0) massa="";
        QString str;
        str+=tr("Марка - ")+marka+"\n";
        str+=tr("Диаметр, мм - ")+diam+"\n";
        str+=tr("Плавка - ")+plavka+"\n";
        str+=tr("Партия - ")+part+"\n";
        str+=tr("Носитель - ")+spool+"\n";
        str+=tr("№ - ")+nam+"\n";
        //str+=tr("ОТК - ")+otk+"\n";
        str+=tr("Масса нетто, кг - ")+massa+"\n";
        str+=tr("Дата изг. - ")+date;
        createLblEd(str,ean,opt);

    } else {
        QMessageBox::critical(NULL,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    }
}

void LblEngine::createLblEd(QString text, QString barcode, bool opt)
{
    QString templ=QString::fromLocal8Bit("СЗСМ");
    if (map.value(templ).isNull()){
        QMessageBox::critical(NULL,tr("Ошибка"),tr("Ошибка шаблона"),QMessageBox::Ok);
        return;
    }
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));
    QDomElement root = doc.createElement("Glabels-document");
    doc.appendChild(root);
    root.appendChild(map.value(templ));
    QDomElement obj = doc.createElement("Objects");
    obj.setAttribute("id", 0);
    obj.setAttribute("rotate", "True");
    root.appendChild(obj);

    obj.appendChild(newText("4mm","35mm","38mm","28.5mm",text,7,true,&doc));
    if (!opt){
        obj.appendChild(newImage("6.5mm","4mm","33mm","13mm",dir+"/images/simb.png",&doc));
        obj.appendChild(newImage("5.5mm","17.5mm","35mm","2.5mm",dir+"/images/site.png",&doc));
    } else {
        obj.appendChild(newImage("6.5mm","4mm","33mm","13mm",dir+"/images/logo-dark.png",&doc));
    }
    if (!barcode.isEmpty()) {
        obj.appendChild(newBarcode("4mm","20.5mm","38mm","17mm",barcode,&doc));
    }
    QFile file("lbl.glabels");
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        doc.save(stream,1);
        file.close();
        system("./runlbl.sh&");
    }
}

void LblEngine::createLblGroup(int id_part)
{
    QString marka, diam, spool, plavka, part, date, massa, ean, description;
    QSqlQuery query;
    query.prepare("select pr.nam, d.diam, k.short, b.n_plav, m.n_s, m.dat, t.mas_group, e.ean_ed, e.ean_group, pr.description "
                  "from wire_parti as p "
                  "inner join wire_parti_m as m on p.id_m=m.id "
                  "inner join provol as pr on m.id_provol=pr.id "
                  "inner join diam as d on m.id_diam=d.id "
                  "inner join wire_pack_kind as k on p.id_pack=k.id "
                  "inner join prov_buht as b on m.id_buht=b.id "
                  "left join wire_pack as t on p.id_pack_type=t.id "
                  "left join wire_ean as e on (pr.id=e.id_prov and d.id=e.id_diam and k.id=e.id_spool and t.id=e.id_pack) "
                  "where p.id = :id_part ");
    query.bindValue(":id_part",id_part);
    if (query.exec()){
        while (query.next()){
            marka= query.value(0).toString();
            diam=QLocale().toString(query.value(1).toDouble());
            spool=query.value(2).toString();
            plavka=query.value(3).toString();
            part=query.value(4).toString();
            date=query.value(5).toDate().toString("dd.MM.yy");
            massa=query.value(6).toString();
            ean=query.value(8).toString();
            if (ean.isEmpty()) ean=query.value(7).toString();
            description=query.value(9).toString();
        }
        if (massa.toDouble()==0.0) massa="";

        QString tuStr=tr("Проволока сварочная  ");
        QSqlQuery tuQuery;
        tuQuery.prepare("select g.nam from wire_parti_gost as w "
                        "inner join gost_new as g on w.id_gost=g.id "
                        "where w.id_parti = (select p.id_m from wire_parti as p where p.id = :id) order by g.nam");
        tuQuery.bindValue(":id",id_part);
        if (tuQuery.exec()){
            while(tuQuery.next()){
                tuStr+=tuQuery.value(0).toString()+", ";
            }
        } else {
            QMessageBox::critical(NULL,tr("Error"),tuQuery.lastError().text(),QMessageBox::Ok);
        }
        if (!tuStr.isEmpty()) tuStr.truncate(tuStr.size()-2);

        if (map.value(QString::fromLocal8Bit("SZSM-02")).isNull()){
            QMessageBox::critical(NULL,tr("Ошибка"),tr("Ошибка шаблона"),QMessageBox::Ok);
            return;
        }

        QString str0=marka+tr(" ø ")+diam+tr(" мм");
        QString str1;
        str1+=tr("Плавка - ")+plavka+"\n";
        str1+=tr("Партия - ")+part+"\n";
        str1+=tr("Носитель - ")+spool+"\n";
        str1+=tr("Масса нетто, кг - ")+massa+"\n";
        str1+=tr("Дата изг. - ")+date+"\n"+"\n";
        str1+=tr("Изготовлено в России");

        QDomDocument doc;
        doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));
        QDomElement root = doc.createElement("Glabels-document");
        doc.appendChild(root);
        root.appendChild(map.value(QString::fromLocal8Bit("SZSM-02")));
        QDomElement obj = doc.createElement("Objects");
        obj.setAttribute("id", 0);
        obj.setAttribute("rotate", "True");
        root.appendChild(obj);
        obj.appendChild(newRect("3mm","3mm","129mm","54mm",&doc));
        obj.appendChild(newLine("3mm","17mm","129mm","0mm",&doc));
        obj.appendChild(newLine("3mm","23mm","129mm","0mm",&doc));
        obj.appendChild(newLine("90mm","23mm","0mm","34mm",&doc));
        obj.appendChild(newLine("3mm","44mm","87mm","0mm",&doc));
        obj.appendChild(newImage("4mm","4mm","26mm","12mm",dir+"/images/simb.png",&doc));
        obj.appendChild(newText("35mm","5.5mm","95mm","10mm",str0,18,false,&doc));
        obj.appendChild(newText("4mm","18mm","127mm","4mm",tuStr,9,false,&doc));
        obj.appendChild(newText("4mm","24mm","85mm","19mm",description,9,false,&doc));
        obj.appendChild(newText("91mm","24mm","40mm","32mm",str1,9,false,&doc));
        if (!ean.isEmpty()) obj.appendChild(newBarcode("51mm","42mm","38mm","17mm",ean,&doc));

        QFile file("lbl.glabels");
        if ( file.open( QIODevice::WriteOnly ) ) {
            QTextStream stream( &file );
            doc.save(stream,1);
            file.close();
            system("./runlbl.sh&");
        }


    } else {
        QMessageBox::critical(NULL,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    }
}

QDomElement LblEngine::newImage(QString x, QString y, QString w, QString h, QString fnam, QDomDocument *doc)
{
    QDomElement l=doc->createElement("Object-image");
    l.setAttribute("x",x);
    l.setAttribute("y",y);
    l.setAttribute("w",w);
    l.setAttribute("h",h);
    l.setAttribute("src",fnam);
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    return l;
}

QDomElement LblEngine::newBarcode(QString x, QString y, QString w, QString h, QString code, QDomDocument *doc)
{
    QDomElement l =doc->createElement("Object-barcode");
    l.setAttribute("x",x);
    l.setAttribute("y",y);
    l.setAttribute("w",w);
    l.setAttribute("h",h);
    l.setAttribute("backend","gnu-barcode");
    l.setAttribute("style","EAN-13");
    l.setAttribute("text","True");
    l.setAttribute("color","0x000000ff");
    l.setAttribute("data",code);
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    return l;
}

QDomElement LblEngine::newText(QString x, QString y, QString w, QString h, QString text, int size, bool bold, QDomDocument *doc)
{
    QDomElement l =doc->createElement("Object-text");
    l.setAttribute("x",x);
    l.setAttribute("y",y);
    l.setAttribute("w",w);
    l.setAttribute("h",h);
    l.setAttribute("justify","Left");
    l.setAttribute("valign","Center");
    l.setAttribute("auto_shrink","False");
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    QDomElement span = doc->createElement("Span");
    span.setAttribute("color","0x000000ff");
    span.setAttribute("font_family","Liberation Sans");
    span.setAttribute("font_size",size);
    span.setAttribute("font_weight",bold ? "Bold" : "Regular");
    span.setAttribute("font_italic","False");
    span.setAttribute("line_spacing",1);
    span.appendChild(doc->createTextNode(text));
    l.appendChild(span);
    return l;
}

QDomElement LblEngine::newRect(QString x, QString y, QString w, QString h, QDomDocument *doc)
{
    QDomElement l =doc->createElement("Object-box");
    l.setAttribute("x",x);
    l.setAttribute("y",y);
    l.setAttribute("w",w);
    l.setAttribute("h",h);
    l.setAttribute("line_width","0.352778mm");
    l.setAttribute("line_color","0x000000ff");
    l.setAttribute("fill_color","0x00000000");
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    return l;
}

QDomElement LblEngine::newLine(QString x, QString y, QString dx, QString dy, QDomDocument *doc)
{
    QDomElement l =doc->createElement("Object-line");
    l.setAttribute("x",x);
    l.setAttribute("y",y);
    l.setAttribute("dx",dx);
    l.setAttribute("dy",dy);
    l.setAttribute("line_width","0.352778mm");
    l.setAttribute("line_color","0x000000ff");
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    return l;
}
