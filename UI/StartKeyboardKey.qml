import QtQuick 2.0

Rectangle {
    property var textInput
    property var textVal
    property var click
    property  var textColor
    property int  keyTextSize: 15
    border.width: 1
    border.color: "black"
    radius: 5
    Text {
        anchors.fill: parent
        text: textVal
        font.pointSize: keyTextSize
        fontSizeMode: Text.Fit
        renderType: Text.QtRendering
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: textColor
        font.family: "IBM Plex Sans"
        font.weight: Font.Light
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(textVal === "<-"){
                    textInput.remove(textInput.length, textInput.length - 1)
                }else if(textVal ==="Clr"){
                    textInput.clear()
                }else{
                    textInput.insert(textInput.length, parent.text)
                }
            }
        }
    }
}
