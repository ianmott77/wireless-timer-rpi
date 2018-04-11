import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Rectangle {
    property var racers: Controller.getRacerJSON()
    anchors.fill: parent

    function countRuns() {
        var i = 0
        for (var bib in racers) {
            i++
        }
        return i
    }

    function getKeyAt(index) {
        var i = 0
        for (var bib in racers) {
            if (i === index)
                return bib
            i++
        }
    }

    function getTimeFor(key) {
        return (racers[key]["finish-time"] !== 0) ? racers[key]["finish-time"] - racers[key]["start-time"] : 0;
    }

    ScrollView {
        anchors.fill: parent
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.horizontal.interactive: false
        clip: true
        ColumnLayout {
            spacing: 5
            Repeater {
                model: countRuns()
                Rectangle {
                    property var bib: getKeyAt(index)
                    Layout.fillWidth: true
                    height: 45
                    RowLayout {
                        anchors.fill: parent
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 40
                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Text {
                                text: bib
                                fontSizeMode: Text.Fit
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.family: "IBM Plex Sans"
                            }
                        }
                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Text {
                                property var time: (getTimeFor(bib) === 0) ? "DNF" : parseFloat( getTimeFor(bib) / 1000.00).toFixed(2)
                                text: time
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                fontSizeMode: Text.Fit
                                color: (time === "DNF") ? "red" : "black"
                                font.family: "IBM Plex Sans"
                            }
                        }
                    }
                }
            }
        }
    }
}
