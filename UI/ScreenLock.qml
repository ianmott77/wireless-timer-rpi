import QtQuick 2.0
import QtQuick.Controls 2.3

Rectangle {
    id: rectangle
    color: "#802f2e7b"
    function setMsg(msg){
        loadMsg.text = msg;
    }
    MouseArea{
        anchors.fill: parent;
        onClicked: {
            mouse.accepted = true;
        }
        preventStealing: true
    }

    Text {
        id: loadMsg
        color: "#80ffffff"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        font.pointSize: 36
    }

    BusyIndicator {
        id: busyIndicator
        x: 311
        y: 320
        font.pointSize: 50
        wheelEnabled: true
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
