import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "Sensor Data Viewer"

    Column {
        anchors.centerIn: parent
        spacing: 10

        Text {
            text: "Sensor Current: " + sensorDataModel.sensor_mA.toFixed(2) + " mA"
        }
        Text {
            text: "Distance: " + sensorDataModel.distance_m.toFixed(2) + " m"
        }
        Text {
            text: "Power Supply: " + sensorDataModel.powerSupplyVolt.toFixed(2) + " V"
        }
        Text {
            text: "Solar Panel: " + sensorDataModel.solarPanelVolt.toFixed(2) + " V"
        }
    }
}
