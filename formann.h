#ifndef FORMANN_H
#define FORMANN_H

#include <QWidget>
#include "models_edt.h"
#include "db/dbmapper.h"
#include "db/dbtablemodel.h"
#include "modelwarehouse.h"
#include "dialogflt.h"

namespace Ui {
class FormAnn;
}

class FormAnn : public QWidget
{
    Q_OBJECT

public:
    explicit FormAnn(QWidget *parent = 0);
    ~FormAnn();

private:
    Ui::FormAnn *ui;
    ModelAnn *modelAnn;
    DbMapper *push;
    DbTableModel *modelCont;
    DbTableModel *modelMechSrc;
    DbTableModel *modelMechAnn;
    DbRelation *relBuht;
    ModelPodtMech *modelPodtMech;
    ModelPodtNext *modelPodtNext;
    ModelPodtPart *modelPodtPart;

private slots:
    void refresh();
    void refreshCont(int index);
    void refreshMech(QModelIndex index);
    void fltPodt();
};

#endif // FORMANN_H
