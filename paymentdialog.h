#ifndef PAYMENTDIALOG_H
#define PAYMENTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QGroupBox>

class PaymentDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PaymentDialog(const QString& productName,
                           double price,
                           const QString& imagePath,
                           QWidget *parent = nullptr);

private:
    void setupUI();
    void setupProductSection();
    void setupPaymentSection();
    void updatePaymentFields();
    void processPayment();

    // Product Section
    QWidget *productSection;
    QLabel *productImage;
    QLabel *productNameLabel;
    QLabel *productPriceLabel;

    // Payment Section
    QWidget *paymentSection;
    QComboBox *paymentMethodCombo;
    QLineEdit *cardNumberEdit;
    QLineEdit *cvvEdit;
    QDateEdit *expirationDateEdit;
    QLineEdit *accountNumberEdit;

    QPushButton *payButton;
    QPushButton *cancelButton;

    QString currentProductImage;
    QString currentProductName;
    double currentProductPrice;
};

#endif // PAYMENTDIALOG_H
