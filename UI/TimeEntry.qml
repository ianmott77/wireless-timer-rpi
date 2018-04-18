import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

RowLayout {
    property var racer
    function updateTime() {
        var time = parseFloat(
                    (racer["finish_time"] - racer["start_time"]) / 1000.00).toFixed(
                    2)
        if(time > 0){
            timeText.text = time
        }else{
            timeText.text = "DNF"
            timeText.color = "red"
        }
    }
    Connections {
        target: Controller
        onFinished: {
            updateTime()
        }
    }
    Layout.fillWidth: true
    Layout.fillHeight: true
    spacing: 40
    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Text {
            text: racer["bib"]
            fontSizeMode: Text.Fit
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: "IBM Plex Sans"
            anchors.fill: parent
        }
    }
    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Text {
            id: timeText
            text:updateTime()
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            fontSizeMode: Text.Fit
            color: "black"
            font.family: "IBM Plex Sans"
            anchors.fill: parent
        }
    }
}
