#ifndef LBLENGINE_H
#define LBLENGINE_H

#include <QObject>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVariant>
#include <QDate>
#include <QSqlError>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QBuffer>
#include <QTextDocument>
#include <QTextCursor>
#include <QDir>
#include <QDesktopServices>

class LblEngine : public QObject
{
    Q_OBJECT
public:
    explicit LblEngine(QObject *parent = 0);
    void createLblEd(int id_part, QString cod, bool barcode);
    void createLblEd(QString text, QString barcode);
    void createLblGroup(int id_part);

private:
    QString lbldir;
    QMap <QString,QDomElement> map;
    QDomElement newImage(QString x, QString y, QString w, QString h, QString fnam, QDomDocument *doc);
    QDomElement newBarcode(QString x, QString y, QString w, QString h, QString code, QDomDocument *doc);
    QDomElement newText(QString x, QString y, QString w, QString h, QString text, int size, bool bold, QDomDocument *doc);
    QDomElement newRect(QString x, QString y, QString w, QString h, QDomDocument *doc);
    QDomElement newLine(QString x, QString y, QString dx, QString dy, QDomDocument *doc);
    void openSysFile(QString fname);

signals:

public slots:
};

#endif // LBLENGINE_H
