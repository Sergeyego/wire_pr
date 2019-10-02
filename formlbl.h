#ifndef FORMLBL_H
#define FORMLBL_H

#include <QWidget>
#include "models.h"
#include "dbtablemodel.h"
#include "lblengine.h"

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
    void setComboBoxModel(QComboBox *c, DbRelation *r);
    DbTableModel *modelOtk;
    DbTableModel *modelNam;
    DbTableModel *modelVol;
    QString getNum(QComboBox *c);

private slots:
    void goLbl1();
};

#endif // FORMLBL_H
