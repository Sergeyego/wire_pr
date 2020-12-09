#ifndef FORMNAKLPODT_H
#define FORMNAKLPODT_H

#include <QWidget>
#include "modelwarehouse.h"
#include <QTcpSocket>

namespace Ui {
class FormNaklPodt;
}

class FormNaklPodt : public QWidget
{
    Q_OBJECT

public:
    explicit FormNaklPodt(QWidget *parent = 0);
    ~FormNaklPodt();

private:
    Ui::FormNaklPodt *ui;
    ModelNaklPodt *modelNaklPodt;
    ModelNaklPodtCont *modelNaklPodtCont;
    QSqlQueryModel *modelType;
    void loadSettings();
    void saveSettings();

private slots:
    void refreshNakl();
    void refreshCont(QModelIndex index);
    void printNakl();
};

#endif // FORMNAKLPODT_H
