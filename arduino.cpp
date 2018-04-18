#include "arduino.h"

Arduino::Arduino(ConnectionManager *manager) : Device(this->getID(),
                                                      this->getPosition()) {
  this->manager        = manager;
  this->intervalSignal = false;
  this->dist           = new DistanceThread(this->manager);
  this->timeQueue      = new QQueue<Racer *>();
  this->pace           = 0;
  this->paceSet        = false;
  this->raceType       = 0;
  this->getInfo();
}

Arduino::~Arduino() {}

void Arduino::getInfo() {
  std::cout << "getting arduino info" << std::endl << std::flush;

  if (this->manager->connect(I2C)) {
    if (this->manager->send(200, 0)) {
      int data     = 0;
      Packet *pack = 0;

      pack = this->manager->read();
      memcpy(&data, (int *)pack->getData(), pack->getSize());

      this->setID(data);

      pack = this->manager->read();
      memcpy(&data, (int *)pack->getData(), pack->getSize());

      this->setPosition(data);

      pack = this->manager->read();
      memcpy(&data, (int *)pack->getData(), pack->getSize());

      this->setNextAddress(data);

      pack = this->manager->read();
      memcpy(&data, (int *)pack->getData(), pack->getSize());

      this->setPrevAddress(data);

      pack = this->manager->read();
      memcpy(&data, (int *)pack->getData(), pack->getSize());

      this->setType(data);

      this->manager->send(201, 0);
    }
  }
} // Arduino::getInfo

int Arduino::getType() {
  return this->type;
}

void Arduino::setType(int type) {
  this->type = type;
}

void Arduino::setNextAddress(int next) {
  this->nextAddress = next;
}

void Arduino::setPrevAddress(int prev) {
  this->prevAddress = prev;
}

int Arduino::getNextAddress() {
  return this->nextAddress;
}

int Arduino::getPrevAddress() {
  return this->prevAddress;
}

bool Arduino::isPaceSet() {
  return this->paceSet.load();
}

void Arduino::setPaceSet(bool set) {
  this->paceSet = set;
}

unsigned long Arduino::getPace() {
  return this->pace.load();
}

void Arduino::setPace(unsigned long pace) {
  this->pace = pace;
}

void Arduino::sync() {
  if (this->manager->connect(I2C)) {
    if (this->manager->send(2)) {
      if (this->manager->connect(SERIAL)) {
        while (this->manager->getCurrentConnection()->available() <= 0) ;
        Packet *p    = this->manager->read();
        int     done = 0;
        memcpy(&done, (int *)p->getData(), p->getSize());
        emit this->syncDone();
      }
    }
  }
}

