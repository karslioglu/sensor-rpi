#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QDir>
#include <QDebug>
#include <QCursor>
#include "sensorlogger.h"
#include "sensordatamodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Mouse İmlecini Gizle
    QGuiApplication::setOverrideCursor(Qt::BlankCursor);

    // Hidrolink veritabanı dizini kontrolü
    QString dbDir = "/var/lib/hidrolink";
    QDir dir(dbDir);
    if (!dir.exists()) {
        if (dir.mkpath(".")) {
            qInfo() << "Created directory:" << dbDir;
        } else {
            qCritical() << "Failed to create directory:" << dbDir;
        }
    }

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

    // Mouse İmlecini Uygulama Kapanırken Geri Getir
    QObject::connect(&app, &QCoreApplication::aboutToQuit, []() {
        QGuiApplication::restoreOverrideCursor();
    });

    return app.exec();
}
