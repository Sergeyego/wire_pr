#ifndef FORMREPORTPODT_H
#define FORMREPORTPODT_H

#include <QWidget>
#include "rels.h"

namespace Ui {
class FormReportPodt;
}

class ModelReportPodt : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelReportPodt(QObject *parent = 0);
    void refresh(QDate begDate, QDate endDate, int num, int id_type);
    QVariant data(const QModelIndex &index,int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:
    bool by_Part;
};

class FormReportPodt : public QWidget
{
    Q_OBJECT

public:
    explicit FormReportPodt(QWidget *parent = 0);
    ~FormReportPodt();

private:
    Ui::FormReportPodt *ui;
    ModelReportPodt *modelReport;

private slots:
    void upd();
    void save();
};

#endif // FORMREPORTPODT_H
