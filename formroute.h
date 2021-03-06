#ifndef FORMROUTE_H
#define FORMROUTE_H

#include <QWidget>
#include "models.h"
#include "db/dbmapper.h"
#include "drawing.h"

namespace Ui {
class FormRoute;
}

class FormRoute : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormRoute(QWidget *parent = 0);
    ~FormRoute();
    
private:
    Ui::FormRoute *ui;
    DbMapper *push;
    Drawing *draw;
    DbTableModel *modelRoute;
    void loadSettings();
    void saveSettings();
};

#endif // FORMROUTE_H
