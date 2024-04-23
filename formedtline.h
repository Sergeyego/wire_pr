#ifndef FORMEDTLINE_H
#define FORMEDTLINE_H

#include <QWidget>
#include "db/dbtablemodel.h"
#include "rels.h"

namespace Ui {
class FormEdtLine;
}

class FormEdtLine : public QWidget
{
    Q_OBJECT

public:
    explicit FormEdtLine(QWidget *parent = 0);
    ~FormEdtLine();

private:
    Ui::FormEdtLine *ui;
    DbTableModel *modelLine;
};

#endif // FORMEDTLINE_H
