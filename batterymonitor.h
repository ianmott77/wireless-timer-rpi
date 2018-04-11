#ifndef BATTERYMONITOR_H
#define BATTERYMONITOR_H

#include <QFile>
#include <QElapsedTimer>
#include <QObject>
#include <iostream>
#include <thread>

class BatteryMonitor  : public QObject {
  Q_OBJECT

public:

  BatteryMonitor();
  QString getPowerSource();
  float   getPowerLevel();
  QFile * getBatteryStatusFile();
  bool    readBatteryStatus();
  qint64  getUpTime();

signals:

  void updatePowerStatus(float, QString);

private:

  QStringList *rawData;
  QElapsedTimer *timer;
  std::thread *monitor;
  QFile *batteryStatus;
  std::atomic<QString>powerSource;
  std::atomic<float>batteryLevel;
};

#endif // BATTERYMONITOR_H
