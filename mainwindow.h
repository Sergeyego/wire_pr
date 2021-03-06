#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "formpart.h"
#include "models.h"
#include "formroute.h"
#include "formship.h"
#include "formwarehouse.h"
#include "formedtwire.h"
#include "checkform.h"
#include "formperepack.h"
#include "formpresence.h"
#include "formreport.h"
#include "olap/cubewidget.h"
#include "formpodt.h"
#include "formann.h"
#include "formedtpack.h"
#include "formedtline.h"
#include "formlbl.h"
#include "formnakl.h"
#include "formfixpodt.h"
#include "formpresencepodt.h"
#include "formnaklpodt.h"
#include "formreportpodt.h"
#include "formmatr.h"
#include "formpodtnorn.h"
#include "formnorm.h"
#include <QMdiSubWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString k, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString key;

    void loadSettings();
    void saveSettings();
    bool exist(QObject *a);
    void actAction(QAction *a, void (MainWindow::*sl)());
    void addSubWindow(QWidget *w, QObject *a);
    bool setActiveSubWindow(QString t);
    QMap <QString,QAction*> actions;

private slots:
    void closeTab(int index);

    void edtWire();
    void edtSpool();
    void edtRoute();
    void edtLine();
    void edtMatr();
    void podtPart();
    void podtPresence();
    void podtReport();
    void podtFix();
    void podtNakl();
    void podtNorm();
    void wireAnn();
    void wirePart();
    void wireLbl();
    void perepack();
    void presence();
    void report();
    void warehouse();
    void nakl();
    void norm();
    void cubeNam();
    void cubePack();
    void cubePost();
    void cubeRas();
    void cubePerepack();
    void cubePodt();
    void cubePodtOut();
    void sert();
    void check();
};

#endif // MAINWINDOW_H
