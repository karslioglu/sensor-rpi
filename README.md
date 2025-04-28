# Sensor HAT Test Application

Raspberry Pi 4 üzerinde çalışan sensör okuma ve veri gösterim uygulamaları.

Bu proje, Yocto Linux üzerinde çalışan bir sistemde 4-20mA çıkışlı sensörleri ve voltaj girişlerini okumak, konsolda veya QML arayüzü üzerinden görselleştirmek için geliştirilmiştir.

## Proje Yapısı

- `sensorreaderlib/`  
  MCP3008 ADC entegresi üzerinden veri okuyarak sensör verilerini sağlayan paylaşımlı kütüphane (.so).  
  Ortak sınıflar:  
  - `SensorReader` : MCP3008 üzerinden kanal okuma  
  - `SensorLogger` : Sensör verilerini belirli aralıklarla okuyup sinyal yayan sınıf

- `rpi-test-vega/`  
  Konsol uygulaması.  
  3 saniyede bir sensör verisi okuyarak terminal ekranına yazdırır.

- `rpi-test-vega-gui/`  
  Qt 6 QML GUI uygulaması.  
  Sensör verilerini görsel olarak bir arayüzde gösterir.

## Derleme Talimatları

Projeyi Qt6 ve qmake ile derleyebilirsiniz.

```bash
cd sensorreaderlib
qmake
make

cd ../rpi-test-vega
qmake
make

cd ../rpi-test-vega-gui
qmake
make
