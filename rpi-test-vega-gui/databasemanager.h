#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QDateTime>
#include "gozbis_definitions.h"

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    struct ConnectionInformation {
        QString driverName;
        QString connectionName;
        QString database;
        QString host;
        int port;
        QString username;
        QString password;

        ConnectionInformation() {
            driverName = QString();
            connectionName = QString();
            database = QString();
            host = QString();
            username = QString();
            password = QString();
        };
    };

    struct Event {
        QString worker;
        int timestamp;
        int value;

        Event() {
            worker = QString();
            timestamp = 0;
            value = 0;
        }
    };

    struct Channel {
        int setupID;
        int id;
        QString title;
        ChannelCategory category;
        ChannelSubCategory subCategory;
        ChannelParameter parameter;
        int logInterval;
        double offset;
        MeasurementUnit unit;

        Channel() {
            setupID = 0;
            id = 0;
            title = QString();
            category = ChannelCategory::ChannelCategory_Akarsu;
            subCategory = ChannelSubCategory::ChannelSubCategory_NoSubCategory;
            parameter = ChannelParameter::ChannelParameter_Unknown;
            logInterval = 0;
            offset = 0.0;
            unit = MeasurementUnit::MeasurementUnit_NoUnit;
        }
    };

    struct SensorData {
        int id;
        ChannelCategory channelCategory;
        ChannelSubCategory channelSubCategory;
        ChannelParameter channelParameter;
        ValueType valueType;
        int timestamp;
        double value;
        MeasurementUnit measurementUnit;
        int accumulatorPercent;
        int gsmSignalPercent;

        SensorData() {
            id = 0;
            channelCategory = ChannelCategory_Akarsu;
            channelSubCategory = ChannelSubCategory_NoSubCategory;
            channelParameter = ChannelParameter_WAL;
            valueType = ValueType_INS;
            timestamp = 0;
            value = 0;
            measurementUnit = MeasurementUnit_Meter;
            accumulatorPercent = 100;
            gsmSignalPercent = 100;
        }
    };

    struct GOZBISData
    {
        QString istCode;
        QString securtiyCode;
        int accumulatorPercent;
        int gsmSignalPercent;
        QString tag;
        QList<SensorData> sensors;

        GOZBISData() {
            istCode = QString();
            securtiyCode = QString();
            accumulatorPercent = 100;
            gsmSignalPercent = 100;
            tag = QString("INSDATA");
        }
    };

    explicit DatabaseManager(QObject *parent = nullptr);

    bool createConnection(ConnectionInformation info);
    bool removeConnection();

    bool connect();
    void disconnect();
    bool isConnected();
    bool beginTransaction();
    bool commit();
    bool rollback();
    bool isAddedDatabase();

    bool createTable(QString sql);
    bool createTables();
    bool alterTables();

    bool loadDefaults();
    QList<Channel> getChannels();

    bool saveData(int setup_id, int channel_id, ValueType value_type, QDateTime timestamp, double value);
    GOZBISData gozbisData();
    bool sentRecords(QList<int> ids);

private:
    ConnectionInformation currentConnectionInformation;
    bool addedDatabase;

signals:
};

#endif // DATABASEMANAGER_H
