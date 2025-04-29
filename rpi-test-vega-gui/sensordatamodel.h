#ifndef SENSORDATAMODEL_H
#define SENSORDATAMODEL_H

#include <QObject>
#include <QTimer>
#include <QList>
#include <QProcess>
#include "databasemanager.h"

class SensorDataModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float level READ level NOTIFY dataChanged)
    Q_PROPERTY(float supplyVoltage READ supplyVoltage NOTIFY dataChanged)
    Q_PROPERTY(float solarPanelVoltage READ solarPanelVoltage NOTIFY dataChanged)
    Q_PROPERTY(QString wlanIp READ wlanIp NOTIFY dataChanged)
    Q_PROPERTY(QString wwanIp READ wwanIp NOTIFY dataChanged)
    Q_PROPERTY(QString currentTime READ currentTime NOTIFY dataChanged)

public:
    explicit SensorDataModel(QObject *parent = nullptr);

    float level() const;
    float supplyVoltage() const;
    float solarPanelVoltage() const;
    QString wlanIp() const;
    QString wwanIp() const;
    QString currentTime() const;

public slots:
    void updateData(float sensor_mA, int distance_mm, float powerSupplyVolt, float solarPanelVolt);

private slots:
    void checkHourlySave();
    void saveHourlyAverage();
    void updateSystemInfo();

signals:
    void dataChanged();

private:
    float m_sensor_mA;
    float m_distance_m;
    float m_powerSupplyVolt;
    float m_solarPanelVolt;

    QList<float> m_levelList;
    QList<float> m_supplyVoltageList;

    QTimer *hourlyTimer;
    QTimer *systemInfoTimer;
    DatabaseManager *dbManager;

    QString m_wlanIp;
    QString m_wwanIp;
    QString m_currentTime;

    QString getIpAddress(const QString &interfaceName);
};

#endif // SENSORDATAMODEL_H
