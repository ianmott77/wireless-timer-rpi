#include "uicontroller.h"

UIController::UIController(Arduino               *arduino,
                           QQmlApplicationEngine *engine,
                           QObject               *qml,
                           QObject               *parent) : QObject(parent)
{
  this->distance = 0;
  this->arduino  = arduino;
  this->manager  = this->arduino->getManager();
  this->engine   = engine;
  this->rtm      = new RunTimeManager();

  if (this->arduino->getType() == 2) {
    this->battery = new BatteryMonitor();
    QObject::connect(this->battery,
                     SIGNAL(updatePowerStatus(float,QString)),
                     this,
                     SIGNAL(batteryStatusChanged(float,QString)));
  }
  QObject::connect(this->arduino, SIGNAL(signalChoice(int)), this,
                   SLOT(choiceDispatch(int)));
  QObject::connect(this->arduino->dist, SIGNAL(updateDistance(int)), this,
                   SLOT(updateDistance(int)));
  QObject::connect(this->arduino->dist, SIGNAL(finished()), this,
                   SLOT(start()));
  QObject::connect(this, SIGNAL(busy(QString)), this,
                   SLOT(stop(QString)));
  QObject::connect(this, SIGNAL(free()), this,
                   SLOT(start()));
  QObject::connect(this->arduino, SIGNAL(readyToUpdate()), this,
                   SLOT(updateInfo()));
  QObject::connect(this->arduino, SIGNAL(error(int)), this,
                   SLOT(error(int)));

  this->qml = qml;
  this->startSerialLine();
}

UIController::~UIController() {
  delete this->rtm;
}

QString UIController::getLoadingMsg() {
  return this->loadingMsg;
}

void UIController::error(int error) {
  std::cout << "error: " << error << std::endl << std::flush;

  if (error == 200) {}
  emit this->errorSignal(error);
}

void UIController::startRacer(int bib) {
  emit this->busy(QString("Ready!"));
  this->bib = bib;

  while (this->arduino->getMonitor()->isRunning()) ;  // wait
  std::thread *thr = new std::thread([this] {
    this->arduino->startRacer(this->bib);
    emit this->free();
  });
}

void UIController::setLoadingMsg(QString msg) {
  this->loadingMsg = msg;
}

void UIController::start() {
  this->startSerialLine();
  emit this->unlock();
}

void UIController::stop(QString reason) {
  this->setLoadingMsg(reason);

  if (this->arduino->getMonitor()->isRunning()) {
    this->stopSerialLine();
  }
  emit this->lock();
}

bool UIController::addDevice(int ID, int pos) {
  emit this->busy(QString("Adding device"));
  this->tempDevice = new Device(ID, pos);

  while (this->arduino->getMonitor()->isRunning()) ;  // wait

  if (this->arduino->addDevice(this->tempDevice->getID(),
                               this->tempDevice->getPosition())) {
    this->arduino->sync();
  }
  emit free();
}

void UIController::choiceDispatch(int choice) {
  Packet *p = 0;

  if (choice == 1) {
    emit busy(QString("Being added to network"));
    std::thread *thr = new std::thread([this] {
      Packet *p = 0;
      Device *device = new Device();

      if (this->arduino->addDevice(-1, -1, device)) {
        std::cout << "Device : " << device->getID() << "-" << device->getPosition() << std::endl << std::flush;

        if (this->arduino->getManager()->connect(SERIAL)) {
          if (this->arduino->getManager()->send(1)) {
            while (!this->arduino->getManager()->getCurrentConnection()->available()) ;

            //
            // wait
            int confirm = 0;
            p = this->arduino->getManager()->read();
            memcpy(&confirm, (int *)p->getData(), p->getSize());

            if (confirm == 1) {
              this->updateInfo();
              emit free();
            } else {
              emit error(confirm);
              emit free();
            }
          }
        }
      }
    });
  } else if (choice == 2) {
    this->synchronize();
  } else if (choice == 4) {
    emit this->raceMode();
    this->goRaceMode();
  }
} // UIController::choiceDispatch

bool UIController::synchronize() {
  emit busy(QString("Synchronizing"));

  std::thread *thr = new std::thread([this] {
    Packet *p = 0;

    if (this->manager->connect(SERIAL)) {
      std::cout << "Synchronizing" << std::flush;

      for (int i = 0; this->manager->getCurrentConnection()->available() <= 0;
           i++) {
        if (i % 100000 == 0) std::cout << "." << std::flush;
      }
      int done = 0;
      p = this->manager->read();
      memcpy(&done, (int *)p->getData(),
             p->getSize());

      if (done == 5) {
        emit this->arduino->syncDone();
      }
      emit free();
    }
  });
}

