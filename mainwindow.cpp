#include "mainwindow.h"
#include "collapsiblesidebar.h"
#include "productcard.h"
#include "flowlayout.h"
#include "logindialog.h"
#include "databasemanager.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QScrollArea>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Crear widget central y asignarlo
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    /* --- Database connection --- */
    DatabaseManager::instance().connect();


    /* --- CollapsibleSideBar --- */

    CollapsibleSidebar *sidebar = new CollapsibleSidebar(centralWidget, 120, 15);

    QSqlTableModel ProductCategory;
    ProductCategory.setTable("Product_Category");
    ProductCategory.select();

    for (int i = 0; i < ProductCategory.rowCount(); ++i) {
        QSqlRecord record = ProductCategory.record(i);
        QString categoryName = record.value("name").toString();
        QIcon categoryIcon = QIcon(":/icons/manzana.png");
        sidebar->addCategoryButton(categoryName, categoryIcon, QSize(61, 61));
    }


    /* --- ProductContainer (Panel de productos) --- */

    // Panel principal que contendrá los botones y las tarjetas
    QWidget *panel = new QWidget();
    panel->setObjectName("panel");
    panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Layout principal del panel (vertical)
    QVBoxLayout *panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(10, 10, 10, 10);
    panelLayout->setSpacing(15);

    // --- Sección de botones ---
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignRight);

    QPushButton *btnLogin = new QPushButton("Login");
    QPushButton *btnRegister = new QPushButton("Register");

    // Estilo oscuro con bordes sutiles y efectos al pasar el mouse

    buttonLayout->addWidget(btnLogin);
    buttonLayout->addWidget(btnRegister);

    connect(btnLogin, &QPushButton::clicked, this, [this](){
        LoginDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            qDebug() << "Login successful!";
        }
    });

    // Agregar botones al panel
    panelLayout->addLayout(buttonLayout);


    // --- Contenedor de tarjetas ---
    QWidget *cardsContainer = new QWidget();
    cardsContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    FlowLayout *flowLayout = new FlowLayout(cardsContainer, 10, 15, 10);
    flowLayout->setContentsMargins(0, 0, 0, 0);  // Eliminar márgenes internos

    // Agregar tarjetas
    flowLayout->addWidget(new ProductCard("Producto 1", 100.00));
    flowLayout->addWidget(new ProductCard("Producto 2", 200.00));
    flowLayout->addWidget(new ProductCard("Producto 1", 100.00));
    flowLayout->addWidget(new ProductCard("Producto 2", 200.00));
    flowLayout->addWidget(new ProductCard("Producto 1", 100.00));
    flowLayout->addWidget(new ProductCard("Producto 2", 200.00));
    flowLayout->addWidget(new ProductCard("Producto 1", 100.00));
    flowLayout->addWidget(new ProductCard("Producto 2", 200.00));

    // Agregar contenedor de tarjetas al panel
    panelLayout->addWidget(cardsContainer);

    // Configurar el área de scroll
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(panel);
    scrollArea->setWidgetResizable(true);

    /* --- UNIR TODO AL FINAL --- */

    // Layout principal: Sidebar + Contenido
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(scrollArea);
}

MainWindow::~MainWindow()
{

}
