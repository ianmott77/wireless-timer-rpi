import QtQuick 2.0

Item {
    id: item1
    anchors.fill: parent
    width: 238
    height: 402

    function setBibNumButtonColor(){
       rectangle.color = sendBibNum.enabled ?  "green" : "lightgrey"
    }

    Connections {
        target: Controller
        onInfoChanged: {
            console.log(Arduino.getNextAddress())
            if(Arduino.getNextAddress() !== 255){
                sendBibNum.enabled = true
            }
            setBibNumButtonColor()
        }
    }

    Rectangle {
        id: bibNumWrapper
        border.color: "black"
        radius: 5
        anchors.horizontalCenterOffset: 1
        border.width: 3
        height: 60
        anchors.horizontalCenter: parent.horizontalCenter
        y: 9
        width: 221
        TextInput {
            id: bibNum
            anchors.fill: parent
            font.pixelSize: 50
            horizontalAlignment: Text.AlignHCenter
            selectionColor: "#803500"
            inputMethodHints: Qt.ImhDigitsOnly
            text: "1"
            font.weight: Font.Light
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            font.family: "IBM Plex Sans"
            maximumLength: 4
        }
    }
    Rectangle {
        id: rectangle
        y: 75
        color: setBibNumButtonColor()
        anchors.horizontalCenter: parent.horizontalCenter
        width: 150
        height: 50
        radius: 5
        anchors.horizontalCenterOffset: 0
        Text {
            width: 150
            height: 49
            text: "Okay"
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.Light
            elide: Text.ElideMiddle
            fontSizeMode: Text.Fit
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            font.pixelSize: 43
            font.family: "IBM Plex Sans"
        }
        MouseArea {
            id: sendBibNum
            width: 150
            height: 50
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                Controller.startRacer(bibNum.text)
                bibNum.text++
            }
            enabled: Arduino.getNextAddress() !== 255 ? true : false
        }
    }
    StartKeyboard {
        id: startKeyboard
        height: 320
        w: parent.width - 10
        h: ((parent.height / 4) * 3) - 45
        textIn: bibNum
        numColor: "#9E9E9E"
        clrColor: "#666666"
        color: "black"
        anchors.topMargin: 132
    }
}
