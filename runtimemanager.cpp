#include "runtimemanager.h"

RunTimeManager::RunTimeManager() {
  this->database =
    new DatabaseController("wtdb.db", QSqlDatabase::addDatabase("QSQLITE"));

  QSqlQuery runs = this->database->query("SELECT * FROM runs");

  while (runs.next()) {
    Racer *r = new Racer(runs.value("bib").toInt(),
                         runs.value("start_time").toInt(), 0, 0);
    r->setFinishTime(runs.value("finish_time").toInt());
    this->racers.append(r);
  }
}

RunTimeManager::~RunTimeManager() {
  delete this->database;
}

bool RunTimeManager::addRun(Racer *racer) {
  this->racers.append(racer);
  QMap<QString, QVariant> newRun;
  newRun.insert("start_time",  QVariant((uint)racer->getStartTime()));
  newRun.insert("finish_time", QVariant(racer->getFinishTime()));
  newRun.insert("bib",         QVariant(racer->getBib()));
  return this->database->insert("runs", newRun);
}

QJsonObject RunTimeManager::toJson(const QString& query) {
  QJsonObject result;
  QSqlQuery   allRuns = this->database->query(query);

  while (allRuns.next()) {
    QJsonArray  runs;
    QJsonObject run;
    run.insert("id",         allRuns.value("id").toJsonValue());
    run.insert("start_time", allRuns.value("start_time").toJsonValue());
    run.insert("finsh_time", allRuns.value("finish_time").toJsonValue());

    if (result.contains(allRuns.value("bib").toString())) {
      runs = result.value(allRuns.value("bib").toString()).toArray();
      result.remove(allRuns.value("bib").toString());
    }

    runs.append(QJsonValue(run));
    result.insert(allRuns.value("bib").toString(), runs);
  }
  return result;
}

QVector<Racer *>RunTimeManager::getRacers() {
  return racers;
}

bool RunTimeManager::clearAllRuns() {
  QSqlQuery result = this->database->query("DELETE FROM runs");

  return result.isValid();
}
