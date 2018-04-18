#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QtSql>
#include <QSqlDatabase>
#include <iostream>
#include <QSqlQuery>
#include <QSqlTableModel>

class DatabaseController : QSqlTableModel {
public:

  DatabaseController(const QString&,
                     QSqlDatabase);
  void       reset(const QString&);
  void       initTable(const QString&);
  QSqlRecord get(const QString&,
                 const QString&,
                 const QVariant&);
  QSqlRecord update(const QString&,
                    int,
                    QMap<QString, QVariant>);
  bool       insert(const QString&,
                    QMap<QString, QVariant>);
  QSqlRecord remove(const QString&,
                    const QString&,
                    const QVariant&);
  QSqlQuery  query(const QString&);

private:

  QSqlDatabase *database;
};

#endif // DATABASECONTROLLER_H
