#ifndef FORMLBL_H
#define FORMLBL_H

#include <QWidget>
#include <QComboBox>
#include "rels.h"
#include "db/dbtablemodel.h"
#include "lblengine.h"
#include "db/dbcombobox.h"

namespace Ui {
class FormLbl;
}

class FormLbl : public QWidget
{
    Q_OBJECT

public:
    explicit FormLbl(QWidget *parent = 0);
    ~FormLbl();

private:
    Ui::FormLbl *ui;
    DbTableModel *modelOtk;
    DbTableModel *modelNam;
    DbTableModel *modelVol;
    QString getNum(QComboBox *c);
    void setComboBoxModel(DbComboBox *c, DbSqlRelation *r);

private slots:
    void goLbl1();
};

#endif // FORMLBL_H
