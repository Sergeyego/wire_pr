#include "drawing.h"
#include <QDebug>

Vol::Vol(QWidget *parent, QString n) :
    QWidget(parent)
{
    hbl = new QHBoxLayout(this);
    num = new QLabel(this);
    QLabel* lq= new QLabel("Q=",this);
    QLabel* ld= new QLabel("D=",this);
    QLabel* qs= new QLabel("Qs=",this);
    q= new QDoubleSpinBox(this);
    d= new QDoubleSpinBox(this);
    qsum= new QLineEdit(this);
    q->setFixedWidth(85);
    d->setFixedWidth(90);
    qsum->setFixedWidth(60);
    qsum->setReadOnly(true);
    q->setRange(0,40);
    q->setDecimals(3);
    q->setSingleStep(0.01);
    q->setValue(0);
    d->setRange(0,10);
    d->setSingleStep(0.001);
    d->setDecimals(3);
    q->setSuffix(" %");
    d->setSuffix(" mm");
    hbl->addWidget(num);
    hbl->addWidget(lq);
    hbl->addWidget(q);
    hbl->addWidget(ld);
    hbl->addWidget(d);
    hbl->addWidget(qs);
    hbl->addWidget(qsum);
    num->setText(n);
    num->setFixedWidth(20);
    this->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    this->setLayout(hbl);
    connect(q,SIGNAL(valueChanged(double)),this,SIGNAL(rupd()));
    connect(d,SIGNAL(editingFinished()),this,SLOT(calcq()));
}

void Vol::calc()
{
    d->setValue(dp*sqrt(1-(q->value()*0.01)));
    double qs=(1-((d->value()*d->value())/(dn*dn)))*100;
    qsum->setText(QString::number(qs,0,3));
}

void Vol::setdp(double diam)
{
    dp=diam;
}

void Vol::setdn(double dna)
{
    dn=dna;
}


void Vol::calcq()
{
    q->setValue((1-(d->value()/dp)*(d->value()/dp))*100);
}


Drawing::Drawing(QLineEdit *lineEditRoute, QWidget *parent) :
    QWidget(parent)
{
    route=lineEditRoute;
    nV=18;
    hbl = new QHBoxLayout();
    vbl = new QVBoxLayout();
    gbl = new QGridLayout();
    QLabel* tkvo = new QLabel(tr("Кол-во волок"),this);
    QLabel* td0 = new QLabel(tr("Нач. диаметр"),this);
    this->setWindowTitle(tr("Расчёт маршрута волочения"));
    skvo = new QSpinBox(this);
    d0 = new QDoubleSpinBox(this);
    d0->setSingleStep(0.5);
    d0->setDecimals(3);
    d0->setRange(0,10);
    d0->setValue(5);
    d0->setSuffix(" mm");
    skvo->setRange(1,nV);
    skvo->setValue(10);
    gbl->addWidget(tkvo,0,0);
    gbl->addWidget(skvo,0,1);
    gbl->addWidget(td0,1,0);
    gbl->addWidget(d0,1,1);
    vbl->addLayout(gbl);
    mrc = new Vol*[nV];
    for (int i=0; i<nV; i++){
        mrc[i]= new Vol(this,QString::number(i+1));
        vbl->addWidget(mrc[i]);
    }
    hbl->addLayout(vbl);
    this->setLayout(hbl);
    tkvo->setFixedWidth(90);

    connect(skvo,SIGNAL(valueChanged(int)),this,SLOT(setKvo()));
    for (int i=0; i<nV; i++)
        connect(mrc[i],SIGNAL(rupd()),this,SLOT(updv()));
    connect(d0,SIGNAL(valueChanged(double)),this,SLOT(updv()));
    setKvo();
    updv();
}

Drawing::~Drawing()
{
    delete mrc;
    delete gbl;
    delete vbl;
    delete hbl;
}

void Drawing::setKvo()
{
    for (int i=0; i<nV; i++){
        mrc[i]->setVisible(i<skvo->value());
    }
    updv();
}

void Drawing::updv()
{
    mrc[0]->setdp(d0->value());
    mrc[0]->setdn(d0->value());
    mrc[0]->calc();
    for (int i=1; i</*skvo->value()*/nV; i++){
        mrc[i]->setdp(mrc[i-1]->d->value());
        mrc[i]->setdn(d0->value());
        mrc[i]->calc();
    }
    QString str;
    str+=QString::number(d0->value());
    for (int i=0; i<skvo->value(); i++){
        str+="->"+QString::number(mrc[i]->d->value());
    }
    route->setText(str);
}

void Drawing::loadRoute()
{
    QStringList volList;
    volList=route->text().split("->");
    //qDebug()<<volList;
    if (volList.size()>0){
        skvo->setValue(volList.size()-1);
        d0->setValue(volList.at(0).toDouble());
        for (int i=1; i<volList.size(); i++){
            mrc[i-1]->d->setValue(volList.at(i).toDouble());
            mrc[i-1]->calcq();
        }
    }
}
