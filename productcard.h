#ifndef PRODUCTCARD_H
#define PRODUCTCARD_H

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class ProductCard : public QFrame {
    Q_OBJECT

public:
    explicit ProductCard(const QString& productName, const QString imagePath, double price, QWidget *parent = nullptr);

private slots:
    void openPaymentDialog();

private:
    QString productName;
    double price;
    QString imagePath;
    QPushButton* buyButton;
};

#endif // PRODUCTCARD_H
