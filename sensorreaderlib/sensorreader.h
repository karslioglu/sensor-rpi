#ifndef SENSORREADER_H
#define SENSORREADER_H

#include <QString>

class SensorReader
{
public:
    explicit SensorReader(const QString &spiDevice = "/dev/spidev0.0");
    ~SensorReader();

    float readSensor();        // CH0 → 4-20mA sensör
    float readPowerSupply();   // CH1 → Giriş voltajı
    float readSolarPanel();    // CH2 → Solar panel voltajı

private:
    class Impl;
    Impl* impl;
};

#endif // SENSORREADER_H
