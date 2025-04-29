#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager(const QString &path, QObject *parent)
    : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()) {
        qCritical() << "Failed to open database:" << db.lastError().text();
    } else {
        initialize();
    }
}

DatabaseManager::~DatabaseManager()
{
    if (db.isOpen())
        db.close();
}

void DatabaseManager::initialize()
{
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS measurements ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "timestamp TEXT NOT NULL, "
               "level REAL, "
               "supplyVoltage REAL)");
}

bool DatabaseManager::insertMeasurement(float level, float supplyVoltage)
{
    if (!db.isOpen())
        return false;

    QSqlQuery query(db);
    query.prepare("INSERT INTO measurements (timestamp, level, supplyVoltage) "
                  "VALUES (datetime('now'), ?, ?)");
    query.addBindValue(level);
    query.addBindValue(supplyVoltage);

    if (!query.exec()) {
        qCritical() << "Insert failed:" << query.lastError().text();
        return false;
    }
    return true;
}
