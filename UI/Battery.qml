import QtQuick 2.0

Item {
    property double level: 0.5

    id: batteryIndicator

    function updateBatteryColor(){
        if(batteryIndicator.level > 0.66){
            batteryLevel.color = "green"
        }else if(batteryIndicator.level < 0.66 && batteryIndicator.level > 0.33){
            batteryLevel.color ="yellow"
        }else if(batteryIndicator.level < 0.33){
           batteryLevel.color = "red"
        }
    }


    Rectangle {
        id: batteryContainer
        width: (batteryIndicator.width/15) * 14
        height: batteryIndicator.height
        color: "transparent"
        border.width: 3
        border.color: "black"
        Rectangle {
            id: batteryLevel
            x: batteryContainer.border.width
            y: batteryContainer.border.width
            color: updateBatteryColor()
            width: (batteryContainer.width - (batteryContainer.border.width * 2)) * level
            height: parent.height- (batteryContainer.border.width * 2)
        }
    }
    Rectangle{
        id: batteryNub
        color: "black"
        height: batteryIndicator.height/2;
        width: batteryIndicator.width/15;
        y: batteryContainer.height * 0.25;
        x: batteryContainer.width
    }

}
