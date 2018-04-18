import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Rectangle {
    id: timeWrapper
    anchors.fill: parent
    property var racers: []
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        Rectangle {
            id: runControlBar
            Layout.fillWidth: true
            height: 50
            color: "black"
            RowLayout {
                spacing: 0
                anchors.fill: parent
                Rectangle {
                    color: "white"
                    Layout.fillWidth: true
                    height: 35
                    Text {
                        anchors.fill: parent
                        text: " Clear All"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "IBM Plex Sans"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            Controller.clearAllRuns()
                        }
                    }
                }
                Rectangle {
                    color: "black"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    RowLayout {
                        spacing: 5
                        Text {
                            color: "white"
                            Layout.fillHeight: true
                            text: "Sort By"
                            fontSizeMode: Text.Fit
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.family: "IBM Plex Sans"
                        }
                        ComboBox {
                            font.family: "IBM Plex Sans"
                            font.pointSize: 10
                            width: 30
                            Layout.fillHeight: true
                            model: ["Time", "Bib"]
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 25
            color: "#b3615050"
            id: tableHeader
            RowLayout {
                anchors.fill: parent
                spacing: 0
                Text {
                    text: "Bib #"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    fontSizeMode: Text.Fit
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    font.family: "IBM Plex Sans"
                }
                Text {
                    text: "Time"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    fontSizeMode: Text.Fit
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    font.family: "IBM Plex Sans"
                }
            }
        }

        Rectangle {
            id: scrollWrapper
            Layout.fillHeight: true
            Layout.fillWidth: true
            y: 10
            ScrollView {
                id: timesScroll
                anchors.fill: parent
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.horizontal.interactive: false
                clip: true
                ColumnLayout {
                    id: timeView
                    spacing: 5
                    Repeater {
                        model: Controller.getNumRacers()
                        Rectangle {
                            Layout.fillWidth: true
                            height: 45
                            TimeEntry {
                                width: timesScroll.width
                                racer: Controller.racerAtToJson(index)
                            }
                        }
                    }
                }
            }
        }
    }
    Connections {
        target: Controller
        onNewRacerOnCourse: {
            timeWrapper.racers[bib] = Qt.createComponent(
                        "TimeEntry.qml").createObject(timeView, {
                                                          width: timeWrapper.width,
                                                          height: 35,
                                                          x: 20,
                                                          racer: {
                                                              bib: bib,
                                                              start_time: 0,
                                                              finish_time: time
                                                          }
                                                      })
        }
        onFinished: {
            timeWrapper.racers[bib]["racer"]["start_time"] = startTime
            timeWrapper.racers[bib]["racer"]["finish_time"] = time
        }
    }
}
