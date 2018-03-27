import QtQuick 2.0
import QtQuick.Layouts 1.3

Rectangle {
    property int h: parent.height / 2
    property int w: parent.width
    property var textIn
    property var numColor
    property var clrColor
    anchors.fill: parent
    height: h
    ColumnLayout {
        Repeater {
            id: rowRep
            model: 3
            Rectangle {
                property int outerIndex: index
                width: w
                height: h / 4
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "black"
                RowLayout {
                    anchors.fill: parent
                    Layout.fillWidth: true
                    Repeater {
                        id: colRep
                        model: 3
                        StartKeyboardKey {
                            width: w / 3
                            height: parent.height
                            textVal: (outerIndex * 3 + index) + 1
                            textInput: textIn
                            color: numColor
                            textColor: "white"
                            keyTextSize: 20
                        }
                    }
                }
            }
        }
        Rectangle {
            property int outerIndex: index
            width: w
            height: h / 4
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "black"
            RowLayout {
                anchors.fill: parent
                Layout.fillWidth: true
                StartKeyboardKey {
                    width: w / 3
                    height: parent.height
                    textVal: "<-"
                    textInput: textIn
                    color: clrColor
                    textColor: "white"
                    keyTextSize: 20
                }
                StartKeyboardKey {
                    width: w / 3
                    height: parent.height
                    textVal: "0"
                    textInput: textIn
                    color: numColor
                    textColor: "white"
                    keyTextSize: 20
                }
                StartKeyboardKey {
                    width: w / 3
                    height: parent.height
                    textVal: "Clr"
                    textInput: textIn
                    color: clrColor
                    textColor: "white"
                    keyTextSize: 20
                }
            }
        }
    }
}
