#include "paymentdialog.h"
#include "sessionmanager.h"
#include "databasemanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QDateEdit>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <stdexcept>

PaymentDialog::PaymentDialog(const QString &productName,
                             double price,
                             const QString &imagePath,
                             QWidget *parent)
    : QDialog(parent),
    currentProductName(productName),
    currentProductPrice(price),
    currentProductImage(imagePath)
{
    setWindowTitle("Payment Details");
    setFixedSize(1000, 600);

    // Main Layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(30);

    // Product Section (Left)
    productSection = new QWidget();
    productSection->setStyleSheet("background-color: #454b5a; border-radius: 10px; padding: 20px;");
    QVBoxLayout *productLayout = new QVBoxLayout(productSection);

    productImage = new QLabel();
    productImage->setPixmap(QPixmap(currentProductImage).scaled(200, 200, Qt::KeepAspectRatio));
    productImage->setAlignment(Qt::AlignCenter);
    productImage->setStyleSheet("border: 2px solid #767B91; border-radius: 10px;");

    productNameLabel = new QLabel(currentProductName);
    productNameLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold; margin-top: 15px;");

    productPriceLabel = new QLabel(QString("$%1").arg(currentProductPrice, 0, 'f', 2));
    productPriceLabel->setStyleSheet("color: #4CAF50; font-size: 24px; margin-top: 10px;");

    productLayout->addWidget(productImage);
    productLayout->addWidget(productNameLabel);
    productLayout->addWidget(productPriceLabel);
    productLayout->addStretch();

    mainLayout->addWidget(productSection, 40);  // 40% width

    // Payment Section (Right)
    paymentSection = new QWidget();
    QVBoxLayout *paymentLayout = new QVBoxLayout(paymentSection);

    // Payment Method
    QLabel *methodLabel = new QLabel("Select Payment Method:");
    paymentMethodCombo = new QComboBox();
    paymentMethodCombo->addItem("💳 Credit/Debit Card");
    paymentMethodCombo->addItem("🏦 Bank Transfer");

    // Card Information
    cardGroup = new QGroupBox("Card Information");
    QGridLayout *cardLayout = new QGridLayout(cardGroup);

    cardNumberEdit = new QLineEdit();
    cvvEdit = new QLineEdit();
    expirationDateEdit = new QDateEdit();
    expirationDateEdit->setCalendarPopup(true);
    expirationDateEdit->setDisplayFormat("MM/yyyy");

    cardNumberEdit->setPlaceholderText("1234 5678 9012 3456");
    cvvEdit->setPlaceholderText("123");

    cardLayout->addWidget(new QLabel("Card Number:"), 0, 0);
    cardLayout->addWidget(cardNumberEdit, 0, 1);
    cardLayout->addWidget(new QLabel("Expiration Date:"), 1, 0);
    cardLayout->addWidget(expirationDateEdit, 1, 1);
    cardLayout->addWidget(new QLabel("CVV:"), 2, 0);
    cardLayout->addWidget(cvvEdit, 2, 1);

    // Bank Transfer Information
    transferGroup = new QGroupBox("Bank Transfer Information");
    QVBoxLayout *transferLayout = new QVBoxLayout(transferGroup);

    accountNumberEdit = new QLineEdit();
    accountNumberEdit->setPlaceholderText("Enter account number");
    transferPasswordEdit = new QLineEdit();
    transferPasswordEdit->setPlaceholderText("Enter transfer password");
    transferPasswordEdit->setEchoMode(QLineEdit::Password);

    transferLayout->addWidget(new QLabel("Account Number:"));
    transferLayout->addWidget(accountNumberEdit);
    transferLayout->addWidget(new QLabel("Transfer Password:"));
    transferLayout->addWidget(transferPasswordEdit);

    transferGroup->setVisible(false); // Initially hidden

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    payButton = new QPushButton("Confirm Payment");
    cancelButton = new QPushButton("Cancel");

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(payButton);

    // Assemble payment section
    paymentLayout->addWidget(methodLabel);
    paymentLayout->addWidget(paymentMethodCombo);
    paymentLayout->addWidget(cardGroup);
    paymentLayout->addWidget(transferGroup);
    paymentLayout->addStretch();
    paymentLayout->addLayout(buttonLayout);

    mainLayout->addWidget(paymentSection, 60);  // 60% width

    // Connections
    connect(paymentMethodCombo, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        bool isCard = text.contains("Card");
        cardGroup->setVisible(isCard);
        transferGroup->setVisible(!isCard);
    });
    connect(payButton, &QPushButton::clicked, this, &PaymentDialog::processPayment);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void PaymentDialog::processPayment() {
    bool paymentAuthorized = false;
    QString errorMessage;
    QString paymentMethod = paymentMethodCombo->currentText();
    int paymentAttempts = 0;
    int accountId = -1;

    // Obtener el ID del usuario actual
    int userId = SessionManager::instance().getUserId();
    qDebug() << userId;
    if (userId == -1) {
        QMessageBox::critical(this, "Error", "Usuario no autenticado.");
        return;
    }

    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    db.transaction();

    try {
        if(paymentMethod.contains("Card")) {
            QString cardNumber = cardNumberEdit->text().remove(" ");
            QString cvv = cvvEdit->text();
            QDate expiration = expirationDateEdit->date();

            // Validación de tarjeta
            QSqlQuery cardQuery(db);
            cardQuery.prepare(
                "SELECT c.verified, c.expiration, c.cvv, a.id, a.verified, a.money "
                "FROM Card c "
                "JOIN Account a ON c.account_id = a.id "
                "WHERE c.number = :number AND a.user_id = :user_id"
                );
            cardQuery.bindValue(":number", cardNumber);
            cardQuery.bindValue(":user_id", userId);

            if (!cardQuery.exec() || !cardQuery.next()) {
                throw std::runtime_error("Tarjeta no encontrada o no pertenece al usuario");
            }
            if (!cardQuery.value(0).toBool()) {
                throw std::runtime_error("Tarjeta no verificada");
            }
            if (cardQuery.value(1).toDate() < QDate::currentDate()) {
                throw std::runtime_error("Tarjeta expirada");
            }
            if (cardQuery.value(2).toString() != cvv) {
                throw std::runtime_error("CVV incorrecto");
            }
            if (cardQuery.value(5).toDouble() < currentProductPrice) {
                throw std::runtime_error("Fondos insuficientes");
            }
            accountId = cardQuery.value(3).toInt();
        }
        else if(paymentMethod.contains("Bank Transfer")) {
            QString accountNumber = accountNumberEdit->text();
            QString transferPassword = transferPasswordEdit->text();

            // Validación de cuenta
            QSqlQuery accountQuery(db);
            accountQuery.prepare(
                "SELECT a.id, a.money, a.verified "
                "FROM Account a "
                "WHERE a.number = :number AND a.user_id = :user_id"
                );
            accountQuery.bindValue(":number", accountNumber);
            accountQuery.bindValue(":user_id", userId);

            if (!accountQuery.exec() || !accountQuery.next()) {
                throw std::runtime_error("Cuenta no encontrada o no pertenece al usuario");
            }
            if (!accountQuery.value(2).toBool()) {
                throw std::runtime_error("Cuenta no verificada");
            }
            // Validar contraseña de transferencia
            QSqlQuery transferQuery(db);
            transferQuery.prepare(
                "SELECT 1 FROM Transfers "
                "WHERE account_id = :account_id AND password = :password"
                );
            transferQuery.bindValue(":account_id", accountQuery.value(0).toInt());
            transferQuery.bindValue(":password", transferPassword);
            if (!transferQuery.exec() || !transferQuery.next()) {
                throw std::runtime_error("Contraseña de transferencia incorrecta");
            }
            if (accountQuery.value(1).toDouble() < currentProductPrice) {
                throw std::runtime_error("Fondos insuficientes");
            }
            accountId = accountQuery.value(0).toInt();
        }

        // Validar intentos previos
        QSqlQuery attemptsQuery(db);
        attemptsQuery.prepare(
            "SELECT attempts FROM Payments "
            "WHERE account_id = :account_id "
            "ORDER BY payment_date DESC LIMIT 1"
            );
        attemptsQuery.bindValue(":account_id", accountId);
        if (attemptsQuery.exec() && attemptsQuery.next()) {
            paymentAttempts = attemptsQuery.value(0).toInt();
            if (paymentAttempts >= 3) {
                throw std::runtime_error("Límite de intentos excedido");
            }
        }

        // Actualizar fondos en caso de transferencia
        if(paymentMethod.contains("Bank Transfer")) {
            QSqlQuery updateFunds(db);
            updateFunds.prepare(
                "UPDATE Account SET money = money - :amount "
                "WHERE id = :account_id"
                );
            updateFunds.bindValue(":amount", currentProductPrice);
            updateFunds.bindValue(":account_id", accountId);
            if (!updateFunds.exec()) {
                throw std::runtime_error("Error al actualizar fondos");
            }
        }

        // Registrar el pago
        QSqlQuery paymentQuery(db);
        paymentQuery.prepare(
            "INSERT INTO Payments ("
            "  payment_method_id, account_id, payment_date, "
            "  status_id, product_id, attempts"
            ") VALUES ("
            "  (SELECT id FROM Payment_Method WHERE name = :method),"
            "  :account_id, datetime('now'),"
            "  (SELECT id FROM Status WHERE name = 'Exitoso'),"
            "  (SELECT id FROM Product WHERE name = :product_name),"
            "  :attempts"
            ")"
            );
        paymentQuery.bindValue(":method", paymentMethod.contains("Card") ? "Tarjeta" : "Transferencia");
        paymentQuery.bindValue(":account_id", accountId);
        paymentQuery.bindValue(":product_name", currentProductName);
        paymentQuery.bindValue(":attempts", ++paymentAttempts);
        if (!paymentQuery.exec()) {
            throw std::runtime_error("Error al registrar el pago");
        }

        // Crear factura
        QSqlQuery invoiceQuery(db);
        invoiceQuery.prepare(
            "INSERT INTO Invoices ("
            "  payment_id, issue_date, status_id"
            ") VALUES ("
            "  :payment_id, datetime('now'),"
            "  (SELECT id FROM Status WHERE name = 'Exitoso')"
            ")"
            );
        invoiceQuery.bindValue(":payment_id", paymentQuery.lastInsertId());
        if (!invoiceQuery.exec()) {
            throw std::runtime_error("Error al generar factura");
        }
        paymentAuthorized = true;
        qDebug() << paymentAuthorized;
        db.commit();
    }
    catch (const std::exception &e) {
        db.rollback();
        errorMessage = e.what();

        // Registrar intento fallido
        if (accountId != -1) {
            QSqlQuery failQuery(db);
            failQuery.prepare(
                "INSERT INTO Payments ("
                "  payment_method_id, account_id, payment_date, "
                "  status_id, product_id, attempts"
                ") VALUES ("
                "  (SELECT id FROM Payment_Method WHERE name = :method),"
                "  :account_id, datetime('now'),"
                "  (SELECT id FROM Status WHERE name = 'fallido'),"
                "  (SELECT id FROM Product WHERE name = :product_name),"
                "  :attempts"
                ")"
                );
            failQuery.bindValue(":method", paymentMethod.contains("Card") ? "Tarjeta" : "Transferencia");
            failQuery.bindValue(":account_id", accountId);
            failQuery.bindValue(":product_name", currentProductName);
            failQuery.bindValue(":attempts", ++paymentAttempts);
            failQuery.exec();
        }
    }

    if (paymentAuthorized) {
        QMessageBox::information(this, "Pago", "Pago autorizado con éxito");
        accept();
    } else {
        QMessageBox::critical(this, "Error", QString("Pago no autorizado: %1").arg(errorMessage));
    }
}
