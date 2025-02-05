#include "databasemanager.h"

// Implementación del Singleton
DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager() {
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("pokemart3");
    db.setUserName("root");
    db.setPassword("Spongebob400!");
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
    qDebug() << "Conexión exitosa a la base de datos";
    return true;
}

QSqlDatabase& DatabaseManager::getDatabase() {
    return db;
}
