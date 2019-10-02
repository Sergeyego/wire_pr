#ifndef FORMSHIP_H
#define FORMSHIP_H

#include <QWidget>
#include "modelship.h"
#include "editor.h"
#include "sertbuild.h"
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QProgressDialog>

namespace Ui {
class FormShip;
}

class FormShip : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormShip(QWidget *parent = 0);
    ~FormShip();
    
private:
    Ui::FormShip *ui;
    ModelShip *modelShip;
    ModelDataShip *modelDataShip;
    Editor *editor;
    SertBuild *sertificat;
    void loadSettings();
    void saveSettings();

private slots:
    void updateShip();
    void updateDataShip(QModelIndex shipIndex);
    void updateSertificat(QModelIndex shipDataIndex);
    void printAll();
    void pdfAll();
    void printAll(QPagedPaintDevice *printer);
    void multipagePdf();
};

#endif // FORMSHIP_H
