#ifndef FORMNAKLPODT_H
#define FORMNAKLPODT_H

#include <QWidget>

namespace Ui {
class FormNaklPodt;
}

class FormNaklPodt : public QWidget
{
    Q_OBJECT

public:
    explicit FormNaklPodt(QWidget *parent = 0);
    ~FormNaklPodt();

private:
    Ui::FormNaklPodt *ui;
};

#endif // FORMNAKLPODT_H
