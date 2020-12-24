#ifndef FORMPRESENCEPODT_H
#define FORMPRESENCEPODT_H

#include <QWidget>
#include "modelpresence.h"

namespace Ui {
class FormPresencePodt;
}

class FormPresencePodt : public QWidget
{
    Q_OBJECT

public:
    explicit FormPresencePodt(QWidget *parent = 0);
    ~FormPresencePodt();

private:
    Ui::FormPresencePodt *ui;
    ModelPresencePodt *modelPresence;

private slots:
    void upd();
    void save();
};

#endif // FORMPRESENCEPODT_H
