#ifndef FORMEDTWIRE_H
#define FORMEDTWIRE_H

#include <QWidget>
#include "db/dbtablemodel.h"
#include "rels.h"
#include "db/dbmapper.h"

namespace Ui {
class FormEdtWire;
}

class ModelEan : public DbTableModel
{
    Q_OBJECT
public:
    ModelEan(QObject *parent=0);
    void refresh(int id_prov);
public slots:
    void updRels(QModelIndex index);
};

class FormEdtWire : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormEdtWire(QWidget *parent = 0);
    ~FormEdtWire();
    
private:
    Ui::FormEdtWire *ui;
    DbTableModel *modelWire;
    DbMapper *mapper;
    DbTableModel *modelChemTu;
    ModelEan *modelEan;
    DbTableModel *modelKr;
    DbTableModel *modelDim;
    DbTableModel *modelGost;
    void loadSettings();
    void saveSettings();

private slots:
    void updEan(int row);
};

#endif // FORMEDTWIRE_H
