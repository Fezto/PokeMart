#include "productcard.h"

ProductCard::ProductCard(const QString& productName, double price, QWidget *parent)
    : QFrame(parent), productName(productName), price(price) {
    setFrameShape(QFrame::Box);
    setObjectName("productCard");
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMaximumSize(250, 400);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);

    QLabel* nameLabel = new QLabel(productName);
    nameLabel->setAlignment(Qt::AlignCenter);

    QLabel* priceLabel = new QLabel(QString("$%1").arg(price, 0, 'f', 2));
    priceLabel->setAlignment(Qt::AlignCenter);

    QLabel* productImage = new QLabel();
    productImage->setPixmap(QPixmap(":/icons/manzana.png").scaled(150, 150, Qt::KeepAspectRatio));
    productImage->setScaledContents(true);
    productImage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    buyButton = new QPushButton("Buy");
    connect(buyButton, &QPushButton::clicked, this, &ProductCard::openPaymentDialog);

    layout->addWidget(nameLabel);
    layout->addWidget(priceLabel);
    layout->addWidget(productImage, 1);
    layout->addWidget(buyButton);
}

void ProductCard::openPaymentDialog() {
    PaymentDialog paymentDialog(productName, price, ":/icons/manzana.png", this);
    if (paymentDialog.exec() == QDialog::Accepted) {
        // Aquí podrías agregar la lógica de compra confirmada
    }
}
