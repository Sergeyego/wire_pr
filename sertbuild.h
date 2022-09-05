#ifndef SERTBUILD_H
#define SERTBUILD_H

#include <QTextDocument>
#include <QtGui>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextCharFormat>
#include <QMessageBox>
#include "models.h"
#include "qr/qrencode.h"

struct sLang {
    QString rus;
    QString eng;
};

struct generalData {
    sLang adres;
    QString contact;
    sLang otk;
    QImage logo;
    QImage sign;
};

struct headData {
    int id_wparti;
    double netto;
    QString nomSert;
    QDate dateVidSert;
    QString nomPart;
    int yearPart;
    QDate datePart;
    QString nPlav;
    QString srcProv;
    QString prov;
    double diam;
    sLang spool;
    sLang poluch;
    bool is_cored;
};

struct chemData {
    QString name;
    QVariant value;
};

struct mechData {
    int id_cat;
    sLang nam_html;
    sLang sig_htlm;
    sLang prefix;
    QVariant value;
    QVariant value_max;
};

struct sertData {
    int id_ved;
    sLang doc_nam;
    sLang ved_nam;
    QString nom_doc;
    QDate date_doc;
    QString gr_tech_ust;
    int id_doc_t;
    sLang ved_short;
    QString grade_nam;
    int id_doc;
    bool en;
};

typedef QVector<chemData> cvData;
typedef QVector<mechData> mvData;
typedef QVector<sertData> svData;

class DataSert : public QObject
{
    Q_OBJECT

public:
    DataSert(QObject *parent = 0);
    void refresh(int id, bool is_ship=true, bool sample=false);

    const generalData* general();
    QString tu();
    const headData* head();
    const cvData* chem();
    sLang mechCategory(int id);
    const mvData* mech();
    const svData* sert();
    const QImage* qrCode();
    void setDocEn(int id_doc, bool en);
    void setDefaultDoc();

private:
    void refreshTu();
    void refreshChem();
    void refreshMech();
    void refreshSert();
    void refreshQR(int id, bool is_ship, bool sample=false);
    QStringList tuList;
    generalData gData;
    headData hData;
    cvData cData;
    QMap<int,sLang> mechCat;
    mvData mData;
    svData sData;
    QImage qr_code;
    QMap <int, bool> mapSert;

public slots:
    void refreshMechCategory();

signals:
    void sigRefresh();
};

class SertBuild : public QTextDocument
{
    Q_OBJECT
public:
    explicit SertBuild(QObject *parent = 0);
    QString getNomPart();
    QString getYearSert();
    QString getYearPart();
    QString getNomSert();
    bool getPrn();
    DataSert* sData();

public slots:
    void build(int id, bool is_ship);
    void rebuild();
    void setPrn(bool p);
    void setLRus(bool b);
    void setLEn(bool b);
    void setLMix(bool b);
    void setSample(bool b);
    void setDocEn(int id_doc, bool en);
    void setDefaultDoc();

private:
    QTextCursor* cursor;
    DataSert *data;
    bool prn;
    bool l_rus;
    bool l_en;
    bool sample;
    int current_id;
    bool current_is_ship;
    void insertText(QTextCursor &c, const QString &rus, const QString &eng, bool newpar=false, bool sep=true, bool html=false);
    void insertHtml(QTextCursor &c, const QString &html);
    void insertDouble(QTextCursor &c, const QVariant &val, int dec);
    void insertDate(QTextCursor &c, const QDate &date, bool newpar=true);
    
signals:
    void sigRefresh();

};


#endif // SERTBUILD_H
