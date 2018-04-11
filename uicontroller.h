#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QElapsedTimer>
#include "arduino.h"
#include "runtimemanager.h"

class UIController : public QObject {
  Q_OBJECT

public:

  explicit  UIController(Arduino *,
                         QQmlApplicationEngine *,
                         QObject * = 0,
                         QObject *parent = nullptr);
  ~UIController();
  void setQML(QObject *);

signals:

  void newFinishDistance(int distance);
  void measurementStarted();
  void distanceChanged(int);
  void infoChanged();
  void busy(QString reason = "");
  void free();
  void lock();
  void unlock();
  void raceMode();
  void racerOnCourse(int);
  void errorSignal(int error);
  void dnfRacer(int racer);
  void doneRaceMode();

public slots:

  void        setFinishDistance(int distance);
  int         getFinishDistance();
  void        startDistanceMeasurement();
  void        updateDistance(int);
  int         getDistance();
  void        setDistance(int);
  bool        addDevice(int,
                        int);
  void        startSerialLine();
  void        stopSerialLine();
  void        choiceDispatch(int);
  void        updateInfo();
  QString     getLoadingMsg();
  void setLoadingMsg(QString);
  void        stop(QString reason = "");
  void        start();
  void        startRacer(int);
  int         getCurrentTime();
  int         getCurrentBib();
  void        setCurrentBib(int);
  void        goRaceMode();
  Racer     * getLastRacer();
  void        error(int error);
  QVariantMap getRacerJSON();

private:

  int distance = -1;
  int bib      = -1;
  std::atomic<int>currentTime;
  Device *tempDevice;
  RunTimeManager *rtm        = 0;
  std::thread *serialLine    = 0;
  Arduino *arduino           = 0;
  ConnectionManager *manager = 0;
  QQmlApplicationEngine *engine;
  QObject *qml = 0;
  QString loadingMsg;
  std::atomic<bool>uiCounter;
};

#endif // UICONTROLLER_H