Racer * UIController::getLastRacer() {
  return this->arduino->getTimeQueue()->head();
}

void UIController::goRaceMode() {
  std::thread *thr = new std::thread([this] {
    Packet *p = 0;
    uiCounter = false;
    std::thread *th;

    while (this->arduino->getMonitor()->isRunning()) ;  // wait

    std::cout << "Run Started" << std::endl << std::flush;
    Racer *r = this->arduino->getRacerInfo();
    std::cout << "Bib #: " << r->getBib() << std::endl << "Start ms: " << r->getStartTime() << std::endl << "Current Time: " << r->getStartDelay() << std::endl << std::flush;
    std::thread *thra =
      new std::thread(&Arduino::receiveIntervalSignal, this->arduino);
    emit this->newRacerOnCourse(r->getBib(), r->getStartTime(), r->getTime());


    // this is the ui timer
    QElapsedTimer timer;
    timer.start();
    int last = timer.elapsed();
    bool done = false;
    bool withinPace = true;

    while (!done) {
      int now = timer.elapsed();

      // check if we have already started the thread to updat the current time
      if (!uiCounter) {
        this->setCurrentBib(this->getLastRacer()->getBib());
        uiCounter = true;
        th = new std::thread([this] {
          while (uiCounter) {
            this->currentTime = this->getLastRacer()->getTime();
          }
        });
      }

      // we need to allow a small amount of time for the PC to update otherwise
      // it glitches
      if (now - last > 50) {
        emit this->nextToFinish(this->getLastRacer()->getBib(), this->getLastRacer()->getTime());

        for (int i = 0; i < this->arduino->getTimeQueue()->size(); i++) {
          Racer *r =  this->arduino->getTimeQueue()->at(i);
          emit this->racerOnCourse(r->getBib(), r->getTime());
        }

        last = timer.elapsed();
      }

      if (withinPace && ((this->getLastRacer()->getRaceMode() == 2) &&
                         (this->getLastRacer()->getTime() >
                          this->arduino->getPace() + 10000)) &&
          this->arduino->isPaceSet()) {
        withinPace = false;
        this->arduino->dnfRacer(this->getLastRacer()->getBib());
        std::cout << "Time is now outside the pace: "  << this->getLastRacer()->getTime()  << std::endl << std::flush;
      }

      // check if we received a signal from the arduino either to add a new
      // racer to the stack or that someone has crossed the finish
      if (this->arduino->getIntervalStatus()) {
        while (this->arduino->getMonitor()->isRunning()) ;  // wait

        p = this->manager->read();
        int option = 0;
        memcpy(&option,
               (int *)p->getData(),
               p->getSize());
        std::cout << "option: " << option << std::endl << std::flush;

        if (option == 1) { // somone crossed the finish line
          uiCounter = false;
          th->join();

          int interval = 0;
          int racerCount = 0;

          p = this->manager->read();
          memcpy(&interval, (int *)p->getData(), p->getSize());

          std::cout << "Interval: " << interval << std::endl << std::flush;

          p = this->manager->read();
          memcpy(&racerCount, (int *)p->getData(), p->getSize());

          this->currentTime = (interval == 0) ? 0 : interval - this->getLastRacer()->getStartTime();

          withinPace = true;

          /* we need to check if the race type is pace mode and make sure that
             it's not a DNF*/

          std::cout << "Race Mode: " << this->getLastRacer()->getRaceMode()  << std::endl << std::flush;

          if ((this->getLastRacer()->getRaceMode() == 2) &&
              (this->currentTime > 0) && this->arduino->isPaceSet()) {
            if (this->getLastRacer()->getTime()  <
                this->arduino->getPace() - 10000) {
              withinPace = false;
            }
          }

          Racer *finisher = 0;

          if (!withinPace && (this->getLastRacer()->getRaceMode() == 2) &&
              this->arduino->isPaceSet()) this->currentTime = 1;
          std::string yn =  (withinPace) ? "Yes" : "No";
          std::cout << "Is within pace time: " << yn << std::endl << std::flush;

          if (this->manager->send((uint32_t)this->currentTime)) {
            if (withinPace || (this->getLastRacer()->getRaceMode() != 2)) {
              finisher = this->arduino->getTimeQueue()->dequeue();
              finisher->setFinishTime(interval);

              if (finisher->getRaceMode() == 3) { // setting the pace
                uint32_t pace = (uint32_t)this->currentTime;
                std::cout << "pace time: " << pace << std::endl << std::flush;

                if (this->manager->connect(SERIAL)) {
                  if (!this->manager->send(
                        pace)) {
                    std::cout << "Failed to send pace time" << std::endl << std::flush;
                  }
                  this->arduino->setPace(pace);
                  this->arduino->setPaceSet(true);
                  std::cout << "Pace Time: " << this->arduino->getPace() << std::endl << std::flush;
                }
              }

              emit this->finished(finisher->getBib(), finisher->getStartTime(), finisher->getFinishTime());

              double t = (this->currentTime == 0) ? 0 : this->currentTime / 1000.00;

              if (this->rtm->addRun(finisher)) {
                std::cout << "Time saved!" << std::endl << std::flush;
              }

              if (this->currentTime == 0) emit this->dnfRacer(finisher->getBib());

              std::cout << "Bib #: " << finisher->getBib() << std::endl << "Start ms: " << finisher->getStartTime() << std::endl << "Time ms: " << this->currentTime << std::endl << "Time: " << t << std::endl << std::flush;
              std::cout << "Racer Count: " << racerCount << std::endl;
              withinPace = true;
            }
          }
        } else if (option == 4) { // a new racer is on the track
          std::cout << "Run Started" << std::endl << std::flush;
          Racer *starter = this->arduino->getRacerInfo();
          std::cout << "Bib #: " << starter->getBib() << std::endl << "Start ms: " << starter->getStartTime() << std::endl << "Current Time: " << starter->getTime() << std::endl << std::flush;
          emit this->newRacerOnCourse(starter->getBib(), starter->getStartTime(), starter->getTime());
        }

        this->arduino->setIntervalStatus(false);

        if (this->arduino->getTimeQueue()->size() > 0) {
          thra = new std::thread(&Arduino::receiveIntervalSignal, this->arduino);
        } else {
          done = true;
          emit this->doneRaceMode();
          emit this->free();
        }
      }
    }
  });
} // UIController::goRaceMode

