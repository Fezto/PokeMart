#include "paymentdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QDateEdit>
#include <QMessageBox>

PaymentDialog::PaymentDialog(const QString& productName,
                             double price,
                             const QString& imagePath,
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
    QWidget *productSection = new QWidget();
    productSection->setStyleSheet("background-color: #454b5a; border-radius: 10px; padding: 20px;");
    QVBoxLayout *productLayout = new QVBoxLayout(productSection);

    QLabel *productImage = new QLabel();
    productImage->setPixmap(QPixmap(currentProductImage).scaled(200, 200, Qt::KeepAspectRatio));
    productImage->setAlignment(Qt::AlignCenter);
    productImage->setStyleSheet("border: 2px solid #767B91; border-radius: 10px;");

    QLabel *productNameLabel = new QLabel(currentProductName);
    productNameLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold; margin-top: 15px;");

    QLabel *productPriceLabel = new QLabel(QString("$%1").arg(currentProductPrice, 0, 'f', 2));
    productPriceLabel->setStyleSheet("color: #4CAF50; font-size: 24px; margin-top: 10px;");

    productLayout->addWidget(productImage);
    productLayout->addWidget(productNameLabel);
    productLayout->addWidget(productPriceLabel);
    productLayout->addStretch();

    mainLayout->addWidget(productSection, 40);  // 40% width

    // Payment Section (Right)
    QWidget *paymentSection = new QWidget();
    QVBoxLayout *paymentLayout = new QVBoxLayout(paymentSection);

    // Payment Method
    QLabel *methodLabel = new QLabel("Select Payment Method:");
    QComboBox *paymentMethodCombo = new QComboBox();
    paymentMethodCombo->addItem("💳 Credit/Debit Card");
    paymentMethodCombo->addItem("🏦 Bank Transfer");

    // Card Information
    QGroupBox *cardGroup = new QGroupBox("Card Information");
    QGridLayout *cardLayout = new QGridLayout(cardGroup);

    QLineEdit *cardNumberEdit = new QLineEdit();
    QLineEdit *cvvEdit = new QLineEdit();
    QDateEdit *expirationDateEdit = new QDateEdit();
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

    // Bank Transfer Info
    QGroupBox *transferGroup = new QGroupBox("Bank Transfer Information");
    QVBoxLayout *transferLayout = new QVBoxLayout(transferGroup);

    QLineEdit *accountNumberEdit = new QLineEdit();
    accountNumberEdit->setPlaceholderText("Enter account number");
    QLineEdit *bankNameEdit = new QLineEdit();
    bankNameEdit->setPlaceholderText("Enter bank name");

    transferLayout->addWidget(new QLabel("Account Number:"));
    transferLayout->addWidget(accountNumberEdit);
    transferLayout->addWidget(new QLabel("Bank Name:"));
    transferLayout->addWidget(bankNameEdit);

    transferGroup->setVisible(false); // Initially hidden

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *payButton = new QPushButton("Confirm Payment");
    QPushButton *cancelButton = new QPushButton("Cancel");

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
    connect(paymentMethodCombo, &QComboBox::currentTextChanged, this, [cardGroup, transferGroup](const QString& text) {
        bool isCard = text.contains("Card");
        cardGroup->setVisible(isCard);
        transferGroup->setVisible(!isCard);
    });

    connect(payButton, &QPushButton::clicked, this, &PaymentDialog::processPayment);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void PaymentDialog::processPayment() {
    // Aquí puedes agregar la lógica de validación de los datos de pago
    accept();
}
