#include "device.h"

Device::Device(int ID, int position)
{
    this->ID = ID;
    this->position = position;
}

Device::Device()
{
    this->ID = -1;
    this->position = -1;
}

int Device::getID(){
    return this->ID;
}

int Device::getPosition(){
    return this->position;
}

void Device::setID(int ID){
    this->ID  = ID;
}

void Device::setPosition(int position){
    this->position = position;
}
