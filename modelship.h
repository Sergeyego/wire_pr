#ifndef MODELSHIP_H
#define MODELSHIP_H

#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDate>
#include <QSqlError>
#include <QSqlQuery>
#include <QtPrintSupport/QPrintDialog>

class ModelShip : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelShip(QObject *parent = 0);
    void refresh(QDate begDate, QDate endDate);
    
signals:
    
public slots:
    
};

class ModelDataShip : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelDataShip(QObject *parent = 0);
    void refresh(int id_ship);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;

private:
    QMap <int,int> colorState;
    void refreshState(int id_ship);

signals:

public slots:

};

#endif // MODELSHIP_H
