#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "collapsiblesidebar.h"
#include "productcard.h"
#include "flowlayout.h"

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
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* --- DBConnection --- */
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("pokemart");
    db.setUserName("root");
    db.setPassword("Spongebob400!");
    bool ok = db.open();

    if(ok){
        qDebug() << "¡Conexión exitosa!";
    }
    else {
        qDebug() << "Algo a salido mal al intentar la conexión a la base de datos";
    }

    // Crear widget central y asignarlo
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    /* --- CollapsibleSideBar --- */

    // Instanciar nuestro sidebar colapsable
    CollapsibleSidebar *sidebar = new CollapsibleSidebar(centralWidget, 120, 15);


    // Agregar algunos botones de ejemplo al sidebar

    QSqlTableModel ProductCategory;
    ProductCategory.setTable("Product_Category");
    ProductCategory.select();

    for (int i = 0; i < ProductCategory.rowCount(); ++i) {
        QSqlRecord record = ProductCategory.record(i);

        // Crear el botón
        QToolButton *btnInicio = new QToolButton;
        btnInicio->setCheckable(true);

        // Crear un layout vertical (QVBoxLayout) para organizar el icono y el texto
        QVBoxLayout *layout = new QVBoxLayout;
        layout->setAlignment(Qt::AlignCenter);  // Asegura que los elementos estén centrados

        // Crear un widget contenedor para el layout
        QWidget *container = new QWidget;
        container->setLayout(layout);

        // Agregar el icono y el texto al layout
        QLabel *iconLabel = new QLabel;
        iconLabel->setPixmap(QIcon(":/icons/manzana.png").pixmap(61, 61));  // Usar un QLabel para el icono
        layout->addWidget(iconLabel);

        QLabel *textLabel = new QLabel(record.value("name").toString());
        textLabel->setAlignment(Qt::AlignCenter);  // Asegura que el texto esté centrado
        layout->addWidget(textLabel);

        // Establecer el contenedor como widget del botón
        btnInicio->setLayout(layout);

        // Hacer que el botón ocupe todo el espacio disponible
        btnInicio->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // Agregar el botón al sidebar
        sidebar->addWidget(btnInicio);
    }



    /* --- ProductContainer (Panel de productos) --- */

    // Panel principal que contendrá los botones y las tarjetas
    QWidget *panel = new QWidget();
    panel->setStyleSheet("background-color: #454b5a;");

    panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Layout principal del panel (vertical)
    QVBoxLayout *panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(10, 10, 10, 10);
    panelLayout->setSpacing(15);

    // --- Sección de botones ---
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignRight);

    QPushButton *btnFiltrar = new QPushButton("Filtrar");
    QPushButton *btnOrdenar = new QPushButton("Ordenar");

    // Estilo para los botones
    QString buttonStyle =
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover { background-color: #45a049; }";

    btnFiltrar->setStyleSheet(buttonStyle);
    btnOrdenar->setStyleSheet(buttonStyle);

    buttonLayout->addWidget(btnFiltrar);
    buttonLayout->addWidget(btnOrdenar);

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
    delete ui;
}
