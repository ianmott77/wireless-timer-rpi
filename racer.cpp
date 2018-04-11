#include "racer.h"

Racer::Racer(int bib,
             unsigned long startTime,
             int startDelay, int raceMode,
             QElapsedTimer *timer) {
  this->bib        = bib;
  this->startTime  = startTime;
  this->startDelay = startDelay;
  this->timer      = timer;
  this->raceMode   = raceMode;
}

void Racer::start() {
  this->timer->start();
}

int Racer::getBib() {
  return this->bib;
}

int Racer::getStartDelay() {
  return this->startDelay;
}

unsigned long Racer::getStartTime() {
  return this->startTime;
}

int Racer::getTime() {
  return this->startDelay + this->timer->elapsed();
}

void Racer::setFinishTime(int time) {
  this->finishTime = time;
}

int Racer::getFinishTime() {
  return this->finishTime;
}

int Racer::getRaceMode() {
  return this->raceMode;
}

void Racer::setRaceMode(int mode) {
  this->raceMode = mode;
}
