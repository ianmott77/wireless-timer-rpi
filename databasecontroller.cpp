#include "databasecontroller.h"

DatabaseController::DatabaseController(const QString& db,
                                       QSqlDatabase   database) :
  QSqlTableModel(
    NULL,
    database)
{
  this->database = new QSqlDatabase(database);
  this->database->setDatabaseName(db);

  if (!this->database->open()) {
    std::cout << "Failed to open database!" << std::endl << std::flush;
  }

  if (!this->database->tables().contains(QString("runs"))) {
    QSqlQuery result = this->database->exec(
      "CREATE TABLE `runs`"
      "(`id`	INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,`start_time`	INTEGER,`finish_time`	INTEGER,`bib`	INTEGER)");

    if (result.lastError().isValid()) {
      std::cout << result.lastError().text().toStdString().c_str() << std::endl <<
        std::flush;
    }
  }
  this->initTable("runs");
}

void DatabaseController::reset(const QString& table) {
  this->setTable(table);
  this->setEditStrategy(QSqlTableModel::OnFieldChange);
  this->setFilter(QString(""));
  this->select();
}

void DatabaseController::initTable(const QString& table) {
  this->setTable(table);
  this->setEditStrategy(QSqlTableModel::OnFieldChange);
}

QSqlRecord DatabaseController::get(const QString & table,
                                   const QString & key,
                                   const QVariant& val) {
  this->initTable(table);
  this->setFilter(QString("%1 = '%2'").arg(key).arg(val.toString()));
  this->select();
  return this->record(0);
}

QSqlRecord DatabaseController::update(const QString& table,
                                      int i,
                                      QMap<QString, QVariant>values) {
  this->initTable(table);

  QString query = QString("UPDATE %1 SET ").arg(table);

  QMapIterator<QString, QVariant> it(values);

  for (int j = 0; it.hasNext(); j++) {
    it.next();

    if (j == 0) query += QString("'%1' = '%2'").arg(it.key()).arg(
        it.value().toString());
    else query +=
        QString(", '%1' = '%2'").arg(it.key()).arg(it.value().toString());
  }
  query += QString(" WHERE id = '%1'").arg(i);
  return this->database->exec(query).record();
}

bool DatabaseController::insert(const QString& table,
                                QMap<QString, QVariant>values) {
  this->initTable(table);
  QSqlRecord record;
  QMapIterator<QString, QVariant> it(values);

  while (it.hasNext()) {
    it.next();
    QSqlField field(it.key());
    field.setValue(it.value());
    record.append(field);
  }

  if (this->insertRecord(-1, record)) {
    return true;
  }
  return false;
}

QSqlRecord DatabaseController::remove(const QString & table,
                                      const QString & key,
                                      const QVariant& val) {
  this->initTable(table);
  return this->database->exec(QString("DELETE FROM %1 WHERE %2 = '%3' ").arg(
                                table).arg(
                                key).arg(
                                val.toString())).record();
}

QSqlQuery DatabaseController::query(const QString& query) {
  return this->database->exec(query);
}
