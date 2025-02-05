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
#include <QString>

class PaymentDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PaymentDialog(const QString &productName,
                           double price,
                           const QString &imagePath,
                           QWidget *parent = nullptr);

private slots:
    void processPayment();

private:
    void setupUI();

    // Sección de Producto
    QWidget   *productSection;
    QLabel    *productImage;
    QLabel    *productNameLabel;
    QLabel    *productPriceLabel;

    // Sección de Pago
    QWidget   *paymentSection;
    QComboBox *paymentMethodCombo;
    QGroupBox *cardGroup;
    QLineEdit *cardNumberEdit;
    QLineEdit *cvvEdit;
    QDateEdit *expirationDateEdit;
    QGroupBox *transferGroup;
    QLineEdit *accountNumberEdit;
    QLineEdit *transferPasswordEdit;

    QPushButton *payButton;
    QPushButton *cancelButton;

    // Datos del producto actual
    QString currentProductImage;
    QString currentProductName;
    double  currentProductPrice;
};

#endif // PAYMENTDIALOG_H
