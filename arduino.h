#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QQuickItem>
#include <QQueue>

#include "Connection/SerialMonitor.h"
#include "distancethread.h"
#include "device.h"
#include "racer.h"

class Arduino : public Device {
  Q_OBJECT

public:

  Arduino(ConnectionManager *);
  ~Arduino();
  DistanceThread *dist = 0;

signals:

  void signalChoice(int);
  void readyToUpdate();
  void error(int);

public slots:

  int                getType();
  void               setType(int);
  int                getNextAddress();
  int                getPrevAddress();
  void               setNextAddress(int);
  void               setPrevAddress(int);
  bool               addDevice(int     ID = -1,
                               int     position = -1,
                               Device *device = 0);
  SerialMonitor    * getMonitor();
  ConnectionManager* getManager();
  void               getInfo();
  void               stopSerialLine();
  std::thread      * startSerialLine();
  void               setFinishDistance(int distance);
  int                getFinishDistance();
  void               startDistanceMeasurement();
  void               updateDistance(int);
  int                getDistance();
  void               setDistance(int);
  void               sync();
  void               startRacer(int);
  void               receiveIntervalSignal();
  bool               getIntervalStatus();
  void               setIntervalStatus(bool);
  Racer            * getRacerInfo();
  void               raceMode();
  QQueue<Racer *>  * getTimeQueue();
  bool               isMonitorRunning();
  void               dnfRacer(int);
  int                getRaceType();
  void               setRaceType(int);
  bool               isPaceSet();
  void               setPaceSet(bool);
  unsigned long      getPace();
  void               setPace(unsigned long);

private:

  void monitorSerialLine();
  std::thread *monLine;
  std::thread *line;
  QQueue<Racer *> *timeQueue = 0;
  SerialMonitor *monitor     = 0;
  ConnectionManager *manager = 0;
  std::atomic<bool>intervalSignal;
  int type;
  int nextAddress;
  int prevAddress;
  int distance = -1;
  int choice   = 0;
  std::atomic<bool>monitorRunning;
  std::atomic<int>raceType;
  std::atomic<bool>paceSet;
  std::atomic<unsigned long>pace;
};

#endif // ARDUINO_H
