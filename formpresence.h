#ifndef FORMPRESENCE_H
#define FORMPRESENCE_H

#include <QWidget>
#include "modelpresence.h"

namespace Ui {
class FormPresence;
}

class FormPresence : public QWidget
{
    Q_OBJECT

public:
    explicit FormPresence(QWidget *parent = 0);
    ~FormPresence();

private:
    Ui::FormPresence *ui;
    ModelPresence *modelPresence;

private slots:
    void updPresence();
    void save();
};

#endif // FORMPRESENCE_H
