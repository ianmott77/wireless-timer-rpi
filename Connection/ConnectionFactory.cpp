#include "ConnectionFactory.h"

ConnectionFactory::ConnectionFactory(){
    this->lastConnection = 0;
}

ConnectionFactory::~ConnectionFactory(){
    delete this->lastConnection;
}

Connection * ConnectionFactory::newConnection(ConnectionType type){
    if(type == SPI){
        this->lastConnection = new Spi();
	}else if(type == I2C){
        this->lastConnection =  new i2c();
	}else if(type == SERIAL){
        this->lastConnection = new Serial();
	}
    return this->lastConnection;
}
