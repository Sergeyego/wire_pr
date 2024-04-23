#ifndef FORMREPORT_H
#define FORMREPORT_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlError>

namespace Ui {
class FormReport;
}

class ModelReport : public QSqlQueryModel
{
    Q_OBJECT
public:
    ModelReport(QObject *parent = 0);
    void refresh(QDate begDate, QDate endDate, bool bypart);
    QVariant data(const QModelIndex &index,int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:
    bool by_Part;
};

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
