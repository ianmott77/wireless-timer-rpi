import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

ScrollView {
    height: 150
    width: parent.width
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical.policy: ScrollBar.AlwaysOn
    Rectangle {
        height: parent.parent.height
        width: parent.parent.width
        color: "transparent"
        ColumnLayout {
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
                    text: "Device Position:"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    color: "white"
                    font.pixelSize: 30
                }
            }
            Rectangle {
                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                width: parent.width
                id: addDevicePosInput
                color: "#b3000000"
                radius: 5
                TextInput {
                    id: devicePosition
                    cursorVisible: true
                    anchors.fill: parent
                    focus: true
                    color: "white"
                    font.pixelSize: 30
                    inputMethodHints: Qt.ImhDigitsOnly
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            devicePosition.forceActiveFocus()
                            pullDownPage.y = 0
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
                    text: "Device ID:"
                    elide: Text.ElideNone
                    horizontalAlignment: Text.AlignLeft
                    color: "white"
                    font.pixelSize: 30
                    verticalAlignment: Text.AlignVCenter
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
                    cursorVisible: true
                    anchors.fill: parent
                    color: "white"
                    font.pixelSize: 30
                    inputMethodHints: Qt.ImhDigitsOnly
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            deviceID.forceActiveFocus()
                            pullDownPage.y = 0
                        }
                    }
                }
            }
            Rectangle {
                id: okayButton
                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                width: 150
                color: "#00e40c"
                radius: 5
                Text {
                    text: "Okay"
                    textFormat: Text.AutoText
                    anchors.fill: parent
                    color: "white"
                    font.pixelSize: 30
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                MouseArea {
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
