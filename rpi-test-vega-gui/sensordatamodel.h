#ifndef SENSORDATAMODEL_H
#define SENSORDATAMODEL_H

#include <QObject>

class SensorDataModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float sensor_mA READ sensor_mA NOTIFY dataChanged)
    Q_PROPERTY(float distance_m READ distance_m NOTIFY dataChanged)
    Q_PROPERTY(float powerSupplyVolt READ powerSupplyVolt NOTIFY dataChanged)
    Q_PROPERTY(float solarPanelVolt READ solarPanelVolt NOTIFY dataChanged)

public:
    explicit SensorDataModel(QObject *parent = nullptr);

    float sensor_mA() const;
    float distance_m() const;
    float powerSupplyVolt() const;
    float solarPanelVolt() const;

public slots:
    void updateData(float sensor_mA, int distance_mm, float powerSupplyVolt, float solarPanelVolt);

signals:
    void dataChanged();

private:
    float m_sensor_mA;
    float m_distance_m;
    float m_powerSupplyVolt;
    float m_solarPanelVolt;
};

#endif // SENSORDATAMODEL_H
