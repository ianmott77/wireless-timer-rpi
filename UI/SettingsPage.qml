import QtQuick 2.0

Rectangle {
    width: 238
    height: 402
    AddDevice {
        id: addDev
        keyboard: startKeyboard
        anchors.fill: parent
    }
    StartKeyboard {
        id: startKeyboard
        y: 180
        height: 320
        w: parent.width - 10
        h: ((parent.height / 4) * 2) - 32.5
        numColor: "#9E9E9E"
        clrColor: "#666666"
        color: "black"
        anchors.topMargin: 190
    }
}
