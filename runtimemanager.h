#ifndef RUNTIMEMANAGER_H
#define RUNTIMEMANAGER_H

#include "Connection/ConnectionManager.h"
#include "racer.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMap>

class RunTimeManager {
public:

  RunTimeManager();
  ~RunTimeManager();
  QJsonDocument     * getJsonDoc();
  QJsonObject         toJson();
  void                addTime(Racer *);
  bool                writeFile();
  QMap<int, Racer *>* getRacerMap();

private:

  QJsonDocument *jsonDoc;
  QFile *jsonFile;
  QByteArray rawFile;
  QJsonObject jsonObj;
  QMap<int, Racer *> *racerMap;
};

#endif // RUNTIMEMANAGER_H
