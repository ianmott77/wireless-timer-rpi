import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
   anchors.fill: parent
    Connections{
        target: Controller
        onRacerOnCourse:{
            bibNumber.text =Controller.getCurrentBib();
            currentTimeText.text = Controller.getCurrentTime()/1000.00;
        }
        onRaceMode:{
            bibNumber.text = '-1';
            currentTimeText.text = '00:00';
        }
    }

    Rectangle{
        id: rectangle
        anchors.fill: parent
        Text{
            id: bibNumber
            y: 131
            width: 177
            height: 98
            font.pixelSize: 50
            font.underline: true
            anchors.horizontalCenterOffset: 1
            anchors.horizontalCenter: parent.horizontalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: "Arial"
        }

        Text {
            id: currentTimeText
            x: 322
            y: 271
            width: 381
            height: 178
            text: qsTr("00:00")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 75
            font.family: "Courier"
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
