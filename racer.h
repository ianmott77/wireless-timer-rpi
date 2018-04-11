#ifndef RACER_H
#define RACER_H

#include <QObject>
#include <QElapsedTimer>

class Racer : QObject {
public:

  Racer(int,
        unsigned long,
        int,
        int,
        QElapsedTimer *timer = 0);
  int           getBib();
  unsigned long getStartTime();
  int           getStartDelay();
  int           getTime();
  int           getFinishTime();
  int           getRaceMode();
  void          setRaceMode(int);
  void          setFinishTime(int);
  void          start();

private:

  int bib        = -1;
  int startTime  = 0;
  int startDelay = 0;
  int finishTime = 0;
  int raceMode   = 0;
  QElapsedTimer *timer;
};

#endif // RACER_H
