#ifndef FORMWIRE_H
#define FORMWIRE_H

#include <QWidget>
#include "models.h"
#include "db/dbtablemodel.h"

namespace Ui {
class FormWire;
}

class FormWire : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormWire(QWidget *parent = 0);
    ~FormWire();
    
private:
    Ui::FormWire *ui;
    DbTableModel *modelGost;
    DbTableModel *modelDiamSert;
    void loadSettings();
    void saveSettings();
private slots:
    void setCurrentIndex(QModelIndex index);
};

#endif // FORMWIRE_H
