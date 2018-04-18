import QtQuick 2.0

Rectangle {
    property var l: pageLoader
    id: parentLoader
    width: 238
    height: 402
    function load() {
        if (Arduino.getRaceType() === 1 || Arduino.getRaceType() === 2) {
            pageLoader.sourceComponent = parentLoader.start
        } else {
            pageLoader.sourceComponent = parentLoader.ms
        }
    }
    property var start: Component {
        Start {
        }
    }

    property var ms: Component {
        ModeSelectPage {
            start: "Start.qml"
        }
    }

    Loader {
        id: pageLoader
        anchors.fill: parent
        sourceComponent: load()
    }
}
