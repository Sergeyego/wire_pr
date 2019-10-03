#ifndef FORMPEREPACK_H
#define FORMPEREPACK_H

#include <QWidget>
#include "db/dbtablemodel.h"
#include "db/dbmapper.h"
#include "models.h"
#include "modelwarehouse.h"
#include <QTcpSocket>

namespace Ui {
class FormPerePack;
}

class FormPerePack : public QWidget
{
    Q_OBJECT

public:
    explicit FormPerePack(QWidget *parent = 0);
    ~FormPerePack();

private:
    Ui::FormPerePack *ui;
    ModelNakl *modelNakl;
    ModelPerepack *modelPerepack;
    DbMapper *push;

private slots:
    void refresh();
    void refreshData(int row);
    void setPartFilter(int index);
    void printNakl();
};

#endif // FORMPEREPACK_H
