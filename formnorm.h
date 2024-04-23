#ifndef FORMNORM_H
#define FORMNORM_H

#include <QWidget>
#include "rels.h"
#include "olap/cubewidget.h"

namespace Ui {
class FormNorm;
}

typedef struct
{
    int id_matr;
    double kvo;
    int id_vid;
    QDate d_beg;
    QDate d_end;
} pnorm;

class ModelProd : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelProd(QObject *parent);
    void refresh(QDate beg, QDate end, int id_type);
    QVariant data(const QModelIndex &item, int role) const;
    bool ready();
private:
    QStringList exList;
public slots:
    void updState();
};

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
