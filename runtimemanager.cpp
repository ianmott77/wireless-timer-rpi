#include "runtimemanager.h"

RunTimeManager::RunTimeManager() {
  this->jsonFile = new QFile("/home/pi/.wt_times.json");

  if (!this->jsonFile->open(QIODevice::ReadOnly)) {
    std::cout << "Error initializing couldn't open fle" << std::endl <<
      std::flush;
    return;
  }
  this->rawFile  = this->jsonFile->readAll();
  this->jsonDoc  = new QJsonDocument(QJsonDocument::fromJson(this->rawFile));
  this->racerMap = new QMap<int, Racer *>();

  QJsonObject temp = this->jsonDoc->object();

  // create the racerMap
  for (int i = 0; i < temp.keys().length(); i++) {
    int bib           = temp.keys().at(i).toInt();
    QJsonObject times = temp[QString::number(bib)].toObject();
    int startTime     = times[QString("start-time")].toInt();
    int finishTime    = times[QString("finish-time")].toInt();
    Racer *r          = new Racer(bib, startTime, 0, 0);
    r->setFinishTime(finishTime);
    this->racerMap->insert(bib, r);
  }

  this->jsonFile->close();
}

RunTimeManager::~RunTimeManager() {
  delete this->jsonDoc;
  delete this->jsonFile;
}

QJsonDocument * RunTimeManager::getJsonDoc() {
  return this->jsonDoc;
}

QJsonObject RunTimeManager::toJson() {
  return this->jsonDoc->object();
}

QMap<int, Racer *> * RunTimeManager::getRacerMap() {
  return this->racerMap;
}

void RunTimeManager::addTime(Racer *racer) {
  // add the racer to the map
  this->racerMap->insert(racer->getBib(), racer);

  // get the old JSON
  QJsonObject fullJson = this->jsonDoc->object();

  // create a new JSON object to hold the time values
  QJsonObject newObject;

  newObject.insert(QString("start-time"),
                   QJsonValue((int)racer->getStartTime()));
  newObject.insert(QString("finish-time"),
                   QJsonValue((int)racer->getFinishTime()));

  // insert the new object with the bib as the key
  fullJson.insert(QString::number(racer->getBib()), newObject);

  // delete the old QJsonDocument
  delete this->jsonDoc;

  // create a a new QJsonDocument with the new JSON object in it
  this->jsonDoc = new QJsonDocument(fullJson);
}

bool RunTimeManager::writeFile() {
  if (!this->jsonFile->open(QIODevice::WriteOnly)) return false;

  this->jsonFile->write(this->jsonDoc->toJson());
  this->jsonFile->flush();
  this->jsonFile->close();
  return true;
}
