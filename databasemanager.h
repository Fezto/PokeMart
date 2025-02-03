#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class DatabaseManager
{
public:
    static DatabaseManager& instance();  // Singleton

    bool connect();
    QSqlDatabase& getDatabase();

private:
    DatabaseManager();  // Constructor privado
    ~DatabaseManager();
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
