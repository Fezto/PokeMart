#ifndef PRODUCTCARD_H
#define PRODUCTCARD_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

class ProductCard : public QFrame
{
    Q_OBJECT
public:
    explicit ProductCard(const QString& productName, double price, QWidget *parent = nullptr);
};

#endif // PRODUCTCARD_H
