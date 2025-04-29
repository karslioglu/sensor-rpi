#include "databasemanager.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject{parent}
{
    addedDatabase = false;
}

bool DatabaseManager::createConnection(ConnectionInformation info)
{
    currentConnectionInformation = info;
    QSqlDatabase db = QSqlDatabase::addDatabase(info.driverName, info.connectionName);

    db.setHostName(info.host);
    db.setPort(info.port);
    db.setDatabaseName(info.database);
    db.setUserName(info.username);
    db.setPassword(info.password);

    addedDatabase = true;

    if(!db.open()) {
        qDebug("RecordDatabaseManager::createConnection: [%s] %s", db.connectionName().toStdString().c_str(), db.lastError().text().toStdString().c_str());
        qDebug("RecordDatabaseManager::createConnection: [%s] %s", db.connectionName().toStdString().c_str(), currentConnectionInformation.connectionName.toStdString().c_str());
        return false;
    }

    bool created = createTables();

    // The data db connection must be closed when it isn't used
    db.close();

    return created;
}

bool DatabaseManager::removeConnection()
{
    if(!addedDatabase)
        return true;
    {
        QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);
        if(!db.isValid()) {
            qDebug("RecordDatabaseManager::removeConnection: The %s named connection could not be removed. It's maybe already removed.", currentConnectionInformation.connectionName.toStdString().c_str());
            return false;
        }

        db.close();
    }

    QSqlDatabase::removeDatabase(currentConnectionInformation.connectionName);

    addedDatabase = false;

    return true;
}

bool DatabaseManager::connect()
{
    QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);

    if(db.isOpen())
        return true;

    return db.open();
}

void DatabaseManager::disconnect()
{
    QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);

    if(db.isOpen())
        db.close();
}

bool DatabaseManager::isConnected()
{
    QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);

    return db.isOpen();
}

bool DatabaseManager::beginTransaction()
{
    QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);

    return db.transaction();
}

bool DatabaseManager::commit()
{
    QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);

    return db.commit();
}

bool DatabaseManager::rollback()
{
    QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);

    return db.rollback();
}

bool DatabaseManager::isAddedDatabase()
{
    return this->addedDatabase;
}

bool DatabaseManager::createTable(QString sql)
{
    QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);

    QSqlQuery query(db);

    if(!query.exec(sql)) {
        qDebug("DatabaseManager::createTable: %s", query.lastError().text().toStdString().c_str());
        qDebug("DatabaseManager::createTable: %s", query.lastQuery().simplified().toStdString().c_str());
        return false;
    }

    return true;
}

bool DatabaseManager::createTables()
{
    QString sql;

    sql = QString(
                " CREATE TABLE IF NOT EXISTS station (            "
                "     setup_id      INTEGER,                      "
                "     station_code  TEXT NOT NULL,                "
                "     station_title TEXT NOT NULL,                "
                "     security_code TEXT,                         "
                "     latitude      NUMERIC NOT NULL DEFAULT 0.0, "
                "     longitude     NUMERIC NOT NULL DEFAULT 0.0, "
                "     region        INTEGER NOT NULL DEFAULT 0,   "
                "     basin         INTEGER NOT NULL DEFAULT 0,   "
                "     no            INTEGER NOT NULL DEFAULT 0,   "
                "     phone         TEXT,                         "
                "     PRIMARY KEY(setup_id AUTOINCREMENT));       "
        );

    if(!createTable(sql))
        return false;

    sql = QString(
                " CREATE TABLE IF NOT EXISTS channels (                "
                "     setup_id            INTEGER,                     "
                "     channel_id          INTEGER,                     "
                "     channel_title       INTEGER NOT NULL,            "
                "     channel_category    INTEGER NOT NULL DEFAULT 0,  "
                "     channel_subcategory INTEGER NOT NULL DEFAULT 0,  "
                "     channel_parameter   INTEGER NOT NULL DEFAULT 1,  "
                "     log_interval        INTEGER NOT NULL DEFAULT 60, "
                "     offset              INTEGER DEFAULT 0.0,         "
                "     unit                INTEGER NOT NULL DEFAULT 0,  "
                "     PRIMARY KEY(setup_id,channel_id)                 "
                " );                                                   "
        );

    if(!createTable(sql))
        return false;

    sql = QString(
                " CREATE TABLE IF NOT EXISTS data (                    "
                "     id                 INTEGER NOT NULL,             "
                "     setup_id           INTEGER,                      "
                "     channel_id         INTEGER,                      "
                "     value_type         INTEGER,                      "
                "     timestamp          INTEGER,                      "
                "     value              REAL,                         "
                "     battery_percentage INTEGER NOT NULL DEFAULT 100, "
                "     signal_strength    INTEGER NOT NULL DEFAULT 100, "
                "     PRIMARY KEY(id AUTOINCREMENT)                    "
                " );                                                   "
        );

    if(!createTable(sql))
        return false;

    return alterTables();

    //return loadDefaults();
}

