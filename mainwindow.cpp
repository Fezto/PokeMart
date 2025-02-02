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
    CollapsibleSidebar *sidebar = new CollapsibleSidebar(centralWidget, 100, 15);

    // Agregar algunos botones de ejemplo al sidebar

    QSqlTableModel ProductCategory;
    ProductCategory.setTable("Product_Category");
    ProductCategory.select();

    for (int i = 0; i < ProductCategory.rowCount(); ++i) {
        QSqlRecord record = ProductCategory.record(i);

        QToolButton *btnInicio = new QToolButton;

        btnInicio->setIcon(QIcon(":/icons/manzana.png"));
        btnInicio->setText(record.value("name").toString());
        btnInicio->setIconSize(QSize(60, 60));
        btnInicio->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btnInicio->setCheckable(true);

        // Hacer que el botón ocupe todo el espacio disponible
        btnInicio->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        // Agregar el botón al sidebar
        sidebar->addWidget(btnInicio);
    }


    /* --- ProductContainer (Panel de productos) --- */

    // Panel que contendrá las tarjetas de productos
    QFrame *panel = new QFrame();
    panel->setStyleSheet("background-color: #f0f0f0; padding: 10px;");
    panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    FlowLayout *flowLayout = new FlowLayout(panel, 10, 50, 10); // Márgenes y espaciado
    flowLayout->setContentsMargins(10, 10, 10, 10);
    flowLayout->setAlignment(Qt::AlignCenter);


    // Agregar varias tarjetas de productos
    flowLayout->addWidget(new ProductCard("Producto 1", 100.00));
    flowLayout->addWidget(new ProductCard("Producto 2", 200.00));
    flowLayout->addWidget(new ProductCard("Producto 3", 150.00));
    flowLayout->addWidget(new ProductCard("Producto 1", 100.00));
    flowLayout->addWidget(new ProductCard("Producto 2", 200.00));
    flowLayout->addWidget(new ProductCard("Producto 3", 150.00));
    flowLayout->addWidget(new ProductCard("Producto 1", 100.00));
    flowLayout->addWidget(new ProductCard("Producto 2", 200.00));
    flowLayout->addWidget(new ProductCard("Producto 3", 150.00));


    // Hacer que el panel sea desplazable si hay muchas tarjetas
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
