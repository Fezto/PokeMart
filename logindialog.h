#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    QString getEmail() const;
    QString getPassword() const;

private slots:
    void handleLogin();

private:
    QLineEdit *emailField;
    QLineEdit *passwordField;
    QPushButton *loginButton;
};

#endif // LOGINDIALOG_H
