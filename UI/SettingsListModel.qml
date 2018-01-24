import QtQuick 2.0

ListModel {
    ListElement {
        setting: "Add Device"
        settingsView: "AddDevice.qml"
        settingHeight: 350
    }
    ListElement {
        setting: "Sync"
        settingsView: "Sync.qml"
        settingHeight: 200
    }
}
