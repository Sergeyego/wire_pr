#ifndef DIALOGOTK_H
#define DIALOGOTK_H

#include <QDialog>
#include "models.h"
#include "dbtablemodel.h"

namespace Ui {
class DialogOtk;
}

class DialogOtk : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOtk(QString vol, QString nam, QDate d, QWidget *parent = 0);
    ~DialogOtk();
    QString getNam();
    QString getOtk();
    QString getVol();
    QString getCod();
    bool barCode();
    bool getOpt();

private:
    Ui::DialogOtk *ui;
    QString getNum(QComboBox *c);
};

#endif // DIALOGOTK_H
