#include "mainwindow.h"
#include <QResource>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString globalStyle =
        "QPushButton {"
        "   background-color: #5A5F73;"
        "   color: white;"
        "   border: 1px solid #767B91;"
        "   padding: 8px 16px;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover { background-color: #6C7187; }"

        "QLineEdit, QDateEdit {"
        "   padding: 8px;"
        "   border: 1px solid #767B91;"
        "   border-radius: 5px;"
        "   background-color: #454b5a;"
        "   color: white;"
        "}"

        "QLabel {"
        "   color: white;"
        "   font-size: 12px;"
        "}"

        "QComboBox {"
        "   padding: 8px;"
        "   border-radius: 5px;"
        "   background-color: #5A5F73;"
        "   color: white;"
        "}"
        "QComboBox::drop-down {"
        "   subcontrol-origin: padding;"
        "   subcontrol-position: center right;"
        "   width: 20px;"
        "   border-left: 1px solid #767B91;"
        "}"
        "QComboBox::down-arrow {"
        "   image: url(:/icons/down-arrow.png);"
        "   width: 12px;"
        "   height: 12px;"
        "}"

        "QGroupBox {"
        "   color: white;"
        "   border: 2px solid #767B91;"
        "   border-radius: 8px;"
        "   margin-top: 10px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 5px;"
        "}";

    a.setStyleSheet(globalStyle);

    MainWindow w;
    w.show();
    return a.exec();
}
