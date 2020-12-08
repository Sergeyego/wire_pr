#ifndef FORMPRESENCEPODT_H
#define FORMPRESENCEPODT_H

#include <QWidget>

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
};

#endif // FORMPRESENCEPODT_H