bool DatabaseManager::alterTables()
{
    QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);
    QSqlQuery query(db);

    QString sql = QString("PRAGMA table_info(data)");

    if (!query.exec(sql) || !query.next()) {
        qDebug("DatabaseManager::alterTables: %s", query.lastError().text().toStdString().c_str());
        return false;
    }

    bool isFoundTheValueColumn = false;
    while (query.next()) {
        if (query.value("name").toString() == "is_sent") {
            isFoundTheValueColumn = true;
            break;
        }
    }

    if (!isFoundTheValueColumn) {
        sql = QString("ALTER TABLE data ADD COLUMN is_sent BOOLEAN NOT NULL DEFAULT 0");
        if (!query.exec(sql)) {
            qDebug("DatabaseManager::alterTables: %s", query.lastError().text().toStdString().c_str());
            return false;
        }
    }

    return true;
}

bool DatabaseManager::loadDefaults()
{
    QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);
    QSqlQuery query(db);

    QString sql = QString("SELECT COUNT(*) AS cnt FROM station");
    if(!query.exec(sql)) {
        qDebug("DatabaseManager::alterTables: %s", query.lastError().text().toStdString().c_str());
        return false;
    }

    int count = 0;
    if(query.next()) {
        count = query.value("cnt").toInt();
    }

    if(!count) {
        sql = QString("INSERT INTO station(station_code, station_title, security_code) VALUES('KARSTST', 'Kars Demo', 'qwerty')");
        if(!query.exec(sql)) {
            qDebug("DatabaseManager::alterTables: %s", query.lastError().text().toStdString().c_str());
            return false;
        }

        sql = QString("INSERT INTO channels(setup_id, channel_id, channel_title, channel_category, channel_subcategory, channel_parameter, unit) VALUES(1, 1, 'Seviye', 4, 1, 18, 5)");
        if(!query.exec(sql)) {
            qDebug("DatabaseManager::alterTables: %s", query.lastError().text().toStdString().c_str());
            return false;
        }

        sql = QString("INSERT INTO channels(setup_id, channel_id, channel_title, channel_category, channel_subcategory, channel_parameter, unit) VALUES(1, 2, 'Sicaklik', 4, 1, 20, 10)");
        if(!query.exec(sql)) {
            qDebug("DatabaseManager::alterTables: %s", query.lastError().text().toStdString().c_str());
            return false;
        }

        sql = QString("INSERT INTO channels(setup_id, channel_id, channel_title, channel_category, channel_subcategory, channel_parameter, unit) VALUES(1, 3, 'Iletkenlik', 4, 1, 6, 9)");
        if(!query.exec(sql)) {
            qDebug("DatabaseManager::alterTables: %s", query.lastError().text().toStdString().c_str());
            return false;
        }
    }

    return true;
}

QList<DatabaseManager::Channel> DatabaseManager::getChannels()
{
    QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);
    QSqlQuery query(db);
    QList<DatabaseManager::Channel> channels;

    QString sql = QString("SELECT * FROM channels");
    if(!query.exec(sql)) {
        qDebug("DatabaseManager::insertEvent: %s", query.lastError().text().toStdString().c_str());
        return channels;
    }

    while (query.next()) {
        DatabaseManager::Channel channel;
        channel.setupID = query.value("setup_id").toInt();
        channel.id = query.value("channel_id").toInt();
        channel.title = query.value("channel_title").toString();
        channel.category = ChannelCategory(query.value("channel_category").toInt());
        channel.subCategory = ChannelSubCategory(query.value("channel_subcategory").toInt());
        channel.parameter = ChannelParameter(query.value("channel_parameter").toInt());
        channel.logInterval = query.value("log_interval").toInt();
        channel.offset = query.value("offset").toDouble();
        channel.unit = MeasurementUnit(query.value("unit").toInt());

        channels.append(channel);
    }

    return channels;
}

