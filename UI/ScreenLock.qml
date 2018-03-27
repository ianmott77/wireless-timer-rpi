import QtQuick 2.0
import QtQuick.Controls 2.3

Rectangle {
    id: rectangle
    width: 238
    height: 402
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
        y: 0
        width: 238
        height: 165
        color: "#80ffffff"
        text: ""
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenterOffset: 0
        font.pointSize: 40
        fontSizeMode: Text.Fit
        renderType: Text.NativeRendering
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: "IBM Plex Sans"
    }

    BusyIndicator {
        id: busyIndicator
        x: 311
        y: 320
        anchors.verticalCenter: parent.verticalCenter
        font.pointSize: 50
        wheelEnabled: true
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