bool Arduino::addDevice(int ID, int position, Device *device) {
  if ((ID != -1) && (position != -1)) {
    if ((position > this->getPosition() + 1) ||
        (position < this->getPosition())) {
      emit error(205);
      return false;
    }

    if (this->manager->connect(I2C)) {
      // send 1 to say we're adding a device
      if (this->manager->send(1)) {
        // send the parameters of the device to add
        std::cout << "sent choice" << std::endl << std::flush;

        if (this->manager->send(ID)) {
          std::cout << "sent ID" << std::endl << std::flush;

          if (this->manager->send(position)) {
            std::cout << "sent position" << std::endl << std::flush;

            if (this->manager->connect(SERIAL)) {
              Packet *p       = 0;
              int     confirm = 0;
              bool    print   = false;

              while (this->manager->getCurrentConnection()->available() <= 0) {
                if (!print) {
                  std::cout << "Waiting..." << std::endl << std::flush;
                  print = true;
                }
              }
              p = this->manager->read();
              memcpy(&confirm, (int *)p->getData(), p->getSize());

              if (confirm == 1) {
                std::cout << "Confirmed" << std::endl << std::flush;
                emit readyToUpdate();
                return true;
              } else {
                // error adding device to network, either the wrong device ID or
                // it's to far away
                std::cout << confirm << std::endl << std::flush;
                emit error(confirm);
                return false;
              }
            }
          } else {
            return false;
          }
        } else {
          return false;
        }
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else {
    Packet *p    = 0;
    int     data = 0;

    p = this->manager->read();

    if (p != 0) {
      memcpy(&data, (int *)p->getData(), p->getSize());
    } else {
      return false;
    }
    device->setID(data);

    p = this->manager->read();

    if (p != 0) {
      memcpy(&data, (int *)p->getData(), p->getSize());
    } else {
      return false;
    }
    device->setPosition(data);
    return true;
  }
} // Arduino::addDevice

void Arduino::receiveIntervalSignal() {
  this->monitor = new SerialMonitor(this->manager);
  std::thread *serMon = this->monitor->start();
  serMon->join();

  if (!this->monitor->wasCancelled()) {
    delete this->monitor;
    this->intervalSignal = true;
  }

  if (!this->intervalSignal) delete this->monitor;
}

void              Arduino::raceMode() {}

QQueue<Racer *> * Arduino::getTimeQueue() {
  return this->timeQueue;
}

Racer * Arduino::getRacerInfo() {
  Packet *p               = 0;
  int     bib             = 0;
  unsigned long int start = 0;
  int curTime             = 0;
  int raceMode            =  0;

  p = this->manager->read();
  memcpy(&bib,      (int *)p->getData(),           p->getSize());
  p = this->manager->read();
  memcpy(&start,    (unsigned long *)p->getData(), p->getSize());
  p = this->manager->read();
  memcpy(&curTime,  (int *)p->getData(),           p->getSize());
  p = this->manager->read();
  memcpy(&raceMode, (int *)p->getData(),           p->getSize());
  Racer *r = new Racer(bib, start, curTime, raceMode, new QElapsedTimer());
  r->start();
  this->timeQueue->enqueue(r);
  return r;
}

SerialMonitor * Arduino::getMonitor() {
  return this->monitor;
}

ConnectionManager * Arduino::getManager() {
  return this->manager;
}

void Arduino::stopSerialLine() {
  this->monitor->cancel();
}

std::thread * Arduino::startSerialLine() {
  return new  std::thread([this] {
    this->monitorSerialLine();
  });
}

bool Arduino::isMonitorRunning() {
  return monitorRunning.load();
}

void Arduino::setIntervalStatus(bool status) {
  this->intervalSignal = status;
}

void Arduino::monitorSerialLine() {
  this->monitor = new SerialMonitor(this->manager);

  this->monitorRunning = true;

  bool a = false;

  while (this->dist->isRunning()) {
    if (!a) {
      std::cout << "dt still running" << std::endl << std::flush;
      a = true;
    }
  }

  std::cout << "step 1" << std::endl << std::flush;

  std::cout << "available: " <<
    this->manager->getCurrentConnection()->available() << std::endl << std::flush;
  std::cout << "running: " << this->monitor->isRunning() << std::endl <<
    std::flush;
  std::cout << "cancelled: " << this->monitor->wasCancelled() << std::endl <<
    std::flush;
  this->line = this->monitor->start();
  std::cout << "step 2" << std::endl << std::flush;
  std::cout << "available: " <<
    this->manager->getCurrentConnection()->available() << std::endl << std::flush;
  std::cout << "running: " << this->monitor->isRunning() << std::endl <<
    std::flush;
  std::cout << "cancelled: " << this->monitor->wasCancelled() << std::endl <<
    std::flush;
  bool printed = false;

  while (this->monitor->isRunning() && !this->monitor->wasCancelled()) {
    if (!printed) {
      std::cout << "Monitor Running" << std::endl;
      printed = true;
    }
  }

  this->line->join();
  delete this->line;
  std::cout << "step 3" << std::endl << std::flush;
  std::cout << "available: " <<
    this->manager->getCurrentConnection()->available() << std::endl << std::flush;
  std::cout << "running: " << this->monitor->isRunning() << std::endl <<
    std::flush;
  std::cout << "cancelled: " << this->monitor->wasCancelled() << std::endl <<
    std::flush;

  std::cout << "monitor not running" << std::endl << std::flush;

  if (!this->monitor->wasCancelled()) {
    std::cout << "here" << std::endl;
    Packet *p;

    p = this->manager->read();

    memcpy(&this->choice, (int *)p->getData(), p->getSize());

    std::cout << "Choice: " << this->choice << std::endl << std::flush;
    emit signalChoice(this->choice);
  }
  this->monitorRunning = false;
  delete this->monitor;
} // Arduino::monitorSerialLine

void Arduino::setFinishDistance(int distance) {
  this->setDistance(distance);
  std::cout << distance << std::endl << std::flush;
  this->dist->setDistance(distance);
}

int Arduino::getFinishDistance() {
  return this->distance;
}

void Arduino::startDistanceMeasurement() {
  this->dist->start();
}

void Arduino::updateDistance(int distance) {
  this->setDistance(distance);
}

int Arduino::getDistance() {
  return this->distance;
}

void Arduino::setDistance(int distance) {
  this->distance = distance;
}

int Arduino::getRaceType() {
  return this->raceType.load();
}

void Arduino::setRaceType(int race) {
  this->raceType = race;
}

void Arduino::startRacer(int bib) {
  std::cout << "Bib #:" << bib << std::endl << std::flush;

  if (this->manager->connect(I2C)) {
    if (this->manager->send(4)) {
      int type =
        (this->getRaceType() == 2 && !this->paceSet) ? 3 : this->getRaceType();

      if (this->manager->send(type)) {
        if (this->manager->send(bib)) {
          if (this->manager->connect(SERIAL)) {
            std::cout << "Ready!" << std::endl << std::flush;

            for (int i = 0;
                 this->manager->getCurrentConnection()->available() <= 0; i++) {
              if (i % 100000 == 0) std::cout << "." << std::flush;
            }
            std::cout << std::endl;
            Packet *p           = this->manager->read();
            unsigned long start = 0;
            memcpy(&start, (unsigned long *)p->getData(), p->getSize());
            std::cout << "Start ms:" << start << std::endl << std::flush;

            if (type == 3) {
              unsigned long ptime = 0;


              while (this->manager->getCurrentConnection()->available() <= 0) ;

              // wait

              p = this->manager->read();
              memcpy(&ptime, (unsigned long *)p->getData(), p->getSize());
              std::cout << "Pace Time: " << ptime << std::endl <<
                std::flush;

              if (ptime > 0) {
                this->setPaceSet(true);
                this->setPace(ptime);
              }
            }
          }
        }
      }
    }
  }
}

void Arduino::dnfRacer(int racer) {
  if (this->manager->send(10)) {}
}

bool Arduino::getIntervalStatus() {
  return this->intervalSignal.load();
}
