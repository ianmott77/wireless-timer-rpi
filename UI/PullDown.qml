import QtQuick 2.0
import QtQuick.VirtualKeyboard 2.3

Rectangle {
    id: pullDownPage
    property var wrapper
    property var main
    property var footer
    property int footerHeight: 0
    property int dragMargin: 100
    property var keyboard: settingsKeyboard

    z: 1
    x: 0
    y: footerHeight - wrapper.height
    width: wrapper.width
    height: wrapper.height + dragMargin
    Item {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: settingsKeyboard.top
        Rectangle {
            id: pageMain
            height: wrapper.height - footerHeight
            width: parent.width
            color: "transparent"
            Loader {
                anchors.fill: parent
                source: pullDownPage.main
            }
        }

        Rectangle {
            id: pageFooter
            y: pageMain.y + pageMain.height
            width: parent.width
            height: pullDownPage.footerHeight
            color: "transparent"
            Loader {
                anchors.fill: parent
                source: pullDownPage.footer
            }
        }
    }
        MouseArea {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: settingsKeyboard.top
            drag.target: parent
            drag.axis: Drag.YAxis
            drag.minimumY: footerHeight - wrapper.height
            drag.maximumY: 0
            drag.filterChildren: true
            propagateComposedEvents: true
            onClicked: {
                mouse.accepted = false
            }
        }


    InputPanel {
        id: settingsKeyboard
        anchors.left: parent.left
        anchors.right: parent.right
        visible: settingsKeyboard.active ? true : false
        y: settingsKeyboard.active ? (parent.height - pullDownPage.dragMargin)
                                     - settingsKeyboard.height : parent.height
    }
}
