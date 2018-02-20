#ifndef SERIAL_MONITOR_H
#define SERIAL_MONITOR_H

#include "ConnectionManager.h"
#include <thread>
#include <atomic>

class SerialMonitor{
	public:
		SerialMonitor(ConnectionManager *);
		~SerialMonitor();
		void monitor();
		void cancel();
        bool wasCancelled();
        bool isRunning();
        void reset();
        ConnectionManager * getManager();
        std::thread *  start();

	private:
        bool sendAck();
		ConnectionManager * manager;
		Packet * pac;
        std::atomic<bool> cont;
        std::atomic<bool> running;
};

#endif
