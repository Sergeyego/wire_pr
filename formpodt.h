#ifndef FORMPODT_H
#define FORMPODT_H

#include <QWidget>
#include "db/dbmapper.h"
#include "modelwarehouse.h"
#include "db/dbtablemodel.h"
#include "models.h"
#include "models_edt.h"
#include "dialogflt.h"

namespace Ui {
class FormPodt;
}

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
    //ModelPodtMech *modelPodtMech;
    ModelPodtAnn *modelPodtAnn;
    ModelPodtCex *modelPodtIn;
    ModelPodtCex *modelPodtOut;

private slots:
    void refresh();
    void updPart(int index);
    void setContItogo(QString s);
    void fltSrc();
    void fltPodt();
    void setInItogo(QString s);
    void setOutItogo(QString s);
};

#endif // FORMPODT_H
