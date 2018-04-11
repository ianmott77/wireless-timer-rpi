#include "Serial.h"

Serial::Serial() : Connection() {}

Serial::~Serial() {}

bool Serial::setup()
{
  struct termios tty;
  struct termios tty_old;

  bool connected = false;

  this->filename = "/dev/serial0";

  this->file = open(this->filename.c_str(), O_RDWR | O_NOCTTY);

  if (this->file >= 0) {
    connected = true;
  }

  if (!connected) {
    std::cout << "Open device error:" << strerror(errno) << std::endl <<
      std::flush;
    return false;
  }

  memset(&tty, 0, sizeof tty);

  if (tcgetattr(this->file, &tty) != 0) {
    printf("tcgetattr error: %s\n", strerror(errno));
    return false;
  }

  tty_old = tty;

  cfsetospeed(&tty, (speed_t)B115200);
  cfsetispeed(&tty, (speed_t)B115200);


  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag &= ~HUPCL;
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cflag |= (CREAD | CLOCAL);

  /*
   * non-canonical mode
   * tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | IGNCR | INLCR| ICRNL |
   * IXON);
   * tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
   * tty.c_oflag &= ~OPOST;
   */

  tty.c_cc[VMIN]  = 1;
  tty.c_cc[VTIME] = 1;

  cfmakeraw(&tty);

  tcflush(this->file, TCIOFLUSH);


  if (tcsetattr(this->file, TCSAFLUSH, &tty) != 0) {
    printf("tcsetattr error: %s\n", strerror(errno));
    return false;
  }

  return true;
} // Serial::setup

ConnectionType
Serial::getType()
{
  return SERIAL;
}

int Serial::readInt()
{
  uint8_t buf[2];

  for (int i = 0; i < 2; i++) {
    if (read(this->file, &buf[i], 1) < 0) {
      std::cout << "Read error #" << errno << " : " << strerror(errno) <<
        std::endl << std::flush;
      return -1;
    }
  }
  int r = buf[0] << 8;
  r |= buf[1] & 0xFF;
  return r;
}

long Serial::readLong()
{
  union {
    uint8_t buf[4];
    long    val;
  } long_union;

  for (int i = 0; i < 4; i++) {
    if (read(this->file, &long_union.buf[i], 1) <= 0) {
      std::cout << "Read error #" << errno << " : " << strerror(errno) <<
        std::endl << std::flush;
      return -1;
    }
  }
  return long_union.val;
}

unsigned long Serial::readULong()
{
  union {
    uint8_t       buf[4];
    unsigned long val;
  } long_union;

  for (int i = 0; i < 4; i++) {
    if (read(this->file, &long_union.buf[i], 1) <= 0) {
      std::cout << "Read error #" << errno << " : " << strerror(errno) <<
        std::endl << std::flush;
      return -1;
    }
  }
  return long_union.val;
}

float Serial::readFloat()
{
  union {
    uint8_t buf[4];
    float   val;
  } float_union;

  for (int i = 0; i < 4; i++) {
    if (read(this->file, &float_union.buf[i], 1) <= 0) {
      std::cout << "Read error #" << errno << " : " << strerror(errno) <<
        std::endl << std::flush;
      return -1;
    }
  }

  return float_union.val;
}

bool Serial::send(long data) {
  union {
    uint8_t buf[4];
    long    val;
  } long_union;

  long_union.val = data;

  std::cout << "dfadfasdfa" << std::endl << std::flush;

  if (write(this->file, long_union.buf, 4) != 4) {
    std::cout << "write error #" << errno << " : " << strerror(errno) <<
      std::endl << std::flush;
    return false;
  }
  return true;
}
