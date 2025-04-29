#include "sensorlogger.h"
#include <QDebug>

static int currentToDistance_mm(float current_mA) {
    if (current_mA < 4.0f) current_mA = 4.0f;
    if (current_mA > 20.0f) current_mA = 20.0f;
    return static_cast<int>((1.0f - (current_mA - 4.0f) / 16.0f) * 15000.0f);
}

SensorLogger::SensorLogger(const QString &spiDevice, QObject *parent)
    : QObject(parent)
{
    reader = new SensorReader(spiDevice);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SensorLogger::readSensors);
}

void SensorLogger::start()
{
    timer->start(3000); // 3 saniyede bir oku
}

void SensorLogger::stop()
{
    timer->stop();
}

void SensorLogger::readSensors()
{
    float sensor_mA = reader->readSensor();
    float powerSupplyVolt = reader->readPowerSupply();
    float solarPanelVolt = reader->readSolarPanel();
    int distance_mm = currentToDistance_mm(sensor_mA);

    emit newDataReady(sensor_mA, distance_mm, powerSupplyVolt, solarPanelVolt);
}
