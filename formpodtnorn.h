#ifndef FORMPODTNORN_H
#define FORMPODTNORN_H

#include <QWidget>
#include "modelwarehouse.h"
#include "db/dbtablemodel.h"
#include "models.h"

namespace Ui {
class FormPodtNorn;
}

typedef struct
{
    int id_matr;
    double kvo;
    int id_vid;
} mnorm;

class FormPodtNorn : public QWidget
{
    Q_OBJECT

public:
    explicit FormPodtNorn(QWidget *parent = 0);
    ~FormPodtNorn();

private:
    Ui::FormPodtNorn *ui;
    ModelPodtProd *modelPodtProd;
    DbTableModel *modelNorm;
    QVector<mnorm> buf;

private slots:
    void upd();
    void updNorm(QModelIndex ind);
    void copy();
    void paste();
};

#endif // FORMPODTNORN_H
