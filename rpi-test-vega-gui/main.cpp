#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include "sensorlogger.h"
#include "sensordatamodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    SensorDataModel dataModel;

    QThread *thread = new QThread;
    SensorLogger *logger = new SensorLogger("/dev/spidev0.0");

    logger->moveToThread(thread);

    QObject::connect(thread, &QThread::started, logger, &SensorLogger::start);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, logger, &SensorLogger::stop);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, thread, &QThread::quit);
    QObject::connect(thread, &QThread::finished, logger, &QObject::deleteLater);
    QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    QObject::connect(logger, &SensorLogger::newDataReady,
                     &dataModel, &SensorDataModel::updateData);

    thread->start();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("sensorDataModel", &dataModel);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
