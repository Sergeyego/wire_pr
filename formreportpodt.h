#ifndef FORMREPORTPODT_H
#define FORMREPORTPODT_H

#include <QWidget>

namespace Ui {
class FormReportPodt;
}

class FormReportPodt : public QWidget
{
    Q_OBJECT

public:
    explicit FormReportPodt(QWidget *parent = 0);
    ~FormReportPodt();

private:
    Ui::FormReportPodt *ui;
};

#endif // FORMREPORTPODT_H
