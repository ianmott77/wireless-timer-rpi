import QtQuick 2.0
import QtQuick.Layouts 1.3

Rectangle {
    width: 238
    Connections {
        target: Controller
        onInfoChanged: {
            deviceIDText.text = "ID: " + Arduino.getID()
            deviceTypeText.text = deviceTypeText.getType()
            devicePositionText.text = "P: " + Arduino.getPosition()
            nextAddrText.text = "->: " + Arduino.getNextAddress()
            prevAddrText.text = "<-: " + Arduino.getPrevAddress()
        }
    }
    color: "#b31d1818"
    anchors.fill: parent
    RowLayout {
        spacing: 4
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
                text: "ID:" + Arduino.getID()
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                fontSizeMode: Text.Fit
                renderType: Text.NativeRendering
                font.pixelSize: 8
                font.family: "IBM Plex Sans"

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
                font.pixelSize: 8
                fontSizeMode: Text.Fit
                renderType: Text.NativeRendering
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
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
                text: "P:" + Arduino.getPosition()
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 8
                fontSizeMode: Text.Fit
                renderType: Text.NativeRendering
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
                text: "->:" + Arduino.getNextAddress()
                font.pixelSize: 8
                fontSizeMode: Text.Fit
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
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
                text: "<-:" + Arduino.getPrevAddress()
                font.pixelSize: 8
                fontSizeMode: Text.Fit
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}
