#ifndef DISTANCETHREAD_H
#define DISTANCETHREAD_H

#include "Connection/ConnectionManager.h"
#include <atomic>
#include <QQuickItem>
#include <QThread>
#include <QElapsedTimer>

class DistanceThread : public QThread {
  Q_OBJECT
  Q_PROPERTY(int distance READ getDistance WRITE setDistance NOTIFY updateDistance)

public:

  DistanceThread(ConnectionManager *);
  int                getDistance();
  bool               isDone();
  bool               isRunning();
  void               setRunning(bool);
  void               setDone(bool);
  ConnectionManager* getManager();

signals:

  void started();
  void updateDistance(int);
  void lock();
  void finished();

public slots:

  void interrupt();
  void setDistance(int);

private:

  ConnectionManager *cm;
  std::atomic<bool>running;
  std::atomic<bool>done;
  int distance = 1;

protected:

  void run();
};

#endif // DISTANCETHREAD_H
