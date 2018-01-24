#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QQuickItem>
#include "Connection/SerialMonitor.h"
#include "distancethread.h"
#include "device.h"

class Arduino: public Device
{
    Q_OBJECT
public:
    Arduino(ConnectionManager *);
    ~Arduino();
    DistanceThread * dist = 0;

signals:
    void signalChoice(int);
    void readyToUpdate();
 public slots:
    int getType();
    void setType(int);
    int getNextAddress();
    int getPrevAddress();
    void setNextAddress(int);
    void setPrevAddress(int);
    bool addDevice(int ID = -1, int position = -1, Device * device = 0);
    SerialMonitor * getMonitor();
    ConnectionManager * getManager();
    void getInfo();
    void stopSerialLine();
    std::thread * startSerialLine();
    void setFinishDistance(int distance);
    int getFinishDistance();
    void startDistanceMeasurement();
    void updateDistance(int);
    int getDistance();
    void setDistance(int);
    void sync();
    void startRacer(int);
    void receiveIntervalSignal();
    bool getIntervalStatus();
    void setIntervalStatus(bool);
private:
    void monitorSerialLine();
    std::thread * monLine;
    std::thread * line;
    SerialMonitor * monitor = 0;
    ConnectionManager * manager = 0;
    std::atomic<bool> intervalSignal;
    int type;
    int nextAddress;
    int prevAddress;
    int distance = -1;
    int choice = 0;

};

#endif // ARDUINO_H
