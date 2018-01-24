import QtQuick 2.0
import QtQuick.Layouts 1.3

Rectangle {
    Connections {
        target: Controller
        onInfoChanged: {
            deviceIDText.text = "Device ID: " + Arduino.getID()
            deviceTypeText.text = deviceTypeText.getType()
            devicePositionText.text = "Position: " + Arduino.getPosition()
            nextAddrText.text = "Next Device: " + Arduino.getNextAddress()
            prevAddrText.text = "Previos Device: " + Arduino.getPrevAddress()
        }
    }
    color: "#b31d1818"
    anchors.fill: parent
    RowLayout {
        anchors.fill: parent
        Rectangle {
            id: deviceID
            Layout.alignment: Qt.AlignCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "transparent"
            Text {
                id:deviceIDText
                color: "white"
                text: "Device ID: " + Arduino.getID()
            }
        }
        Rectangle {
            id: deviceType
            Layout.alignment: Qt.AlignCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "transparent"
            Text {
                id: deviceTypeText
                function getType() {
                    if (Arduino.getType() === 1) {
                        return "Starter"
                    } else if (Arduino.getType() === 2) {
                        return "Interval"
                    } else if (Arduino.getType() === 3) {
                        return "Monitor"
                    }
                }
                color: "white"
                text: getType()
            }
        }
        Rectangle {
            id: devicePos
            Layout.alignment: Qt.AlignCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "transparent"
            Text {
                id: devicePositionText
                color: "white"
                text: "Position: " + Arduino.getPosition()
            }
        }
        Rectangle {
            id: nextAddr
            Layout.alignment: Qt.AlignCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "transparent"
            Text {
                id:nextAddrText
                color: "white"
                text: "Next Device: " + Arduino.getNextAddress()
            }
        }
        Rectangle {
            id: prevAddr
            Layout.alignment: Qt.AlignCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "transparent"
            Text {
                id: prevAddrText
                color: "white"
                text: "Previos Device: " + Arduino.getPrevAddress()
            }
        }
    }
}
