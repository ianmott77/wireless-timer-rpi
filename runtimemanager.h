#ifndef RUNTIMEMANAGER_H
#define RUNTIMEMANAGER_H

#include "Connection/ConnectionManager.h"
#include "racer.h"
#include "databasecontroller.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMap>

class RunTimeManager {
public:

  RunTimeManager();
  ~RunTimeManager();
  QJsonObject     toJson(const QString&);
  bool            addRun(Racer *);
  QVector<Racer *>getRacers();
  bool            clearAllRuns();

private:

  DatabaseController *database;
  QVector<Racer *>racers;
};

#endif // RUNTIMEMANAGER_H
