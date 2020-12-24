#ifndef FORMREPORTPODT_H
#define FORMREPORTPODT_H

#include <QWidget>
#include "modelpresence.h"

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
    ModelReportPodt *modelReport;

private slots:
    void upd();
    void save();
};

#endif // FORMREPORTPODT_H
