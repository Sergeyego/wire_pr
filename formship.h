#ifndef FORMSHIP_H
#define FORMSHIP_H

#include <QWidget>
#include "editor.h"
#include "sertbuild.h"
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QProgressDialog>

namespace Ui {
class FormShip;
}

class ModelShip : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelShip(QObject *parent = 0);
    void refresh(QDate begDate, QDate endDate);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:

};

class ModelDataShip : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelDataShip(QObject *parent = 0);
    void refresh(int id_ship);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:

};

class FormShip : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormShip(QWidget *parent = 0);
    ~FormShip();
    
private:
    Ui::FormShip *ui;
    ModelShip *modelShip;
    ModelDataShip *modelDataShip;
    Editor *editor;
    SertBuild *sertificat;
    void loadSettings();
    void saveSettings();

private slots:
    void updateShip();
    void updateDataShip(QModelIndex shipIndex);
    void updateSertificat(QModelIndex index);
    void printAll();
    void pdfAll();
    void printAll(QPagedPaintDevice *printer);
    void multipagePdf();
};

#endif // FORMSHIP_H