bool DatabaseManager::saveData(int setup_id, int channel_id, ValueType value_type, QDateTime timestamp, double value)
{
    QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);
    QSqlQuery query(db);

    QString sql = QString("INSERT INTO data(setup_id, channel_id, value_type, timestamp, value) VALUES(%0, %1, %2, %3, %4)")
            .arg(setup_id)
            .arg(channel_id)
            .arg(value_type)
            .arg(timestamp.toSecsSinceEpoch())
            .arg(value);

    qDebug("saveData::sql: %s", sql.toStdString().c_str());

    if(!query.exec(sql)) {
        qDebug("DatabaseManager::saveData: %s", query.lastError().text().toStdString().c_str());
        return false;
    }

    return true;
}

DatabaseManager::GOZBISData DatabaseManager::gozbisData()
{
    GOZBISData data;
    QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);
    QSqlQuery query(db);

    QString sql = QString("SELECT station_code, security_code FROM station");
    if(!query.exec(sql)) {
        qDebug("DatabaseManager::gozbisData: %s", query.lastError().text().toStdString().c_str());
        return data;
    }

    if(query.next()) {
        data.istCode = query.value("station_code").toString();
        data.securtiyCode = query.value("security_code").toString();
    }
    /*
    sensor.append(sensorData.channelCategory);
    sensor.append(sensorData.channelSubCategory);
    sensor.append(sensorData.channelParameter);
    sensor.append(sensorData.valueType);
    sensor.append(sensorData.timestamp);
    sensor.append(sensorData.value);
    sensor.append(sensorData.measurementUnit);
    sensor.append(sensorData.accumulatorPercent);
    sensor.append(sensorData.gsmSignalPercent);
    */
    sql = QString(
                "    SELECT data.id, data.channel_id, data.value_type, data.timestamp, data.value, channels.channel_category, channels.channel_subcategory, channels.channel_parameter, channels.unit "
                "	  FROM data                                                                                                                                                              "
                " LEFT JOIN channels ON (channels.setup_id = data.setup_id AND channels.channel_id = data.channel_id)                                                                        "
                "     WHERE data.is_sent = 0                                                                                                                                                 "
                "	   AND data.value_type = 3                                                                                                                                               "
                );

    //qDebug("gozbisData::sql: %s", sql.simplified().toStdString().c_str());

    if(!query.exec(sql)) {
        qDebug("DatabaseManager::gozbisData: %s", query.lastError().text().toStdString().c_str());
        return data;
    }

    while (query.next()) {
        SensorData s = SensorData();

        s.id = query.value("id").toInt();
        int n = query.value("channel_category").toInt();
        s.channelCategory = ChannelCategory(n);
        s.channelSubCategory = ChannelSubCategory(query.value("channel_subcategory").toInt());
        s.channelParameter = ChannelParameter(query.value("channel_parameter").toInt());
        s.valueType = ValueType(query.value("value_type").toInt());
        s.timestamp = query.value("timestamp").toInt();
        s.value= query.value("value").toDouble();
        s.measurementUnit = MeasurementUnit(query.value("unit").toInt());

        data.sensors.append(s);
    }
    return data;
}

bool DatabaseManager::sentRecords(QList<int> ids)
{
    QSqlDatabase db = QSqlDatabase::database(currentConnectionInformation.connectionName);
    QSqlQuery query(db);

    QString sql = QString("UPDATE data SET is_sent=1 WHERE id IN (");
    bool first = true;
    foreach (int id, ids) {
        if(first) {
            first = false;
        } else {
            sql.append(", ");
        }
        sql.append(QString::number(id));
    }
    sql.append(")");

    qDebug("DatabaseManager::sentRecords: %s", sql.toStdString().c_str());

    if(!query.exec(sql)) {
        qDebug("DatabaseManager::gozbisData: %s", query.lastError().text().toStdString().c_str());
        return false;
    }

    return true;
}
