#ifndef DIALOGOTK_H
#define DIALOGOTK_H

#include <QDialog>
#include <QCompleter>
#include "models.h"
#include "db/dbtablemodel.h"

namespace Ui {
class DialogOtk;
}

class DialogOtk : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOtk(int id_p, QWidget *parent = 0);
    ~DialogOtk();
    QString getCod();
    bool barCode();

private:
    Ui::DialogOtk *ui;
    QString getNum(QComboBox *c);
};

#endif // DIALOGOTK_H
