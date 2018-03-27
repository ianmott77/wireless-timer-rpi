import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

Rectangle {
    property  var keyboard
    width: 238
    height: 180
    Rectangle {
        width: parent.parent.width
        height: 145
        color: "transparent"
        ColumnLayout {
            height: 140
            spacing: 5
            anchors.bottomMargin: -35
            anchors.fill: parent
            Rectangle {
                id: addDevicePosWrapper
                color: "#00000000"
                radius: 5
                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                width: parent.width
                Text {
                    anchors.fill: parent
                    text: "Device Position"
                    fontSizeMode: Text.Fit
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 30
                    font.family: "IBM Plex Sans"
                }
            }
            Rectangle {
                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                width: parent.width
                id: addDevicePosInput
                color: "#b3000000"
                radius: 2
                TextInput {
                    id: devicePosition
                    anchors.fill: parent
                    focus: true
                    font.pixelSize: 25
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    inputMethodHints: Qt.ImhDigitsOnly
                    font.family: "IBM Plex Sans"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if(keyboard.textIn)
                                keyboard.textIn.parent.border.width = 0
                            keyboard.textIn = devicePosition
                            keyboard.textIn .parent.border.width = 3
                            keyboard.textIn .parent.border.color = "green"
                        }
                    }
                }
            }
            Rectangle {
                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                width: parent.width
                id: addDeviceIDWrapper
                color: "#00000000"
                radius: 5
                border.color: "#00000000"
                Text {
                    anchors.fill: parent
                    text: "Device ID"
                    fontSizeMode: Text.Fit
                    elide: Text.ElideNone
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 30
                    verticalAlignment: Text.AlignVCenter
                    font.family: "IBM Plex Sans"

                }
            }
            Rectangle {
                id: addDeviceIDInput
                color: "#b3000000"
                radius: 5
                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                width: parent.width
                TextInput {
                    id: deviceID
                    anchors.fill: parent
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 25
                    inputMethodHints: Qt.ImhDigitsOnly
                    font.family: "IBM Plex Sans"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if(keyboard.textIn)
                                keyboard.textIn.parent.border.width = 0
                            keyboard.textIn = deviceID
                            keyboard.textIn .parent.border.width = 3
                            keyboard.textIn .parent.border.color = "green"
                        }
                    }
                }
            }
            Rectangle {
                id: okayButton
                Layout.alignment: Qt.AlignCenter
                width: parent.width/2
                color: "#00e40c"
                radius: 5
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: false
                visible: true
                Text {
                    text: "Okay"
                    renderType: Text.NativeRendering
                    fontSizeMode: Text.Fit
                    anchors.bottomMargin: 0
                    textFormat: Text.AutoText
                    anchors.fill: parent
                    color: "white"
                    font.pixelSize: 30
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: "IBM Plex Sans"
                }
                MouseArea {
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    anchors.bottomMargin: 0
                    anchors.fill: parent
                    onClicked: {
                        var id = deviceID.text
                        var pos = devicePosition.text
                        Controller.addDevice(id, pos);
                    }
                }
            }
        }
    }
}
