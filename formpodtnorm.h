#ifndef FORMPODTNORM_H
#define FORMPODTNORM_H

#include <QWidget>
#include "db/dbtablemodel.h"
#include "rels.h"
#include "olap/cubewidget.h"

namespace Ui {
class FormPodtNorm;
}

typedef struct
{
    int id_matr;
    double kvo;
    int id_vid;
    QDate d_beg;
    QDate d_end;
} mnorm;

class ModelPodtProd : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelPodtProd(QObject *parent);
    void refresh(QDate beg, QDate end, int id_type);
    QVariant data(const QModelIndex &item, int role) const;
    bool ready();
private:
    QStringList exList;
public slots:
    void updState();
};

class FormPodtNorm : public QWidget
{
    Q_OBJECT

public:
    explicit FormPodtNorm(QWidget *parent = 0);
    ~FormPodtNorm();

private:
    Ui::FormPodtNorm *ui;
    ModelPodtProd *modelPodtProd;
    DbTableModel *modelNorm;
    QVector<mnorm> buf;
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

#endif // FORMPODTNORM_H
