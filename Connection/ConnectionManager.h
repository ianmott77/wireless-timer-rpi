#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H
#include <typeinfo>
#include <sys/wait.h>

#include "ConnectionFactory.h"
#include "Packet.h"

class ConnectionManager {
public:

  ConnectionManager();
  ~ConnectionManager();
  bool connect(ConnectionType);
  Connection* getCurrentConnection();
  void        setCurrentConnection(Connection *);
  void switchTo(ConnectionType);
  bool        send(Packet *);
  bool send(uint32_t,
            int position = 0);
  bool        send(int,
                   int position = 0);
  bool        send(long,
                   int position = 0);
  bool        send(unsigned long,
                   int position = 0);
  bool        send(float,
                   int position = 0);
  bool        send(std::string,
                   int position = 0);

  Packet    * read();
  Packet    * receive();
  void        closeConnection();

private:

  Connection *connections[3];
  ConnectionType currentConnection;
  ConnectionFactory *factory;
};

#endif // ifndef CONNECTION_MANAGER_H
