#ifndef CHECKFORM_H
#define CHECKFORM_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class CheckForm;
}

class CheckForm : public QWidget
{
    Q_OBJECT

public:
    explicit CheckForm(QWidget *parent = 0);
    ~CheckForm();

private:
    Ui::CheckForm *ui;
private slots:
    void go();
};

#endif // CHECKFORM_H
