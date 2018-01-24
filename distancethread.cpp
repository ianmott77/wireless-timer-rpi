#include "distancethread.h"

DistanceThread::DistanceThread(ConnectionManager * manager){
    this->cm = manager;
    this->running = false;
    this->done = false;
}

void DistanceThread::setDone(bool done){
    this->done = done;
}

void DistanceThread::setRunning(bool running){
    this->running = running;
}

int DistanceThread::getDistance(){
    return this->distance;
}

void DistanceThread::setDistance(int distance){
    this->distance = distance;
}

void DistanceThread::interrupt(){
    this->running = false;
}

bool DistanceThread::isDone(){
    return this->done.load();
}

bool DistanceThread::isRunning(){
    return this->running.load();
}

ConnectionManager * DistanceThread::getManager(){
    return this->cm;
}

void DistanceThread::run(){
    emit started();
    this->running = true;
    if(this->cm->connect(I2C)){
        if(this->cm->send(204)){
            if(this->cm->connect(SERIAL)){
                int distance = 0;
                int set = 0;
                for(int i = 0; this->running; i++){
                    if(this->cm->send(1)){
                        Packet * p = this->cm->read();
                        if(p != 0){
                            memcpy(&distance, (int*) p->getData(), p->getSize());
                             if(distance >= 0){
                                 if(i == 0){
                                     set = distance;
                                     emit this->updateDistance(set);
                                 }
                                 if(distance > set + 5 || distance < set- 5){
                                        set = distance;
                                        emit this->updateDistance(set);
                                 }
                             }
                        }
                    }
                }
                this->distance = set;
                if(this->cm->send(this->distance)){
                    emit this->lock();
                    while(this->cm->getCurrentConnection()->available() > 0){
                        Packet * p =this->cm->read();
                        int data = 0;
                        memcpy(&data, (int*) p->getData(), p->getSize());
                    }
                    this->done = true;
                    this->running = false;
                    std::cout << "finished" << std::endl << std::flush;
                    sleep(2);
                    emit finished();
                }
            }
        }
    }
}
