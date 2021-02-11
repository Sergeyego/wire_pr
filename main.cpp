#include "mainwindow.h"
#include <QApplication>
#include "db/dblogin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/wire.ico"));

    QString key="";
    if (argc>1){
        key=QString(argv[1]);
    }

    QPixmap logo(":/images/simb_labl.png");
    DbLogin d(QObject::tr("Производство проволоки"),logo);
    if (d.exec()!=QDialog::Accepted) exit(1);

    QDir dir(":fonts");
    QFileInfoList list = dir.entryInfoList();
    foreach (QFileInfo i, list) {
        QFontDatabase::addApplicationFont(i.absoluteFilePath());
    }

    MainWindow w(key);
    w.show();

    return a.exec();
}
