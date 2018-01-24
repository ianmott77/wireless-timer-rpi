import QtQuick 2.0

Item {
    property var keyboard : pullDownPage.keyboard
    id: settingsWrapper
    anchors.fill: parent
    Rectangle {
        anchors.fill: parent
        color: "#cc000000"
        SettingsMenu {
            keyboard: settingsWrapper.keyboard
        }
    }
}
