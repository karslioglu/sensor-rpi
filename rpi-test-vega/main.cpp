#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include "sensorlogger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread *thread = new QThread;
    SensorLogger *logger = new SensorLogger("/dev/spidev0.0");

    logger->moveToThread(thread);

    QObject::connect(thread, &QThread::started, logger, &SensorLogger::start);
    QObject::connect(&a, &QCoreApplication::aboutToQuit, logger, &SensorLogger::stop);
    QObject::connect(&a, &QCoreApplication::aboutToQuit, thread, &QThread::quit);
    QObject::connect(thread, &QThread::finished, logger, &QObject::deleteLater);
    QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    QObject::connect(logger, &SensorLogger::newDataReady, [](float sensor_mA, int distance_mm, float powerSupplyVolt, float solarPanelVolt){
        qInfo() << "Sensor (mA):" << sensor_mA
                << "Distance (mm):" << distance_mm
                << "Power Supply (V):" << powerSupplyVolt
                << "Solar Panel (V):" << solarPanelVolt;
    });

    thread->start();

    return a.exec();
}
