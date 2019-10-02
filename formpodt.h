#ifndef FORMPODT_H
#define FORMPODT_H

#include <QWidget>
#include "pushform.h"
#include "modelwarehouse.h"
#include "dbtablemodel.h"
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
    PushForm *push;
    void loadSettings();
    void saveSettings();
    ModelPodtCont *modelCont;
    ModelPodtMech *modelPodtMech;
    ModelPodtAnn *modelPodtAnn;

private slots:
    void refresh();
    void updPart(int index);
    void setContItogo(QString s);
    void fltSrc();
    void fltPodt();
};

#endif // FORMPODT_H
