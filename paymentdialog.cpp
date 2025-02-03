#include "paymentdialog.h"
#include <QGridLayout>
#include <QFile>

PaymentDialog::PaymentDialog(const QString& productName,
                             double price,
                             const QString& imagePath,
                             QWidget *parent)
    : QDialog(parent),
    currentProductName(productName),
    currentProductPrice(price),
    currentProductImage(imagePath)
{
    setupUI();
}

void PaymentDialog::setupUI() {
    setWindowTitle("Payment Details");
    setFixedSize(1000, 600);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(30);

    // Product Section (Left)
    setupProductSection();
    mainLayout->addWidget(productSection, 40); // 40% width

    // Payment Section (Right)
    setupPaymentSection();
    mainLayout->addWidget(paymentSection, 60); // 60% width
}

void PaymentDialog::setupProductSection() {
    productSection = new QWidget();
    productSection->setStyleSheet(
        "background-color: #454b5a;"
        "border-radius: 10px;"
        "padding: 20px;"
        );

    QVBoxLayout *productLayout = new QVBoxLayout(productSection);

    // Product Image
    productImage = new QLabel();
    productImage->setPixmap(QPixmap(currentProductImage).scaled(200, 200, Qt::KeepAspectRatio));
    productImage->setAlignment(Qt::AlignCenter);
    productImage->setStyleSheet("border: 2px solid #767B91; border-radius: 10px;");

    // Product Name
    productNameLabel = new QLabel(currentProductName);
    productNameLabel->setStyleSheet(
        "color: white;"
        "font-size: 20px;"
        "font-weight: bold;"
        "margin-top: 15px;"
        );

    // Product Price
    productPriceLabel = new QLabel(QString("$%1").arg(currentProductPrice, 0, 'f', 2));
    productPriceLabel->setStyleSheet(
        "color: #4CAF50;"
        "font-size: 24px;"
        "margin-top: 10px;"
        );

    productLayout->addWidget(productImage);
    productLayout->addWidget(productNameLabel);
    productLayout->addWidget(productPriceLabel);
    productLayout->addStretch();
}

void PaymentDialog::setupPaymentSection() {
    paymentSection = new QWidget();
    QVBoxLayout *paymentLayout = new QVBoxLayout(paymentSection);

    // Payment Method Selection
    QLabel *methodLabel = new QLabel("Select Payment Method:");
    methodLabel->setStyleSheet("color: white; font-size: 16px;");

    paymentMethodCombo = new QComboBox();
    paymentMethodCombo->addItem("💳 Credit/Debit Card");
    paymentMethodCombo->addItem("🏦 Bank Transfer");
    paymentMethodCombo->setStyleSheet(
        "QComboBox {"
        "   padding: 8px;"
        "   border-radius: 5px;"
        "   background-color: #5A5F73;"
        "   color: white;"
        "}"
        "QComboBox::drop-down { border: none; }"
        );

    // Card Details
    QGroupBox *cardGroup = new QGroupBox("Card Information");
    cardGroup->setStyleSheet(
        "QGroupBox {"
        "   color: white;"
        "   border: 1px solid #767B91;"
        "   border-radius: 8px;"
        "   margin-top: 10px;"
        "}"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; }"
        );

    QGridLayout *cardLayout = new QGridLayout(cardGroup);
    cardLayout->setContentsMargins(15, 25, 15, 15);
    cardLayout->setVerticalSpacing(12);

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

    // Bank Transfer Details
    accountNumberEdit = new QLineEdit();
    accountNumberEdit->setPlaceholderText("Enter account number");

    accountNumberEdit->hide();

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    payButton = new QPushButton("Confirm Payment");
    cancelButton = new QPushButton("Cancel");

    QString buttonStyle =
        "QPushButton {"
        "   padding: 10px 25px;"
        "   border-radius: 5px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover { opacity: 0.9; }";

    payButton->setStyleSheet(buttonStyle + "background-color: #4CAF50; color: white;");
    cancelButton->setStyleSheet(buttonStyle + "background-color: #5A5F73; color: white;");

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(payButton);

    // Assemble payment section
    paymentLayout->addWidget(methodLabel);
    paymentLayout->addWidget(paymentMethodCombo);
    paymentLayout->addWidget(cardGroup);
    paymentLayout->addWidget(accountNumberEdit);
    paymentLayout->addStretch();
    paymentLayout->addLayout(buttonLayout);

    // Connections
    connect(paymentMethodCombo, &QComboBox::currentTextChanged, this, &PaymentDialog::updatePaymentFields);
    connect(payButton, &QPushButton::clicked, this, &PaymentDialog::processPayment);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void PaymentDialog::updatePaymentFields() {
    bool isCard = paymentMethodCombo->currentText().contains("Card");
    dynamic_cast<QGroupBox*>(cardNumberEdit->parentWidget())->setVisible(isCard);
    accountNumberEdit->setVisible(!isCard);
}

void PaymentDialog::processPayment() {
    // Add validation logic here
    accept();
}
