import QtQuick 2.0
import QtQuick.Layouts 1.3

Rectangle {
    id: barWrapper
    width: 240
    x: 0
    y: 0
    height: 40
    color: "#cc636373"
    Connections {
        target: Controller
        onBatteryStatusChanged: {
            batteryIcon.level = level
            batteryIcon.updateBatteryColor()
        }
    }

    Connections {
        target: Arduino
        onSyncDone: {
            syncStatus.color = "green"
        }
    }

    RowLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        anchors.fill: parent
        Rectangle {
            id: rectangle
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "transparent"
            RowLayout {
                anchors.verticalCenter: parent.verticalCenter
                spacing: 5
                Layout.fillHeight: true
                Layout.fillWidth: true
                Text {
                    color: "#ffffff"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    text: "Sync: "
                    font.pixelSize: 20
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: "IBM Plex Sans"
                }
                Rectangle {
                    id: syncStatus
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    height: 25
                    width: 25
                    radius: 12.5
                    color: (Arduino.getNextAddress() === 255
                            && Arduino.getPrevAddress() === 255) || (Arduino.getNextAddress() === 127
                                                                     && Arduino.getPrevAddress() === 127) ? "red" : "green"
                }
            }
        }
        Battery {
            id: batteryIcon
            width: barWrapper.width / 5
            height: barWrapper.height - 10
        }
    }
}
