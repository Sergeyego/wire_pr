#ifndef FORMREPORT_H
#define FORMREPORT_H

#include <QWidget>
#include "modelpresence.h"

namespace Ui {
class FormReport;
}

class FormReport : public QWidget
{
    Q_OBJECT

public:
    explicit FormReport(QWidget *parent = 0);
    ~FormReport();

private:
    Ui::FormReport *ui;
    ModelReport *modelReport;
    void loadSettings();
    void saveSettings();

private slots:
    void updReport();
    void save();
};

#endif // FORMREPORT_H
