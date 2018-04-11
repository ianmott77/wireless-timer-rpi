import QtQuick 2.0

Rectangle {
    id:barWrapper
    width: parent.width
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

    Battery {
        id: batteryIcon
        width: barWrapper.width / 5
        height: barWrapper.height - 10
        x: (barWrapper.width - 10)/5 * 4
        anchors.verticalCenter: parent.verticalCenter
    }
}
