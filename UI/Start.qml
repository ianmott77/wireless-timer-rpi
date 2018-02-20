import QtQuick 2.0
import QtQuick.VirtualKeyboard 2.3

Item {
    id: item1
    anchors.fill: parent
    Text{
        text: "Bib #"
        anchors.horizontalCenterOffset: 1
        y: 193
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 45
    }

    Rectangle {
        id: bibNumWrapper
        border.color: "black"
        radius: 5
        border.width: 3
        width: parent.width / 4
        height: 60
        anchors.horizontalCenter: parent.horizontalCenter
        y: 250
        TextInput {
            id: bibNum
            anchors.fill: parent
            font.pixelSize: 45
            horizontalAlignment: Text.AlignHCenter
            selectionColor: "#7d8000"
            inputMethodHints: Qt.ImhDigitsOnly
            text: "1"
        }
    }
    Rectangle {
        id: rectangle
        color: "green"
        anchors.horizontalCenter: parent.horizontalCenter
        width: 150
        y: bibNumWrapper.y + bibNumWrapper.height + 20
        height: 50
        radius: 5
        Text {
            text: "Okay"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            font.pixelSize: 45
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                Controller.startRacer(bibNum.text);
                bibNum.text++;
            }
        }
    }

    InputPanel {
        id: startKeyboard
        anchors.left: parent.left
        anchors.right: parent.right
        visible: startKeyboard.active ? true : false
        y: startKeyboard.active ? parent.height - startKeyboard.height : parent.height
    }
}
