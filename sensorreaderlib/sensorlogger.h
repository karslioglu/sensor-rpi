#ifndef SENSORLOGGER_H
#define SENSORLOGGER_H

#include <QObject>
#include <QTimer>
#include "sensorreader.h"

class SensorLogger : public QObject
{
    Q_OBJECT
public:
    explicit SensorLogger(const QString &spiDevice = "/dev/spidev0.0", QObject *parent = nullptr);

public slots:
    void start();
    void stop();

signals:
    void newDataReady(float sensor_mA, int distance_mm, float powerSupplyVolt, float solarPanelVolt);

private slots:
    void readSensors();

private:
    SensorReader *reader;
    QTimer *timer;
};

#endif // SENSORLOGGER_H
