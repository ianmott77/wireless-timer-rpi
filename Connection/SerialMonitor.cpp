#include "SerialMonitor.h"

SerialMonitor::SerialMonitor(ConnectionManager * manager){
	this->manager = manager;
	this->cont = true;
    this->running = true;
}

SerialMonitor::~SerialMonitor(){}

std::thread * SerialMonitor::start(){
    return   new std::thread( [this] {
        this->monitor();
    } );
}

bool SerialMonitor::sendAck(){
	if(this->manager->send(1)){
		return true;
	}
	return false;
}

bool SerialMonitor::isRunning(){
    return this->running.load();
}

void SerialMonitor::cancel(){
	this->cont = false;
}

bool SerialMonitor::wasCancelled(){
    if(this->cont.load() == true)
	 return false;
 	else
	  return true;
}

void SerialMonitor::reset(){
    this->cont = true;
    this->running = true;
}

void SerialMonitor::monitor(){
	if(this->manager->connect(SERIAL)){
        while(this->manager->getCurrentConnection()->available() <= 0 && !this->wasCancelled());
		if(this->cont){
			this->pac = this->manager->read();
            if(this->pac != 0){   
                delete this->pac;
                if(!this->sendAck()){
                    std::cout << "Failed to send ACK" << std::endl<< std::flush ;
                }

            }else{
                std::cout << "Received corrupt packet" << std::endl << std::flush;
            }
		}
    } else{
        std::cout << "Failed to connect"  << std::endl<< std::flush;
    }
    this->running = false;
}

ConnectionManager * SerialMonitor::getManager(){
    return this->manager;
}
