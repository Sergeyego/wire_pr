#ifndef FORMPEREPACK_H
#define FORMPEREPACK_H

#include <QWidget>
#include "db/dbtablemodel.h"
#include "db/dbmapper.h"
#include "rels.h"

namespace Ui {
class FormPerePack;
}

class ModelNakl : public DbTableModel
{
    Q_OBJECT
public:
    ModelNakl(QObject *parent=0);
    void refresh(QDate beg, QDate end);
    bool insertRow(int row, const QModelIndex &parent);
};

class ModelPerepack : public DbTableModel
{
    Q_OBJECT
public:
    ModelPerepack(QObject *parent=0);
    void refresh(int id_nakl);
    bool submit();
};

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
    void printNakl();
};

#endif // FORMPEREPACK_H
