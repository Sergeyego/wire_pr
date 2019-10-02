#ifndef PGDIALOG_H
#define PGDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QtSql>

namespace Ui {
    class PgDialog;
}

class PgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PgDialog(const QString title, QWidget *parent = 0);
    ~PgDialog();

private:
    Ui::PgDialog *ui;
    QSqlDatabase db;

private slots:
    void dBconnect();
};

#endif // PGDIALOG_H
