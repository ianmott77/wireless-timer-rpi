import QtQuick 2.0
import QtQuick.Layouts 1.3

Rectangle {
    anchors.fill: parent
    Connections {
        target: Controller
        onInfoChanged: {
            deviceIDText.text = "ID: " + Arduino.getID()
            deviceTypeText.text = "Type: " + deviceTypeText.getType()
            devicePositionText.text = "Position: " + Arduino.getPosition()
            nextAddrText.text = "Next: " + Arduino.getNextAddress()
            prevAddrText.text = "Previous: " + Arduino.getPrevAddress()
        }
    }
    ColumnLayout {
        anchors.fill: parent
        Layout.fillHeight: true
        Layout.fillWidth: true
        Rectangle {
            id: deviceID
            Layout.alignment: Qt.AlignCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "transparent"
            Text {
                id: deviceIDText
                color: "#000000"
                text: "ID: " + Arduino.getID()
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                fontSizeMode: Text.Fit
                renderType: Text.NativeRendering
                font.pixelSize: 20
                font.family: "IBM Plex Sans"
                anchors.fill: parent
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
                color: "#000000"
                text: "Type: " + getType()
                font.pixelSize: 20
                fontSizeMode: Text.Fit
                renderType: Text.NativeRendering
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.family: "IBM Plex Sans"
                anchors.fill: parent
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
                color: "#000000"
                text: "Position: " + Arduino.getPosition()
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 20
                fontSizeMode: Text.Fit
                renderType: Text.NativeRendering
                font.family: "IBM Plex Sans"
                anchors.fill: parent
            }
        }
        Rectangle {
            id: nextAddr
            Layout.alignment: Qt.AlignCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "transparent"
            Text {
                id: nextAddrText
                color: "#000000"
                text: "Next:" + Arduino.getNextAddress()
                font.pixelSize: 20
                fontSizeMode: Text.Fit
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.family: "IBM Plex Sans"
                anchors.fill: parent
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
                color: "#000000"
                text: "Previous: " + Arduino.getPrevAddress()
                font.pixelSize: 20
                fontSizeMode: Text.Fit
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.family: "IBM Plex Sans"
                anchors.fill: parent
            }
        }
    }
}
