QT += quick
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    Connection/Connection.cpp \
    Connection/ConnectionFactory.cpp \
    Connection/ConnectionManager.cpp \
    Connection/I2C.cpp \
    Connection/main.old.cpp \
    Connection/Packet.cpp \
    Connection/Serial.cpp \
    Connection/SerialMonitor.cpp \
    Connection/SPI.cpp \
    distancethread.cpp \
    uicontroller.cpp \
    arduino.cpp \
    device.cpp \
    racer.cpp \
    runtimemanager.cpp \
    batterymonitor.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

HEADERS += \
    Connection/Connection.h \
    Connection/ConnectionFactory.h \
    Connection/ConnectionManager.h \
    Connection/defs.h \
    Connection/I2C.h \
    Connection/Packet.h \
    Connection/Serial.h \
    Connection/SerialMonitor.h \
    Connection/SPI.h \
    distancethread.h \
    uicontroller.h \
    arduino.h \
    device.h \
    racer.h \
    runtimemanager.h \
    batterymonitor.h
