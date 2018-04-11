import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {
    property var start
    anchors.fill: parent
    Rectangle {
        anchors.fill: parent
        Loader {
            id: pageLoader
            anchors.fill: parent
            sourceComponent: content
        }
    }

    property var content: Component {
        Rectangle {
            anchors.fill: parent
            id: modePage
            width: 238
            height: 402

            Rectangle {
                id: titleWrapper
                anchors.horizontalCenter: parent.horizontalCenter
                Text {
                    text: "Mode Select"
                    font.pixelSize: 25
                    font.underline: true
                    fontSizeMode: Text.Fit
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "IBM Plex Sans"
                }
            }

            Rectangle {
                id: modeButtonsWrapper
                width: 185
                height: 190
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                ColumnLayout {
                    spacing: 45
                    anchors.fill: parent
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "blue"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                Arduino.setRaceType(1)
                                pageLoader.source = start
                            }
                        }
                        Text {
                            color: "#ffffff"
                            anchors.fill: parent
                            text: "Stopwatch"
                            font.pixelSize: 17
                            fontSizeMode: Text.Fit
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.family: "IBM Plex Sans"
                        }
                    }
                    Rectangle {
                        color: "blue"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                Arduino.setRaceType(2)
                                pageLoader.source = start
                            }
                        }
                        Text {
                            color: "#ffffff"
                            anchors.fill: parent
                            text: "Pace"
                            font.pointSize: 17
                            fontSizeMode: Text.Fit
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.family: "IBM Plex Sans"
                        }
                    }
                }
            }
        }
    }
}
