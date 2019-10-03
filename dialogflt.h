#ifndef DIALOGFLT_H
#define DIALOGFLT_H

#include <QDialog>
#include "models.h"
#include "db/dbtablemodel.h"

namespace Ui {
class DialogFlt;
}

class DialogFlt : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFlt(QString nam, DbRelation *rel, QWidget *parent = 0);
    ~DialogFlt();

private:
    Ui::DialogFlt *ui;
    DbRelation *relation;

public slots:
    void accept();
};

#endif // DIALOGFLT_H
