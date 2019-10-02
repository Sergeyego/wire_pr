#include "mainwindow.h"
#include <QApplication>
#include "pgdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/wire.png"));

    QString key;
    if (argc>1){
        key=QString(argv[1]);
    }

    PgDialog d(QObject::tr("Производство проволоки"));
    if (d.exec()!=QDialog::Accepted) exit(1);

    MainWindow w(key);
    w.show();

    return a.exec();
}
