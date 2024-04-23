#ifndef FORMNAKLPODT_H
#define FORMNAKLPODT_H

#include <QWidget>
#include <QSqlQueryModel>
#include "rels.h"

namespace Ui {
class FormNaklPodt;
}

class ModelNaklPodt : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelNaklPodt(QObject *parent=0);
    void refresh(QDate beg, QDate end, int id_type, int id_podt_type);
};

class ModelNaklPodtCont : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelNaklPodtCont(QObject *parent=0);
    void refresh(QDate dat, int id_type, int id_podt_type);
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
signals:
    void sigSum(QString sum);
};

class FormNaklPodt : public QWidget
{
    Q_OBJECT

public:
    explicit FormNaklPodt(QWidget *parent = 0);
    ~FormNaklPodt();

private:
    Ui::FormNaklPodt *ui;
    ModelNaklPodt *modelNaklPodt;
    ModelNaklPodtCont *modelNaklPodtCont;
    void loadSettings();
    void saveSettings();

private slots:
    void refreshNakl();
    void refreshCont(QModelIndex index);
    void printNakl();
};

#endif // FORMNAKLPODT_H
