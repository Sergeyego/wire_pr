#ifndef FORMNAKL_H
#define FORMNAKL_H

#include <QWidget>
#include "models.h"
#include "modelwarehouse.h"
#include <QTcpSocket>

namespace Ui {
class FormNakl;
}

class FormNakl : public QWidget
{
    Q_OBJECT

public:
    explicit FormNakl(QWidget *parent = 0);
    ~FormNakl();

private:
    Ui::FormNakl *ui;
    ModelNaklGen *modelNakl;
    ModelNaklGenCont *modelNaklCont;
    void loadSettings();
    void saveSettings();

private slots:
    void refreshNakl();
    void refreshCont(QModelIndex index);
    void printNakl();
};

#endif // FORMNAKL_H
