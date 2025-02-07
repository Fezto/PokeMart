#include "productcard.h"
#include "paymentdialog.h"
#include <QDebug>

ProductCard::ProductCard(const QString& productName, const QString imagePath, double price, QWidget *parent)
    : QFrame(parent), productName(productName), price(price), imagePath(imagePath) {
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

    // Cargar la imagen correctamente
    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen desde:" << imagePath;
    } else {
        qDebug() << "Imagen cargada correctamente desde:" << imagePath;
    }
    productImage->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::FastTransformation));
    productImage->setScaledContents(false);
    productImage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    buyButton = new QPushButton("Buy");
    connect(buyButton, &QPushButton::clicked, this, &ProductCard::openPaymentDialog);

    layout->addWidget(nameLabel);
    layout->addWidget(priceLabel);
    layout->addWidget(productImage, 1);
    layout->addWidget(buyButton);
}

void ProductCard::openPaymentDialog() {
    PaymentDialog paymentDialog(productName, price, imagePath, this);
    paymentDialog.exec();
}
