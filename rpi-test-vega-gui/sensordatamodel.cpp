#include "sensordatamodel.h"
#include <numeric>
#include <QDateTime>
#include <QRegularExpression>
#include <QDebug>

SensorDataModel::SensorDataModel(QObject *parent)
    : QObject(parent),
      m_sensor_mA(0.0f),
      m_distance_m(0.0f),
      m_powerSupplyVolt(0.0f),
      m_solarPanelVolt(0.0f)
{
    dbManager = new DatabaseManager("/var/lib/hidrolink/data.db", this);

    hourlyTimer = new QTimer(this);
    connect(hourlyTimer, &QTimer::timeout, this, &SensorDataModel::checkHourlySave);
    hourlyTimer->start(10000);

    systemInfoTimer = new QTimer(this);
    connect(systemInfoTimer, &QTimer::timeout, this, &SensorDataModel::updateSystemInfo);
    systemInfoTimer->start(1000);
}

float SensorDataModel::level() const { return m_distance_m; }
float SensorDataModel::supplyVoltage() const { return m_powerSupplyVolt; }
float SensorDataModel::solarPanelVoltage() const { return m_solarPanelVolt; }
QString SensorDataModel::wlanIp() const { return m_wlanIp; }
QString SensorDataModel::wwanIp() const { return m_wwanIp; }
QString SensorDataModel::currentTime() const { return m_currentTime; }

void SensorDataModel::updateData(float sensor_mA, int distance_mm, float powerSupplyVolt, float solarPanelVolt)
{
    m_sensor_mA = sensor_mA;
    m_distance_m = distance_mm / 1000.0f;
    m_powerSupplyVolt = powerSupplyVolt;
    m_solarPanelVolt = solarPanelVolt;

    emit dataChanged();

    m_levelList.append(m_distance_m);
    m_supplyVoltageList.append(m_powerSupplyVolt);
}

void SensorDataModel::checkHourlySave()
{
    QTime now = QTime::currentTime();
    if (now.minute() == 0 && now.second() < 10) {
        saveHourlyAverage();
    }
}

void SensorDataModel::saveHourlyAverage()
{
    if (m_levelList.isEmpty() || m_supplyVoltageList.isEmpty())
        return;

    float avgLevel = std::accumulate(m_levelList.begin(), m_levelList.end(), 0.0f) / m_levelList.size();
    float avgSupply = std::accumulate(m_supplyVoltageList.begin(), m_supplyVoltageList.end(), 0.0f) / m_supplyVoltageList.size();

    if (dbManager) {
        dbManager->insertMeasurement(avgLevel, avgSupply);
    }

    m_levelList.clear();
    m_supplyVoltageList.clear();
}

void SensorDataModel::updateSystemInfo()
{
    m_currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    m_wlanIp = getIpAddress("wlan0");
    m_wwanIp = getIpAddress("wwan0");

    emit dataChanged();
}

QString SensorDataModel::getIpAddress(const QString &interfaceName)
{
    QString ip;
    QProcess process;
    process.start("ip", QStringList() << "addr" << "show" << interfaceName);
    process.waitForFinished(1000);
    QString output = process.readAllStandardOutput();
    QRegularExpression regex("inet ([0-9.]+)");
    QRegularExpressionMatch match = regex.match(output);
    if (match.hasMatch()) {
        ip = match.captured(1);
    }
    return ip;
}
