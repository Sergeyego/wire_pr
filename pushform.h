#ifndef PUSHFORM_H
#define PUSHFORM_H

#include <QWidget>
#include "dbtablemodel.h"
#include <QDataWidgetMapper>
#include "cbrelationdelegate.h"
#include <QTableView>

namespace Ui {
class PushForm;
}

class PushForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit PushForm(QAbstractItemView *v, QWidget *parent = 0);
    ~PushForm();
    void addLock(QWidget *widget);
    void addUnLock(QWidget *widget);
    void addEmptyLock(QWidget *widget);
    bool isLock();
    void addMapping(QWidget *widget, int section);
    int currentIndex();
    
private:
    Ui::PushForm *ui;
    QVector <QWidget*> lock1;
    QVector <QWidget*> lock2;
    QVector <QWidget*> lockEmpty;
    QDataWidgetMapper *mapper;
    QAbstractItemView *viewer;
    bool isEdt;

public slots:
    void slotNew();
    void slotEdt();
    void slotDel();
    void slotWrite();
    void slotEsc();
    void first();
    void setCurrentViewRow(int row);
    void lock(bool val);

private slots:
    void checkEmpty();

signals:
    void currentIndexChanged(int index);
};

#endif // PUSHFORM_H
