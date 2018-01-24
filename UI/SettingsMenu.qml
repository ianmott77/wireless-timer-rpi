import QtQuick 2.0

Item {
    property var keyboard
    id: settingsMenu
    anchors.fill: parent
    Component {
        id: settingDelegate
        Item {
            width: parent.width
            height: 75
            Rectangle {
                height: 75
                width: parent.width
                color: "gray"
                border.width: 1
                border.color: "#b3737171"
                Text {
                    font.pixelSize: 30
                    text: setting
                    font.weight: Font.Thin
                    fontSizeMode: Text.Fit
                    renderType: Text.NativeRendering
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                }
            }
            MouseArea {
                height: 75
                width: parent.width
                onClicked: {
                    var showing = parent.children[2].visible
                    parent.children[2].y = (showing) ? 0 : parent.height
                    parent.height = (showing) ? 75 : parent.height + settingHeight
                    parent.children[2].visible = (showing) ? false : true
                }
            }
            Loader {
                width: parent.width
                height: settingHeight
                x: 0
                y: parent.height
                visible: false
                source: settingsView
            }
        }
    }
    ListView {
        anchors.fill: parent
        anchors.topMargin: 0
        model: SettingsListModel {
        }
        delegate: settingDelegate
    }
}
