#ifndef DRAWING_H
#define DRAWING_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <math.h>

class Vol : public QWidget
{
    Q_OBJECT
    double dp;
    double dn;
public:
    Vol(QWidget *parent, QString n);
    QHBoxLayout* hbl;
    QLabel* num;
    QDoubleSpinBox* q;
    QDoubleSpinBox* d;
    QLineEdit* b;
    QLineEdit* qsum;
    void setdp(double diam);
    void setdn(double dna);
    void calc();
signals:
    void rupd();
public slots:
    void calcq();
};

class Drawing : public QWidget
{
    Q_OBJECT
public:
    explicit Drawing(QLineEdit *lineEditRoute, QWidget *parent = 0);
    ~Drawing();
private:
    QLineEdit *route;
    QSpinBox* skvo;
    QDoubleSpinBox* d0;
    QDoubleSpinBox* b0;
    Vol** mrc;
    int nV;
    QHBoxLayout* hbl;
    QVBoxLayout* vbl;
    QGridLayout* gbl;

private slots:
    void setKvo();
    void updv();

public slots:
    void loadRoute();
    
};

#endif // DRAWING_H
