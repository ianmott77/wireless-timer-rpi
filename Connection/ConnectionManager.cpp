#include "ConnectionManager.h"

ConnectionManager::ConnectionManager() {
  this->factory = new ConnectionFactory();
}

ConnectionManager::~ConnectionManager() {
  delete this->factory;
}

void ConnectionManager::setCurrentConnection(Connection *c) {
  this->connections[this->currentConnection] = c;
}

Connection * ConnectionManager::getCurrentConnection() {
  return this->connections[this->currentConnection];
}

bool ConnectionManager::connect(ConnectionType type) {
  this->currentConnection = type;
  this->setCurrentConnection(this->factory->newConnection(this->currentConnection));

  if (!this->getCurrentConnection()->setup()) {
    return false;
  }
  return true;
}

void ConnectionManager::switchTo(ConnectionType type) {
  this->currentConnection = type;
}

bool ConnectionManager::send(Packet *pack) {
  if (this->getCurrentConnection()->send(pack->getSize())) {
    if (this->getCurrentConnection()->send(pack->getDataType())) {
      if (this->getCurrentConnection()->send(pack->getPosition())) {
        if (pack->getDataType() == INT) {
          if (this->getCurrentConnection()->send(*(int *)pack->getData())) {
            return true;
          }
        } else if ((pack->getDataType() == LONG) ||
                   (pack->getDataType() == ULONG)) {
          if (this->getCurrentConnection()->send(*(uint32_t *)pack->getData())) {
            return true;
          }
        } else if (pack->getDataType() == STRING) {
          if (this->getCurrentConnection()->send((const char *)pack->getData())) {
            return true;
          }
        } else {
          std::cout << "Invalid data type" << std::endl << std::flush;
          return false;
        }
      } else {
        // failed to send dataType
        std::cout << "Failed to send position" << std::endl << std::flush;
        std::cout << "Write Error # " << errno << ":" << strerror(errno) <<
          std::endl << std::flush;
        return false;
      }
    } else {
      // failed to send dataType
      std::cout << "Failed to send data type" << std::endl << std::flush;
      std::cout << "Write Error # " << errno << ":" << strerror(errno) <<
        std::endl << std::flush;
      return false;
    }
  } else {
    // failed to send size
    std::cout << "Failed to send size" << std::endl << std::flush;
    std::cout << "Write Error # " << errno << ":" << strerror(errno) <<
      std::endl << std::flush;
    return false;
  }
  tcdrain(this->getCurrentConnection()->file);
}

bool ConnectionManager::send(int data, int position) {
  return this->send(new Packet(&data, INT, sizeof(data), position));
}

bool ConnectionManager::send(long data, int position) {
  return this->send(new Packet(&data, LONG, sizeof(data), position));
}

bool ConnectionManager::send(uint32_t data, int position) {
  return this->send(new Packet(&data, ULONG, sizeof(data), position));
}

bool ConnectionManager::send(unsigned long data, int position) {
  return this->send(new Packet(&data, ULONG, sizeof(data), position));
}

bool ConnectionManager::send(float data, int position) {
  return this->send(new Packet(&data, FLOAT, sizeof(data), position));
}

bool ConnectionManager::send(std::string data, int position) {
  return this->send(new Packet((void *)data.c_str(), STRING, data.length(),
                               position));
}

Packet * ConnectionManager::read() {
  return this->receive();
}

void ConnectionManager::closeConnection() {
  this->getCurrentConnection()->closeConnection();
}

Packet * ConnectionManager::receive() {
  int size = this->getCurrentConnection()->readInt();

  if (size > 0) {
    DataType dataType = (DataType)this->getCurrentConnection()->readInt();
    int position      = this->getCurrentConnection()->readInt();

    if (dataType == INT) {
      int data = this->getCurrentConnection()->readInt();
      return new Packet(&data, dataType, size, position);
    } else if (dataType == FLOAT) {
      float data = this->getCurrentConnection()->readLong();
      return new Packet(&data, dataType, size, position);
    } else if (dataType == LONG) {
      signed long data = this->getCurrentConnection()->readLong();
      return new Packet(&data, dataType, size, position);
    } else if (dataType == ULONG) {
      unsigned long data = this->getCurrentConnection()->readULong();
      return new Packet(&data, dataType, size, position);
    } else if (dataType == STRING) {
      std::string data = this->getCurrentConnection()->readString(size);
      return new Packet((char *)data.c_str(), dataType, size, position);
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}
