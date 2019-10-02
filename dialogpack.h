#ifndef DIALOGPACK_H
#define DIALOGPACK_H

#include <QDialog>
#include "models.h"

namespace Ui {
class DialogPack;
}

class DialogPack : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPack(QString dafault, QWidget *parent = 0);
    ~DialogPack();
    int getId();
    int getIdType();
    QString getText();

private slots:
    void selectPackType(int n);

private:
    Ui::DialogPack *ui;
};

#endif // DIALOGPACK_H
