#include <QVBoxLayout>
#include <QTableView>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include "paymenttable.h"
#include "databasemanager.h"

PaymentTable::PaymentTable(QWidget *parent) {
    setWindowTitle("Your Payments!");
    setMinimumHeight(200);
    setMinimumWidth(700);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QTableView *table = new QTableView(this);
    layout->addWidget(table);

    QSqlDatabase db = DatabaseManager::instance().getDatabase();

    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this, db);
    model->setTable("Payments");

    // Configurar relaciones
    model->setRelation(1, QSqlRelation("Payment_Method", "id", "name"));
    model->setRelation(2, QSqlRelation("Account", "id", "number"));
    model->setRelation(4, QSqlRelation("Status", "id", "name"));
    model->setRelation(5, QSqlRelation("Product", "id", "name"));

    // Configurar el modelo para que los datos sean de solo lectura
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->select();
    table->setModel(model);
    table->setItemDelegate(new QSqlRelationalDelegate(table));

    // Hacer la tabla de solo lectura
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Configurar encabezados
    model->setHeaderData(0, Qt::Horizontal, "Payment ID");
    model->setHeaderData(1, Qt::Horizontal, "Payment Method");
    model->setHeaderData(2, Qt::Horizontal, "Account Number");
    model->setHeaderData(3, Qt::Horizontal, "Amount");
    model->setHeaderData(4, Qt::Horizontal, "Status");
    model->setHeaderData(5, Qt::Horizontal, "Product");
    model->setHeaderData(6, Qt::Horizontal, "Payment Date");

    table->resizeColumnsToContents();
}
