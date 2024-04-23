#ifndef FORMPRESENCEPODT_H
#define FORMPRESENCEPODT_H

#include <QWidget>
#include "rels.h"

namespace Ui {
class FormPresencePodt;
}

class ModelPresencePodt : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelPresencePodt(QObject *parent = 0);
    void refresh(QDate date, bool bypart, int id_type);
    QVariant data(const QModelIndex &index,int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:
    bool by_Part;
    double sum;
    int colsum;
};

class FormPresencePodt : public QWidget
{
    Q_OBJECT

public:
    explicit FormPresencePodt(QWidget *parent = 0);
    ~FormPresencePodt();

private:
    Ui::FormPresencePodt *ui;
    ModelPresencePodt *modelPresence;

private slots:
    void upd();
    void save();
};

#endif // FORMPRESENCEPODT_H
