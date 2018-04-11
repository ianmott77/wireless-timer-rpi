import QtQuick 2.0

Item {
    id: finishDistanceWindow
    property var distance :  0
    width: 238
    height: 362
    Connections {
        target: Controller
        onDistanceChanged: {
            finishDistance.text = Arduino.getDistance()/ 100.00
        }
    }

    Item {
        id: startButtonWrapper
        x: 31
        y: 257
        visible: true
        width: 175
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
                    cancelWrapper.visible = true
                    Controller.startDistanceMeasurement()
                }
            }

            Text {
                text: qsTr("Start")
                anchors.verticalCenterOffset: 1
                anchors.horizontalCenterOffset: 0
                font.family: "IBM Plex Sans"
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
        y: 86
        width: 175
        height: 80
        text: distance
        fontSizeMode: Text.Fit
        renderType: Text.NativeRendering
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 70
        font.family: "IBM Plex Sans"
    }

    Item {
        id: cancelWrapper
        anchors.horizontalCenter: parent.horizontalCenter
        y: 187
        visible: false
        width: 175
        height: 50
        anchors.horizontalCenterOffset: 2
        Rectangle {
            anchors.fill: parent
            color: "red"
            Text {
                anchors.fill: parent
                text: qsTr("Cancel")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                renderType: Text.NativeRendering
                font.pixelSize: 25
                color: "#ffffff"
                font.family: "IBM Plex Sans"
            }

            MouseArea {
                id: cancel
                anchors.fill: parent
                onClicked: {
                    setDistanceWrapper.visible = false
                    startButtonWrapper.visible = true
                    cancelWrapper.visible = false
                    Controller.setFinishDistance(distance * 100)
                    finishDistance.text = distance
                }
            }
        }
    }

    Item {
        id: setDistanceWrapper
        anchors.horizontalCenter: parent.horizontalCenter
        y: 257
        visible: false
        width: 176
        height: 50
        anchors.horizontalCenterOffset: 0
        Rectangle {
            anchors.fill: parent
            color: "green"
            Text {
                width: 176
                height: 50
                text: qsTr("Set")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                renderType: Text.NativeRendering
                font.pixelSize: 25
                color: "#ffffff"
                font.family: "IBM Plex Sans"
            }

            MouseArea {
                id: setDistance
                anchors.fill: parent
                onClicked: {
                    setDistanceWrapper.visible = false
                    startButtonWrapper.visible = true
                    cancelWrapper.visible = false
                    distance = finishDistance.text
                    Controller.setFinishDistance(finishDistanceWindow.distance * 100)
                }
            }
        }
    }

    Text {
        id: setDistanceTitle
        x: 271
        y: 15
        width: 224
        height: 55
        text: qsTr("Set Finish")
        anchors.horizontalCenterOffset: 1
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.underline: true
        anchors.horizontalCenter: parent.horizontalCenter
        fontSizeMode: Text.Fit
        font.family: "IBM Plex Sans"
        renderType: Text.NativeRendering
        font.pixelSize: 48
    }
}