int UIController::getCurrentBib() {
  return this->bib;
}

int UIController::getCurrentTime() {
  return this->currentTime.load();
}

void UIController::setCurrentBib(int bib) {
  this->bib = bib;
}

void UIController::startSerialLine() {
  this->arduino->startSerialLine();
  std::cout << "Started Monitor" << std::endl << std::flush;
}

void UIController::stopSerialLine() {
  this->arduino->stopSerialLine();

  while (this->arduino->isMonitorRunning()) ;  // wait
  std::cout << "Stopped Monitor" << std::endl << std::flush;
}

int UIController::getFinishDistance() {
  return this->arduino->getFinishDistance();
}

void UIController::setFinishDistance(int distance) {
  this->arduino->setFinishDistance(distance);
}

void UIController::startDistanceMeasurement() {
  this->stopSerialLine();
  this->arduino->startDistanceMeasurement();
  emit measurementStarted();
}

void UIController::updateDistance(int distance) {
  this->arduino->setDistance(distance);
  emit distanceChanged(distance);
}

int UIController::getDistance() {
  return this->arduino->getDistance();
}

void UIController::setDistance(int distance) {
  this->arduino->setDistance(distance);
}

QVector<Racer *>UIController::getRacers() {
  return this->rtm->getRacers();
}

int UIController::getNumRacers() {
  return this->rtm->getRacers().size();
}

QJsonObject UIController::racerAtToJson(int index) {
  QJsonObject result;

  Racer *r = this->rtm->getRacers().at(index);

  result.insert("bib",         r->getBib());
  result.insert("start_time",  (int)r->getStartTime());
  result.insert("finish_time", r->getFinishTime());
  return result;
}

void UIController::setQML(QObject *qml) {
  this->qml = qml;
}

void UIController::updateInfo() {
  this->arduino->getInfo();
  emit this->infoChanged();
}

void UIController::batteryStatusUpdate(float level,  QString source) {
  emit this->batteryStatusChanged(level, source);
}

bool UIController::clearAllRuns() {
  return this->rtm->clearAllRuns();
}
