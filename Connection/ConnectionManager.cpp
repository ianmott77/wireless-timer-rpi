#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(){
	this->factory = new ConnectionFactory();
}

ConnectionManager::~ConnectionManager(){
    delete this->factory;
}

void ConnectionManager::setCurrentConnection(Connection * c){
	this->connections[this->currentConnection] = c;
}

Connection * ConnectionManager::getCurrentConnection(){
	return this->connections[this->currentConnection];
}
	
bool ConnectionManager::connect(ConnectionType type){
	this->currentConnection = type;
	this->setCurrentConnection(this->factory->newConnection(this->currentConnection));
	if(!this->getCurrentConnection()->setup()){
		return false;
	}
	return true;
}

void ConnectionManager::switchTo(ConnectionType type){
	this->currentConnection = type;
}

bool ConnectionManager::send(Packet * p){
	if(this->getCurrentConnection()->send(p))
		return true;
	return false;
}

bool ConnectionManager::send(int data, int position){
	return this->send(new Packet(&data, INT, sizeof(data), position));
}

bool ConnectionManager::send(long data, int position){
	return this->send(new Packet(&data, LONG, sizeof(data), position));
}

bool ConnectionManager::send(float data, int position){
	return this->send(new Packet(&data, FLOAT, sizeof(data), position));
}

bool ConnectionManager::send(std::string data, int position){
	return this->send(new Packet((void*)data.c_str(), STRING, data.length(), position));
}

Packet * ConnectionManager::read(){
    return this->receive();
}

void ConnectionManager::closeConnection(){
	this->getCurrentConnection()->closeConnection();
}

Packet * ConnectionManager::receive(){
    int size = this->getCurrentConnection()->readInt();
    if(size > 0){
        DataType dataType = (DataType) this->getCurrentConnection()->readInt();
        int position = this->getCurrentConnection()->readInt();
        if(dataType == INT){
            int data = this->getCurrentConnection()->readInt();
            return new Packet(&data, dataType, size, position);
        }else if(dataType == FLOAT){
            float data = this->getCurrentConnection()->readLong();
            return new Packet(&data, dataType, size, position);
        }else if(dataType == LONG){
            signed long data = this->getCurrentConnection()->readLong();
            return new Packet(&data, dataType, size, position);
        }else if(dataType == ULONG){
            unsigned long data = this->getCurrentConnection()->readLong();
            return new Packet(&data, dataType, size, position);
        }else if(dataType == STRING){
            std::string data = this->getCurrentConnection()->readString(size);
            return new Packet((char*) data.c_str(), dataType, size, position);
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}


