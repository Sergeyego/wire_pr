#ifndef FORMPRESENCE_H
#define FORMPRESENCE_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlError>

namespace Ui {
class FormPresence;
}

class ModelPresence : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelPresence(QObject *parent = 0);
    void refresh(QDate date, bool bypart);
    QVariant data(const QModelIndex &index,int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:
    bool by_Part;

public slots:

signals:
    void sigUpd();

};

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
