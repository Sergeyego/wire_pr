#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSettings>
#include <QMdiSubWindow>
#include "olap/cubewidget.h"
#include "formedtwire.h"
#include "formedtpack.h"
#include "formedtline.h"
#include "formmatr.h"
#include "formpodt.h"
#include "formpresencepodt.h"
#include "formreportpodt.h"
#include "formfixpodt.h"
#include "formnaklpodt.h"
#include "formpodtnorm.h"
#include "formpart.h"
#include "formlbl.h"
#include "formperepack.h"
#include "formpresence.h"
#include "formreport.h"
#include "formmovwire.h"
#include "formnakl.h"
#include "formnorm.h"


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
    void edtLine();
    void edtMatr();
    void podtPart();
    void podtPresence();
    void podtReport();
    void podtFix();
    void podtNakl();
    void podtNorm();
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
};

#endif // MAINWINDOW_H
