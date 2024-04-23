#ifndef FORMMATR_H
#define FORMMATR_H

#include <QWidget>
#include "db/dbtablemodel.h"
#include "rels.h"

namespace Ui {
class FormMatr;
}

class FormMatr : public QWidget
{
    Q_OBJECT

public:
    explicit FormMatr(QWidget *parent = 0);
    ~FormMatr();

private:
    Ui::FormMatr *ui;
    DbTableModel *modelMatr;
};

#endif // FORMMATR_H
