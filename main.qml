import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.VirtualKeyboard 2.3

import "UI" as UI

ApplicationWindow {
    id: mainWindow
    width: 1024
    height: 600
    visible: true
    property int type: Arduino.getType()

    Item {
        id: mainWrapper
        anchors.fill: parent

        UI.PullDown{
            wrapper: mainWrapper
            dragMargin: 150
            footerHeight: 75
            main: "Settings.qml"
            footer: "SettingsMenuFooter.qml"

        }

        Rectangle {
            id: mainWindowFill
            x: 0
            y: 100
            height: parent.height - 100
            width: parent.width
            SwipeView {
                id: mainSwipeView
                currentIndex: 0
                anchors.fill: parent
                Connections{
                    target: Controller
                    onRaceMode:{
                        mainSwipeView.currentIndex = 1
                    }
                }

                Repeater {
                    function getNumPages() {
                        if (mainWindow.type === 2) {
                            return 2
                        } else {
                            return 1
                        }
                    }
                    model: getNumPages()
                    Loader {
                        active: SwipeView.isCurrentItem || SwipeView.isNextItem
                                || SwipeView.isPreviousItem
                        sourceComponent: Loader {
                            function getSource(index) {
                                if (mainWindow.type == 1) {
                                    return "UI/Start.qml"
                                } else if (mainWindow.type == 2) {
                                    if (index === 0) {
                                        return "UI/SetFinishDistance.qml"
                                    } else if (index === 1) {
                                        return "UI/ViewTime.qml"
                                    }
                                } else if (mainWindow.type == 3) {

                                }
                            }
                            anchors.fill: parent
                            source: getSource(index)
                        }
                    }
                }
            }

            PageIndicator {
                id: mainSwipeViewIndicator
                y: 562
                anchors.horizontalCenterOffset: 0
                count: mainSwipeView.count
                currentIndex: mainSwipeView.currentIndex
                anchors.bottom: mainSwipeView.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    UI.ScreenLock{
        id:scrLock
        visible: false;
        anchors.fill: parent
        z: 99999;
        Connections{
            target: Controller
            onLock:{
               mainWindow.lockScreen(Controller.getLoadingMsg())
            }
            onUnlock:{
                 mainWindow.unlockScreen();
            }
        }
    }
    function lockScreen(msg){
        scrLock.setMsg(msg);
        scrLock.visible = true;
    }

    function unlockScreen(){
        scrLock.visible = false;
    }
}
