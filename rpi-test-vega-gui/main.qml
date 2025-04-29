import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

ApplicationWindow {
    visible: true
    visibility: "FullScreen"
    title: qsTr("Hidrolink Sensor Dashboard")
    color: "#121212"

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        Label {
            text: "HIDROLINK SENSOR PANEL"
            font.pixelSize: 48
            font.bold: true
            color: "#00FFC5"
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 50

            // --- LEVEL ---
            Rectangle {
                width: 250
                height: 200
                radius: 20
                color: "#1E1E1E"
                border.color: "#00FFC5"
                border.width: 2

                Column {
                    anchors.centerIn: parent
                    spacing: 10

                    Text {
                        text: "Level"
                        font.pixelSize: 28
                        color: "#FFFFFF"
                    }
                    Text {
                        text: (sensorDataModel.level ?? 0.0).toFixed(2) + " m"
                        font.pixelSize: 40
                        color: "#00FFC5"
                    }
                }
            }

            // --- SUPPLY VOLTAGE ---
            Rectangle {
                width: 250
                height: 200
                radius: 20
                color: "#1E1E1E"
                border.color: "#FFC107"
                border.width: 2

                Column {
                    anchors.centerIn: parent
                    spacing: 10

                    Text {
                        text: "Supply Voltage"
                        font.pixelSize: 28
                        color: "#FFFFFF"
                    }
                    Text {
                        text: (sensorDataModel.supplyVoltage ?? 0.0).toFixed(2) + " V"
                        font.pixelSize: 40
                        color: "#FFC107"
                    }
                }
            }

            // --- SOLAR VOLTAGE ---
            Rectangle {
                width: 250
                height: 200
                radius: 20
                color: "#1E1E1E"
                border.color: "#FF3D00"
                border.width: 2

                Column {
                    anchors.centerIn: parent
                    spacing: 10

                    Text {
                        text: "Solar Voltage"
                        font.pixelSize: 28
                        color: "#FFFFFF"
                    }
                    Text {
                        text: (sensorDataModel.solarPanelVoltage ?? 0.0).toFixed(2) + " V"
                        font.pixelSize: 40
                        color: "#FF3D00"
                    }
                }
            }
        }

        RowLayout {
            spacing: 5
            Layout.alignment: Qt.AlignLeft

            Text {
                text: "" + sensorDataModel.currentTime
                font.pixelSize: 24
                color: "#00FFC5"
            }
            Text {
                text: "WLAN: " + (sensorDataModel.wlanIp.length > 0 ? sensorDataModel.wlanIp : "N/A")
                font.pixelSize: 24
                color: "#00FFC5"
            }
            Text {
                text: "LTE: " + (sensorDataModel.wwanIp.length > 0 ? sensorDataModel.wwanIp : "N/A")
                font.pixelSize: 24
                color: "#00FFC5"
            }
        }
    }
}
