#ifndef FORMEDTPACK_H
#define FORMEDTPACK_H

#include <QWidget>
#include "db/dbtablemodel.h"
#include "models.h"

namespace Ui {
class FormEdtPack;
}

class FormEdtPack : public QWidget
{
    Q_OBJECT

public:
    explicit FormEdtPack(QWidget *parent = 0);
    ~FormEdtPack();

private:
    Ui::FormEdtPack *ui;
    DbTableModel *modelPack;
    DbTableModel *modelPackType;
};

#endif // FORMEDTPACK_H
