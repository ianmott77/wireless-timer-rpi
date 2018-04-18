import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Rectangle {
    anchors.fill: parent
    ColumnLayout {
        anchors.fill: parent
        width: parent.width
        Layout.fillHeight: true
        Rectangle {
            id: changeModeWrapper
            height: 40
            Layout.fillWidth: true
            color: "grey"
            MouseArea {
                id: changeMode
                anchors.fill: parent
                onClicked: {
                    Arduino.setRaceType(0)
                    var it = mainSwipeView.itemAt(1).item.item.l
                    it.sourceComponent = Qt.createComponent("StartLoader.qml")
                    mainSwipeView.setCurrentIndex(1)
                }
            }
            Text {
                anchors.fill: parent
                text: "Change Modes"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.family: "IBM Plex Sans"
            }
        }
        Rectangle {
            id: resyncWrapper
            Layout.fillWidth: true
            height: 40
            color: "grey"
            MouseArea {
                id: resync
                anchors.fill: parent
                onClicked: {
                    Arduino.sync()
                }
            }
            Text {
                anchors.fill: parent
                text: "Resync"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.family: "IBM Plex Sans"
            }
        }
    }
}
