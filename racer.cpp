#include "racer.h"

Racer::Racer(int bib, unsigned long startTime, int startDelay, QElapsedTimer * timer){
    this->bib = bib;
    this->startTime = startTime;
    this->startDelay = startDelay;
    this->timer = timer;
}

void Racer::start(){
    this->timer->start();
}

int Racer::getBib(){
    return this->bib;
}

int Racer::getStartDelay(){
    return this->startDelay;
}

unsigned long Racer::getStartTime(){
    return this->startTime;
}

int Racer::getTime(){
    return this->startDelay +this->timer->elapsed();
}
