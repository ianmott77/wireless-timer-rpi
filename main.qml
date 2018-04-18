import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.VirtualKeyboard 2.3

import "UI" as UI

ApplicationWindow {
    id: mainWindow
    width: 238
    height: 402
    visible: true
    property int type: Arduino.getType()
    Item {
        id: mainWrapper
        anchors.fill: parent

        UI.StatusBar {
            id: statusBar
            y: 0
            height: 32.5
        }

        Rectangle {
            id: mainWindowFill
            x: 0
            y: (Arduino.getType() === 3) ? 50 : statusBar.height
            height: (Arduino.getType(
                         ) === 3) ? parent.height - 100 : parent.height - statusBar.height
            width: parent.width
            SwipeView {
                id: mainSwipeView
                currentIndex: 0
                anchors.fill: parent

                Connections {
                    target: Controller
                    onRaceMode: {
                        mainSwipeView.currentIndex = 1
                    }
                }

                Repeater {
                    function getNumPages() {
                        if (mainWindow.type === 1) {
                            return 4
                        } else if (mainWindow.type === 2) {
                            return 4
                        } else {
                            return 1
                        }
                    }
                    model: getNumPages()

                    Loader {

                        active: SwipeView.isCurrentItem || SwipeView.isNextItem
                                || SwipeView.isPreviousItem || SwipeView.isNextItem + 1 || SwipeView.isPreviousItem - 1
                        sourceComponent: Loader {
                            function getSource(index) {
                                if (mainWindow.type == 1) {
                                    mainSwipeView.currentIndex = 1
                                    if(index === 0){
                                        return "UI/OptionsPage.qml"
                                    }else if (index === 1) {
                                        return "UI/StartLoader.qml"
                                    } else if (index === 2) {
                                        return "UI/StatusPage.qml"
                                    } else if (index === 3) {
                                        return "UI/SettingsPage.qml"
                                    }
                                } else if (mainWindow.type == 2) {
                                    if (index === 0) {
                                        return "UI/SetFinishDistance.qml"
                                    } else if (index === 1) {
                                        return "UI/ViewTime.qml"
                                    } else if (index === 2) {
                                        return "UI/StatusPage.qml"
                                    } else if (index === 3) {
                                        return "UI/TimesPage.qml"
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
                y: 575
                anchors.horizontalCenterOffset: 0
                count: mainSwipeView.count
                currentIndex: mainSwipeView.currentIndex
                anchors.bottom: mainSwipeView.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                // visible:  (Arduino.getType() === 2) ? true :false
            }
        }
    }

    UI.ScreenLock {
        id: scrLock
        visible: false
        anchors.fill: parent
        z: 99999
        Connections {
            target: Controller
            onLock: {
                mainWindow.lockScreen(Controller.getLoadingMsg())
            }
            onUnlock: {
                mainWindow.unlockScreen()
            }
        }
    }

    function lockScreen(msg) {
        scrLock.setMsg(msg)
        scrLock.visible = true
    }

    function unlockScreen() {
        scrLock.visible = false
    }

    UI.ErrorScreen {
        id: errScr
        visible: false
        anchors.fill: parent
        z: 99999
        Connections {
            target: Controller
            onErrorSignal: {
                errorScreen(error)
            }
        }
    }

    function errorScreen(err) {
        var msg
        errScr.error = err
        if (err === 200) {
            msg = "The device you are trying to connect to couldn't be reached"
        } else if (err === 201) {
            msg = "There was an error receiving a packet"
        } else if (err === 205) {
            msg = "Invalid position to add to the network"
        }else if(err === 300){
            msg = "Clearing the runs database failed!"
        }

        errScr.setMsg(msg)
        errScr.visible = true
    }
}
