#ifndef FORMNORM_H
#define FORMNORM_H

#include <QWidget>
#include "models.h"
#include "modelwarehouse.h"
#include "olap/cubewidget.h"

namespace Ui {
class FormNorm;
}

typedef struct
{
    int id_matr;
    double kvo;
    int id_vid;
} pnorm;

class FormNorm : public QWidget
{
    Q_OBJECT

public:
    explicit FormNorm(QWidget *parent = 0);
    ~FormNorm();

private:
    Ui::FormNorm *ui;
    ModelProd *modelProd;
    DbTableModel *modelNorm;
    QVector<pnorm> buf;
    QVariant currentData(int column);
    int getIdType();
    bool ready();

private slots:
    void upd();
    void updNorm(QModelIndex ind);
    void copy();
    void paste();
    void report();
    void save();
    void calcSum();
};

#endif // FORMNORM_H
