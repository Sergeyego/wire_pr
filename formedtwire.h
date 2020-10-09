#ifndef FORMEDTWIRE_H
#define FORMEDTWIRE_H

#include <QWidget>
#include "db/dbtablemodel.h"
#include "models.h"
#include "models_edt.h"
#include "db/dbmapper.h"

namespace Ui {
class FormEdtWire;
}

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
