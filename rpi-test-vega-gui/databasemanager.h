#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(const QString &path, QObject *parent = nullptr);
    ~DatabaseManager();

    bool insertMeasurement(float level, float supplyVoltage);

private:
    void initialize();
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
