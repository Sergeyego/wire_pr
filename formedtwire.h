#ifndef FORMEDTWIRE_H
#define FORMEDTWIRE_H

#include <QWidget>
#include "dbtablemodel.h"
#include "models.h"
#include "pushform.h"

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
    PushForm *mapper;
    DbTableModel *modelEan;
    DbTableModel *modelKr;

private slots:
    void updEan(int row);
};

#endif // FORMEDTWIRE_H
