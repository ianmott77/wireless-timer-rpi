#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QElapsedTimer>
#include "arduino.h"

class UIController : public QObject
{
    Q_OBJECT


public:
    explicit UIController(Arduino *, QQmlApplicationEngine *,  QObject * =0 , QObject *parent = nullptr);
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

public slots:
    void setFinishDistance(int distance);
    int getFinishDistance();
    void startDistanceMeasurement();
    void updateDistance(int);
    int getDistance();
    void setDistance(int);
    bool addDevice(int, int);
    void startSerialLine();
    void stopSerialLine();
    void choiceDispatch(int);
     void updateInfo();
     QString getLoadingMsg();
     void setLoadingMsg(QString);
      void stop(QString reason = "");
      void start();
      void startRacer(int);
      int getCurrentTime();
      int getCurrentBib();
      void setCurrentBib(int);
private:
    int distance = -1;
    int bib = -1;
   std::atomic<int> currentTime;
    Device * tempDevice;
    std::thread * serialLine = 0;
    Arduino * arduino = 0;
    ConnectionManager * manager = 0;
    QQmlApplicationEngine * engine;
    QObject * qml = 0;
    QString loadingMsg;
};

#endif // UICONTROLLER_H
