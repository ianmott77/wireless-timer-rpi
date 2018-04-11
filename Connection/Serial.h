#ifndef SERIAL_H
#define SERIAL_H

#include <termios.h>
#include "Connection.h"


class Serial : public Connection {
public:

  Serial();
  ~Serial();
  bool           setup();
  ConnectionType getType();
  bool           send(long);
  int            readInt();
  long           readLong();
  unsigned long  readULong();
  float          readFloat();
};

#endif // ifndef SERIAL_H
