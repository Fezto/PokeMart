#include "databasemanager.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

// Implementación del Singleton
DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager() {
    // Obtener la ruta de almacenamiento de la base de datos en el mismo directorio que el ejecutable
    QString databasePath = QCoreApplication::applicationDirPath() + "/pokemart.db";

    // Ya no es necesario verificar si el archivo existe y copiarlo aquí,
    // ya que CMake lo copia automáticamente al directorio de compilación.

    // Configurar SQLite con la ruta accesible
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);

    // Verifica si la base de datos se abre correctamente
    if (!db.open()) {
        qDebug() << "Error al conectar con la base de datos:" << db.lastError().text();
    } else {
        qDebug() << "Conexión exitosa a la base de datos SQLite";
    }
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::connect() {
    if (!db.open()) {
        qDebug() << "Error al conectar con la base de datos:" << db.lastError().text();
        return false;
    }
    qDebug() << "Conexión exitosa a la base de datos SQLite";
    return true;
}

QSqlDatabase& DatabaseManager::getDatabase() {
    return db;
}
