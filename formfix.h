#ifndef FORMFIX_H
#define FORMFIX_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include "db/dbtablemodel.h"
#include "rels.h"

namespace Ui {
class FormFix;
}

class FormFix : public QWidget
{
    Q_OBJECT

public:
    explicit FormFix(QWidget *parent = nullptr);
    ~FormFix();

private:
    Ui::FormFix *ui;
    DbTableModel *modelFix;
    QSqlQueryModel *modelDat;

private slots:
    void updFix();
    void updFixData(int index);
    void fixNewOst();
    void delOst(QDate date);
};

#endif // FORMFIX_H
