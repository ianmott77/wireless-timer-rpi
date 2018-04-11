import QtQuick 2.0

Rectangle {
    id: parentLoader
    width: 238
    height: 402

    property var start: Component {
        Start {}
    }

    property var ms: Component {
        ModeSelectPage {
            start: "Start.qml"
        }
    }

    function load() {
        if (Arduino.getRaceType() === 1 || Arduino.getRaceType() === 2) {
            return start
        } else {
            return ms
        }
    }

    Loader {
       id:pageLoader
        anchors.fill: parent
        sourceComponent: load()
    }
}
