#include "mainwindow.h"
#include "collapsiblesidebar.h"
#include "productcard.h"
#include "flowlayout.h"
#include "logindialog.h"
#include "databasemanager.h"
#include "sessionmanager.h"

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

    QSqlTableModel productCategory;
    productCategory.setTable("Product_Category");
    productCategory.select();

    for (int i = 0; i < productCategory.rowCount(); ++i) {
        QSqlRecord record = productCategory.record(i);
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

    // Crear botones
    btnLogin = new QPushButton("Login");
    btnRegister = new QPushButton("Register");
    btnLogout = new QPushButton("Logout");

    // Agregar botones
    buttonLayout->addWidget(btnLogin);
    buttonLayout->addWidget(btnRegister);
    buttonLayout->addWidget(btnLogout);

    // Conectar el botón de login
    connect(btnLogin, &QPushButton::clicked, this, [this]() {
        LoginDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            qDebug() << "Login successful!";
            updateButtons(); // Actualizar UI después de login
        }
    });

    // Conectar el botón de logout
    connect(btnLogout, &QPushButton::clicked, this, [this]() {
        SessionManager::instance().logout();
        qDebug() << "User logged out!";
        updateButtons(); // Actualizar UI después de logout
    });

    // Agregar layout de botones al panel
    panelLayout->addLayout(buttonLayout);

    // Ocultar o mostrar botones al inicio
    updateButtons();



    // --- Contenedor de tarjetas ---
    QWidget *cardsContainer = new QWidget();
    cardsContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    FlowLayout *flowLayout = new FlowLayout(cardsContainer, 10, 15, 10);
    flowLayout->setContentsMargins(0, 0, 0, 0);  // Eliminar márgenes internos


    QSqlTableModel product;
    product.setTable("Product");
    product.select();

    for (int i = 0; i < product.rowCount(); ++i) {
        QSqlRecord record = product.record(i);
        QString productName = record.value("name").toString();
        double price = record.value("price").toDouble();
        flowLayout->addWidget(new ProductCard(productName, price));
    }

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

void MainWindow::updateButtons() {
    bool loggedIn = SessionManager::instance().isUserLoggedIn();

    if (loggedIn) {
        btnLogin->hide();
        btnRegister->hide();
        btnLogout->show();
    } else {
        btnLogin->show();
        btnRegister->show();
        btnLogout->hide();
    }
}


MainWindow::~MainWindow()
{

}
