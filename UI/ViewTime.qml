import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
    id: rectangle1
    width: 238
    height: 402
   anchors.fill: parent
    Connections{
        target: Controller
        onRacerOnCourse:{
            bibNumber.text =Controller.getCurrentBib();
            var time = parseFloat(Controller.getCurrentTime()/1000.00).toFixed(2);
            currentTimeText.text = (time === 0) ? "DNF" : time;
            currentTimeText.color = "black"
        }
        onRaceMode:{
            bibNumber.text = '0';
            currentTimeText.color = "black"
            currentTimeText.text = '00:00';
        }
        onDnfRacer:{
            currentTimeText.color = "red"
            currentTimeText.text = "DNF"
        }
    }

    Rectangle{
        id: rectangle
        anchors.fill: parent
        Text{
            id: bibNumber
            text: "0";
            y: 25
            width: 175
            height: 100
            font.pixelSize: 50
            font.underline: true
            anchors.horizontalCenter: parent.horizontalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: "IBM Plex Sans"
        }

        Text {
            id: currentTimeText
            y: 145
            width: 225
            height: 115
            text: qsTr("00:00")
            fontSizeMode: Text.Fit
            renderType: Text.QtRendering
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 75
            font.family: "IBM Plex Sans"
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
    Rectangle{
        y: 300
        height: 50
        width: 175
        color: "red"
        anchors.horizontalCenterOffset: 1
        anchors.horizontalCenter: parent.horizontalCenter
        Text{
            anchors.fill: parent
            color: "white"
            text: "DNF"
            font.pointSize: 25
            anchors.horizontalCenter: parent.horizontalCenter
            fontSizeMode: Text.Fit
            renderType: Text.QtRendering
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: "IBM Plex Sans"
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                Arduino.dnfRacer(Controller.getCurrentBib())
            }
        }
    }
}
