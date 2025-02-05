#include "logindialog.h"
#include "databasemanager.h"
#include "sessionmanager.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Login");
    setFixedSize(300, 180);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *labelEmail = new QLabel("Email:");
    emailField = new QLineEdit();
    emailField->setPlaceholderText("Enter your email");

    QLabel *labelPass = new QLabel("Password:");
    passwordField = new QLineEdit();
    passwordField->setPlaceholderText("Enter your password");
    passwordField->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login");


    layout->addWidget(labelEmail);
    layout->addWidget(emailField);
    layout->addWidget(labelPass);
    layout->addWidget(passwordField);
    layout->addWidget(loginButton);

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::handleLogin);
}

QString LoginDialog::getEmail() const {
    return emailField->text();
}

QString LoginDialog::getPassword() const {
    return passwordField->text();
}

void LoginDialog::handleLogin() {
    QSqlDatabase& db = DatabaseManager::instance().getDatabase();
    QSqlQuery query(db);

    query.prepare("SELECT id, email FROM User WHERE email = :email AND password = :password");
    query.bindValue(":email", getEmail());
    query.bindValue(":password", getPassword());

    if (query.exec() && query.next()) {
        int userId = query.value("id").toInt();
        QString email = query.value("email").toString();

        // Guardar en sesión
        SessionManager::instance().login(userId, email);

        QMessageBox::information(this, "Login exitoso", "¡Bienvenido! Has iniciado sesión correctamente.");
        accept();
    } else {
        QMessageBox::critical(this, "Error", "Email o contraseña incorrectos. Inténtalo de nuevo.");
    }
}
