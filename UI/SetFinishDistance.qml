import QtQuick 2.0

Item {
    id: finishDistanceWindow
    property var distance
    Connections {
        target: Controller
        onDistanceChanged: {
            finishDistanceWindow.distance = Controller.getDistance()
            finishDistance.text = finishDistanceWindow.distance / 100.00
        }
    }

    Item {
        id: startButtonWrapper
        anchors.horizontalCenter: parent.horizontalCenter
        y: 285
        visible: true
        width: 150
        height: 50
        Rectangle {
            id: startButton
            anchors.fill: parent
            color: "green"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    startButtonWrapper.visible = false
                    setDistanceWrapper.visible = true
                    Controller.startDistanceMeasurement()
                }
            }

            Text {
                text: qsTr("Start")
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                renderType: Text.NativeRendering
                font.pixelSize: 25
                color: "white"
            }
        }
    }

    Text {
        id: finishDistance
        y: 227
        width: 150
        height: 50
        text: qsTr("00.00")
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 60
    }

    Item {
        id: setDistanceWrapper
        anchors.horizontalCenter: parent.horizontalCenter
        y: 350
        visible: false
        width: 150
        height: 50
        Rectangle {
            anchors.fill: parent
            color: "red"
            Text {
                text: qsTr("Set")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                renderType: Text.NativeRendering
                font.pixelSize: 25
                color: "black"
            }

            MouseArea {
                id: setDistance
                anchors.fill: parent
                onClicked: {
                    setDistanceWrapper.visible = false
                    startButtonWrapper.visible = true
                    Controller.setFinishDistance(finishDistanceWindow.distance)
                }
            }
        }
    }

    Text {
        id: setDistanceTitle
        x: 271
        y: 120
        text: qsTr("Set Finish Distance")
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: "Arial"
        renderType: Text.NativeRendering
        font.pixelSize: 62
    }
}
