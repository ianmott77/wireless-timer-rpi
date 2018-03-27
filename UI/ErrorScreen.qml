import QtQuick 2.0
import QtQuick.Controls 2.3

Rectangle {
    property int error: 0
    id: errorScreen
    width: 238
    height: 402
    color: "#b3a30000"

    function setMsg(msg) {
        errMsg.text = msg
    }

    Text {
        anchors.fill: parent
        text: "Error: "+ error
        font.underline: true
        font.bold: true
        anchors.bottomMargin: 346
        fontSizeMode: Text.HorizontalFit
        font.pixelSize: 40
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.family: "IBM Plex Sans"
        color: "#80ffffff"
    }

    Rectangle {
        width: 200
        height: 75
        color: "#cc625d5d"
        anchors.verticalCenterOffset: 156
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        z: 1
        Text {
            anchors.fill: parent
            text: "Okay"
            fontSizeMode: Text.Fit
            font.pixelSize: 30
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: "IBM Plex Sans"
            color: "#80ffffff"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                errorScreen.visible = false
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            mouse.accepted = true
        }
        preventStealing: true
        z: 0
    }

    TextArea {
        id: errMsg
        y: 62
        width: 238
        height: 251
        color: "#80ffffff"
        text: ""
        font.pointSize: 14
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenterOffset: 0
        renderType: Text.QtRendering
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: "IBM Plex Sans"
    }
}
