#include "sensordatamodel.h"

SensorDataModel::SensorDataModel(QObject *parent)
    : QObject(parent),
      m_sensor_mA(0.0f),
      m_distance_m(0.0f),
      m_powerSupplyVolt(0.0f),
      m_solarPanelVolt(0.0f)
{}

float SensorDataModel::sensor_mA() const { return m_sensor_mA; }
float SensorDataModel::distance_m() const { return m_distance_m; }
float SensorDataModel::powerSupplyVolt() const { return m_powerSupplyVolt; }
float SensorDataModel::solarPanelVolt() const { return m_solarPanelVolt; }

void SensorDataModel::updateData(float sensor_mA, int distance_mm, float powerSupplyVolt, float solarPanelVolt)
{
    m_sensor_mA = sensor_mA;
    m_distance_m = distance_mm / 1000.0f;
    m_powerSupplyVolt = powerSupplyVolt;
    m_solarPanelVolt = solarPanelVolt;
    emit dataChanged();
}
