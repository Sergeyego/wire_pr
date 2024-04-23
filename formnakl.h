#ifndef FORMNAKL_H
#define FORMNAKL_H

#include <QWidget>
#include "rels.h"


namespace Ui {
class FormNakl;
}

class ModelNaklGen : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelNaklGen(QObject *parent=0);
    void refresh(QDate beg, QDate end, int id_type);
};

class ModelNaklGenCont : public QSqlQueryModel
{
    Q_OBJECT
public:
    QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;
    ModelNaklGenCont(QObject *parent=0);
    void refresh(QDate dat, int id_type);
signals:
    void sigSum(QString sum);
};

class FormNakl : public QWidget
{
    Q_OBJECT

public:
    explicit FormNakl(QWidget *parent = 0);
    ~FormNakl();

private:
    Ui::FormNakl *ui;
    ModelNaklGen *modelNakl;
    ModelNaklGenCont *modelNaklCont;
    void loadSettings();
    void saveSettings();

private slots:
    void refreshNakl();
    void refreshCont(QModelIndex index);
    void printNakl();
};

#endif // FORMNAKL_H
