#include "sensorreader.h"
#include "mcp3008reader.h"
#include "adcfilter.h"
#include <mutex>

class SensorReader::Impl
{
public:
    explicit Impl(const QString &device)
        : adc(device), currentFilter(0.2f) {}

    MCP3008Reader adc;
    ADCFilter currentFilter;
    std::mutex adcMutex;

    float adcToVoltage(int value) {
        return value * 3.3f / 1023.0f;
    }

    float adcToCurrent(int value) {
        float voltage = adcToVoltage(value);
        return voltage / 165.0f * 1000.0f;
    }
};

SensorReader::SensorReader(const QString &spiDevice)
{
    impl = new Impl(spiDevice);
}

SensorReader::~SensorReader()
{
    delete impl;
}

float SensorReader::readSensor()
{
    std::lock_guard<std::mutex> lock(impl->adcMutex);
    int ch0 = impl->adc.readChannel(0);
    if (ch0 < 0) return -1.0f;

    float current_mA = impl->adcToCurrent(ch0);
    return impl->currentFilter.filter(current_mA);
}

float SensorReader::readPowerSupply()
{
    std::lock_guard<std::mutex> lock(impl->adcMutex);
    int ch1 = impl->adc.readChannel(1);
    if (ch1 < 0) return -1.0f;

    float adcVolt = impl->adcToVoltage(ch1);
    return adcVolt * 7.8f;
}

float SensorReader::readSolarPanel()
{
    std::lock_guard<std::mutex> lock(impl->adcMutex);
    int ch2 = impl->adc.readChannel(2);
    if (ch2 < 0) return -1.0f;

    float adcVolt = impl->adcToVoltage(ch2);
    return adcVolt * 7.8f;
}
