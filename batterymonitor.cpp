#include "batterymonitor.h"

BatteryMonitor::BatteryMonitor()
{
  this->batteryStatus = new QFile(QString("/home/pi/.pi_power_status"));

  if (!this->readBatteryStatus()) {
    return;
  }

  this->monitor = new std::thread([this] {
    this->timer = new QElapsedTimer();
    this->timer->start();
    unsigned long last = this->timer->elapsed();
    unsigned long now = this->timer->elapsed();

    while (true) {
      now = timer->elapsed();

      if (now - last >= 10000) {
        readBatteryStatus();
        last = this->timer->elapsed();
      }
    }
  });
}

bool BatteryMonitor::readBatteryStatus() {
  if (this->batteryStatus->open(QIODevice::ReadOnly)) {
    QByteArray data = this->batteryStatus->readAll();
    QString    stats(data);
    this->rawData = new QStringList(stats.split(","));

    if (rawData->length() >= 2) {
      this->batteryLevel = this->rawData->at(0).toFloat();
      this->powerSource  = this->rawData->at(1);
      emit this->updatePowerStatus(this->getPowerLevel(), this->getPowerSource());
    } else {
      return false;
    }

    this->batteryStatus->close();
    return true;
  }
  return false;
}

QString BatteryMonitor::getPowerSource()       {
  return this->powerSource.load();
}

float BatteryMonitor::getPowerLevel()        {
  return this->batteryLevel.load();
}

QFile * BatteryMonitor::getBatteryStatusFile() {
  return this->batteryStatus;
}

qint64 BatteryMonitor::getUpTime() {
  return this->timer->elapsed();
}
