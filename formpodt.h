#ifndef FORMPODT_H
#define FORMPODT_H

#include <QWidget>
#include "db/dbmapper.h"
#include "db/dbtablemodel.h"
#include "rels.h"

namespace Ui {
class FormPodt;
}

class ModelPodtPart : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelPodtPart(QObject *parent=0);
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
public slots:
    void refresh(int id_podt);
};

class ModelPodtCont : public DbTableModel
{
    Q_OBJECT
public:
    ModelPodtCont(QObject *parent=0);
    void refresh(int id_part);
signals:
    void sigSum(QString sum);
private slots:
    void calcSum();
};

class ModelPodtVol : public DbTableModel
{
    Q_OBJECT
public:
    ModelPodtVol(QObject *parent=0);
    void refresh(int id_part);
signals:
    void sigSum(QString sum);
private slots:
    void calcSum();
};

class ModelPodtCex : public DbTableModel
{
    Q_OBJECT
public:
    ModelPodtCex(QObject *parent=0);
    void refresh(int id_podt, int id_op);
private slots:
    void calcSum();
signals:
    void sigSum(QString sum);
};

class ModelPodt : public DbTableModel
{
    Q_OBJECT
public:
    ModelPodt(QObject *parent=0);
    void refresh(QDate beg, QDate end, int id_mark, int id_diam, int id_type);
    bool insertRow(int row, const QModelIndex &parent);
};

class FormPodt : public QWidget
{
    Q_OBJECT

public:
    explicit FormPodt(QWidget *parent = 0);
    ~FormPodt();

private:
    Ui::FormPodt *ui;
    ModelPodt *modelPodt;
    ModelPodtPart *modelPodtPart;
    DbMapper *push;
    void loadSettings();
    void saveSettings();
    ModelPodtCont *modelCont;
    ModelPodtVol *modelPodtVol;
    ModelPodtCex *modelPodtIn;
    ModelPodtCex *modelPodtOut;
    ModelPodtCex *modelPodtDef;

private slots:
    void refresh();
    void fltChanged(QString s);
    void updPart(int index);
    void setContItogo(QString s);
    void setVolItogo(QString s);
    void setInItogo(QString s);
    void setOutItogo(QString s);
    void setDefItogo(QString s);
};

#endif // FORMPODT_H
