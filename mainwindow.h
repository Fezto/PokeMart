#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
class MainWindow;

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateButtons(); // Función para actualizar los botones

private:
    QPushButton *btnLogin;
    QPushButton *btnCheck;
    QPushButton *btnLogout;
};
#endif // MAINWINDOW_H
