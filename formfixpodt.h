#ifndef FORMFIXPODT_H
#define FORMFIXPODT_H

#include <QWidget>
#include "db/dbtablemodel.h"
#include "models.h"
#include <QSqlQueryModel>
#include "dialogflt.h"

namespace Ui {
class FormFixPodt;
}

class FormFixPodt : public QWidget
{
    Q_OBJECT

public:
    explicit FormFixPodt(QWidget *parent = 0);
    ~FormFixPodt();

private:
    Ui::FormFixPodt *ui;
    DbTableModel *modelFix;
    QSqlQueryModel *modelDat;

private slots:
    void updFix();
    void updFixData(int index);
    void setFltPodt();
    void fixNewOst();

};

#endif // FORMFIXPODT_H
