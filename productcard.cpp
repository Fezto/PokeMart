#include "productcard.h"

ProductCard::ProductCard(const QString& productName, double price, QWidget *parent)
    : QFrame(parent)
{
    setFrameShape(QFrame::Box);
    setStyleSheet("background-color: white; border-radius: 10px; padding: 10px; margin-bottom: 10px;");




    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* nameLabel = new QLabel(QString("<b>%1</b>").arg(productName));
    QLabel* priceLabel = new QLabel(QString("$%1").arg(price, 0, 'f', 2));
    QLabel* productImage = new QLabel();
    productImage->setPixmap(QPixmap(":/icons/manzana.png"));
    productImage->setScaledContents(true);


    layout->addWidget(nameLabel);
    layout->addWidget(priceLabel);
    layout->addWidget(productImage);
}
