
#include "arduino.h"

Arduino::Arduino(ConnectionManager * manager) : Device(this->getID(), this->getPosition())
{
    this->manager =manager;
    this->intervalSignal = false;
    this->dist = new DistanceThread(this->manager);
    this->getInfo();
}

Arduino::~Arduino(){}

void Arduino::getInfo(){
    if(this->manager->connect(I2C)){
        if(this->manager->send(200, 0)){
            int data =  0;
            Packet * pack = 0;

            pack =  this->manager->read();
            memcpy(&data, (int*) pack->getData(), pack->getSize());

            this->setID(data);

           pack =  this->manager->read();
           memcpy(&data, (int*) pack->getData(), pack->getSize());

           this->setPosition(data);

           pack =  this->manager->read();
           memcpy(&data, (int*) pack->getData(), pack->getSize());

           this->setNextAddress(data);

           pack =  this->manager->read();
           memcpy(&data, (int*) pack->getData(), pack->getSize());

           this->setPrevAddress(data);

           pack =  this->manager->read();
           memcpy(&data, (int*) pack->getData(), pack->getSize());

           this->setType(data);

           this->manager->send(201, 0);
        }
    }
}

int Arduino::getType(){
    return this->type;
}

void Arduino::setType(int type){
    this->type = type;
}
void Arduino::setNextAddress(int next){
    this->nextAddress = next;
}

void Arduino::setPrevAddress(int prev){
    this->prevAddress = prev;
}

int Arduino::getNextAddress(){
    return this->nextAddress;
}

int Arduino::getPrevAddress(){
    return this->prevAddress;
}

void Arduino::sync(){
    if(this->manager->connect(I2C)){
       if(this->manager->send(2)){
          if( this->manager->connect(SERIAL)){
                 while(this->manager->getCurrentConnection()->available() <= 0);
                Packet * p = this->manager->read();
                int done = 0;
                memcpy(&done, (int*) p->getData(), p->getSize());
            }
        }
    }
}

bool Arduino::addDevice(int ID, int position, Device * device){
    if(ID != -1 && position != -1){
        if(this->manager->connect(I2C)){
             //send 1 to say we're adding a device
             if(this->manager->send(1)){
                    //send the parameters of the device to add
                 if(this->manager->send(ID)){
                    if(this->manager->send(position)){
                        if(this->manager->connect(SERIAL)){
                            Packet * p = 0;
                            int confirm = 0;
                            bool print = false;
                            while(this->manager->getCurrentConnection()->available() <= 0){
                                if(!print){
                                    std::cout << "Waiting..." << std::endl << std::flush;
                                    print = true;
                                }
                            }
                            std::cout << this->manager->getCurrentConnection()->available()  << std::endl << std::flush;
                            p = this->manager->read();
                            memcpy(&confirm, (int*) p->getData(), p->getSize());
                            emit readyToUpdate();
                            return true;
                        }
                     }else{
                         return false;
                    }
                 }else{
                    return false;
                 }
            }else{
                return false;
            }
        }else{
            return false;
        }
    }else{
        Packet * p  = 0;
        int data = 0;

        p = this-> manager->read();

        if(p != 0){
            memcpy(&data, (int*) p->getData(), p->getSize());
        }else{
            return false;
        }
        device->setID(data);

        p = this->manager->read();

        if(p != 0){
            memcpy(&data, (int*) p->getData(), p->getSize());
        }else{
            return false;
        }
        device->setPosition(data);

        return true;
    }
}

void Arduino::receiveIntervalSignal(){
    SerialMonitor * mon = new SerialMonitor(this->manager);
    std::thread serMon(&SerialMonitor::monitor, mon);
    serMon.join();
    if(!mon->wasCancelled()){
        this->intervalSignal = true;
    }
}

SerialMonitor * Arduino::getMonitor(){
    return this->monitor;
}

ConnectionManager * Arduino::getManager(){
    return this->manager;
}

void Arduino::stopSerialLine(){
    this->monitor->cancel();
}

std::thread *  Arduino::startSerialLine(){
    return  new  std::thread( [this] {
        this->monitorSerialLine();
    } );
}
void Arduino::setIntervalStatus(bool status){
    this->intervalSignal = status;
}
void Arduino::monitorSerialLine(){
    this->monitor = new SerialMonitor(this->manager);

    bool a = false;

    while(this->dist->isRunning()){
        if(!a){
            std::cout << "dt still running" << std::endl << std::flush;
            a = true;
        }
    }

    std::cout << "step 1" << std::endl <<std::flush;

   std::cout <<"available: " << this->manager->getCurrentConnection()->available() << std::endl<< std::flush;
   std::cout << "running: " <<this->monitor->isRunning()<< std::endl<< std::flush;
   std::cout << "cancelled: " <<this->monitor->wasCancelled() << std::endl << std::flush;
   this->line = this->monitor->start();
   std::cout << "step 2" << std::endl <<std::flush;
   std::cout <<"available: " << this->manager->getCurrentConnection()->available() << std::endl<< std::flush;
   std::cout << "running: " <<this->monitor->isRunning()<< std::endl<< std::flush;
   std::cout << "cancelled: " <<this->monitor->wasCancelled() << std::endl << std::flush;
   bool printed = false;

   while(this->monitor->isRunning() && !this->monitor->wasCancelled()){
        if(!printed){
            std::cout << "Monitor Running" << std::endl;
            printed = true;
        }
    }

    this->line->join();
    delete this->line;
   std::cout << "step 3" << std::endl <<std::flush;
   std::cout <<"available: " << this->manager->getCurrentConnection()->available() << std::endl<< std::flush;
   std::cout << "running: " <<this->monitor->isRunning()<< std::endl<< std::flush;
   std::cout << "cancelled: " <<this->monitor->wasCancelled() << std::endl << std::flush;

    std::cout << "monitor not running" << std::endl << std::flush;

    if(!this->monitor->wasCancelled()){
        Packet * p;

        p = this->manager->read();

        memcpy(&this->choice, (int*) p->getData(), p->getSize());

        std::cout << "Choice: " <<  this->choice << std::endl << std::flush;
        emit signalChoice(this->choice);
   }
    delete this->monitor;
}

void Arduino::setFinishDistance(int distance){}

int Arduino::getFinishDistance(){
    return this->distance;
}

void Arduino::startDistanceMeasurement(){
    this->dist->start();
}

void Arduino::updateDistance(int distance){
    this->setDistance(distance);
}

int Arduino::getDistance(){
    return this->distance;
}

void Arduino::setDistance(int distance){
    this->distance = distance;
}

void Arduino::startRacer(int bib){
    std::cout << "bib #:" << std::endl;
    if(this->manager->connect(I2C)){
        if(this->manager->send(4)){
             if(this->manager->send(bib)){
                 if(this->manager->connect(SERIAL)){
                    std::cout << "Ready!" << std::endl;
                    for(int i = 0; this->manager->getCurrentConnection()->available() <= 0; i++){
                        if(i%100000 == 0)
                            std::cout << "." << std::flush;
                    }
                    std::cout << std::endl;
                    Packet * p = this->manager->read();
                    unsigned long start = 0;
                    memcpy(&start, (unsigned long*) p->getData(), p->getSize());
                    std::cout << "Start ms:" << start << std::endl;
                }
            }
        }
    }
}

bool Arduino::getIntervalStatus(){
    return this->intervalSignal.load();
}
