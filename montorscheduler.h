#ifndef MONTORSCHEDULER_H
#define MONTORSCHEDULER_H

#include "Connection/SerialMonitor.h"

class MontorScheduler
{
public:
    MontorScheduler(SerialMonitor *);
    SerialMonitor * getMonitor();
    void start();
    void stop();
private:
    SerialMonitor * monitor;
};

#endif // MONTORSCHEDULER_H
