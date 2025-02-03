#ifndef PRODUCTCARD_H
#define PRODUCTCARD_H

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "paymentdialog.h"

class ProductCard : public QFrame {
    Q_OBJECT

public:
    explicit ProductCard(const QString& productName, double price, QWidget *parent = nullptr);

private slots:
    void openPaymentDialog();

private:
    QString productName;
    double price;
    QPushButton* buyButton;
};

#endif // PRODUCTCARD_H
