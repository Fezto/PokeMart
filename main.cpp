#include "mainwindow.h"
#include <QResource>
#include <QFile>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* --- Style implementation --- */

    QFile *styleFile = new QFile(":styles/style.qss");
    styleFile->open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleFile->readAll());

    a.setStyleSheet(styleSheet);

    /* --- App execution --- */

    MainWindow w;
    w.show();
    return a.exec();
}
